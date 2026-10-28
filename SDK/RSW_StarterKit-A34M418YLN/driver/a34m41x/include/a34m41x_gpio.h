/**
 *******************************************************************************
 * @file        a34m41x_gpio.h
 * @author      ABOV R&D Division
 * @brief       GPIO Header File
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
 * @defgroup    driver_gpio GPIO
 * @{
 * @brief       GPIO driver
 */

#ifndef _A34M41X_GPIO_H_
#define _A34M41X_GPIO_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "a34m41x.h"
#include "a34m41x_driver_common.h"

/**
 * GPIO access must be enabled before chaning any register values.
 */
static __INLINE void GPIO_Access_Enable(void)
{
    PORTEN->EN = 0x15;
    PORTEN->EN = 0x51;
}

/**
 * GPIO access must be disabled to protect the settings.
 */
static __INLINE void GPIO_Access_Disable()
{
    PORTEN->EN = 0x00;
}


/* PAMR */

#define PA0_MUX_PA0             (0)
#define PA0_MUX_AN0             (7)

#define PA1_MUX_PA1             (0)
#define PA1_MUX_AN1             (7)

#define PA2_MUX_PA2             (0)
#define PA2_MUX_AN2             (7)

#define PA3_MUX_PA3             (0)
#define PA3_MUX_AN3             (7)

#define PA4_MUX_PA4             (0)
#define PA4_MUX_T0IO            (2)
#define PA4_MUX_CP0A            (7)
#define PA4_MUX_AN4             (7)

#define PA5_MUX_PA5             (0)
#define PA5_MUX_T1IO            (2)
#define PA5_MUX_CAPEU           (3)
#define PA5_MUX_CP0B            (7)
#define PA5_MUX_AN5             (7)

#define PA6_MUX_PA6             (0)
#define PA6_MUX_T2IO            (2)
#define PA6_MUX_CAPEV           (3)
#define PA6_MUX_CP0C            (7)
#define PA6_MUX_AN6             (7)

#define PA7_MUX_PA7             (0)
#define PA7_MUX_T3IO            (2)
#define PA7_MUX_CAPEW           (3)
#define PA7_MUX_CREF0           (7)
#define PA7_MUX_AN7             (7)

#define PA8_MUX_PA8             (0)
#define PA8_MUX_CAN_RX          (1)
#define PA8_MUX_CP1A            (7)
#define PA8_MUX_AN8             (7)

#define PA9_MUX_PA9             (0)
#define PA9_MUX_CAN_TX          (1)
#define PA9_MUX_CP1B            (7)
#define PA9_MUX_AN9             (7)

#define PA10_MUX_PA10           (0)
#define PA10_MUX_RXD1           (1)
#define PA10_MUX_SCAPEU         (3)
#define PA10_MUX_CP1C           (7)
#define PA10_MUX_AN10           (7)

#define PA11_MUX_PA11           (0)
#define PA11_MUX_TXD1           (1)
#define PA11_MUX_SCAPEV         (3)
#define PA11_MUX_CREF1          (7)
#define PA11_MUX_AN11           (7)

#define PA12_MUX_PA12           (0)
#define PA12_MUX_SS0            (1)
#define PA12_MUX_QEI0_UPDN      (2)
#define PA12_MUX_SCAPEW         (3)
#define PA12_MUX_AN12           (7)

#define PA13_MUX_PA13           (0)
#define PA13_MUX_SCK0           (1)
#define PA13_MUX_QEI0_A         (2)
#define PA13_MUX_AN13           (7)

#define PA14_MUX_PA14           (0)
#define PA14_MUX_MOSI0          (1)
#define PA14_MUX_QEI0_B         (2)
#define PA14_MUX_PRTINEV        (3)
#define PA14_MUX_AN14           (7)

#define PA15_MUX_PA15           (0)
#define PA15_MUX_MISO0          (1)
#define PA15_MUX_QEI0_IDX       (2)
#define PA15_MUX_OVINEV         (3)
#define PA15_MUX_AN15           (7)


/* PBMR */

#define PB0_MUX_PB0             (0)
#define PB0_MUX_MPWM0UH         (3)

#define PB1_MUX_PB1             (0)
#define PB1_MUX_MPWM0UL         (3)

#define PB2_MUX_PB2             (0)
#define PB2_MUX_MPWM0VH         (3)

#define PB3_MUX_PB3             (0)
#define PB3_MUX_MPWM0VL         (3)

#define PB4_MUX_PB4             (0)
#define PB4_MUX_T8IO            (2)
#define PB4_MUX_MPWM0WH         (3)

#define PB5_MUX_PB5             (0)
#define PB5_MUX_T9IO            (2)
#define PB5_MUX_MPWM0WL         (3)

#define PB6_MUX_PB6             (0)
#define PB6_MUX_WDTO            (2)
#define PB6_MUX_PRTIN0U         (3)

#define PB7_MUX_PB7             (0)
#define PB7_MUX_STBYO           (2)
#define PB7_MUX_OVIN0U          (3)

#define PB8_MUX_PB8             (0)
#define PB8_MUX_RXD3            (1)
#define PB8_MUX_PRTIN1U         (3)

#define PB9_MUX_PB9             (0)
#define PB9_MUX_TXD3            (1)
#define PB9_MUX_OVIN1U          (3)

#define PB10_MUX_PB10           (0)
#define PB10_MUX_MPWM1UH        (3)

#define PB11_MUX_PB11           (0)
#define PB11_MUX_MPWM1UL        (3)

#define PB12_MUX_PB12           (0)
#define PB12_MUX_MPWM1VH        (3)

#define PB13_MUX_PB13           (0)
#define PB13_MUX_MPWM1VL        (3)

#define PB14_MUX_PB14           (0)
#define PB14_MUX_MPWM1WH        (3)

#define PB15_MUX_PB15           (0)
#define PB15_MUX_MPWM1WL        (3)


/* PCMR */

#define PC0_MUX_PC0             (0)
#define PC0_MUX_RXD0            (1)
#define PC0_MUX_TCK_SWCLK       (3)

#define PC1_MUX_PC1             (0)
#define PC1_MUX_TXD0            (1)
#define PC1_MUX_TMS_SWDIO       (3)

#define PC2_MUX_PC2             (0)
#define PC2_MUX_TDO_SWO         (3)

#define PC3_MUX_PC3             (0)
#define PC3_MUX_TDI             (3)

#define PC4_MUX_PC4             (0)
#define PC4_MUX_T0IO            (2)
#define PC4_MUX_nTRST           (3)

#define PC5_MUX_PC5             (0)
#define PC5_MUX_RXD1            (1)
#define PC5_MUX_T1IO            (2)

#define PC6_MUX_PC6             (0)
#define PC6_MUX_TXD1            (1)
#define PC6_MUX_T2IO            (2)

#define PC7_MUX_PC7             (0)
#define PC7_MUX_SCL0            (1)
#define PC7_MUX_T3IO            (2)

#define PC8_MUX_PC8             (0)
#define PC8_MUX_SDA0            (1)
#define PC8_MUX_T4IO            (2)

#define PC9_MUX_PC9             (0)
#define PC9_MUX_T8IO            (2)
#define PC9_MUX_CLKO            (3)

#define PC10_MUX_PC10           (0)
#define PC10_MUX_nRESET         (3)

#define PC11_MUX_PC11           (0)
#define PC11_MUX_T9IO           (2)
#define PC11_MUX_BOOT           (3)

#define PC12_MUX_PC12           (0)
#define PC12_MUX_XIN            (7)

#define PC13_MUX_PC13           (0)
#define PC13_MUX_XOUT           (7)

#define PC14_MUX_PC14           (0)
#define PC14_MUX_MOSI0          (1)
#define PC14_MUX_RXD0           (3)

#define PC15_MUX_PC15           (0)
#define PC15_MUX_MISO0          (1)
#define PC15_MUX_TXD0           (3)



/* PDMR */

#define PD0_MUX_PD0             (0)
#define PD0_MUX_SS1             (1)
#define PD0_MUX_SXIN            (7)

#define PD1_MUX_PD1             (0)
#define PD1_MUX_SCK1            (1)
#define PD1_MUX_SXOUT           (7)

#define PD2_MUX_PD2             (0)
#define PD2_MUX_MOSI1           (1)

#define PD3_MUX_PD3             (0)
#define PD3_MUX_MISO1           (1)

#define PD4_MUX_PD4             (0)
#define PD4_MUX_SCL1            (1)
#define PD4_MUX_AN16            (7)

#define PD5_MUX_PD5             (0)
#define PD5_MUX_SDA1            (1)
#define PD5_MUX_AN17            (7)

#define PD6_MUX_PD6             (0)
#define PD6_MUX_TXD2            (1)
#define PD6_MUX_AN18            (7)

#define PD7_MUX_PD7             (0)
#define PD7_MUX_RXD2            (1)
#define PD7_MUX_AN19            (7)

#define PD8_MUX_PD8             (0)
#define PD8_MUX_T6IO            (2)
#define PD8_MUX_WDTO            (3)

#define PD9_MUX_PD9             (0)
#define PD9_MUX_T7IO            (2)
#define PD9_MUX_STBYO           (3)

#define PD10_MUX_PD10           (0)
#define PD10_MUX_T0IO           (2)
#define PD10_MUX_AD0S           (3)

#define PD11_MUX_PD11           (0)
#define PD11_MUX_T1IO           (2)
#define PD11_MUX_AD0E           (3)

#define PD12_MUX_PD12           (0)
#define PD12_MUX_T2IO           (2)
#define PD12_MUX_AD1S           (3)

#define PD13_MUX_PD13           (0)
#define PD13_MUX_T3IO           (2)
#define PD13_MUX_AD1E           (3)

#define PD14_MUX_PD14           (0)
#define PD14_MUX_SS0            (1)
#define PD14_MUX_AD2S           (3)

#define PD15_MUX_PD15           (0)
#define PD15_MUX_SCK0           (1)
#define PD15_MUX_AD2E           (3)


/* PEMR */

#define PE0_MUX_PE0             (0)

#define PE1_MUX_PE1             (0)
#define PE1_MUX_CP2             (7)
#define PE1_MUX_AN20            (7)

#define PE2_MUX_PE2             (0)
#define PE2_MUX_CREF2           (7)
#define PE2_MUX_AN21            (7)

#define PE3_MUX_PE3             (0)
#define PE3_MUX_SCL0            (1)

#define PE4_MUX_PE4             (0)
#define PE4_MUX_SDA0            (1)

#define PE5_MUX_PE5             (0)
#define PE5_MUX_T5IO            (2)

#define PE6_MUX_PE6             (0)
#define PE6_MUX_T5IO            (2)
#define PE6_MUX_QEI1_UPDN       (3)

#define PE7_MUX_PE7             (0)
#define PE7_MUX_T6IO            (2)
#define PE7_MUX_QEI1_A          (3)

#define PE8_MUX_PE8             (0)
#define PE8_MUX_T7IO            (2)
#define PE8_MUX_QEI1_B          (3)

#define PE9_MUX_PE9             (0)
#define PE9_MUX_T8IO            (2)
#define PE9_MUX_QEI1_IDX        (3)

#define PE10_MUX_PE10           (0)
#define PE10_MUX_T9IO           (2)

#define PE11_MUX_PE11           (0)
#define PE11_MUX_SCL1           (1)
#define PE11_MUX_T0IO           (2)

#define PE12_MUX_PE12           (0)
#define PE12_MUX_SDA1           (1)
#define PE12_MUX_T1IO           (2)

#define PE13_MUX_PE13           (0)
#define PE13_MUX_TXD4           (1)
#define PE13_MUX_T2IO           (2)

#define PE14_MUX_PE14           (0)
#define PE14_MUX_RXD4           (1)
#define PE14_MUX_T3IO           (2)

#define PE15_MUX_PE15           (0)


/* PFMR */

#define PF0_MUX_PF0             (0)

#define PF1_MUX_PF1             (0)

#define PF2_MUX_PF2             (0)
#define PF2_MUX_CP3             (7)
#define PF2_MUX_AN22            (7)

#define PF3_MUX_PF3             (0)
#define PF3_MUX_CREF3           (7)
#define PF3_MUX_AN23            (7)

#define PF4_MUX_PF4             (0)
#define PF4_MUX_TXD5            (1)

#define PF5_MUX_PF5             (0)
#define PF5_MUX_RXD5            (1)

#define PF6_MUX_PF6             (0)
#define PF6_MUX_CAN_RX          (1)
#define PF6_MUX_PRTINEW         (3)

#define PF7_MUX_PF7             (0)
#define PF7_MUX_CAN_TX          (1)
#define PF7_MUX_OVINEW          (3)

#define PF8_MUX_PF8             (0)

#define PF9_MUX_PF9             (0)

#define PF10_MUX_PF10           (0)

#define PF11_MUX_PF11           (0)

#define PF12_MUX_PF12           (0)

#define PF13_MUX_PF13           (0)

#define PF14_MUX_PF14           (0)

#define PF15_MUX_PF15           (0)


/* PGMR */

#define PG0_MUX_PG0             (0)
#define PG0_MUX_SS2             (1)

#define PG1_MUX_PG1             (0)
#define PG1_MUX_SCK2            (1)

#define PG2_MUX_PG2             (0)
#define PG2_MUX_MOSI2           (1)

#define PG3_MUX_PG3             (0)
#define PG3_MUX_MISO2           (1)

#define PG4_MUX_PG4             (0)

#define PG5_MUX_PG5             (0)

#define PG6_MUX_PG6             (0)

#define PG7_MUX_PG7             (0)

#define PG8_MUX_PG8             (0)
#define PG8_MUX_RXD3            (1)

#define PG9_MUX_PG9             (0)
#define PG9_MUX_TXD3            (1)

#define PG10_MUX_PG10           (0)



/**
 * GPIO port number
 */
enum gpio_port
{
    PORTA       = 0,
    PORTB       = 1,
    PORTC       = 2,
    PORTD       = 3,
    PORTE       = 4,
    PORTF       = 5,
    PORTG       = 6,
    PORT_MAX    = 7,
};

/**
 * GPIO pin number
 */
enum gpio_pin
{
    PIN0    = 0,
    PIN1    = 1,
    PIN2    = 2,
    PIN3    = 3,
    PIN4    = 4,
    PIN5    = 5,
    PIN6    = 6,
    PIN7    = 7,
    PIN8    = 8,
    PIN9    = 9,
    PIN10   = 10,
    PIN11   = 11,
    PIN12   = 12,
    PIN13   = 13,
    PIN14   = 14,
    PIN15   = 15,
    PIN_MAX = 16,
};

/**
 * GPIO mode
 */
enum gpio_mode
{
    GPIO_MODE_INPUT     = 0,                /** intput */
    GPIO_MODE_OUTPUT    = 1,                /** output */
    GPIO_MODE_FUNC      = 2,                /** alternative function */
    GPIO_MODE_MAX       = 3,                /** Mode Max */
};

/**
 * GPIO output mode configuration
 */
enum gpio_output_type
{
    GPIO_OUTPUT_PUSH_PULL   = 0,            /**< push pull */
    GPIO_OUTPUT_OPEN_DRAIN  = 1,            /**< open drain */
};

/**
 * GPIO pull up down configuration
 */
enum gpio_pull
{
    GPIO_PULL_DISABLE   = 0,                /**< pull up pull down not used */
    GPIO_PULL_UP        = 2,                /**< pull up */
    GPIO_PULL_DOWN      = 3,                /**< pull down */
};

/**
 * GPIO interrupt triger control
 */
enum gpio_irq_trig
{
    GPIO_IRQ_TRIG_LEVEL = 0x01,             /**< level trigger */
    GPIO_IRQ_TRIG_EDGE  = 0x03,             /**< edge trigger */
};

/**
 * GPIO interrupt mode control
 */
enum gpio_irq_mode
{
    GPIO_IRQ_LOW_OR_FALLING     = 0x01,     /**< low level or falling edge */
    GPIO_IRQ_HIGH_OR_RISING     = 0x02,     /**< high level or rising edge */
    GPIO_IRQ_FALLING_AND_RISING = 0x03,     /**< falling/rising */
};

/**
 * GPIO strength
 */
enum gpio_strength
{
    GPIO_STRENGTH_LEVEL_1   = 0,            /**< strength level 1 */
    GPIO_STRENGTH_LEVEL_2   = 1,            /**< strength level 2 */
    GPIO_STRENGTH_LEVEL_3   = 2,            /**< strength level 3 */
    GPIO_STRENGTH_LEVEL_4   = 3,            /**< strength level 4 */
};

/**
 * GPIO debounce clock configuration
 */
enum gpio_dbc_clk_src
{
    GPIO_DBC_CLK_SRC_LSI,               /**< debounce clock from LSI */
    GPIO_DBC_CLK_SRC_LSE,               /**< debounce clock from LSE */
    GPIO_DBC_CLK_SRC_MCLK,              /**< debounce clock from MCLK */
    GPIO_DBC_CLK_SRC_HSI,               /**< debounce clock from HSI */
    GPIO_DBC_CLK_SRC_HSE,               /**< debounce clock from HSE */
    GPIO_DBC_CLK_SRC_PLL,               /**< debounce clock from PLL */
};


/**
 * number of irqs to support.
 * If an application requires more than this number of GPIO inputs
 * to be used as interrupt pins, then increase the number as much
 * as needed.
 */
#ifndef CONFIG_GPIO_NUM_IRQ
#define CONFIG_GPIO_NUM_IRQ    4
#endif

/**
 *******************************************************************************
 * @brief       GPIO interrupt callback function type
 * @param[in]   context context provided during initialization
 ******************************************************************************/
typedef void (*gpio_irq_handler_t)(void *context);

/**
 *******************************************************************************
 * @brief       Initialize GPIO.
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_Init(void);

/**
 *******************************************************************************
 * @brief       Set mode.
 * @param[in]   port port number
 * @param[in]   pin pin number
 * @param[in]   mode mode to be used
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_SetMode(enum gpio_port port, enum gpio_pin pin, enum gpio_mode mode);

/**
 *******************************************************************************
 * @brief       Set function.
 *              GPIO must be configured as function first.
 * @param[in]   port port number
 * @param[in]   pin pin number
 * @param[in]   func function to be used instead of input/output
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_SetFunction(enum gpio_port port, enum gpio_pin pin, uint32_t func);

/**
 *******************************************************************************
 * @brief       Set output type
 * @param[in]   port port number
 * @param[in]   pin pin number
 * @param[in]   type output type
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_SetOutput(enum gpio_port port, enum gpio_pin pin, enum gpio_output_type type);

/**
 *******************************************************************************
 * @brief       Set pull up or down
 * @param[in]   port port number
 * @param[in]   pin pin number
 * @param[in]   pull pull up/down/none
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_SetPull(enum gpio_port port, enum gpio_pin pin, enum gpio_pull pull);

/**
 *******************************************************************************
 * @brief       Set strength
 * @param[in]   port port number
 * @param[in]   pin pin number
 * @param[in]   strength strength level
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_SetStrength(enum gpio_port port, enum gpio_pin pin, enum gpio_strength strength);

/**
 *******************************************************************************
 * @brief       Set GPIO Debouncing.
 * @param[in]   port port number
 * @param[in]   pin pin number
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_SetDebounce(enum gpio_port port, enum gpio_pin pin, uint8_t enable);

/**
 *******************************************************************************
 * @brief       Set GPIO Debouncing Clock
 * @param[in]   port port number
 * @param[in]   clk_src debounce clock source
 * @param[in]   clk_div debounce clock divider
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_SetDebounceClock(enum gpio_port port, enum gpio_dbc_clk_src clk_src,
                                  uint8_t clk_div);

/**
 *******************************************************************************
 * @brief       Enable interrupt.
 *              Interrupt are configured per port, interrupt priority can be
 *              set only once for each port.
 * @param[in]   port port number
 * @param[in]   pin pin number
 * @param[in]   trig interrupt trigger
 * @param[in]   mode interrupt mode
 * @param[in]   irq_prio interrupt priority
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_EnableIRQ(enum gpio_port port, enum gpio_pin pin,
                           enum gpio_irq_trig trig,
                           enum gpio_irq_mode mode,
                           uint8_t irq_prio);

/**
 *******************************************************************************
 * @brief       Disable interrupt.
 * @param[in]   port port number
 * @param[in]   pin pin number
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_DisableIRQ(enum gpio_port port, enum gpio_pin pin);

/**
 *******************************************************************************
 * @brief       Set GPIO callback function to receive event.
 * @param[in]   port port number
 * @param[in]   pin pin number
 * @param[in]   handler callback function to receive event
 * @param[in]   context caller's context
 * @return      driver error code
 * @return      ::DRIVER_ERROR_NO_RESOURCE no more gpio irq can be set.
 *              See ::CONFIG_GPIO_NUM_IRQ
 ******************************************************************************/
int32_t HAL_GPIO_SetIRQHandler(enum gpio_port port, enum gpio_pin pin, gpio_irq_handler_t handler, void *context);

/**
 *******************************************************************************
 * @brief       Set (high) the value of the specific port/pin.
 * @param[in]   port port number
 * @param[in]   pin pin number
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_SetPin(enum gpio_port port, enum gpio_pin pin);

/**
 *******************************************************************************
 * @brief       Clear (low) the value of the specific port/pin.
 * @param[in]   port port number
 * @param[in]   pin pin number
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_ClearPin(enum gpio_port port, enum gpio_pin pin);

/**
 *******************************************************************************
 * @brief       Read the value of the specific port/pin.
 * @param[in]   port port number
 * @param[in]   pin pin number
 * @param[out]  value value of the pin
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_ReadPin(enum gpio_port port, enum gpio_pin pin, uint8_t *value);

/**
 *******************************************************************************
 * @brief       Write the value for the whole GPIO port.
 *              It only sets the value for the pins configured as output.
 *              Other bits are not valid.
 * @param[in]   port port number
 * @param[in]   value port value
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_WritePort(enum gpio_port port, uint8_t value);

/**
 *******************************************************************************
 * @brief       Read the value for the whole GPIO port.
 *              It only returns the value for the pins configured as input.
 *              Other bits are not valid.
 * @param[in]   port port number
 * @param[out]  value port value
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_ReadPort(enum gpio_port port, uint8_t *value);

#ifdef __cplusplus
}
#endif

#endif /* _A34M41X_GPIO_H_ */

/** @} */
/** @} */

