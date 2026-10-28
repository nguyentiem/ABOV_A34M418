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

#include "main.h"
#include "debug_log.h"

static void app_timer_irq_handler(uint32_t event, void *context)
{
    if (event & TIMER_EVENT_COUNT_MATCH)
    {
        LOG("Timer expired after 1s\n");
    }
}

static void mainloop(void)
{
    struct timer_cfg cfg = APP_TIMER_CONFIG;

    /* Initialize Timer */
    HAL_TIMER_Init(TIMER_ID_0, &cfg, app_timer_irq_handler, NULL);

    LOG("Timer start oneshot\n");

    /* Start Timer */
    HAL_TIMER_StartPWM(TIMER_ID_0, TIMER_PWM_START_LOW, 300, 1000, 0);

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

    /* Configure PWM pin */
    Port_ConfigureTimerOut();

    LOG("\n");
    LOG("************************************************\n");
    LOG("\t MCU: %s\n", APP_DEVICE);
    LOG("\t Core: %s\n", APP_CORE);
    LOG("\t Debug: UART%d - %dbps\n", APP_UART_ID, APP_UART_BAUD);
    LOG("\t %s, %s\n", __DATE__, __TIME__);
    LOG("\t Timer Oneshot\n");
    LOG("************************************************\n");

    /* enable irq */
    __enable_irq();

    /* Start main loop */
    mainloop();
}
