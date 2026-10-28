/**
 *******************************************************************************
 * @file        a34m41x_rng.c
 * @author      ABOV R&D Division
 * @brief       Random Number Generator
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
#include "a34m41x_rng.h"


struct rng_ctrl_block
{
    uint32_t *          value;
    rng_irq_handler_t   handler;
    void *              context;
};

static struct rng_ctrl_block g_rcb;


int32_t HAL_RNG_Init(struct rng_cfg *cfg, rng_irq_handler_t handler, void *context)
{
    uint32_t reg_val;

    DRIVER_ASSERT(cfg != NULL);

    SCU->PER2 &= ~(1UL << SCU_PER2_RNG_Pos);
    SCU->PCER2 &= ~(1UL << SCU_PCER2_RNG_Pos);

    SCU->PER2 |= (1UL << SCU_PER2_RNG_Pos);
    SCU->PCER2 |= (1UL << SCU_PCER2_RNG_Pos);

    reg_val = 0;
    reg_val |= (cfg->gcp << RNG_CTRL_GCP_Pos);
    reg_val |= (cfg->casr << RNG_CTRL_CCS_Pos);
    reg_val |= (cfg->lfsr << RNG_CTRL_LCS_Pos);

    RNG->CTRL = reg_val;
    RNG->SEED = cfg->seed;

    g_rcb.handler = handler;
    g_rcb.context = context;

    NVIC_ClearPendingIRQ(RNG_IRQn);
    NVIC_SetPriority(RNG_IRQn, cfg->irq_prio);
    NVIC_EnableIRQ(RNG_IRQn);

    return DRIVER_ERROR_OK;
}

int32_t HAL_RNG_Deinit(void)
{
    SCU->PER2 &= ~(1UL << SCU_PER2_RNG_Pos);
    SCU->PCER2 &= ~(1UL << SCU_PCER2_RNG_Pos);

    NVIC_DisableIRQ(RNG_IRQn);

    return DRIVER_ERROR_OK;
}

int32_t HAL_RNG_Generate(uint32_t *value, enum rng_io_type io_type)
{
    if (io_type == RNG_IO_TYPE_BLOCK)
    {
        uint32_t status;

        RNG->CTRL &= ~(1 << RNG_CTRL_EN_Pos);
        RNG->CTRL &= ~((1 << RNG_CTRL_ERRIE_Pos) | (1 << RNG_CTRL_RDYIE_Pos));
        RNG->CTRL |= (1 << RNG_CTRL_EN_Pos);

        while (1)
        {
            status = RNG->STAT;
            if (status & (RNG_STAT_RDY_Msk | RNG_STAT_ERR_Msk))
            {
                break;
            }
        }

        if (status & RNG_STAT_RDY_Msk)
        {
            *value = RNG->RNGD;
            return DRIVER_ERROR_OK;
        }
        else
        {
            return DRIVER_ERROR_HW;
        }
    }
    else
    {
        DRIVER_ASSERT(g_rcb.handler != NULL);

        g_rcb.value = value;
        RNG->CTRL &= ~(1 << RNG_CTRL_EN_Pos);
        RNG->CTRL |= ((1 << RNG_CTRL_ERRIE_Pos) | (1 << RNG_CTRL_RDYIE_Pos));
        RNG->CTRL |= (1 << RNG_CTRL_EN_Pos);

        return DRIVER_ERROR_OK;
    }
}

void RNG_IRQHandler(void)
{
    uint32_t status;

    status = RNG->STAT;
    /* clear interrupt */
    RNG->STAT = status;
	
    if (g_rcb.handler)
    {
        int32_t event;

        if (status & RNG_STAT_RDYI_Msk)
        {
            *g_rcb.value = RNG->RNGD;
            event = RNG_EVENT_DONE;
        }
        else
        {
            event = RNG_EVENT_ERROR;
        }
        g_rcb.handler(event, g_rcb.context);
    }
}

