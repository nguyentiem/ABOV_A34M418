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

void Port_Configure_SPI(void)
{
    HAL_GPIO_SetFunction(PORTA, PIN12, PA12_MUX_SS0);
    HAL_GPIO_SetMode(PORTA, PIN12, GPIO_MODE_OUTPUT);
    HAL_GPIO_SetPull(PORTA, PIN12, GPIO_PULL_DISABLE);

    HAL_GPIO_SetFunction(PORTA, PIN13, PA13_MUX_SCK0);
    HAL_GPIO_SetMode(PORTA, PIN13, GPIO_MODE_OUTPUT);
    HAL_GPIO_SetPull(PORTA, PIN13, GPIO_PULL_DISABLE);

    HAL_GPIO_SetFunction(PORTA, PIN14, PA14_MUX_MOSI0);
    HAL_GPIO_SetMode(PORTA, PIN14, GPIO_MODE_OUTPUT);
    HAL_GPIO_SetPull(PORTA, PIN14, GPIO_PULL_DISABLE);

    HAL_GPIO_SetFunction(PORTA, PIN15, PA15_MUX_MISO0);
    HAL_GPIO_SetMode(PORTA, PIN15, GPIO_MODE_INPUT);
    HAL_GPIO_SetPull(PORTA, PIN15, GPIO_PULL_UP);
}


