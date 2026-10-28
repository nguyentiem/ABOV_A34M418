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

static struct led_def app_led0 = LED0;
static struct led_def app_led1 = LED1;

static void app_comp_irq_handler(uint32_t event, void *context)
{
    uint8_t status;

    HAL_COMP_GetStatus(COMP_ID_0, &status);

    if (status == COMP_STATUS_UP)
    {
        HAL_GPIO_SetPin(app_led0.port, app_led0.pin);
        HAL_GPIO_ClearPin(app_led1.port, app_led1.pin);
    }
    else
    {
        HAL_GPIO_SetPin(app_led1.port, app_led1.pin);
        HAL_GPIO_ClearPin(app_led0.port, app_led0.pin);
    }
}

static void mainloop(void)
{
    struct comp_cfg cfg = APP_COMP_CONFIG;

    HAL_COMP_Init(COMP_ID_0, &cfg, app_comp_irq_handler, NULL);
    HAL_COMP_Start(COMP_ID_0);

    while (1)
    {
        /* wait for interrupt and flag check*/
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

    /* Port configure COMP */
    Port_Configure_COMP();

    LOG("\n");
    LOG("************************************************\n");
    LOG("\t MCU: %s\n", APP_DEVICE);
    LOG("\t Core: %s\n", APP_CORE);
    LOG("\t Debug: UART%d - %dbps\n", APP_UART_ID, APP_UART_BAUD);
    LOG("\t %s, %s\n", __DATE__, __TIME__);
    LOG("\t Comparator Interrupt\n");
    LOG("************************************************\n");

    /* Start main loop */
    mainloop();
}

