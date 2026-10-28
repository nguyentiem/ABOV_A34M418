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
#include <stdlib.h>

#include "main.h"
#include "debug_cmd.h"
#include "debug_log.h"


static void app_qei_irq_handler(uint32_t event, void *context)
{
    if (event & QEI_EVENT_ERROR)
    {
    }

    if (event & QEI_EVENT_DIRECTION)
    {
    }

    if (event & QEI_EVENT_FORWARD || event & QEI_EVENT_BACKWARD)
    {
        int32_t count;

        HAL_QEI_Read(QEI_ID_0, &count);
        if (event & QEI_EVENT_FORWARD)
        {
            LOG("QEI foward,   %d\n", count);
        }
        else
        {
            LOG("QEI backward, %d\n", count);
        }
    }

    if (event & QEI_EVENT_POS0)
    {
        LOG("QEI Position 0\n");
    }
    if (event & QEI_EVENT_POS1)
    {
        LOG("QEI Position 1\n");
    }
    if (event & QEI_EVENT_POS2)
    {
        LOG("QEI Position 2\n");
    }
}

static void simulate_quadrature_encoder_start(uint8_t direction, uint8_t speed)
{
    LOG("simulate direction = %d, speed = %d\n", direction, speed);

    if (direction == 1 && speed == 1)
    {
        struct timer_cfg cfg = APP_TIMER_SIM_FAST_CONFIG;
        HAL_TIMER_Init(TIMER_ID_0, &cfg, NULL, NULL);
        HAL_TIMER_Init(TIMER_ID_1, &cfg, NULL, NULL);
        HAL_TIMER_StartPWM(TIMER_ID_0, TIMER_PWM_START_LOW, 1000, 2000, TIMER_PWM_EVENT_NONE);
        SystemDelay(50);
        HAL_TIMER_StartPWM(TIMER_ID_1, TIMER_PWM_START_LOW, 1000, 2000, TIMER_PWM_EVENT_NONE);
    }
    else if (direction == 0 && speed == 1)
    {
        struct timer_cfg cfg = APP_TIMER_SIM_FAST_CONFIG;
        HAL_TIMER_Init(TIMER_ID_0, &cfg, NULL, NULL);
        HAL_TIMER_Init(TIMER_ID_1, &cfg, NULL, NULL);

        HAL_TIMER_StartPWM(TIMER_ID_1, TIMER_PWM_START_LOW, 1000, 2000, TIMER_PWM_EVENT_NONE);
        SystemDelay(50);
        HAL_TIMER_StartPWM(TIMER_ID_0, TIMER_PWM_START_LOW, 1000, 2000, TIMER_PWM_EVENT_NONE);
    }
    else if (direction == 0 && speed == 0)
    {
        struct timer_cfg cfg = APP_TIMER_SIM_SLOW_CONFIG;
        HAL_TIMER_Init(TIMER_ID_0, &cfg, NULL, NULL);
        HAL_TIMER_Init(TIMER_ID_1, &cfg, NULL, NULL);

        HAL_TIMER_StartPWM(TIMER_ID_1, TIMER_PWM_START_LOW, 1000, 2000, TIMER_PWM_EVENT_NONE);
        SystemDelay(50);
        HAL_TIMER_StartPWM(TIMER_ID_0, TIMER_PWM_START_LOW, 1000, 2000, TIMER_PWM_EVENT_NONE);
    }
    else if (direction == 0 && speed == 0)
    {
        struct timer_cfg cfg = APP_TIMER_SIM_SLOW_CONFIG;
        HAL_TIMER_Init(TIMER_ID_0, &cfg, NULL, NULL);
        HAL_TIMER_Init(TIMER_ID_1, &cfg, NULL, NULL);

        HAL_TIMER_StartPWM(TIMER_ID_1, TIMER_PWM_START_LOW, 1000, 2000, TIMER_PWM_EVENT_NONE);
        SystemDelay(50);
        HAL_TIMER_StartPWM(TIMER_ID_0, TIMER_PWM_START_LOW, 1000, 2000, TIMER_PWM_EVENT_NONE);
    }
}

static void simulate_quadrature_encoder_stop(void)
{
    HAL_TIMER_Stop(TIMER_ID_0);
    HAL_TIMER_Stop(TIMER_ID_1);
}

static void mainloop(void)
{
    struct qei_cfg cfg = APP_QEI_CONFIG;

    HAL_QEI_Init(QEI_ID_0, &cfg, app_qei_irq_handler, NULL);
    HAL_QEI_EnablePositionCompare(QEI_ID_0, QEI_POS_COMP_0, 20);
    HAL_QEI_EnablePositionCompare(QEI_ID_0, QEI_POS_COMP_1, 30);
    HAL_QEI_EnablePositionCompare(QEI_ID_0, QEI_POS_COMP_2, 50);

    HAL_QEI_Start(QEI_ID_0);

    while (1)
    {
        /* if there is a command ready, process it */
        debug_cmd_process();
    }
}

static enum debug_cmd_status cmd_simulate_on(int argc, char *argv[])
{
    uint8_t direction;
    uint8_t speed;

    if (argc != 3)
    {
        return DEBUG_CMD_INVALID;
    }

    direction = atoi(argv[1]);
    speed = atoi(argv[2]);

    simulate_quadrature_encoder_start(direction, speed);

    return DEBUG_CMD_SUCCESS;
}

static enum debug_cmd_status cmd_simulate_off(int argc, char *argv[])
{
    simulate_quadrature_encoder_stop();

    return DEBUG_CMD_SUCCESS;
}

static enum debug_cmd_status cmd_clear(int argc, char *argv[])
{
    HAL_QEI_ClearCounter(QEI_ID_0, QEI_COUNTER_POSITION);

    return DEBUG_CMD_SUCCESS;
}

static const struct debug_cmd test_commands[] =
{
    { "on", cmd_simulate_on, "simulate on", "arg1=0(forward),1(reverse), arg2=0(slow),1(fast)" },
    { "off", cmd_simulate_off, "simulate off" },
    { "clear", cmd_clear, "clear counter" },
};

int main(void)
{
    /* Initialize all port */
    Port_Init();

    /* Configure system settings */
    System_Setting_Config();

    /* Configure QEI pins */
    Port_Configure_QEI();

    /* Configure PWM pin */
    Port_ConfigureTimerPWM();

    /* Initialize UART Debugging framework to use logging or command input */
    Debug_Init();

    LOG("\n");
    LOG("************************************************\n");
    LOG("\t MCU: %s\n", APP_DEVICE);
    LOG("\t Core: %s\n", APP_CORE);
    LOG("\t Debug: UART%d - %dbps\n", APP_UART_ID, APP_UART_BAUD);
    LOG("\t %s, %s\n", __DATE__, __TIME__);
    LOG("\t QEI Position\n");
    LOG("************************************************\n");

    /* enable irq */
    __enable_irq();

    /* Initialze command line processing */
    debug_cmd_init(test_commands, DEBUG_CMD_LIST_COUNT(test_commands));

    /* Start main loop */
    mainloop();
}

