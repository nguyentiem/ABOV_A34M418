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

static void mainloop(void)
{
    /* Set mode to output */
    HAL_GPIO_SetMode(app_led.port, app_led.pin, GPIO_MODE_OUTPUT);

    /* Set output type to push pull */
    HAL_GPIO_SetOutput(app_led.port, app_led.pin, GPIO_OUTPUT_PUSH_PULL);

    while (1)
    {
        HAL_GPIO_SetPin(app_led.port, app_led.pin);
        SystemDelay(500);

        HAL_GPIO_ClearPin(app_led.port, app_led.pin);
        SystemDelay(500);
    }
}

int main(void)
{
    /* Initialize all port */
    Port_Init();

    /* Configure system settings External sub-oscillator 32.768khz */
    System_Setting_Config();

#if 0
    /* Initialize UART Debugging framework to use logging or command input */
    Debug_Init();

    LOG("\n");
    LOG("************************************************\n");
    LOG("\t MCU: %s\n", APP_DEVICE);
    LOG("\t Core: %s\n", APP_CORE);
    LOG("\t Debug: UART%d - %dbps\n", APP_UART_ID, APP_UART_BAUD);
    LOG("\t %s, %s\n", __DATE__, __TIME__);
    LOG("\t Clock Setting - External sub-oscillator 32.768khz \n");
    LOG("************************************************\n");
#endif    

    /* Start main loop */
    mainloop();
}

