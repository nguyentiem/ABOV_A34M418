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
#include <string.h>

#include "main.h"
#include "debug_log.h"

static struct app_gpio_def app_led = APP_GPIO_LED;
static struct app_gpio_def app_inp = APP_GPIO_INP;

static void sleep_enable(void)
{
    HAL_GPIO_SetPin(app_led.port, app_led.pin);

    LOG("Entering Sleep After 1S (LED off)...Connect PB2 pin to GND to exit sleep mode!\n");

    SystemDelay(1000);

    HAL_GPIO_ClearPin(app_led.port, app_led.pin);

    /* Enter PowerDown Mode */
    HAL_PWR_EnterSleepMode();

    LOG("\nWaked Up from sleep mode!\n");

    HAL_GPIO_SetPin(app_led.port, app_led.pin);
}

static void mainloop(void)
{
    /* Set mode to output */
    HAL_GPIO_SetMode(app_led.port, app_led.pin, GPIO_MODE_OUTPUT);

    /* Set output type to push pull */
    HAL_GPIO_SetOutput(app_led.port, app_led.pin, GPIO_OUTPUT_PUSH_PULL);

    /* clear led */
    HAL_GPIO_ClearPin(app_led.port, app_led.pin);

    /* Set mode as input */
    HAL_GPIO_SetMode(app_inp.port, app_inp.pin, GPIO_MODE_INPUT);

    /* Set pull up */
    HAL_GPIO_SetPull(app_inp.port, app_inp.pin, GPIO_PULL_UP);

    /* Enable interrupt */
    HAL_GPIO_EnableIRQ(app_inp.port, app_inp.pin,
                       GPIO_IRQ_TRIG_EDGE,
                       GPIO_IRQ_FALLING_AND_RISING,
                       DRIVER_PRIORITY_LOWEST);

    /* Run sleep Mode */
    sleep_enable();

    while (1)
    {
    }
}

int main(void)
{
    /* Initialize all port */
    Port_Init();

    /* Configure system settings */
    System_Setting_Config();

    /* Initialize UART Debugging framework to use logging or command input */
    Debug_Init();

    LOG("\n");
    LOG("************************************************\n");
    LOG("\t MCU: %s\n", APP_DEVICE);
    LOG("\t Core: %s\n", APP_CORE);
    LOG("\t Debug: UART%d - %dbps\n", APP_UART_ID, APP_UART_BAUD);
    LOG("\t %s, %s\n", __DATE__, __TIME__);
    LOG("\t sleep mode\n");
    LOG("************************************************\n");

    /* enable irq */
    __enable_irq();

    /* Start main loop */
    mainloop();
}

