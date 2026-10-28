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

void Port_Configure_ADC(void)
{
    /* Configure AIN0 pin to be used */
    HAL_GPIO_SetFunction(PORTA, PIN0, PA0_MUX_AN0);
    HAL_GPIO_SetMode(PORTA, PIN0, GPIO_MODE_INPUT);

    /* Configure AIN1 to be used */
    HAL_GPIO_SetFunction(PORTA, PIN1, PA1_MUX_AN1);
    HAL_GPIO_SetMode(PORTA, PIN1, GPIO_MODE_INPUT);

    /* Configure AIN2 to be used */
    HAL_GPIO_SetFunction(PORTA, PIN2, PA2_MUX_AN2);
    HAL_GPIO_SetMode(PORTA, PIN2, GPIO_MODE_INPUT);

    /* Configure AIN3 to be used */
    HAL_GPIO_SetFunction(PORTA, PIN3, PA3_MUX_AN3);
    HAL_GPIO_SetMode(PORTA, PIN3, GPIO_MODE_INPUT);

    /* Configure AIN4 to be used */
    HAL_GPIO_SetFunction(PORTA, PIN4, PA4_MUX_AN4);
    HAL_GPIO_SetMode(PORTA, PIN4, GPIO_MODE_INPUT);

    /* Configure AIN5 to be used */
    HAL_GPIO_SetFunction(PORTA, PIN5, PA5_MUX_AN5);
    HAL_GPIO_SetMode(PORTA, PIN5, GPIO_MODE_INPUT);

}

