/**
 *******************************************************************************
 * @file        ringbuf.h
 * @author      ABOV R&D Division
 * @brief       Ring Buffer Header File
 *
 * Copyright 2017 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/
#ifndef _RINGBUF_H_
#define _RINGBUF_H_

#include <stdint.h>

struct ringbuf
{
    uint32_t    head;
    uint32_t    tail;
    uint32_t    capacity;
    uint8_t *   data_pool;
};

struct ringbuf_seg
{
    uint8_t *   data;
    uint8_t     size;
};

enum ringbuf_status
{
    RINGBUF_SUCCESS,
    RINGBUF_SIZE_OVERFLOW,
    RINGBUF_NOT_ENOUGH_SPACE,
    RINGBUF_NOT_ENOUGH_DATA,
};

void ringbuf_init(struct ringbuf *rbuf, uint32_t capacity, uint8_t *data_pool);
void ringbuf_reset(struct ringbuf *rbuf);
uint8_t ringbuf_empty(struct ringbuf *rbuf);
uint32_t ringbuf_space(struct ringbuf *rbuf);
uint8_t ringbuf_put(struct ringbuf *rbuf, uint8_t *data, uint32_t size);
uint8_t ringbuf_get(struct ringbuf *rbuf, uint8_t *data, uint32_t size);
uint8_t ringbuf_get_claim(struct ringbuf *rbuf, uint8_t **data, uint32_t size);
uint8_t ringbuf_get_release(struct ringbuf *rbuf, uint32_t size);

#endif /* _RINGBUF_H_ */

