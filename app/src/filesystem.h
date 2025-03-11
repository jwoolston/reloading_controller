//
// Created by Jared Woolston on 3/10/2025.
//

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/storage/disk_access.h>
#include <zephyr/logging/log.h>
#include <zephyr/fs/fs.h>

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

#elif defined(CONFIG_FILE_SYSTEM_EXT2)

#include <zephyr/fs/ext2.h>

#define DISK_DRIVE_NAME "SD"
#define DISK_MOUNT_PT "/ext"

#endif

#if defined(CONFIG_FAT_FILESYSTEM_ELM)
#define FS_RET_OK FR_OK
#else
#define FS_RET_OK 0
#endif

int mount_filesystem(void);

int unmount_filesystem(void);

#endif //FILESYSTEM_H
