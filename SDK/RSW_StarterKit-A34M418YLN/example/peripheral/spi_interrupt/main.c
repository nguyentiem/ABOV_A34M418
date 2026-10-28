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
#include <stdlib.h>

#include "main.h"
#include "debug_log.h"
#include "debug_cmd.h"

#define DATA_MAX_LEN        100
volatile uint8_t complete = 0;
volatile uint8_t trans_len = 0;

static uint8_t send_val[DATA_MAX_LEN];
static uint8_t send_val2[DATA_MAX_LEN];
static uint8_t recv_val[DATA_MAX_LEN];

struct spi_cfg app_spi_cfg = APP_SPI_CONFIG;

void app_spi_irq_handler(uint32_t event, void *context)
{
    if (event & SPI_EVENT_TRANSFER_DONE)
    {
        complete = 1;
    }

    if (event & SPI_EVENT_OVERRUN)
    {
        LOG("SPI_EVENT_OVERRUN\n");
    }

    if (event & SPI_EVENT_UNDERRUN)
    {
        LOG("SPI_EVENT_UNDERRUN\n");
    }
}

static enum debug_cmd_status cmd_master_send(int argc, char *argv[])
{
    uint8_t len;
    uint32_t timeout = 0x10000000;

    if (argc < 2)
    {
        return DEBUG_CMD_INVALID;
    }
    
    if (app_spi_cfg.mode == SPI_MODE_SLAVE)
    {
        HAL_SPI_Deinit(SPI_ID_0);
    }

    len = (uint8_t)atoi(argv[1]);
    app_spi_cfg.mode = SPI_MODE_MASTER;

    HAL_SPI_Init(SPI_ID_0, &app_spi_cfg, app_spi_irq_handler, NULL);

    HAL_SPI_SetTransmitDMA(SPI_ID_0, DMA_ID_2, APP_SPI_DMA_ENABLE);
    HAL_SPI_SetReceiveDMA(SPI_ID_0, DMA_ID_3, APP_SPI_DMA_ENABLE);

    complete = 0;
    HAL_SPI_Transfer(SPI_ID_0, send_val, len, recv_val, len, SPI_IO_TYPE_NON_BLOCK);
    while (!complete)
    {
        timeout--;
        if(timeout==0) break;   /* do nothing*/
    }

    LOG("master send val :");
    LOG_HEX(send_val, len);
    LOG("\n");

    LOG("master recv val :");
    LOG_HEX(recv_val, len);
    LOG("\n");

    return DEBUG_CMD_SUCCESS;
}



static enum debug_cmd_status cmd_slave_recv(int argc, char *argv[])
{
    uint8_t len;

    if (argc < 2)
    {
        return DEBUG_CMD_INVALID;
    }
    if (app_spi_cfg.mode == SPI_MODE_MASTER)
    {
        HAL_SPI_Deinit(SPI_ID_0);
    }

    len = (uint8_t)atoi(argv[1]);

    app_spi_cfg.mode = SPI_MODE_SLAVE;
    HAL_SPI_Init(SPI_ID_0, &app_spi_cfg, app_spi_irq_handler, NULL);

    HAL_SPI_SetTransmitDMA(SPI_ID_0, DMA_ID_2, APP_SPI_DMA_ENABLE);
    HAL_SPI_SetReceiveDMA(SPI_ID_0, DMA_ID_3, APP_SPI_DMA_ENABLE);
    complete = 0;
    trans_len = len;
    HAL_SPI_Transfer(SPI_ID_0, send_val2, len, recv_val, len, SPI_IO_TYPE_NON_BLOCK);

    while (!complete)
    {
    }

    LOG("slave recv val :");
    LOG_HEX(recv_val, len);
    LOG("\n");
    
    LOG("slave send val :");
    LOG_HEX(send_val2, len);
    LOG("\n");

    return DEBUG_CMD_SUCCESS;
}

static const struct debug_cmd test_commands[] =
{
    { "ms", cmd_master_send, "send value to slave", "\targ1 = send len , ex) ms 010\n" },
    { "sr", cmd_slave_recv, "recv value from master", "\targ1 = recv len , ex) sr 010\n" },
};

static void mainloop(void)
{
    uint8_t i;

    for (i = 1; i <= DATA_MAX_LEN; i++)
    {
        send_val[i - 1] = i;
        send_val2[DATA_MAX_LEN - i] = i;
    }
    app_spi_cfg.mode = SPI_MODE_SLAVE;
    HAL_SPI_Init(SPI_ID_0, &app_spi_cfg, app_spi_irq_handler, NULL);

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

    /* Configure SPI pins */
    Port_Configure_SPI();

    LOG("\n");
    LOG("************************************************\n");
    LOG("\t MCU: %s\n", APP_DEVICE);
    LOG("\t Core: %s\n", APP_CORE);
    LOG("\t Debug: UART%d - %dbps\n", APP_UART_ID, APP_UART_BAUD);
    LOG("\t %s, %s\n", __DATE__, __TIME__);
    LOG("\t SPI interrupt test\n");
    LOG("************************************************\n");

    /* Initialze command line processing */
    debug_cmd_init(test_commands, DEBUG_CMD_LIST_COUNT(test_commands));

    /* enable irq */
    __enable_irq();

    /* Start main loop */
    mainloop();
}

