/**
 *******************************************************************************
 * @file        a34m41x_uart.c
 * @author      ABOV R&D Division
 * @brief       Uart Serial Communication
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

#include "a34m41x.h"
#include "a34m41x_scu.h"
#include "a34m41x_uart.h"

#define UART_IIR_RX_LINE_ERROR  (0x06)
#define UART_IIR_RX_DATA_AVAIL  (0x04)
#define UART_IIR_TX_HOLD_EMPTY  (0x02)
#define UART_IIR_RX_DMA_DONE    (0x0C)
#define UART_IIR_TX_DMA_DONE    (0x0A)

#define UART_REG_OFFSET     0x100

static __INLINE UART_Type *UART_REG(enum uart_id id)
{
    return (UART_Type *)(UART0_BASE + (UART_REG_OFFSET * id));
}

static __INLINE IRQn_Type UART_IRQ_NUM(enum uart_id id)
{
    return (IRQn_Type)((uint8_t)UART0_IRQn + id);
}

static __INLINE enum dma_peri UART_RX_DMA_PERI(enum uart_id id)
{
    return (enum dma_peri)(DMA_PERI_UART0_RX + (id * 2));
}

static __INLINE enum dma_peri UART_TX_DMA_PERI(enum uart_id id)
{
    return (enum dma_peri)(DMA_PERI_UART0_TX + (id * 2));
}

struct uart_ctrl_block
{
    uart_irq_handler_t  handler;
    void *              context;
    uint32_t            tx_busy : 1;
    uint32_t            rx_busy : 1;
    uint8_t *           tx_buf;
    uint8_t *           rx_buf;
    uint32_t            tx_len;
    uint32_t            rx_len;
    volatile uint32_t   tx_count;
    volatile uint32_t   rx_count;
    enum dma_ch         tx_dma_ch;
    uint8_t             tx_dma_enable;
    enum dma_ch         rx_dma_ch;
    uint8_t             rx_dma_enable;
};

static struct uart_ctrl_block g_ucb[UART_ID_MAX];

static int32_t UART_Configure(struct uart_ctrl_block *ucb,
                              UART_Type *uart,
                              struct uart_cfg *cfg)
{
    uint32_t cfg_val = 0;
    uint32_t numerator;
    uint32_t denominator;
    uint32_t bdr, bfr;
    uint32_t fd;
    uint32_t div;

    switch (cfg->data_bit)
    {
        case UART_DATA_BIT_5:
        case UART_DATA_BIT_6:
        case UART_DATA_BIT_7:
        case UART_DATA_BIT_8:
            cfg_val |= (cfg->data_bit << UART_LCR_DLEN_Pos);
            break;
        default:
            return DRIVER_ERROR_PARAMETER;
    }

    switch (cfg->parity_bit)
    {
        case UART_PARITY_BIT_NONE:
            break;
        case UART_PARITY_BIT_ODD:
            cfg_val |= (1 << UART_LCR_PEN_Pos);
            break;
        case UART_PARITY_BIT_EVEN:
            cfg_val |= (1 << UART_LCR_PEN_Pos);
            cfg_val |= (1 << UART_LCR_PARITY_Pos);
            break;
        case UART_PARITY_SP_0:
            cfg_val |= (1 << UART_LCR_PEN_Pos);
            cfg_val |= (2 << UART_LCR_PARITY_Pos);
            break;
        case UART_PARITY_SP_1:
            cfg_val |= (1 << UART_LCR_PEN_Pos);
            cfg_val |= (3 << UART_LCR_PARITY_Pos);
            break;
        default:
            return DRIVER_ERROR_PARAMETER;
    }

    switch (cfg->stop_bit)
    {
        case UART_STOP_BIT_1:
            break;
        case UART_STOP_BIT_2:
            cfg_val |= (1 << UART_LCR_STOPBIT_Pos);
            break;
        default:
            return DRIVER_ERROR_PARAMETER;
    }

    uart->LCR = cfg_val;

    div = (SCU->MCCR7 & SCU_MCCR7_UARTCDIV_Msk) >> SCU_MCCR7_UARTCDIV_Pos;
    numerator = SystemCoreClock / div;

    denominator = 16 * cfg->baudrate;

    bdr = numerator / denominator;
    fd = numerator - (bdr * denominator);
    bfr = (fd * 256) / denominator;

    uart->BDR = bdr & 0xFFFF;
    uart->BFR = bfr & 0xFF;

    return DRIVER_ERROR_OK;
}

int32_t HAL_UART_Init(enum uart_id id, struct uart_cfg *cfg,
                      uart_irq_handler_t handler,
                      void *context)
{
    struct uart_ctrl_block *ucb;
    UART_Type *uart;
    uint32_t reg_val;
    int32_t ret;

    DRIVER_ASSERT(id < UART_ID_MAX);
    DRIVER_ASSERT(cfg != NULL);

    ucb = &g_ucb[id];
    uart = UART_REG(id);

    memset(ucb, 0, sizeof(struct uart_ctrl_block));

    ucb->handler = handler;
    ucb->context = context;

    SCU->PER2 |= (1 << (SCU_PER2_UART0_Pos + id));
    SCU->PCER2 |= (1 << (SCU_PCER2_UART0_Pos + id));

    SCU->MCCR7 = 2 << SCU_MCCR7_UARTCDIV_Pos |
                 4 << SCU_MCCR7_UARTCSEL_Pos;

    while (uart->LSR & UART_LSR_DR_Msk)
    {
        reg_val = uart->RBR;
    }

    while (!(uart->LSR & UART_LSR_THRE_Msk))
    {
    }

    uart->IER = 0;
    uart->LCR = 0;
    uart->DCR = 0;

    reg_val = uart->LSR;
    reg_val = uart->IIR;

    (void)reg_val;

    ret = UART_Configure(ucb, uart, cfg);
    if (ret != DRIVER_ERROR_OK)
    {
        HAL_UART_Deinit(id);
        return ret;
    }

    NVIC_ClearPendingIRQ(UART_IRQ_NUM(id));
    NVIC_SetPriority(UART_IRQ_NUM(id), cfg->irq_prio);
    NVIC_EnableIRQ(UART_IRQ_NUM(id));

    return ret;
}

int32_t HAL_UART_Deinit(enum uart_id id)
{
    DRIVER_ASSERT(id < UART_ID_MAX);

    SCU->PER2 &= ~(1 << (SCU_PER2_UART0_Pos + id));
    SCU->PCER2 &= ~(1 << (SCU_PCER2_UART0_Pos + id));

    NVIC_DisableIRQ(UART_IRQ_NUM(id));

    return DRIVER_ERROR_OK;
}

int32_t HAL_UART_Transmit(enum uart_id id, uint8_t *out, uint32_t len,
                          enum uart_io_type io_type)
{
    struct uart_ctrl_block *ucb;
    UART_Type *uart;

    DRIVER_ASSERT(id < UART_ID_MAX);
    DRIVER_ASSERT(len > 0);

    ucb = &g_ucb[id];
    uart = UART_REG(id);

    if (ucb->tx_busy == 1)
    {
        return DRIVER_ERROR_BUSY;
    }

    ucb->tx_busy = 1;

    ucb->tx_buf = (uint8_t *)out;
    ucb->tx_len = len;
    ucb->tx_count = 0;

    if (io_type == UART_IO_TYPE_BLOCK)
    {
        while (ucb->tx_len > ucb->tx_count)
        {
            uart->THR = ucb->tx_buf[ucb->tx_count++];
            while (!(uart->LSR & UART_LSR_THRE_Msk))
            {
            }
        }

        ucb->tx_busy = 0;
    }
    else
    {
        if (ucb->tx_dma_enable)
        {
            uart->IER |= (1 << UART_IER_DTXIEN_Pos);

            HAL_DMA_Start(ucb->tx_dma_ch, (uint32_t)ucb->tx_buf, (uint32_t)&uart->THR, ucb->tx_len);
        }
        else
        {
            if (uart->LSR & UART_LSR_THRE_Msk)
            {
                uart->THR = ucb->tx_buf[ucb->tx_count++];
            }

            uart->IER |= (1 << UART_IER_THREIE_Pos);
        }
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_UART_Receive(enum uart_id id, uint8_t *in, uint32_t len,
                         enum uart_io_type io_type)
{
    struct uart_ctrl_block *ucb;
    uint32_t tmp;
    UART_Type *uart;

    DRIVER_ASSERT(id < UART_ID_MAX);
    DRIVER_ASSERT(len > 0);

    ucb = &g_ucb[id];
    uart = UART_REG(id);

    if (ucb->rx_busy == 1)
    {
        return DRIVER_ERROR_BUSY;
    }

    ucb->rx_busy = 1;

    ucb->rx_buf = (uint8_t *)in;
    ucb->rx_len = len;
    ucb->rx_count = 0;

    if (io_type == UART_IO_TYPE_BLOCK)
    {
        while (ucb->rx_len > ucb->rx_count)
        {
            uint32_t status = 0;

            while (!(uart->LSR & UART_LSR_DR_Msk))
            {
            }

            status |= uart->LSR & UART_LSR_FE_Msk;
            status |= uart->LSR & UART_LSR_PE_Msk;
            status |= uart->LSR & UART_LSR_OE_Msk;

            if (status)
            {
                ucb->rx_busy = 0;
                return DRIVER_ERROR_HW;
            }
            tmp = uart->RBR;
            ucb->rx_buf[ucb->rx_count++] = tmp;
        }

        ucb->tx_busy = 0;
    }
    else
    {
        if (ucb->rx_dma_enable)
        {
            uart->IER |= (1 << UART_IER_DRXIEN_Pos);

            HAL_DMA_Start(ucb->rx_dma_ch, (uint32_t)&uart->RBR, (uint32_t)ucb->rx_buf, ucb->rx_len);
        }
        else
        {
            uart->IER |= (1 << UART_IER_DRIE_Pos);
        }
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_UART_Abort(enum uart_id id)
{
    struct uart_ctrl_block *ucb;
    UART_Type *uart;

    DRIVER_ASSERT(id < UART_ID_MAX);

    ucb = &g_ucb[id];
    uart = UART_REG(id);

    uart->LCR |= (1 << UART_LCR_BREAK_Pos);
    uart->LCR &= ~(1 << UART_LCR_BREAK_Pos);

    ucb->tx_busy = 0;
    ucb->rx_busy = 0;

    return DRIVER_ERROR_OK;
}

int32_t HAL_UART_SetTransmitDMA(enum uart_id id, enum dma_ch ch, uint8_t enable)
{
    struct uart_ctrl_block *ucb;

    DRIVER_ASSERT(id < UART_ID_MAX);

    ucb = &g_ucb[id];

    ucb->tx_dma_enable = enable;
    ucb->tx_dma_ch = ch;

    if (enable)
    {
        struct dma_cfg cfg =
        {
            .peri = UART_TX_DMA_PERI(id),
            .size = DMA_BUS_SIZE_BYTE,
            .dir = DMA_DIR_MEM_TO_PERI,
        };

        HAL_DMA_Init();
        HAL_DMA_Setup(ch, &cfg);
    }
    else
    {
        HAL_DMA_Clear(ucb->rx_dma_ch);
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_UART_SetReceiveDMA(enum uart_id id, enum dma_ch ch, uint8_t enable)
{
    struct uart_ctrl_block *ucb;

    DRIVER_ASSERT(id < UART_ID_MAX);

    ucb = &g_ucb[id];

    ucb->rx_dma_enable = enable;
    ucb->rx_dma_ch = ch;

    if (enable)
    {
        struct dma_cfg cfg =
        {
            .peri = UART_RX_DMA_PERI(id),
            .size = DMA_BUS_SIZE_BYTE,
            .dir = DMA_DIR_PERI_TO_MEM,
        };

        HAL_DMA_Init();
        HAL_DMA_Setup(ch, &cfg);
    }
    else
    {
        HAL_DMA_Clear(ucb->rx_dma_ch);
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_UART_GetBusyStatus(enum uart_id id, uint8_t *busy)
{
    UART_Type *uart;

    DRIVER_ASSERT(id < UART_ID_MAX);

    uart = UART_REG(id);

    if (uart->LSR & UART_LSR_TEMT_Msk)
    {
        *busy = 0;
    }
    else
    {
        *busy = 1;
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_UART_GetLineStatus(enum uart_id id, uint8_t *status)
{
    UART_Type *uart;

    DRIVER_ASSERT(id < UART_ID_MAX);

    uart = UART_REG(id);

    *status = (uart->LSR & 0x7F);

    return DRIVER_ERROR_OK;
}

int32_t HAL_UART_SetIFDelay(enum uart_id id, uint8_t delay)
{
    UART_Type *uart;

    DRIVER_ASSERT(id < UART_ID_MAX);
    DRIVER_ASSERT(delay <= 7);

    uart = UART_REG(id);
    uart->IDTR = delay;

    return DRIVER_ERROR_OK;
}

int32_t HAL_UART_SetIRQ(enum uart_id id, enum uart_irq_type type, uint8_t enable)
{
    uint32_t reg_val = 0;
    UART_Type *uart;

    DRIVER_ASSERT(id < UART_ID_MAX);

    uart = UART_REG(id);

    switch (type)
    {
        case UART_IRQ_RX_AVAIL:
            reg_val = (1 << UART_IER_DRIE_Pos);
            break;
        case UART_IRQ_TX_EMPTY:
            reg_val = (1 << UART_IER_THREIE_Pos);
            break;
        case UART_IRQ_TX_EXIT:
            reg_val = (1 << UART_IER_TXEIE_Pos);
            break;
        case UART_IRQ_LINE_STATUS:
            reg_val = (1 << UART_IER_RLSIE_Pos);
            break;
        case UART_IRQ_RX_DMA_DONE:
            reg_val = (1 << UART_IER_DRXIEN_Pos);
            break;
        case UART_IRQ_TX_DMA_DONE:
            reg_val = (1 << UART_IER_DTXIEN_Pos);
            break;
        default:
            return DRIVER_ERROR_PARAMETER;
    }

    if (enable)
    {
        uart->IER |= reg_val;
    }
    else
    {
        uart->IER &= ~reg_val;
    }

    return DRIVER_ERROR_OK;
}

static void UART_IRQHandler(enum uart_id id)
{
    struct uart_ctrl_block *ucb;
    UART_Type *uart;
    volatile uint32_t int_status;
    volatile uint32_t line_status;
    volatile uint32_t event = 0;
    uint32_t tmp;
     
    ucb = &g_ucb[id];
    uart = UART_REG(id);

    int_status = uart->IIR;

    if ((int_status & UART_IIR_RX_LINE_ERROR) == UART_IIR_RX_LINE_ERROR)
    {
        volatile uint8_t reg_val;

        event |= UART_EVENT_LINE_ERROR;
        line_status = uart->LSR;
        if (line_status & UART_LSR_BI_Msk)
        {
            event |= UART_EVENT_BREAK_ERROR;
        }
        if (line_status & UART_LSR_FE_Msk)
        {
            event |= UART_EVENT_FRAME_ERROR;
        }
        if (line_status & UART_LSR_PE_Msk)
        {
            event |= UART_EVENT_PARITY_ERROR;
        }
        if (line_status & UART_LSR_OE_Msk)
        {
            event |= UART_EVENT_OVERFLOW_ERROR;
        }

        reg_val = uart->RBR;
        uart->IER = 0;
        (void)(reg_val);
    }
    else
    {
        if ((int_status & UART_IIR_RX_DMA_DONE) == UART_IIR_RX_DMA_DONE)
        {
            ucb->rx_busy = 0;
            uart->IER &= ~(1 << UART_IER_DRXIEN_Pos);
            event |= UART_EVENT_RECEIVE_DONE;
        }
        else if (int_status & UART_IIR_RX_DATA_AVAIL)
        {
            tmp = uart->RBR;
            ucb->rx_buf[ucb->rx_count] = tmp;
 
            ucb->rx_count++;
            if (ucb->rx_len == ucb->rx_count)
            {
                uart->IER &= ~(1 << UART_IER_DRIE_Pos);
                ucb->rx_busy = 0;
                event |= UART_EVENT_RECEIVE_DONE;
            }
        }

        if ((int_status & UART_IIR_TX_DMA_DONE) == UART_IIR_TX_DMA_DONE)
        {
            ucb->tx_busy = 0;
            uart->IER &= ~(1 << UART_IER_DTXIEN_Pos);
            event |= UART_EVENT_TRANSMIT_DONE;
        }
        else if (int_status & UART_IIR_TX_HOLD_EMPTY)
        {
            if (ucb->tx_len != ucb->tx_count)
            {
                uart->THR = ucb->tx_buf[ucb->tx_count++];
            }
            else
            {
                ucb->tx_busy = 0;
                uart->IER &= ~(1 << UART_IER_THREIE_Pos);
                event |= UART_EVENT_TRANSMIT_DONE;
            }
        }
    }

    if (ucb->handler && event)
    {
        ucb->handler(event, ucb->context);
    }
}

void UART0_IRQHandler(void)
{
    UART_IRQHandler(UART_ID_0);
}

void UART1_IRQHandler(void)
{
    UART_IRQHandler(UART_ID_1);
}

void UART2_IRQHandler(void)
{
    UART_IRQHandler(UART_ID_2);
}

void UART3_IRQHandler(void)
{
    UART_IRQHandler(UART_ID_3);
}

void UART4_IRQHandler(void)
{
    UART_IRQHandler(UART_ID_4);
}

void UART5_IRQHandler(void)
{
    UART_IRQHandler(UART_ID_5);
}

