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
#include "a34m41x_comp.h"

#define APP_DEVICE                      "A34M41X"
#define APP_CORE                        "ARM Cortex-M4F"
#define APP_UART_ID                     0
#define APP_UART_BAUD                   38400

#define APP_COMP_CONFIG                     \
    {                                       \
        .filter = COMP_FILTER_7,            \
        .hystresis = COMP_HYSTRESIS_NONE,   \
        .irq_type = COMP_IRQ_NONE,          \
        .input_neg = COMP_INPUT_NEG_INPUT0, \
        .input_pos = COMP_INPUT_POS_INPUT0, \
        .irq_prio = DRIVER_PRIORITY_LOWEST, \
    }

void Port_Init(void);
void Port_Configure_COMP(void);
void System_Setting_Config(void);
void Debug_Init(void);
void Assert(char *file, uint32_t line);

#endif /* _MAIN_H_ */

