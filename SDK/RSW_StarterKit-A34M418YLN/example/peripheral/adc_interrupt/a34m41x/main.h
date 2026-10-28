/**
 *******************************************************************************
 * @file        main.h
 * @author      ABOV R&D Division
 * @brief       Main Header File
 *
 * Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#ifndef _MAIN_H_
#define _MAIN_H_

#include "a34m41x.h"
#include "a34m41x_adc.h"
#include "a34m41x_timer.h"

#define APP_DEVICE                      "A34M41X"
#define APP_CORE                        "ARM Cortex-M4F"
#define APP_UART_ID                     0
#define APP_UART_BAUD                   38400

#define APP_ADC_DMA_ENABLE              0

#define APP_ADC_CONFIG                  \
    {                                   \
        .mode = ADC_MODE_SINGLE,        \
        .sampling_time = 14,            \
        .seq_cnt = 1,                   \
        .restart_en = 0,                \
        .in_clk_div = 5,                \
        .irq_prio = DRIVER_PRIORITY_3,  \
    }

void Port_Init(void);
void Port_Configure_ADC(void);
void System_Setting_Config(void);
void Debug_Init(void);
void Assert(char *file, uint32_t line);

#endif /* _MAIN_H_ */

