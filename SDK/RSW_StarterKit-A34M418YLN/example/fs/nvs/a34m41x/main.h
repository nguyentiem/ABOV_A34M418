/**
 *******************************************************************************
 * @file        main.h
 * @author      ABOV R&D Division
 * @brief       Main Header File
 *
 * Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#ifndef _MAIN_H_
#define _MAIN_H_

#include "a34m41x.h"
#include "a34m41x_flash.h"

#define APP_DEVICE                      "A34M41X"
#define APP_CORE                        "ARM Cortex-M4F"
#define APP_UART_ID                     0
#define APP_UART_BAUD                   38400

#define APP_NVS_DATA_CFMC               1
#define APP_NVS_DATA_DFMC               2
#define APP_NVS_DATA_TYPE               APP_NVS_DATA_DFMC

#if (APP_NVS_DATA_TYPE == APP_NVS_DATA_CFMC)
#define APP_NVS_ADDR                    0x0000D000
#define APP_NVS_SECTOR_SIZE             512
#define APP_NVS_SECTOR_COUNT            3
#define APP_NVS_ERASE_VALUE             0xFF
#define APP_NVS_WRITE_SIZE              4
#elif (APP_NVS_DATA_TYPE == APP_NVS_DATA_DFMC)
#define APP_NVS_ADDR                    0x0E000000
#define APP_NVS_SECTOR_SIZE             512
#define APP_NVS_SECTOR_COUNT            3
#define APP_NVS_ERASE_VALUE             0xFF
#define APP_NVS_WRITE_SIZE              4
#endif

void Port_Init(void);
void System_Setting_Config(void);
void Debug_Init(void);
void Assert(char *file, uint32_t line);

#endif /* _MAIN_H_ */

