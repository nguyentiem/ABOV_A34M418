/**
 *******************************************************************************
 * @file        a34m41x_spi.h
 * @author      ABOV R&D Division
 * @brief       SPI Header File
 *
 * Copyright 2023 ABOV Semiconductor Co.,Ltd. All rights reserved.
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
 * @defgroup    driver_spi SPI
 * @{
 * @brief       SPI driver
 */

#ifndef _A34M41X_SPI_H_
#define _A34M41X_SPI_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <string.h>
#include "a34m41x_driver_common.h"
#include "a34m41x_dma.h"

/**
 * SPI instance id
 */
enum spi_id
{
    SPI_ID_0    = 0,                        /**< SPI 0 */
    SPI_ID_1    = 1,                        /**< SPI 1 */
    SPI_ID_2    = 2,                        /**< SPI 2 */
    SPI_ID_MAX  = 3,                        /**< SPI max */
};

/**
 * SPI master clock frequency
 */
enum spi_freq
{
    SPI_FREQ_4MHZ   = 30,                    /**< frequency 4 MHz */
    SPI_FREQ_2MHZ   = 60,                   /**< frequency 2 MHz */
    SPI_FREQ_1MHZ   = 120,                   /**< frequency 1 MHz */
    SPI_FREQ_500KHZ = 240,                   /**< frequency 500 KHz */
    SPI_FREQ_250KHZ = 480,                   /**< frequency 250 KHz */
    SPI_FREQ_125KHZ = 960,                  /**< frequency 125 KHz */
};

/**
 * SPI bit order
 */
enum spi_bit_order
{
    SPI_BIT_ORDER_MSB_FIRST = 0,            /**< use MSB first */
    SPI_BIT_ORDER_LSB_FIRST = 1,            /**< use LSB first */
};

/**
 * SPI mode
 */
enum spi_mode
{
    SPI_MODE_MASTER = 0,                    /**< master mode */
    SPI_MODE_SLAVE  = 1,                    /**< slave mode */
};

/**
 * SPI clock and data sync mode
 */
enum spi_sync_mode
{
    SPI_SYHC_MODE_CPOL_0_CPHA_0 = 0,        /**< polarity 0, phase 0 */
    SPI_SYHC_MODE_CPOL_0_CPHA_1 = 1,        /**< polarity 0, phase 1 */
    SPI_SYHC_MODE_CPOL_1_CPHA_0 = 2,        /**< polarity 1, phase 0 */
    SPI_SYHC_MODE_CPOL_1_CPHA_1 = 3,        /**< polarity 1, phase 1 */
};

/**
 * SPI data bit
 */
enum spi_data_bit
{
    SPI_DATA_BIT_8  = 0,                    /**< 8 bit mode */
    SPI_DATA_BIT_9  = 1,                    /**< 9 bit mode */
    SPI_DATA_BIT_16 = 2,                    /**< 16 bit mode */
    SPI_DATA_BIT_17 = 3,                    /**< 17 bit mode */
};

/**
 * SPI slave select type
 */
enum spi_ss_type
{
    SPI_SS_AUTO     = 0,                    /**< automatic slave select signal */
    SPI_SS_MANUAL   = 1,                    /**< manual slave select signal */
};

/**
 * SPI slave select state
 */
enum ss_state
{
    SPI_SS_STATE_LOW    = 0,                /**< slave select low */
    SPI_SS_STATE_HIGH   = 1,                /**< slave select high */
};

/**
 * SPI IO type
 */
enum spi_io_type
{
    SPI_IO_TYPE_BLOCK       = 0,            /**< blocking */
    SPI_IO_TYPE_NON_BLOCK   = 1,            /**< non blocking */
};

/**
 * SPI event
 */
enum spi_event
{
    SPI_EVENT_TRANSFER_DONE = BIT(0),       /**< transfer complete */
    SPI_EVENT_OVERRUN       = BIT(1),       /**< overrun error */
    SPI_EVENT_UNDERRUN      = BIT(2),       /**< underrun error */
};

/**
 *******************************************************************************
 * @brief       SPI interrupt callback function type
 * @param[in]   event event type ::spi_event
 * @param[in]   context context provided during initialization
 ******************************************************************************/
typedef void (*spi_irq_handler_t)(uint32_t event, void *context);

/**
 * SPI configuration
 */
struct spi_cfg
{
    enum spi_mode       mode;               /**< mode */
    enum spi_freq       freq;               /**< frequency */
    enum spi_bit_order  bit_order;          /**< bit oder */
    enum spi_sync_mode  sync_mode;          /**< sync mode */
    enum spi_data_bit   data_bit;           /**< data bit */
    enum spi_ss_type    ss_type;            /**< slave selection type */
    uint8_t             irq_prio;           /**< interrupt priority */
};

/**
 *******************************************************************************
 * @brief       Initialize.
 * @param[in]   id instance id
 * @param[in]   cfg configuration
 * @param[in]   handler callback function to receive event
 * @param[in]   context caller's context
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SPI_Init(enum spi_id id, struct spi_cfg *cfg,
                     spi_irq_handler_t handler,
                     void *context);

/**
 *******************************************************************************
 * @brief       Deintialize.
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SPI_Deinit(enum spi_id id);

/**
 *******************************************************************************
 * @brief       Sonfigure SPI delay
 *              For operation using flash memory data, it is recommended to
 *              use some delay, epecially for high speed SPI clock configuration.
 * @param[in]   id instance id
 * @param[in]   start start delay
 * @param[in]   burst burst delay
 * @param[in]   stop stop delay
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SPI_SetDelay(enum spi_id id, uint8_t start, uint8_t burst, uint8_t stop);

/**
 *******************************************************************************
 * @brief       Control SS Output in SPI peripheral
 * @param[in]   id instance id
 * @param[in]   ss slave selection state
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SPI_SSManual(enum spi_id id, enum ss_state ss);

/**
 *******************************************************************************
 * @brief       Trasnfer data.
 *              If io_type is nonblocking, the function returns immediately.
 *              Completion is signalled by the interrupt handler.
 *              If io_type is blocking, the function waits until the requested
 *              amount of the data is received.
 * @param[in]   id instance id
 * @param[in]   out data to transmit from
 * @param[in]   out_len length of data
 * @param[out]  in data to receive to
 * @param[in]   in_len len length of data
 * @param[in]   io_type operation type
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SPI_Transfer(enum spi_id id,
                         uint8_t *out, uint32_t out_len,
                         uint8_t *in, uint32_t in_len,
                         enum spi_io_type io_type);

/**
 *******************************************************************************
 * @brief       Transmit data.
 *              Data should not be changed until trasmittion is complete.
 *              If io_type is nonblocking, the function returns immediately.
 *              Completion is signalled by the interrupt handler.
 *              If io_type is blocking, the function waits until the requested
 *              amount of the data is transmitted.
 * @param[in]   id instance id
 * @param[in]   out data to transmit from
 * @param[in]   len length of data
 * @param[in]   io_type operation type
 * @return      driver error code
 ******************************************************************************/
static __INLINE int32_t HAL_SPI_Transmit(enum spi_id id, uint8_t *out, uint32_t len,
                                         enum spi_io_type io_type)
{
    return HAL_SPI_Transfer(id, out, len, NULL, 0, io_type);
}

/**
 *******************************************************************************
 * @brief       Receive data.
 *              If io_type is nonblocking, the function returns immediately.
 *              Completion is signalled by the interrupt handler.
 *              If io_type is blocking, the function waits until the requested
 *              amount of the data is received.
 * @param[in]   id instance id
 * @param[out]  in data to receive to
 * @param[in]   len length of data
 * @param[in]   io_type operation type
 * @return      driver error code
 ******************************************************************************/
static __INLINE int32_t HAL_SPI_Receive(enum spi_id id, uint8_t *in, uint32_t len,
                                        enum spi_io_type io_type)
{
    return HAL_SPI_Transfer(id, NULL, 0, in, len, io_type);
}

/**
 *******************************************************************************
 * @brief       DMA control for SPI transmit.
 * @param[in]   id instance id
 * @param[in]   ch dma channel
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SPI_SetTransmitDMA(enum spi_id id, enum dma_ch ch, uint8_t enable);

/**
 *******************************************************************************
 * @brief       DMA control for SPI receive.
 * @param[in]   id instance id
 * @param[in]   ch dma channel
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SPI_SetReceiveDMA(enum spi_id id, enum dma_ch ch, uint8_t enable);

#ifdef __cplusplus
}
#endif

#endif /* _A34M41X_SPI_H_ */

/** @} */
/** @} */

