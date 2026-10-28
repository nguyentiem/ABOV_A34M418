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


static struct app_gpio_def app_btn = APP_GPIO_BTN;
static struct app_gpio_def app_led = APP_GPIO_LED;

static void mainloop(void)
{
    /* Set mode as input */
    HAL_GPIO_SetMode(app_btn.port, app_btn.pin, GPIO_MODE_INPUT);

    /* Set pull up */
    HAL_GPIO_SetPull(app_btn.port, app_btn.pin, GPIO_PULL_UP);

    /* Set mode to output */
    HAL_GPIO_SetMode(app_led.port, app_led.pin, GPIO_MODE_OUTPUT);

    /* Set output type to push pull */
    HAL_GPIO_SetOutput(app_led.port, app_led.pin, GPIO_OUTPUT_PUSH_PULL);

    while (1)
    {
        LOG("GPIO LED Set\n");
        HAL_GPIO_SetPin(app_led.port, app_led.pin);
        SystemDelay(500);

        LOG("GPIO LED Clear\n");
        HAL_GPIO_ClearPin(app_led.port, app_led.pin);
        SystemDelay(500);
    }
}

int main(void)
{
    /* Initialize all port */
    Port_Init();

    /* Configure system settings external main oscillator 8Mhz */
    System_Setting_Config();

    /* Initialize UART Debugging framework to use logging or command input */
    Debug_Init();

    LOG("\n");
    LOG("************************************************\n");
    LOG("\t MCU: %s\n", APP_DEVICE);
    LOG("\t Core: %s\n", APP_CORE);
    LOG("\t Debug: UART%d - %dbps\n", APP_UART_ID, APP_UART_BAUD);
    LOG("\t %s, %s\n", __DATE__, __TIME__);
    LOG("\t Clock Setting - External main oscillator 8Mhz \n");
    LOG("************************************************\n");

    /* Start main loop */
    mainloop();
}

