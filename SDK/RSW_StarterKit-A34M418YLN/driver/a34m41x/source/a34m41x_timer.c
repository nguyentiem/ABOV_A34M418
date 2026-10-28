/**
 *******************************************************************************
 * @file        a34m41x_timer.c
 * @author      ABOV R&D Division
 * @brief       Timer
 *
 * Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/

#include <string.h>
#include "a34m41x.h"
#include "a34m41x_scu.h"
#include "a34m41x_timer.h"

#define TIMER_REG_OFFSET    0x40

static __INLINE TIMER_Type *TIMER_REG(enum timer_id id)
{
    return (TIMER_Type *)(TIMER0_BASE + (TIMER_REG_OFFSET * id));
}

static __INLINE IRQn_Type TIMER_IRQ_NUM(enum timer_id id)
{
    return (IRQn_Type)((uint8_t)TIMER0_IRQn + id);
}

struct timer_ctrl_block
{
    uint8_t             mode;
    uint8_t             pwm_event;
    timer_irq_handler_t handler;
    void *              context;
};

static struct timer_ctrl_block g_tcb[TIMER_ID_MAX];

static int32_t TIMER_Configure(struct timer_ctrl_block *tcb,
                               TIMER_Type *timer,
                               struct timer_cfg *cfg)
{
    uint32_t cfg_val = 0;

    switch (cfg->mode)
    {
        case TIMER_MODE_PWM:
        case TIMER_MODE_ONESHOT:
            cfg_val |= (1 << TIMER_CR1_IOSEL_Pos);
        /* fall through */
        case TIMER_MODE_PERIODIC:
        case TIMER_MODE_CAPTURE:
            cfg_val |= (cfg->mode << TIMER_CR1_MODE_Pos);
            tcb->mode = cfg->mode;
            break;
        default:
            return DRIVER_ERROR_PARAMETER;
    }

    switch (cfg->clk_sel)
    {
        case TIMER_CLK_DIV_2_PCLK:
        case TIMER_CLK_DIV_4_PCLK:
        case TIMER_CLK_DIV_16_PCLK:
        case TIMER_CLK_DIV_64_PCLK:
        case TIMER_CLK_MCCLK:
        case TIMER_CLK_TNIO:
            cfg_val |= (cfg->clk_sel << TIMER_CR1_CKSEL_Pos);
            break;
        default:
            return DRIVER_ERROR_PARAMETER;
    }

    if (cfg->prescaler > 1024)
    {
        return DRIVER_ERROR_PARAMETER;
    }

    cfg_val |= (cfg->adc_trig << TIMER_CR1_ADCTRGEN_Pos);

    timer->CR1 = cfg_val;
    timer->PRS = (cfg->prescaler - 1) & TIMER_PRS_PRS_Msk;

    return DRIVER_ERROR_OK;
}

int32_t HAL_TIMER_Init(enum timer_id id, struct timer_cfg *cfg,
                       timer_irq_handler_t handler,
                       void *context)
{
    struct timer_ctrl_block *tcb;
    TIMER_Type *timer;
    int32_t ret;

    DRIVER_ASSERT(id < TIMER_ID_MAX);
    DRIVER_ASSERT(cfg != NULL);

    tcb = &g_tcb[id];
    timer = TIMER_REG(id);

    memset(tcb, 0, sizeof(struct timer_ctrl_block));

    tcb->handler = handler;
    tcb->context = context;

    SCU->PER1 &= ~(1 << (SCU_PER1_TIMER0_Pos + id));
    SCU->PCER1 &= ~(1 << (SCU_PCER1_TIMER0_Pos + id));

    SCU->PER1 |= (1 << (SCU_PER1_TIMER0_Pos + id));
    SCU->PCER1 |= (1 << (SCU_PCER1_TIMER0_Pos + id));

    ret = TIMER_Configure(tcb, timer, cfg);
    if (ret != DRIVER_ERROR_OK)
    {
        HAL_TIMER_Deinit(id);
        return ret;
    }

    NVIC_ClearPendingIRQ(TIMER_IRQ_NUM(id));
    NVIC_SetPriority(TIMER_IRQ_NUM(id), cfg->irq_prio);
    NVIC_EnableIRQ(TIMER_IRQ_NUM(id));

    return ret;
}

int32_t HAL_TIMER_Deinit(enum timer_id id)
{
    DRIVER_ASSERT(id < TIMER_ID_MAX);

    SCU->PER1 &= ~(1 << (SCU_PER1_TIMER0_Pos + id));
    SCU->PCER1 &= ~(1 << (SCU_PCER1_TIMER0_Pos + id));

    NVIC_DisableIRQ(TIMER_IRQ_NUM(id));

    return DRIVER_ERROR_OK;
}

int32_t HAL_TIMER_GetStatus(enum timer_id id, uint8_t *status)
{
    TIMER_Type *timer;

    DRIVER_ASSERT(id < TIMER_ID_MAX);

    timer = TIMER_REG(id);

    *status = timer->SR;

    return DRIVER_ERROR_OK;
}

int32_t HAL_TIMER_Start(enum timer_id id, uint16_t count)
{
    struct timer_ctrl_block *tcb;
    TIMER_Type *timer;

    DRIVER_ASSERT(id < TIMER_ID_MAX);

    tcb = &g_tcb[id];

    DRIVER_ASSERT(tcb->mode == TIMER_MODE_PERIODIC);

    timer = TIMER_REG(id);

    timer->GRA = count;                          // duty
    timer->GRB = count;                          // period
    timer->IER = (1 << TIMER_IER_MBIE_Pos);    
    timer->CR1 |= (1 << TIMER_CR1_IOSEL_Pos);    // TxIO direction is changed to output for oneshot and periodic mode.    
    timer->CR2 = (1 << TIMER_CR2_TCLR_Pos);
    timer->CR2 = (1 << TIMER_CR2_TEN_Pos);
    timer->CNT = 0;

    return DRIVER_ERROR_OK;
}

int32_t HAL_TIMER_StartPWM(enum timer_id id, enum timer_pwm_start_level level,
                           uint16_t duty, uint16_t period,
                           uint8_t event)
{
    struct timer_ctrl_block *tcb;
    TIMER_Type *timer;

    DRIVER_ASSERT(id < TIMER_ID_MAX);

    tcb = &g_tcb[id];

    DRIVER_ASSERT(tcb->mode == TIMER_MODE_PWM || tcb->mode == TIMER_MODE_ONESHOT);

    timer = TIMER_REG(id);

    if (event & TIMER_PWM_EVENT_DUTY)
    {
        timer->IER |= (1 << TIMER_IER_MAIE_Pos);
    }

    if (event & TIMER_PWM_EVENT_PERIOD)
    {
        timer->IER |= (1 << TIMER_IER_MBIE_Pos);
    }

    tcb->pwm_event = event;

    timer->GRA = duty;
    timer->GRB = period;
    if (level == TIMER_PWM_START_HIGH)
    {
        timer->CR1 |= (1 << TIMER_CR1_STARTLVL_Pos);
    }
    else
    {
        timer->CR1 &= ~(1 << TIMER_CR1_STARTLVL_Pos);
    }
    timer->CR2 = (1 << TIMER_CR2_TCLR_Pos);
    timer->CR2 = (1 << TIMER_CR2_TEN_Pos);

    return DRIVER_ERROR_OK;
}

int32_t HAL_TIMER_StartCapture(enum timer_id id, enum timer_capture_clear clear)
{
    struct timer_ctrl_block *tcb;
    TIMER_Type *timer;

    DRIVER_ASSERT(id < TIMER_ID_MAX);

    tcb = &g_tcb[id];

    DRIVER_ASSERT(tcb->mode == TIMER_MODE_CAPTURE);

    timer = TIMER_REG(id);

    timer->CR1 |= clear << TIMER_CR1_CLRMOD_Pos;
    timer->IER |= (1 << TIMER_IER_MBIE_Pos) | (1 << TIMER_IER_MAIE_Pos);
    timer->CR2 = (1 << TIMER_CR2_TCLR_Pos);
    timer->CR2 = (1 << TIMER_CR2_TEN_Pos);

    return DRIVER_ERROR_OK;
}

int32_t HAL_TIMER_EnableSynch(enum timer_id id,
                              enum timer_id master,
                              enum timer_sync_start start,
                              enum timer_sync_clear clear,
                              uint16_t delay)
{
    TIMER_Type *timer;
    TIMER_Type *timer_master;

    if (id <= TIMER_ID_4)
    {
        DRIVER_ASSERT(master <= TIMER_ID_4);
    }
    else
    {
        DRIVER_ASSERT(master > TIMER_ID_4);
    }

    /*
     * TODO: delayed sync does not work yet.
     */

    timer_master = TIMER_REG(master);
    timer_master->SYNC = (1 << ((uint8_t )id + TIMER_SYNC_T0SYNCB_Pos)) | \
                         (start << TIMER_SYNC_SSYNC_Pos) |                \
                         (clear << TIMER_SYNC_CSYNC_Pos) |                \
                         (delay << TIMER_SYNC_SYNCDLY_Pos);

    timer = TIMER_REG(id);
    timer->SYNC = (start << TIMER_SYNC_SSYNC_Pos) | \
                  (clear << TIMER_SYNC_CSYNC_Pos);

    return DRIVER_ERROR_OK;
}

int32_t HAL_TIMER_DisableSynch(enum timer_id id, enum timer_id master)
{
    TIMER_Type *timer;
    TIMER_Type *timer_master;

    if (id <= TIMER_ID_4)
    {
        DRIVER_ASSERT(master <= TIMER_ID_4);
    }
    else
    {
        DRIVER_ASSERT(master > TIMER_ID_4);
    }

    timer_master = TIMER_REG(master);
    timer_master->SYNC = 0;
    timer = TIMER_REG(id);
    timer->SYNC = 0;

    return DRIVER_ERROR_OK;
}

int32_t HAL_TIMER_Stop(enum timer_id id)
{
    TIMER_Type *timer;

    DRIVER_ASSERT(id < TIMER_ID_MAX);

    timer = TIMER_REG(id);

    timer->CR2 = 0;
    timer->IER = 0;
    timer->CNT = 0;

    return DRIVER_ERROR_OK;
}

int32_t HAL_TIMER_GetCount(enum timer_id id, uint16_t *count)
{
    TIMER_Type *timer;

    DRIVER_ASSERT(id < TIMER_ID_MAX);

    timer = TIMER_REG(id);

    *count = timer->CNT;

    return DRIVER_ERROR_OK;
}

int32_t HAL_TIMER_ClearCount(enum timer_id id)
{
    TIMER_Type *timer;

    DRIVER_ASSERT(id < TIMER_ID_MAX);

    timer = TIMER_REG(id);

    timer->CNT = 0;

    return DRIVER_ERROR_OK;
}

int32_t HAL_TIMER_GetCaptureCount(enum timer_id id, uint16_t *count, enum timer_capture_reg reg)
{
    TIMER_Type *timer;

    DRIVER_ASSERT(id < TIMER_ID_MAX);

    timer = TIMER_REG(id);

    if (reg == TIMER_CAPTURE_REG_GRA)
    {
        *count = timer->GRA;
    }
    else
    {
        *count = timer->GRB;
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_TIMER_SetClock(enum timer_id id, enum timer_clk_sel clk_sel, uint16_t prescaler,
                           enum timer_mcclk_sel mcclk_sel, uint8_t mcclk_div)
{
    TIMER_Type *timer;

    DRIVER_ASSERT(id < TIMER_ID_MAX);

    timer = TIMER_REG(id);

    if (prescaler > 1024)
    {
        return DRIVER_ERROR_PARAMETER;
    }

    if (clk_sel == TIMER_CLK_MCCLK)
    {
        if (id < 5)
        {
            SCU->MCCR3 &= ~(SCU_MCCR3_TIMER04CSEL_Msk | SCU_MCCR3_TIMER04CDIV_Msk);
            SCU->MCCR3 |= (mcclk_sel << SCU_MCCR3_TIMER04CSEL_Pos | mcclk_div << SCU_MCCR3_TIMER04CDIV_Pos);
        }
        else
        {
            SCU->MCCR3 &= ~(SCU_MCCR3_TIMER59CSEL_Msk | SCU_MCCR3_TIMER59CDIV_Msk);
            SCU->MCCR3 |= (mcclk_sel << SCU_MCCR3_TIMER59CSEL_Pos | mcclk_div << SCU_MCCR3_TIMER59CDIV_Pos);
        }
    }

    timer->CR1 &= ~TIMER_CR1_CKSEL_Msk;
    timer->CR1 |= clk_sel << TIMER_CR1_CKSEL_Pos;
    timer->PRS = (prescaler - 1) & TIMER_PRS_PRS_Msk;

    return DRIVER_ERROR_OK;
}

int32_t HAL_TIMER_SetIRQ(enum timer_id id, enum timer_irq_type type,
                         uint8_t enable)
{
    TIMER_Type *timer;

    DRIVER_ASSERT(id < TIMER_ID_MAX);

    timer = TIMER_REG(id);

    switch (type)
    {
        case TIMER_IRQ_OVERFLOW:
            if (enable)
            {
                timer->IER |= 1 << TIMER_IER_OVIE_Pos;
            }
            else
            {
                timer->IER &= ~(1 << TIMER_IER_OVIE_Pos);
            }
            break;
        case TIMER_IRQ_PWM_DUTY:
        case TIMER_IRQ_CAPTURE_GRA:
            if (enable)
            {
                timer->IER |= 1 << TIMER_IER_MAIE_Pos;
            }
            else
            {
                timer->IER &= ~(1 << TIMER_IER_MAIE_Pos);
            }
            break;
        case TIMER_IRQ_COUNT_MATCH:
        case TIMER_IRQ_PWM_PERIOD:
        case TIMER_IRQ_CAPTURE_GRB:
            if (enable)
            {
                timer->IER |= 1 << TIMER_IER_MBIE_Pos;
            }
            else
            {
                timer->IER &= ~(1 << TIMER_IER_MBIE_Pos);
            }
            break;
        default:
            return DRIVER_ERROR_PARAMETER;
    }

    return DRIVER_ERROR_OK;
}

static void TIMER_IRQHandler(enum timer_id id)
{
    struct timer_ctrl_block *tcb;
    TIMER_Type *timer;
    volatile uint32_t event = 0;
    volatile uint8_t status;

    tcb = &g_tcb[id];
    timer = TIMER_REG(id);
    status = timer->SR;
    timer->SR = status;		

    if (tcb->handler)
    {
        if (status & TIMER_SR_MFA_Msk)
        {
            if (tcb->mode == TIMER_MODE_PWM && (tcb->pwm_event & TIMER_PWM_EVENT_DUTY))
            {
                event |= TIMER_EVENT_PWM_DUTY;
            }
            else if (tcb->mode == TIMER_MODE_CAPTURE)
            {
                event |= TIMER_EVENT_CAPTURE_GRA;
            }
        }

        if (status & TIMER_SR_MFB_Msk)
        {
            if (tcb->mode == TIMER_MODE_PERIODIC || tcb->mode == TIMER_MODE_ONESHOT)
            {
                event |= TIMER_EVENT_COUNT_MATCH;
            }
            else if (tcb->mode == TIMER_MODE_PWM && (tcb->pwm_event & TIMER_PWM_EVENT_PERIOD))
            {
                event |= TIMER_EVNET_PWM_PERIOD;
            }
            else if (tcb->mode == TIMER_MODE_CAPTURE)
            {
                event |= TIMER_EVENT_CAPTURE_GRB;
            }
        }

        if (status & TIMER_SR_OVF_Msk)
        {
            event |= TIMER_EVNET_OVERFLOW;
        }

        if (event)
        {
            tcb->handler(event, tcb->context);
        }
    }
}

void TIMER0_IRQHandler(void)
{
    TIMER_IRQHandler(TIMER_ID_0);
}

void TIMER1_IRQHandler(void)
{
    TIMER_IRQHandler(TIMER_ID_1);
}

void TIMER2_IRQHandler(void)
{
    TIMER_IRQHandler(TIMER_ID_2);
}

void TIMER3_IRQHandler(void)
{
    TIMER_IRQHandler(TIMER_ID_3);
}

void TIMER4_IRQHandler(void)
{
    TIMER_IRQHandler(TIMER_ID_4);
}

void TIMER5_IRQHandler(void)
{
    TIMER_IRQHandler(TIMER_ID_5);
}

void TIMER6_IRQHandler(void)
{
    TIMER_IRQHandler(TIMER_ID_6);
}

void TIMER7_IRQHandler(void)
{
    TIMER_IRQHandler(TIMER_ID_7);
}

void TIMER8_IRQHandler(void)
{
    TIMER_IRQHandler(TIMER_ID_8);
}

void TIMER9_IRQHandler(void)
{
    TIMER_IRQHandler(TIMER_ID_9);
}
