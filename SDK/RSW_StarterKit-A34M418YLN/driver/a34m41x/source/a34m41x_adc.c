/**
 *******************************************************************************
 * @file        a34m41x_adc.c
 * @author      ABOV R&D Division
 * @brief       Analog-to-Digital Converter
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
#include "a34m41x_adc.h"
#include "a34m41x_scu.h"

#define ADC_REG_OFFSET  0x100


static __INLINE ADC_Type *ADC_REG(enum adc_id id)
{
    return (ADC_Type *)(ADC0_BASE + (ADC_REG_OFFSET * id));
}

static __INLINE IRQn_Type ADC_IRQ_NUM(enum adc_id id)
{
    return (IRQn_Type)((uint8_t)ADC0_IRQn + id);
}

static __INLINE enum dma_peri ADC_DMA_PERI(enum adc_id id)
{
    return (enum dma_peri)((uint8_t)DMA_PERI_ADC0_RX + id);
}

static __INLINE uint32_t ADC_GET_VAL(ADC_Type *adc, uint8_t seq_id)
{
    return (*(volatile uint32_t *)(&adc->DR0 + seq_id));
}

struct adc_ctrl_block
{
    adc_irq_handler_t   handler;
    void *              context;
    enum adc_mode       mode;
    uint32_t *          buf;
    uint32_t            len;
    uint32_t            count;
    uint8_t             busy;
    uint8_t             seq_cnt;
    uint8_t             trg_sel;
    enum dma_ch         dma_ch;
    uint8_t             dma_enable;
};

static struct adc_ctrl_block g_acb[ADC_ID_MAX];


int32_t HAL_ADC_Init(enum adc_id id, struct adc_cfg *cfg,
                     adc_irq_handler_t handler,
                     void *context)
{
    struct adc_ctrl_block *acb;
    ADC_Type *adc;
    uint32_t reg_val;

    DRIVER_ASSERT(id < ADC_ID_MAX);
    DRIVER_ASSERT(cfg != NULL);

    acb = &g_acb[id];
    acb->handler = handler;
    acb->context = context;

    SCU->PER2 &= ~(1 << (SCU_PER2_ADC0_Pos + id));
    SCU->PCER2 &= ~(1 << (SCU_PCER2_ADC0_Pos + id));

    SCU->PER2 |= (1 << (SCU_PER2_ADC0_Pos + id));
    SCU->PCER2 |= (1 << (SCU_PCER2_ADC0_Pos + id));

    adc = ADC_REG(id);

    acb->mode = cfg->mode;
    acb->seq_cnt = cfg->seq_cnt;

    if (acb->mode == ADC_MODE_SINGLE)
    {
        reg_val = 0
                  | (1 << ADC_MR_DMAEN_Pos)
                  | ((cfg->sampling_time & 0x1f) << ADC_MR_STSEL_Pos)
                  | (((cfg->seq_cnt - 1) & 7) << ADC_MR_SEQCNT_Pos)
                  | (1 << ADC_MR_ADEN_Pos)
                  | (((cfg->restart_en) & 1) << ADC_MR_ARST_Pos)
                  | (0 << ADC_MR_ADMOD_Pos);
    }
    else if (acb->mode == ADC_MODE_SEQUNCE)
    {
        reg_val = 0
                  | (1 << ADC_MR_DMAEN_Pos)
                  | ((cfg->sampling_time & 0x1f) << ADC_MR_STSEL_Pos)
                  | (((cfg->seq_cnt - 1) & 7) << ADC_MR_SEQCNT_Pos)
                  | (1 << ADC_MR_ADEN_Pos)
                  | (((cfg->restart_en) & 1) << ADC_MR_ARST_Pos)
                  | (0 << ADC_MR_ADMOD_Pos);
    }
    else if (acb->mode == ADC_MODE_BURST)
    {
        reg_val = 0
                  | (1 << ADC_MR_DMAEN_Pos)
                  | ((cfg->sampling_time & 0x1f) << ADC_MR_STSEL_Pos)
                  | (((cfg->seq_cnt - 1) & 7) << ADC_MR_SEQCNT_Pos)
                  | (1 << ADC_MR_ADEN_Pos)
                  | (((cfg->restart_en) & 1) << ADC_MR_ARST_Pos)
                  | (1 << ADC_MR_ADMOD_Pos);
    }
    else
    {
        reg_val = 0
                  | (1 << ADC_MR_DMAEN_Pos)
                  | ((cfg->sampling_time & 0x1f) << ADC_MR_STSEL_Pos)
                  | (((cfg->seq_cnt - 1) & 7) << ADC_MR_SEQCNT_Pos)
                  | (1 << ADC_MR_ADEN_Pos)
                  | (((cfg->restart_en) & 1) << ADC_MR_ARST_Pos)
                  | (2 << ADC_MR_ADMOD_Pos);
    }
    adc->MR = reg_val;

    reg_val = 0
    /* CKDIV  1.5Mhz * 15 =22.5Mhz,  PCLK / CLKDIV =48Mhz / 4 =  12Mhz*/
              | ((cfg->in_clk_div & 0x7f) << ADC_CCR_CLKDIV_Pos)
              | (0 << ADC_CCR_ADCPD_Pos)
              | (0 << ADC_CCR_CLKINVT_Pos)
    ;

    adc->CCR = reg_val;
    adc->CSCR = 0;

    /*Set stop mode for ADC*/
    adc->CR = (1UL << ADC_CR_ASTOP_Pos);

    /* default interrupt setting */
    // adc->IER &= ~(1UL<<ADC_IER_EOCIE_Pos);
    // adc->IER |= (1UL<<ADC_IER_EOCIE_Pos);

    NVIC_ClearPendingIRQ(ADC_IRQ_NUM(id));
    NVIC_SetPriority(ADC_IRQ_NUM(id), cfg->irq_prio);
    NVIC_EnableIRQ(ADC_IRQ_NUM(id));

    return DRIVER_ERROR_OK;
}

int32_t HAL_ADC_DeInit(enum adc_id id)
{
    DRIVER_ASSERT(id < ADC_ID_MAX);

    SCU->PER2 &= ~(1 << (SCU_PER2_ADC0_Pos + id));
    SCU->PCER2 &= ~(1 << (SCU_PCER2_ADC0_Pos + id));

    NVIC_DisableIRQ(ADC_IRQ_NUM(id));

    return DRIVER_ERROR_OK;
}

void HAL_ADC_Calibration()
{
	uint32_t timeout;		
	uint32_t var;
	/* ADC configuration */
  struct adc_cfg cfg;
	{                                 
			cfg.mode = ADC_MODE_BURST,        
			cfg.sampling_time = 14, // Tsmpl = STSEL+2 = 24M @ 0.666us          
			cfg.seq_cnt = 1,                  
			cfg.restart_en = 0,               
			cfg.in_clk_div = 5,     // 120M / 5 = 24M          
			cfg.irq_prio = DRIVER_PRIORITY_3;
	}
	
	HAL_ADC_Init(ADC_ID_0, &cfg, NULL, NULL);
	HAL_ADC_Init(ADC_ID_1, &cfg, NULL, NULL);
	HAL_ADC_Init(ADC_ID_2, &cfg, NULL, NULL);
		
	/* ------- Source code -------- */
	for(var=0; var<2; var++)
	{
		ADC0->CCR = 0x0500;	/* ADC control : 1/5 clk, PD off, sample 2 cycle */
		ADC1->CCR = 0x0500;	/* ADC control : 1/5 clk, PD off, sample 2 cycle */
		ADC2->CCR = 0x0500;	/* ADC control : 1/5 clk, PD off, sample 2 cycle */
		ADC0->MR = 0x010080;	/* ADC mode reg : ADC enable, single */
		ADC1->MR = 0x010080;	/* ADC mode reg : ADC enable, single */
		ADC2->MR = 0x010080;	/* ADC mode reg : ADC enable, single */
		ADC0->SR = 0x1;
		ADC1->SR = 0x1;
		ADC2->SR = 0x1;
		ADC0->CR = 0x000001;  /* ADC start */
		ADC1->CR = 0x000001;  /* ADC start */
		ADC2->CR = 0x000001;  /* ADC start */
		
		timeout = ADC_READY_TIMEOUT;			
		while((ADC0->SR & 0x01) == 0)	
		{
			timeout--;
			if(timeout==0) break;	/* do nothing*/
		}
		timeout = ADC_READY_TIMEOUT;	
		while((ADC1->SR & 0x01) == 0)	
		{
			timeout--;
			if(timeout==0) break;	 /* do nothing*/
		}
		timeout = ADC_READY_TIMEOUT;	
		while((ADC2->SR & 0x01) == 0)	
		{
			timeout--;
			if(timeout==0) break;	 /* do nothing*/
		}			
	}
	ADC0->CCR = 0x0080;
	ADC1->CCR = 0x0080;
	ADC2->CCR = 0x0080;
	ADC0->MR  = 0x0000;
	ADC1->MR  = 0x0000;
	ADC2->MR  = 0x0000;
	//-----------------------------------
	HAL_ADC_Stop(ADC_ID_0);
	HAL_ADC_Stop(ADC_ID_1);
	HAL_ADC_Stop(ADC_ID_2);
}

int32_t HAL_ADC_SetMR(enum adc_id id, uint32_t mode)
{
    ADC_Type *adc;

    DRIVER_ASSERT(id < ADC_ID_MAX);

    adc = ADC_REG(id);
    adc->MR = mode;

    return DRIVER_ERROR_OK;
}

int32_t HAL_ADC_GetData(enum adc_id id, uint16_t *data)
{
    ADC_Type *adc;

    DRIVER_ASSERT(id < ADC_ID_MAX);

    adc = ADC_REG(id);
    *data = (adc->DDR >> ADC_DDR_ADDMAR_Pos) & 0x0FFF;

    return DRIVER_ERROR_OK;
}

int32_t HAL_ADC_GetSequenceData(enum adc_id id, enum adc_sequence_id reg, uint16_t *data)
{
    ADC_Type *adc;

    DRIVER_ASSERT(id < ADC_ID_MAX);
    DRIVER_ASSERT(reg < ADC_SEQUENCE_MAX);

    adc = ADC_REG(id);
    switch (reg)
    {
        case ADC_SEQUENCE_ID_0:
            *data = adc->DR0 >> 4;
            break;
        case ADC_SEQUENCE_ID_1:
            *data = adc->DR1 >> 4;
            break;
        case ADC_SEQUENCE_ID_2:
            *data = adc->DR2 >> 4;
            break;
        case ADC_SEQUENCE_ID_3:
            *data = adc->DR3 >> 4;
            break;
        case ADC_SEQUENCE_ID_4:
            *data = adc->DR4 >> 4;
            break;
        case ADC_SEQUENCE_ID_5:
            *data = adc->DR5 >> 4;
            break;
        case ADC_SEQUENCE_ID_6:
            *data = adc->DR6 >> 4;
            break;
        case ADC_SEQUENCE_ID_7:
            *data = adc->DR7 >> 4;
            break;
        default:
            break;
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_ADC_SetDMA(enum adc_id id, enum dma_ch ch, uint8_t enable)
{
    struct adc_ctrl_block *acb;

    DRIVER_ASSERT(id < ADC_ID_MAX);

    acb = &g_acb[id];

    acb->dma_enable = enable;
    acb->dma_ch = ch;

    if (enable)
    {
        struct dma_cfg cfg =
        {
            .peri = ADC_DMA_PERI(id),
            .size = DMA_BUS_SIZE_WORD,
            .dir = DMA_DIR_PERI_TO_MEM,
        };

        HAL_DMA_Init();
        HAL_DMA_Setup(ch, &cfg);
    }
    else
    {
        HAL_DMA_Clear(acb->dma_ch);
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_ADC_StartSingle(enum adc_id id, uint32_t *data, enum adc_io_type io_type)
{
    struct adc_ctrl_block *acb;
    ADC_Type *adc;
		uint32_t timeout;
	
    DRIVER_ASSERT(id < ADC_ID_MAX);

    acb = &g_acb[id];

    if (acb->busy == 1)
    {
        return DRIVER_ERROR_BUSY;
    }

    acb->busy = 1;

    adc = ADC_REG(id);

    if (io_type == ADC_IO_TYPE_BLOCK)
    {
        uint32_t status;

        adc->CR = (1UL << ADC_CR_ASTART_Pos);
				
				timeout = ADC_SR_READY_TIMEOUT;	
        while (1)	
        {
            timeout--;
						status = adc->SR;
            if (status & (1 << ADC_SR_EOCIF_Pos))
            {
                adc->SR = status;
                break;
            }
						if (timeout==0) break;
        }

        *data = adc->DDR;

        acb->busy = 0;
    }
    else
    {
        acb->buf = data;

        if (acb->dma_enable)
        {
            adc->MR |= (1 << ADC_MR_DMAEN_Pos);
            adc->IER |= (1 << ADC_IER_DMAIE_Pos);
            adc->CR = (1 << ADC_CR_ASTART_Pos);
            HAL_DMA_Start(acb->dma_ch, (uint32_t)&adc->DDR, (uint32_t)acb->buf, 1);
        }
        else
        {
            adc->IER |= (1 << ADC_IER_EOCIE_Pos);
            adc->CR = (1 << ADC_CR_ASTART_Pos);
        }
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_ADC_StartBurst(enum adc_id id, uint32_t *data, enum adc_io_type io_type)
{
    struct adc_ctrl_block *acb;
    ADC_Type *adc;
		uint32_t timeout;
	
    DRIVER_ASSERT(id < ADC_ID_MAX);

    acb = &g_acb[id];
    adc = ADC_REG(id);

    if (io_type == ADC_IO_TYPE_BLOCK)
    {
        uint32_t status;
        uint8_t i;

        if (acb->busy == 1)
        {
            return DRIVER_ERROR_BUSY;
        }

        acb->busy = 1;

        if (acb->trg_sel != 0)
        {
            acb->busy = 0;
            return DRIVER_ERROR_INVALID_USE;
        }

        adc->CR = (1UL << ADC_CR_ASTART_Pos);

        timeout = ADC_SR_READY_TIMEOUT;	
        while (1)	
        {
            timeout--;
            status = adc->SR;
            if (status & (1 << ADC_SR_EOSIF_Pos))
            {
                adc->SR = status;
                break;
            }
            if (timeout==0) break;
        }				

        for (i = 0; i < acb->seq_cnt; i++)
        {
            data[i] = ADC_GET_VAL(adc, i);
        }

        acb->busy = 0;
    }
    else
    {
        acb->buf = data;

        if (acb->dma_enable)
        {
            adc->MR |= (1 << ADC_MR_DMAEN_Pos);
            adc->IER |= (1 << ADC_IER_DMAIE_Pos);
            HAL_DMA_Start(acb->dma_ch, (uint32_t)&adc->DDR, (uint32_t)acb->buf, acb->seq_cnt);
            if (acb->trg_sel == 0)
            {
                adc->CR = (1 << ADC_CR_ASTART_Pos);
            }

        }
        else
        {
            adc->IER |= (1 << ADC_IER_EOSIE_Pos) | (1 << ADC_IER_EOCIE_Pos);
            if (acb->trg_sel == 0)
            {
                adc->CR = (1 << ADC_CR_ASTART_Pos);
            }
        }
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_ADC_StartSequence(enum adc_id id, uint32_t *data, enum adc_io_type io_type)
{
    struct adc_ctrl_block *acb;
    ADC_Type *adc;
		uint32_t timeout;
	
    DRIVER_ASSERT(id < ADC_ID_MAX);

    acb = &g_acb[id];
    adc = ADC_REG(id);

    if (io_type == ADC_IO_TYPE_BLOCK)
    {
        uint32_t status;
        uint8_t i;

        if (acb->busy == 1)
        {
            return DRIVER_ERROR_BUSY;
        }

        acb->busy = 1;

        if (acb->trg_sel != 0)
        {
            acb->busy = 0;
            return DRIVER_ERROR_INVALID_USE;
        }

        adc->CR = (1UL << ADC_CR_ASTART_Pos);

        timeout = ADC_SR_READY_TIMEOUT;	
        while (1)	
        {
            timeout--;
            status = adc->SR;
            if (status & (1 << ADC_SR_EOSIF_Pos))
            {
                adc->SR = status;
                break;
            }
        if (timeout==0) break;
        }		

        for (i = 0; i < acb->seq_cnt; i++)
        {
            data[i] = ADC_GET_VAL(adc, i);
        }

        acb->busy = 0;
    }
    else
    {
        acb->buf = data;

        if (acb->dma_enable)
        {
            adc->MR |= (1 << ADC_MR_DMAEN_Pos);
            adc->IER |= (1 << ADC_IER_DMAIE_Pos);
            HAL_DMA_Start(acb->dma_ch, (uint32_t)&adc->DDR, (uint32_t)acb->buf, acb->seq_cnt);
            if (acb->trg_sel == 0)
            {
                adc->CR = (1 << ADC_CR_ASTART_Pos);
            }

        }
        else
        {
            adc->IER |= (1 << ADC_IER_EOSIE_Pos) | (1 << ADC_IER_EOCIE_Pos);
            if (acb->trg_sel == 0)
            {
                adc->CR = (1 << ADC_CR_ASTART_Pos);
            }
        }
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_ADC_StartMultiple(enum adc_id id, uint32_t *data, enum adc_io_type io_type)
{
    struct adc_ctrl_block *acb;
    ADC_Type *adc;
    uint32_t timeout;
	
    DRIVER_ASSERT(id < ADC_ID_MAX);

    acb = &g_acb[id];
    adc = ADC_REG(id);

    if (io_type == ADC_IO_TYPE_BLOCK)
    {
        uint32_t status;
        uint8_t i;

        if (acb->busy == 1)
        {
            return DRIVER_ERROR_BUSY;
        }

        acb->busy = 1;

        if (acb->trg_sel != 0)
        {
            acb->busy = 0;
            return DRIVER_ERROR_INVALID_USE;
        }

        adc->CR = (1UL << ADC_CR_ASTART_Pos);

        timeout = ADC_SR_READY_TIMEOUT;	
        while (1)	
        {
            timeout--;
            status = adc->SR;
            if (status & (1 << ADC_SR_EOSIF_Pos))
            {
                adc->SR = status;
                break;
            }
            if (timeout==0) break;
        }		

        for (i = 0; i < acb->seq_cnt; i++)
        {
            data[i] = ADC_GET_VAL(adc, i);
        }

        acb->busy = 0;
    }
    else
    {
        acb->buf = data;

        adc->IER |= (1 << ADC_IER_EOSIE_Pos) | (1 << ADC_IER_EOCIE_Pos);
        if (acb->trg_sel == 0)
        {
            adc->CR = (1 << ADC_CR_ASTART_Pos);
        }
    }

    return DRIVER_ERROR_OK;
}


int32_t HAL_ADC_Start(enum adc_id id)
{
    ADC_Type *adc;

    DRIVER_ASSERT(id < ADC_ID_MAX);

    adc = ADC_REG(id);
    adc->CR = ((1UL << ADC_CR_ASTART_Pos));

    return DRIVER_ERROR_OK;;
}


int32_t HAL_ADC_Stop(enum adc_id id)
{
    struct adc_ctrl_block *acb;
    ADC_Type *adc;

    DRIVER_ASSERT(id < ADC_ID_MAX);

    acb = &g_acb[id];
    adc = ADC_REG(id);
    adc->CR = ((1UL << ADC_CR_ASTOP_Pos));
    acb->busy = 0;

    return DRIVER_ERROR_OK;
}

int32_t HAL_ADC_EnterPowerdownMode(enum adc_id id, uint8_t enable)
{
    ADC_Type *adc;

    DRIVER_ASSERT(id < ADC_ID_MAX);

    adc = ADC_REG(id);
    adc->CCR &= ~(1UL << ADC_CCR_ADCPD_Pos);
    if (enable)
    {
        adc->CCR |= (1UL << ADC_CCR_ADCPD_Pos);
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_ADC_SelectChannel(enum adc_id id, enum adc_sequence_id seq_id, uint32_t channel)
{
    ADC_Type *adc;

    DRIVER_ASSERT(id < ADC_ID_MAX);

    adc = ADC_REG(id);

    if (seq_id < ADC_SEQUENCE_ID_4)
    {
        adc->SCSR1 &= ~(0x1FUL << (seq_id * 8));
        adc->SCSR1 |= channel << (seq_id * 8);
    }
    else
    {
        adc->SCSR2 &= ~(0x1FUL << ((seq_id - ADC_SEQUENCE_ID_4) * 8));
        adc->SCSR2 |= channel << ((seq_id - ADC_SEQUENCE_ID_4) * 8);
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_ADC_SelectChannel1(enum adc_id id, uint32_t channel)
{
    ADC_Type *adc;

    DRIVER_ASSERT(id < ADC_ID_MAX);

    adc = ADC_REG(id);
    adc->SCSR1 = channel;

    return DRIVER_ERROR_OK;
}

int32_t HAL_ADC_SelectChannel2(enum adc_id id, uint32_t channel)
{
    ADC_Type *adc;

    DRIVER_ASSERT(id < ADC_ID_MAX);

    adc = ADC_REG(id);
    adc->SCSR2 = channel;

    return DRIVER_ERROR_OK;
}

int32_t HAL_ADC_SelectTriggerTimer(enum adc_id id, enum adc_trigger_sequence_id trg_seq_id,
                                   enum adc_trigger_timer_id trg_tm_id)
{
    struct adc_ctrl_block *acb;
    ADC_Type *adc;

    DRIVER_ASSERT(id < ADC_ID_MAX);

    acb = &g_acb[id];
    adc = ADC_REG(id);

    adc->TRG &= ~(0x0F << (trg_seq_id * 4));
    adc->TRG |= (trg_tm_id << (trg_seq_id * 4));

    adc->MR &= ~ADC_MR_TRGSEL_Msk;
    adc->MR |= 1 << ADC_MR_TRGSEL_Pos;
    acb->trg_sel = 1;

    return DRIVER_ERROR_OK;
}

int32_t HAL_ADC_SelectTriggerMPWM(enum adc_id id, enum adc_trigger_sequence_id trg_seq_id,
                                  enum adc_trigger_mpwm_type trg_mpwm_type,
                                  enum adc_trigger_mpwm_id trg_mwpm_id)
{
    struct adc_ctrl_block *acb;
    ADC_Type *adc;

    DRIVER_ASSERT(id < ADC_ID_MAX);

    acb = &g_acb[id];
    adc = ADC_REG(id);

    adc->TRG &= ~(0x0F << (trg_seq_id * 4));
    adc->TRG |= (trg_mwpm_id << (trg_seq_id * 4));

    adc->MR &= ~ADC_MR_TRGSEL_Msk;
    if (trg_mpwm_type == ADC_TRIGGER_MPWM_TYPE_MPWM0)
    {
        adc->MR |= 2 << ADC_MR_TRGSEL_Pos;
        acb->trg_sel = 2;
    }
    else
    {
        adc->MR |= 3 << ADC_MR_TRGSEL_Pos;
        acb->trg_sel = 3;
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_ADC_GetStatus(enum adc_id id, uint32_t *status)
{
    ADC_Type *adc;

    DRIVER_ASSERT(id < ADC_ID_MAX);

    adc = ADC_REG(id);
    *status = adc->SR;

    return DRIVER_ERROR_OK;
}

int32_t HAL_ADC_ClearStatus(enum adc_id id, uint32_t status)
{
    ADC_Type *adc;

    DRIVER_ASSERT(id < ADC_ID_MAX);

    adc = ADC_REG(id);
    adc->SR = status;

    return DRIVER_ERROR_OK;
}

int32_t HAL_ADC_SetIRQ(enum adc_id id, enum adc_irq_type type, uint8_t enable)
{
    uint32_t reg_val = 0;
    ADC_Type *adc;

    DRIVER_ASSERT(id < ADC_ID_MAX);

    adc = ADC_REG(id);

    switch (type)
    {
        case ADC_IRQ_SINGLE:
            reg_val |= (1 << ADC_IER_EOCIE_Pos);
            break;
        case ADC_IRQ_SEQUENCE:
            reg_val |= (1 << ADC_IER_EOSIE_Pos);
            break;
        case ADC_IRQ_TRIGGER:
            reg_val |= (1 << ADC_IER_TRGIE_Pos);
            break;
        case ADC_IRQ_DMA:
            reg_val |= (1 << ADC_IER_DMAIE_Pos);
            break;
    }

    if (enable)
    {
        adc->IER |= reg_val;
    }
    else
    {
        adc->IER &= ~reg_val;
    }

    return DRIVER_ERROR_OK;
}

void ADC_IRQHandler(enum adc_id id)
{
    struct adc_ctrl_block *acb;
    ADC_Type *adc;
    uint32_t status;
    uint32_t event = 0;

    acb = &g_acb[id];
    adc = ADC_REG(id);

    if (acb->mode == ADC_MODE_SINGLE)
    {
        status = adc->SR;
        adc->SR = status;

        if ((status & ADC_SR_EOCIF_Msk) && acb->dma_enable == 0)
        {
            *acb->buf = adc->DDR;
            event |= ADC_EVENT_SINGLE;
        }
        else if ((status & ADC_SR_DMAF_Msk) && acb->dma_enable)
        {
            event |= ADC_EVENT_DMA;
        }

        acb->busy = 0;
    }
    else if (acb->mode == ADC_MODE_BURST)
    {
        status = adc->SR;
        adc->SR = status;

        if (status & ADC_SR_TRGIF_Msk)
        {
            event |= ADC_EVENT_TRIGGER;
        }

        if ((status & ADC_SR_EOSIF_Msk) && acb->dma_enable == 0)
        {
            uint8_t i;

            for (i = 0; i < acb->seq_cnt; i++)
            {
                acb->buf[i] = ADC_GET_VAL(adc, i);
            }
            event |= ADC_EVENT_SEQUENCE;
        }
        else if ((status & ADC_SR_DMAF_Msk) && acb->dma_enable)
        {
            event |= ADC_EVENT_DMA;
            if (acb->trg_sel)
            {
                HAL_DMA_Start(acb->dma_ch, (uint32_t)&adc->DDR, (uint32_t)acb->buf, acb->seq_cnt);
            }
        }
    }
    else if (acb->mode == ADC_MODE_SEQUNCE)
    {
        status = adc->SR;
        adc->SR = status;

        if (status & ADC_SR_TRGIF_Msk)
        {
            event |= ADC_EVENT_TRIGGER;
        }

        if ((status & ADC_SR_EOSIF_Msk) && acb->dma_enable == 0)
        {
            uint8_t i;

            for (i = 0; i < acb->seq_cnt; i++)
            {
                acb->buf[i] = ADC_GET_VAL(adc, i);
            }
            event |= ADC_EVENT_SEQUENCE;
        }
        else if ((status & ADC_SR_DMAF_Msk) && acb->dma_enable)
        {
            event |= ADC_EVENT_DMA;
            if (acb->trg_sel)
            {
                HAL_DMA_Start(acb->dma_ch, (uint32_t)&adc->DDR, (uint32_t)acb->buf, acb->seq_cnt);
            }
        }
    }
    else if (acb->mode == ADC_MODE_MULTI)
    {
        status = adc->SR;
        adc->SR = status;

        if (status & ADC_SR_TRGIF_Msk)
        {
            event |= ADC_EVENT_TRIGGER;
        }

        if ((status & ADC_SR_EOCIF_Msk))
        {
            uint8_t i;

            for (i = 0; i < acb->seq_cnt; i++)
            {
                acb->buf[i] = ADC_GET_VAL(adc, i);
            }
            event |= ADC_EVENT_SEQUENCE;
        }
    }
    else
    {
        /* TODO */
        status = adc->SR;
        adc->SR = status;

    }

    if (event && acb->handler)
    {
        acb->handler(event, acb->context);
    }
}

void ADC0_IRQHandler(void)
{
    ADC_IRQHandler(ADC_ID_0);
}

void ADC1_IRQHandler(void)
{
    ADC_IRQHandler(ADC_ID_1);
}

void ADC2_IRQHandler(void)
{
    ADC_IRQHandler(ADC_ID_2);
}

