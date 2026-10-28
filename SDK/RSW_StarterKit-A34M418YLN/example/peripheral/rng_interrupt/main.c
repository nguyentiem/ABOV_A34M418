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
#include "debug_cmd.h"
#include "debug_log.h"

static uint32_t value;
static int count;

static void app_rng_irq_handler(uint32_t event, void *context)
{
    if (event == RNG_EVENT_DONE)
    {
        LOG("random number = 0x%08x\n\r", value);
        count++;
    }
    else
    {
        LOG("error occured\r\n");
    }

        if(count>=10) 
        {
            HAL_RNG_Deinit();
        }
}

static void mainloop(void)
{
    struct rng_cfg cfg = APP_RNG_CONFIG;

    /* initialize RNG */
    HAL_RNG_Init(&cfg, app_rng_irq_handler, NULL);

    /* request to generate a random number */
    HAL_RNG_Generate(&value, RNG_IO_TYPE_NON_BLOCK);

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
    LOG("\t Random Number Generation Interrupt\n");
    LOG("************************************************\n");

    /* enable irq */
    __enable_irq();

    /* Start main loop */
    mainloop();
}

