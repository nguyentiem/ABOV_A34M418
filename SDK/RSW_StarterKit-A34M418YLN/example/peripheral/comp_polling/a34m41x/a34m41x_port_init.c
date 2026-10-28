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

void Port_Configure_COMP(void)
{
    HAL_GPIO_SetFunction(PORTA, PIN4, PA4_MUX_CP0A);
    HAL_GPIO_SetMode(PORTA, PIN4, GPIO_MODE_INPUT);
    HAL_GPIO_SetPull(PORTA, PIN4, GPIO_PULL_DISABLE);

    HAL_GPIO_SetFunction(PORTA, PIN5, PA5_MUX_CP0B);
    HAL_GPIO_SetMode(PORTA, PIN5, GPIO_MODE_INPUT);
    HAL_GPIO_SetPull(PORTA, PIN5, GPIO_PULL_DISABLE);

    HAL_GPIO_SetFunction(PORTA, PIN6, PA6_MUX_CP0C);
    HAL_GPIO_SetMode(PORTA, PIN6, GPIO_MODE_INPUT);
    HAL_GPIO_SetPull(PORTA, PIN6, GPIO_PULL_DISABLE);

    HAL_GPIO_SetFunction(PORTA, PIN7, PA7_MUX_CREF0);
    HAL_GPIO_SetMode(PORTA, PIN7, GPIO_MODE_INPUT);
    HAL_GPIO_SetPull(PORTA, PIN7, GPIO_PULL_DISABLE);
}

