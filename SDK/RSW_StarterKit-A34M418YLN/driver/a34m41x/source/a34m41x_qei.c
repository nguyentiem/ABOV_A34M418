/**
 *******************************************************************************
 * @file        a34m41x_qei.c
 * @author      ABOV R&D Division
 * @brief       Quadrature Encoder Interface
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
#include "a34m41x_qei.h"


#define QEI_REG_OFFSET      0x100



static __INLINE QEI_Type *QEI_REG(enum qei_id id)
{
    return (QEI_Type *)(QEI0_BASE + (QEI_REG_OFFSET * id));
}

static __INLINE IRQn_Type QEI_IRQ_NUM(enum qei_id id)
{
    return (IRQn_Type)((uint8_t)QEI0_IRQn + id);
}

struct qei_ctrl_block
{
    qei_irq_handler_t   handler;
    void *              context;
    int32_t             count;
};

static struct qei_ctrl_block g_qcb[QEI_ID_MAX];


int32_t HAL_QEI_Init(enum qei_id id, struct qei_cfg *cfg,
                     qei_irq_handler_t handler,
                     void *context)
{
    struct qei_ctrl_block *qcb;
    QEI_Type *qei;

    DRIVER_ASSERT(id < QEI_ID_MAX);
    DRIVER_ASSERT(cfg != NULL);

    qcb = &g_qcb[id];
    qei = QEI_REG(id);

    memset(qcb, 0, sizeof(struct qei_ctrl_block));

    SCU->PER1 |= (1 << SCU_PER1_QEI0_Pos);
    SCU->PCER1 |= (1 << SCU_PCER1_QEI0_Pos);

    qcb->handler = handler;
    qcb->context = context;

    qei->MR = 0;
    qei->IER = 0;

    qei->CON = (1 << QEI_CON_RESP_Pos);
    if (cfg->enable_position)
    {
        qei->MR |= (1 << QEI_MR_DIRPC_Pos) | (1 << QEI_MR_DIRI_Pos);
        qei->IER |= (1 << QEI_IER_ENCLKEN_Pos) | (1 << QEI_IER_ERREN_Pos) | (1 << QEI_IER_DIREN_Pos);
    }
    if (cfg->enable_velocity)
    {
        qei->IER |= (1 << QEI_IER_VELCEN_Pos) | (1 << QEI_IER_VELTEN_Pos);
        qei->MR |= (1 << QEI_MR_QDVEL_Pos);
    }

    NVIC_ClearPendingIRQ(QEI_IRQ_NUM(id));
    NVIC_SetPriority(QEI_IRQ_NUM(id), cfg->irq_prio);
    NVIC_EnableIRQ(QEI_IRQ_NUM(id));

    return DRIVER_ERROR_OK;
}

int32_t HAL_QEI_Deinit(enum qei_id id)
{
    DRIVER_ASSERT(id < QEI_ID_MAX);

    SCU->PER1 &= ~(1 << SCU_PER1_QEI0_Pos);
    SCU->PCER1 &= ~(1 << SCU_PCER1_QEI0_Pos);

    NVIC_DisableIRQ(QEI_IRQ_NUM(id));

    return DRIVER_ERROR_OK;
}

int32_t HAL_QEI_Start(enum qei_id id)
{
    QEI_Type *qei;

    DRIVER_ASSERT(id < QEI_ID_MAX);

    qei = QEI_REG(id);

    qei->POS = 0;
    qei->MR |= (1 << QEI_MR_QDMOD_Pos);

    return DRIVER_ERROR_OK;
}

int32_t HAL_QEI_Stop(enum qei_id id)
{
    QEI_Type *qei;

    DRIVER_ASSERT(id < QEI_ID_MAX);

    qei = QEI_REG(id);

    qei->CON = (1 << QEI_CON_RESP_Pos) | (1 << QEI_CON_RESI_Pos) | (1 << QEI_CON_RESV_Pos) | (1 << QEI_CON_INVI_Pos);
    qei->POS = 0;
    qei->MR &= ~(1 << QEI_MR_QDMOD_Pos);

    return DRIVER_ERROR_OK;
}

int32_t HAL_QEI_Read(enum qei_id id, int32_t *count)
{
    struct qei_ctrl_block *qcb;

    DRIVER_ASSERT(id < QEI_ID_MAX);

    qcb = &g_qcb[id];

    *count = qcb->count;

    return DRIVER_ERROR_OK;
}

int32_t HAL_QEI_ClearCounter(enum qei_id id, enum qei_counter_type type)
{
    QEI_Type *qei;

    DRIVER_ASSERT(id < QEI_ID_MAX);

    qei = QEI_REG(id);

    if (type == QEI_COUNTER_POSITION)
    {
        qei->CON |= (1 << QEI_CON_RESP_Pos);
    }
    else if (type == QEI_COUNTER_INDEX)
    {
        qei->CON |= (1 << QEI_CON_RESI_Pos);
    }
    else if (type == QEI_COUNTER_VELOCITY)
    {
        qei->CON |= (1 << QEI_CON_RESV_Pos);
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_QEI_EnablePositionCompare(enum qei_id id, enum qei_pos_comp comp, uint32_t value)
{
    QEI_Type *qei;

    DRIVER_ASSERT(id < QEI_ID_MAX);

    qei = QEI_REG(id);

    if (comp == QEI_POS_COMP_0)
    {
        qei->CMP0 = value;
        qei->IER |= (1 << QEI_IER_POS0EN_Pos);
    }
    else if (comp == QEI_POS_COMP_1)
    {
        qei->CMP1 = value;
        qei->IER |= (1 << QEI_IER_POS1EN_Pos);
    }
    else if (comp == QEI_POS_COMP_2)
    {
        qei->CMP2 = value;
        qei->IER |= (1 << QEI_IER_POS2EN_Pos);
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_QEI_DisablePositionCompare(enum qei_id id, enum qei_pos_comp comp)
{
    QEI_Type *qei;

    DRIVER_ASSERT(id < QEI_ID_MAX);

    qei = QEI_REG(id);

    if (comp == QEI_POS_COMP_0)
    {
        qei->IER &= ~(1 << QEI_IER_POS0EN_Pos);
    }
    else if (comp == QEI_POS_COMP_1)
    {
        qei->IER &= ~(1 << QEI_IER_POS1EN_Pos);
    }
    else if (comp == QEI_POS_COMP_2)
    {
        qei->IER &= ~(1 << QEI_IER_POS2EN_Pos);
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_QEI_SetIRQ(enum qei_id id, enum qei_irq_type type, uint8_t enable)
{
    uint32_t reg_val = 0;
    QEI_Type *qei;

    DRIVER_ASSERT(id < QEI_ID_MAX);

    qei = QEI_REG(id);

    switch (type)
    {
        case QEI_IRQ_ERROR:
            reg_val = (1 << QEI_IER_ERREN_Pos);
            break;
        case QEI_IRQ_POSITION:
            reg_val = (1 << QEI_IER_ENCLKEN_Pos);
            break;
        case QEI_IRQ_DIRECTION:
            reg_val = (1 << QEI_IER_DIREN_Pos);
            break;
        default:
            return DRIVER_ERROR_PARAMETER;
    }

    if (enable)
    {
        qei->IER |= reg_val;
    }
    else
    {
        qei->IER &= ~reg_val;
    }

    return DRIVER_ERROR_OK;
}

void QEI_IRQHandler(enum qei_id id)
{
    struct qei_ctrl_block *qcb;
    QEI_Type *qei;
    uint32_t status;
    uint32_t event = 0;

    qcb = &g_qcb[id];
    qei = QEI_REG(id);

    status = qei->ISR;
    qei->ISCR = status;

    if (status & (1 << QEI_ISR_ERR_Pos))
    {
        event |= QEI_EVENT_ERROR;
    }

    if (status & (1 << QEI_ISR_DIR_Pos))
    {
        event |= QEI_EVENT_DIRECTION;
    }

    if (status & (1 << QEI_ISR_ENCLK_Pos))
    {
        if (qei->SR & (1 << QEI_SR_Direction_Pos))
        {
            event |= QEI_EVENT_FORWARD;
        }
        else
        {
            event |= QEI_EVENT_BACKWARD;
        }

        qcb->count = qei->POS;
    }

    if (status & (1 << QEI_ISR_POS0_Pos))
    {
        event |= QEI_EVENT_POS0;
    }
    if (status & (1 << QEI_ISR_POS1_Pos))
    {
        event |= QEI_EVENT_POS1;
    }
    if (status & (1 << QEI_ISR_POS2_Pos))
    {
        event |= QEI_EVENT_POS2;
    }

    qcb->handler(event, qcb->context);
}

void QEI0_IRQHandler(void)
{
    QEI_IRQHandler(QEI_ID_0);
}

void QEI1_IRQHandler(void)
{
    QEI_IRQHandler(QEI_ID_1);
}

