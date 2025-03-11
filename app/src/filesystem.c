//
// Created by Jared Woolston on 3/10/2025.
//

#include "filesystem.h"

LOG_MODULE_REGISTER(filesystem);

#if defined(CONFIG_FAT_FILESYSTEM_ELM)

static FATFS fat_fs;
/* mounting info */
static struct fs_mount_t mp = {
    .type = FS_FATFS,
    .fs_data = &fat_fs,
};

#elif defined(CONFIG_FILE_SYSTEM_EXT2)

static struct fs_mount_t mp = {
    .type = FS_EXT2,
    .flags = FS_MOUNT_FLAG_NO_FORMAT,
    .storage_dev = (void *)DISK_DRIVE_NAME,
    .mnt_point = "/ext",
};

#endif

static const char* disk_mount_pt = DISK_MOUNT_PT;

int mount_filesystem(void) {
    mp.mnt_point = disk_mount_pt;

    const int res = fs_mount(&mp);

    if (res == FS_RET_OK) {
        LOG_DBG("Disk mounted.");
    } else {
        LOG_ERR("Error mounting disk: %d", res);
    }

    return res;
}

int unmount_filesystem(void) {
    const int res = fs_unmount(&mp);
    if (res != FS_RET_OK) {
        LOG_DBG("Error unmounting disk");
        return res;
    }
    return 0;
}