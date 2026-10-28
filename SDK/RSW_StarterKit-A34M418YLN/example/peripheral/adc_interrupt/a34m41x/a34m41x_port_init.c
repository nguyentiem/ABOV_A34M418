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

    /* Configure AIN6 to be used */
    HAL_GPIO_SetFunction(PORTA, PIN6, PA6_MUX_AN6);
    HAL_GPIO_SetMode(PORTA, PIN6, GPIO_MODE_INPUT);

    /* Configure AIN7 to be used */
    HAL_GPIO_SetFunction(PORTA, PIN7, PA7_MUX_AN7);
    HAL_GPIO_SetMode(PORTA, PIN7, GPIO_MODE_INPUT);

    /* Configure AIN8 to be used */
    HAL_GPIO_SetFunction(PORTA, PIN8, PA8_MUX_AN8);
    HAL_GPIO_SetMode(PORTA, PIN8, GPIO_MODE_INPUT);

    /* Configure AIN9 to be used */
    HAL_GPIO_SetFunction(PORTA, PIN9, PA9_MUX_AN9);
    HAL_GPIO_SetMode(PORTA, PIN9, GPIO_MODE_INPUT);

    /* Configure AIN10 to be used */
    HAL_GPIO_SetFunction(PORTA, PIN10, PA10_MUX_AN10);
    HAL_GPIO_SetMode(PORTA, PIN10, GPIO_MODE_INPUT);

    /* Configure AIN11 to be used */
    HAL_GPIO_SetFunction(PORTA, PIN11, PA11_MUX_AN11);
    HAL_GPIO_SetMode(PORTA, PIN11, GPIO_MODE_INPUT);

    /* Configure AIN12 to be used */
    HAL_GPIO_SetFunction(PORTA, PIN12, PA12_MUX_AN12);
    HAL_GPIO_SetMode(PORTA, PIN12, GPIO_MODE_INPUT);

    /* Configure AIN13 to be used */
    HAL_GPIO_SetFunction(PORTA, PIN13, PA13_MUX_AN13);
    HAL_GPIO_SetMode(PORTA, PIN13, GPIO_MODE_INPUT);

    /* Configure AIN14 to be used */
    HAL_GPIO_SetFunction(PORTA, PIN14, PA14_MUX_AN14);
    HAL_GPIO_SetMode(PORTA, PIN14, GPIO_MODE_INPUT);

    /* Configure AIN15 to be used */
    HAL_GPIO_SetFunction(PORTA, PIN15, PA15_MUX_AN15);
    HAL_GPIO_SetMode(PORTA, PIN15, GPIO_MODE_INPUT);

    /* Configure AIN16 to be used */
    HAL_GPIO_SetFunction(PORTD, PIN4, PD4_MUX_AN16);
    HAL_GPIO_SetMode(PORTD, PIN4, GPIO_MODE_INPUT);

    /* Configure AIN17 to be used */
    HAL_GPIO_SetFunction(PORTD, PIN5, PD5_MUX_AN17);
    HAL_GPIO_SetMode(PORTD, PIN5, GPIO_MODE_INPUT);

    /* Configure AIN18 to be used */
    HAL_GPIO_SetFunction(PORTD, PIN6, PD6_MUX_AN18);
    HAL_GPIO_SetMode(PORTD, PIN6, GPIO_MODE_INPUT);

    /* Configure AIN19 to be used */
    HAL_GPIO_SetFunction(PORTD, PIN7, PD7_MUX_AN19);
    HAL_GPIO_SetMode(PORTD, PIN7, GPIO_MODE_INPUT);

    /* Configure AIN20 to be used */
    HAL_GPIO_SetFunction(PORTE, PIN1, PE1_MUX_AN20);
    HAL_GPIO_SetMode(PORTE, PIN1, GPIO_MODE_INPUT);

    /* Configure AIN21 to be used */
    HAL_GPIO_SetFunction(PORTE, PIN2, PE2_MUX_AN21);
    HAL_GPIO_SetMode(PORTE, PIN2, GPIO_MODE_INPUT);

    /* Configure AIN22 to be used */
    HAL_GPIO_SetFunction(PORTF, PIN2, PF2_MUX_AN22);
    HAL_GPIO_SetMode(PORTF, PIN2, GPIO_MODE_INPUT);

    /* Configure AIN23 to be used */
    HAL_GPIO_SetFunction(PORTF, PIN3, PF3_MUX_AN23);
    HAL_GPIO_SetMode(PORTF, PIN3, GPIO_MODE_INPUT);
}

