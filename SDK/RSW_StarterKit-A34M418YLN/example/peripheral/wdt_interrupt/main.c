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

static void app_wdt_irq_handler(unsigned long *stacked_regs, void *context)
{
    /*
     * print out where the processor was at when the watchdog occured
     * for this example, the program counter should be somewhere inside
     * the mainloop near "while (1)".
     * One can verify this with the program counter values and the map
     * file listing all the function addresses.
     * Cortex M contains the stacked register values. See startup file.
     */
    LOG("WDT interrupt\n");
    LOG("LR=0x%08x\n", ((unsigned long *)stacked_regs)[5]);
    LOG("PC=0x%08x\n", ((unsigned long *)stacked_regs)[6]);

    /* feed the Watchdog */
    HAL_WDT_Feed();
}

static void mainloop(void)
{
    struct wdt_cfg cfg = APP_WDT_CONFIG;
    uint32_t wdt_feed = 0;
    uint32_t wdt_counter;

    /* Initialize Watchdog */
    HAL_WDT_Init(&cfg, app_wdt_irq_handler, NULL);

    /* Start Watchdog: e.g. 5 seconds */
    HAL_WDT_Start(5 * SystemPeriClock);

    while (1)
    {
        /* feed the watchdog for a while */
        if (wdt_feed < 5)
        {
            wdt_feed++;
            HAL_WDT_Feed();
        }

        HAL_WDT_GetCounter(&wdt_counter);
        LOG("WDT: %d\n", wdt_counter);

        SystemDelay(1000);
    }
}

int main(void)
{
    /* Initialize all port */
    Port_Init();

    /* Configure system settings */
    System_Setting_Config();

    /* enable irq */
    __enable_irq();

    /* Initialize UART Debugging framework to use logging or command input */
    Debug_Init();

    LOG("\n");
    LOG("************************************************\n");
    LOG("\t MCU: %s\n", APP_DEVICE);
    LOG("\t Core: %s\n", APP_CORE);
    LOG("\t Debug: UART%d - %dbps\n", APP_UART_ID, APP_UART_BAUD);
    LOG("\t %s, %s\n", __DATE__, __TIME__);
    LOG("\t Watchdog Timer Interrupt\n");
    LOG("************************************************\n");

    /* Start main loop */
    mainloop();
}

