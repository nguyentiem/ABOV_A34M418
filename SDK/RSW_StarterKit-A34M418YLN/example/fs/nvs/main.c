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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

#include "debug_cmd.h"
#include "debug_log.h"

#include "nvs.h"


static struct nvs_fs fs =
{
    .base_addr = APP_NVS_ADDR,
    .sector_size = APP_NVS_SECTOR_SIZE,
    .sector_count = APP_NVS_SECTOR_COUNT,
    .erase_value = APP_NVS_ERASE_VALUE,
    .write_size = APP_NVS_WRITE_SIZE,
    .flash_read = HAL_FLASH_Read,
    .flash_write = HAL_FLASH_Write,
    .flash_erase = HAL_FLASH_Erase,
};

static uint8_t data[1024];

static enum debug_cmd_status cmd_nvs_init(int argc, char *argv[])
{
    int32_t ret;
    uint8_t i;

    ret = nvs_init(&fs);
    if (ret)
    {
        LOG("NVS initialize error, flash erase and retry\n");

        /* clear flash areas to be used as nvs */
        for (i = 0; i < fs.sector_count; i++)
        {
            HAL_FLASH_Erase(fs.base_addr + fs.sector_size * i);
        }

        ret = nvs_init(&fs);
        if (ret)
        {
            LOG("NVS initialize error, can't proceed\n");

            return DEBUG_CMD_SUCCESS;
        }
    }

    LOG("NVS initialized, 0x%08x: %d x %d\n",
        APP_NVS_ADDR, APP_NVS_SECTOR_SIZE, APP_NVS_SECTOR_COUNT);

    return DEBUG_CMD_SUCCESS;
}

static enum debug_cmd_status cmd_nvs_read(int argc, char *argv[])
{
    uint16_t id;
    char str[32];
    int32_t ret;

    if (argc != 2)
    {
        return DEBUG_CMD_INVALID;
    }

    memset(str, 0, sizeof(str));
    id = strtol(argv[1], NULL, 16);

    ret = nvs_read(&fs, id, str, 32);
    if (ret < 0)
    {
        LOG("NVS read error\n");
    }
    else if (ret > 0)
    {
        LOG("Data: ");
        for (int i = 0; i < ret; i++)
        {
            LOG("%02x ", str[i]);
        }
        LOG("\n");
    }

    return DEBUG_CMD_SUCCESS;
}

static enum debug_cmd_status cmd_nvs_write(int argc, char *argv[])
{
    uint16_t id;
    char str[32] __ALIGNED(4);
    int32_t ret;

    if (argc != 3)
    {
        return DEBUG_CMD_INVALID;
    }

    id = strtol(argv[1], NULL, 16);
    strcpy(str, argv[2]);

    ret = nvs_write(&fs, id, str, strlen(str));
    if (ret < 0)
    {
        LOG("NVS write error\n");
    }

    return DEBUG_CMD_SUCCESS;
}

static enum debug_cmd_status cmd_nvs_delete(int argc, char *argv[])
{
    uint16_t id;
    int32_t ret;

    if (argc != 2)
    {
        return DEBUG_CMD_INVALID;
    }

    id = strtol(argv[1], NULL, 16);

    ret = nvs_write(&fs, id, NULL, 0);
    if (ret < 0)
    {
        LOG("NVS delete error\n");
    }

    return DEBUG_CMD_SUCCESS;
}

static enum debug_cmd_status cmd_nvs_clear(int argc, char *argv[])
{
    int32_t ret;

    ret = nvs_clear(&fs);
    if (ret < 0)
    {
        LOG("NVS clear error\n");
    }

    return DEBUG_CMD_SUCCESS;
}

static enum debug_cmd_status cmd_nvs_space(int argc, char *argv[])
{
    uint32_t space;

    space = nvs_free_space(&fs);

    LOG("NVS space = %d\n", space);

    return DEBUG_CMD_SUCCESS;
}

static enum debug_cmd_status cmd_flash_read(int argc, char *argv[])
{
    uint32_t addr;
    uint32_t len;
    uint32_t i;
    int32_t ret;

    if (argc != 3)
    {
        return DEBUG_CMD_INVALID;
    }

    addr = strtol(argv[1], NULL, 16);
    len = atoi(argv[2]);

    ret = HAL_FLASH_Read(addr, data, len);
    if (ret != 0)
    {
        LOG("flash read error\n");
    }

    for (i = 0; i < len; i++)
    {
        if ((i % 16) == 0)
        {
            LOG("\n0x%08x: ", addr + i);
        }
        LOG("%02x ", data[i]);
    }
    LOG("\n");

    return DEBUG_CMD_SUCCESS;
}

static enum debug_cmd_status cmd_flash_write(int argc, char *argv[])
{
    uint32_t addr;
    uint32_t i;
    uint32_t len;
    int32_t ret;

    if (argc != 3)
    {
        return DEBUG_CMD_INVALID;
    }

    addr = strtol(argv[1], NULL, 16);
    len = atoi(argv[2]);

    for (i = 0; i < len; i++)
    {
        data[i] = i;
    }

    ret = HAL_FLASH_Write(addr, data, len);
    if (ret != 0)
    {
        LOG("flash write error\n");
    }

    return DEBUG_CMD_SUCCESS;
}

static enum debug_cmd_status cmd_flash_erase(int argc, char *argv[])
{
    uint32_t addr;
    int32_t ret;

    if (argc != 2)
    {
        return DEBUG_CMD_INVALID;
    }

    addr = strtol(argv[1], NULL, 16);

    ret = HAL_FLASH_Erase(addr);
    if (ret != 0)
    {
        LOG("flash erase error\n");
    }

    return DEBUG_CMD_SUCCESS;
}

static const struct debug_cmd test_commands[] =
{
    { "ni", cmd_nvs_init, "nvs init" },
    { "nr", cmd_nvs_read, "nvs read", "arg1=id" },
    { "nw", cmd_nvs_write, "nvs write", "arg1=id, arg2=str" },
    { "nd", cmd_nvs_delete, "nvs delete", "arg1=id" },
    { "nc", cmd_nvs_clear, "nvs clear" },
    { "ns", cmd_nvs_space, "nvs space" },
    { "fr", cmd_flash_read, "flash read", "arg1=addr,arg2=len" },
    { "fw", cmd_flash_write, "flash write", "arg1=addr,arg2=str" },
    { "fe", cmd_flash_erase, "flash erase", "arg1=addr" },
};

static void mainloop(void)
{
    HAL_FLASH_Init();

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
    LOG("\t NVS\n");
    LOG("************************************************\n");

    /* Initialze command line processing */
    debug_cmd_init(test_commands, DEBUG_CMD_LIST_COUNT(test_commands));

    /* Start main loop */
    mainloop();
}

