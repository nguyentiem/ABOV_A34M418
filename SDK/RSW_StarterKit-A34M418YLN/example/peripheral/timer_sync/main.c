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
    uint8_t timer_id = *(uint8_t *)context;

    if (event & TIMER_EVENT_PWM_DUTY)
    {
        LOG("timer %d pwm duty\n", timer_id);
    }

    if (event & TIMER_EVNET_PWM_PERIOD)
    {
        LOG("timer %d pwm prd\n", timer_id);
    }
}

static void mainloop(void)
{
    struct timer_cfg cfg = APP_TIMER_CONFIG;

    static const uint8_t timer0_context = TIMER_ID_0;
    static const uint8_t timer1_context = TIMER_ID_1;

    /* Initialize Timer */
    HAL_TIMER_Init(TIMER_ID_0, &cfg, app_timer_irq_handler, (void *)&timer0_context);
    HAL_TIMER_Init(TIMER_ID_1, &cfg, app_timer_irq_handler, (void *)&timer1_context);

    /* Start timer 0 */
    HAL_TIMER_StartPWM(TIMER_ID_0, TIMER_PWM_START_LOW, 500, 1000,
                       TIMER_PWM_EVENT_DUTY | TIMER_PWM_EVENT_PERIOD);

    /* delay before starting timer 1 to make timers not synchronized */
    SystemDelay(25);

	    /* Synchronize the timer1 to the timer 0 */
    HAL_TIMER_EnableSynch(TIMER_ID_1, TIMER_ID_0,
                          TIMER_SYNC_START_ENABLE,
                          TIMER_SYNC_CLEAR_ENABLE,
                          0);
	
    /* Start timer 1 */
    HAL_TIMER_StartPWM(TIMER_ID_1, TIMER_PWM_START_LOW, 500, 1000,
                       TIMER_PWM_EVENT_DUTY | TIMER_PWM_EVENT_PERIOD);

    /* delay for a while to see if the timers are running not synchronized */
    SystemDelay(2000);



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
    Port_ConfigureTimerPWM();

    LOG("\n");
    LOG("************************************************\n");
    LOG("\t MCU: %s\n", APP_DEVICE);
    LOG("\t Core: %s\n", APP_CORE);
    LOG("\t Debug: UART%d - %dbps\n", APP_UART_ID, APP_UART_BAUD);
    LOG("\t %s, %s\n", __DATE__, __TIME__);
    LOG("\t Timer sync\n");
    LOG("************************************************\n");

    /* enable irq */
    __enable_irq();

    /* Start main loop */
    mainloop();
}
