/**
 *******************************************************************************
 * @file        a34m41x_system.c
 * @author      ABOV R&D Division
 * @brief       CMSIS Cortex-M4F Device Peripheral Access Layer for A34M41x
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
#include "a34m41x.h"

uint32_t SystemCoreClock;
uint32_t SystemPeriClock;
uint32_t SystemDelayCount;

void SystemInit(void)
{
    __disable_irq();
	
	
    // WDT Access Enable
    WDT->AEN = 0xA55A;	
    // WDT Disable 
    WDT->CON = 0;
	

    // SCU Access Enable
    SCU->SYSTEN = 0x57;
    SCU->SYSTEN = 0x75;
		
    // GPIO Access Enable
    PORTEN->EN = 0x15;
    PORTEN->EN = 0x51;

    SystemCoreClock = 500000;               // 500KHz
    SystemPeriClock = 500000;               // 500KHz

    // =========================================================================
    // Flash Memory Control
    // =========================================================================

    // Data/Instruction Cache Disable
    CFMC->CONF &= ~(CFMC_CONF_DCEN_Msk | CFMC_CONF_ICEN_Msk);

    // Data/Instruction Cache Reset
    CFMC->CONF |= (1 << CFMC_CONF_DCRST_Pos | 1 << CFMC_CONF_ICRST_Pos);

    // Data/Instruction Cache Reset  clear
    CFMC->CONF &= ~(1 << CFMC_CONF_DCRST_Pos | 1 << CFMC_CONF_ICRST_Pos);	
	
    // Data/Instruction Cache Enable
    CFMC->CONF |= (1 << CFMC_CONF_DCEN_Pos | 1 << CFMC_CONF_ICEN_Pos);

    // Flash Wait 7
    CFMC->CONF |= 7 << CFMC_CONF_LATENCY_Pos;	// kyi - set flash wait after MCLK setting ?

	
    __enable_irq();
}

void SystemDelay(uint32_t ms)
{
    int i;	// kyi - type volatile

    for (i = 0; i < (SystemDelayCount * ms); i++)
    {
        __NOP();
    }
}
