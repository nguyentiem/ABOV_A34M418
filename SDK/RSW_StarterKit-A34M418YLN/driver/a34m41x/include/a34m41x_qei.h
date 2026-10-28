/**
 *******************************************************************************
 * @file        a34m41x_qei.h
 * @author      ABOV R&D Division
 * @brief       QEI Header File
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
 * @defgroup    driver_qei QEI
 * @{
 * @brief       QEI driver
 */

#ifndef _A31R21X_QEI_H_
#define _A31R21X_QEI_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "a34m41x_driver_common.h"

/**
 * QEI instance id
 */
enum qei_id
{
    QEI_ID_0    = 0,                        /**< QEI 0 */
    QEI_ID_1    = 1,                        /**< QEI 1 */
    QEI_ID_MAX  = 2,
};

/**
 * QEI position compare type
 */
enum qei_pos_comp
{
    QEI_POS_COMP_0,                         /**< compare 0 */
    QEI_POS_COMP_1,                         /**< compare 1 */
    QEI_POS_COMP_2,                         /**< compare 1 */
};

/**
 * QEI counter type
 */
enum qei_counter_type
{
    QEI_COUNTER_POSITION,                   /**< position counter */
    QEI_COUNTER_INDEX,                      /**< index counter */
    QEI_COUNTER_VELOCITY,                   /**< velocity counter */
};

/**
 * QEI interrupt type
 */
enum qei_irq_type
{
    QEI_IRQ_ERROR,                          /**< error interrupt */
    QEI_IRQ_POSITION,                       /**< position changed interrupt */
    QEI_IRQ_DIRECTION,                      /**< direction changed interrupt */
};

/**
 * QEI event
 */
enum qei_event
{
    QEI_EVENT_ERROR     = BIT(0),           /**< error event */
    QEI_EVENT_FORWARD   = BIT(1),           /**< forward event */
    QEI_EVENT_BACKWARD  = BIT(2),           /**< backward event */
    QEI_EVENT_DIRECTION = BIT(3),           /**< direction changed event */
    QEI_EVENT_POS0      = BIT(4),           /**< position 0 match */
    QEI_EVENT_POS1      = BIT(5),           /**< position 1 match */
    QEI_EVENT_POS2      = BIT(6),           /**< position 2 match */
};

/**
 * QEI configuration
 */
struct qei_cfg
{
    uint8_t enable_position;                /**< enable position */
    uint8_t enable_velocity;                /**< enable velocity */
    uint8_t irq_prio;                       /**< interrupt priority */
};


/**
 *******************************************************************************
 * @brief       QEI interrupt callback function type
 * @param[in]   event event type ::qei_event
 * @param[in]   context context provided during initialization
 ******************************************************************************/
typedef void (*qei_irq_handler_t)(uint32_t event, void *context);


/**
 *******************************************************************************
 * @brief       Initialize QEI.
 * @param[in]   id instance id
 * @param[in]   cfg configuration
 * @param[in]   handler callback function to receive event
 * @param[in]   context caller's context
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_QEI_Init(enum qei_id id, struct qei_cfg *cfg,
                     qei_irq_handler_t handler,
                     void *context);

/**
 *******************************************************************************
 * @brief       Deintialize QEI.
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_QEI_Deinit(enum qei_id id);

/**
 *******************************************************************************
 * @brief       Start QEI.
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_QEI_Start(enum qei_id id);

/**
 *******************************************************************************
 * @brief       Stop QEI.
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_QEI_Stop(enum qei_id id);

/**
 *******************************************************************************
 * @brief       Read QEI position counter
 * @param[in]   id instance id
 * @param[out]  count accumulated counter position
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_QEI_Read(enum qei_id id, int32_t *count);

/**
 *******************************************************************************
 * @brief       Clear QEI counter
 * @param[in]   id instance id
 * @param[out]  type counter type
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_QEI_ClearCounter(enum qei_id id, enum qei_counter_type type);

/**
 *******************************************************************************
 * @brief       Enable position compare
 * @param[in]   id instance id
 * @param[in]   comp compare id
 * @param[in]   value compare value
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_QEI_EnablePositionCompare(enum qei_id id, enum qei_pos_comp comp, uint32_t value);

/**
 *******************************************************************************
 * @brief       Disable position compare
 * @param[in]   id instance id
 * @param[in]   comp compare id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_QEI_DisablePositionCompare(enum qei_id id, enum qei_pos_comp comp);

/**
 *******************************************************************************
 * @brief       Set QEI interrupt.
 *              Different QEI interrupt can be enabled or disable.
 * @param[in]   id instance id
 * @param[in]   type interrupt type
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_QEI_SetIRQ(enum qei_id id, enum qei_irq_type type, uint8_t enable);

#endif /* _A31R21X_QEI_H_ */

/** @} */
/** @} */


