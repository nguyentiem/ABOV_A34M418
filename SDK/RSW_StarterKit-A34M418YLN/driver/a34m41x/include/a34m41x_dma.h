/**
 *******************************************************************************
 * @file        a34m41x_dma.h
 * @author      ABOV R&D Division
 * @brief       DMA Header File
 *
 * Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/

/**
 * @addtogroup  driver
 * @{
 * @defgroup    driver_dma DMA
 * @{
 * @brief       DMA driver
 */


#ifndef _A34M41X_DMA_H_
#define _A34M41X_DMA_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "a34m41x_driver_common.h"

/**
 * DMA id
 */
enum dma_ch
{
    DMA_ID_0    = 0,                        /**< DMA 0 */
    DMA_ID_1    = 1,                        /**< DMA 1 */
    DMA_ID_2    = 2,                        /**< DMA 2 */
    DMA_ID_3    = 3,                        /**< DMA 3 */
    DMA_ID_4    = 4,                        /**< DMA 4 */
    DMA_ID_5    = 5,                        /**< DMA 5 */
    DMA_ID_6    = 6,                        /**< DMA 6 */
    DMA_ID_7    = 7,                        /**< DMA 7 */
    DMA_ID_8    = 8,                        /**< DMA 8 */
    DMA_ID_9    = 9,                        /**< DMA 9 */
    DMA_ID_10   = 10,                       /**< DMA 10 */
    DMA_ID_11   = 11,                       /**< DMA 11 */
    DMA_ID_12   = 12,                       /**< DMA 12 */
    DMA_ID_13   = 13,                       /**< DMA 13 */
    DMA_ID_14   = 14,                       /**< DMA 14 */
    DMA_ID_15   = 15,                       /**< DMA 15 */
    DMA_ID_MAX  = 16,                       /**< DMA max */
};

/**
 * DMA peripheral id
 */
enum dma_peri
{
    DMA_PERI_NONE       = 0,                /**< peripheral id none */
    DMA_PERI_UART0_RX   = 1,                /**< peripheral id UART0 RX */
    DMA_PERI_UART0_TX   = 2,                /**< peripheral id UART0 TX */
    DMA_PERI_UART1_RX   = 3,                /**< peripheral id UART1 RX */
    DMA_PERI_UART1_TX   = 4,                /**< peripheral id UART1 TX */
    DMA_PERI_UART2_RX   = 5,                /**< peripheral id UART2 RX */
    DMA_PERI_UART2_TX   = 6,                /**< peripheral id UART2 TX */
    DMA_PERI_UART3_RX   = 7,                /**< peripheral id UART3 RX */
    DMA_PERI_UART3_TX   = 8,                /**< peripheral id UART3 TX */
    DMA_PERI_UART4_RX   = 9,                /**< peripheral id UART4 RX */
    DMA_PERI_UART4_TX   = 10,               /**< peripheral id UART4 TX */
    DMA_PERI_UART5_RX   = 11,               /**< peripheral id UART5 RX */
    DMA_PERI_UART5_TX   = 12,               /**< peripheral id UART5 TX */
    DMA_PERI_SPI0_RX    = 13,               /**< peripheral id SPI0 RX */
    DMA_PERI_SPI0_TX    = 14,               /**< peripheral id SPI0 TX */
    DMA_PERI_SPI1_RX    = 15,               /**< peripheral id SPI1 RX */
    DMA_PERI_SPI1_TX    = 16,               /**< peripheral id SPI1 TX */
    DMA_PERI_SPI2_RX    = 17,               /**< peripheral id SPI2 RX */
    DMA_PERI_SPI2_TX    = 18,               /**< peripheral id SPI2 TX */
    DMA_PERI_ADC0_RX    = 19,               /**< peripheral id ADC0 RX */
    DMA_PERI_ADC1_RX    = 20,               /**< peripheral id ADC1 RX */
    DMA_PERI_ADC2_RX    = 21,               /**< peripheral id ADC2 RX */
    DMA_PERI_AES128_RX  = 22,               /**< peripheral id AES128 RX */
    DMA_PERI_AES128_TX  = 23,               /**< peripheral id AES128 TX */
    DMA_PERI_CRC_TX     = 24,               /**< peripheral id CRC TX */
};

/**
 * DMA source transfer width and destination transfer width
 */
enum dma_bus_size
{
    DMA_BUS_SIZE_BYTE       = (0UL),        /**< size = 1 byte */
    DMA_BUS_SIZE_HALFWORD   = (1UL),        /**< size = 2 bytes */
    DMA_BUS_SIZE_WORD       = (2UL),        /**< size = 4 bytes */
};

/**
 * DMA address direction
 */
enum dma_dir
{
    DMA_DIR_MEM_TO_PERI = 0,                /**< memory to peripheral */
    DMA_DIR_PERI_TO_MEM = 1,                /**< peripheral to memory */
};

/**
 * DMA event
 */
enum dma_event
{
    DMA_EVENT_DONE  = 0,                    /**< generation complete */
    DMA_EVENT_ERROR = 1,                    /**< generation error */
};

/**
 * DMA configuration
 */

struct dma_cfg
{
    enum dma_peri       peri;               /**< peripheral id */
    enum dma_bus_size   size;               /**< bus size */
    enum dma_dir        dir;                /**< direction */
};


/**
 *******************************************************************************
 * @brief       Initialize DMA.
 *              DMA may be used by multiple peripherals, and initialization
 *              will enable DMA only once for the first time.
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_DMA_Init(void);

/**
 *******************************************************************************
 * @brief       Deinitialize DMA.
 *              DMA may be used by multiple peripherals, and deinitialzation
 *              will disable DMA only if no other peripherals are in use of DMA.
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_DMA_Deinit(void);

/**
 *******************************************************************************
 * @brief       Setup DMA channel.
 * @param[in]   ch DMA channel
 * @param[in]   cfg configuration
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_DMA_Setup(enum dma_ch ch, struct dma_cfg *cfg);

/**
 *******************************************************************************
 * @brief       Clear DMA channel.
 * @param[in]   ch DMA channel
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_DMA_Clear(enum dma_ch ch);

/**
 *******************************************************************************
 * @brief       Start DMA.
 * @param[in]   ch DMA channel
 * @param[in]   src source address for the DMA transfer
 * @param[in]   dst destination address for the DMA transfer
 * @param[in]   len length of the DMA transfer, should be less than 0xFFF
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_DMA_Start(enum dma_ch ch, uint32_t src, uint32_t dst, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* _A34M41X_DMA_H_ */

/** @} */
/** @} */


