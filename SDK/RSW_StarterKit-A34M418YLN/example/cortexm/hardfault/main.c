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

static void corrupt_stack(uint8_t *buffer)
{
    uint8_t i;

    /* overwrite stack */
    for (i = 0; i < 64; i++)
    {
        buffer[i] = i;
    }
}

static enum debug_cmd_status cmd_fault(int argc, char *argv[])
{
    uint8_t buffer;

    LOG("causing fault\n");

    SystemDelay(100);

    corrupt_stack(&buffer);

    return DEBUG_CMD_SUCCESS;
}

static const struct debug_cmd test_commands[] =
{
    { "fault", cmd_fault, "cause hard fault by stack corruption" },
};

static void mainloop(void)
{
    while (1)
    {
        /* if there is a command ready, process it */
        debug_cmd_process();
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
    LOG("\t hardfault\n");
    LOG("************************************************\n");

    /* Initialze command line processing */
    debug_cmd_init(test_commands, DEBUG_CMD_LIST_COUNT(test_commands));

    /* Start main loop */
    mainloop();
}

