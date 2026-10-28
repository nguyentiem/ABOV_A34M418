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


/* macro to access value at a specific memory address */
#define MEM_VALUE(addr)             *((volatile unsigned long  *)(addr))

/* macro to get bitband alias address for the corresponding memory address */
#define BIT_ADDR(addr, bitnum)      (              \
        (addr & 0xF0000000) + 0x2000000 +          \
        ((addr & 0x000FFFFF) << 5) + (bitnum << 2) \
        )

/* macro to access bitband value */
#define BIT_VALUE(addr, bitnum)     MEM_VALUE(BIT_ADDR(addr, bitnum))


#define TEST_RAM_ADDR       0x20001000
#define TEST_RAM_VALUE      0xA5


static void app_bitband(void)
{
    /* assign a value */
    MEM_VALUE(TEST_RAM_ADDR) = TEST_RAM_VALUE;

    /* log normal address and value */
    LOG("RAM ADDR = 0x%08x, VALUE = 0x%02x\n", TEST_RAM_ADDR, MEM_VALUE(TEST_RAM_ADDR));
    LOG("\n");

    /* read each bit using bit band address */
    LOG("BB0 ADDR = 0x%08x, VALUE = 0x%08x\n", BIT_ADDR(TEST_RAM_ADDR, 0), BIT_VALUE(TEST_RAM_ADDR, 0));
    LOG("BB1 ADDR = 0x%08x, VALUE = 0x%08x\n", BIT_ADDR(TEST_RAM_ADDR, 1), BIT_VALUE(TEST_RAM_ADDR, 1));
    LOG("BB2 ADDR = 0x%08x, VALUE = 0x%08x\n", BIT_ADDR(TEST_RAM_ADDR, 2), BIT_VALUE(TEST_RAM_ADDR, 2));
    LOG("BB3 ADDR = 0x%08x, VALUE = 0x%08x\n", BIT_ADDR(TEST_RAM_ADDR, 3), BIT_VALUE(TEST_RAM_ADDR, 3));
    LOG("BB4 ADDR = 0x%08x, VALUE = 0x%08x\n", BIT_ADDR(TEST_RAM_ADDR, 4), BIT_VALUE(TEST_RAM_ADDR, 4));
    LOG("BB5 ADDR = 0x%08x, VALUE = 0x%08x\n", BIT_ADDR(TEST_RAM_ADDR, 5), BIT_VALUE(TEST_RAM_ADDR, 5));
    LOG("BB6 ADDR = 0x%08x, VALUE = 0x%08x\n", BIT_ADDR(TEST_RAM_ADDR, 6), BIT_VALUE(TEST_RAM_ADDR, 6));
    LOG("BB7 ADDR = 0x%08x, VALUE = 0x%08x\n", BIT_ADDR(TEST_RAM_ADDR, 7), BIT_VALUE(TEST_RAM_ADDR, 7));
    LOG("\n");

    /* write each bit using bit band address */
    BIT_VALUE(TEST_RAM_ADDR, 0) = 0;
    BIT_VALUE(TEST_RAM_ADDR, 1) = 0;
    BIT_VALUE(TEST_RAM_ADDR, 2) = 1;
    BIT_VALUE(TEST_RAM_ADDR, 3) = 1;
    BIT_VALUE(TEST_RAM_ADDR, 4) = 0;
    BIT_VALUE(TEST_RAM_ADDR, 5) = 0;
    BIT_VALUE(TEST_RAM_ADDR, 6) = 1;
    BIT_VALUE(TEST_RAM_ADDR, 7) = 1;

    /* log normal address and value */
    LOG("RAM ADDR = 0x%08x, VALUE = 0x%02x\n", TEST_RAM_ADDR, MEM_VALUE(TEST_RAM_ADDR));
}

static void mainloop(void)
{
    app_bitband();

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
    LOG("\t bitband\n");
    LOG("************************************************\n");

    /* Start main loop */
    mainloop();
}

