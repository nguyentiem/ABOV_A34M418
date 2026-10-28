/**
 *******************************************************************************
 * @file        a34m41x_frt.c
 * @author      ABOV R&D Division
 * @brief       Free-run Timer
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
#include "a34m41x_frt.h"

#define FRT_REG_OFFSET  0x100

static __INLINE FRT_Type *FRT_REG(enum frt_id id)
{
    return (FRT_Type *)(FRT0_BASE + (FRT_REG_OFFSET * id));
}

static __INLINE IRQn_Type FRT_IRQ_NUM(enum frt_id id)
{
    return (IRQn_Type)((uint8_t)FRT0_IRQn + id);
}

struct frt_ctrl_block
{
    frt_irq_handler_t   handler;
    void *              context;
};

static struct frt_ctrl_block g_fcb[FRT_ID_MAX];

int32_t HAL_FRT_Init(enum frt_id id, struct frt_cfg *cfg,
                     frt_irq_handler_t handler,
                     void *context)
{
    struct frt_ctrl_block *fcb;
    FRT_Type *frt;
    uint32_t reg_val;

    DRIVER_ASSERT(id < FRT_ID_MAX);
    DRIVER_ASSERT(cfg != NULL);

    fcb = &g_fcb[id];
    fcb->handler = handler;
    fcb->context = context;

    SCU->PER1 &= ~(1 << (SCU_PER1_FRT0_Pos + id));
    SCU->PCER1 &= ~(1 << (SCU_PCER1_FRT0_Pos + id));

    SCU->PER1 |= (1 << (SCU_PER1_FRT0_Pos + id));
    SCU->PCER1 |= (1 << (SCU_PCER1_FRT0_Pos + id));

    frt = FRT_REG(id);

    if (cfg->mode == FRT_MODE_FREE_RUN)
    {
        reg_val = frt->CTRL;
        reg_val &= ~(1 << FRT_CTRL_MODE_Pos);
        reg_val |= (0 << FRT_CTRL_MODE_Pos);
        frt->CTRL = reg_val;
        
        /* frt runs in overflow interrupt mode */
        if (cfg->run_type == FRT_RUN_INTERRUPT) {
            frt->CTRL |= (1 << FRT_CTRL_OVFIE_Pos);
        } else {
            frt->CTRL &= ~(1 << FRT_CTRL_OVFIE_Pos);
        }
        
    }
    else if (cfg->mode == FRT_MODE_MATCH)
    {
        reg_val = frt->CTRL;
        reg_val &= ~(1 << FRT_CTRL_MODE_Pos);
        reg_val |= (1 << FRT_CTRL_MODE_Pos);
        frt->CTRL = reg_val;
        
        /* frt runs in match interrupt mode */
        if (cfg->run_type == FRT_RUN_INTERRUPT) {
            frt->CTRL |= (1 << FRT_CTRL_MATCHIE_Pos);
        } else {
            frt->CTRL &= ~(1 << FRT_CTRL_MATCHIE_Pos);
        }        
    }
    
    if (cfg->run_type == FRT_RUN_INTERRUPT) {
        NVIC_ClearPendingIRQ(FRT_IRQ_NUM(id));
        NVIC_SetPriority(FRT_IRQ_NUM(id), cfg->irq_prio);
        NVIC_EnableIRQ(FRT_IRQ_NUM(id));
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_FRT_Deinit(enum frt_id id)
{
    DRIVER_ASSERT(id < FRT_ID_MAX);

    SCU->PER1 &= ~(1 << (SCU_PER1_FRT0_Pos + id));
    SCU->PCER1 &= ~(1 << (SCU_PCER1_FRT0_Pos + id));

    NVIC_DisableIRQ(FRT_IRQ_NUM(id));

    return DRIVER_ERROR_OK;
}

int32_t HAL_FRT_SetClock(enum frt_id id, enum frt_clk cl_src, uint8_t clk_div)
{
    DRIVER_ASSERT(id < FRT_ID_MAX);
    DRIVER_ASSERT(clk_div < 0xFF);

    if (id == FRT_ID_0)
    {
        SCU->MCCR6 &= ~(0x07 << 8);
        SCU->MCCR6 &= ~(0xFF << 0);
        SCU->MCCR6 |= (((cl_src & 0x07) << 8) | ((clk_div & 0xFF) << 0));
    }
    else if (id == FRT_ID_1)
    {
        SCU->MCCR6 &= ~(0x07 << 24);
        SCU->MCCR6 &= ~(0xFF << 16);
        SCU->MCCR6 |= (((cl_src & 0x07) << 24) | ((clk_div & 0xFF) << 16));
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_FRT_Start(enum frt_id id)
{
    FRT_Type *frt;

    DRIVER_ASSERT(id < FRT_ID_MAX);

    frt = FRT_REG(id);
    frt->CTRL |= (1 << FRT_CTRL_EN_Pos);

    return DRIVER_ERROR_OK;
}

int32_t HAL_FRT_Stop(enum frt_id id)
{
    FRT_Type *frt;

    DRIVER_ASSERT(id < FRT_ID_MAX);

    frt = FRT_REG(id);
    frt->CTRL &= ~(1 << FRT_CTRL_EN_Pos);

    return DRIVER_ERROR_OK;
}

int32_t HAL_FRT_GetCounter(enum frt_id id, uint32_t *counter)
{
    FRT_Type *frt;

    DRIVER_ASSERT(id < FRT_ID_MAX);

    frt = FRT_REG(id);
    *counter = frt->CNT;

    return DRIVER_ERROR_OK;
}

int32_t HAL_FRT_ClearCounter(enum frt_id id)
{
    FRT_Type *frt;

    DRIVER_ASSERT(id < FRT_ID_MAX);

    frt = FRT_REG(id);
    frt->CNT = 0;

    return DRIVER_ERROR_OK;
}

int32_t HAL_FRT_SetMatchCounter(enum frt_id id, uint32_t counter)
{
    FRT_Type *frt;

    DRIVER_ASSERT(id < FRT_ID_MAX);

    frt = FRT_REG(id);
    frt->MCNT = counter;

    return DRIVER_ERROR_OK;
}

int32_t HAL_FRT_SetIRQ(enum frt_id id, enum frt_irq_type type, uint8_t enable)
{
    FRT_Type *frt;

    DRIVER_ASSERT(id < FRT_ID_MAX);

    frt = FRT_REG(id);

    if (type == FRT_IRQ_MATCH)
    {
        if (enable)
        {
            frt->CTRL |= (1 << FRT_CTRL_MATCHIE_Pos);
        }
        else
        {
            frt->CTRL &= ~(1 << FRT_CTRL_MATCHIE_Pos);
        }
    }
    else if (type == FRT_IRQ_OVERFLOW)
    {
        if (enable)
        {
            frt->CTRL |= (1 << FRT_CTRL_OVFIE_Pos);
        }
        else
        {
            frt->CTRL &= ~(1 << FRT_CTRL_OVFIE_Pos);
        }
    }

    return DRIVER_ERROR_OK;
}

void FRT_IRQHandler(enum frt_id id)
{
    struct frt_ctrl_block *fcb;
    FRT_Type *frt;
    uint32_t status;

    fcb = &g_fcb[id];
    frt = FRT_REG(id);
    status = frt->STAT;

    if (status & FRT_STAT_OVFIF_Msk)
    {
        if (fcb->handler)
        {
            fcb->handler(FRT_EVENT_OVERFLOW, fcb->context);
        }
    }

    if (status & FRT_STAT_MATCHIF_Msk)
    {
        if (fcb->handler)
        {
            fcb->handler(FRT_EVENT_MATCH, fcb->context);
        }
    }

    frt->STAT = status;
}

void FRT0_IRQHandler(void)
{
    FRT_IRQHandler(FRT_ID_0);
}

void FRT1_IRQHandler(void)
{
    FRT_IRQHandler(FRT_ID_1);
}

