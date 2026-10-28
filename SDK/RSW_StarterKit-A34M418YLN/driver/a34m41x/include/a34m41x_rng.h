/**
 *******************************************************************************
 * @file        a34m41x_rng.h
 * @author      ABOV R&D Division
 * @brief       RNG Header File
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
 * @defgroup    driver_rng RNG
 * @{
 * @brief       RNG driver
 */


#ifndef _A34M41X_RNG_H_
#define _A34M41X_RNG_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "a34m41x_driver_common.h"

/**
 * RNG IO type
 */
enum rng_io_type
{
    RNG_IO_TYPE_BLOCK       = 0,            /**< blocking */
    RNG_IO_TYPE_NON_BLOCK   = 1,            /**< non-blocking */
};

/**
 * RNG clock
 */
enum rng_clk
{
    RNG_CLK_OSC     = 0,                    /**< OSC, system clock must be set accordingly */
    RNG_CLK_LSIHSI  = 1,                    /**< LSI/HSI, system clock must be set accordingly */
};

/**
 * RNG event
 */
enum rng_event
{
    RNG_EVENT_DONE  = 0,                    /**< generation complete */
    RNG_EVENT_ERROR = 1,                    /**< generation error */
};

/**
 *******************************************************************************
 * @brief       RNG interrupt callback function type
 * @param[in]   event event type ::rng_event
 * @param[in]   context context provided during initialization
 ******************************************************************************/
typedef void (*rng_irq_handler_t)(uint32_t event, void *context);

/**
 * RNG configuration
 */
struct rng_cfg
{
    uint16_t gcp;                           /**< generation counter */
    enum rng_clk casr;                      /**< CASR clock */
    enum rng_clk lfsr;                      /**< LFSR clock */
    uint32_t seed;                          /**< seed value */
    uint8_t irq_prio;                       /**< interrupt priority */
};

/**
 *******************************************************************************
 * @brief       Initialize RNG.
 * @param[in]   cfg configuration
 * @param[in]   handler callback function to receive event
 * @param[in]   context caller's context
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_RNG_Init(struct rng_cfg *cfg, rng_irq_handler_t handler, void *context);

/**
 *******************************************************************************
 * @brief       Deinitialize RNG.
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_RNG_Deinit(void);

/**
 *******************************************************************************
 * @brief       Request to generate a random number
 * @param[out]  value value to get the random number
 * @param[in]   io_type operation type
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_RNG_Generate(uint32_t *value, enum rng_io_type io_type);

#ifdef __cplusplus
}
#endif

#endif /* _A34M41X_RNG_H_ */

/** @} */
/** @} */


