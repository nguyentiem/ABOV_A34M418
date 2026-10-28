/**
 *******************************************************************************
 * @file        a34m41x_flash.c
 * @author      ABOV R&D Division
 * @brief       Flash
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
#include "a34m41x_flash.h"

#define FLASH_SIZE_128K             0x00020000
#define FLASH_SIZE_256K             0x00040000
#define FLASH_SIZE_512K             0x00080000

#define FLASH_DATA_ADRESS_Msk       0x0E000000
#define FLASH_DATA_SIZE             0x00008000

#define FLASH_OPT_ADDRESS_Msk       0x0F000000
#define FLASH_OPT_SIZE              0x00000600

#define FLASH_TYPE_CODE             0
#define FLASH_TYPE_DATA             1

#define FLASH_ACCESS_KEY1           0x01234567UL
#define FLASH_ACCESS_KEY2           0x12345678UL
#define FLASH_ACCESS_KEY3           0x23456789UL

#define FLASH_OTP_ACCESS_KEY1       0x3456789AUL
#define FLASH_OTP_ACCESS_KEY2       0x456789ABUL
#define FLASH_OTP_ACCESS_KEY3       0x56789ABCUL

#define FLASH_CODE_ERASE_CTRL_Msk    (CFMC_CTRL_PERS_Msk |   \
                                      CFMC_CTRL_S1KERS_Msk | \
                                      CFMC_CTRL_S4KERS_Msk)

#define FLASH_DATA_ERASE_CTRL_Msk    (DFMC_CTRL_PERS_Msk |   \
                                      DFMC_CTRL_S1KERS_Msk | \
                                      DFMC_CTRL_S4KERS_Msk)

#define FLASH_CODE_ERASE_CTRL_Msk    (CFMC_CTRL_PERS_Msk |   \
                                      CFMC_CTRL_S1KERS_Msk | \
                                      CFMC_CTRL_S4KERS_Msk)

#define FLASH_DATA_ERASE_CTRL_Msk    (DFMC_CTRL_PERS_Msk |   \
                                      DFMC_CTRL_S1KERS_Msk | \
                                      DFMC_CTRL_S4KERS_Msk)

#define flash_code_busy_check(void)              \
    {                                            \
        uint32_t timeout;			\
				timeout = FLASH_BUSY_BLOCKING_TIMEOUT;		\
				while (CFMC->STAT & CFMC_STAT_WBUSY_Msk) \
				{		\
						timeout--;		\
						if(timeout==0) break;		\
				}	\
    }

#define flash_data_busy_check(void)              \
    {                                            \
        uint32_t timeout;			\
				timeout = FLASH_BUSY_BLOCKING_TIMEOUT;		\
				while (DFMC->STAT & DFMC_STAT_WBUSY_Msk) \
				{		\
						timeout--;		\
						if(timeout==0) break;		\
				}                                    \
    }

#define VIO(addr)   (*(volatile uint32_t *)(addr))

uint32_t g_code_flash_size = FLASH_SIZE_512K;
enum flash_sector_size g_erase_code_sector_size = FLASH_SECTOR_SIZE_512B;
enum flash_sector_size g_erase_data_sector_size = FLASH_SECTOR_SIZE_512B;

static void FLASH_CodeWrite(uint32_t flash_addr, uint8_t *data, uint32_t len)
{
    uint32_t *data32;
    uint32_t i;

  	/* All interrupts must be stopped when flash erase or write */
  	__disable_irq();
	
    data32 = (uint32_t *)data;

    /* Data/Instruction Cache Disable */
    CFMC->CONF &= ~(CFMC_CONF_DCEN_Msk | CFMC_CONF_ICEN_Msk);

    /* Data/Instruction Cache Reset */
    CFMC->CONF |= (1 << CFMC_CONF_DCRST_Pos | 1 << CFMC_CONF_ICRST_Pos);

    /* Data/Instruction Cache Reset  clear */
    CFMC->CONF &= ~(1 << CFMC_CONF_DCRST_Pos | 1 << CFMC_CONF_ICRST_Pos);	
 
		CFMC->FLSKEY = FLASH_ACCESS_KEY1;
    CFMC->FLSKEY = FLASH_ACCESS_KEY2;
    CFMC->FLSKEY = FLASH_ACCESS_KEY3;
 
    flash_code_busy_check();

    CFMC->CTRL |= 1 << CFMC_CTRL_PGM_Pos;

    for (i = 0; i < len / 4; i++)
    {
        VIO(flash_addr + (i * 4)) = *data32++;

        flash_code_busy_check();
    }

    CFMC->CTRL &= ~(1 << CFMC_CTRL_PGM_Pos);

    /* Data/Instruction Cache Enable */
    CFMC->CONF |= (1 << CFMC_CONF_DCEN_Pos | 1 << CFMC_CONF_ICEN_Pos);
	__enable_irq();
}

static void FLASH_CodeErase(uint32_t flash_addr, enum flash_sector_size sector_size)
{
    uint32_t reg_val;
    uint32_t sector_type = 1 << CFMC_CTRL_PERS_Pos;

		/* All interrupts must be stopped when flash erase or write */
		__disable_irq();
	
    CFMC->FLSKEY = FLASH_ACCESS_KEY1;
    CFMC->FLSKEY = FLASH_ACCESS_KEY2;
    CFMC->FLSKEY = FLASH_ACCESS_KEY3;

    /* Data/Instruction Cache Disable */
    CFMC->CONF &= ~(CFMC_CONF_DCEN_Msk | CFMC_CONF_ICEN_Msk);

    /* Data/Instruction Cache Reset */
    CFMC->CONF |= (1 << CFMC_CONF_DCRST_Pos | 1 << CFMC_CONF_ICRST_Pos);

    /* Data/Instruction Cache Reset  clear */
    CFMC->CONF &= ~(1 << CFMC_CONF_DCRST_Pos | 1 << CFMC_CONF_ICRST_Pos);	

    flash_code_busy_check();

    switch (sector_size)
    {
        case FLASH_SECTOR_SIZE_512B:
            sector_type = 1 << CFMC_CTRL_PERS_Pos;
            break;
        case FLASH_SECTOR_SIZE_1KB:
            sector_type = 1 << CFMC_CTRL_S1KERS_Pos;
            break;
        case FLASH_SECTOR_SIZE_4KB:
            sector_type = 1 << CFMC_CTRL_S4KERS_Pos;
            break;
        default:
            DRIVER_ASSERT(0);
            break;
    }

    reg_val = CFMC->CTRL;
    reg_val &= ~FLASH_CODE_ERASE_CTRL_Msk;
    reg_val |= sector_type;

    CFMC->CTRL = reg_val;
    VIO(flash_addr) = 0;

    flash_code_busy_check();

    CFMC->CTRL &= ~(sector_type);

    /* Data/Instruction Cache Enable */
    CFMC->CONF |= (1 << CFMC_CONF_DCEN_Pos | 1 << CFMC_CONF_ICEN_Pos);
	__enable_irq();
}

static void FLASH_DataWrite(uint32_t flash_addr, uint8_t *data, uint32_t len)
{
    uint32_t *data32;
    uint32_t i;

		/* All interrupts must be stopped when flash erase or write */
		__disable_irq();	
	
    data32 = (uint32_t *)data;

    DFMC->FLSKEY = FLASH_ACCESS_KEY1;
    DFMC->FLSKEY = FLASH_ACCESS_KEY2;
    DFMC->FLSKEY = FLASH_ACCESS_KEY3;

    flash_data_busy_check();

    DFMC->CTRL |= 1 << DFMC_CTRL_PGM_Pos;

    for (i = 0; i < len / 4; i++)
    {
        VIO(flash_addr + (i * 4)) = *data32++;

        flash_data_busy_check();
    }

    DFMC->CTRL &= ~(1 << DFMC_CTRL_PGM_Pos);
	__enable_irq();
}

static void FLASH_DataErase(uint32_t flash_addr, enum flash_sector_size sector_size)
{
    uint32_t reg_val;
    uint32_t sector_type = 1 << DFMC_CTRL_PERS_Pos;

		/* All interrupts must be stopped when flash erase or write */
		__disable_irq();
	
    DFMC->FLSKEY = FLASH_ACCESS_KEY1;
    DFMC->FLSKEY = FLASH_ACCESS_KEY2;
    DFMC->FLSKEY = FLASH_ACCESS_KEY3;

    flash_data_busy_check();

    switch (sector_size)
    {
        case FLASH_SECTOR_SIZE_512B:
            sector_type = 1 << DFMC_CTRL_PERS_Pos;
            break;
        case FLASH_SECTOR_SIZE_1KB:
            sector_type = 1 << DFMC_CTRL_S1KERS_Pos;
            break;
        case FLASH_SECTOR_SIZE_4KB:
            sector_type = 1 << DFMC_CTRL_S4KERS_Pos;
            break;
        default:
            DRIVER_ASSERT(0);
            break;
    }

    reg_val = DFMC->CTRL;
    reg_val &= ~FLASH_DATA_ERASE_CTRL_Msk;
    reg_val |= sector_type;

    DFMC->CTRL = reg_val;
    VIO(flash_addr) = 0;

    flash_data_busy_check();

    DFMC->CTRL &= ~(sector_type);
	__enable_irq();
}

static int32_t FLASH_GetStatus(uint8_t type)
{
    int32_t ret = DRIVER_ERROR_OK;
    uint32_t stat;

    if (type == FLASH_TYPE_DATA)
    {
        stat = DFMC->STAT;
        DFMC->STAT = stat;
    }
    else
    {
        stat = CFMC->STAT;
        CFMC->STAT = stat;
    }

    if (stat & CFMC_STAT_FLERR_Msk)
    {
        ret = DRIVER_ERROR_INVALID_USE;
    }

    if (stat & CFMC_STAT_OLERR_Msk)
    {
        ret = DRIVER_ERROR_INVALID_USE;
    }

    if (stat & CFMC_STAT_FPERR_Msk)
    {
        ret = DRIVER_ERROR_INVALID_USE;
    }

    if (stat & CFMC_STAT_OPERR_Msk)
    {
        ret = DRIVER_ERROR_INVALID_USE;
    }

    if (stat & CFMC_STAT_WSERR_Msk)
    {
        ret = DRIVER_ERROR_HW;
    }

    if (stat & CFMC_STAT_RPERR_Msk)
    {
        ret = DRIVER_ERROR_INVALID_USE;
    }

    return ret;
}

static uint32_t FLASH_GetEraseSize(enum flash_sector_size sector_size)
{
    uint32_t erase_len = 512;

    switch (sector_size)
    {
        case FLASH_SECTOR_SIZE_512B:
            erase_len = 512;
            break;
        case FLASH_SECTOR_SIZE_1KB:
            erase_len = 1024;
            break;
        case FLASH_SECTOR_SIZE_4KB:
            erase_len = 4096;
            break;
        default:
            erase_len = 512;
            break;
    }

    return erase_len;
}


int32_t HAL_FLASH_Init(void)
{
    if (*(volatile uint32_t *)0x4000F004 & 0x02)
    {
        g_code_flash_size = FLASH_SIZE_128K;
    }
    else if (*(volatile uint32_t *)0x4000F004 & 0x01)
    {
        g_code_flash_size = FLASH_SIZE_256K;
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_FLASH_Read(uint32_t flash_addr, uint8_t *data, uint32_t len)
{
    if (flash_addr & FLASH_DATA_ADRESS_Msk)
    {
        if ((flash_addr & ~(FLASH_DATA_ADRESS_Msk)) + len > FLASH_DATA_SIZE)
        {
            return DRIVER_ERROR_PARAMETER;
        }
    }
    else if (flash_addr + len > g_code_flash_size)
    {
        return DRIVER_ERROR_PARAMETER;
    }

    memcpy(data, (void *)flash_addr, len);

    return DRIVER_ERROR_OK;
}

int32_t HAL_FLASH_Write(uint32_t flash_addr, uint8_t *data, uint32_t len)
{
    int32_t ret = DRIVER_ERROR_OK;

    DRIVER_ASSERT((flash_addr & 0x03) == 0 && (len & 0x03) == 0);

    if (flash_addr & FLASH_DATA_ADRESS_Msk)
    {
        if ((flash_addr & ~(FLASH_DATA_ADRESS_Msk)) + len > FLASH_DATA_SIZE)
        {
            return DRIVER_ERROR_PARAMETER;
        }
    }
    else if (flash_addr + len > g_code_flash_size)
    {
        return DRIVER_ERROR_PARAMETER;
    }

    if (flash_addr & FLASH_DATA_ADRESS_Msk)
    {
        FLASH_DataWrite(flash_addr, data, len);
        ret = FLASH_GetStatus(FLASH_TYPE_DATA);
    }
    else
    {
        FLASH_CodeWrite(flash_addr, data, len);
        ret = FLASH_GetStatus(FLASH_TYPE_CODE);
    }

    return ret;
}

int32_t HAL_FLASH_Erase(uint32_t flash_addr)
{
    int32_t ret = DRIVER_ERROR_OK;
    uint32_t erase_len = 512;

    if (flash_addr & FLASH_DATA_ADRESS_Msk)
    {
        erase_len = FLASH_GetEraseSize(g_erase_data_sector_size);

        if ((flash_addr & ~FLASH_DATA_ADRESS_Msk) + erase_len > FLASH_DATA_SIZE)
        {
            return DRIVER_ERROR_PARAMETER;
        }

        FLASH_DataErase(flash_addr, g_erase_data_sector_size);
        ret = FLASH_GetStatus(FLASH_TYPE_DATA);
    }
    else
    {
        erase_len = FLASH_GetEraseSize(g_erase_code_sector_size);

        if (flash_addr + erase_len > g_code_flash_size)
        {
            return DRIVER_ERROR_PARAMETER;
        }

        FLASH_CodeErase(flash_addr, g_erase_code_sector_size);
        ret = FLASH_GetStatus(FLASH_TYPE_CODE);
    }

    return ret;
}

int32_t HAL_FLASH_OTP_Read(uint32_t otp_addr, uint8_t *data, uint32_t len)
{
    if ((otp_addr & ~(FLASH_OPT_ADDRESS_Msk)) + len > FLASH_OPT_SIZE)
    {
        return DRIVER_ERROR_PARAMETER;
    }

    memcpy(data, (void *)otp_addr, len);

    return DRIVER_ERROR_OK;
}

int32_t HAL_FLASH_SetEraseSecotorSize(enum flash_area area, enum flash_sector_size sector_size)
{
    if (area == FLASH_AREA_CODE)
    {
        g_erase_code_sector_size = sector_size;
    }
    else
    {
        g_erase_data_sector_size = sector_size;
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_FLASH_SetWriteProtect32KCodeArea(enum flash_prot32k_code_sector sector, uint8_t enable)
{
    if (enable)
    {
        CFMC->FLSPROT |= sector;
    }
    else
    {
        CFMC->FLSPROT &= ~(sector);
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_FLASH_SetWriteProtect512BCodeArea(enum flash_prot512b_code_sector sector, uint8_t enable)
{
    if (enable)
    {
        CFMC->FLSPROT |= (sector);
    }
    else
    {
        CFMC->FLSPROT &= ~(sector);
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_FLASH_SetWriteProtect2KDataArea(enum flash_prot2k_data_sector sector, uint8_t enable)
{
    if (enable)
    {
        DFMC->FLSPROT |= sector;
    }
    else
    {
        DFMC->FLSPROT &= ~(sector);
    }

    return DRIVER_ERROR_OK;
}

int32_t HAL_FLASH_SetWriteProtect512BDataArea(enum flash_prot512b_data_sector sector, uint8_t enable)
{
    if (enable)
    {
        DFMC->FLSPROT |= (sector);
    }
    else
    {
        DFMC->FLSPROT &= ~(sector);
    }      

    return DRIVER_ERROR_OK;
}

int32_t HAL_FLASH_SetReadProtectCodeArea(enum flash_read_prot_lvl prot_lvl)
{
    switch (prot_lvl)
    {
        case FLASH_READ_PROT_LVL_UNPROTECT:
            CFMC->READPROT = 0xFF;
            break;
        case FLASH_READ_PROT_LVL_1:
            CFMC->READPROT = 0x39;
            break;
        case FLASH_READ_PROT_LVL_1_ONE_PW:
            CFMC->READPROT = 0xB9;
            break;
        case FLASH_READ_PROT_LVL_2_TWO_PW:
            CFMC->READPROT = 0x80;
            break;
        case FLASH_READ_PROT_LVL_2_NO_PW:
            CFMC->READPROT = 0x01;
            break;
        default:
            break;
    }

    return DRIVER_ERROR_OK;
}


int32_t HAL_CFMC_FlashAccessCmd(uint8_t enable)	
{
	if (enable)
	{
		CFMC->FLSKEY = FLASH_ACCESS_KEY1;
		
		CFMC->FLSKEY = FLASH_ACCESS_KEY2;
		
		CFMC->FLSKEY = FLASH_ACCESS_KEY3;
	}
	
	else
	{
		CFMC->CTRL |= (1UL << CFMC_CTRL_FLOCK_Pos);	
	}
	
	 return DRIVER_ERROR_OK;
}
