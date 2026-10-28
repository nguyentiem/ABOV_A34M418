/*  NVS: non volatile storage in flash
 *
 * Copyright (c) 2018 Laczen
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#include "nvs.h"


#define ADDR_SECT_MASK                  0xFFFF0000
#define ADDR_SECT_SHIFT                 16
#define ADDR_OFFS_MASK                  0x0000FFFF

#define NVS_BLOCK_SIZE                  4
#define NVS_MIN(a, b)                   (((a) < (b)) ? (a) : (b))

struct nvs_meta
{
    uint16_t    id;
    uint16_t    offset;
    uint16_t    len;
    uint8_t     part;
    uint8_t     crc8;
} __PACKED;

static uint32_t flash_align_size(struct nvs_fs *fs, uint32_t len)
{
    return ((len + (fs->write_size - 1)) & ~(fs->write_size - 1));
}

static int32_t flash_read_data(struct nvs_fs *fs, uint32_t addr, void *data, uint32_t len)
{
    int32_t ret;
    uint32_t offset;

    offset = fs->base_addr;
    offset += fs->sector_size * (addr >> ADDR_SECT_SHIFT);
    offset += addr & ADDR_OFFS_MASK;
    ret = fs->flash_read(offset, data, len);

    return ret;
}

static int32_t flash_read_meta(struct nvs_fs *fs, uint32_t addr, struct nvs_meta *entry)
{
    int32_t ret;

    ret = flash_read_data(fs, addr, entry, sizeof(struct nvs_meta));

    return ret;
}

static int32_t flash_align_write(struct nvs_fs *fs, uint32_t addr, void *data, uint32_t len)
{
    uint8_t *data8 = (uint8_t *)data;
    int32_t ret = 0;
    uint32_t offset;
    uint32_t blen;
    uint8_t buf[NVS_BLOCK_SIZE];

    if (len == 0)
    {
        return NVS_SUCCESS;
    }

    offset = fs->base_addr;
    offset += fs->sector_size * (addr >> ADDR_SECT_SHIFT);
    offset += addr & ADDR_OFFS_MASK;

    blen = len & ~(fs->write_size - 1U);
    if (blen > 0)
    {
        ret = fs->flash_write(offset, data8, blen);
        if (ret)
        {
            goto end;
        }
        len -= blen;
        offset += blen;
        data8 += blen;
    }

    if (len)
    {
        memcpy(buf, data8, len);
        memset(buf + len, fs->erase_value, fs->write_size - len);
        ret = fs->flash_write(offset, buf, fs->write_size);
        if (ret)
        {
            goto end;
        }
    }

end:

    return ret;
}

static int32_t flash_write_meta(struct nvs_fs *fs, struct nvs_meta *entry)
{
    int32_t ret;

    ret = flash_align_write(fs, fs->meta_addr, entry, sizeof(struct nvs_meta));
    fs->meta_addr -= flash_align_size(fs, sizeof(struct nvs_meta));

    return ret;
}

static int32_t flash_write_data(struct nvs_fs *fs, void *data, uint32_t len)
{
    int32_t ret;

    ret = flash_align_write(fs, fs->data_addr, data, len);
    fs->data_addr += flash_align_size(fs, len);

    return ret;
}

static int32_t flash_cmp_block(struct nvs_fs *fs, uint32_t addr, void *data, uint32_t len)
{
    uint8_t *data8 = (uint8_t *)data;
    int32_t ret;
    uint32_t bytes_to_cmp, block_size;
    uint8_t buf[NVS_BLOCK_SIZE];

    block_size = NVS_BLOCK_SIZE & ~(fs->write_size - 1U);

    while (len)
    {
        bytes_to_cmp = NVS_MIN(block_size, len);
        ret = flash_read_data(fs, addr, buf, bytes_to_cmp);
        if (ret)
        {
            return ret;
        }

        ret = memcmp(data8, buf, bytes_to_cmp);
        if (ret)
        {
            return 1;
        }
        len -= bytes_to_cmp;
        addr += bytes_to_cmp;
        data8 += bytes_to_cmp;
    }

    return 0;
}

static int32_t flash_cmp_const(struct nvs_fs *fs, uint32_t addr, uint8_t value, uint32_t len)
{
    int32_t ret;
    uint32_t bytes_to_cmp, block_size;
    uint8_t cmp[NVS_BLOCK_SIZE];

    block_size = NVS_BLOCK_SIZE & ~(fs->write_size - 1U);

    memset(cmp, value, block_size);

    while (len)
    {
        bytes_to_cmp = NVS_MIN(block_size, len);
        ret = flash_cmp_block(fs, addr, cmp, bytes_to_cmp);

        if (ret)
        {
            return ret;
        }
        len -= bytes_to_cmp;
        addr += bytes_to_cmp;
    }

    return 0;
}

static int32_t flash_move_block(struct nvs_fs *fs, uint32_t addr, uint32_t len)
{
    int32_t ret;
    uint32_t bytes_to_copy, block_size;
    uint8_t buf[NVS_BLOCK_SIZE];

    block_size = NVS_BLOCK_SIZE & ~(fs->write_size - 1U);

    while (len)
    {
        bytes_to_copy = NVS_MIN(block_size, len);
        ret = flash_read_data(fs, addr, buf, bytes_to_copy);
        if (ret)
        {
            return ret;
        }
        ret = flash_write_data(fs, buf, bytes_to_copy);
        if (ret)
        {
            return ret;
        }
        len -= bytes_to_copy;
        addr += bytes_to_copy;
    }

    return 0;
}

static int32_t flash_erase_sector(struct nvs_fs *fs, uint32_t addr)
{
    int32_t ret;
    uint32_t offset;

    addr &= ADDR_SECT_MASK;
    ret = flash_cmp_const(fs, addr, fs->erase_value, fs->sector_size);
    if (ret <= 0)
    {
        return ret;
    }

    offset = fs->base_addr;
    offset += fs->sector_size * (addr >> ADDR_SECT_SHIFT);

    ret = fs->flash_erase(offset);
    if (ret)
    {
        return ret;
    }

    return 0;
}

static const uint8_t crc8_ccitt_small_table[16] = {
    0x00, 0x07, 0x0e, 0x09, 0x1c, 0x1b, 0x12, 0x15,
    0x38, 0x3f, 0x36, 0x31, 0x24, 0x23, 0x2a, 0x2d
};

static uint8_t crc8_ccitt(uint8_t val, const void *buf, size_t cnt)
{
    size_t i;
    const uint8_t *p = buf;

    for (i = 0; i < cnt; i++)
    {
        val ^= p[i];
        val = (val << 4) ^ crc8_ccitt_small_table[val >> 4];
        val = (val << 4) ^ crc8_ccitt_small_table[val >> 4];
    }
    return val;
}

static void nvs_meta_crc8_update(struct nvs_meta *entry)
{
    uint8_t crc8;

    crc8 = crc8_ccitt(0xff, entry, offsetof(struct nvs_meta, crc8));
    entry->crc8 = crc8;
}

static int32_t nvs_meta_crc8_check(struct nvs_meta *entry)
{
    uint8_t crc8;

    crc8 = crc8_ccitt(0xff, entry, offsetof(struct nvs_meta, crc8));
    if (crc8 == entry->crc8)
    {
        return 0;
    }
    return 1;
}

static int32_t nvs_meta_cmp_const(struct nvs_meta *entry, uint8_t value)
{
    uint8_t *data8 = (uint8_t *)entry;
    int32_t i;

    for (i = 0; i < sizeof(struct nvs_meta); i++)
    {
        if (data8[i] != value)
        {
            return 1;
        }
    }

    return 0;
}

static int32_t nvs_flash_wrt_entry(struct nvs_fs *fs, uint16_t id, void *data, uint32_t len)
{
    int32_t ret;
    struct nvs_meta entry;
    uint32_t meta_size;

    meta_size = flash_align_size(fs, sizeof(struct nvs_meta));
    (void)meta_size;

    entry.id = id;
    entry.offset = (uint16_t)(fs->data_addr & ADDR_OFFS_MASK);
    entry.len = (uint16_t)len;
    entry.part = 0xFF;

    nvs_meta_crc8_update(&entry);

    ret = flash_write_data(fs, data, len);
    if (ret)
    {
        return ret;
    }

    ret = flash_write_meta(fs, &entry);
    if (ret)
    {
        return ret;
    }

    return 0;
}

static int32_t nvs_recover_last_meta(struct nvs_fs *fs, uint32_t *addr)
{
    uint32_t data_end_addr, meta_end_addr;
    struct nvs_meta end_meta;
    uint32_t meta_size;
    int32_t ret;

    meta_size = flash_align_size(fs, sizeof(struct nvs_meta));

    *addr -= meta_size;
    meta_end_addr = *addr;
    data_end_addr = *addr & ADDR_SECT_MASK;

    while (meta_end_addr > data_end_addr)
    {
        ret = flash_read_meta(fs, meta_end_addr, &end_meta);
        if (ret)
        {
            return ret;
        }
        if (!nvs_meta_crc8_check(&end_meta))
        {
            data_end_addr &= ADDR_SECT_MASK;
            data_end_addr += end_meta.offset + end_meta.len;
            *addr = meta_end_addr;
        }
        meta_end_addr -= meta_size;
    }

    return 0;
}

static int32_t nvs_prev_meta(struct nvs_fs *fs, uint32_t *addr, struct nvs_meta *meta)
{
    int32_t ret;
    struct nvs_meta close_meta;
    uint32_t meta_size;

    meta_size = flash_align_size(fs, sizeof(struct nvs_meta));

    ret = flash_read_meta(fs, *addr, meta);
    if (ret)
    {
        return ret;
    }

    *addr += meta_size;
    if (((*addr) & ADDR_OFFS_MASK) != (fs->sector_size - meta_size))
    {
        return 0;
    }

    /* last meta in sector, do jump to previous sector */
    if (((*addr) >> ADDR_SECT_SHIFT) == 0U)
    {
        *addr += ((fs->sector_count - 1) << ADDR_SECT_SHIFT);
    }
    else
    {
        *addr -= (1 << ADDR_SECT_SHIFT);
    }

    ret = flash_read_meta(fs, *addr, &close_meta);
    if (ret)
    {
        return ret;
    }

    ret = nvs_meta_cmp_const(&close_meta, fs->erase_value);
    /* at the end of filesystem */
    if (!ret)
    {
        *addr = fs->meta_addr;
        return 0;
    }

    if (!nvs_meta_crc8_check(&close_meta))
    {
        /* updmeta the address so it points to the last added meta.
         * do a check on close_meta.offset so that it does not point
         * outside a sector and is aligned to meta size.
         */
        if (close_meta.offset < (fs->sector_size - meta_size) &&
            !(close_meta.offset % meta_size))
        {
            (*addr) &= ADDR_SECT_MASK;
            (*addr) += close_meta.offset;
            return 0;
        }
    }
    /* The close_meta had an invalid CRC8 or the last added meta offset was
     * recognized as incorrect, `lets find out the last valid meta
     * and point the address to this found meta.
     *
     * remark: if there was absolutely no valid data in the sector *addr
     * is kept at sector_end - 2*meta_size, the next read will contain
     * invalid data and continue with a sector jump
     */
    return nvs_recover_last_meta(fs, addr);
}

static void nvs_sector_advance(struct nvs_fs *fs, uint32_t *addr)
{
    *addr += (1 << ADDR_SECT_SHIFT);
    if ((*addr >> ADDR_SECT_SHIFT) == fs->sector_count)
    {
        *addr -= (fs->sector_count << ADDR_SECT_SHIFT);
    }
}

/* allocation entry close (this closes the current sector) by writing offset
 * of last meta to the sector end.
 */
static int32_t nvs_sector_close(struct nvs_fs *fs)
{
    int32_t ret;
    struct nvs_meta close_meta;
    uint32_t meta_size;

    meta_size = flash_align_size(fs, sizeof(struct nvs_meta));

    close_meta.id = 0xFFFF;
    close_meta.len = 0U;
    close_meta.offset = (uint16_t)((fs->meta_addr + meta_size) & ADDR_OFFS_MASK);

    fs->meta_addr &= ADDR_SECT_MASK;
    fs->meta_addr += (fs->sector_size - meta_size);

    nvs_meta_crc8_update(&close_meta);

    ret = flash_write_meta(fs, &close_meta);

    nvs_sector_advance(fs, &fs->meta_addr);

    fs->data_addr = fs->meta_addr & ADDR_SECT_MASK;

    return ret;
}

static int32_t nvs_gc(struct nvs_fs *fs)
{
    int32_t ret;
    struct nvs_meta close_meta, gc_meta, wlk_meta;
    uint32_t sec_addr, gc_addr, gc_prev_addr, wlk_addr, wlk_prev_addr,
             data_addr, stop_addr;
    uint32_t meta_size;

    meta_size = flash_align_size(fs, sizeof(struct nvs_meta));

    sec_addr = (fs->meta_addr & ADDR_SECT_MASK);
    nvs_sector_advance(fs, &sec_addr);
    gc_addr = sec_addr + fs->sector_size - meta_size;

    /* if the sector is not closed don't do gc */
    ret = flash_read_meta(fs, gc_addr, &close_meta);
    if (ret < 0)
    {
        /* flash error */
        return ret;
    }

    ret = nvs_meta_cmp_const(&close_meta, fs->erase_value);
    if (!ret)
    {
        ret = flash_erase_sector(fs, sec_addr);
        if (ret)
        {
            return ret;
        }
        return 0;
    }

    stop_addr = gc_addr - meta_size;

    if (!nvs_meta_crc8_check(&close_meta))
    {
        gc_addr &= ADDR_SECT_MASK;
        gc_addr += close_meta.offset;
    }
    else
    {
        ret = nvs_recover_last_meta(fs, &gc_addr);
        if (ret)
        {
            return ret;
        }
    }

    do
    {
        gc_prev_addr = gc_addr;
        ret = nvs_prev_meta(fs, &gc_addr, &gc_meta);
        if (ret)
        {
            return ret;
        }

        if (nvs_meta_crc8_check(&gc_meta))
        {
            continue;
        }

        wlk_addr = fs->meta_addr;
        do
        {
            wlk_prev_addr = wlk_addr;
            ret = nvs_prev_meta(fs, &wlk_addr, &wlk_meta);
            if (ret)
            {
                return ret;
            }
            /* if meta with same id is reached we might need to copy.
             * only consider valid wlk_meta's. Something wrong might
             * have been written that has the same meta but is
             * invalid, don't consider these as a match.
             */
            if ((wlk_meta.id == gc_meta.id) &&
                (!nvs_meta_crc8_check(&wlk_meta)))
            {
                break;
            }
        }
        while (wlk_addr != fs->meta_addr);

        /* if walk has reached the same address as gc_addr copy is
         * needed unless it is a deleted item.
         */
        if ((wlk_prev_addr == gc_prev_addr) && gc_meta.len)
        {
            /* copy needed */

            data_addr = (gc_prev_addr & ADDR_SECT_MASK);
            data_addr += gc_meta.offset;

            gc_meta.offset = (uint16_t)(fs->data_addr & ADDR_OFFS_MASK);
            nvs_meta_crc8_update(&gc_meta);

            ret = flash_move_block(fs, data_addr, gc_meta.len);
            if (ret)
            {
                return ret;
            }

            ret = flash_write_meta(fs, &gc_meta);
            if (ret)
            {
                return ret;
            }
        }
    }
    while (gc_prev_addr != stop_addr);

    ret = flash_erase_sector(fs, sec_addr);
    if (ret)
    {
        return ret;
    }
    return 0;
}

static int32_t nvs_startup(struct nvs_fs *fs)
{
    int32_t ret;
    struct nvs_meta last_meta;
    uint32_t meta_size, empty_len;
    /* Initialize addr to 0 for the case fs->sector_count == 0. This
     * should never happen as this is verified in nvs_init() but both
     * Coverity and GCC believe the contrary.
     */
    uint32_t addr = 0U;
    uint16_t i, closed_sectors = 0;
    uint8_t erase_value = fs->erase_value;

    meta_size = flash_align_size(fs, sizeof(struct nvs_meta));
    /* step through the sectors to find a open sector following
     * a closed sector, this is where NVS can to write.
     */
    for (i = 0; i < fs->sector_count; i++)
    {
        addr = (i << ADDR_SECT_SHIFT) +
               (uint16_t)(fs->sector_size - meta_size);
        ret = flash_cmp_const(fs, addr, erase_value,
                              sizeof(struct nvs_meta));
        if (ret)
        {
            /* closed sector */
            closed_sectors++;
            nvs_sector_advance(fs, &addr);
            ret = flash_cmp_const(fs, addr, erase_value,
                                  sizeof(struct nvs_meta));
            if (!ret)
            {
                /* open sector */
                break;
            }
        }
    }
    /* all sectors are closed, this is not a nvs fs */
    if (closed_sectors == fs->sector_count)
    {
        ret = NVS_ERROR;
        goto end;
    }

    if (i == fs->sector_count)
    {
        /* none of the sectors where closed, in most cases we can set
         * the address to the first sector, except when there are only
         * two sectors. Then we can only set it to the first sector if
         * the last sector contains no meta's. So we check this first
         */
        ret = flash_cmp_const(fs, addr - meta_size, erase_value,
                              sizeof(struct nvs_meta));
        if (!ret)
        {
            /* empty meta */
            nvs_sector_advance(fs, &addr);
        }
    }

    /* addr contains address of the last meta in the most recent sector
     * search for the first meta containing all cells erased.
     */
    fs->meta_addr = addr - meta_size;
    fs->data_addr = addr & ADDR_SECT_MASK;

    while (fs->meta_addr >= fs->data_addr)
    {
        ret = flash_read_meta(fs, fs->meta_addr, &last_meta);
        if (ret)
        {
            goto end;
        }

        ret = nvs_meta_cmp_const(&last_meta, erase_value);

        if (!ret)
        {
            /* found ff empty location */
            break;
        }

        if (!nvs_meta_crc8_check(&last_meta))
        {
            /* crc8 is ok, complete write of meta was performed */
            fs->data_addr = addr & ADDR_SECT_MASK;
            fs->data_addr += last_meta.offset;
            fs->data_addr += flash_align_size(fs, last_meta.len);

            /* meta on the last possition within the sector is
             * reserved for deletion an entry
             */
            if (fs->meta_addr == fs->data_addr && last_meta.len)
            {
                /* not a delete meta */
                ret = NVS_ERROR;
                goto end;
            }
        }

        fs->meta_addr -= meta_size;
    }

    /* possible data write after last meta write, updmeta data_addr */
    while (fs->meta_addr > fs->data_addr)
    {
        empty_len = fs->meta_addr - fs->data_addr;

        ret = flash_cmp_const(fs, fs->data_addr, erase_value,
                              empty_len);
        if (ret < 0)
        {
            goto end;
        }
        if (!ret)
        {
            break;
        }

        fs->data_addr += fs->write_size;
    }

    /* if the sector after the write sector is not empty gc was interrupted
     * we need to restart gc, first erase the sector before restarting gc
     * otherwise the data may not fit into the sector.
     */
    addr = fs->meta_addr & ADDR_SECT_MASK;
    nvs_sector_advance(fs, &addr);
    ret = flash_cmp_const(fs, addr, erase_value, fs->sector_size);
    if (ret < 0)
    {
        goto end;
    }
    if (ret)
    {
        /* the sector after fs->meta_wrt is not empty */
        ret = flash_erase_sector(fs, fs->meta_addr);
        if (ret)
        {
            goto end;
        }
        fs->meta_addr &= ADDR_SECT_MASK;
        fs->meta_addr += (fs->sector_size - 2 * meta_size);
        fs->data_addr = (fs->meta_addr & ADDR_SECT_MASK);
        ret = nvs_gc(fs);
        if (ret)
        {
            goto end;
        }
    }

end:
    return ret;
}

int32_t nvs_init(struct nvs_fs *fs)
{
    int32_t ret;

    if (fs->sector_count < 2)
    {
        return NVS_ERROR;
    }

    if (fs->base_addr % fs->sector_size)
    {
        return NVS_ERROR;
    }

    ret = nvs_startup(fs);
    if (ret)
    {
        return ret;
    }

    /* nvs is ready for use */
    fs->ready = 1;

    return NVS_SUCCESS;
}

int32_t nvs_clear(struct nvs_fs *fs)
{
    int32_t ret;
    uint32_t addr;

    if (!fs->ready)
    {
        return NVS_ERROR;
    }

    for (uint16_t i = 0; i < fs->sector_count; i++)
    {
        addr = i << ADDR_SECT_SHIFT;
        ret = flash_erase_sector(fs, addr);
        if (ret)
        {
            return ret;
        }
    }
    return NVS_SUCCESS;
}

int32_t nvs_write(struct nvs_fs *fs, uint16_t id, void *data, uint32_t len)
{
    int32_t ret, gc_count;
    uint32_t meta_size, data_size;
    struct nvs_meta wlk_meta;
    uint32_t wlk_addr, rd_addr;
    uint16_t required_space = 0U; /* no space, approprimeta for delete meta */
    bool prev_found = 0;

    if (!fs->ready)
    {
        return NVS_ERROR;
    }

    meta_size = flash_align_size(fs, sizeof(struct nvs_meta));
    data_size = flash_align_size(fs, len);

    /* The maximum data size is sector size - 3 meta
     * where: 1 meta for data, 1 meta for sector close
     * and 1 meta to always allow a delete.
     */
    if ((len > (fs->sector_size - 3 * meta_size)) ||
        ((len > 0) && (data == NULL)))
    {
        return NVS_ERROR;
    }

    /* find lmetast entry with same id */
    wlk_addr = fs->meta_addr;
    rd_addr = wlk_addr;

    while (1)
    {
        rd_addr = wlk_addr;
        ret = nvs_prev_meta(fs, &wlk_addr, &wlk_meta);
        if (ret)
        {
            return ret;
        }
        if ((wlk_meta.id == id) && (!nvs_meta_crc8_check(&wlk_meta)))
        {
            prev_found = 1;
            break;
        }
        if (wlk_addr == fs->meta_addr)
        {
            break;
        }
    }

    if (prev_found)
    {
        /* previous entry found */
        rd_addr &= ADDR_SECT_MASK;
        rd_addr += wlk_meta.offset;

        if (len == 0)
        {
            /* do not try to compare with empty data */
            if (wlk_meta.len == 0U)
            {
                /* skip delete entry as it is already the
                 * last one
                 */
                return 0;
            }
        }
        else if (len == wlk_meta.len)
        {
            /* do not try to compare if lengths are not equal */
            /* compare the data and if equal return 0 */
            ret = flash_cmp_block(fs, rd_addr, data, len);
            if (ret <= 0)
            {
                return ret;
            }
        }
    }
    else
    {
        /* skip delete entry for non-existing entry */
        if (len == 0)
        {
            return 0;
        }
    }

    /* calculmeta required space if the entry contains data */
    if (data_size)
    {
        /* Leave space for delete meta */
        required_space = data_size + meta_size;
    }

    gc_count = 0;
    while (1)
    {
        if (gc_count == fs->sector_count)
        {
            /* gc'ed all sectors, no extra space will be cremetad
             * by extra gc.
             */
            ret = NVS_ERROR;
            goto end;
        }

        if (fs->meta_addr >= fs->data_addr + required_space)
        {

            ret = nvs_flash_wrt_entry(fs, id, data, len);
            if (ret)
            {
                goto end;
            }
            break;
        }


        ret = nvs_sector_close(fs);
        if (ret)
        {
            goto end;
        }

        ret = nvs_gc(fs);
        if (ret)
        {
            goto end;
        }
        gc_count++;
    }
    ret = len;
end:
    return ret;
}

int32_t nvs_read_hist(struct nvs_fs *fs, uint16_t id, void *data, uint32_t len, uint16_t cnt)
{
    int32_t ret;
    uint32_t wlk_addr, rd_addr;
    uint16_t cnt_his;
    struct nvs_meta wlk_meta;
    uint32_t meta_size;

    if (!fs->ready)
    {
        return NVS_ERROR;
    }

    meta_size = flash_align_size(fs, sizeof(struct nvs_meta));

    if (len > (fs->sector_size - 2 * meta_size))
    {
        return NVS_ERROR;
    }

    cnt_his = 0U;

    wlk_addr = fs->meta_addr;
    rd_addr = wlk_addr;

    while (cnt_his <= cnt)
    {
        rd_addr = wlk_addr;
        ret = nvs_prev_meta(fs, &wlk_addr, &wlk_meta);
        if (ret)
        {
            goto err;
        }
        if ((wlk_meta.id == id) &&  (!nvs_meta_crc8_check(&wlk_meta)))
        {
            cnt_his++;
        }
        if (wlk_addr == fs->meta_addr)
        {
            break;
        }
    }

    if (((wlk_addr == fs->meta_addr) && (wlk_meta.id != id)) ||
        (wlk_meta.len == 0U) || (cnt_his < cnt))
    {
        return NVS_ERROR;
    }

    rd_addr &= ADDR_SECT_MASK;
    rd_addr += wlk_meta.offset;
    ret = flash_read_data(fs, rd_addr, data, NVS_MIN(len, wlk_meta.len));
    if (ret)
    {
        goto err;
    }

    return wlk_meta.len;

err:
    return ret;
}

int32_t nvs_read(struct nvs_fs *fs, uint16_t id, void *data, uint32_t len)
{
    int32_t ret;

    ret = nvs_read_hist(fs, id, data, len, 0);
    return ret;
}

int32_t nvs_free_space(struct nvs_fs *fs)
{

    int32_t ret;
    struct nvs_meta step_meta, wlk_meta;
    uint32_t step_addr, wlk_addr;
    uint32_t meta_size, free_space;

    if (!fs->ready)
    {
        return NVS_ERROR;
    }

    meta_size = flash_align_size(fs, sizeof(struct nvs_meta));

    free_space = 0;
    for (uint16_t i = 1; i < fs->sector_count; i++)
    {
        free_space += (fs->sector_size - meta_size);
    }

    step_addr = fs->meta_addr;

    while (1)
    {
        ret = nvs_prev_meta(fs, &step_addr, &step_meta);
        if (ret)
        {
            return ret;
        }

        wlk_addr = fs->meta_addr;

        while (1)
        {
            ret = nvs_prev_meta(fs, &wlk_addr, &wlk_meta);
            if (ret)
            {
                return ret;
            }
            if ((wlk_meta.id == step_meta.id) ||
                (wlk_addr == fs->meta_addr))
            {
                break;
            }
        }

        if ((wlk_addr == step_addr) && step_meta.len &&
            (!nvs_meta_crc8_check(&step_meta)))
        {
            /* count needed */
            free_space -= flash_align_size(fs, step_meta.len);
            free_space -= meta_size;
        }

        if (step_addr == fs->meta_addr)
        {
            break;
        }

    }
    return free_space;
}


