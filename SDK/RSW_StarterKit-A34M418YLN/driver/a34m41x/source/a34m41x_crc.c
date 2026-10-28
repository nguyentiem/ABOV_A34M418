/**
 *******************************************************************************
 * @file        a34m41x_crc.c
 * @author      ABOV R&D Division
 * @brief       Cyclic Redundancy Check
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
#include "a34m41x_crc.h"

struct crc_ctrl_block
{
    crc_irq_handler_t   handler;
    void *              context;
    uint32_t *          value;
    enum dma_ch         dma_ch;
    uint8_t             dma_enable;
};

static struct crc_ctrl_block g_ccb;

int32_t HAL_CRC_Init(struct crc_cfg *cfg, crc_irq_handler_t handler, void *context)
{
    uint32_t ctrl = 0;

    DRIVER_ASSERT(cfg != NULL);

    SCU->PER2 &= ~(1UL << SCU_PER2_CRC_Pos);
    SCU->PCER2 &= ~(1UL << SCU_PCER2_CRC_Pos);

    SCU->PER2 |= (1UL << SCU_PER2_CRC_Pos);
    SCU->PCER2 |= (1UL << SCU_PCER2_CRC_Pos);

    if (cfg->out_inv)
    {
        ctrl |= (1 << CRC_CTRL_OUT_INV_Pos);
    }

    if (cfg->out_rev)
    {
        ctrl |= (1 << CRC_CTRL_OUT_REV_Pos);
    }

    if (cfg->in_rev)
    {
        ctrl |= (0 << CRC_CTRL_IN_REV_Pos);	
    }
    else
    {
        ctrl |= (1 << CRC_CTRL_IN_REV_Pos);
    }

    /* enable DMA interrupt regardless of the user. No effect unless DMA enabled */
    ctrl |= (1 << CRC_CTRL_DMADINT_Pos);

    ctrl |= cfg->poly << 1;

    CRC->CTRL = ctrl;

    g_ccb.handler = handler;
    g_ccb.context = context;

    NVIC_ClearPendingIRQ(CRC_IRQn);
    NVIC_SetPriority(CRC_IRQn, cfg->irq_prio);
    NVIC_EnableIRQ(CRC_IRQn);

    return DRIVER_ERROR_OK;
}

int32_t HAL_CRC_Deinit(void)
{
    SCU->PER2 &= ~(1UL << SCU_PER2_CRC_Pos);
    SCU->PCER2 &= ~(1UL << SCU_PCER2_CRC_Pos);

    memset(&g_ccb, 0, sizeof(struct crc_ctrl_block));

    NVIC_DisableIRQ(CRC_IRQn);

    return DRIVER_ERROR_OK;
}

int32_t HAL_CRC_SetDMA(enum dma_ch ch, uint8_t enable)
{
    if (enable)
    {
        struct dma_cfg cfg =
        {
            .peri = DMA_PERI_CRC_TX,
            .size = DMA_BUS_SIZE_BYTE,
            .dir = DMA_DIR_MEM_TO_PERI,
        };

        if (g_ccb.dma_enable)
        {
            return DRIVER_ERROR_INVALID_USE;
        }

        g_ccb.dma_enable = 1;
        g_ccb.dma_ch = ch;

        HAL_DMA_Init();
        HAL_DMA_Setup(ch, &cfg);
    }
    else
    {
        if (!g_ccb.dma_enable)
        {
            return DRIVER_ERROR_INVALID_USE;
        }

        g_ccb.dma_enable = 0;
        HAL_DMA_Clear(g_ccb.dma_ch);
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_CRC_Compute(uint32_t init, uint8_t *data, uint32_t len, uint32_t *value)
{
    uint32_t i;

    CRC->INIT = init;
    CRC->CTRL &= ~(1 << CRC_CTRL_INIT_EN_Pos);
    CRC->CTRL |= (1 << CRC_CTRL_INIT_EN_Pos);

    if (g_ccb.dma_enable)
    {
        DRIVER_ASSERT(g_ccb.handler != NULL);

        g_ccb.value = value;
        HAL_DMA_Start(g_ccb.dma_ch, (uint32_t)data, (uint32_t)&CRC->IDR, len);
    }
    else
    {
        for (i = 0; i < len; i++)
        {
            CRC->IDR = data[i];
        }

        *value = CRC->ODR;
    }

    return DRIVER_ERROR_OK;
}

void CRC_IRQHandler(void)
{
    uint32_t status;
    uint32_t event;

    status = CRC->STAT;

    if (status & CRC_STAT_DMADINT_Msk)
    {
        *g_ccb.value = CRC->ODR;
        event = CRC_EVENT_DONE;
    }
    else
    {
        event = CRC_EVENT_ERROR;
    }

    if (g_ccb.handler)
    {
        g_ccb.handler(event, g_ccb.context);
    }

    CRC->STAT = status;
}

