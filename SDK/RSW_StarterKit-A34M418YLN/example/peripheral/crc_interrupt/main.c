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

#include "crc.h"

#define APP_CRC_DATA_STRING     1
#define APP_CRC_DATA_RAM        2

/*
 * CRC input data to test can be selected
 */
#define APP_CRC_DATA_TYPE       APP_CRC_DATA_STRING


#if (APP_CRC_DATA_TYPE == APP_CRC_DATA_STRING)

static uint8_t test_string[] =
{
    '0', '1', '2', '3'
};

#define APP_CRC_DATA            test_string
#define APP_CRC_SIZE            sizeof(test_string)

#elif (APP_CRC_DATA_TYPE == APP_CRC_DATA_RAM)

#define APP_CRC_DATA            (uint8_t *)0x2000000
#define APP_CRC_SIZE            0x0FFF

#endif

static volatile uint8_t crc_done;

static void app_crc_irq_handler(uint32_t event, void *context)
{
    if (event == CRC_EVENT_DONE)
    {
        crc_done = 1;
    }
}

static void app_crc7(void)
{
    uint32_t crc;
    struct crc_cfg cfg = APP_CRC_CONFIG;

    LOG("CRC7\n");

    HAL_CRC_Init(&cfg, app_crc_irq_handler, NULL);
    HAL_CRC_SetDMA(DMA_ID_0, 1);
    crc_done = 0;
    HAL_CRC_Compute(0, APP_CRC_DATA, APP_CRC_SIZE, &crc);
    while (crc_done == 0)
        ;
    LOG("HW = 0x%08x\n", crc);

    crc = crc7_be(0, APP_CRC_DATA, APP_CRC_SIZE);
    LOG("SW = 0x%08x\n", (crc >> 1));
}

static void app_crc8(void)
{
    uint32_t crc;
    struct crc_cfg cfg = APP_CRC_CONFIG;

    cfg.poly = CRC_POLY_CRC8;

    LOG("CRC8\n");

    HAL_CRC_Init(&cfg, app_crc_irq_handler, NULL);
    HAL_CRC_SetDMA(DMA_ID_0, 1);
    crc_done = 0;
    HAL_CRC_Compute(0, APP_CRC_DATA, APP_CRC_SIZE, &crc);
    while (crc_done == 0)
        ;
    LOG("HW = 0x%08x\n", crc);

    crc = crc8_ccitt(0, APP_CRC_DATA, APP_CRC_SIZE);
    LOG("SW = 0x%08x\n", crc);
}

static void app_crc16(void)
{
    uint32_t crc;
    struct crc_cfg cfg = APP_CRC_CONFIG;

    cfg.in_rev = 1;
    cfg.poly = CRC_POLY_CRC16;

    LOG("CRC16\n");

    HAL_CRC_Init(&cfg, app_crc_irq_handler, NULL);
    HAL_CRC_SetDMA(DMA_ID_0, 1);
    crc_done = 0;
    HAL_CRC_Compute(0xFFFF, APP_CRC_DATA, APP_CRC_SIZE, &crc);
    while (crc_done == 0)
        ;
    LOG("HW = 0x%08x\n", crc);

    /*
     * Note: CRC16 implementation is differnt from standard
     * Use software for CRC16
     */
    crc = crc16_ansi(APP_CRC_DATA, APP_CRC_SIZE);
    LOG("SW = 0x%08x\n", crc);
    crc = crc16_itu_t(0xFFFF, APP_CRC_DATA, APP_CRC_SIZE);
    LOG("SW = 0x%08x\n", crc);
    crc = crc16_ccitt(0xFFFF, APP_CRC_DATA, APP_CRC_SIZE);
    LOG("SW = 0x%08x\n", crc);
}

static void app_crc32(void)
{
    uint32_t crc;
    struct crc_cfg cfg = APP_CRC_CONFIG;

    cfg.out_inv = 1;
    cfg.out_rev = 1;
    cfg.in_rev = 1;
    cfg.poly = CRC_POLY_CRC32;

    LOG("CRC32\n");

    HAL_CRC_Init(&cfg, app_crc_irq_handler, NULL);
    HAL_CRC_SetDMA(DMA_ID_0, 1);
    crc_done = 0;
    HAL_CRC_Compute(0xFFFFFFFFUL, APP_CRC_DATA, APP_CRC_SIZE, &crc);
    while (crc_done == 0)
        ;
    LOG("HW = 0x%08x\n", crc);

    crc = crc32_ieee(APP_CRC_DATA, APP_CRC_SIZE);
    LOG("SW = 0x%08x\n", crc);
}

static void mainloop(void)
{
    app_crc7();
    app_crc8();
    app_crc16();
    app_crc32();

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
    LOG("\t CRC DMA\n");
    LOG("************************************************\n");

    /* Start main loop */
    mainloop();
}

