/**
 *******************************************************************************
 * @file        a34m41x_aes128.c
 * @author      ABOV R&D Division
 * @brief       Advanced Encryption Standard-128
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
#include "a34m41x_aes128.h"


#define AES128_MODE_CIPHER      1
#define AES128_MODE_INV_CIPHER  2

struct aes128_ctrl_block
{
    aes128_irq_handler_t    handler;
    void *                  context;
    uint8_t *               out;
    uint8_t                 busy;
    uint8_t                 tx_dma_enable;
    enum dma_ch             tx_dma_ch;
    uint8_t                 rx_dma_enable;
    enum dma_ch             rx_dma_ch;
};

static struct aes128_ctrl_block g_acb;

/* return 32bit value from 8bit values */
static __INLINE uint32_t aes_value32(uint8_t *value8)
{
    return (value8[0] << 24) | \
           (value8[1] << 16) | \
           (value8[2] << 8) |  \
           (value8[3] << 0);
}

/* fill in 8bit values from 32bit value */
static __INLINE void aes_value8(uint8_t *value8, uint32_t value32)
{
    value8[0] = value32 >> 24;
    value8[1] = value32 >> 16;
    value8[2] = value32 >> 8;
    value8[3] = value32 >> 0;
}

int32_t HAL_AES128_Init(struct aes128_cfg *cfg,
                        aes128_irq_handler_t handler,
                        void *context)
{
    struct aes128_ctrl_block *acb;

    DRIVER_ASSERT(cfg != NULL);

    SCU->PER2 &= ~(1UL << SCU_PER2_AES_Pos);
    SCU->PCER2 &= ~(1UL << SCU_PER2_AES_Pos);

    SCU->PER2 |= (1UL << SCU_PER2_AES_Pos);
    SCU->PCER2 |= (1UL << SCU_PER2_AES_Pos);

    AES128->CTRL &= ~(AES128_CTRL_TOINVMD_Msk | AES128_CTRL_TIINVMD_Msk);
    AES128->CTRL |= (cfg->out_order << AES128_CTRL_TOINVMD_Pos) | \
                    (cfg->in_order << AES128_CTRL_TIINVMD_Pos);

    acb = &g_acb;

    memset(acb, 0, sizeof(struct aes128_ctrl_block));

    acb->handler = handler;
    acb->context = context;

    NVIC_ClearPendingIRQ(AES128_IRQn);
    NVIC_SetPriority(AES128_IRQn, cfg->irq_prio);
    NVIC_EnableIRQ(AES128_IRQn);

    return DRIVER_ERROR_OK;
}

int32_t HAL_AES128_Deinit(void)
{
    SCU->PER2 &= ~(1UL << SCU_PER2_AES_Pos);
    SCU->PCER2 &= ~(1UL << SCU_PER2_AES_Pos);
	
    NVIC_DisableIRQ(AES128_IRQn);

    return DRIVER_ERROR_OK;
}

int32_t HAL_AES128_SetTransmitDMA(enum dma_ch ch, uint8_t enable)
{
    struct aes128_ctrl_block *acb = &g_acb;

    if (enable)
    {
        struct dma_cfg cfg =
        {
            .peri = DMA_PERI_AES128_TX,
            .size = DMA_BUS_SIZE_WORD,
            .dir = DMA_DIR_MEM_TO_PERI,
        };

        if (acb->tx_dma_enable)
        {
            return DRIVER_ERROR_INVALID_USE;
        }

        acb->tx_dma_enable = 1;
        acb->tx_dma_ch = ch;

        HAL_DMA_Init();
        HAL_DMA_Setup(ch, &cfg);
    }
    else
    {
        if (!acb->tx_dma_enable)
        {
            return DRIVER_ERROR_INVALID_USE;
        }

        acb->tx_dma_enable = 0;
        HAL_DMA_Clear(acb->tx_dma_ch);
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_AES128_SetReceiveDMA(enum dma_ch ch, uint8_t enable)
{
    struct aes128_ctrl_block *acb = &g_acb;

    if (enable)
    {
        struct dma_cfg cfg =
        {
            .peri = DMA_PERI_AES128_RX,
            .size = DMA_BUS_SIZE_WORD,
            .dir = DMA_DIR_PERI_TO_MEM,
        };

        if (acb->rx_dma_enable)
        {
            return DRIVER_ERROR_INVALID_USE;
        }

        acb->rx_dma_enable = 1;
        acb->rx_dma_ch = ch;

        HAL_DMA_Init();
        HAL_DMA_Setup(ch, &cfg);
    }
    else
    {
        if (!acb->rx_dma_enable)
        {
            return DRIVER_ERROR_INVALID_USE;
        }

        acb->rx_dma_enable = 0;
        HAL_DMA_Clear(acb->rx_dma_ch);
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_AES128_SetOrder(enum aes128_order in_order, enum aes128_order out_order)
{
    AES128->CTRL &= ~(AES128_CTRL_TOINVMD_Msk | AES128_CTRL_TIINVMD_Msk);
    AES128->CTRL |= (in_order << AES128_CTRL_TOINVMD_Pos) | \
                    (out_order << AES128_CTRL_TIINVMD_Pos);

    return DRIVER_ERROR_OK;
}

int32_t HAL_AES128_SetKey(uint8_t *key)
{
    AES128->KEYIN0 = aes_value32(&key[12]);
    AES128->KEYIN1 = aes_value32(&key[8]);
    AES128->KEYIN2 = aes_value32(&key[4]);
    AES128->KEYIN3 = aes_value32(&key[0]);

    return DRIVER_ERROR_OK;
}

int32_t HAL_AES128_Encrypt(uint8_t *out, uint8_t *in, enum aes128_io_type io_type)
{
    struct aes128_ctrl_block *acb = &g_acb;

    if (acb->busy)
    {
        return DRIVER_ERROR_BUSY;
    }

    acb->busy = 1;

    AES128->CTRL &= ~AES128_CTRL_MODE_Msk;
    AES128->CTRL |= (AES128_MODE_CIPHER & 0x03);

    AES128->CTRL |= (1 << AES128_CTRL_INFIFOFLUSH_Pos);
    AES128->CTRL |= (1 << AES128_CTRL_OUTFIFOFLUSH_Pos);

    if (io_type == AES128_IO_TYPE_BLOCK)
    {
        AES128->CTRL &= ~(1 << AES128_CTRL_CIPHERIE_Pos);
        AES128->INFIFO = aes_value32(&in[12]);
        AES128->INFIFO = aes_value32(&in[8]);
        AES128->INFIFO = aes_value32(&in[4]);
        AES128->INFIFO = aes_value32(&in[0]);

        while (1)	// kyi
        {
            if (AES128->STAT & AES128_STAT_CIPDRS_Msk)
            {
                break;
            }
        }
        aes_value8(&out[12], AES128->TEXTOUT0);
        aes_value8(&out[8], AES128->TEXTOUT1);
        aes_value8(&out[4], AES128->TEXTOUT2);
        aes_value8(&out[0], AES128->TEXTOUT3);

		// kyi - add clear CIPDRS

        acb->busy = 0;
    }
    else
    {
        DRIVER_ASSERT(acb->handler != NULL);

        if (acb->rx_dma_enable)
        {
            HAL_DMA_Start(acb->rx_dma_ch, (uint32_t)&AES128->OUTFIFO, (uint32_t)out, 4);
        }
        else
        {
            acb->out = out;
        }

        AES128->CTRL |= (1 << AES128_CTRL_CIPHERIE_Pos);

        if (acb->tx_dma_enable)
        {
            HAL_DMA_Start(acb->tx_dma_ch, (uint32_t)in, (uint32_t)&AES128->INFIFO, 4);
        }
        else
        {
            AES128->INFIFO = aes_value32(&in[12]);
            AES128->INFIFO = aes_value32(&in[8]);
            AES128->INFIFO = aes_value32(&in[4]);
            AES128->INFIFO = aes_value32(&in[0]);
        }
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_AES128_Decrypt(uint8_t *out, uint8_t *in, enum aes128_io_type io_type)
{
    struct aes128_ctrl_block *acb = &g_acb;

    if (acb->busy)
    {
        return DRIVER_ERROR_BUSY;
    }

    acb->busy = 1;

    AES128->CTRL &= ~AES128_CTRL_MODE_Msk;
    AES128->CTRL |= (AES128_MODE_INV_CIPHER & 0x03);

    AES128->CTRL |= (1 << AES128_CTRL_INFIFOFLUSH_Pos);
    AES128->CTRL |= (1 << AES128_CTRL_OUTFIFOFLUSH_Pos);

    if (io_type == AES128_IO_TYPE_BLOCK)
    {
        AES128->CTRL &= ~(1 << AES128_CTRL_INVCIPHERIE_Pos);
        AES128->INFIFO = aes_value32(&in[12]);
        AES128->INFIFO = aes_value32(&in[8]);
        AES128->INFIFO = aes_value32(&in[4]);
        AES128->INFIFO = aes_value32(&in[0]);

        while (1)	// kyi
        {
            if (AES128->STAT & AES128_STAT_INVCIPDRS_Msk)
            {
                break;
            }
        }
        aes_value8(&out[12], AES128->TEXTOUT0);
        aes_value8(&out[8], AES128->TEXTOUT1);
        aes_value8(&out[4], AES128->TEXTOUT2);
        aes_value8(&out[0], AES128->TEXTOUT3);

		// kyi - add clear INVCIPDRS

        acb->busy = 0;
    }
    else
    {
        DRIVER_ASSERT(acb->handler != NULL);

        if (acb->rx_dma_enable)
        {
            HAL_DMA_Start(acb->rx_dma_ch, (uint32_t)&AES128->OUTFIFO, (uint32_t)out, 4);
        }
        else
        {
            acb->out = out;
        }

        AES128->CTRL |= (1 << AES128_CTRL_INVCIPHERIE_Pos);

        if (acb->tx_dma_enable)
        {
            HAL_DMA_Start(acb->tx_dma_ch, (uint32_t)in, (uint32_t)&AES128->INFIFO, 4);
        }
        else
        {
            AES128->INFIFO = aes_value32(&in[12]);
            AES128->INFIFO = aes_value32(&in[8]);
            AES128->INFIFO = aes_value32(&in[4]);
            AES128->INFIFO = aes_value32(&in[0]);
        }
    }

    return DRIVER_ERROR_OK;
}

void AES128_IRQHandler(void)
{
    struct aes128_ctrl_block *acb;
    uint32_t status;

    status = AES128->STAT;

    acb = &g_acb;

    if (acb->handler)
    {
        if (!acb->rx_dma_enable)
        {
            aes_value8(&acb->out[12], AES128->TEXTOUT0);
            aes_value8(&acb->out[8], AES128->TEXTOUT1);
            aes_value8(&acb->out[4], AES128->TEXTOUT2);
            aes_value8(&acb->out[0], AES128->TEXTOUT3);
        }

        if (AES128->STAT & AES128_STAT_CIPDRS_Msk)
        {
            acb->handler(AES128_EVENT_ENCRYPT_DONE, acb->context);
        }
        else if (AES128->STAT & AES128_STAT_INVCIPDRS_Msk)
        {
            acb->handler(AES128_EVENT_DECRYPT_DONE, acb->context);
        }
    }

    AES128->STAT = status | AES128_STAT_CIPDONE_Msk | AES128_STAT_INVCIPDONE_Msk;

    acb->busy = 0;
}

