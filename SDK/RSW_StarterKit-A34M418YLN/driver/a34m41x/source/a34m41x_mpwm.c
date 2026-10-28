/**
 *******************************************************************************
 * @file        a34m41x_mpwm.c
 * @author      ABOV R&D Division
 * @brief       Motor Pulse Width Modulation
 *
 * Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "a34m41x.h"
#include "a34m41x_scu.h"
#include "a34m41x_mpwm.h"

#define MPWM_REG_BIT_SET(r, b, pos)        ((b == 1) ? (r |= (1UL << pos)) : (r &= ~(1UL << pos)))
#define MPWM_REG_BIT_SET2(r, b, b2, pos)   (((b == 1) || (b2 == 1)) ? (r |= (1UL << pos)) : (r &= ~(1UL << pos)))
#define MPWM_REG_VAL_SET(r, d, pos, msk)   r &= ~msk; r |= ((d << pos) & msk)

#define MPWM_IER_INDIVIDUAL_U_Msk       MPWM_IER_PRDUIE_Msk | MPWM_IER_BOTUIE_Msk | \
    MPWM_IER_ULIE_ATR1IE_Msk | MPWM_IER_UHIE_ATR4IE_Msk
#define MPWM_IER_NONINDIVIDUAL_U_Msk    MPWM_IER_PRDUIE_Msk | MPWM_IER_BOTUIE_Msk | \
    MPWM_IER_ULIE_ATR1IE_Msk | MPWM_IER_UHIE_ATR4IE_Msk
#define MPWM_IER_U_Msk(mode)            (mode == MPWM_MODE_INDIVIDUAL ? \
                                         MPWM_IER_INDIVIDUAL_U_Msk : MPWM_IER_NONINDIVIDUAL_U_Msk)

#define MPWM_IER_INDIVIDUAL_V_Msk       MPWM_IER_PRDVIE_Pos | MPWM_IER_BOTVIE_Pos | \
    MPWM_IER_ULIE_ATR1IE_Msk | MPWM_IER_UHIE_ATR4IE_Msk
#define MPWM_IER_NONINDIVIDUAL_V_Msk    MPWM_IER_PRDUIE_Msk | MPWM_IER_BOTUIE_Msk | \
    MPWM_IER_VLIE_ATR2IE_Msk | MPWM_IER_VHIE_ATR5IE_Msk
#define MPWM_IER_V_Msk(mode)            (mode == MPWM_MODE_INDIVIDUAL ? \
                                         MPWM_IER_INDIVIDUAL_V_Msk : MPWM_IER_NONINDIVIDUAL_V_Msk)

#define MPWM_IER_INDIVIDUAL_W_Msk       MPWM_IER_PRDWIE_Pos | MPWM_IER_BOTWIE_Pos | \
    MPWM_IER_WLIE_ATR3IE_Msk | MPWM_IER_WHIE_ATR6IE_Msk
#define MPWM_IER_NONINDIVIDUAL_W_Msk    MPWM_IER_PRDUIE_Msk | MPWM_IER_BOTUIE_Msk | \
    MPWM_IER_WLIE_ATR3IE_Msk | MPWM_IER_WHIE_ATR6IE_Msk
#define MPWM_IER_W_Msk(mode)            (mode == MPWM_MODE_INDIVIDUAL ? \
                                         MPWM_IER_INDIVIDUAL_W_Msk : MPWM_IER_NONINDIVIDUAL_V_Msk)

#define MPWM_REG_OFFSET         0x1000UL

struct mpwm_ctrl_block
{
    uint8_t             mode;
    mpwm_irq_handler_t  handler;
    void *              context;
};

static struct mpwm_ctrl_block g_mcb[MPWM_ID_MAX];

static __INLINE MPWM_Type *MPWM_REG(enum mpwm_id id)
{
    return (MPWM_Type *)(MPWM0_BASE + (MPWM_REG_OFFSET * id));
}

static __INLINE IRQn_Type MPWM_PROT_IRQ_NUM(enum mpwm_id id)
{
    return (IRQn_Type)(MPWM0PROT_IRQn + (id * 5));
}

static __INLINE IRQn_Type MPWM_OVV_IRQ_NUM(enum mpwm_id id)
{
    return (IRQn_Type)(MPWM0OVV_IRQn + (id * 5));
}

static __INLINE IRQn_Type MPWM_U_IRQ_NUM(enum mpwm_id id)
{
    return (IRQn_Type)(MPWM0U_IRQn + (id * 5));
}

static __INLINE IRQn_Type MPWM_V_IRQ_NUM(enum mpwm_id id)
{
    return (IRQn_Type)(MPWM0V_IRQn + (id * 5));
}

static __INLINE IRQn_Type MPWM_W_IRQ_NUM(enum mpwm_id id)
{
    return (IRQn_Type)(MPWM0W_IRQn + (id * 5));
}

static __INLINE void MPWM_IRQ_EN(uint32_t en, IRQn_Type IRQn)
{
    if (en)
    {
        NVIC_EnableIRQ(IRQn);
    }
    else
    {
        NVIC_DisableIRQ(IRQn);
    }
}

static int MPWM_Configure(struct mpwm_ctrl_block *mcb,
                          MPWM_Type *mpwm,
                          struct mpwm_cfg *cfg)
{
    mpwm->MR = 0;

    mpwm->MR |= (1 << MPWM_MR_UAO_Pos);
    mpwm->MR |= (cfg->mode << MPWM_MR_MOTORB_Pos);
    mcb->mode = cfg->mode;

    if (cfg->mode != MPWM_MODE_NORMAL)
    {
        mpwm->MR |= (cfg->chan_mode << MPWM_MR_MCHMOD_Pos);
    }

    mpwm->MR |= (cfg->counter_mode << MPWM_MR_UPDOWN_Pos);

    return DRIVER_ERROR_OK;
}

int32_t HAL_MPWM_Init(enum mpwm_id id, struct mpwm_cfg *cfg,
                      mpwm_irq_handler_t handler,
                      void *context)
{
    struct mpwm_ctrl_block *mcb;
    MPWM_Type *mpwm;
    int32_t ret;

    DRIVER_ASSERT(id < MPWM_ID_MAX);
    DRIVER_ASSERT(cfg != NULL);

    mcb = &g_mcb[id];
    mpwm = MPWM_REG(id);

    memset(mcb, 0, sizeof(struct mpwm_ctrl_block));

    SCU->PER2 |= (1 << (SCU_PER2_MPWM0_Pos + id));
    SCU->PCER2 |= (1 << (SCU_PCER2_MPWM0_Pos + id));


    if (id == MPWM_ID_0)
    {
        SCU->MCCR2 &= ~(SCU_MCCR2_MPWM0CSEL_Msk | SCU_MCCR2_MPWM0CDIV_Msk);
        SCU->MCCR2 |= (cfg->clk_sel << SCU_MCCR2_MPWM0CSEL_Pos |
                       cfg->clk_div << SCU_MCCR2_MPWM0CDIV_Pos);
    }
    else
    {
        SCU->MCCR2 &= ~(SCU_MCCR2_MPWM1CSEL_Msk | SCU_MCCR2_MPWM1CDIV_Msk);
        SCU->MCCR2 |= (cfg->clk_sel << SCU_MCCR2_MPWM1CSEL_Pos |
                       cfg->clk_div << SCU_MCCR2_MPWM1CDIV_Pos);
    }

    mcb->handler = handler;
    mcb->context = context;

    MPWM_REG_BIT_SET(mpwm->CR1, 1, MPWM_CR1_PWMEN_Pos);

    ret = MPWM_Configure(mcb, mpwm, cfg);
    if (ret != DRIVER_ERROR_OK)
    {
        HAL_MPWM_Deinit(id);
        return ret;
    }

    return ret;
}

int32_t HAL_MPWM_Deinit(enum mpwm_id id)
{
    DRIVER_ASSERT(id < MPWM_ID_MAX);

    SCU->PER2 &= ~(1 << (SCU_PER2_MPWM0_Pos + id));
    SCU->PCER2 &= ~(1 << (SCU_PCER2_MPWM0_Pos + id));

    NVIC_DisableIRQ(MPWM_PROT_IRQ_NUM(id));
    NVIC_DisableIRQ(MPWM_OVV_IRQ_NUM(id));
    NVIC_DisableIRQ(MPWM_U_IRQ_NUM(id));
    NVIC_DisableIRQ(MPWM_V_IRQ_NUM(id));
    NVIC_DisableIRQ(MPWM_W_IRQ_NUM(id));

    return DRIVER_ERROR_OK;
}

int32_t HAL_MPWM_SetIRQConfig(enum mpwm_id id, union mpwm_irq_cfg *cfg)
{
    struct mpwm_ctrl_block *mcb;
    MPWM_Type *mpwm;

    DRIVER_ASSERT(id < MPWM_ID_MAX);

    mpwm = MPWM_REG(id);
    mcb = &g_mcb[id];

    if (mcb->mode == MPWM_MODE_INDIVIDUAL)
    {
        MPWM_REG_BIT_SET(mpwm->IER, cfg->irq_en_bit.u_period, MPWM_IER_PRDUIE_Pos);
        MPWM_REG_BIT_SET(mpwm->IER, cfg->irq_en_bit.v_period, MPWM_IER_PRDVIE_Pos);
        MPWM_REG_BIT_SET(mpwm->IER, cfg->irq_en_bit.w_period, MPWM_IER_PRDWIE_Pos);
        MPWM_REG_BIT_SET(mpwm->IER, cfg->irq_en_bit.u_bottom, MPWM_IER_BOTUIE_Pos);
        MPWM_REG_BIT_SET(mpwm->IER, cfg->irq_en_bit.v_bottom, MPWM_IER_BOTVIE_Pos);
        MPWM_REG_BIT_SET(mpwm->IER, cfg->irq_en_bit.w_bottom, MPWM_IER_BOTWIE_Pos);
    }
    else
    {
        MPWM_REG_BIT_SET(mpwm->IER, cfg->irq_en_bit.period, MPWM_IER_PRDUIE_Pos);
        MPWM_REG_BIT_SET(mpwm->IER, cfg->irq_en_bit.bottom, MPWM_IER_BOTUIE_Pos);
    }

    MPWM_REG_BIT_SET2(mpwm->IER, cfg->irq_en_bit.atr_1, cfg->irq_en_bit.ul_duty, MPWM_IER_ULIE_ATR1IE_Pos);
    MPWM_REG_BIT_SET2(mpwm->IER, cfg->irq_en_bit.atr_2, cfg->irq_en_bit.vl_duty, MPWM_IER_VLIE_ATR2IE_Pos);
    MPWM_REG_BIT_SET2(mpwm->IER, cfg->irq_en_bit.atr_3, cfg->irq_en_bit.wl_duty, MPWM_IER_WLIE_ATR3IE_Pos);
    MPWM_REG_BIT_SET2(mpwm->IER, cfg->irq_en_bit.atr_4, cfg->irq_en_bit.uh_duty, MPWM_IER_UHIE_ATR4IE_Pos);
    MPWM_REG_BIT_SET2(mpwm->IER, cfg->irq_en_bit.atr_5, cfg->irq_en_bit.uh_duty, MPWM_IER_VHIE_ATR5IE_Pos);
    MPWM_REG_BIT_SET2(mpwm->IER, cfg->irq_en_bit.atr_6, cfg->irq_en_bit.uh_duty, MPWM_IER_WHIE_ATR6IE_Pos);

    MPWM_REG_BIT_SET(mpwm->PCR, cfg->irq_en_bit.protect, MPWM_PCR_PROTEN_Pos);
    MPWM_REG_BIT_SET(mpwm->OCR, cfg->irq_en_bit.overvoltage, MPWM_OCR_OVINEN_Pos);

    MPWM_IRQ_EN((mpwm->IER & MPWM_IER_U_Msk(mcb->mode)), MPWM_U_IRQ_NUM(id));
    MPWM_IRQ_EN((mpwm->IER & MPWM_IER_V_Msk(mcb->mode)), MPWM_V_IRQ_NUM(id));
    MPWM_IRQ_EN((mpwm->IER & MPWM_IER_W_Msk(mcb->mode)), MPWM_W_IRQ_NUM(id));
    MPWM_IRQ_EN(cfg->irq_en_bit.protect, MPWM_PROT_IRQ_NUM(id));
    MPWM_IRQ_EN(cfg->irq_en_bit.overvoltage, MPWM_OVV_IRQ_NUM(id));


    return DRIVER_ERROR_OK;
}

int32_t HAL_MPWM_SetPotectionInputConfig(enum mpwm_id id, enum mpwm_phase_type p_type,
                                         struct mpwm_input_prot_cfg *cfg)
{
    struct mpwm_ctrl_block *mcb;
    MPWM_Type *mpwm;

    DRIVER_ASSERT(id < MPWM_ID_MAX);

    mpwm = MPWM_REG(id);
    mcb = &g_mcb[id];

    if ((mcb->mode == MPWM_MODE_INDIVIDUAL &&
         (p_type == MPWM_PHASE_TYPE_U || p_type == MPWM_PHASE_TYPE_ALL)) ||
        (mcb->mode != MPWM_MODE_INDIVIDUAL))
    {
        MPWM_REG_BIT_SET(mpwm->PCR, cfg->enable, MPWM_PCR_PROTEN_Pos);
        MPWM_REG_BIT_SET(mpwm->PCR, cfg->polarity, MPWM_PCR_PROTPOL_Pos);
        MPWM_REG_VAL_SET(mpwm->PCR, cfg->debounce, MPWM_PCR_PROTD_Pos, MPWM_PCR_PROTD_Msk);
    }

    if (mcb->mode == MPWM_MODE_INDIVIDUAL &&
        (p_type == MPWM_PHASE_TYPE_V || p_type == MPWM_PHASE_TYPE_ALL))
    {
        MPWM_REG_BIT_SET(mpwm->PCR, cfg->enable, MPWM_PCR_VPROTEN_Pos);
        MPWM_REG_BIT_SET(mpwm->PCR, cfg->polarity, MPWM_PCR_VPROTPOL_Pos);
        MPWM_REG_VAL_SET(mpwm->PCR, cfg->debounce, MPWM_PCR_VPROTD_Pos, MPWM_PCR_VPROTD_Msk);
    }

    if (mcb->mode == MPWM_MODE_INDIVIDUAL &&
        (p_type == MPWM_PHASE_TYPE_W || p_type == MPWM_PHASE_TYPE_ALL))
    {
        MPWM_REG_BIT_SET(mpwm->PCR, cfg->enable, MPWM_PCR_WPROTEN_Pos);
        MPWM_REG_BIT_SET(mpwm->PCR, cfg->polarity, MPWM_PCR_WPROTPOL_Pos);
        MPWM_REG_VAL_SET(mpwm->PCR, cfg->debounce, MPWM_PCR_WPROTD_Pos, MPWM_PCR_WPROTD_Msk);
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_MPWM_SetProtectionOutputConfig(enum mpwm_id id, enum mpwm_phase_type p_type,
                                           enum mpwm_output_type ot, uint8_t en)
{
    MPWM_Type *mpwm;

    DRIVER_ASSERT(id < MPWM_ID_MAX);

    mpwm = MPWM_REG(id);

    if (p_type == MPWM_PHASE_TYPE_U || p_type == MPWM_PHASE_TYPE_ALL)
    {
        if (ot == MPWM_OUTPUT_TYPE_L)
        {
            MPWM_REG_BIT_SET(mpwm->PCR, en, MPWM_PCR_ULPROTM_Pos);
        }
        else
        {
            MPWM_REG_BIT_SET(mpwm->PCR, en, MPWM_PCR_UHPROTM_Pos);
        }
    }

    if (p_type == MPWM_PHASE_TYPE_V || p_type == MPWM_PHASE_TYPE_ALL)
    {
        if (ot == MPWM_OUTPUT_TYPE_L)
        {
            MPWM_REG_BIT_SET(mpwm->PCR, en, MPWM_PCR_VLPROTM_Pos);
        }
        else
        {
            MPWM_REG_BIT_SET(mpwm->PCR, en, MPWM_PCR_VHPROTM_Pos);
        }
    }

    if (p_type == MPWM_PHASE_TYPE_W || p_type == MPWM_PHASE_TYPE_ALL)
    {
        if (ot == MPWM_OUTPUT_TYPE_L)
        {
            MPWM_REG_BIT_SET(mpwm->PCR, en, MPWM_PCR_WLPROTM_Pos);
        }
        else
        {
            MPWM_REG_BIT_SET(mpwm->PCR, en, MPWM_PCR_WHPROTM_Pos);
        }
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_MPWM_SetOverVoltageInputConfig(enum mpwm_id id, enum mpwm_phase_type p_type,
                                           struct mpwm_input_ovv_cfg *cfg)
{
    struct mpwm_ctrl_block *mcb;
    MPWM_Type *mpwm;

    DRIVER_ASSERT(id < MPWM_ID_MAX);

    mpwm = MPWM_REG(id);
    mcb = &g_mcb[id];

    if ((mcb->mode == MPWM_MODE_INDIVIDUAL &&
         (p_type == MPWM_PHASE_TYPE_U || p_type == MPWM_PHASE_TYPE_ALL)) ||
        (mcb->mode != MPWM_MODE_INDIVIDUAL))
    {
        MPWM_REG_BIT_SET(mpwm->OCR, cfg->enable, MPWM_OCR_OVINEN_Pos);
        MPWM_REG_BIT_SET(mpwm->OCR, cfg->polarity, MPWM_OCR_OVINPOL_Pos);
        MPWM_REG_VAL_SET(mpwm->OCR, cfg->debounce, MPWM_OCR_OVIND_Pos, MPWM_OCR_OVIND_Msk);
    }

    if (mcb->mode == MPWM_MODE_INDIVIDUAL &&
        (p_type == MPWM_PHASE_TYPE_V || p_type == MPWM_PHASE_TYPE_ALL))
    {
        MPWM_REG_BIT_SET(mpwm->OCR, cfg->enable, MPWM_OCR_VOVINEN_Pos);
        MPWM_REG_BIT_SET(mpwm->OCR, cfg->polarity, MPWM_OCR_VOVINPOL_Pos);
        MPWM_REG_VAL_SET(mpwm->OCR, cfg->debounce, MPWM_OCR_VOVIND_Pos, MPWM_OCR_VOVIND_Msk);
    }

    if (mcb->mode == MPWM_MODE_INDIVIDUAL &&
        (p_type == MPWM_PHASE_TYPE_W || p_type == MPWM_PHASE_TYPE_ALL))
    {
        MPWM_REG_BIT_SET(mpwm->OCR, cfg->enable, MPWM_OCR_WOVINEN_Pos);
        MPWM_REG_BIT_SET(mpwm->OCR, cfg->polarity, MPWM_OCR_WOVINPOL_Pos);
        MPWM_REG_VAL_SET(mpwm->OCR, cfg->debounce, MPWM_OCR_WOVIND_Pos, MPWM_OCR_WOVIND_Msk);
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_PMWM_SetOverVolatgeOutputConfig(enum mpwm_id id, enum mpwm_phase_type p_type,
                                            enum mpwm_output_type ot, uint8_t en)
{
    MPWM_Type *mpwm;

    DRIVER_ASSERT(id < MPWM_ID_MAX);

    mpwm = MPWM_REG(id);

    if (p_type == MPWM_PHASE_TYPE_U || p_type == MPWM_PHASE_TYPE_ALL)
    {
        if (ot == MPWM_OUTPUT_TYPE_L)
        {
            MPWM_REG_BIT_SET(mpwm->OCR, en, MPWM_OCR_ULOVINM_Pos);
        }
        else
        {
            MPWM_REG_BIT_SET(mpwm->OCR, en, MPWM_OCR_UHOVINM_Pos);
        }
    }

    if (p_type == MPWM_PHASE_TYPE_V || p_type == MPWM_PHASE_TYPE_ALL)
    {
        if (ot == MPWM_OUTPUT_TYPE_L)
        {
            MPWM_REG_BIT_SET(mpwm->OCR, en, MPWM_OCR_VLOVINM_Pos);
        }
        else
        {
            MPWM_REG_BIT_SET(mpwm->OCR, en, MPWM_OCR_VHOVINM_Pos);
        }
    }

    if (p_type == MPWM_PHASE_TYPE_W || p_type == MPWM_PHASE_TYPE_ALL)
    {
        if (ot == MPWM_OUTPUT_TYPE_L)
        {
            MPWM_REG_BIT_SET(mpwm->OCR, en, MPWM_OCR_WLOVINM_Pos);
        }
        else
        {
            MPWM_REG_BIT_SET(mpwm->OCR, en, MPWM_OCR_WHOVINM_Pos);
        }
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_MPWM_SetDeadTimeConfig(enum mpwm_id id, enum mpwm_phase_type p_type, struct mpwm_dt_cfg *cfg)
{
    struct mpwm_ctrl_block *mcb;
    MPWM_Type *mpwm;

    DRIVER_ASSERT(id < MPWM_ID_MAX);

    mpwm = MPWM_REG(id);
    mcb = &g_mcb[id];

    if (mcb->mode == MPWM_MODE_NORMAL || mcb->mode == MPWM_MODE_MOTOR)
    {
        MPWM_REG_BIT_SET(mpwm->DTR, cfg->enable, MPWM_DTR_DTEN_Pos);
        MPWM_REG_BIT_SET(mpwm->DTR, cfg->short_circuit_protect_en, MPWM_DTR_PSHRT_Pos);
        MPWM_REG_BIT_SET(mpwm->DTR, cfg->mode, MPWM_DTR_DTMDSEL_Pos);
        MPWM_REG_VAL_SET(mpwm->DTR, cfg->clk, MPWM_DTR_DTCLK_Pos, MPWM_DTR_DTCLK_Msk);
        MPWM_REG_VAL_SET(mpwm->DTR, cfg->dead_time, MPWM_DTR_DT_Pos, MPWM_DTR_DT_Msk);
    }

    if (mcb->mode == MPWM_MODE_INDIVIDUAL &&
        (p_type == MPWM_PHASE_TYPE_U || p_type == MPWM_PHASE_TYPE_ALL))
    {
        MPWM_REG_BIT_SET(mpwm->DTRU, cfg->enable, MPWM_DTRU_UDTEN_Pos);
        MPWM_REG_BIT_SET(mpwm->DTRU, cfg->short_circuit_protect_en, MPWM_DTRU_UPSHRT_Pos);
        MPWM_REG_BIT_SET(mpwm->DTRU, cfg->mode, MPWM_DTRU_DTMDSEL_Pos);
        MPWM_REG_VAL_SET(mpwm->DTRU, cfg->clk, MPWM_DTRU_UDTCLK_Pos, MPWM_DTRU_UDTCLK_Msk);
        MPWM_REG_VAL_SET(mpwm->DTRU, cfg->dead_time, MPWM_DTRU_UDT_Pos, MPWM_DTRU_UDT_Msk);
    }

    if (mcb->mode == MPWM_MODE_INDIVIDUAL &&
        (p_type == MPWM_PHASE_TYPE_V || p_type == MPWM_PHASE_TYPE_ALL))
    {
        MPWM_REG_BIT_SET(mpwm->DTRV, cfg->enable, MPWM_DTRV_VDTEN_Pos);
        MPWM_REG_BIT_SET(mpwm->DTRV, cfg->short_circuit_protect_en, MPWM_DTRV_VPSHRT_Pos);
        MPWM_REG_BIT_SET(mpwm->DTRV, cfg->mode, MPWM_DTRV_DTMDSEL_Pos);
        MPWM_REG_VAL_SET(mpwm->DTRV, cfg->clk, MPWM_DTRV_VDTCLK_Pos, MPWM_DTRV_VDTCLK_Msk);
        MPWM_REG_VAL_SET(mpwm->DTRV, cfg->dead_time, MPWM_DTRV_VDT_Pos, MPWM_DTRV_VDT_Msk);
    }

    if (mcb->mode == MPWM_MODE_INDIVIDUAL &&
        (p_type == MPWM_PHASE_TYPE_W || p_type == MPWM_PHASE_TYPE_ALL))
    {
        MPWM_REG_BIT_SET(mpwm->DTRW, cfg->enable, MPWM_DTRW_WDTEN_Pos);
        MPWM_REG_BIT_SET(mpwm->DTRW, cfg->short_circuit_protect_en, MPWM_DTRW_WPSHRT_Pos);
        MPWM_REG_BIT_SET(mpwm->DTRW, cfg->mode, MPWM_DTRW_DTMDSEL_Pos);
        MPWM_REG_VAL_SET(mpwm->DTRW, cfg->clk, MPWM_DTRW_WDTCLK_Pos, MPWM_DTRW_WDTCLK_Msk);
        MPWM_REG_VAL_SET(mpwm->DTRW, cfg->dead_time, MPWM_DTRW_WDT_Pos, MPWM_DTRW_WDT_Msk);
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_MPWM_SetADCTriggerConfig(enum mpwm_id id, enum mpwm_trgger_id t_id, struct mpwm_adc_tr_cfg *cfg)
{
    MPWM_Type *mpwm;
    uint32_t *ATR;

    DRIVER_ASSERT(id < MPWM_ID_MAX);

    mpwm = MPWM_REG(id);

    ATR = (uint32_t *)(&mpwm->ATR1 + (id * 4));
    MPWM_REG_VAL_SET(*ATR, cfg->tr_src, MPWM_ATR1_ATSRC_Pos, MPWM_ATR1_ATSRC_Msk);
    MPWM_REG_VAL_SET(*ATR, cfg->mode, MPWM_ATR1_ATMOD_Pos, MPWM_ATR1_ATMOD_Msk);
    MPWM_REG_BIT_SET(*ATR, cfg->reg_mode, MPWM_ATR1_ATUDT_Pos);
    MPWM_REG_VAL_SET(*ATR, cfg->tr_count, MPWM_ATR1_ATCNT_Pos, MPWM_ATR1_ATCNT_Msk);

    return DRIVER_ERROR_OK;
}

int32_t HAL_MPWM_SetIRQinterval(enum mpwm_id id, enum mpwm_phase_type p_type,
                                uint8_t irq_intv)
{
    struct mpwm_ctrl_block *mcb;
    MPWM_Type *mpwm;

    DRIVER_ASSERT(id < MPWM_ID_MAX);

    mpwm = MPWM_REG(id);
    mcb = &g_mcb[id];

    if (mcb->mode != MPWM_MODE_INDIVIDUAL)
    {
        MPWM_REG_VAL_SET(mpwm->CR1, irq_intv, MPWM_CR1_IRQN_Pos, MPWM_CR1_IRQN_Msk);
    }
    else
    {
        if (p_type == MPWM_PHASE_TYPE_U || p_type == MPWM_PHASE_TYPE_ALL)
        {
            MPWM_REG_VAL_SET(mpwm->CR3, irq_intv, MPWM_CR3_UIRQN_Pos, MPWM_CR3_UIRQN_Msk);
        }

        if (p_type == MPWM_PHASE_TYPE_V || p_type == MPWM_PHASE_TYPE_ALL)
        {
            MPWM_REG_VAL_SET(mpwm->CR3, irq_intv, MPWM_CR3_VIRQN_Pos, MPWM_CR3_VIRQN_Msk);
        }

        if (p_type == MPWM_PHASE_TYPE_W || p_type == MPWM_PHASE_TYPE_ALL)
        {
            MPWM_REG_VAL_SET(mpwm->CR3, irq_intv, MPWM_CR3_WIRQN_Pos, MPWM_CR3_WIRQN_Msk);
        }
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_MPWM_SetDisabledOutputLevel(enum mpwm_id id,
                                        enum mpwm_phase_type p_type,
                                        enum mpwm_output_type ot, uint8_t hl)
{
    MPWM_Type *mpwm;

    DRIVER_ASSERT(id < MPWM_ID_MAX);

    mpwm = MPWM_REG(id);

    if (p_type == MPWM_PHASE_TYPE_U || p_type == MPWM_PHASE_TYPE_ALL)
    {
        if (ot == MPWM_OUTPUT_TYPE_L)
        {
            MPWM_REG_BIT_SET(mpwm->OLR, hl, MPWM_OLR_DOLUL_Pos);
        }
        else
        {
            MPWM_REG_BIT_SET(mpwm->OLR, hl, MPWM_OLR_DOLUH_Pos);
        }
    }

    if (p_type == MPWM_PHASE_TYPE_V || p_type == MPWM_PHASE_TYPE_ALL)
    {
        if (ot == MPWM_OUTPUT_TYPE_L)
        {
            MPWM_REG_BIT_SET(mpwm->OLR, hl, MPWM_OLR_DOLVL_Pos);
        }
        else
        {
            MPWM_REG_BIT_SET(mpwm->OLR, hl, MPWM_OLR_DOLVH_Pos);
        }
    }

    if (p_type == MPWM_PHASE_TYPE_W || p_type == MPWM_PHASE_TYPE_ALL)
    {
        if (ot == MPWM_OUTPUT_TYPE_L)
        {
            MPWM_REG_BIT_SET(mpwm->OLR, hl, MPWM_OLR_DOLWL_Pos);
        }
        else
        {
            MPWM_REG_BIT_SET(mpwm->OLR, hl, MPWM_OLR_DOLWH_Pos);
        }
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_MPWM_SetStartOutputLevel(enum mpwm_id id,
                                     enum mpwm_phase_type p_type,
                                     enum mpwm_output_type ot, uint8_t hl)
{
    MPWM_Type *mpwm;

    DRIVER_ASSERT(id < MPWM_ID_MAX);

    mpwm = MPWM_REG(id);

    if (p_type == MPWM_PHASE_TYPE_U || p_type == MPWM_PHASE_TYPE_ALL)
    {
        if (ot == MPWM_OUTPUT_TYPE_L)
        {
            MPWM_REG_BIT_SET(mpwm->OLR, hl, MPWM_OLR_ULL_Pos);
        }
        else
        {
            MPWM_REG_BIT_SET(mpwm->OLR, hl, MPWM_OLR_UHL_Pos);
        }
    }

    if (p_type == MPWM_PHASE_TYPE_V || p_type == MPWM_PHASE_TYPE_ALL)
    {
        if (ot == MPWM_OUTPUT_TYPE_L)
        {
            MPWM_REG_BIT_SET(mpwm->OLR, hl, MPWM_OLR_VLL_Pos);
        }
        else
        {
            MPWM_REG_BIT_SET(mpwm->OLR, hl, MPWM_OLR_VHL_Pos);
        }
    }

    if (p_type == MPWM_PHASE_TYPE_W || p_type == MPWM_PHASE_TYPE_ALL)
    {
        if (ot == MPWM_OUTPUT_TYPE_L)
        {
            MPWM_REG_BIT_SET(mpwm->OLR, hl, MPWM_OLR_WLL_Pos);
        }
        else
        {
            MPWM_REG_BIT_SET(mpwm->OLR, hl, MPWM_OLR_WHL_Pos);
        }
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_MPWM_SetForceOutputLevel(enum mpwm_id id,
                                     enum mpwm_phase_type p_type,
                                     enum mpwm_output_type ot, uint8_t hl)
{
    MPWM_Type *mpwm;

    DRIVER_ASSERT(id < MPWM_ID_MAX);

    mpwm = MPWM_REG(id);

    if (p_type == MPWM_PHASE_TYPE_U || p_type == MPWM_PHASE_TYPE_ALL)
    {
        if (ot == MPWM_OUTPUT_TYPE_L)
        {
            MPWM_REG_BIT_SET(mpwm->FOLR, hl, MPWM_FOLR_ULFL_Pos);
        }
        else
        {
            MPWM_REG_BIT_SET(mpwm->FOLR, hl, MPWM_FOLR_UHFL_Pos);
        }
    }

    if (p_type == MPWM_PHASE_TYPE_V || p_type == MPWM_PHASE_TYPE_ALL)
    {
        if (ot == MPWM_OUTPUT_TYPE_L)
        {
            MPWM_REG_BIT_SET(mpwm->FOLR, hl, MPWM_FOLR_VLFL_Pos);
        }
        else
        {
            MPWM_REG_BIT_SET(mpwm->FOLR, hl, MPWM_FOLR_VHFL_Pos);
        }
    }

    if (p_type == MPWM_PHASE_TYPE_W || p_type == MPWM_PHASE_TYPE_ALL)
    {
        if (ot == MPWM_OUTPUT_TYPE_L)
        {
            MPWM_REG_BIT_SET(mpwm->FOLR, hl, MPWM_FOLR_WLFL_Pos);
        }
        else
        {
            MPWM_REG_BIT_SET(mpwm->FOLR, hl, MPWM_FOLR_WHFL_Pos);
        }
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_MPWM_SetPeriod(enum mpwm_id id, enum mpwm_phase_type p_type, uint16_t prd)
{
    struct mpwm_ctrl_block *mcb;
    MPWM_Type *mpwm;

    DRIVER_ASSERT(id < MPWM_ID_MAX);

    mpwm = MPWM_REG(id);
    mcb = &g_mcb[id];

    if (prd < 0x10)
    {
        return DRIVER_ERROR_PARAMETER;
    }

    if (mcb->mode == MPWM_MODE_MOTOR || mcb->mode == MPWM_MODE_NORMAL)
    {
        MPWM_REG_VAL_SET(mpwm->PRD, prd, MPWM_PRD_PERIOD_Pos, MPWM_PRD_PERIOD_Msk);
    }
    else
    {
        if (p_type == MPWM_PHASE_TYPE_U || p_type == MPWM_PHASE_TYPE_ALL)
        {
            MPWM_REG_VAL_SET(mpwm->PRDU, prd, MPWM_PRDU_PERIOD_U_Pos, MPWM_PRDU_PERIOD_U_Msk);
        }

        if (p_type == MPWM_PHASE_TYPE_V || p_type == MPWM_PHASE_TYPE_ALL)
        {
            MPWM_REG_VAL_SET(mpwm->PRDV, prd, MPWM_PRDV_PERIOD_V_Pos, MPWM_PRDV_PERIOD_V_Msk);
        }

        if (p_type == MPWM_PHASE_TYPE_W || p_type == MPWM_PHASE_TYPE_ALL)
        {
            MPWM_REG_VAL_SET(mpwm->PRDW, prd, MPWM_PRDW_PERIOD_W_Pos, MPWM_PRDW_PERIOD_W_Msk);
        }
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_MPWM_SetDuty(enum mpwm_id id, enum mpwm_phase_type p_type,
                         enum mpwm_output_type ot, uint16_t duty)
{
    MPWM_Type *mpwm;

    DRIVER_ASSERT(id < MPWM_ID_MAX);

    mpwm = MPWM_REG(id);

    if (p_type == MPWM_PHASE_TYPE_U || p_type == MPWM_PHASE_TYPE_ALL)
    {
        if (ot == MPWM_OUTPUT_TYPE_L)
        {
            MPWM_REG_VAL_SET(mpwm->DUL, duty, MPWM_DUL_DUTY_UL_Pos, MPWM_DUL_DUTY_UL_Msk);
        }
        else
        {
            MPWM_REG_VAL_SET(mpwm->DUH, duty, MPWM_DUH_DUTY_UH_Pos, MPWM_DUH_DUTY_UH_Msk);
        }
    }

    if (p_type == MPWM_PHASE_TYPE_V || p_type == MPWM_PHASE_TYPE_ALL)
    {
        if (ot == MPWM_OUTPUT_TYPE_L)
        {
            MPWM_REG_VAL_SET(mpwm->DVL, duty, MPWM_DVL_DUTY_VL_Pos, MPWM_DVL_DUTY_VL_Msk);
        }
        else
        {
            MPWM_REG_VAL_SET(mpwm->DVH, duty, MPWM_DVH_DUTY_VH_Pos, MPWM_DVH_DUTY_VH_Msk);
        }
    }

    if (p_type == MPWM_PHASE_TYPE_W || p_type == MPWM_PHASE_TYPE_ALL)
    {
        if (ot == MPWM_OUTPUT_TYPE_L)
        {
            MPWM_REG_VAL_SET(mpwm->DWL, duty, MPWM_DWL_DUTY_WL_Pos, MPWM_DWL_DUTY_WL_Msk);
        }
        else
        {
            MPWM_REG_VAL_SET(mpwm->DWH, duty, MPWM_DWH_DUTY_WH_Pos, MPWM_DWH_DUTY_WH_Msk);
        }
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_MPWM_Start(enum mpwm_id id, enum mpwm_phase_type p_type)
{
    struct mpwm_ctrl_block *mcb;
    MPWM_Type *mpwm;

    DRIVER_ASSERT(id < MPWM_ID_MAX);

    mpwm = MPWM_REG(id);
    mcb = &g_mcb[id];

    if (mcb->mode != MPWM_MODE_INDIVIDUAL)
    {
        MPWM_REG_BIT_SET(mpwm->CR2, 1, MPWM_CR2_PSTART_Pos);
    }
    else
    {
        if (p_type == MPWM_PHASE_TYPE_U || p_type == MPWM_PHASE_TYPE_ALL)
        {
            MPWM_REG_BIT_SET(mpwm->CR3, 1, MPWM_CR3_UEN_Pos);
            MPWM_REG_BIT_SET(mpwm->CR3, 1, MPWM_CR3_USTART_Pos);
        }

        if (p_type == MPWM_PHASE_TYPE_V || p_type == MPWM_PHASE_TYPE_ALL)
        {
            MPWM_REG_BIT_SET(mpwm->CR3, 1, MPWM_CR3_VEN_Pos);
            MPWM_REG_BIT_SET(mpwm->CR3, 1, MPWM_CR3_VSTART_Pos);
        }

        if (p_type == MPWM_PHASE_TYPE_W || p_type == MPWM_PHASE_TYPE_ALL)
        {
            MPWM_REG_BIT_SET(mpwm->CR3, 1, MPWM_CR3_WEN_Pos);
            MPWM_REG_BIT_SET(mpwm->CR3, 1, MPWM_CR3_WSTART_Pos);
        }
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_MPWM_Stop(enum mpwm_id id, enum mpwm_phase_type p_type)
{
    struct mpwm_ctrl_block *mcb;
    MPWM_Type *mpwm;

    DRIVER_ASSERT(id < MPWM_ID_MAX);

    mpwm = MPWM_REG(id);
    mcb = &g_mcb[id];

    if (mcb->mode != MPWM_MODE_INDIVIDUAL)
    {
        MPWM_REG_BIT_SET(mpwm->CR2, 0, MPWM_CR2_PSTART_Pos);
        MPWM_REG_BIT_SET(mpwm->CR1, 0, MPWM_CR1_PWMEN_Pos);
    }
    else
    {
        if (p_type == MPWM_PHASE_TYPE_U || p_type == MPWM_PHASE_TYPE_ALL)
        {
            MPWM_REG_BIT_SET(mpwm->CR4, 1, MPWM_CR4_USTOP_Pos);
            MPWM_REG_BIT_SET(mpwm->CR4, 1, MPWM_CR4_UDIS_Pos);
        }

        if (p_type == MPWM_PHASE_TYPE_V || p_type == MPWM_PHASE_TYPE_ALL)
        {
            MPWM_REG_BIT_SET(mpwm->CR4, 1, MPWM_CR4_VSTOP_Pos);
            MPWM_REG_BIT_SET(mpwm->CR4, 1, MPWM_CR4_VDIS_Pos);
        }

        if (p_type == MPWM_PHASE_TYPE_W || p_type == MPWM_PHASE_TYPE_ALL)
        {
            MPWM_REG_BIT_SET(mpwm->CR4, 1, MPWM_CR4_WSTOP_Pos);
            MPWM_REG_BIT_SET(mpwm->CR4, 1, MPWM_CR4_WDIS_Pos);
        }
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_MPWM_Halt(enum mpwm_id id, enum mpwm_phase_type p_type)
{
    struct mpwm_ctrl_block *mcb;
    MPWM_Type *mpwm;

    DRIVER_ASSERT(id < MPWM_ID_MAX);

    mpwm = MPWM_REG(id);
    mcb = &g_mcb[id];

    if (mcb->mode != MPWM_MODE_INDIVIDUAL)
    {
        MPWM_REG_BIT_SET(mpwm->CR2, 1, MPWM_CR2_HALT_Pos);
    }
    else
    {
        if (p_type == MPWM_PHASE_TYPE_U || p_type == MPWM_PHASE_TYPE_ALL)
        {
            MPWM_REG_BIT_SET(mpwm->CR3, 1, MPWM_CR3_UHALT_Pos);
        }

        if (p_type == MPWM_PHASE_TYPE_V || p_type == MPWM_PHASE_TYPE_ALL)
        {
            MPWM_REG_BIT_SET(mpwm->CR3, 1, MPWM_CR3_VHALT_Pos);
        }

        if (p_type == MPWM_PHASE_TYPE_W || p_type == MPWM_PHASE_TYPE_ALL)
        {
            MPWM_REG_BIT_SET(mpwm->CR3, 1, MPWM_CR3_WHALT_Pos);
        }
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_MPWM_Resume(enum mpwm_id id, enum mpwm_phase_type p_type)
{
    struct mpwm_ctrl_block *mcb;
    MPWM_Type *mpwm;

    DRIVER_ASSERT(id < MPWM_ID_MAX);

    mpwm = MPWM_REG(id);
    mcb = &g_mcb[id];

    if (mcb->mode != MPWM_MODE_INDIVIDUAL)
    {
        MPWM_REG_BIT_SET(mpwm->CR2, 0, MPWM_CR2_HALT_Pos);
    }
    else
    {
        if (p_type == MPWM_PHASE_TYPE_U || p_type == MPWM_PHASE_TYPE_ALL)
        {
            MPWM_REG_BIT_SET(mpwm->CR3, 0, MPWM_CR3_UHALT_Pos);
        }

        if (p_type == MPWM_PHASE_TYPE_V || p_type == MPWM_PHASE_TYPE_ALL)
        {
            MPWM_REG_BIT_SET(mpwm->CR3, 0, MPWM_CR3_VHALT_Pos);
        }

        if (p_type == MPWM_PHASE_TYPE_W || p_type == MPWM_PHASE_TYPE_ALL)
        {
            MPWM_REG_BIT_SET(mpwm->CR3, 0, MPWM_CR3_WHALT_Pos);
        }
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_MPWM_ReadCount(enum mpwm_id id, enum mpwm_phase_type p_type, uint16_t *count)
{
    struct mpwm_ctrl_block *mcb;
    MPWM_Type *mpwm;

    DRIVER_ASSERT(id < MPWM_ID_MAX);

    mpwm = MPWM_REG(id);
    mcb = &g_mcb[id];

    if (mcb->mode != MPWM_MODE_INDIVIDUAL)
    {
        *count = mpwm->CNT;
    }
    else
    {
        if (p_type == MPWM_PHASE_TYPE_U)
        {
            *count = mpwm->CNTU;
        }

        if (p_type == MPWM_PHASE_TYPE_V)
        {
            *count = mpwm->CNTU;
        }

        if (p_type == MPWM_PHASE_TYPE_W)
        {
            *count = mpwm->CNTU;
        }
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_MPWM_ReadStatus(enum mpwm_id id, uint32_t *status)
{
    MPWM_Type *mpwm;

    DRIVER_ASSERT(id < MPWM_ID_MAX);

    mpwm = MPWM_REG(id);

    *status = mpwm->SR;

    return DRIVER_ERROR_OK;
}

int32_t HAL_MPWM_ReadProtectionStatus(enum mpwm_id id, uint32_t *status)
{
    MPWM_Type *mpwm;

    DRIVER_ASSERT(id < MPWM_ID_MAX);

    mpwm = MPWM_REG(id);

    *status = mpwm->PSR;

    return DRIVER_ERROR_OK;
}

int32_t HAL_MPWM_ReadOverVoltageStatus(enum mpwm_id id, uint32_t *status)
{
    MPWM_Type *mpwm;

    DRIVER_ASSERT(id < MPWM_ID_MAX);

    mpwm = MPWM_REG(id);

    *status = mpwm->OSR;

    return DRIVER_ERROR_OK;
}

static void MPWM_IRQHandler(enum mpwm_id id, enum mpwm_irq_event_type type)
{
    struct mpwm_ctrl_block *mcb;

    mcb = &g_mcb[id];

    if (mcb->handler)
    {
        mcb->handler(type, mcb->context);
    }
}

void MPWM0PROT_IRQHandler(void)
{
    MPWM_IRQHandler(MPWM_ID_0, MPWM_IRQ_EVENT_TYPE_PROT);
}

void MPWM0OVV_IRQHandler(void)
{
    MPWM_IRQHandler(MPWM_ID_0, MPWM_IRQ_EVENT_TYPE_OVV);
}

void MPWM0U_IRQHandler(void)
{
    MPWM_IRQHandler(MPWM_ID_0, MPWM_IRQ_EVENT_TYPE_U);
}

void MPWM0V_IRQHandler(void)
{
    MPWM_IRQHandler(MPWM_ID_0, MPWM_IRQ_EVENT_TYPE_V);
}

void MPWM0W_IRQHandler(void)
{
    MPWM_IRQHandler(MPWM_ID_0, MPWM_IRQ_EVENT_TYPE_W);
}

void MPWM1PROT_IRQHandler(void)
{
    MPWM_IRQHandler(MPWM_ID_1, MPWM_IRQ_EVENT_TYPE_PROT);
}

void MPWM1OVV_IRQHandler(void)
{
    MPWM_IRQHandler(MPWM_ID_1, MPWM_IRQ_EVENT_TYPE_OVV);
}

void MPWM1U_IRQHandler(void)
{
    MPWM_IRQHandler(MPWM_ID_1, MPWM_IRQ_EVENT_TYPE_U);
}

void MPWM1V_IRQHandler(void)
{
    MPWM_IRQHandler(MPWM_ID_1, MPWM_IRQ_EVENT_TYPE_V);
}

void MPWM1W_IRQHandler(void)
{
    MPWM_IRQHandler(MPWM_ID_1, MPWM_IRQ_EVENT_TYPE_W);
}
