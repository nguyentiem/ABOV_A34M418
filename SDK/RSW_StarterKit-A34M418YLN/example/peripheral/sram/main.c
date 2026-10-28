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

/* macro to access 32bit value */
#define VALUE32(addr)       (*(volatile unsigned int *)(addr))


static enum debug_cmd_status cmd_sram_erase(int argc, char *argv[])
{
    uint32_t i;

    LOG("erasing...");

    for (i = APP_SRAM_REGION_OFFSET; i < APP_SRAM_REGION_SIZE; i = i + 4)
    {
        VALUE32(APP_SRAM_REGION_BASE + i) = APP_SRAM_ERASE_VALUE;
    }

    LOG("done\n");

    return DEBUG_CMD_SUCCESS;
}

static enum debug_cmd_status cmd_sram_write(int argc, char *argv[])
{
    uint32_t i;

    LOG("writing...");

    for (i = APP_SRAM_REGION_OFFSET; i < APP_SRAM_REGION_SIZE; i = i + 4)
    {
        VALUE32(APP_SRAM_REGION_BASE + i) = APP_SRAM_WRITE_VALUE;
    }

    LOG("done\n");

    return DEBUG_CMD_SUCCESS;
}

static enum debug_cmd_status cmd_sram_verify(int argc, char *argv[])
{
    uint32_t i;
    uint32_t value;
    uint8_t success;

    LOG("verifying...");

    success = 1;

    for (i = APP_SRAM_REGION_OFFSET; i < APP_SRAM_REGION_SIZE; i = i + 4)
    {
        value = VALUE32(APP_SRAM_REGION_BASE + i);

        if (value != APP_SRAM_WRITE_VALUE)
        {
            success = 0;
            break;
        }
    }

    if (success)
    {
        LOG("success\n");
    }
    else
    {
        LOG("failure\n");
    }

    return DEBUG_CMD_SUCCESS;
}

static const struct debug_cmd test_commands[] =
{
    { "erase", cmd_sram_erase, "erase sram" },
    { "write", cmd_sram_write, "write sram" },
    { "verify", cmd_sram_verify, "verify sram" },
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
    LOG("\t SRAM 0x%08x ~ 0x%08x\n",
        APP_SRAM_REGION_BASE + APP_SRAM_REGION_OFFSET,
        APP_SRAM_REGION_BASE + APP_SRAM_REGION_SIZE);
    LOG("\t erase\n");
    LOG("\t write\n");
    LOG("\t verify\n");
    LOG("************************************************\n");

    /* Initialze command line processing */
    debug_cmd_init(test_commands, DEBUG_CMD_LIST_COUNT(test_commands));

    /* Start main loop */
    mainloop();
}

