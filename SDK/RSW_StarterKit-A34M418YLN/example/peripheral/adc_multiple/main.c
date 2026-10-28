/**
 *******************************************************************************
 * @file        main.c
 * @author      ABOV R&D Division
 * @brief       Main Example Code
 *
 * Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/

#include <stdint.h>
#include <stdio.h>

#include "main.h"
#include "debug_cmd.h"
#include "debug_log.h"

static volatile uint32_t adc_irq_seq_flg;

uint32_t adc_buf[4];

static void app_timer_irq_handler(uint32_t event, void *context)
{
}

static void app_adc_irq_handler(uint32_t event, void *context)
{
    if (event & ADC_EVENT_SEQUENCE)
    {
        /* ADC sequence conversion event*/
        LOG("ADC sequence conversion\n");
        adc_irq_seq_flg = 1;
    }
    if (event & ADC_EVENT_TRIGGER)
    {
        /* ADC trigger conversion event*/
        LOG("ADC trigger conversion\n");
    }
    if (event & ADC_EVENT_DMA)
    {
        /* ADC DMA conversion event*/
        LOG("ADC DMA conversion \n");
        adc_irq_seq_flg = 1;
    }
}

static void app_adc_run(void)
{

    while (adc_irq_seq_flg == 0)
    {
    }
    adc_irq_seq_flg = 0;

    LOG("CH[00]=0x%04x\n", ADC_RAW_TO_VAL(adc_buf[0]));
    LOG("CH[02]=0x%04x\n", ADC_RAW_TO_VAL(adc_buf[1]));
    LOG("CH[04]=0x%04x\n", ADC_RAW_TO_VAL(adc_buf[2]));
}

static void app_timer_config(void)
{
    struct timer_cfg cfg = APP_TIMER_CONFIG;

    /* Initialize Timer */
    HAL_TIMER_Init(TIMER_ID_0, &cfg, app_timer_irq_handler, NULL);

    /* Start Timer */
    HAL_TIMER_Start(TIMER_ID_0, 1000);

    /* Initialize Timer */
    HAL_TIMER_Init(TIMER_ID_1, &cfg, app_timer_irq_handler, NULL);

    /* Start Timer */
    HAL_TIMER_Start(TIMER_ID_1, 1000);

    /* Initialize Timer */
    HAL_TIMER_Init(TIMER_ID_2, &cfg, app_timer_irq_handler, NULL);

    /* Start Timer */
    HAL_TIMER_Start(TIMER_ID_2, 1000);
}

static void mainloop(void)
{
    /* ADC configuration */
    struct adc_cfg cfg = APP_ADC_CONFIG;

    /* Set the ADC Calibration */ 
    HAL_ADC_Calibration();

    /* Initialize the ADC with the configuration */
    HAL_ADC_Init(ADC_ID_0, &cfg, app_adc_irq_handler, NULL);

    /* Trigger setting */
    HAL_ADC_SelectTriggerTimer(ADC_ID_0, ADC_TRIGGER_SEQUENCE_ID_0,
                               ADC_TRIGGER_TIMER_ID_0);
    HAL_ADC_SelectTriggerTimer(ADC_ID_0, ADC_TRIGGER_SEQUENCE_ID_1,
                               ADC_TRIGGER_TIMER_ID_1);
    HAL_ADC_SelectTriggerTimer(ADC_ID_0, ADC_TRIGGER_SEQUENCE_ID_2,
                               ADC_TRIGGER_TIMER_ID_2);

    /* channelSel setting */
    HAL_ADC_SelectChannel(ADC_ID_0, ADC_SEQUENCE_ID_0, 0);
    HAL_ADC_SelectChannel(ADC_ID_0, ADC_SEQUENCE_ID_1, 2);
    HAL_ADC_SelectChannel(ADC_ID_0, ADC_SEQUENCE_ID_2, 4);

    HAL_ADC_StartMultiple(ADC_ID_0, adc_buf, ADC_IO_TYPE_NON_BLOCK);

    app_timer_config();

    while (1)
    {
        app_adc_run();
    }
}

int main(void)
{
    /* Initialize all port */
    Port_Init();

    /* Configure system settings */
    System_Setting_Config();

    /* Initialize ADC port */
    Port_Configure_ADC();

    /* Initialize UART Debugging framework to use logging or command input */
    Debug_Init();

    LOG("\n");
    LOG("************************************************\n");
    LOG("\t MCU: %s\n", APP_DEVICE);
    LOG("\t Core: %s\n", APP_CORE);
    LOG("\t Debug: UART%d - %dbps\n", APP_UART_ID, APP_UART_BAUD);
    LOG("\t %s, %s\n", __DATE__, __TIME__);
    LOG("\t ADC multiple interrupt\n");
    LOG("************************************************\n");

    /* Start main loop */
    mainloop();
}

