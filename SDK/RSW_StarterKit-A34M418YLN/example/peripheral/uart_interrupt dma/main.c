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


static uint8_t app_uart_rx_buf[1];
static uint8_t app_uart_tx_buf[1];

static volatile uint8_t app_uart_tx_done;
static volatile uint8_t app_uart_rx_done;

static void app_uart_irq_handler(uint32_t event, void *context)
{
    int32_t ret;

    if (event & UART_EVENT_RECEIVE_DONE)
    {
        /* UART received, assign received byte to be transmitted (loopback) */
        app_uart_tx_buf[0] = app_uart_rx_buf[0];

        /* Receive 1 byte again */
        ret = HAL_UART_Receive(UART_ID_0, app_uart_rx_buf, 1, UART_IO_TYPE_NON_BLOCK);
        if (ret != DRIVER_ERROR_OK)
        {
            /* error occured */
        }

        /* Transmit received byte */
        ret = HAL_UART_Transmit(UART_ID_0, app_uart_tx_buf, 1, UART_IO_TYPE_NON_BLOCK);
        if (ret != DRIVER_ERROR_OK)
        {
            /* error occured */
        }

        app_uart_rx_done = 1;
    }

    if (event & UART_EVENT_TRANSMIT_DONE)
    {
        /* do nothing */
        app_uart_tx_done = 1;
    }

    /* error handling */
}

static void mainloop(void)
{
    static const char *msg =
        "\r\n"
        "************************************************\r\n"
        "\t MCU: " APP_DEVICE "\r\n"
        "\t Core: " APP_CORE "\r\n"
        "\t " __DATE__ __TIME__ "\r\n"
        "\t UART Interrupt\r\n"
        "************************************************\r\n"
        "UART interrupt Loopback - type in a character\r\n"
    ;

    struct uart_cfg cfg =
    {
        .baudrate = APP_UART_BAUD,
        .data_bit = UART_DATA_BIT_8,
        .parity_bit = UART_PARITY_BIT_NONE,
        .stop_bit = UART_STOP_BIT_1,
        .irq_prio = DRIVER_PRIORITY_LOWEST,
    };

    /* Initialize UART driver */
    HAL_UART_Init(UART_ID_0, &cfg, app_uart_irq_handler, NULL);

#if (APP_UART_DMA_ENABLE == 1)
    /* Enable DMA */
    HAL_UART_SetTransmitDMA(UART_ID_0, DMA_ID_0, 1);
    HAL_UART_SetReceiveDMA(UART_ID_0, DMA_ID_1, 1);
#endif

    /* Print message */
    HAL_UART_Transmit(UART_ID_0, (uint8_t *)msg, strlen(msg), UART_IO_TYPE_NON_BLOCK);

    /* Wait for the messages to be displayed */
    SystemDelay(500);

    /* Receive 1 byte */
    HAL_UART_Receive(UART_ID_0, app_uart_rx_buf, 1, UART_IO_TYPE_NON_BLOCK);

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

    /* Configure UART pins */
    Port_Configure_UART();

    /* Start main loop */
    mainloop();
}

