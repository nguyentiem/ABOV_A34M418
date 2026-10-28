/**
 *******************************************************************************
 * @file        debug_cmd.h
 * @author      ABOV R&D Division
 * @brief       Debug Command Header File
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
 * @defgroup    debug_cmd Command
 * @{
 * @brief       debugging with command line input
 */

#ifndef _DEBUG_CMD_H_
#define _DEBUG_CMD_H_

#include <stdint.h>

/**
 * command status
 */
enum debug_cmd_status
{
    DEBUG_CMD_SUCCESS,      /**< command function succeeded */
    DEBUG_CMD_INVALID,      /**< command parameters are invalid, help displayed */
    DEBUG_CMD_FAILED,       /**< command function failed */
};

/**
 * command function type
 */
typedef enum debug_cmd_status (*debug_cmd_func_t)(int argc, char *argv[]);

/**
 * command, function, description, argument information
 */
struct debug_cmd
{
    char *              str;    /**< command name */
    debug_cmd_func_t    func;   /**< function to be executed */
    const char *        desc;   /**< description of the command */
    const char *        usage;  /**< description of the command argument */
};

/**
 * macro to count number of command list
 */
#define DEBUG_CMD_LIST_COUNT(list) (sizeof(list) / sizeof(struct debug_cmd))


#if (CONFIG_DEBUG == 1)

/**
 *******************************************************************************
 * @brief       Initialize command line input processing.
 * @param[in]   list list of commands
 * @param[in]   list_size size of the list
 * @return      none
 ******************************************************************************/
void debug_cmd_init(const struct debug_cmd *list, uint8_t list_size);

/**
 *******************************************************************************
 * @brief       Deinitialize command line input processing.
 * @return      none
 ******************************************************************************/
void debug_cmd_deinit(void);

/**
 *******************************************************************************
 * @brief       Processs command line input. Should be called after an (UART)
 *              interrupt to check if there is a carriage return entered
 *              from the user, and process the string if so.
 * @return      none
 ******************************************************************************/
void debug_cmd_process(void);

#else

#define debug_cmd_init(list, list_size)  do { \
        (void)list;                           \
        (void)list_size;                      \
} while (0)
#define debug_cmd_deinit()
#define debug_cmd_process()

#endif

#endif /* _DEBUG_CMD_H_ */

/** @} */
/** @} */

