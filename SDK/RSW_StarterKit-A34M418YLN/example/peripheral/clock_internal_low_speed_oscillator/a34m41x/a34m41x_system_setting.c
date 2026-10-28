/**
 *******************************************************************************
 * @file        a34m41x_system_setting.c
 * @author      ABOV R&D Division
 * @brief       System Setting Source File
 *
 * Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/

#include "main.h"

#include "a34m41x_scu.h"

#define USED_LSI		
//#define USED_HSI		
//#define USED_HSE		
//#define USED_LSE		
//#define USED_HSEPLL
//#define USED_HSIPLL

void System_Setting_Config(void)
{

	HAL_SCU_EnableClockOut(SCU_CLOCK_OUT_SEL_PLL, 5);

#ifdef USED_LSI	
    struct scu_main_clock_cfg cfg =
    {
        .mclk_sel = SCU_CLOCK_LSI,
        .mclk_div = SCU_MAIN_CLOCK_DIV_NONE,
        .hclk_div = SCU_AHB_CLOCK_DIV_NONE,
        .mclk_hz = 500000,
    };
#endif

#ifdef USED_LSE
    struct scu_main_clock_cfg cfg =
    {
        .mclk_sel = SCU_CLOCK_LSE,
        .mclk_div = SCU_MAIN_CLOCK_DIV_NONE,
        .hclk_div = SCU_AHB_CLOCK_DIV_NONE,
        .mclk_hz = 32768,
    };
#endif
		
#ifdef USED_HSI	
    struct scu_main_clock_cfg cfg =
    {
        .mclk_sel = SCU_CLOCK_HSI,
        .mclk_div = SCU_MAIN_CLOCK_DIV_NONE,
        .hclk_div = SCU_AHB_CLOCK_DIV_NONE,
        .mclk_hz = 32000000,
    };
#endif
	
#ifdef USED_HSE	
    struct scu_main_clock_cfg cfg =
    {
        .mclk_sel = SCU_CLOCK_HSE,
        .mclk_div = SCU_MAIN_CLOCK_DIV_NONE,
        .hclk_div = SCU_AHB_CLOCK_DIV_NONE,
        .mclk_hz = 8000000,
    };		
#endif



#ifdef USED_HSEPLL

    struct scu_pll_clock_cfg p_cfg =
    {
        .out_div = 0,
        .post_div1 = 59,
        .post_div2 = 0,
        .pre_div = 3,
        .p_mode = SCU_PLL_MODE_VCO_EQUAL,
        .pclk_src = SCU_PLL_CLOCK_SRC_HSE,
        .p_input_clk_div = SCU_PLL_CLOCK_INPUT_DIV_NONE,
    };
    struct scu_main_clock_cfg cfg =
    {
        .mclk_sel = SCU_CLOCK_PLL,
        .mclk_div = SCU_MAIN_CLOCK_DIV_NONE,
        .hclk_div = SCU_AHB_CLOCK_DIV_NONE,
        .mclk_hz = 120000000,
    };
		
		HAL_SCU_SetPLLConfig(&p_cfg);

#endif

#ifdef USED_HSIPLL
    struct scu_pll_clock_cfg p_cfg =
    {
        .out_div = 0,
        .post_div1 = 59,
        .post_div2 = 0,
        .pre_div = 3,
        .p_mode = SCU_PLL_MODE_VCO_EQUAL,
        .pclk_src = SCU_PLL_CLOCK_SRC_HSI,
        .p_input_clk_div = SCU_PLL_CLOCK_INPUT_DIV_NONE,
    };
    struct scu_main_clock_cfg cfg =
    {
        .mclk_sel = SCU_CLOCK_PLL,
        .mclk_div = SCU_MAIN_CLOCK_DIV_NONE,
        .hclk_div = SCU_AHB_CLOCK_DIV_NONE,
        .mclk_hz = 120000000,
    };
		
		HAL_SCU_SetPLLConfig(&p_cfg);				
#endif

	HAL_SCU_UpdateMainClock(&cfg);
		
}

