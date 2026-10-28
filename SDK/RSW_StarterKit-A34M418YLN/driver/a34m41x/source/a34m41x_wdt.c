/**
 *******************************************************************************
 * @file        a34m41x_wdt.c
 * @author      ABOV R&D Division
 * @brief       Watchdog Timer
 *
 * Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "a34m41x.h"
#include "a34m41x_wdt.h"

#define WDT_LOAD_MAX        0xFFFFFFFF

struct wdt_ctrl_block
{
    wdt_irq_handler_t   handler;
    void *              context;
    uint32_t            counter;
};

static struct wdt_ctrl_block g_wcb;

 __INLINE void WDT_AccessEnable(void)
{
    WDT->AEN = 0xA55A;
}

 __INLINE void WDT_AccessDisable(void)
{
    WDT->AEN = 0x0000;
}

static int32_t WDT_Configure(struct wdt_cfg *cfg)
{
    uint32_t reg_cfg = 0;

    if (cfg->clk_src == WDT_CLK_SRC_EXTERNAL)
    {
        reg_cfg |= (1 << WDT_CON_CKSEL_Pos);
    }

    if (cfg->clk_div > WDT_CLK_DIV_256)
    {
        return DRIVER_ERROR_PARAMETER;
    }

    reg_cfg |= (cfg->clk_div << WDT_CON_WPRS_Pos);

    if (cfg->mode == WDT_MODE_RESET)
    {
        reg_cfg |= (1 << WDT_CON_WDTRE_Pos);
    }
    else if (cfg->mode == WDT_MODE_IRQ)
    {
        reg_cfg |= (1 << WDT_CON_WDTIE_Pos);
    }
    else
    {
        return DRIVER_ERROR_PARAMETER;
    }

    WDT->CON = reg_cfg;

    NVIC_ClearPendingIRQ(WDT_IRQn);
    NVIC_SetPriority(WDT_IRQn, cfg->irq_prio);
    NVIC_EnableIRQ(WDT_IRQn);

    return DRIVER_ERROR_OK;
}

int32_t HAL_WDT_Init(struct wdt_cfg *cfg, wdt_irq_handler_t handler, void *context)
{
    DRIVER_ASSERT(cfg != NULL);

    if (cfg->mode == WDT_MODE_IRQ)
    {
        DRIVER_ASSERT(handler);
    }

    g_wcb.handler = handler;
    g_wcb.context = context;

    WDT->CON = 0;

    WDT_Configure(cfg);

    return DRIVER_ERROR_OK;
}

int32_t HAL_WDT_Deinit(void)
{
    memset(&g_wcb, 0, sizeof(struct wdt_ctrl_block));

    WDT->CON = 0;

    NVIC_DisableIRQ(WDT_IRQn);

    return DRIVER_ERROR_OK;
}

int32_t HAL_WDT_Start(uint32_t counter)
{
	  WDT->CON |= (1 << WDT_CON_WDTEN_Pos);	
	
		g_wcb.counter = counter;
		WDT->LR = counter;
		
    return DRIVER_ERROR_OK;
}

int32_t HAL_WDT_Stop(void)
{
    /*
     * counter value can only be set when WDT is enabled
     * set it to max for the next start request
     */
    WDT->LR = WDT_LOAD_MAX;
    WDT->CON &= ~(1 << WDT_CON_WDTEN_Pos);

    return DRIVER_ERROR_OK;
}

int32_t HAL_WDT_Feed(void)
{
    WDT->LR = g_wcb.counter;

    return DRIVER_ERROR_OK;
}

int32_t HAL_WDT_GetCounter(uint32_t *counter)
{
    *counter = WDT->CNT;

    return DRIVER_ERROR_OK;
}

void WDT_IRQHandlerC(unsigned long *args)
{
    if (g_wcb.handler)
    {
        g_wcb.handler(args, g_wcb.context);
    }
}

