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
    /* FRT configuration */
    struct frt_cfg cfg = APP_FRT_CONFIG;

    /* Initialize the FRT with the configuration */
    HAL_FRT_Init(FRT_ID_0, &cfg, NULL, NULL);

    /* Set the FRT clock */
    HAL_FRT_SetClock(FRT_ID_0, APP_FRT_CLK, 1);

    /* Set the FRT match counter to a desired value */
    HAL_FRT_SetMatchCounter(FRT_ID_0, 0x1000000);

    /* FRT count clear */
    HAL_FRT_ClearCounter(FRT_ID_0);

    /* Enable the FRT */
    HAL_FRT_Start(FRT_ID_0);

    while (1)
    {
        uint32_t counter;

        /* counter value varies from 0 to 0x10000000 */
        HAL_FRT_GetCounter(FRT_ID_0, &counter);

        LOG("FRT count = 0x%08x\n", counter);

        /* note: when frt runs in polling mode, frt match flag is not set. */

        SystemDelay(100);
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
    LOG("\t FRT polling\n");
    LOG("************************************************\n");

    /* Start main loop */
    mainloop();
}

