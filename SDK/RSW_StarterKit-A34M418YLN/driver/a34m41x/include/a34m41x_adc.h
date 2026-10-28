/**
 *******************************************************************************
 * @file        a34m41x_adc.h
 * @author      ABOV R&D Division
 * @brief       ADC Header File
 *
 * Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#ifndef _A34M41X_ADC_H_
#define _A34M41X_ADC_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "a34m41x_driver_common.h"
#include "a34m41x_dma.h"

/** ADC Ready time-out definitions in case of using function
 * 
 */
#define ADC_READY_TIMEOUT               (0x10UL)	
#define ADC_SR_READY_TIMEOUT            (0x1000UL)	
/*
 * These bits allow control over which A/D channels generate
 * interrupts for conversion completion
 */
#define ADC_INTEN_DMA                   ((1UL << 4))
#define ADC_INTEN_TRIGGER               ((1UL << 3))
#define ADC_INTEN_SEQUENCE              ((1UL << 2))
#define ADC_INTEN_SINGLE                ((1UL << 0))

#define ADC_STAT_SINGLE                 ((1UL << 0))

#define ADC_RAW_TO_VAL(x)               ((x >> 4) & 0x0FFF)

/**
 * ADC id
 */
enum adc_id
{
    ADC_ID_0,                               /**< ADC 0 */
    ADC_ID_1,                               /**< ADC 1 */
    ADC_ID_2,                               /**< ADC 2 */
    ADC_ID_MAX,                             /**< ADC max */
};

/**
 * ADC Sequence DR id
 */
enum adc_sequence_id
{
    ADC_SEQUENCE_ID_0,                      /**< Sequence DR 0 data*/
    ADC_SEQUENCE_ID_1,                      /**< Sequence DR 1 data*/
    ADC_SEQUENCE_ID_2,                      /*!< Sequence DR 2 data*/
    ADC_SEQUENCE_ID_3,                      /*!< Sequence DR 3 data*/
    ADC_SEQUENCE_ID_4,                      /**< Sequence DR 4 data*/
    ADC_SEQUENCE_ID_5,                      /**< Sequence DR 5 data*/
    ADC_SEQUENCE_ID_6,                      /*!< Sequence DR 6 data*/
    ADC_SEQUENCE_ID_7,                      /*!< Sequence DR 7 data*/
    ADC_SEQUENCE_MAX,
};

/**
 * ADC mode
 */
enum adc_mode
{
    ADC_MODE_SINGLE,                        /**< adc single conversion mode*/
    ADC_MODE_SEQUNCE,                       /**< adc sequence conversion mode*/
    ADC_MODE_BURST,                         /**< adc burst conversion mode */
    ADC_MODE_MULTI,                         /*!< adc multiple conversion mode */
};

/**
 * ADC Sequence DR id
 */
enum adc_trigger_sequence_id
{
    ADC_TRIGGER_SEQUENCE_ID_BURST   = 0,     /**< Sequence DR 0 data*/
    ADC_TRIGGER_SEQUENCE_ID_0       = 0,     /**< Sequence DR 0 data*/
    ADC_TRIGGER_SEQUENCE_ID_1       = 1,     /**< Sequence DR 1 data*/
    ADC_TRIGGER_SEQUENCE_ID_2       = 2,     /*!< Sequence DR 2 data*/
    ADC_TRIGGER_SEQUENCE_ID_3       = 3,     /*!< Sequence DR 3 data*/
    ADC_TRIGGER_SEQUENCE_ID_4       = 4,     /**< Sequence DR 4 data*/
    ADC_TRIGGER_SEQUENCE_ID_5       = 5,     /**< Sequence DR 5 data*/
    ADC_TRIGGER_SEQUENCE_ID_6       = 6,     /*!< Sequence DR 6 data*/
    ADC_TRIGGER_SEQUENCE_ID_7       = 7,     /*!< Sequence DR 7 data*/
};

enum adc_trigger_timer_id
{
    ADC_TRIGGER_TIMER_ID_0  = 0,
    ADC_TRIGGER_TIMER_ID_1  = 1,
    ADC_TRIGGER_TIMER_ID_2  = 2,
    ADC_TRIGGER_TIMER_ID_3  = 3,
    ADC_TRIGGER_TIMER_ID_4  = 4,
    ADC_TRIGGER_TIMER_ID_5  = 5,
    ADC_TRIGGER_TIMER_ID_6  = 6,
    ADC_TRIGGER_TIMER_ID_7  = 7,
    ADC_TRIGGER_TIMER_ID_8  = 8,
    ADC_TRIGGER_TIMER_ID_9  = 9,
};

enum adc_trigger_mpwm_type
{
    ADC_TRIGGER_MPWM_TYPE_MPWM0 = 0,
    ADC_TRIGGER_MPWM_TYPE_MPWM1 = 1,
};

enum adc_trigger_mpwm_id
{
    ADC_TRIGGER_MPWM_ID_ATR1  = 0,
    ADC_TRIGGER_MPWM_ID_ATR2  = 1,
    ADC_TRIGGER_MPWM_ID_ATR3  = 2,
    ADC_TRIGGER_MPWM_ID_ATR4  = 3,
    ADC_TRIGGER_MPWM_ID_ATR5  = 4,
    ADC_TRIGGER_MPWM_ID_ATR6  = 5,
    ADC_TRIGGER_MPWM_ID_PERIOD_U = 6,
    ADC_TRIGGER_MPWM_ID_BOTTON_U = 7,
    ADC_TRIGGER_MPWM_ID_PERIOD_V = 8,
    ADC_TRIGGER_MPWM_ID_BOTTON_V = 9,
    ADC_TRIGGER_MPWM_ID_PERIOD_W = 10,
    ADC_TRIGGER_MPWM_ID_BOTTON_W = 11,
};

/**
 * ADC external clock configuration
 */
enum adc_ext_clk
{
    ADC_INTERNAL_CLK,                       /**< Internal clock (CLKDIV enablement)*/
    ADC_CLK_Reserved,                       /**< Reserved */
};

/**
 * ADC IO type
 */
enum adc_io_type
{
    ADC_IO_TYPE_BLOCK      = 0,             /**< blocking */
    ADC_IO_TYPE_NON_BLOCK  = 1,             /**< non-blocking */
};

/**
 * ADC interrupt type
 */
enum adc_irq_type
{
    ADC_IRQ_SINGLE,                         /**< ADC single conversion interrupt*/
    ADC_IRQ_SEQUENCE,                       /**< ADC sequence conversion interrupt*/
    ADC_IRQ_TRIGGER,                        /**< ADC trigger conversion interrupt */
    ADC_IRQ_DMA,                            /**< ADC DMA done interrupt. */
};

/**
 * ADC event
 */
enum adc_event
{
    ADC_EVENT_SINGLE    = BIT(0),           /**< ADC single conversion event*/
    ADC_EVENT_SEQUENCE  = BIT(1),           /**< ADC sequence conversion event*/
    ADC_EVENT_TRIGGER   = BIT(2),           /**< ADC trigger conversion event */
    ADC_EVENT_DMA       = BIT(3),           /**< ADC DMA done event */
};

/**
 * ADC configuration
 */
struct adc_cfg
{
    enum adc_mode           mode;           /**< ADC_MODE_SINGLE, ADC_MODE_MULTI ,ADC_MODE_BURST */
    uint32_t                dma_opt;        /**< DMA option = ENABLE or DISABLE */
    uint32_t                sampling_time;  /**< sampleing time */
    uint32_t                seq_cnt;        /**< sequence count = count value 2~8  */
    uint32_t                restart_en;     /**< RestartEn = ENABLE or DISABLE */
    uint32_t                in_clk_div;     /**< clock divider */
    uint8_t                 irq_prio;       /**< interrupt priority */
};

/**
 *******************************************************************************
 * @brief       ADC interrupt callback function type
 * @param[in]   event event type
 * @param[in]   context context provided during initialization
 ******************************************************************************/
typedef void (*adc_irq_handler_t)(uint32_t event, void *context);

/**
 *******************************************************************************
 * @brief       Initialize ADC.
 * @param[in]   id instance id
 * @param[in]   cfg configuration
 * @param[in]   handler callback function to receive event
 * @param[in]   context caller's context
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_Init(enum adc_id id, struct adc_cfg *cfg,
                     adc_irq_handler_t handler,
                     void *context);

/**
 *******************************************************************************
 * @brief       Deintialize ADC.
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_DeInit(enum adc_id id);

/**
 *******************************************************************************
 * @brief       Calibration ADC.
 * @param[in]   none
 * @return      none
 ******************************************************************************/
void HAL_ADC_Calibration(void);

/**
 *******************************************************************************
 * @brief       Set MR ADC.
 * @param[in]   id instance id
 * @param[in]   mode mode
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_SetMR(enum adc_id id, uint32_t mode);

/**
 *******************************************************************************
 * @brief       Get ADC data.
 * @param[in]   id instance id
 * @param[out]  data data
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_GetData(enum adc_id id, uint16_t *data);

/**
 *******************************************************************************
 * @brief       DMA control for ADC.
 * @param[in]   id instance id
 * @param[in]   ch dma channel
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_SetDMA(enum adc_id id, enum dma_ch ch, uint8_t enable);

/**
 *******************************************************************************
 * @brief       Get ADC data sequence.
 * @param[in]   id instance id
 * @param[in]   reg sequence  id
 * @param[out]  data data
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_GetSequenceData(enum adc_id id, enum adc_sequence_id reg, uint16_t *data);

/**
 *******************************************************************************
 * @brief       Start ADC single mode
 * @param[in]   id instance id
 * @param[out]   data pointer to the data
 * @param[in]   io_type operation type
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_StartSingle(enum adc_id id, uint32_t *data, enum adc_io_type io_type);

/**
 *******************************************************************************
 * @brief       Start ADC Burst mode
 * @param[in]   id instance id
 * @param[out]  data pointer to the data
 * @param[in]   io_type operation type
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_StartBurst(enum adc_id id, uint32_t *data, enum adc_io_type io_type);

/**
 *******************************************************************************
 * @brief       Start ADC sequence mode
 * @param[in]   id instance id
 * @param[out]  data pointer to the data
 * @param[in]   io_type operation type
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_StartSequence(enum adc_id id, uint32_t *data, enum adc_io_type io_type);

/**
 *******************************************************************************
 * @brief       Start ADC multiple mode
 * @param[in]   id instance id
 * @param[out]  data pointer to the data
 * @param[in]   io_type operation type
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_StartMultiple(enum adc_id id, uint32_t *data, enum adc_io_type io_type);

/**
 *******************************************************************************
 * @brief       Start ADC
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_Start(enum adc_id id);

/**
 *******************************************************************************
 * @brief       Stop ADC
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_Stop(enum adc_id id);

/**
 *******************************************************************************
 * @brief       Set AD conversion in power mode
 * @param[in]   id instance id
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_EnterPowerdownMode(enum adc_id id, uint8_t enable);

/**
 *******************************************************************************
 * @brief       Select ADC channel number
 * @param[in]   id instance id
 * @param[in]   seq id seuence ID
 * @param[in]   channel channel number
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_SelectChannel(enum adc_id id, enum adc_sequence_id seq_id, uint32_t channel);

/**
 *******************************************************************************
 * @brief       Select ADC channel number
 * @param[in]   id instance id
 * @param[in]   channel channel number
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_SelectChannel1(enum adc_id id, uint32_t channel);

/**
 *******************************************************************************
 * @brief       Select ADC channel number
 * @param[in]   id instance id
 * @param[in]   channel channel number
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_SelectChannel2(enum adc_id id, uint32_t channel);

/**
 *******************************************************************************
 * @brief       Select sequence or burst timer trigger
 * @param[in]   id instance id
 * @param[in]   trg_seq_id trigger sequecen id
 * @param[in]   trg_tm_id trigger timer id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_SelectTriggerTimer(enum adc_id id, enum adc_trigger_sequence_id trg_seq_id,
                                   enum adc_trigger_timer_id trg_tm_id);

/**
 *******************************************************************************
 * @brief       Select sequence or burst mpwm trigger
 * @param[in]   id instance id
 * @param[in]   trg_seq_id trigger sequecen id
 * @param[in]   trg_mpwm_type trigger mpwm type
 * @param[in]   trg_mpwm_id trigger mpwm id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_SelectTriggerMPWM(enum adc_id id, enum adc_trigger_sequence_id trg_seq_id,
                                  enum adc_trigger_mpwm_type trg_mpwm_type,
                                  enum adc_trigger_mpwm_id trg_mwpm_id);

/**
 *******************************************************************************
 * @brief       Get ADC channel status
 * @param[in]   id instance id
 * @param[in]   status status
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_GetStatus(enum adc_id id, uint32_t *status);

/**
 *******************************************************************************
 * @brief       Clear ADC channel status
 * @param[in]   id instance id
 * @param[in]   status status
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_ClearStatus(enum adc_id id, uint32_t status);

/**
 *******************************************************************************
 * @brief       Enable ADC interrupt
 * @param[in]   id instance id
 * @param[in]   type interrupt type
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_ADC_SetIRQ(enum adc_id id, enum adc_irq_type type, uint8_t enable);

#ifdef __cplusplus
}
#endif

#endif /* _A34M41X_ADC_H_ */

/** @} */
/** @} */

