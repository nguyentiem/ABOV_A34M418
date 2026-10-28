/**
 *******************************************************************************
 * @file        a34m41x_flash.h
 * @author      ABOV R&D Division
 * @brief       Flash Header File
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
 * @defgroup    driver_flash FLASH
 * @{
 * @brief       FLASH driver
 */

#ifndef _A34M41X_FLASH_H_
#define _A34M41X_FLASH_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "a34m41x_driver_common.h"

/** Flash write busy  time-out definitions in case of using function
 * with Blocking Flag mode
 */
#define	FLASH_BUSY_BLOCKING_TIMEOUT			(0x35000UL)	
		
	
/**
 * flash area type
 */
enum flash_area
{
    FLASH_AREA_CODE = 0,                    /**< flash code area, used for instruction and data */
    FLASH_AREA_DATA = 1,                    /**< flash data area, only used as data */
};

/**
 * flash section size
 */
enum flash_sector_size
{
    FLASH_SECTOR_SIZE_512B  = 0,            /**< sector size as 512 byte */
    FLASH_SECTOR_SIZE_1KB   = 1,            /**< sector size as 1024 byte */
    FLASH_SECTOR_SIZE_4KB   = 2,            /**< sector size as 4096 byte */
};

/**
 * flash protection for 32k byte code area
 */
enum flash_prot32k_code_sector
{
    FLASH_PROT32K_CODE_SECTOR0  = BIT(0),   /**< 0x0000_0000 ~ 0x0000_7FFF protect */
    FLASH_PROT32K_CODE_SECTOR1  = BIT(1),   /**< 0x0000_8000 ~ 0x0000_FFFF protect */
    FLASH_PROT32K_CODE_SECTOR2  = BIT(2),   /**< 0x0001_0000 ~ 0x0001_7FFF protect */
    FLASH_PROT32K_CODE_SECTOR3  = BIT(3),   /**< 0x0001_8000 ~ 0x0001_FFFF protect */
    FLASH_PROT32K_CODE_SECTOR4  = BIT(4),   /**< 0x0002_0000 ~ 0x0002_7FFF protect */
    FLASH_PROT32K_CODE_SECTOR5  = BIT(5),   /**< 0x0002_8000 ~ 0x0002_FFFF protect */
    FLASH_PROT32K_CODE_SECTOR6  = BIT(6),   /**< 0x0003_0000 ~ 0x0003_7FFF protect */
    FLASH_PROT32K_CODE_SECTOR7  = BIT(7),   /**< 0x0003_8000 ~ 0x0003_FFFF protect */
    FLASH_PROT32K_CODE_SECTOR8  = BIT(8),   /**< 0x0004_0000 ~ 0x0004_7FFF protect */
    FLASH_PROT32K_CODE_SECTOR9  = BIT(9),   /**< 0x0004_8000 ~ 0x0004_FFFF protect */
    FLASH_PROT32K_CODE_SECTOR10 = BIT(10),  /**< 0x0005_0000 ~ 0x0005_7FFF protect */
    FLASH_PROT32K_CODE_SECTOR11 = BIT(11),  /**< 0x0005_8000 ~ 0x0005_FFFF protect */
    FLASH_PROT32K_CODE_SECTOR12 = BIT(12),  /**< 0x0006_0000 ~ 0x0006_7FFF protect */
    FLASH_PROT32K_CODE_SECTOR13 = BIT(13),  /**< 0x0006_8000 ~ 0x0006_FFFF protect */
    FLASH_PROT32K_CODE_SECTOR14 = BIT(14),  /**< 0x0007_0000 ~ 0x0007_7FFF protect */
    FLASH_PROT32K_CODE_SECTOR15 = BIT(15),  /**< 0x0007_8000 ~ 0x0007_FFFF protect */
};

/**
 * flash protection for 512 byte code area
 */
enum flash_prot512b_code_sector
{
    FLASH_PROT512B_CODE_SECTOR0 = BIT(0),   /**< 0x0007_F000 ~ 0x0007_F1FF protect */
    FLASH_PROT512B_CODE_SECTOR1 = BIT(1),   /**< 0x0007_F200 ~ 0x0007_F3FF protect */
    FLASH_PROT512B_CODE_SECTOR2 = BIT(2),   /**< 0x0007_F400 ~ 0x0007_F5FF protect */
    FLASH_PROT512B_CODE_SECTOR3 = BIT(3),   /**< 0x0007_F600 ~ 0x0007_F7FF protect */
    FLASH_PROT512B_CODE_SECTOR4 = BIT(4),   /**< 0x0007_F800 ~ 0x0007_F9FF protect */
    FLASH_PROT512B_CODE_SECTOR5 = BIT(5),   /**< 0x0007_FA00 ~ 0x0007_FBFF protect */
    FLASH_PROT512B_CODE_SECTOR6 = BIT(6),   /**< 0x0007_FC00 ~ 0x0007_FDFF protect */
    FLASH_PROT512B_CODE_SECTOR7 = BIT(7),   /**< 0x0007_FE00 ~ 0x0007_FFFF protect */
};

/**
 * flash protection for 2K byte data area
 */
enum flash_prot2k_data_sector
{
    FLASH_PROT2K_DATA_SECTOR0   = BIT(0),   /**< 0x0E00_0000 ~ 0x0E00_07FF protect */
    FLASH_PROT2K_DATA_SECTOR1   = BIT(1),   /**< 0x0E00_0800 ~ 0x0E00_0FFF protect */
    FLASH_PROT2K_DATA_SECTOR2   = BIT(2),   /**< 0x0E00_1000 ~ 0x0E00_17FF protect */
    FLASH_PROT2K_DATA_SECTOR3   = BIT(3),   /**< 0x0E00_1800 ~ 0x0E00_1FFF protect */
    FLASH_PROT2K_DATA_SECTOR4   = BIT(4),   /**< 0x0E00_2000 ~ 0x0E00_27FF protect */
    FLASH_PROT2K_DATA_SECTOR5   = BIT(5),   /**< 0x0E00_2800 ~ 0x0E00_2FFF protect */
    FLASH_PROT2K_DATA_SECTOR6   = BIT(6),   /**< 0x0E00_3000 ~ 0x0E00_37FF protect */
    FLASH_PROT2K_DATA_SECTOR7   = BIT(7),   /**< 0x0E00_3800 ~ 0x0E00_3FFF protect */
    FLASH_PROT2K_DATA_SECTOR8   = BIT(8),   /**< 0x0E00_4000 ~ 0x0E00_47FF protect */
    FLASH_PROT2K_DATA_SECTOR9   = BIT(9),   /**< 0x0E00_4800 ~ 0x0E00_4FFF protect */
    FLASH_PROT2K_DATA_SECTOR10  = BIT(10),  /**< 0x0E00_5000 ~ 0x0E00_57FF protect */
    FLASH_PROT2K_DATA_SECTOR11  = BIT(11),  /**< 0x0E00_5800 ~ 0x0E00_5FFF protect */
    FLASH_PROT2K_DATA_SECTOR12  = BIT(12),  /**< 0x0E00_6000 ~ 0x0E00_67FF protect */
    FLASH_PROT2K_DATA_SECTOR13  = BIT(13),  /**< 0x0E00_6800 ~ 0x0E00_6FFF protect */
    FLASH_PROT2K_DATA_SECTOR14  = BIT(14),  /**< 0x0E00_7000 ~ 0x0E00_77FF protect */
    FLASH_PROT2K_DATA_SECTOR15  = BIT(15),  /**< 0x0E00_7800 ~ 0x0E00_7FFF protect */
};

/**
 * flash protection for 512 byte data area
 */
enum flash_prot512b_data_sector
{
    FLASH_PROT512B_DATA_SECTOR0 = BIT(0),   /**< 0x0E00_7000 ~ 0x0E00_71FF protect */
    FLASH_PROT512B_DATA_SECTOR1 = BIT(1),   /**< 0x0E00_7200 ~ 0x0E00_73FF protect */
    FLASH_PROT512B_DATA_SECTOR2 = BIT(2),   /**< 0x0E00_7400 ~ 0x0E00_75FF protect */
    FLASH_PROT512B_DATA_SECTOR3 = BIT(3),   /**< 0x0E00_7600 ~ 0x0E00_77FF protect */
    FLASH_PROT512B_DATA_SECTOR4 = BIT(4),   /**< 0x0E00_7800 ~ 0x0E00_79FF protect */
    FLASH_PROT512B_DATA_SECTOR5 = BIT(5),   /**< 0x0E00_7A00 ~ 0x0E00_7BFF protect */
    FLASH_PROT512B_DATA_SECTOR6 = BIT(6),   /**< 0x0E00_7C00 ~ 0x0E00_7DFF protect */
    FLASH_PROT512B_DATA_SECTOR7 = BIT(7),   /**< 0x0E00_7E00 ~ 0x0E00_7FFF protect */
};

/**
 * flash protection level
 */
enum flash_read_prot_lvl
{
    FLASH_READ_PROT_LVL_UNPROTECT   = 0,    /**< flash protection none */
    FLASH_READ_PROT_LVL_1           = 1,    /**< flash protection leve 1 */
    FLASH_READ_PROT_LVL_1_ONE_PW    = 2,    /**< flash protection leve 1 with one password */
    FLASH_READ_PROT_LVL_2_TWO_PW    = 3,    /**< flash protection leve 2 with two password */
    FLASH_READ_PROT_LVL_2_NO_PW     = 4,    /**< flash protection leve 2 with no password */
};

/**
 *******************************************************************************
 * @brief       Flash initalize
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FLASH_Init(void);

/**
 *******************************************************************************
 * @brief       Read flash memory.
 * @param[in]   flash_addr address of the memory to read
 * @param[out]  data pointer to the memory the data is to be copied to.
 * @param[in]   len length of the memory to read
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FLASH_Read(uint32_t flash_addr, uint8_t *data, uint32_t len);

/**
 *******************************************************************************
 * @brief       Write flash memory.
 *              The memory must be erased first to write new data.
 *              - memory address can be byte aligned.
 * @param[in]   flash_addr address of the memory to read
 * @param[in]   data pointer to the memory the data is to be copied from.
 * @param[in]   len length of the memory to write
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FLASH_Write(uint32_t flash_addr, uint8_t *data, uint32_t len);

/**
 *******************************************************************************
 * @brief       Erase flash memory. The whole sector at the
 *              specified memory will be erased.
 * @param[in]   flash_addr address of the memory to erase
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FLASH_Erase(uint32_t flash_addr);

/**
 *******************************************************************************
 * @brief       Read from OTP flash memory.
 * @param[in]   otp_addr address of the memory to read
 * @param[in]   data pointer to the memory the data is to be copied to.
 * @param[in]   len length of the memory to read
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FLASH_OTP_Read(uint32_t otp_addr, uint8_t *data, uint32_t len);

/**
 *******************************************************************************
 * @brief       This function set flash erase sector size.
 * @param[in]   area flash area (::flash_area)
 * @param[in]   sector_size flash sector size (::flash_sector_size)
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FLASH_SetEraseSecotorSize(enum flash_area area, enum flash_sector_size sector_size);

/**
 *******************************************************************************
 * @brief       This function enable write/erase protect of code area by 32kb unit.
 * @param[in]   sector protection sector (::flash_prot32k_code_sector)
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FLASH_SetWriteProtect32KCodeArea(enum flash_prot32k_code_sector sector, uint8_t enable);

/**
 *******************************************************************************
 * @brief       This function enable write/erase protect of code area by 512b unit.
 * @param[in]   sector protection sector (::flash_prot512b_code_sector)
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FLASH_SetWriteProtect512BCodeArea(enum flash_prot512b_code_sector sector, uint8_t enable);

/**
 *******************************************************************************
 * @brief       This function enable write/erase protect of data area by 2kb unit.
 * @param[in]   sector protection sector (::flash_prot2k_data_sector)
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FLASH_SetWriteProtect2KDataArea(enum flash_prot2k_data_sector sector, uint8_t enable);

/**
 *******************************************************************************
 * @brief       This function enable write/erase protect of data area by 512b unit.
 * @param[in]   sector protection sector (::flash_prot512b_data_sector)
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FLASH_SetWriteProtect512BDataArea(enum flash_prot512b_data_sector sector, uint8_t enable);

/**
 *******************************************************************************
 * @brief       This function select read protection level in debug mode.
 * @param[in]   prot_lvl protection level (::flash_read_prot_lvl)
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FLASH_SetReadProtectCodeArea(enum flash_read_prot_lvl prot_lvl);

/**
 *******************************************************************************
 * @brief       This function select read protection level in debug mode.
 * @param[in]   prot_lvl protection level (::flash_read_prot_lvl)
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_CFMC_FlashAccessCmd(uint8_t enable);

#endif /* _A34M41X_FLASH_H_ */

/** @} */
/** @} */

