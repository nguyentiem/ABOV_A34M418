/**
 *******************************************************************************
 * @file        a34m41x_port_init.c
 * @author      ABOV R&D Division
 * @brief       Port Initialization Source File
 *
 * Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/

#include "main.h"

#include "a34m41x_gpio.h"

void Port_Init(void)
{
    HAL_GPIO_Init();
}

void Port_Configure_UART(void)
{
    /* Configure UART RXD pin to be used */
    HAL_GPIO_SetFunction(PORTC, PIN14, PC14_MUX_RXD0);
    HAL_GPIO_SetMode(PORTC, PIN14, GPIO_MODE_INPUT);
    HAL_GPIO_SetPull(PORTC, PIN14, GPIO_PULL_UP);

    /* Configure UART TXD pin to be used */
    HAL_GPIO_SetFunction(PORTC, PIN15, PC15_MUX_TXD0);
    HAL_GPIO_SetMode(PORTC, PIN14, GPIO_MODE_OUTPUT);
    HAL_GPIO_SetPull(PORTC, PIN15, GPIO_PULL_UP);
}

