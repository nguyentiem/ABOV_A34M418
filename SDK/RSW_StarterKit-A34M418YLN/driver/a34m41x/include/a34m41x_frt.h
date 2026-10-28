/**
 *******************************************************************************
 * @file        a34m41x_frt.h
 * @author      ABOV R&D Division
 * @brief       FRT Header File
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
 * @defgroup    driver_frt FRT
 * @{
 * @brief       FRT driver
 */

#ifndef _A34M41X_FRT_H_
#define _A34M41X_FRT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "a34m41x_driver_common.h"


/**
 * FRT id
 */
enum frt_id
{
    FRT_ID_0,                               /**< FRT 0 */
    FRT_ID_1,                               /**< FRT 1 */
    FRT_ID_MAX,                             /**< FRT max */
};

/**
 * FRT mode
 */
enum frt_mode
{
    FRT_MODE_FREE_RUN,                      /**< free run mode */
    FRT_MODE_MATCH,                         /**< match mode */
};

/**
 * FRT run type
 */
enum frt_run
{
    FRT_RUN_POLLING,                   /**< frt polling */
    FRT_RUN_INTERRUPT,                 /**< frt interrupt mode */
};

/**
 * FRT clock
 */
enum frt_clk
{
    FRT_MCCR_CSEL_LSI   = 0x00,             /**< LSI */
    FRT_MCCR_CSEL_LSE   = 0x01,             /**< LSE */
    FRT_MCCR_CSEL_MCLK  = 0x04,             /**< MCLK */
    FRT_MCCR_CSEL_HSI   = 0x05,             /**< HSI */
    FRT_MCCR_CSEL_HSE   = 0x06,             /**< HSE */
    FRT_MCCR_CSEL_PLL   = 0x07,             /**< PLL */
};

/**
 * FRT event
 */
enum frt_event
{
    FRT_EVENT_MATCH,                        /**< match event */
    FRT_EVENT_OVERFLOW,                     /**< overflow event */
};

/**
 * FRT interrupt type
 */
enum frt_irq_type
{
    FRT_IRQ_MATCH,                          /**< match interrupt */
    FRT_IRQ_OVERFLOW,                       /**< overflow interrupt */
};

/**
 *******************************************************************************
 * @brief       FRT interrupt callback function type
 * @param[in]   event event type
 * @param[in]   context context provided during initialization
 ******************************************************************************/
typedef void (*frt_irq_handler_t)(uint32_t event, void *context);

/**
 * FRT configuration
 */
struct frt_cfg
{
    enum frt_mode   mode;                   /**< mode */
    enum frt_run    run_type;               /**< event type */
    uint8_t         irq_prio;               /**< interrupt priority */
};

/**
 *******************************************************************************
 * @brief       Initialize FRT.
 * @param[in]   id instance id
 * @param[in]   cfg configuration
 * @param[in]   handler callback function to receive event
 * @param[in]   context caller's context
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FRT_Init(enum frt_id id, struct frt_cfg *cfg,
                     frt_irq_handler_t handler,
                     void *context);

/**
 *******************************************************************************
 * @brief       Deinitialize FRT.
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FRT_Deinit(enum frt_id id);

/**
 *******************************************************************************
 * @brief       Set FRT clock.
 * @param[in]   id instance id
 * @param[in]   cl_src clock source
 * @param[in]   clk_div clock divider (0x01~0xFF)
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FRT_SetClock(enum frt_id id, enum frt_clk cl_src, uint8_t clk_div);

/**
 *******************************************************************************
 * @brief       Start FRT.
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FRT_Start(enum frt_id id);

/**
 *******************************************************************************
 * @brief       Stop FRT.
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FRT_Stop(enum frt_id id);

/**
 *******************************************************************************
 * @brief       Get FRT counter.
 * @param[in]   id instance id
 * @param[out]  counter counter value
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FRT_GetCounter(enum frt_id id, uint32_t *counter);

/**
 *******************************************************************************
 * @brief       Clear FRT counter.
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FRT_ClearCounter(enum frt_id id);

/**
 *******************************************************************************
 * @brief       Set FRT match counter.
 * @param[in]   id instance id
 * @param[in]   counter counter value
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FRT_SetMatchCounter(enum frt_id id, uint32_t counter);

/**
 *******************************************************************************
 * @brief       Set FRT interrupt
 * @param[in]   id instance id
 * @param[in]   type interrupt type
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FRT_SetIRQ(enum frt_id id, enum frt_irq_type type, uint8_t enable);


#ifdef __cplusplus
}
#endif

#endif /* _A34M41X_FRT_H_ */

/** @} */
/** @} */

