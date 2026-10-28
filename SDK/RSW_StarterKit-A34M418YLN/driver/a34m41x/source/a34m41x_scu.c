/**
 *******************************************************************************
 * @file        a34m41x_scu.c
 * @author      ABOV R&D Division
 * @brief       System Control Unit
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
#include <string.h>

#include "a34m41x_scu.h"

struct scu_ctrl_block
{
    scu_clk_fail_handler_t          mclk_handler;
    void *                          mclk_context;
    scu_clk_fail_handler_t          lse_handler;
    void *                          lse_context;
    scu_clk_fail_handler_t          hse_handler;
    void *                          hse_context;
    uint8_t                         pll_configured;
    enum scu_clock                  mclk;
    enum scu_pll_clock_src          pclk_src;
    enum scu_pll_clock_input_div    p_input_clk_div;
};

static struct scu_ctrl_block g_scb =
{
    .mclk_handler = NULL,
    .mclk_context = NULL,
    .lse_handler = NULL,
    .lse_context = NULL,
    .hse_handler = NULL,
    .hse_context = NULL,
    .pll_configured = 0,
};

static void SCU_FlashLatencySet(uint32_t hclk)
{
		uint32_t reg_val;
	
    if (hclk <= 28000000)
    {
        reg_val = CFMC->CONF;
				reg_val &= ~(CFMC_CONF_LATENCY_Msk);
				CFMC->CONF = reg_val;
				
				reg_val = DFMC->CONF;
				reg_val &= ~(DFMC_CONF_LATENCY_Msk);
				DFMC->CONF = reg_val;
    }
    else if (hclk <= 56000000)
    {
				reg_val = CFMC->CONF;
				reg_val &= ~(CFMC_CONF_LATENCY_Msk);
				reg_val |= 1 << CFMC_CONF_LATENCY_Pos;
				CFMC->CONF = reg_val;
				
				reg_val = DFMC->CONF;
				reg_val &= ~(DFMC_CONF_LATENCY_Msk);
				reg_val |= 1 << DFMC_CONF_LATENCY_Pos;
				DFMC->CONF = reg_val;
    }
    else if (hclk <= 84000000)
    {
				reg_val = CFMC->CONF;
				reg_val &= ~(CFMC_CONF_LATENCY_Msk);
				reg_val |= 2 << CFMC_CONF_LATENCY_Pos;
				CFMC->CONF = reg_val;
				
				reg_val = DFMC->CONF;
				reg_val &= ~(DFMC_CONF_LATENCY_Msk);
				reg_val |= 2 << DFMC_CONF_LATENCY_Pos;
				DFMC->CONF = reg_val;
    }
    else if (hclk <= 112000000)
    {
      	reg_val = CFMC->CONF;
				reg_val &= ~(CFMC_CONF_LATENCY_Msk);
				reg_val |= 3 << CFMC_CONF_LATENCY_Pos;
				CFMC->CONF = reg_val;
				
				reg_val = DFMC->CONF;
				reg_val &= ~(DFMC_CONF_LATENCY_Msk);
				reg_val |= 3 << DFMC_CONF_LATENCY_Pos;
				DFMC->CONF = reg_val;
    }
    else
    {
      	reg_val = CFMC->CONF;
				reg_val &= ~(CFMC_CONF_LATENCY_Msk);
				reg_val |= 4 << CFMC_CONF_LATENCY_Pos;
				CFMC->CONF = reg_val;
				
				reg_val = DFMC->CONF;
				reg_val &= ~(DFMC_CONF_LATENCY_Msk);
				reg_val |= 4 << DFMC_CONF_LATENCY_Pos;
				DFMC->CONF = reg_val;
    }
}

/**
CLOCK_DELAY value must be changed according to the system clock used by the user.
1. LSI500kHz    : 0x80 
2. HSE8MHz      : 0x800
3. HSE16MHz     : 0x1000
4. HSI32MHz     : 0x2000
*/

/* LSI 500kHz - 1ms */
#define CLOCK_DELAY	0x80	

static void SCU_ClockEnable(enum scu_clock mclk_sel, enum scu_pll_clock_src pclk_src)
{
		uint32_t reg_val;
		uint32_t i;
		
		switch (mclk_sel)
    {
        case SCU_CLOCK_LSI:
						reg_val = SCU->CSCR;
						reg_val |= 1 << SCU_CSCR_LSICON_Pos;
						SCU->CSCR = reg_val;
				    /* about 1ms delay, The delay value may vary depending on the user's circuit environment. */
						for(i=0;i<CLOCK_DELAY;i++);						
						reg_val = SCU->SCCR;
						reg_val &= ~SCU_SCCR_MCLKSEL_Msk;
						SCU->SCCR = reg_val;
            break;
        case SCU_CLOCK_LSE:
						reg_val = SCU->CSCR;
						reg_val |= 1 << SCU_CSCR_LSECON_Pos;
						SCU->CSCR = reg_val;
				    /* Need more Wait for LSE stable, check LSE pin oscillation.
				        about 1s delay, The delay value may vary depending on the user's circuit environment. */
						for(i=0;i<CLOCK_DELAY*1000;i++);			
						reg_val = SCU->SCCR;
						reg_val &= ~SCU_SCCR_MCLKSEL_Msk;
						reg_val |= 1 << SCU_SCCR_MCLKSEL_Pos;
						SCU->SCCR = reg_val;
            break;
        case SCU_CLOCK_HSI:
						reg_val = SCU->CSCR;
						reg_val |= 1 << SCU_CSCR_HSICON_Pos;
						SCU->CSCR = reg_val;	
				    /* about 1ms delay, The delay value may vary depending on the user's circuit environment. */
						for(i=0;i<CLOCK_DELAY;i++);			
						reg_val = SCU->SCCR;
						reg_val &= ~SCU_SCCR_MCLKSEL_Msk;
						reg_val |= 2 << SCU_SCCR_MCLKSEL_Pos;
						SCU->SCCR = reg_val;
            break;
        case SCU_CLOCK_HSE:
						reg_val = SCU->CSCR;
						reg_val |= 1 << SCU_CSCR_HSECON_Pos;
						SCU->CSCR = reg_val;
				    /* Need more Wait for HSE stable, check HSE pin oscillation.
				        about 10ms delay, The delay value may vary depending on the user's circuit environment. */
						for(i=0;i<CLOCK_DELAY*10;i++);					
						reg_val = SCU->SCCR;
						reg_val &= ~SCU_SCCR_MCLKSEL_Msk;
						reg_val |= 6 << SCU_SCCR_MCLKSEL_Pos;
						SCU->SCCR = reg_val;
            break;
        default:
            DRIVER_ASSERT(0);
            break;
    }
}

int32_t HAL_SCU_UpdateMainClock(struct scu_main_clock_cfg *cfg)
{
    uint32_t sccr;
    uint32_t hclk;

    if (cfg->mclk_sel == SCU_CLOCK_PLL && g_scb.pll_configured != 1)
    {
        DRIVER_ASSERT(0);
    }
		
		if (cfg->mclk_sel != SCU_CLOCK_PLL)
		{
			sccr = SCU->SCCR;

			sccr &= SCU_SCCR_MCLKSEL_Msk;
			
			sccr |= cfg->mclk_div << SCU_SCCR_HCLKDIV_Pos |
							cfg->hclk_div << SCU_SCCR_PCLKDIV_Pos |
							g_scb.pclk_src << SCU_SCCR_PLLCLKSEL_Pos |
							g_scb.p_input_clk_div << SCU_SCCR_PLLPREDIV_Pos;

			SCU->SCCR = sccr;

			SCU_ClockEnable(cfg->mclk_sel, g_scb.pclk_src);
		}
		
    g_scb.mclk = cfg->mclk_sel;

    SystemCoreClock = cfg->mclk_hz;
    hclk = cfg->mclk_hz / (1 << cfg->mclk_div);
    SystemPeriClock = hclk / (1 << cfg->hclk_div);

    SystemDelayCount = (uint32_t)(((uint64_t)SystemCoreClock * 100) / 1000000);

    SCU_FlashLatencySet(hclk);

    return DRIVER_ERROR_OK;
}

/* PLL start timeout */
#define PLL_STARTUP_TIMEOUT  1000 		// flash wait 7 -> over 500us

int32_t HAL_SCU_SetPLLConfig(struct scu_pll_clock_cfg *cfg)
{
	uint32_t startup_counter = 0; 
	uint32_t reg_val;
//	uint32_t pll_check_result = 0;
	
	uint32_t i;	

	/* Flash Wait 7 */
	reg_val = CFMC->CONF;
	reg_val &= ~CFMC_CONF_LATENCY_Msk;
	reg_val |= 7 << CFMC_CONF_LATENCY_Pos;
	CFMC->CONF = reg_val;
	
		if(cfg->pclk_src == SCU_PLL_CLOCK_SRC_HSE)
		{
			reg_val = SCU->CSCR;	
			reg_val &= ~ SCU_CSCR_HSECON_Msk;
			reg_val |= 1 << SCU_CSCR_HSECON_Pos;	/* HSE8Hz on */
			SCU->CSCR = reg_val;	
			/* Need more Wait for HSE stable, check HSE pin oscillation.
			    about 10ms delay, The delay value may vary depending on the user's circuit environment. */
			for(i=0;i<CLOCK_DELAY*10;i++);	
			reg_val = SCU->SCCR;	
			reg_val &= ~(SCU_SCCR_MCLKSEL_Msk << SCU_SCCR_MCLKSEL_Pos);	
			reg_val |= 6 << SCU_SCCR_MCLKSEL_Pos;	/* main clk - HSE8MHz */
			
			reg_val &= ~(SCU_SCCR_PLLCLKSEL_Msk << SCU_SCCR_PLLCLKSEL_Pos);
			reg_val |= SCU_PLL_CLOCK_SRC_HSE << SCU_SCCR_PLLCLKSEL_Pos;	/* PLL Input clock = HSE8MHz */
			
			SCU->SCCR = reg_val;
			
			HAL_SCU_SetPLLPrediv(SCU_PLL_CLOCK_INPUT_DIV_NONE);	/* PLLINCLK = 8MHz */
		}
		else
		{
			reg_val = SCU->CSCR;	
			reg_val &= ~ SCU_CSCR_HSICON_Msk;
			reg_val |= 1 << SCU_CSCR_HSICON_Pos;	/* HSI32MHz on */
			SCU->CSCR = reg_val;		
			/* about 1ms delay, The delay value may vary depending on the user's circuit environment. */
			for(i=0;i<CLOCK_DELAY;i++);				
			reg_val = SCU->SCCR;
			reg_val &= ~(SCU_SCCR_MCLKSEL_Msk << SCU_SCCR_MCLKSEL_Pos);	
			reg_val |= 2 << SCU_SCCR_MCLKSEL_Pos;	/* main clk - HSI32MHz */
			
			reg_val &= ~(SCU_PLL_CLOCK_SRC_HSE << SCU_SCCR_PLLCLKSEL_Pos);
			reg_val |=  SCU_PLL_CLOCK_SRC_HSI << SCU_SCCR_PLLCLKSEL_Pos;	/* PLL Input clock = HSI32MHz	*/
			
			SCU->SCCR = reg_val;	
			
			HAL_SCU_SetPLLPrediv(SCU_PLL_CLOCK_INPUT_DIV_4);	/* PLLINCLK = 32/4 =8MHz */
		}
			
		SCU->PLLCON = (1<<SCU_PLLCON_PLLEN_Pos);	/* PLL enable */
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();    // Delay for stabilization				
		
    reg_val = 0 |
			((cfg->out_div << SCU_PLLCON_OUTDIV_Pos) & SCU_PLLCON_OUTDIV_Msk) |
			((cfg->post_div1 << SCU_PLLCON_POSTDIV1_Pos) & SCU_PLLCON_POSTDIV1_Msk) |
			((cfg->post_div2 << SCU_PLLCON_POSTDIV2_Pos) & SCU_PLLCON_POSTDIV2_Msk) |
			((cfg->pre_div << SCU_PLLCON_PREDIV_Pos) & SCU_PLLCON_PREDIV_Msk) |
			((cfg->p_mode << SCU_PLLCON_PLLMODE_Pos) & SCU_PLLCON_PLLMODE_Msk) |
			(1 << SCU_PLLCON_BYPASSB_Pos) |		
			(1<<SCU_PLLCON_PLLEN_Pos) |
			(1 << SCU_PLLCON_PLLRSTB_Pos) |
			(6<<24);	// CTRLOPT  01:Current option = 10uA / 10:VCO bias = x1
		
		SCU->PLLCON |= reg_val;	/* PLL Configure setting */
			
		if(cfg->pclk_src == SCU_PLL_CLOCK_SRC_HSE)
			startup_counter = PLL_STARTUP_TIMEOUT;				// HSE-8Mhz
		else
			startup_counter = PLL_STARTUP_TIMEOUT * 4;		// HSI-32Mhz * 4
		while(--startup_counter);		
		/* Check the PLLCON State */
		while((SCU->PLLCON & (1UL<<31)) == 0);						

		
    g_scb.pll_configured = 1;
    g_scb.pclk_src = cfg->pclk_src;
    g_scb.p_input_clk_div = cfg->p_input_clk_div;
		
		/* hclk = mclk/2, pclk = hclk */
		reg_val = SCU->SCCR;
		reg_val &= ~SCU_SCCR_HCLKDIV_Msk;
		reg_val |= ((SCU_MAIN_CLOCK_DIV_2 << SCU_SCCR_HCLKDIV_Pos));	
		reg_val &= ~SCU_SCCR_PCLKDIV_Msk;
		reg_val |= ((SCU_AHB_CLOCK_DIV_NONE << SCU_SCCR_PCLKDIV_Pos));	
		SCU->SCCR = reg_val;
		
		/* main clk - PLL */
		reg_val = SCU->SCCR;
		reg_val &= ~(SCU_SCCR_MCLKSEL_Msk << SCU_SCCR_MCLKSEL_Pos);
		reg_val |=  7 << SCU_SCCR_MCLKSEL_Pos;	
		SCU->SCCR = reg_val;
		
		/* hclk = mclk, pclk = hclk */
		reg_val = SCU->SCCR;
		reg_val &= ~SCU_SCCR_HCLKDIV_Msk;
		reg_val &= ~SCU_SCCR_PCLKDIV_Msk;
		SCU->SCCR = reg_val;
	
	   /* Flash Wait 4 */
		reg_val = CFMC->CONF;
		reg_val &= ~CFMC_CONF_LATENCY_Msk;
		reg_val |= 4 << CFMC_CONF_LATENCY_Pos;
		CFMC->CONF = reg_val;
		
		return DRIVER_ERROR_OK;
}


int32_t HAL_SCU_EnableClock(enum scu_clock clk)
{
		uint32_t reg_val;
    if (clk == g_scb.mclk)
    {
        return DRIVER_ERROR_OK;
    }
		
		reg_val = SCU->CSCR;
    switch (clk)
    {
        case SCU_CLOCK_LSI:
            reg_val |= 1 << SCU_CSCR_LSICON_Pos;
            break;
        case SCU_CLOCK_LSE:
             reg_val |= 1 << SCU_CSCR_LSECON_Pos;
            break;
        case SCU_CLOCK_HSI:
            reg_val |= 1 << SCU_CSCR_HSICON_Pos;
            break;
        case SCU_CLOCK_HSE:
            reg_val |= 1 << SCU_CSCR_HSECON_Pos;
            break;
        default:
            DRIVER_ASSERT(0);
            break;	
    }
		SCU->CSCR = reg_val;
		
    return DRIVER_ERROR_OK;
}

int32_t HAL_SCU_SetPLLPrediv(enum scu_pll_clock_input_div plldiv)
{
		uint32_t reg_val;
	
		reg_val = SCU->SCCR;
		reg_val &= ~(SCU_SCCR_PLLPREDIV_Msk);
		
    switch (plldiv)
    {
        case SCU_PLL_CLOCK_INPUT_DIV_NONE:
						reg_val |= 0 << SCU_SCCR_PLLPREDIV_Pos;
            break;
        case SCU_PLL_CLOCK_INPUT_DIV_2:
						reg_val |= 1 << SCU_SCCR_PLLPREDIV_Pos;
            break;
        case SCU_PLL_CLOCK_INPUT_DIV_4:
						reg_val |= 2 << SCU_SCCR_PLLPREDIV_Pos;
            break;
        case SCU_PLL_CLOCK_INPUT_DIV_8:
						reg_val |= 3 << SCU_SCCR_PLLPREDIV_Pos;
            break;
        default:
            DRIVER_ASSERT(0);
            break;
    }
		
		SCU->SCCR = reg_val;
    return DRIVER_ERROR_OK;
}

int32_t HAL_SCU_DisableClock(enum scu_clock clk)
{
		uint32_t reg_val;
		if (clk == g_scb.mclk)
    {
        return DRIVER_ERROR_PARAMETER;
    }
		
		reg_val = SCU->CSCR;
    switch (clk)
    {
        case SCU_CLOCK_LSI:
						reg_val &= ~(1 << SCU_CSCR_LSICON_Pos);
            break;
        case SCU_CLOCK_LSE:
						reg_val &= ~(1 << SCU_CSCR_LSECON_Pos);
            break;
        case SCU_CLOCK_HSI:
						reg_val &= ~(1 << SCU_CSCR_HSICON_Pos);
            break;
        case SCU_CLOCK_HSE:
						reg_val &= ~(1 << SCU_CSCR_HSECON_Pos);
            break;
        default:
            DRIVER_ASSERT(0);
            break;
    }
		SCU->CSCR = reg_val;

    return DRIVER_ERROR_OK;
}

int32_t HAL_SCU_EnableClockMonitor(enum scu_clock_mon clk,
                                   scu_clk_fail_handler_t handler,
                                   void *context,
                                   uint8_t irq_prio)
{
    switch (clk)
    {
        case SCU_MON_CLOCK_MCLK:
            SCU->CMR |= (1 << SCU_CMR_MCLKIE_Pos | 1 << SCU_CMR_MCLKMNT_Pos);
            g_scb.mclk_handler = handler;
            g_scb.mclk_context = context;
            NVIC_ClearPendingIRQ(SYSCLKFAIL_IRQn);
            NVIC_SetPriority(SYSCLKFAIL_IRQn, irq_prio);
            NVIC_EnableIRQ(SYSCLKFAIL_IRQn);
            break;
        case SCU_MON_CLOCK_LSE:
            SCU->CMR |= (1 << SCU_CMR_LSEIE_Pos | 1 << SCU_CMR_LSEMNT_Pos);
            g_scb.lse_handler = handler;
            g_scb.lse_context = context;
            NVIC_ClearPendingIRQ(LSEFAIL_IRQn);
            NVIC_SetPriority(LSEFAIL_IRQn, irq_prio);
            NVIC_EnableIRQ(LSEFAIL_IRQn);
            break;
        case SCU_MON_CLOCK_HSE:
            g_scb.hse_handler = handler;
            g_scb.hse_context = context;
            SCU->CMR |= (1 << SCU_CMR_HSEIE_Pos | 1 << SCU_CMR_HSEMNT_Pos);
            NVIC_ClearPendingIRQ(HSEFAIL_IRQn);
            NVIC_SetPriority(HSEFAIL_IRQn, irq_prio);
            NVIC_EnableIRQ(HSEFAIL_IRQn);
            break;
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_SCU_DisableClockMonitor(enum scu_clock_mon clk)
{
    switch (clk)
    {
        case SCU_MON_CLOCK_MCLK:
            SCU->CMR &= ~(1 << SCU_CMR_MCLKIE_Pos | 1 << SCU_CMR_MCLKMNT_Pos);
            NVIC_DisableIRQ(SYSCLKFAIL_IRQn);
            break;
        case SCU_MON_CLOCK_LSE:
            SCU->CMR &= ~(1 << SCU_CMR_LSEIE_Pos | 1 << SCU_CMR_LSEMNT_Pos);
            NVIC_DisableIRQ(LSEFAIL_IRQn);
            break;
        case SCU_MON_CLOCK_HSE:
            SCU->CMR &= ~(1 << SCU_CMR_HSEIE_Pos | 1 << SCU_CMR_HSEMNT_Pos);
            NVIC_DisableIRQ(HSEFAIL_IRQn);
            break;
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_SCU_EnableClockOut(enum scu_clock_out_sel clk_sel, uint8_t clk_div)
{
		uint32_t reg_val;
	
		reg_val = SCU->COR;
		reg_val &= ~(SCU_COR_CLKODIV_Msk | SCU_COR_CLKOINSEL_Msk);

    switch (clk_sel)
    {
        case SCU_CLOCK_OUT_SEL_LSE:
						reg_val |= 1 << SCU_COR_CLKOINSEL_Pos; 
            break;
        case SCU_CLOCK_OUT_SEL_MCLK:
						reg_val |= 4 << SCU_COR_CLKOINSEL_Pos;  
            break;
        case SCU_CLOCK_OUT_SEL_HSI:
						reg_val |= 5 << SCU_COR_CLKOINSEL_Pos;   
            break;
        case SCU_CLOCK_OUT_SEL_HSE:
						reg_val |= 6 << SCU_COR_CLKOINSEL_Pos;  
            break;
        case SCU_CLOCK_OUT_SEL_PLL:
						reg_val |= 7 << SCU_COR_CLKOINSEL_Pos;  
            break;
        default:
            break;
    }

		reg_val |= (clk_div << SCU_COR_CLKODIV_Pos) & SCU_COR_CLKODIV_Msk;
		reg_val |= (1 << SCU_COR_CLKOEN_Pos);
		SCU->COR = reg_val;

    return DRIVER_ERROR_OK;
}

int32_t HAL_SCU_DisableClockOut(void)
{
    SCU->COR &= ~(1 << SCU_COR_CLKOEN_Pos);

    return DRIVER_ERROR_OK;
}

int32_t HAL_SCU_EnableWakeupSource(uint32_t src)
{
    SCU->WUER |= src;

    return DRIVER_ERROR_OK;
}

int32_t HAL_SCU_DisableWakeupSource(uint32_t src)
{
    SCU->WUER &= ~src;

    return DRIVER_ERROR_OK;
}

int32_t HAL_SCU_EnableResetSource(uint32_t src)
{
    SCU->RSER |= src;

    return DRIVER_ERROR_OK;
}

int32_t HAL_SCU_DisableResetSource(uint32_t src)
{
    SCU->RSER &= src;

    return DRIVER_ERROR_OK;
}

int32_t HAL_SCU_GetResetReason(uint32_t *reason)
{
    uint32_t value;

    value = SCU->RSSR;
    SCU->RSSR = value;

    *reason = value;

    return DRIVER_ERROR_OK;
}

int32_t HAL_SCU_GetWakeupReason(uint32_t *reason)
{
    uint32_t value;

    value = SCU->RSSR;
    SCU->RSSR = value;

    *reason = value;

    return DRIVER_ERROR_OK;
}

void SYSCLKFAIL_IRQHandler(void)
{
    SCU->CMR |= 1 << SCU_CMR_MCLKFAIL_Pos;
    if (g_scb.mclk_handler)
    {
        g_scb.mclk_handler(SCU_CLOCK_FAIL_EVENT_MCLK, g_scb.mclk_context);
    }
}

void HSEFAIL_IRQHandler(void)
{
    SCU->CMR |= 1 << SCU_CMR_HSEFAIL_Pos;
    if (g_scb.hse_handler)
    {
        g_scb.hse_handler(SCU_CLOCK_FAIL_EVENT_HSE, g_scb.hse_context);
    }
}

void LSEFAIL_IRQHandler(void)
{
    SCU->CMR |= 1 << SCU_CMR_LSEFAIL_Pos;
    if (g_scb.lse_handler)
    {
        g_scb.lse_handler(SCU_CLOCK_FAIL_EVENT_HSE, g_scb.lse_context);
    }
}
