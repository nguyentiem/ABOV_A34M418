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
#include "debug_cmd.h"
#include "debug_log.h"

static void mainloop(void)
{
    struct mpwm_cfg cfg = APP_MPWM_CONFIG;

    HAL_MPWM_Init(MPWM_ID_0, &cfg, NULL, NULL);
    HAL_MPWM_SetPeriod(MPWM_ID_0, MPWM_PHASE_TYPE_ALL, 1000);
    HAL_MPWM_SetDuty(MPWM_ID_0, MPWM_PHASE_TYPE_U, MPWM_OUTPUT_TYPE_H, 500);
    HAL_MPWM_SetDuty(MPWM_ID_0, MPWM_PHASE_TYPE_U, MPWM_OUTPUT_TYPE_L, 500);
    HAL_MPWM_SetDuty(MPWM_ID_0, MPWM_PHASE_TYPE_V, MPWM_OUTPUT_TYPE_H, 400);
    HAL_MPWM_SetDuty(MPWM_ID_0, MPWM_PHASE_TYPE_V, MPWM_OUTPUT_TYPE_L, 400);
    HAL_MPWM_SetDuty(MPWM_ID_0, MPWM_PHASE_TYPE_W, MPWM_OUTPUT_TYPE_H, 300);
    HAL_MPWM_SetDuty(MPWM_ID_0, MPWM_PHASE_TYPE_W, MPWM_OUTPUT_TYPE_L, 300);

    HAL_MPWM_Start(MPWM_ID_0, MPWM_PHASE_TYPE_ALL);

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

    /* Configure MPWM pins */
    Port_Configure_MPWM();

    /* Initialize UART Debugging framework to use logging or command input */
    Debug_Init();

    LOG("\n");
    LOG("************************************************\n");
    LOG("\t MCU: %s\n", APP_DEVICE);
    LOG("\t Core: %s\n", APP_CORE);
    LOG("\t Debug: UART%d - %dbps\n", APP_UART_ID, APP_UART_BAUD);
    LOG("\t %s, %s\n", __DATE__, __TIME__);
    LOG("\t MPWM Motor\n");
    LOG("************************************************\n");

    /* enable irq */
     __enable_irq();

    /* Start main loop */
    mainloop();
}
