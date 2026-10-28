/**
 *******************************************************************************
 * @file        debug_log.h
 * @author      ABOV R&D Division
 * @brief       Debug Log Header File
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
 * @addtogroup  library
 * @{
 * @defgroup    debug_log Log
 * @{
 * @brief       debugging with log
 */

#ifndef _DEBUG_LOG_H_
#define _DEBUG_LOG_H_

#if (CONFIG_DEBUG)

#include <stdint.h>
#include <stdio.h>

/**
 *******************************************************************************
 * @brief       print log
 * @param[in]   fmt printf style variable arguments.
 * @return      0 (does not return the number of characters)
 ******************************************************************************/
int debug_log(const char *fmt, ...);

/**
 *******************************************************************************
 * @brief       print hex formatted string
 * @param[in]   data data to print
 * @param[in]   len length of the data
 * @return      none
 ******************************************************************************/
void debug_log_hex(void *data, uint16_t len);

/**
 * Logging MACRO similar to the printf style
 */
#define LOG             debug_log

/**
 * Logging MACRO to print starting part of the buffer
 */
#define LOG_HEX         debug_log_hex

#else /* no logging */

/**
 * Logging MACRO similar to the printf style
 */
#define LOG(...)

/**
 * Logging MACRO to print starting part of the buffer
 */
#define LOG_HEX(...)

#endif

#endif /* _DEBUG_LOG_H_ */

/** @} */
/** @} */

