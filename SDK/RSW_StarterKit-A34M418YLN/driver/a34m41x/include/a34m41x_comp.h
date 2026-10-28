/**
 *******************************************************************************
 * @file        a34m41x_comp.h
 * @author      ABOV R&D Division
 * @brief       Comparator Header File
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
 * @defgroup    driver_comp COMP
 * @{
 * @brief       COMP driver
 */

#ifndef _A34M41X_COMP_H_
#define _A34M41X_COMP_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "a34m41x_driver_common.h"
#include "a34m41x_dma.h"

/**
 * COMP instance id
 */
enum comp_id
{
    COMP_ID_0   = 0,                        /**< COMP 0 */
    COMP_ID_1   = 1,                        /**< COMP 1 */
    COMP_ID_2   = 2,                        /**< COMP 2 */
    COMP_ID_3   = 3,                        /**< COMP 3 */
    COMP_ID_MAX = 6,                        /**< COMP max */	// kyi - 4?
};

/**
 * COMP Filter
 */
enum comp_filter
{
    COMP_FILTER_0,                          /**< Filter count 0 */
    COMP_FILTER_1,                          /**< Filter count 1 */
    COMP_FILTER_2,                          /**< Filter count 2 */
    COMP_FILTER_3,                          /**< Filter count 3 */
    COMP_FILTER_4,                          /**< Filter count 4 */
    COMP_FILTER_5,                          /**< Filter count 5 */
    COMP_FILTER_6,                          /**< Filter count 6 */
    COMP_FILTER_7,                          /**< Filter count 7 */
    COMP_FILTER_MAX,                        /**< Filter count MAX - should not used this */
};

/**
 * COMP hystresis
 */
enum comp_hystresis_type
{
    COMP_HYSTRESIS_NONE,                    /**< hystresis disabled */
    COMP_HYSTRESIS_5MV,                     /**< hystresis 5mV */
    COMP_HYSTRESIS_20MV,                    /**< hystresis 20mV */
};

/**
 * COMP interrupt type
 */
enum comp_irq_type
{
    COMP_IRQ_NONE,                          /**< interrupt disabled */
    COMP_IRQ_LEVEL,                         /**< interrupt on level */
    COMP_IRQ_LOW_EDGE,                      /**< interrupt on low (falling) edge */
    COMP_IRQ_HIGH_EDGE,                     /**< interrupt on high (rising) edge */
    COMP_IRQ_BOTH_EDGE,                     /**< interrupt on both edges */
};

/**
 * COMP input reference negative selection
 */
enum comp_input_neg
{
    COMP_INPUT_NEG_INPUT0,                  /**< negative reference 0 */
    COMP_INPUT_NEG_INPUT1,                  /**< negative reference 1 */
};

/**
 * COMP input reference positive selection
 */
enum comp_input_pos
{
    COMP_INPUT_POS_INPUT0,                  /**< positive input 0 */
    COMP_INPUT_POS_INPUT1,                  /**< positive input 1 */
    COMP_INPUT_POS_INPUT2,                  /**< positive input 2 */
    COMP_INPUT_POS_INPUT3,                  /**< positive input 3 */
};

/**
 * COMP status
 */
enum comp_status
{
    COMP_STATUS_DOWN,                       /**< comparator voltage crossed down */
    COMP_STATUS_UP,                         /**< comparator voltage crossed up */
};

/**
 * COMP event
 */
enum comp_event
{
    COMP_EVENT_UP   = BIT(0),               /**< comparater triggered */
    COMP_EVENT_DOWN = BIT(0),               /**< comparater triggered */
};

/**
 *******************************************************************************
 * @brief       COMP interrupt callback function type
 * @param[in]   event event type ::comp_event
 * @param[in]   context context provided during initialization
 ******************************************************************************/
typedef void (*comp_irq_handler_t)(uint32_t event, void *context);

/**
 * COMP configuration
 */
struct comp_cfg
{
    enum comp_filter            filter;     /**< filter */
    enum comp_hystresis_type    hystresis;  /**< hystresis */
    enum comp_irq_type          irq_type;   /**< interrupt type */
    enum comp_input_neg         input_neg;  /**< negative input */
    enum comp_input_pos         input_pos;  /**< positive input */
    uint8_t                     irq_prio;   /**< interrupt priority */
};

/**
 *******************************************************************************
 * @brief       Initialize COMP
 * @param[in]   id instance id
 * @param[in]   cfg configuration
 * @param[in]   handler callback function to receive event
 * @param[in]   context caller's context
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_COMP_Init(enum comp_id id, struct comp_cfg *cfg,
                      comp_irq_handler_t handler,
                      void *context);

/**
 *******************************************************************************
 * @brief       Deintialize COMP.
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_COMP_Deinit(enum comp_id id);

/**
 *******************************************************************************
 * @brief       Start COMP.
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_COMP_Start(enum comp_id id);

/**
 *******************************************************************************
 * @brief       Stop COMP.
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_COMP_Stop(enum comp_id id);

/**
 *******************************************************************************
 * @brief       Get COMP output status.
 * @param[in]   id instance id
 * @param[out]  status status of the output
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_COMP_GetStatus(enum comp_id id, uint8_t *status);



#ifdef __cplusplus
}
#endif

#endif /* _A34M41X_COMP_H_ */

/** @} */
/** @} */

