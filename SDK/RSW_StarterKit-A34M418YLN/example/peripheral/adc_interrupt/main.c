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

#define APP_NUM_ADC_CH      4

static volatile uint32_t adc_irq_single_flg;

static void app_adc_irq_handler(uint32_t event, void *context)
{
    if (event & ADC_EVENT_SINGLE)
    {
        /* ADC single conversion event */
        LOG("ADC single conversion\n");
        adc_irq_single_flg = 1;
    }

    if (event & ADC_EVENT_DMA)
    {
        /* ADC DMA conversion event*/
        LOG("ADC DMA conversion \n");
        adc_irq_single_flg = 1;
    }
}

static void app_adc_run(void)
{
    uint32_t i;
    uint32_t adc_buf;

    LOG("Reading ADC\n");

    for (i = 0; i < APP_NUM_ADC_CH; i++)
    {
        /* Select the channel -> ch 0 , 2, 4, 6*/
        HAL_ADC_SelectChannel(ADC_ID_0, ADC_SEQUENCE_ID_0, i*2);

        adc_irq_single_flg = 0;

        /* Start ADC and wait for conversion to be completed */
        HAL_ADC_StartSingle(ADC_ID_0, &adc_buf, ADC_IO_TYPE_NON_BLOCK);

        while (adc_irq_single_flg == 0)
        {
        }

        LOG("CH[%02d]=0x%04x\n", i*2, adc_buf>>4);
    }
    SystemDelay(1000);
}

static void mainloop(void)
{
    /* ADC configuration */
    struct adc_cfg cfg = APP_ADC_CONFIG;

    /* Set the ADC Calibration */ 
    HAL_ADC_Calibration();

    /* Initialize the ADC with the configuration */
    HAL_ADC_Init(ADC_ID_0, &cfg, app_adc_irq_handler, NULL);

#if (APP_ADC_DMA_ENABLE == 1)
    /* Enable DMA */
    HAL_ADC_SetDMA(ADC_ID_0, DMA_ID_0, 1);
#endif

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
    LOG("\t ADC interrupt\n");
    LOG("************************************************\n");

    /* Start main loop */
    mainloop();
}

