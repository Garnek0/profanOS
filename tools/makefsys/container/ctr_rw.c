#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../butterfly.h"

int fs_cnt_rw_core(filesys_t *filesys, sid_t core_sid, uint8_t *buf, uint32_t offset, uint32_t size, int is_read) {
    vdisk_t *vdisk;
    uint8_t *data;

    vdisk = fs_get_vdisk(filesys, core_sid.device);

    if (vdisk == NULL) {
        printf("d%ds%d not found\n", core_sid.device, core_sid.sector);
        return -1;
    }

    // check if sector is used
    if (!vdisk_is_sector_used(vdisk, core_sid)) {
        printf("d%ds%d not used\n", core_sid.device, core_sid.sector);
        return -1;
    }

    // check if sector is core
    data = vdisk_load_sector(vdisk, core_sid);

    if (data[0] != ST_CONT || data[1] != SF_CORE) {
        printf("d%ds%d not core\n", core_sid.device, core_sid.sector);
        vdisk_unload_sector(vdisk, core_sid, data, NO_SAVE);
        return -1;
    }

    // check if offset is valid
    if (offset >= BYTE_IN_CORE) {
        printf("offset %d out of range\n", offset);
        vdisk_unload_sector(vdisk, core_sid, data, NO_SAVE);
        return -1;
    }

    size = min(size, BYTE_IN_CORE - offset);
    offset += 2;

    if (!is_read) {
        memcpy(data + offset, buf, size);
    } else {
        memcpy(buf, data + offset, size);
    }

    vdisk_unload_sector(vdisk, core_sid, data, NO_SAVE);
    return size + offset - 2;
}

int fs_cnt_rw_loca(filesys_t *filesys, sid_t loca_sid, uint8_t *buf, uint32_t offset, int size, int is_read) {
    sid_t next_loca_sid;
    vdisk_t *vdisk;

    uint8_t *data;
    int tmp;

    vdisk = fs_get_vdisk(filesys, loca_sid.device);

    if (vdisk == NULL) {
        printf("d%ds%d not found\n", loca_sid.device, loca_sid.sector);
        return 1;
    }

    int index = -offset;
    while (index < size) {
        // check if sector is used
        if (!vdisk_is_sector_used(vdisk, loca_sid)) {
            printf("d%ds%d not used\n", loca_sid.device, loca_sid.sector);
            return 1;
        }

        // check if sector is locator
        data = vdisk_load_sector(vdisk, loca_sid);

        if (data[0] != ST_CONT || data[1] != SF_LOCA) {
            printf("d%ds%d not locator\n", loca_sid.device, loca_sid.sector);
            vdisk_unload_sector(vdisk, loca_sid, data, NO_SAVE);
            return 1;
        }

        if (index + LINKS_IN_LOCA < 0) {
            index += LINKS_IN_LOCA;
        } else {
            for (int i = 0; i < LINKS_IN_LOCA; i++) {
                if (index >= size) {
                    vdisk_unload_sector(vdisk, loca_sid, data, NO_SAVE);
                    return 0;
                }
                if (index + BYTE_IN_CORE < 0) {
                    index += BYTE_IN_CORE;
                    continue;
                }
                sid_t core_sid = *((sid_t *) (data + (i + 1) * sizeof(sid_t)));
                if (IS_NULL_SID(core_sid)) {
                    printf("no more core, but still %d bytes to %s\n", size - max(index, 0) , is_read ? "read" : "write");
                    vdisk_unload_sector(vdisk, loca_sid, data, NO_SAVE);
                    return 1;
                }
                tmp = fs_cnt_rw_core(filesys, core_sid, buf + max(index, 0), max(0, -index), size - max(index, 0), is_read);
                if (tmp == -1) {
                    printf("failed to %s core d%ds%d\n", is_read ? "read" : "write", core_sid.device, core_sid.sector);
                    vdisk_unload_sector(vdisk, loca_sid, data, NO_SAVE);
                    return 1;
                }
                index += tmp;
            }
        }
        next_loca_sid = *((sid_t *) (data + LAST_SID_OFFSET));
        if (IS_NULL_SID(next_loca_sid)) {
            printf("no more locator after d%ds%d\n", loca_sid.device, loca_sid.sector);
            vdisk_unload_sector(vdisk, loca_sid, data, NO_SAVE);
            return 1;
        }
        vdisk_unload_sector(vdisk, loca_sid, data, NO_SAVE);
        loca_sid = next_loca_sid;
    }
    return 0;
}

int fs_cnt_rw(filesys_t *filesys, sid_t head_sid, void *buf, uint32_t offset, uint32_t size, int is_read) {
    vdisk_t *vdisk;
    uint8_t *data;
    sid_t loca_sid;

    vdisk = fs_get_vdisk(filesys, head_sid.device);

    if (vdisk == NULL) {
        printf("d%ds%d not found\n", head_sid.device, head_sid.sector);
        return 1;
    }

    // check if sector is used
    if (!vdisk_is_sector_used(vdisk, head_sid)) {
        printf("d%ds%d not used\n", head_sid.device, head_sid.sector);
        return 1;
    }

    // check if sector is cnt header
    data = vdisk_load_sector(vdisk, head_sid);

    if (data[0] != ST_CONT || data[1] != SF_HEAD) {
        printf("d%ds%d not cnt header\n", head_sid.device, head_sid.sector);
        vdisk_unload_sector(vdisk, head_sid, data, NO_SAVE);
        return 1;
    }

    // check if offset+size is valid
    if (offset + size > *((uint32_t *) (data + 2 + META_MAXLEN))) {
        printf("cannot %s beyond cnt size\n", is_read ? "read" : "write");
        vdisk_unload_sector(vdisk, head_sid, data, NO_SAVE);
        return 1;
    }

    // rw locator
    loca_sid = *((sid_t *) (data + LAST_SID_OFFSET));
    if (loca_sid.device != 0 || loca_sid.sector != 0) {
        if (fs_cnt_rw_loca(filesys, loca_sid, (uint8_t *) buf, offset, (int) size, is_read)) {
            printf("? failed to %s locator d%ds%d\n", is_read ? "read" : "write", loca_sid.device, loca_sid.sector);
            vdisk_unload_sector(vdisk, head_sid, data, NO_SAVE);
            return 1;
        }
    }

    vdisk_unload_sector(vdisk, head_sid, data, NO_SAVE);
    return 0;
}
