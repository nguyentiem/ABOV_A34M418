/**
 *******************************************************************************
 * @file        a34m41x_timer.h
 * @author      ABOV R&D Division
 * @brief       Timer Header File
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
 * @defgroup    driver_timer TIMER
 * @{
 * @brief       TIMER driver
 */

#ifndef _A34M41X_TIMER_H_
#define _A34M41X_TIMER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "a34m41x_driver_common.h"

/**
 * Timer id
 */
enum timer_id
{
    TIMER_ID_0      = 0,                    /**< TIMER 0 */
    TIMER_ID_1      = 1,                    /**< TIMER 1 */
    TIMER_ID_2      = 2,                    /**< TIMER 2 */
    TIMER_ID_3      = 3,                    /**< TIMER 3 */
    TIMER_ID_4      = 4,                    /**< TIMER 4 */
    TIMER_ID_5      = 5,                    /**< TIMER 5 */
    TIMER_ID_6      = 6,                    /**< TIMER 6 */
    TIMER_ID_7      = 7,                    /**< TIMER 7 */
    TIMER_ID_8      = 8,                    /**< TIMER 8 */
    TIMER_ID_9      = 9,                    /**< TIMER 9 */
    TIMER_ID_MAX    = 10,                   /**< TIMER max */
};

/**
 * TIMER mode
 */
enum timer_mode
{
    TIMER_MODE_PERIODIC = 0,                /**< periodic mode */
    TIMER_MODE_PWM      = 1,                /**< pwm mode */
    TIMER_MODE_ONESHOT  = 2,                /**< oneshot mode */
    TIMER_MODE_CAPTURE  = 3,                /**< capture mode */
};

/**
 * TIMER clock selection
 */
enum timer_clk_sel
{
    TIMER_CLK_DIV_2_PCLK    = 0,            /**< PCLK divided by 2 */
    TIMER_CLK_DIV_4_PCLK    = 1,            /**< PCLK divided by 4 */
    TIMER_CLK_DIV_16_PCLK   = 2,            /**< PCLK divided by 16 */
    TIMER_CLK_DIV_64_PCLK   = 3,            /**< PCLK divided by 64 */
    TIMER_CLK_MCCLK         = 4,            /**< miscellaneous clock */
    TIMER_CLK_TNIO          = 5,            /**< input to pin */
};

/**
 * Miscellaneous Clock
 */
enum timer_mcclk_sel
{
    TIMER_MCCLK_LSI = 0,                    /**< LSI clock */
    TIMER_MCCLK_LSE = 1,                    /**< LSE clock */
    TIMER_MCCLK_MCK = 4,                    /**< MCK clock */
    TIMER_MCCLK_HSI = 5,                    /**< HSI clock */
    TIMER_MCCLK_HSE = 6,                    /**< HSE clock */
    TIMER_MCCLK_PLL = 7,                    /**< PLL clock */
};

/**
 * TIMER PWM start level
 */
enum timer_pwm_start_level
{
    TIMER_PWM_START_LOW     = 0,            /**< PWM mode start low */
    TIMER_PWM_START_HIGH    = 1,            /**< PWM mode start high */
};

/**
 * TIMER ADC trigger
 */
enum timer_adc_trig
{
    TIMER_ADC_TRIG_DISABLE  = 0,            /**< ADC triggering disable */
    TIMER_ADC_TRIG_ENABLE   = 1,            /**< ADC triggering enable */
};

/**
 * TIMER capture clear mode
 */
enum timer_capture_clear
{
    TIMER_CAPTURE_CLEAR_RISING  = 0,        /**< capture clear on rising edge */
    TIMER_CAPTURE_CLEAR_FALLING = 1,        /**< capture clear on falling edge */
    TIMER_CAPTURE_CLEAR_BOTH    = 2,        /**< capture clear on both edges */
    TIMER_CAPTURE_CLEAR_NONE    = 3,        /**< capture clear not used */
};

/**
 * TIMER PWM event flag
 */
enum timer_pwm_event
{
    TIMER_PWM_EVENT_NONE    = 0,            /**< enable no event */
    TIMER_PWM_EVENT_DUTY    = BIT(0),       /**< enable duty event */
    TIMER_PWM_EVENT_PERIOD  = BIT(1),       /**< enable period event */
};

/**
 * TIMTER capture register
 */
enum timer_capture_reg
{
    TIMER_CAPTURE_REG_GRA   = 1,            /**< capture with GRA */
    TIMER_CAPTURE_REG_GRB   = 2,            /**< capture with GRB */
};

/**
 * TIMTER interrupt type
 */
enum timer_irq_type
{
    TIMER_IRQ_OVERFLOW      = 0,            /**< overflow */
    TIMER_IRQ_COUNT_MATCH   = 1,            /**< counter match */
    TIMER_IRQ_PWM_DUTY      = 2,            /**< pwm duty */
    TIMER_IRQ_PWM_PERIOD    = 3,            /**< pwm period */
    TIMER_IRQ_CAPTURE_GRA   = 4,            /**< capture with GRA */
    TIMER_IRQ_CAPTURE_GRB   = 5,            /**< capture with GRB */
};

/**
 * TIMER synchronization for the start count
 */
enum timer_sync_start
{
    TIMER_SYNC_START_DISABLE    = 0,        /**< disable start count synchronization */
    TIMER_SYNC_START_ENABLE     = 1,        /**< enable start count synchronization */
};

/**
 * TIMER synchronization for the clear count
 */
enum timer_sync_clear
{
    TIMER_SYNC_CLEAR_DISABLE    = 0,        /**< disable clear count synchronization */
    TIMER_SYNC_CLEAR_ENABLE     = 1,        /**< enable clear count synchronization */
};

/**
 * TIMER event
 */
enum timer_event
{
    TIMER_EVNET_OVERFLOW    = BIT(0),       /**< overflow */
    TIMER_EVENT_COUNT_MATCH = BIT(1),       /**< counter match */
    TIMER_EVENT_PWM_DUTY    = BIT(2),       /**< pwm duty */
    TIMER_EVNET_PWM_PERIOD  = BIT(3),       /**< pwm period */
    TIMER_EVENT_CAPTURE_GRA = BIT(4),       /**< capture with GRA */
    TIMER_EVENT_CAPTURE_GRB = BIT(5),       /**< capture with GRB */
};

/**
 *******************************************************************************
 * @brief       TIMER interrupt callback function type
 * @param[in]   event event type ::timer_event
 * @param[in]   context context provided during initialization
 ******************************************************************************/
typedef void (*timer_irq_handler_t)(uint32_t event, void *context);

/**
 * TIMER configuration
 */
struct timer_cfg
{
    enum timer_mode     mode;               /**< mode */
    enum timer_clk_sel  clk_sel;            /**< clock selection */
    uint16_t            prescaler;          /**< prescaler */
    enum timer_adc_trig adc_trig;           /**< adc trigger  */
    uint8_t             irq_prio;           /**< interrupt priority */
};

/**
 *******************************************************************************
 * @brief       Initialize TIMER.
 * @param[in]   id instance id
 * @param[in]   cfg configuration
 * @param[in]   handler callback function to receive event
 * @param[in]   context caller's context
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER_Init(enum timer_id id, struct timer_cfg *cfg,
                       timer_irq_handler_t handler,
                       void *context);

/**
 *******************************************************************************
 * @brief       Deintialize TIMER.
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER_Deinit(enum timer_id id);

/**
 *******************************************************************************
 * @brief       Get TIMER status
 * @param[in]   id instance id
 * @param[out]  status status of the TIMER
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER_GetStatus(enum timer_id id, uint8_t *status);

/**
 *******************************************************************************
 * @brief       Start TIMER in either oneshot mode or periodic mode.
 * @param[in]   id instance id
 * @param[in]   count count
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER_Start(enum timer_id id, uint16_t count);

/**
 *******************************************************************************
 * @brief       Start TIMER in PWM mode.
 * @param[in]   id instance id
 * @param[in]   level PWM start level
 * @param[in]   duty PWM duty
 * @param[in]   period PWM period
 * @param[in]   event PWM event to b enabled, see ::timer_pwm_event
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER_StartPWM(enum timer_id id, enum timer_pwm_start_level level,
                           uint16_t duty, uint16_t period,
                           uint8_t event);

/**
 *******************************************************************************
 * @brief       Start TIMER in capture mode.
 * @param[in]   id instance id
 * @param[in]   clear condition for clearing
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER_StartCapture(enum timer_id id, enum timer_capture_clear clear);

/**
 *******************************************************************************
 * @brief       Enable Synchronizing TIMER to master timer.
 * @param[in]   id instance id
 * @param[in]   master instance id of the master timer
 * @param[in]   start synchronize start counter
 * @param[in]   clear synchronize clear counter
 * @param[in]   delay starting delay count
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER_EnableSynch(enum timer_id id,
                              enum timer_id master,
                              enum timer_sync_start start,
                              enum timer_sync_clear clear,
                              uint16_t delay);

/**
 *******************************************************************************
 * @brief       Disable Synchronizing TIMER to master timer.
 * @param[in]   id instance id
 * @param[in]   master instance id of the master timer
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER_DisableSynch(enum timer_id id, enum timer_id master);

/**
 *******************************************************************************
 * @brief       Stop TIMER.
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER_Stop(enum timer_id id);

/**
 *******************************************************************************
 * @brief       Get TIMER count
 * @param[in]   id instance id
 * @param[out]  count the current count value
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER_GetCount(enum timer_id id, uint16_t *count);

/**
 *******************************************************************************
 * @brief       Clear TIMER count
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER_ClearCount(enum timer_id id);

/**
 *******************************************************************************
 * @brief       Get TIMER capture count
 * @param[in]   id instance id
 * @param[out]  count the current count value
 * @param[in]   reg which register to use for the capture
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER_GetCaptureCount(enum timer_id id, uint16_t *count, enum timer_capture_reg reg);

/**
 *******************************************************************************
 * @brief       Set TIMER clock
 *              Timer can be configure to use source clock other than MCLK.
 * @param[in]   id instance id
 * @param[in]   clk_sel clock selection
 * @param[in]   prescaler pre-scaler
 * @param[in]   mcclk_sel miscellaneous clock selection
 * @param[in]   mcclk_div miscellaneous clock divider
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER_SetClock(enum timer_id id, enum timer_clk_sel clk_sel, uint16_t prescaler,
                           enum timer_mcclk_sel mcclk_sel, uint8_t mcclk_div);

/**
 *******************************************************************************
 * @brief       Set TIMER interrupt
 * @param[in]   id instance id
 * @param[in]   type interrypt type
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_TIMER_SetIRQ(enum timer_id id, enum timer_irq_type type, uint8_t enable);

#ifdef __cplusplus
}
#endif

#endif /* _A34M41X_TIMER_H_ */

/** @} */
/** @} */

