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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "debug_cmd.h"
#include "debug_log.h"


/*
 * external dhrystone function to start
 */
extern void dhry_main(void);

/*
 * external dhrystone function to set iteration
 */
extern void dhry_set_iteration(int iteration);

/*
 * time function to be used for measuring performance
 */
uint32_t app_current_time(void)
{
    uint16_t count;

    HAL_TIMER_GetCount(TIMER_ID_0, &count);

    return count;
}

uint32_t app_cpu_freq(void)
{
    return APP_CPU_FREQ;
}

static void app_timer_config()
{
    /* reset and start timer */
    struct timer_cfg cfg =
    {
        .mode = TIMER_MODE_ONESHOT,
        .clk_sel = APP_TIMER_CLK_SEL,
        .prescaler = APP_TIMER_PRESCALER,
        .irq_prio = DRIVER_PRIORITY_LOWEST,
    };

    /* Initialize Timer */
    HAL_TIMER_Init(TIMER_ID_0, &cfg, NULL, NULL);
    HAL_TIMER_Start(TIMER_ID_0, 0xFFFF);
}

static void mainloop(void)
{
    int iteration = 100000;

    if (((uint32_t)dhry_main > 0x20000000) && ((uint32_t)dhry_main < 0x2FFFFFFF))
    {
        LOG("ram code\n");
    }
    else
    {
        LOG("flash code\n");
    }

    LOG("Running dhrystone: %d times\n", iteration);

    /* start timer */
    app_timer_config();

    /* set the number of iteration */
    dhry_set_iteration(iteration);

    /* start Dhrystone */
    dhry_main();

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
    LOG("\t Dhrystone\n");
    LOG("************************************************\n");

    /* Start main loop */
    mainloop();
}

