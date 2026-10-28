/**
 *******************************************************************************
 * @file        a34m41x_wdt.h
 * @author      ABOV R&D Division
 * @brief       Watchdog Timer Header File
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
 * @defgroup    driver_wdt WDT
 * @{
 * @brief       WDT driver
 */


#ifndef _A34M41X_WDT_H_
#define _A34M41X_WDT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "a34m41x_driver_common.h"

/**
 * WDT clock configuration
 */
enum wdt_clk_src
{
    WDT_CLK_SRC_PCLK,                       /**< PCLK as clock source */
    WDT_CLK_SRC_EXTERNAL,                   /**< external clock source */
};

/**
 * WDT clock divider configuration
 */
enum wdt_clk_div
{
    WDT_CLK_DIV_1,                          /**< clock divided by 1 */
    WDT_CLK_DIV_4,                          /**< clock divided by 4 */
    WDT_CLK_DIV_8,                          /**< clock divided by 8 */
    WDT_CLK_DIV_16,                         /**< clock divided by 16 */
    WDT_CLK_DIV_32,                         /**< clock divided by 32 */
    WDT_CLK_DIV_64,                         /**< clock divided by 64 */
    WDT_CLK_DIV_128,                        /**< clock divided by 128 */
    WDT_CLK_DIV_256,                        /**< clock divided by 256 */
};

/**
 * WDT mode configuration
 */
enum wdt_mode
{
    WDT_MODE_RESET,                         /**< reset on expiration */
    WDT_MODE_IRQ,                           /**< interrupt on expiration */
};

/**
 * WDT driver configuration
 */
struct wdt_cfg
{
    enum wdt_clk_src    clk_src;            /**< clock source */
    enum wdt_clk_div    clk_div;            /**< clock divider */
    enum wdt_mode       mode;               /**< mode */
    uint8_t             irq_prio;           /**< interrupt priority */
};

/**
 *******************************************************************************
 * @brief       WDT interrupt callback function type
 * @param[in]   stacked_regs stacked register values during exception
 * @param[in]   context context provided during initialization
 ******************************************************************************/
typedef void (*wdt_irq_handler_t)(unsigned long *stacked_regs, void *context);

/**
 *******************************************************************************
 * @brief       Initialize WDT
 * @param[in]   cfg set of WDT configuration
 * @param[in]   handler callback function to receive event
 * @param[in]   context caller's context
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_WDT_Init(struct wdt_cfg *cfg, wdt_irq_handler_t handler, void *context);

/**
 *******************************************************************************
 * @brief       Deinitialize WDT
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_WDT_Deinit(void);

/**
 *******************************************************************************
 * @brief       Start WDT with initial counter.
 *              counter value is in unit of WDT source clock.
 * @param[in]   counter starting counter, re-used when ::HAL_WDT_Feed is called.
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_WDT_Start(uint32_t counter);

/**
 *******************************************************************************
 * @brief       Stop WDT.
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_WDT_Stop(void);

/**
 *******************************************************************************
 * @brief       Feed the WDT.
 *              It uses the previously installed counter.
 *              When the WDT is started, WDT must be feed periodically to avoid
 *              the interrupt or reset.
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_WDT_Feed(void);

/**
 *******************************************************************************
 * @brief       Get current WDT counter value.
 * @param[out]  counter counter value to get
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_WDT_GetCounter(uint32_t *counter);

#ifdef __cplusplus
}
#endif

#endif /* _A34M41X_WDT_H_ */

/** @} */
/** @} */

