/**
 *******************************************************************************
 * @file        a34m41x_gpio.c
 * @author      ABOV R&D Division
 * @brief       General Purpose Input Output
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
#include "a34m41x_gpio.h"
#include "a34m41x_scu.h"

#define PIN_OFFSET(pin)         (pin << 1)

#define PCU_MR_FUNC_Msk         (0x07UL)
#define PCU_CR_MODE_Msk         (0x03UL)
#define PCU_PRCR_Msk            (0x03UL)
#define PCU_STR_Msk             (0x01UL)

#define GPIO_REG_OFFSET 0x100

static __INLINE PCU_Type *GPIO_REG(enum gpio_port port)
{
    return (PCU_Type *)(PA_BASE + (GPIO_REG_OFFSET * port));
}

static __INLINE IRQn_Type GPIO_IRQ_NUM(enum gpio_port port)
{
    return (IRQn_Type)((uint8_t)GPIOA_IRQn + port);
}

struct gpio_ctrl_block
{
    enum gpio_port      port;
    enum gpio_pin       pin;
    gpio_irq_handler_t  handler;
    void *              context;
};

static struct gpio_ctrl_block g_gcb[CONFIG_GPIO_NUM_IRQ];

static uint8_t gpio_irq_setup[PORT_MAX];

static int8_t GPIO_EmptySlot(enum gpio_port port, enum gpio_pin pin)
{
    uint8_t i;

    for (i = 0; i < CONFIG_GPIO_NUM_IRQ; i++)
    {
        if (g_gcb[i].port == port && g_gcb[i].pin == pin)
        {
            return i;
        }

        if (g_gcb[i].handler == NULL)
        {
            return i;
        }
    }
    return -1;
}

static void GPIO_SetupIRQ(enum gpio_port port, uint8_t irq_prio)
{
    if (gpio_irq_setup[port])
    {
        return;
    }

    NVIC_ClearPendingIRQ(GPIO_IRQ_NUM(port));
    NVIC_SetPriority(GPIO_IRQ_NUM(port), irq_prio);
    NVIC_EnableIRQ(GPIO_IRQ_NUM(port));

    gpio_irq_setup[port] = 1;
}

int32_t HAL_GPIO_Init(void)
{
    PCU_Type *pcu;
    uint8_t i;

    SCU->PER1 |= (1 << SCU_PER1_GPIOA_Pos) |
                 (1 << SCU_PER1_GPIOB_Pos) |
                 (1 << SCU_PER1_GPIOC_Pos) |
                 (1 << SCU_PER1_GPIOD_Pos) |
                 (1 << SCU_PER1_GPIOE_Pos) |
                 (1 << SCU_PER1_GPIOF_Pos) |
                 (1 << SCU_PER1_GPIOG_Pos);

    SCU->PCER1 |= (1 << SCU_PCER1_GPIOA_Pos) |
                  (1 << SCU_PCER1_GPIOB_Pos) |
                  (1 << SCU_PCER1_GPIOC_Pos) |
                  (1 << SCU_PCER1_GPIOD_Pos) |
                  (1 << SCU_PCER1_GPIOE_Pos) |
                  (1 << SCU_PCER1_GPIOF_Pos) |
                  (1 << SCU_PCER1_GPIOG_Pos);

    for (i = 0; i < PORT_MAX; i++)
    {
        pcu = GPIO_REG((enum gpio_port)i);
        if (i == PORTC)
        {
						pcu->ODR =0; /* gpio output default 0 (low) */
            pcu->MR1 = PC4_MUX_nTRST << PCU_MR1_P4MUX_Pos |
                       PC3_MUX_TDI << PCU_MR1_P3MUX_Pos |
                       PC2_MUX_TDO_SWO << PCU_MR1_P2MUX_Pos |
                       PC1_MUX_TMS_SWDIO << PCU_MR1_P1MUX_Pos |
                       PC0_MUX_TCK_SWCLK << PCU_MR1_P0MUX_Pos;

            pcu->MR2 = PC13_MUX_XOUT << PCU_MR2_P13MUX_Pos |
                       PC12_MUX_XIN << PCU_MR2_P12MUX_Pos |
                       PC11_MUX_BOOT << PCU_MR2_P11MUX_Pos |
                       PC10_MUX_nRESET << PCU_MR2_P10MUX_Pos |
                       PC9_MUX_CLKO << PCU_MR2_P9MUX_Pos;
        }
        else if (i == PORTD)
        {
						pcu->ODR =0;	/* gpio output default 0 (low) */
            pcu->MR1 = PD1_MUX_SXOUT << PCU_MR1_P1MUX_Pos |
                       PD0_MUX_SXIN << PCU_MR1_P0MUX_Pos;
        }
        else
        {
						pcu->ODR =0;	/* gpio output default 0 (low) */
            pcu->MR1 = 0;
            pcu->MR2 = 0;
        }
        pcu->CR = 0;
        pcu->PRCR = 0;
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_GPIO_SetMode(enum gpio_port port, enum gpio_pin pin, enum gpio_mode mode)
{
    PCU_Type *pcu;
    uint32_t reg_val;

    DRIVER_ASSERT(port < PORT_MAX && pin < PIN_MAX && mode < GPIO_MODE_MAX);

    pcu = GPIO_REG(port);

    int type = 0;

    if (mode == GPIO_MODE_OUTPUT)
    {
        /* default push pull type */
        type = 0x00;
    }
    else if (mode == GPIO_MODE_INPUT)
    {
        type = 0x02;
    }

    reg_val = pcu->CR;
    reg_val &= ~(PCU_CR_MODE_Msk << (pin << 1));
    reg_val |= (type << (pin << 1));
    pcu->CR = reg_val;

    return DRIVER_ERROR_OK;
}

int32_t HAL_GPIO_SetFunction(enum gpio_port port, enum gpio_pin pin, uint32_t func)
{
    PCU_Type *pcu;
    uint8_t pin_offset;
    uint32_t reg_val;

    pcu = GPIO_REG(port);

    if (pin > 7)
    {
        pin_offset = ((pin - 8) << 2);

        reg_val = pcu->MR2;
        reg_val &= ~(PCU_MR_FUNC_Msk << pin_offset);
        reg_val |= (func << pin_offset);

        pcu->MR2 = reg_val;
    }
    else
    {
        pin_offset = (pin << 2);

        reg_val = pcu->MR1;
        reg_val &= ~(PCU_MR_FUNC_Msk << pin_offset);
        reg_val |= (func << pin_offset);

        pcu->MR1 = reg_val;

    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_GPIO_SetOutput(enum gpio_port port, enum gpio_pin pin, enum gpio_output_type type)
{
    PCU_Type *pcu;
    uint32_t reg_val;

    DRIVER_ASSERT(port < PORT_MAX && pin < PIN_MAX);

    pcu = GPIO_REG(port);

    reg_val = pcu->CR;
    reg_val &= ~(PCU_CR_MODE_Msk << (pin << 1));
    reg_val |= (type << (pin << 1));
    pcu->CR = reg_val;

    return DRIVER_ERROR_OK;
}

int32_t HAL_GPIO_SetPull(enum gpio_port port, enum gpio_pin pin, enum gpio_pull pull)
{
    PCU_Type *pcu;
    uint32_t reg_val;

    DRIVER_ASSERT(port < PORT_MAX && pin < PIN_MAX);

    pcu = GPIO_REG(port);

    reg_val = pcu->PRCR;
    reg_val &= ~(PCU_PRCR_Msk << (pin << 1));
    reg_val |= (pull << (pin << 1));
    pcu->PRCR = reg_val;

    return DRIVER_ERROR_OK;
}

int32_t HAL_GPIO_SetStrength(enum gpio_port port, enum gpio_pin pin, enum gpio_strength strength)
{
    PCU_Type *pcu;
    uint32_t reg_val;

    DRIVER_ASSERT(port < PORT_MAX && pin < PIN_MAX);

    pcu = GPIO_REG(port);

    reg_val = pcu->STR;
    reg_val &= ~(PCU_STR_Msk << pin);
    reg_val |= (strength << pin);
    pcu->STR = reg_val;

    return DRIVER_ERROR_OK;
}

int32_t HAL_GPIO_SetDebounce(enum gpio_port port, enum gpio_pin pin, uint8_t enable)
{
    PCU_Type *pcu;
    uint32_t reg_val;

    DRIVER_ASSERT(port < PORT_MAX && pin < PIN_MAX);

    pcu = GPIO_REG(port);

    reg_val = pcu->DER;
    reg_val &= ~(1 << pin);
    if (enable)
    {
        reg_val |= (1 << pin);
    }
    pcu->DER = reg_val;

    return DRIVER_ERROR_OK;
}

int32_t HAL_GPIO_SetDebounceClock(enum gpio_port port, enum gpio_dbc_clk_src clk_src,
                                  uint8_t clk_div)
{
    uint32_t clk_val = 0;

    DRIVER_ASSERT(port < PORT_MAX);
    DRIVER_ASSERT(clk_div > 0);

    switch (port)
    {
        case PORTA:
        case PORTB:
            clk_val = SCU->MCCR4;
            clk_val &= ~(SCU_MCCR4_PGADCSEL_Msk | SCU_MCCR4_PGADCDIV_Msk);
            SCU->MCCR4 = clk_val | (clk_src << SCU_MCCR4_PGADCSEL_Pos) | (clk_div << SCU_MCCR4_PGADCDIV_Pos);
            break;
        case PORTC:
        case PORTD:
            clk_val = SCU->MCCR5;
            clk_val &= ~(SCU_MCCR5_PGBDCSEL_Msk | SCU_MCCR5_PGBDCDIV_Msk);
            SCU->MCCR5 = clk_val | (clk_src << SCU_MCCR5_PGBDCSEL_Pos) | (clk_div << SCU_MCCR5_PGBDCDIV_Pos);
            break;
        case PORTE:
        case PORTF:
        case PORTG:
            clk_val = SCU->MCCR5;
            clk_val &= ~(SCU_MCCR5_PGCDCSEL_Msk | SCU_MCCR5_PGCDCDIV_Msk);
            SCU->MCCR5 = clk_val | (clk_src << SCU_MCCR5_PGCDCSEL_Pos) | (clk_div << SCU_MCCR5_PGCDCDIV_Pos);
            break;
        default:
            break;
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_GPIO_SetIRQHandler(enum gpio_port port, enum gpio_pin pin, gpio_irq_handler_t handler, void *context)
{
    int8_t i;

    DRIVER_ASSERT(port < PORT_MAX && pin < PIN_MAX);

    i = GPIO_EmptySlot(port, pin);
    if (i == -1)
    {
        return DRIVER_ERROR_NO_RESOURCE;
    }

    g_gcb[i].port = port;
    g_gcb[i].pin = pin;
    g_gcb[i].handler = handler;
    g_gcb[i].context = context;

    return DRIVER_ERROR_OK;
}

int32_t HAL_GPIO_EnableIRQ(enum gpio_port port, enum gpio_pin pin,
                           enum gpio_irq_trig trig,
                           enum gpio_irq_mode mode,
                           uint8_t irq_prio)
{
    PCU_Type *pcu;
    uint32_t ier, icr, isr;

    DRIVER_ASSERT(port < PORT_MAX && pin < PIN_MAX);

    pcu = GPIO_REG(port);

    ier = pcu->IER;
    icr = pcu->ICR;
    isr = pcu->ISR;

    ier &= ~(0x03 << PIN_OFFSET(pin));
    icr &= ~(0x03 << PIN_OFFSET(pin));
    isr &= (0x03 << PIN_OFFSET(pin));

    ier |= (trig << PIN_OFFSET(pin));
    icr |= (mode << PIN_OFFSET(pin));

    pcu->IER = ier;
    pcu->ICR = icr;

    GPIO_SetupIRQ(port, irq_prio);

    return DRIVER_ERROR_OK;
}

int32_t HAL_GPIO_DisableIRQ(enum gpio_port port, enum gpio_pin pin)
{
    PCU_Type *pcu;
    uint32_t ier, icr, isr;

    DRIVER_ASSERT(port < PORT_MAX && pin < PIN_MAX);

    pcu = GPIO_REG(port);

    ier = pcu->IER;
    icr = pcu->ICR;
    isr = pcu->ISR;

    ier &= ~(0x03 << PIN_OFFSET(pin));
    icr &= ~(0x03 << PIN_OFFSET(pin));
    isr &= (0x03 << PIN_OFFSET(pin));

    pcu->IER = ier;
    pcu->ICR = icr;
    pcu->ISR = isr;

    if (!ier)
    {
        NVIC_DisableIRQ(GPIO_IRQ_NUM(port));
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_GPIO_SetPin(enum gpio_port port, enum gpio_pin pin)
{
    PCU_Type *pcu;

    DRIVER_ASSERT(port < PORT_MAX && pin < PIN_MAX);

    pcu = GPIO_REG(port);

    pcu->PSR = (1 << pin);

    return DRIVER_ERROR_OK;
}

int32_t HAL_GPIO_ClearPin(enum gpio_port port, enum gpio_pin pin)
{
    PCU_Type *pcu;

    DRIVER_ASSERT(port < PORT_MAX && pin < PIN_MAX);

    pcu = GPIO_REG(port);
    pcu->PCR = (1 << pin);

    return DRIVER_ERROR_OK;
}

int32_t HAL_GPIO_ReadPin(enum gpio_port port, enum gpio_pin pin, uint8_t *value)
{
    PCU_Type *pcu;
    uint16_t reg_val;

    DRIVER_ASSERT(port < PORT_MAX && pin < PIN_MAX);

    pcu = GPIO_REG(port);

    reg_val = pcu->IDR;

    if ((reg_val >> (pin)) & 0x01)
    {
        *value = 1;
    }
    else
    {
        *value = 0;
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_GPIO_WritePort(enum gpio_port port, uint8_t value)
{
    PCU_Type *pcu;

    DRIVER_ASSERT(port < PORT_MAX);

    pcu = GPIO_REG(port);

    pcu->ODR = value;

    return DRIVER_ERROR_OK;
}

int32_t HAL_GPIO_ReadPort(enum gpio_port port, uint8_t *value)
{
    PCU_Type *pcu;

    DRIVER_ASSERT(port < PORT_MAX);

    pcu = GPIO_REG(port);

    *value = pcu->IDR;

    return DRIVER_ERROR_OK;
}

static void GPIO_Handler(enum gpio_port port)
{
    PCU_Type *pcu;
    uint32_t isr;
    uint8_t pin;
    uint8_t i;
    struct gpio_ctrl_block *gcb;

    pcu = GPIO_REG(port);
    isr = pcu->ISR;

    gcb = &g_gcb[port];

    for (pin = 0; pin < PIN_MAX; pin++)
    {
        if ((isr >> (pin << 1)) & 0x03)
        {
            for (i = 0; i < CONFIG_GPIO_NUM_IRQ; i++)
            {
                gcb = &g_gcb[i];
                if (gcb->port == port && gcb->pin == pin)
                {
                    if (gcb->handler)
                    {
                        gcb->handler(gcb->context);
                    }
                    break;
                }
            }
        }
    }
    pcu->ISR = isr;
}

void GPIOA_IRQHandler(void)
{
    GPIO_Handler(PORTA);
}

void GPIOB_IRQHandler(void)
{
    GPIO_Handler(PORTB);
}

void GPIOC_IRQHandler(void)
{
    GPIO_Handler(PORTC);
}

void GPIOD_IRQHandler(void)
{
    GPIO_Handler(PORTD);
}

void GPIOE_IRQHandler(void)
{
    GPIO_Handler(PORTE);
}

void GPIOF_IRQHandler(void)
{
    GPIO_Handler(PORTF);
}

void GPIOG_IRQHandler(void)
{
    GPIO_Handler(PORTG);
}

