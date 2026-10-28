/**
 *******************************************************************************
 * @file        main.c
 * @author      ABOV R&D Division
 * @brief       Main Example Code
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
#include <stdio.h>

#include "main.h"
#include "debug_log.h"

#include "tinycrypt/aes.h"


/* key for the test */
static uint8_t app_ref_key[16] =
{
    0x2b, 0x7e, 0x15, 0x16,
    0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88,
    0x09, 0xcf, 0x4f, 0x3c,
};

/* plain text for the test */
static uint8_t app_ref_text[16] =
{
    0x32, 0x43, 0xf6, 0xa8,
    0x88, 0x5a, 0x30, 0x8d,
    0x31, 0x31, 0x98, 0xa2,
    0xe0, 0x37, 0x07, 0x34,
};

/* encrypted text for the test */
static uint8_t app_ref_encrypted[16] =
{
    0x39, 0x25, 0x84, 0x1d,
    0x02, 0xdc, 0x09, 0xfb,
    0xdc, 0x11, 0x85, 0x97,
    0x19, 0x6a, 0x0b, 0x32,
};

static uint8_t app_encrypted[16];
static uint8_t app_decrypted[16];

static void app_log_aes(char *str, uint8_t *buf, uint8_t len)
{
    LOG(" %-10s: ", str);
    LOG_HEX(buf, len);
}

static volatile uint8_t aes_encrypt_done;
static volatile uint8_t aes_decrypt_done;

static void app_aes128_irq_handler(uint32_t event, void *context)
{
    if (event == AES128_EVENT_ENCRYPT_DONE)
    {
        aes_encrypt_done = 1;
    }
    else if (event == AES128_EVENT_DECRYPT_DONE)
    {
        aes_decrypt_done = 1;
    }
}

static void app_hw_aes(void)
{
    struct aes128_cfg cfg = APP_AES128_CONFIG;

#if (APP_AES128_DMA_ENABLE == 1)
    cfg.out_order = AES128_ORDER_MSB_BYTE;
    cfg.in_order = AES128_ORDER_MSB_BYTE;
#endif

    /* initialize aes128 */
    HAL_AES128_Init(&cfg, app_aes128_irq_handler, NULL);

#if (APP_AES128_DMA_ENABLE == 1)
    /* Enable DMA */
    HAL_AES128_SetTransmitDMA(DMA_ID_0, 1);
    HAL_AES128_SetReceiveDMA(DMA_ID_1, 1);
#endif

    /* set key */
    HAL_AES128_SetKey(app_ref_key);

    /* set plain text and start aes */
    aes_encrypt_done = 0;
    HAL_AES128_Encrypt(app_encrypted, app_ref_text, AES128_IO_TYPE_NON_BLOCK);
    while (aes_encrypt_done == 0)
        ;

    /* set encrypted text and start aes */
    aes_decrypt_done = 0;
    HAL_AES128_Decrypt(app_decrypted, app_ref_encrypted, AES128_IO_TYPE_NON_BLOCK);
    while (aes_decrypt_done == 0)
        ;
}

static void app_sw_aes(void)
{
    struct tc_aes_key_sched_struct s;

    /* set key */
    tc_aes128_set_encrypt_key(&s, app_ref_key);

    /* encrypt */
    tc_aes_encrypt(app_encrypted, app_ref_text, &s);

    /* decrypt */
    tc_aes_decrypt(app_decrypted, app_ref_encrypted, &s);
}

static void mainloop(void)
{
    LOG("hardware aes\n");

    /* run hardware aes */
    app_hw_aes();

    app_log_aes("plain", app_ref_text, 16);
    app_log_aes("encrypted", app_encrypted, 16);
    app_log_aes("decrypted", app_decrypted, 16);

    LOG("software aes\n");

    /* run software aes */
    app_sw_aes();

    app_log_aes("plain", app_ref_text, 16);
    app_log_aes("encrypted", app_encrypted, 16);
    app_log_aes("decrypted", app_decrypted, 16);

    while (1)
    {
    }
}

int main(void)
{
    /* Initialize all port */
    Port_Init();

    /* Configure system settings */
    System_Setting_Config();

    /* Initialize UART Debugging framework to use logging or command input */
    Debug_Init();

    LOG("\n");
    LOG("************************************************\n");
    LOG("\t MCU: %s\n", APP_DEVICE);
    LOG("\t Core: %s\n", APP_CORE);
    LOG("\t Debug: UART%d - %dbps\n", APP_UART_ID, APP_UART_BAUD);
    LOG("\t %s, %s\n", __DATE__, __TIME__);
    LOG("\t AES128 Interrupt\n");
    LOG("************************************************\n");

    /* Start main loop */
    mainloop();
}

