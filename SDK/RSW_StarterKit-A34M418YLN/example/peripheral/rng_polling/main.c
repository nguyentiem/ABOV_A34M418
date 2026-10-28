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


static void mainloop(void)
{
    struct rng_cfg cfg = APP_RNG_CONFIG;
    uint32_t value;
    int32_t ret;

    /* initialize RNG */
    HAL_RNG_Init(&cfg, NULL, NULL);

    /* request to generate a random number */
    ret = HAL_RNG_Generate(&value, RNG_IO_TYPE_BLOCK);
    if (ret == DRIVER_ERROR_OK)
    {
        LOG("random number = 0x%08x\n", value);
    }
    else
    {
        LOG("error occured\n");
    }

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
    LOG("\t Random Number Generation Polling\n");
    LOG("************************************************\n");

    /* Start main loop */
    mainloop();
}

