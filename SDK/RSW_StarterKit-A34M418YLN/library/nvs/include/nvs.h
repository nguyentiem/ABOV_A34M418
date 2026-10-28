/*  NVS: non volatile storage in flash
 *
 * Copyright (c) 2018 Laczen
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _NVS_H_
#define _NVS_H_

#include <stdint.h>

enum nvs_status
{
    NVS_SUCCESS = 0,
    NVS_ERROR   = -1,
};

struct nvs_fs
{
    uint32_t    base_addr;              /* base address of the nvs */
    uint32_t    meta_addr;              /* internal use */
    uint32_t    data_addr;              /* internal use */
    uint16_t    sector_size;            /* sector size */
    uint16_t    sector_count;           /* sector count */
    uint8_t     erase_value;            /* value of byte after erased */
    uint8_t     write_size;             /* write size */
    uint8_t     ready;                  /* internal use */
    int32_t     (*flash_read)(uint32_t flash_addr, uint8_t *data, uint32_t len);
    int32_t     (*flash_write)(uint32_t flash_addr, uint8_t *data, uint32_t len);
    int32_t     (*flash_erase)(uint32_t flash_addr);
};

int32_t nvs_init(struct nvs_fs *fs);
int32_t nvs_clear(struct nvs_fs *fs);
int32_t nvs_write(struct nvs_fs *fs, uint16_t id, void *data, uint32_t len);
int32_t nvs_read(struct nvs_fs *fs, uint16_t id, void *data, uint32_t len);
int32_t nvs_free_space(struct nvs_fs *fs);

#endif /* _NVS_H_ */

