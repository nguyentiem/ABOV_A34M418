/**
 *******************************************************************************
 * @file        a34m41x_comp.c
 * @author      ABOV R&D Division
 * @brief       Comparator
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
#include <string.h>

#include "a34m41x.h"
#include "a34m41x_scu.h"
#include "a34m41x_comp.h"

#define COMP_REG_OFFSET     0xC

static __INLINE COMP_Type *COMP_REG(enum comp_id id)
{
    return (COMP_Type *)(COMP0_BASE + (COMP_REG_OFFSET * id));
}

static __INLINE IRQn_Type COMP_IRQ_NUM(enum comp_id id)
{
    return (IRQn_Type)((uint8_t)COMP0_IRQn + id);
}

struct comp_ctrl_block
{
    comp_irq_handler_t  handler;
    void *              context;
    uint8_t             irq_enable;
};

static struct comp_ctrl_block g_ccb[COMP_ID_MAX];

int32_t HAL_COMP_Init(enum comp_id id, struct comp_cfg *cfg,
                      comp_irq_handler_t handler,
                      void *context)
{
    struct comp_ctrl_block *ccb;
    COMP_Type *comp;

    DRIVER_ASSERT(id < COMP_ID_MAX);
    DRIVER_ASSERT(cfg != NULL);

    ccb = &g_ccb[id];
    comp = COMP_REG(id);

    memset(ccb, 0, sizeof(struct comp_ctrl_block));

    ccb->handler = handler;
    ccb->context = context;

    SCU->PER2 &= ~(1 << (SCU_PER2_COMPARATOR_Pos + id));
    SCU->PCER2 &= ~(1 << (SCU_PCER2_COMPARATOR_Pos + id));

    SCU->PER2 |= (1 << (SCU_PER2_COMPARATOR_Pos + id));
    SCU->PCER2 |= (1 << (SCU_PCER2_COMPARATOR_Pos + id));

    comp->CONF = 0;

    comp->CONF |= (cfg->filter << COMP_CONF_FLTSEL_Pos);

    if (cfg->hystresis == COMP_HYSTRESIS_20MV)
    {
        comp->CONF |= (1 << COMP_CONF_HYSEN_Pos) | (1 << COMP_CONF_HYSSEL_Pos);
    }
    else if (cfg->hystresis == COMP_HYSTRESIS_5MV)
    {
        comp->CONF |= (1 << COMP_CONF_HYSEN_Pos);
    }

    if (cfg->irq_type == COMP_IRQ_LEVEL)
    {
        comp->CONF |= (1 << COMP_CONF_INTTYPE_Pos);
        ccb->irq_enable = 1;
    }
    else if (cfg->irq_type == COMP_IRQ_LOW_EDGE)
    {
        comp->CONF |= (2 << COMP_CONF_INTTYPE_Pos);
        ccb->irq_enable = 1;
    }
    else if (cfg->irq_type == COMP_IRQ_HIGH_EDGE)
    {
        comp->CONF |= (2 << COMP_CONF_INTTYPE_Pos) | (1 << COMP_CONF_INTPOL_Pos);
        ccb->irq_enable = 1;
    }
    else if (cfg->irq_type == COMP_IRQ_BOTH_EDGE)
    {
        comp->CONF |= (3 << COMP_CONF_INTTYPE_Pos);
        ccb->irq_enable = 1;
    }
    else if (cfg->irq_type == COMP_IRQ_NONE)
    {
        ccb->irq_enable = 0;
    }

    comp->CONF |= (cfg->input_neg << COMP_CONF_CINNSEL_Pos);
    comp->CONF |= (cfg->input_pos << COMP_CONF_CINPSEL_Pos);

    NVIC_ClearPendingIRQ(COMP_IRQ_NUM(id));
    NVIC_SetPriority(COMP_IRQ_NUM(id), cfg->irq_prio);
    NVIC_EnableIRQ(COMP_IRQ_NUM(id));

    return DRIVER_ERROR_OK;
}

int32_t HAL_COMP_Deinit(enum comp_id id)
{
    DRIVER_ASSERT(id < COMP_ID_MAX);

    SCU->PER2 &= ~(1 << (SCU_PER2_COMPARATOR_Pos + id));
    SCU->PCER2 &= ~(1 << (SCU_PCER2_COMPARATOR_Pos + id));

    NVIC_DisableIRQ(COMP_IRQ_NUM(id));

    return DRIVER_ERROR_OK;
}

int32_t HAL_COMP_Start(enum comp_id id)
{
    struct comp_ctrl_block *ccb;
    COMP_Type *comp;

    DRIVER_ASSERT(id < COMP_ID_MAX);

    comp = COMP_REG(id);
    ccb = &g_ccb[id];

	  comp->CTRL |= (1 << COMP_CTRL_COMPEN_Pos);
	
    if (ccb->irq_enable)
    {
        comp->CTRL |= (1 << COMP_CTRL_COMPINTEN_Pos);
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_COMP_Stop(enum comp_id id)
{
    COMP_Type *comp;

    DRIVER_ASSERT(id < COMP_ID_MAX);

    comp = COMP_REG(id);

    comp->CTRL &= ~(1 << COMP_CTRL_COMPEN_Pos);
    comp->CTRL &= ~(1 << COMP_CTRL_COMPINTEN_Pos);

    return DRIVER_ERROR_OK;
}

int32_t HAL_COMP_GetStatus(enum comp_id id, uint8_t *status)
{
    COMP_Type *comp;
    uint32_t value;

    DRIVER_ASSERT(id < COMP_ID_MAX);

    comp = COMP_REG(id);

    value = comp->STAT;
    comp->STAT = value;

    if (value & COMP_STAT_COMPFLAG_Msk)
    {
        *status = COMP_STATUS_UP;
    }
    else
    {
        *status = COMP_STATUS_DOWN;
    }

    return DRIVER_ERROR_OK;
}


static void COMP_IRQHandler(enum comp_id id)
{
    struct comp_ctrl_block *ccb;
    COMP_Type *comp;
    uint32_t status;
    uint32_t event;

    ccb = &g_ccb[id];
    comp = COMP_REG(id);

    status = comp->STAT;
    comp->STAT = status;
    if (status & COMP_STAT_COMPFLAG_Msk)
    {
        event = COMP_EVENT_UP;	// kyi - POL ������ ���� ����Ǵ� ��
    }
    else
    {
        event = COMP_EVENT_DOWN;
    }

    ccb->handler(event, ccb->context);	// kyi -  clear interrupt flag?
}

void COMP0_IRQHandler(void)
{
    COMP_IRQHandler(COMP_ID_0);
}

void COMP1_IRQHandler(void)
{
    COMP_IRQHandler(COMP_ID_1);
}

void COMP2_IRQHandler(void)
{
    COMP_IRQHandler(COMP_ID_2);
}

void COMP3_IRQHandler(void)
{
    COMP_IRQHandler(COMP_ID_3);
}

