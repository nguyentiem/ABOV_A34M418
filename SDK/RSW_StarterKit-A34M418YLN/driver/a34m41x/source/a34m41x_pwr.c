/**
 *******************************************************************************
 * @file        a34m41x_pwr.c
 * @author      ABOV R&D Division
 * @brief       Power sub system
 *
 * Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/

#include <string.h>
#include "a34m41x.h"
#include "a34m41x_scu.h"
#include "a34m41x_pwr.h"


#define LVR_DISABLE_VALUE       0xAA
#define LVI_WTIDKY_VALUE        0x7A

struct pwr_ctrl_block
{
    pwr_irq_handler_t   handler;
    void *              context;
};

static struct pwr_ctrl_block g_pcb;

int32_t HAL_LVI_Init(struct lvi_cfg *cfg,
                     pwr_irq_handler_t handler,
                     void *context)
{
    uint32_t reg_val;
	
    DRIVER_ASSERT(cfg != NULL);

    reg_val = 0;
    reg_val |= (cfg->level << SCU_LVICR_LVIVS_Pos);
    reg_val |= (cfg->auto_off << SCU_LVICR_LVIAON_Pos);
    reg_val |= (1 << SCU_LVICR_LVIINTEN_Pos);
    reg_val |= (1 << SCU_LVICR_LVIEN_Pos);

    SCU->LVICR = reg_val;
	
    g_pcb.handler = handler;
    g_pcb.context = context;

    NVIC_SetPriority(LVI_IRQn, cfg->irq_prio);
    NVIC_EnableIRQ(LVI_IRQn);

    return DRIVER_ERROR_OK;
}

int32_t HAL_LVI_Deinit(void)
{
    SCU->LVICR &= ~(1 << SCU_LVICR_LVIEN_Pos);

    NVIC_DisableIRQ(LVI_IRQn);

    return DRIVER_ERROR_OK;
}

int32_t HAL_LVR_Init(struct lvr_cfg *cfg)
{
    uint32_t reg_val; 
	
    DRIVER_ASSERT(cfg != NULL);

    reg_val = 0;
    reg_val |= cfg->level << SCU_LVRCR_LVRVS_Pos;
    reg_val |= cfg->auto_off << SCU_LVRCR_LVRAON_Pos;
    SCU->LVRCR = reg_val;

    return DRIVER_ERROR_OK;
}

int32_t HAL_LVR_Deinit(void)
{
    SCU->LVRCR = (LVR_DISABLE_VALUE << SCU_LVRCR_LVREN_Pos);

    return DRIVER_ERROR_OK;
}

void HAL_PWR_EnterSleepMode(void)
{
    SCB->SCR = 0;
    __DSB();
    __WFI();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
}

void HAL_PWR_EnterDeepSleepMode(void)
{
    SCB->SCR = (1 << SCB_SCR_SLEEPDEEP_Pos);
    __DSB();		 	
    __WFI();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
}

void LVI_IRQHandler(void)
{  
    SCU->LVISR |= (LVI_WTIDKY_VALUE << SCU_LVISR_WTIDKY_Pos) | (1 << SCU_LVISR_LVIIFLAG_Pos);    

    g_pcb.handler(PWR_EVENT_LVI, g_pcb.context);
}

