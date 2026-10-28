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
#include "a34m41x_uart.h"

#define APP_DEVICE                      "A34M41X"
#define APP_CORE                        "ARM Cortex-M4F"
#define APP_UART_ID                     0
#define APP_UART_BAUD                   38400

#define APP_UART_DMA_ENABLE             0

#define APP_UART_CONFIG                     \
    {                                       \
        .baudrate = APP_UART_BAUD,          \
        .data_bit = UART_DATA_BIT_8,        \
        .parity_bit = UART_PARITY_BIT_NONE, \
        .stop_bit = UART_STOP_BIT_1,        \
        .irq_prio = DRIVER_PRIORITY_LOWEST, \
    };

void Port_Init(void);
void Port_Configure_UART(void);
void System_Setting_Config(void);
void Debug_Init(void);
void Assert(char *file, uint32_t line);

#endif /* _MAIN_H_ */

