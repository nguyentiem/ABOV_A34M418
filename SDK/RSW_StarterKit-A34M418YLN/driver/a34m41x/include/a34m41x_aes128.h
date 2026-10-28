/**
 *******************************************************************************
 * @file        a34m41x_aes128.h
 * @author      ABOV R&D Division
 * @brief       AES-128 Header File
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
 * @defgroup    driver_aes128 AES128
 * @{
 * @brief       AES128 driver
 */

#ifndef _A34M41X_AES128_H_
#define _A34M41X_AES128_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "a34m41x_driver_common.h"
#include "a34m41x_dma.h"

/**
 * AES128 byte order
 */
enum aes128_order
{
    AES128_ORDER_LSB_WORD           = 0,    /**< byte order LSB word */
    AES128_ORDER_MSB_WORD           = 1,    /**< byte order MSB word */
    AES128_ORDER_MSB_BYTE_IN_WORD   = 2,    /**< byte order MSB byte in word */
    AES128_ORDER_MSB_BYTE           = 4,    /**< byte order MSB byte */
};

/**
 * AES128 IO type
 */
enum aes128_io_type
{
    AES128_IO_TYPE_BLOCK        = 0,        /**< blocking */
    AES128_IO_TYPE_NON_BLOCK    = 1,        /**< non blocking */
};

/**
 * AES128 event
 */
enum aes128_event
{
    AES128_EVENT_ENCRYPT_DONE   = 0,        /**< encrypt complete */
    AES128_EVENT_DECRYPT_DONE   = 1,        /**< decrypt complete */
};

/**
 *******************************************************************************
 * @brief       AES128 interrupt callback function type
 * @param[in]   event event type ::aes128_event
 * @param[in]   context context provided during initialization
 ******************************************************************************/
typedef void (*aes128_irq_handler_t)(uint32_t event, void *context);

/**
 * AES128 configuration
 */
struct aes128_cfg
{
    enum aes128_order   out_order;          /**< output order */
    enum aes128_order   in_order;           /**< input order */
    uint32_t            irq_prio;           /**< interrupt priority */
};

/**
 *******************************************************************************
 * @brief       AES Initialize.
 * @param[in]   cfg configuration
 * @param[in]   handler callback function to receive event
 * @param[in]   context caller's context
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_AES128_Init(struct aes128_cfg *cfg,
                        aes128_irq_handler_t handler,
                        void *context);

/**
 *******************************************************************************
 * @brief       AES Deinitialize.
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_AES128_Deinit(void);

/**
 *******************************************************************************
 * @brief       AES transmit DMA control
 * @param[in]   ch dma channel
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_AES128_SetTransmitDMA(enum dma_ch ch, uint8_t enable);

/**
 *******************************************************************************
 * @brief       AES receive DMA control
 * @param[in]   ch dma channel
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_AES128_SetReceiveDMA(enum dma_ch ch, uint8_t enable);

/**
 *******************************************************************************
 * @brief       AES Set key.
 * @param[in]   key key to be used (16bytes)
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_AES128_SetKey(uint8_t *key);

/**
 *******************************************************************************
 * @brief       AES Set byte order.
 *              It allows changing byte order after initialization.
 * @param[in]   in_order order of the key for input
 * @param[in]   out_order order of the key for output
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_AES128_SetOrder(enum aes128_order in_order, enum aes128_order out_order);

/**
 *******************************************************************************
 * @brief       AES Encrypt
 *              If io_type is nonblocking, the function returns immediately.
 *              Completion is signalled by the interrupt handler.
 *              If io_type is blocking, the function waits until
 *              encryption is complete.
 * @param[in]   out output to where encrypted data is to be stored (16bytes)
 * @param[in]   in input data to encrypt (16bytes)
 * @param[in]   io_type operation type
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_AES128_Encrypt(uint8_t *out, uint8_t *in, enum aes128_io_type io_type);

/**
 *******************************************************************************
 * @brief       AES Decrypt
 *              If io_type is nonblocking, the function returns immediately.
 *              Completion is signalled by the interrupt handler.
 *              If io_type is blocking, the function waits until
 *              decryption is complete.
 * @param[in]   out output to where decrypted data is to be stored (16bytes)
 * @param[in]   in input data to decrypt (16bytes)
 * @param[in]   io_type operation type
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_AES128_Decrypt(uint8_t *out, uint8_t *in, enum aes128_io_type io_type);

#ifdef __cplusplus
}
#endif

#endif /* _A34M41X_AES128_H_ */

/** @} */
/** @} */

