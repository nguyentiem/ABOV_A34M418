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

#define SLAVE_ADDR      0x49
#define TEST_DATA_LEN   8

static volatile uint32_t i2c_event = 0;

static uint8_t ms_tx_data[TEST_DATA_LEN] =
{
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
};
static uint8_t ms_rx_data[TEST_DATA_LEN] =
{
    0x00,
};

static uint8_t ms_burst_tx_reg = 0x8A;
static uint8_t ms_burst_tx_data[TEST_DATA_LEN] =
{
    0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10
};

static uint8_t ms_burst_rx_reg = 0x8B;
static uint8_t ms_burst_rx_data[TEST_DATA_LEN] =
{
    0x00,
};

static uint8_t sl_rx_data[TEST_DATA_LEN] =
{
    0x00,
};
static uint8_t sl_tx_data[TEST_DATA_LEN] =
{
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
};

static uint8_t sl_tr_rx_data[TEST_DATA_LEN + 1] =
{
    0x00,
};

static uint8_t sl_tr_tx_reg = 0;
static uint8_t sl_tr_tx_data[TEST_DATA_LEN] =
{
    0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10
};

static void hex_dump(uint8_t *data, uint32_t len)
{
    uint32_t i;

    for (i = 0; i < len; i++)
    {
        LOG("%02x ", data[i]);
    }

    LOG("\n");
}

static void app_i2c_wait_cmpl(void)
{
    while (!i2c_event)
    {
    }

    LOG("event 0x%08x\n", i2c_event);
}

static void app_i2c_irq_handler(uint32_t event, void *context)
{
    i2c_event = event;
}

static enum debug_cmd_status cmd_master_send(int argc, char *argv[])
{
    i2c_event = 0;
    HAL_I2C_MasterTransmit(I2C_ID_0, SLAVE_ADDR, ms_tx_data, TEST_DATA_LEN, I2C_IO_TYPE_NON_BLOCK);
    app_i2c_wait_cmpl();

    LOG("Master Trasmit Data\n");
    hex_dump(ms_tx_data, TEST_DATA_LEN);

    return DEBUG_CMD_SUCCESS;
}

static enum debug_cmd_status cmd_master_recv(int argc, char *argv[])
{
    i2c_event = 0;
    HAL_I2C_MasterReceive(I2C_ID_0, SLAVE_ADDR, ms_rx_data, TEST_DATA_LEN, I2C_IO_TYPE_NON_BLOCK);
    app_i2c_wait_cmpl();

    LOG("Master Receive Data\n");
    hex_dump(ms_rx_data, TEST_DATA_LEN);

    return DEBUG_CMD_SUCCESS;
}

static enum debug_cmd_status cmd_master_burst_send(int argc, char *argv[])
{
    i2c_event = 0;
    HAL_I2C_MasterBurstTransmit(I2C_ID_0, SLAVE_ADDR, &ms_burst_tx_reg, 1,
                                ms_burst_tx_data, TEST_DATA_LEN, I2C_IO_TYPE_NON_BLOCK);
    app_i2c_wait_cmpl();

    LOG("Master Burst Transmit Data\n");
    LOG("REG : %02x\n", ms_burst_tx_reg);
    hex_dump(ms_burst_tx_data, TEST_DATA_LEN);

    return DEBUG_CMD_SUCCESS;
}

static enum debug_cmd_status cmd_master_burst_recv(int argc, char *argv[])
{
    i2c_event = 0;
    HAL_I2C_MasterBurstReceive(I2C_ID_0, SLAVE_ADDR, &ms_burst_rx_reg, 1,
                               ms_burst_rx_data, TEST_DATA_LEN, I2C_IO_TYPE_NON_BLOCK);
    app_i2c_wait_cmpl();

    LOG("Master Burst Receive Data\n");
    LOG("REG : %02x\n", ms_burst_rx_reg);
    hex_dump(ms_burst_rx_data, TEST_DATA_LEN);

    return DEBUG_CMD_SUCCESS;
}

static enum debug_cmd_status cmd_slave_send(int argc, char *argv[])
{
    i2c_event = 0;
    HAL_I2C_SlaveTransmit(I2C_ID_0, sl_tx_data, TEST_DATA_LEN, I2C_IO_TYPE_NON_BLOCK);
    app_i2c_wait_cmpl();

    LOG("Slave Trasmit Data\n");
    hex_dump(sl_tx_data, TEST_DATA_LEN);

    return DEBUG_CMD_SUCCESS;
}

static enum debug_cmd_status cmd_slave_recv(int argc, char *argv[])
{
    i2c_event = 0;
    HAL_I2C_SlaveReceive(I2C_ID_0, sl_rx_data, TEST_DATA_LEN, I2C_IO_TYPE_NON_BLOCK);
    app_i2c_wait_cmpl();

    LOG("Slave Receive Data\n");
    hex_dump(sl_rx_data, TEST_DATA_LEN);

    return DEBUG_CMD_SUCCESS;
}

static enum debug_cmd_status cmd_slave_burst_send(int argc, char *argv[])
{
    i2c_event = 0;
    HAL_I2C_SlaveTransfer(I2C_ID_0, sl_tr_tx_data, TEST_DATA_LEN, &sl_tr_tx_reg, 1, I2C_IO_TYPE_NON_BLOCK);
    app_i2c_wait_cmpl();

    LOG("Slave Burst Transmit Data\n");
    LOG("REG : %02x\n", sl_tr_tx_reg);
    hex_dump(sl_tr_tx_data, TEST_DATA_LEN);

    return DEBUG_CMD_SUCCESS;
}

static enum debug_cmd_status cmd_slave_burst_recv(int argc, char *argv[])
{
    i2c_event = 0;
    HAL_I2C_SlaveReceive(I2C_ID_0, sl_tr_rx_data, TEST_DATA_LEN + 1, I2C_IO_TYPE_NON_BLOCK);
    app_i2c_wait_cmpl();

    LOG("Slave Burst Receive Data\n");
    LOG("REG : %02x\n", sl_tr_rx_data[0]);
    hex_dump(&sl_tr_rx_data[1], TEST_DATA_LEN);

    return DEBUG_CMD_SUCCESS;
}

static const struct debug_cmd test_commands[] =
{
    { "ms", cmd_master_send, "send value to slave" },
    { "mr", cmd_master_recv, "recv value from slave"},
    { "mbs", cmd_master_burst_send, "send value to slave" },
    { "mbr", cmd_master_burst_recv, "recv value from slave"},
    { "ss", cmd_slave_send, "send value to master" },
    { "sr", cmd_slave_recv, "recv value from master"},
    { "sbs", cmd_slave_burst_send, "send value to master" },
    { "sbr", cmd_slave_burst_recv, "recv value from master"},
};

static void mainloop(void)
{
    struct i2c_cfg cfg = APP_I2C_CONFIG;

    HAL_I2C_Init(I2C_ID_0, &cfg, app_i2c_irq_handler, NULL);
    HAL_I2C_SetOwnAddress(I2C_ID_0, SLAVE_ADDR);

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

    /* Configure I2C pins */
    Port_Configure_I2C();

    /* Initialize UART Debugging framework to use logging or command input */
    Debug_Init();

    LOG("\n");
    LOG("************************************************\n");
    LOG("\t MCU: %s\n", APP_DEVICE);
    LOG("\t Core: %s\n", APP_CORE);
    LOG("\t Debug: UART%d - %dbps\n", APP_UART_ID, APP_UART_BAUD);
    LOG("\t %s, %s\n", __DATE__, __TIME__);
    LOG("\t I2C_Master_Slave_Interrupt\n");
    LOG("************************************************\n");

    /* Initialze command line processing */
    debug_cmd_init(test_commands, DEBUG_CMD_LIST_COUNT(test_commands));

    /* enable irq */
	__enable_irq();

    /* Start main loop */
    mainloop();
}

