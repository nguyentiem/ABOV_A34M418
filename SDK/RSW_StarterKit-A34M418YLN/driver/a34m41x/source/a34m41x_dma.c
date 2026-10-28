/**
 *******************************************************************************
 * @file        a34m41x_dma.c
 * @author      ABOV R&D Division
 * @brief       Direct Memory Access
 *
 * Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "a34m41x.h"
#include "a34m41x_scu.h"
#include "a34m41x_dma.h"


#define DMA_REG_OFFSET  0x10

static __INLINE DMA_Type *DMA_REG(enum dma_ch ch)
{
    return (DMA_Type *)(DMA0_BASE + (DMA_REG_OFFSET * ch));
}

static uint8_t g_dma_ref_count = 0;
static uint8_t g_dma_setup[DMA_ID_MAX];

int32_t HAL_DMA_Init(void)
{
    if (g_dma_ref_count == DMA_ID_MAX)
    {
        return DRIVER_ERROR_NO_RESOURCE;
    }

    if (g_dma_ref_count == 0)
    {
        SCU->PER1 &= ~(1UL << SCU_PER1_DMA_Pos);
        SCU->PCER1 &= ~(1UL << SCU_PCER1_DMA_Pos);

        SCU->PER1 |= (1UL << SCU_PER1_DMA_Pos);
        SCU->PCER1 |= (1UL << SCU_PCER1_DMA_Pos);
    }

    g_dma_ref_count++;


    return DRIVER_ERROR_OK;
}

int32_t HAL_DMA_Deinit(void)
{
    if (g_dma_ref_count == 0)
    {
        return DRIVER_ERROR_NO_RESOURCE;
    }

    g_dma_ref_count--;

    if (g_dma_ref_count == 0)
    {
        SCU->PER1 &= ~(1UL << SCU_PER1_DMA_Pos);
        SCU->PCER1 &= ~(1UL << SCU_PCER1_DMA_Pos);
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_DMA_Setup(enum dma_ch ch, struct dma_cfg *cfg)
{
    DMA_Type *reg;

    DRIVER_ASSERT(ch < DMA_ID_MAX);

    if (g_dma_setup[ch] != 0)
    {
        return DRIVER_ERROR_BUSY;
    }

    reg = DMA_REG(ch);
    reg->CR = 0;
    reg->CR = ((cfg->peri << DMA_CR_PERISEL_Pos) & DMA_CR_PERISEL_Msk) |
              ((cfg->size << DMA_CR_SIZE_Pos) & DMA_CR_SIZE_Msk) |
              ((cfg->dir << DMA_CR_DIR_Pos) & DMA_CR_DIR_Msk);

    g_dma_setup[ch] = 1;

    return DRIVER_ERROR_OK;
}

int32_t HAL_DMA_Clear(enum dma_ch ch)
{
    DRIVER_ASSERT(ch < DMA_ID_MAX);

    g_dma_setup[ch] = 0;

    return DRIVER_ERROR_OK;
}

int32_t HAL_DMA_Start(enum dma_ch ch, uint32_t src, uint32_t dst, uint32_t len)
{
    DMA_Type *reg;

    DRIVER_ASSERT(ch < DMA_ID_MAX);
    DRIVER_ASSERT(((len << DMA_CR_TRANSCNT_Pos) & ~DMA_CR_TRANSCNT_Msk) == 0);

    reg = DMA_REG(ch);

    reg->CR &= ~DMA_CR_TRANSCNT_Msk;
    reg->CR |= (len << DMA_CR_TRANSCNT_Pos);

    if (((reg->CR & DMA_CR_DIR_Msk) >> DMA_CR_DIR_Pos) == DMA_DIR_MEM_TO_PERI)
    {
        reg->PAR = dst;
        reg->MAR = src;
    }
    else
    {
        reg->PAR = src;
        reg->MAR = dst;
    }

    reg->SR |= (1 << DMA_SR_DMAEN_Pos);

    return DRIVER_ERROR_OK;
}

int32_t HAL_DMA_Stop(enum dma_ch id)
{
    DMA_Type *reg;

    DRIVER_ASSERT(id < DMA_ID_MAX);

    reg = DMA_REG(id);
    reg->SR &= ~(1 << DMA_SR_DMAEN_Pos);

    return DRIVER_ERROR_OK;
}

