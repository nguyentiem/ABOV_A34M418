/**
 *******************************************************************************
 * @file        a34m41x_scu.h
 * @author      ABOV R&D Division
 * @brief       SCU Header File
 *
 * Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/

/**
 * @addtogroup  driver
 * @{
 * @defgroup    driver_scu SCU
 * @{
 * @brief       SCU driver
 */

#ifndef _A34M41X_SCU_H_
#define _A34M41X_SCU_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "a34m41x.h"
#include "a34m41x_driver_common.h"


	
/**
 * SCU clock selection
 */
enum scu_clock
{
    SCU_CLOCK_LSI   = 0,            /**< Internal low speed clcok(500Khz) */
    SCU_CLOCK_LSE   = 1,            /**< External low speed clcok */
    SCU_CLOCK_HSI   = 2,            /**< Internal high speed clock(32Mhz) */
    SCU_CLOCK_HSE   = 3,            /**< Extern high speed clock */
    SCU_CLOCK_PLL   = 4,            /**< PLL clock */
};

/**
 * SCU main clock divider
 */
enum scu_main_clock_div
{
    SCU_MAIN_CLOCK_DIV_NONE = 0,    /**< Do not divide main clock */
    SCU_MAIN_CLOCK_DIV_2    = 1,    /**< Divide the main clock by 2 */
    SCU_MAIN_CLOCK_DIV_4    = 2,    /**< Divide the main clock by 4 */
    SCU_MAIN_CLOCK_DIV_8    = 3,    /**< Divide the main clock by 8 */
    SCU_MAIN_CLOCK_DIV_16   = 4,    /**< Divide the main clock by 16 */
    SCU_MAIN_CLOCK_DIV_32   = 5,    /**< Divide the main clock by 32 */
    SCU_MAIN_CLOCK_DIV_64   = 6,    /**< Divide the main clock by 64 */
    SCU_MAIN_CLOCK_DIV_128  = 7,    /**< Divide the main clock by 128 */
    SCU_MAIN_CLOCK_DIV_256  = 8,    /**< Divide the main clock by 256 */
    SCU_MAIN_CLOCK_DIV_512  = 9,    /**< Divide the main clock by 512 */
};

/**
 * SCU AHB clock divider
 */
enum scu_ahb_clock_div
{
    SCU_AHB_CLOCK_DIV_NONE  = 0,    /**< Do not divide AHB clock */
    SCU_AHB_CLOCK_DIV_2     = 1,    /**< Divide the AHB clock by 2 */
    SCU_AHB_CLOCK_DIV_4     = 2,    /**< Divide the AHB clock by 4 */
    SCU_AHB_CLOCK_DIV_8     = 3,    /**< Divide the AHB clock by 8 */
    SCU_AHB_CLOCK_DIV_16    = 4,    /**< Divide the AHB clock by 16 */
};

/**
 * SCU PLL clock source
 */
enum scu_pll_clock_src
{
    SCU_PLL_CLOCK_SRC_HSI   = 0,    /**< PLL clock source internal high speed clock(32Mhz) */
    SCU_PLL_CLOCK_SRC_HSE   = 1,    /**< PLL clock source external high speed clock */
};

/**
 * SCU PLL clock input divider
 */
enum scu_pll_clock_input_div
{
    SCU_PLL_CLOCK_INPUT_DIV_NONE    = 0,    /**< Do not divide input clock */
    SCU_PLL_CLOCK_INPUT_DIV_2       = 1,    /**< Divide the input clock by 2 */
    SCU_PLL_CLOCK_INPUT_DIV_4       = 2,    /**< Divide the input clock by 4 */
    SCU_PLL_CLOCK_INPUT_DIV_8       = 3,    /**< Divide the input clock by 8 */
};

/**
 * SCU PLL mode
 */
enum scu_pll_mode
{
    SCU_PLL_MODE_VCO_EQUAL      = 0,    /**< The VCO frequency is the same as FOUT */
    SCU_PLL_MODE_VCO_DOUBLER    = 1,    /**< The VCO frequency is twice FOUT */
};

/**
 * SCU clock output selection
 */
enum scu_clock_out_sel
{
    SCU_CLOCK_OUT_SEL_LSI   = 0,        /**< Internal low speed clcok(500Khz) */
    SCU_CLOCK_OUT_SEL_LSE   = 1,        /**< External low speed clcok */
    SCU_CLOCK_OUT_SEL_MCLK  = 2,        /**< Main clock */
    SCU_CLOCK_OUT_SEL_HSI   = 3,        /**< Internal high speed clock(32Mhz) */
    SCU_CLOCK_OUT_SEL_HSE   = 4,        /**< Extern high speed clock */
    SCU_CLOCK_OUT_SEL_PLL   = 5,        /**< PLL clock */
};

/**
 * SCU clock monitoring
 */
enum scu_clock_mon
{
    SCU_MON_CLOCK_MCLK  = 0,            /**< Mornitoring main clock */
    SCU_MON_CLOCK_LSE   = 1,            /**< Mornigoring external low speed clock */
    SCU_MON_CLOCK_HSE   = 2,            /**< Mornigoring external high speed clock */
};

/**
 * SCU clock failure event
 */
enum scu_clock_fail_event
{
    SCU_CLOCK_FAIL_EVENT_MCLK = BIT(0),     /**< Main clock fail event */
    SCU_CLOCK_FAIL_EVENT_LSE  = BIT(1),     /**< Extern low speed clock fail event */
    SCU_CLOCK_FAIL_EVENT_HSE  = BIT(2),     /**< Extern high speed clock fail event */
};

/**
 * SCU reset source
 */
enum scu_reset_source
{
    SCU_RESET_SOURCE_LOCK   = (1 << 9),     /**< Reset by lock up */
    SCU_RESET_SOURCE_PIN    = (1 << 7),     /**< Reset by pin */
    SCU_RESET_SOURCE_CPU    = (1 << 6),     /**< Reset by cpu */
    SCU_RESET_SOURCE_SW     = (1 << 5),     /**< Reset by s/w */
    SCU_RESET_SOURCE_WDT    = (1 << 4),     /**< Reset by watchdog */
    SCU_RESET_SOURCE_CLKF   = (1 << 3),     /**< Reset by clock failure */
    SCU_RESET_SOURCE_LSE    = (1 << 2),     /**< Reset by LSE */
    SCU_RESET_SOURCE_HSE    = (1 << 1),     /**< Reset by HSE */
    SCU_RESET_SOURCE_LVD    = (1 << 0),     /**< Reset by low voltage */
};

/**
 * SCU reset reason
 */
enum scu_reset_reason
{
    SCU_RESET_REASON_LOCK   = (1 << 9),     /**< Reset by lock up */
    SCU_RESET_REASON_POR    = (1 << 8),     /**< Reset by POR */
    SCU_RESET_REASON_PIN    = (1 << 7),     /**< Reset by pin */
    SCU_RESET_REASON_CPU    = (1 << 6),     /**< Reset by cpu */
    SCU_RESET_REASON_SW     = (1 << 5),     /**< Reset by s/w */
    SCU_RESET_REASON_WDT    = (1 << 4),     /**< Reset by watchdog */
    SCU_RESET_REASON_CLKF   = (1 << 3),     /**< Reset by clock failure */
    SCU_RESET_REASON_LSE    = (1 << 2),     /**< Reset by LSE */
    SCU_RESET_REASON_HSE    = (1 << 1),     /**< Reset by HSE */
    SCU_RESET_REASON_LVD    = (1 << 0),     /**< Reset by low voltage */
};

/**
 * SCU wakeup source
 */
enum scu_wakeup_source
{
    SCU_WAKEUP_SOURCE_GPIOG = (1 << 14),    /**< Wakeup by GPIO G */
    SCU_WAKEUP_SOURCE_GPIOF = (1 << 13),    /**< Wakeup by GPIO F */
    SCU_WAKEUP_SOURCE_GPIOE = (1 << 12),    /**< Wakeup by GPIO E */
    SCU_WAKEUP_SOURCE_GPIOD = (1 << 11),    /**< Wakeup by GPIO D */
    SCU_WAKEUP_SOURCE_GPIOC = (1 << 10),    /**< Wakeup by GPIO C */
    SCU_WAKEUP_SOURCE_GPIOB = (1 << 9),     /**< Wakeup by GPIO B */
    SCU_WAKEUP_SOURCE_GPIOA = (1 << 8),     /**< Wakeup by GPIO A */
    SCU_WAKEUP_SOURCE_FRT1  = (1 << 3),     /**< Wakeup by FRT 1 */
    SCU_WAKEUP_SOURCE_FRT0  = (1 << 2),     /**< Wakeup by FRT 0 */
    SCU_WAKEUP_SOURCE_WDT   = (1 << 1),     /**< Wakeup by WDT */
    SCU_WAKEUP_SOURCE_LV    = (1 << 0),     /**< Wakeup by LVI */
};

/**
 * SCU wakeup reason
 */
enum scu_wakeup_reason
{
    SCU_WAKEUP_REASON_GPIOG = (1 << 14),    /**< Wakeup by GPIO G */
    SCU_WAKEUP_REASON_GPIOF = (1 << 13),    /**< Wakeup by GPIO F */
    SCU_WAKEUP_REASON_GPIOE = (1 << 12),    /**< Wakeup by GPIO E */
    SCU_WAKEUP_REASON_GPIOD = (1 << 11),    /**< Wakeup by GPIO D */
    SCU_WAKEUP_REASON_GPIOC = (1 << 10),    /**< Wakeup by GPIO C */
    SCU_WAKEUP_REASON_GPIOB = (1 << 9),     /**< Wakeup by GPIO B */
    SCU_WAKEUP_REASON_GPIOA = (1 << 8),     /**< Wakeup by GPIO A */
    SCU_WAKEUP_REASON_FRT1  = (1 << 3),     /**< Wakeup by FRT 1 */
    SCU_WAKEUP_REASON_FRT0  = (1 << 2),     /**< Wakeup by FRT 0 */
    SCU_WAKEUP_REASON_WDT   = (1 << 1),     /**< Wakeup by WDT */
    SCU_WAKEUP_REASON_LV    = (1 << 0),     /**< Wakeup by LVI */
};

/**
 *******************************************************************************
 * @brief       SCU clock failure interrupt callback function type
 * @param[in]   event event type ::scu_clock_fail_event
 * @param[in]   context context provided during initialization
 ******************************************************************************/
typedef void (*scu_clk_fail_handler_t)(uint32_t event, void *context);

/**
 * SCU main clock configuration
 */
struct scu_main_clock_cfg
{
    enum scu_clock          mclk_sel;       /**<system main clock */
    enum scu_main_clock_div mclk_div;       /**<divide the main clock and use the AHB clock */
    enum scu_ahb_clock_div  hclk_div;       /**<divide the AHB clock and use the peripheral clock */
    uint32_t                mclk_hz;        /**<frequncy of main clock */
};

/**
 * SCU PLL clock configuration
 */
struct scu_pll_clock_cfg
{
    uint8_t                         out_div;            /**<output divide value */
    uint8_t                         post_div1;          /**<post divide 1 value */
    uint8_t                         post_div2;          /**<post divide 2 value */
    uint8_t                         pre_div;            /**<previous divide value */
    enum scu_pll_mode               p_mode;             /**<pll mode */
    enum scu_pll_clock_src          pclk_src;           /**<pll clock source */
    enum scu_pll_clock_input_div    p_input_clk_div;    /**<pll inpurt clcok divide value */
};

/**
 *******************************************************************************
 * @brief       describe
 * @param       xxxx descripe
 * @return      driver error code
 ******************************************************************************/
static __INLINE void SCU_Access_Enable(void)
{
    SCU->SYSTEN = 0x57;
    SCU->SYSTEN = 0x75;
}

/**
 *******************************************************************************
 * @brief       describe
 * @param       xxxx descripe
 * @return      driver error code
 ******************************************************************************/
static __INLINE void SCU_Access_Disable(void)
{
    SCU->SYSTEN = 0x00;
}

/**
 *******************************************************************************
 * @brief       Update system main clock.
                If the main clock is selected as PLL, must be called
 * @param[in]   cfg configration for the main clcok
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_UpdateMainClock(struct scu_main_clock_cfg *cfg);

/**
 *******************************************************************************
 * @brief       Configure PLL
 * @param[in]   cfg configuration for the PLL clock
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_SetPLLConfig(struct scu_pll_clock_cfg *cfg);

/**
 *******************************************************************************
 * @brief       Configure PLL input clock predivider
 * @param[in]   plldiv type of pll input divider to enable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_SetPLLPrediv(enum scu_pll_clock_input_div plldiv);
	
/**
 *******************************************************************************
 * @brief       Enable system clock
 * @param[in]   clk type of clock to enable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_EnableClock(enum scu_clock clk);

/**
 *******************************************************************************
 * @brief       Disable system clock
 * @param[in]   clk type of clock to enable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_DisableClock(enum scu_clock clk);

/**
 *******************************************************************************
 * @brief       Enable clock monitoring
 * @param[in]   clk clock to monitor
 * @param[in]   handler callback function to receive event
 * @param[in]   context caller's context
 * @param[in]   irq_prio interrupt priority
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_EnableClockMonitor(enum scu_clock_mon clk,
                                   scu_clk_fail_handler_t handler,
                                   void *context,
                                   uint8_t irq_prio);
/**
 *******************************************************************************
 * @brief       Disable clock monitoring
 * @param[in]   clk clock to monitor
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_DisableClockMonitor(enum scu_clock_mon clk);

/**
 *******************************************************************************
 * @brief       Enable clock output
 * @param[in]   clk_sel output clock selection
 * @param[in]   clk_div clock divider
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_EnableClockOut(enum scu_clock_out_sel clk_sel, uint8_t clk_div);

/**
 *******************************************************************************
 * @brief       Disable clock output
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_DisableClockOut(void);

/**
 *******************************************************************************
 * @brief       Enable wakeup source
 * @param[in]   src sources (bit ORed value of ::scu_wakeup_source)
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_EnableWakeupSource(uint32_t src);

/**
 *******************************************************************************
 * @brief       Disable wakeup source
 * @param[in]   src sources (bit ORed value of ::scu_wakeup_source)
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_DisableWakeupSource(uint32_t src);

/**
 *******************************************************************************
 * @brief       Enable reset source
 * @param[in]   src sources (bit ORed value of ::scu_reset_source)
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_EnableResetSource(uint32_t src);

/**
 *******************************************************************************
 * @brief       Disable reset source
 * @param[in]   src sources (bit ORed value of ::scu_reset_source)
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_DisableResetSource(uint32_t src);

/**
 *******************************************************************************
 * @brief       This function returns the reset reason.
 *              Once the reason is obtained, it is cleared within this function
 *              so that next reset will have only the latest reset reason only.
 * @param[in]   reason reset reason (::scu_reset_reason)
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_GetResetReason(uint32_t *reason);

/**
 *******************************************************************************
 * @brief       This function returns the wakeup reason.
 *              Once the reason is obtained, wakeup source is disabled within this function
 *              so that the next wakeup has to be enabled again from the application
 * @param[in]   reason wakeup reason (::scu_wakeup_reason)
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_GetWakeupReason(uint32_t *reason);

#endif /* _A34M41X_SCU_H_ */

/** @} */
/** @} */

