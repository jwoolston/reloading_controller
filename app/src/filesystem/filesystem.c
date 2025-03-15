//
// Created by Jared Woolston on 3/10/2025.
//

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/storage/disk_access.h>
#include <zephyr/fs/fs.h>
#include "lv_fs_zephyr_conf.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(filesystem, CONFIG_FILESYSTEM_SUBSYSTEM_LOG_LEVEL);

#if defined(CONFIG_FAT_FILESYSTEM_ELM)

#include <ff.h>

/*
 *  Note the fatfs library is able to mount only strings inside _VOLUME_STRS
 *  in ffconf.h
 */
#if defined(CONFIG_DISK_DRIVER_MMC)
#define DISK_DRIVE_NAME "SD2"
#else
#define DISK_DRIVE_NAME "SD"
#endif

#define DISK_MOUNT_PT "/"DISK_DRIVE_NAME":"

static FATFS fat_fs;
/* mounting info */
static struct fs_mount_t mp = {
    .type = FS_FATFS,
    .fs_data = &fat_fs,
};

#elif defined(CONFIG_FILE_SYSTEM_EXT2)

#include <zephyr/fs/ext2.h>

#define DISK_DRIVE_NAME "SD"
#define DISK_MOUNT_PT "/ext"

static struct fs_mount_t mp = {
    .type = FS_EXT2,
    .flags = FS_MOUNT_FLAG_NO_FORMAT,
    .storage_dev = (void *)DISK_DRIVE_NAME,
    .mnt_point = "/ext",
};

#endif

#if defined(CONFIG_FAT_FILESYSTEM_ELM)
#define FS_RET_OK FR_OK
#else
#define FS_RET_OK 0
#endif

#define MAX_PATH 128
#define SOME_FILE_NAME "some.dat"
#define SOME_DIR_NAME "some"
#define SOME_REQUIRED_LEN MAX(sizeof(SOME_FILE_NAME), sizeof(SOME_DIR_NAME))

static const char *disk_mount_pt = DISK_MOUNT_PT;

/* List dir entry by path
 *
 * @param path Absolute path to list
 *
 * @return Negative errno code on error, number of listed entries on
 *         success.
 */
static int lsdir(const char *path)
{
    int res;
    struct fs_dir_t dirp;
    static struct fs_dirent entry;
    int count = 0;

    fs_dir_t_init(&dirp);

    /* Verify fs_opendir() */
    res = fs_opendir(&dirp, path);
    if (res) {
        LOG_ERR("Error opening dir %s [%d]", path, res);
        return res;
    }

    LOG_DBG("Listing dir %s ...", path);
    for (;;) {
        // Verify fs_readdir()
        res = fs_readdir(&dirp, &entry);

        // entry.name[0] == 0 means end-of-dir
        if (res || entry.name[0] == 0) {
            break;
        }

        if (entry.type == FS_DIR_ENTRY_DIR) {
            LOG_DBG("[DIR ] %s", entry.name);
            char new_path[MAX_PATH];
            int base = strlen(path);
            strncpy(new_path, path, sizeof(new_path));
            new_path[base++] = '/';
            new_path[base] = 0;
            strcat(&new_path[base], entry.name);
            LOG_DBG("[Attempting to list DIR ] %s", new_path);
            lsdir(new_path);
        } else {
            LOG_DBG("[FILE] %s (size = %zu)", entry.name, entry.size);
        }
        count++;
    }

    // Verify fs_closedir()
    fs_closedir(&dirp);
    if (res == 0) {
        res = count;
    }

    return res;
}

int mount_filesystem(void) {
    /* raw disk i/o */
    do {
        static const char* disk_pdrv = DISK_DRIVE_NAME;
        uint64_t memory_size_mb;
        uint32_t block_count;
        uint32_t block_size;

        if (disk_access_ioctl(disk_pdrv,
                              DISK_IOCTL_CTRL_INIT, NULL) != 0) {
            LOG_ERR("Storage init ERROR!");
            break;
        }

        if (disk_access_ioctl(disk_pdrv,
                              DISK_IOCTL_GET_SECTOR_COUNT, &block_count)) {
            LOG_ERR("Unable to get sector count");
            break;
        }
        LOG_INF("Block count %u", block_count);

        if (disk_access_ioctl(disk_pdrv,
                              DISK_IOCTL_GET_SECTOR_SIZE, &block_size)) {
            LOG_ERR("Unable to get sector size");
            break;
        }
        printk("Sector size %u\n", block_size);

        memory_size_mb = (uint64_t)block_count * block_size;
        printk("Memory Size(MB) %u\n", (uint32_t)(memory_size_mb >> 20));

        if (disk_access_ioctl(disk_pdrv,
                              DISK_IOCTL_CTRL_DEINIT, NULL) != 0) {
            LOG_ERR("Storage deinit ERROR!");
            break;
        }
    } while (0);

    mp.mnt_point = disk_mount_pt;

    const int res = fs_mount(&mp);

    if (res == FS_RET_OK) {
        LOG_DBG("Disk mounted.");
#ifdef CONFIG_LV_USE_FS_ZEPHYR
        LOG_DBG("Initializing LVGL Zephyr FS driver.");
        lv_fs_zephyr_init();
#endif
    } else {
        LOG_ERR("Error mounting disk: %d", res);
        return res;
    }

    //lsdir(disk_mount_pt);

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