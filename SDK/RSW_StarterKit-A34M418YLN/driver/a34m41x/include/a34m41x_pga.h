/**
 *******************************************************************************
 * @file        a34m41x_pga.h
 * @author      ABOV R&D Division
 * @brief       PGA Header File
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
 * @defgroup    driver_pga PGA
 * @{
 * @brief       PGA driver
 */


#ifndef _A34M41X_PGA_H_
#define _A34M41X_PGA_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "a34m41x_driver_common.h"


/**
 * PGA id
 */
enum pga_id
{
    PGA_ID_0,                               /**< PGA 0 */
    PGA_ID_1,                               /**< PGA 1 */
    PGA_ID_2,                               /**< PGA 2 */
    PGA_ID_MAX,                             /**< PGA max */
};

/**
 * PGA current setting
 */
enum pga_current
{
    PGA_CR_AMPISEL_0    = 0,                /**< PGA  Current 0 % */
    PGA_CR_AMPISEL_17   = 1,                /**< PGA  Current 17% */
    PGA_CR_AMPISEL_21   = 2,                /**< PGA  Current -21% */
    PGA_CR_AMPISEL_12   =3                  /**< PGA  Current -12% */
};

/**
 * PGA gain selection
 */
enum pga_gain
{
    PGA_GAINSEL_1_200   = (0x0uL << 0),
    PGA_GAINSEL_1_304   = (0x1uL << 0),
    PGA_GAINSEL_1_404   = (0x2uL << 0),
    PGA_GAINSEL_1_500   = (0x3uL << 0),
    PGA_GAINSEL_1_600   = (0x4uL << 0),
    PGA_GAINSEL_1_702   = (0x5uL << 0),
    PGA_GAINSEL_1_805   = (0x6uL << 0),
    PGA_GAINSEL_1_905   = (0x7uL << 0),
    PGA_GAINSEL_2_000   = (0x8uL << 0),
    PGA_GAINSEL_2_182   = (0x9uL << 0),
    PGA_GAINSEL_2_330   = (0xAuL << 0),
    PGA_GAINSEL_2_500   = (0xBuL << 0),
    PGA_GAINSEL_2_667   = (0xCuL << 0),
    PGA_GAINSEL_2_927   = (0xDuL << 0),
    PGA_GAINSEL_3_000   = (0xEuL << 0),
    PGA_GAINSEL_3_158   = (0xFuL << 0),
    PGA_GAINSEL_3_478   = (0x10uL << 0),
    PGA_GAINSEL_3_871   = (0x11uL << 0),
    PGA_GAINSEL_4_000   = (0x12uL << 0),
    PGA_GAINSEL_4_364   = (0x13uL << 0),
    PGA_GAINSEL_5_000   = (0x14uL << 0),
    PGA_GAINSEL_5_854   = (0x15uL << 0),
    PGA_GAINSEL_6_000   = (0x16uL << 0),
    PGA_GAINSEL_6_857   = (0x17uL << 0),
    PGA_GAINSEL_8_000   = (0x18uL << 0),
    PGA_GAINSEL_8_571   = (0x19uL << 0),
    PGA_GAINSEL_10_00   = (0x1AuL << 0),
    PGA_GAINSEL_12_00   = (0x1BuL << 0),
    PGA_GAINSEL_15_00   = (0x1CuL << 0),
    PGA_GAINSEL_16_00   = (0x1DuL << 0),
};


/**
 * PAG unit gain
 */
enum pga_ugain
{
    PGA_UGAIN_DISABLE   = 0,                /**< unit gain disable */
    PGA_UGAIN_ENABLE    = 1,                /**< unit gain enable  */
};

/**
 * PGA driver configuration
 */
struct pga_cfg
{
    enum pga_current    ampi;               /**< current selection */
    enum pga_gain       gain;               /**< gain selection */
    enum pga_ugain      ugain;              /**< unity gain - disabling gain */
};

/**
 *******************************************************************************
 * @brief       Initialize PGA.
 * @param[in]   id instance id
 * @param[in]   cfg configuration
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_PGA_Init(enum pga_id id, struct pga_cfg *cfg);

/**
 *******************************************************************************
 * @brief       Deinitialize PGA.
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_PGA_Deinit(enum pga_id id);

/**
 *******************************************************************************
 * @brief       Start PGA.
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_PGA_Start(enum pga_id id);

/**
 *******************************************************************************
 * @brief       Stop PGA.
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_PGA_Stop(enum pga_id id);


#ifdef __cplusplus
}
#endif

#endif /* _A34M41X_PGA_H_ */

/** @} */
/** @} */

