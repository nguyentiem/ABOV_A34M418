/**
 *******************************************************************************
 * @file        a34m41x_pga.c
 * @author      ABOV R&D Division
 * @brief       Programmable Gain AMP
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
#include "a34m41x_pga.h"


#define PGA_REG_OFFSET  0x0004

static __INLINE PGA_Type *PGA_REG(enum pga_id id)
{
    return (PGA_Type *)(PGA0_BASE + (PGA_REG_OFFSET * id));
}

static uint8_t g_pga_ref_count = 0;
static uint8_t g_pga_setup[PGA_ID_MAX];

int32_t HAL_PGA_Init(enum pga_id id, struct pga_cfg *cfg)
{
    PGA_Type *pga;

    DRIVER_ASSERT(id < PGA_ID_MAX);
    DRIVER_ASSERT(cfg != NULL);

    if (g_pga_setup[id] == 1)
    {
        return DRIVER_ERROR_NO_RESOURCE;
    }

    if (g_pga_ref_count == 0)
    {
        SCU->PER2 &= ~(1UL << SCU_PER2_PGA_Pos);
        SCU->PCER2 &= ~(1UL << SCU_PCER2_PGA_Pos);

        SCU->PER2 |= (1UL << SCU_PER2_PGA_Pos);
        SCU->PCER2 |= (1UL << SCU_PCER2_PGA_Pos);
    }

    g_pga_ref_count++;
    g_pga_setup[id] = 1;

    pga = PGA_REG(id);
    pga->CR = (cfg->ampi << PGA_CR_AMPISEL_Pos) | \
              (cfg->gain << PGA_CR_GAINSEL_Pos) | \
              (cfg->ugain << PGA_CR_UGAINEN_Pos);

    return DRIVER_ERROR_OK;
}

int32_t HAL_PGA_Deinit(enum pga_id id)
{
    PGA_Type *pga;

    DRIVER_ASSERT(id < PGA_ID_MAX);

    if (g_pga_setup[id] == 0)
    {
        return DRIVER_ERROR_NO_RESOURCE;
    }

    pga = PGA_REG(id);
    pga->CR &= ~(1UL << PGA_CR_AMPEN_Pos);

    g_pga_setup[id] = 0;
    g_pga_ref_count--;

    if (g_pga_ref_count == 0)
    {
        SCU->PER2 &= ~(1UL << SCU_PER2_PGA_Pos);
        SCU->PCER2 &= ~(1UL << SCU_PCER2_PGA_Pos);
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_PGA_Start(enum pga_id id)
{
    PGA_Type *pga;

    DRIVER_ASSERT(id < PGA_ID_MAX);

    pga = PGA_REG(id);
    pga->CR &= ~(1UL << PGA_CR_AMPEN_Pos);
    pga->CR |= (1UL << PGA_CR_AMPEN_Pos);

    return DRIVER_ERROR_OK;
}

int32_t HAL_PGA_Stop(enum pga_id id)
{
    PGA_Type *pga;

    DRIVER_ASSERT(id < PGA_ID_MAX);

    pga = PGA_REG(id);
    pga->CR &= ~(1UL << PGA_CR_AMPEN_Pos);

    return DRIVER_ERROR_OK;
}

