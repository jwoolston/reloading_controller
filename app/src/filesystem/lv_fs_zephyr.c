/**
 * @file lv_fs_zephyr.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_fs_zephyr_conf.h"
#include <lvgl.h>
#include <stdlib.h>
#include <zephyr/fs/fs.h>
#include <core/lv_global.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(lv_fs_zephyr, CONFIG_FILESYSTEM_SUBSYSTEM_LOG_LEVEL);
/*********************
 *      DEFINES
 *********************/

#if !LV_FS_IS_VALID_LETTER(LV_FS_ZEPHYR_LETTER)
    #error "Invalid drive letter"
#endif

/**********************
 *      TYPEDEFS
 **********************/
typedef struct {
    struct fs_dir_t dir_p;
    char next_fn[LV_FS_MAX_PATH_LEN];
} dir_handle_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void* lv_fs_zephyr_open(lv_fs_drv_t* drv, const char* path, lv_fs_mode_t mode);
static lv_fs_res_t lv_fs_zephyr_close(lv_fs_drv_t* drv, void* file_p);
static lv_fs_res_t lv_fs_zephyr_read(lv_fs_drv_t* drv, void* file_p, void* buf, uint32_t btr, uint32_t* br);
static lv_fs_res_t lv_fs_zephyr_write(lv_fs_drv_t* drv, void* file_p, const void* buf, uint32_t btw, uint32_t* bw);
static lv_fs_res_t lv_fs_zephyr_seek(lv_fs_drv_t* drv, void* file_p, uint32_t pos, lv_fs_whence_t whence);
static lv_fs_res_t lv_fs_zephyr_tell(lv_fs_drv_t* drv, void* file_p, uint32_t* pos_p);
static void* lv_fs_zephyr_dir_open(lv_fs_drv_t* drv, const char* path);
static lv_fs_res_t lv_fs_zephyr_dir_read(lv_fs_drv_t* drv, void* dir_p, char* fn, uint32_t fn_len);
static lv_fs_res_t lv_fs_zephyr_dir_close(lv_fs_drv_t* drv, void* dir_p);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_fs_drv_t fs_drv_p;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Register a driver for the File system interface
 */
void lv_fs_zephyr_init(void) {
    /*---------------------------------------------------
     * Register the file system interface in LVGL
     *--------------------------------------------------*/

    lv_fs_drv_init(&fs_drv_p);

    /*Set up fields...*/
    fs_drv_p.letter = LV_FS_ZEPHYR_LETTER;
    fs_drv_p.cache_size = LV_FS_ZEPHYR_CACHE_SIZE;

    fs_drv_p.open_cb = lv_fs_zephyr_open;
    fs_drv_p.close_cb = lv_fs_zephyr_close;
    fs_drv_p.read_cb = lv_fs_zephyr_read;
    fs_drv_p.write_cb = lv_fs_zephyr_write;
    fs_drv_p.seek_cb = lv_fs_zephyr_seek;
    fs_drv_p.tell_cb = lv_fs_zephyr_tell;

    fs_drv_p.dir_close_cb = lv_fs_zephyr_dir_close;
    fs_drv_p.dir_open_cb = lv_fs_zephyr_dir_open;
    fs_drv_p.dir_read_cb = lv_fs_zephyr_dir_read;

    lv_fs_drv_register(&fs_drv_p);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Open a file
 * @param drv   pointer to a driver where this function belongs
 * @param path  path to the file beginning with the driver letter (e.g. S:/folder/file.txt)
 * @param mode  read: FS_MODE_RD, write: FS_MODE_WR, both: FS_MODE_RD | FS_MODE_WR
 * @return pointer to FIL struct or NULL in case of fail
 */
static void* lv_fs_zephyr_open(lv_fs_drv_t* drv, const char* path, lv_fs_mode_t mode) {
    LV_UNUSED(drv);

    fs_mode_t flags = 0;

    if (mode == LV_FS_MODE_WR) {
        flags = FS_O_WRITE;
    } else if (mode == LV_FS_MODE_RD) {
        flags = FS_O_READ;
    } else if (mode == (LV_FS_MODE_WR | LV_FS_MODE_RD)) {
        flags = FS_O_RDWR | FS_O_APPEND;
    }

    /*Make the path relative to the current directory (the projects root folder)*/

    char buf[LV_FS_MAX_PATH_LEN];
    lv_snprintf(buf, sizeof(buf), CONFIG_LV_FS_ZEPHYR_PATH "%s", path);

    struct fs_file_t* file = lv_malloc(sizeof(struct fs_file_t));
    if (!file) {
        LOG_ERR("Failed to allocate file handle");
        return NULL;
    }
    LOG_DBG("Opening file '%s'", buf);
    if (fs_open(file, buf, flags)) {
        LOG_ERR("Failed to open file %s", buf);
        return NULL;
    }
    return file;
}

/**
 * Close an opened file
 * @param drv       pointer to a driver where this function belongs
 * @param file_p    pointer to a FILE variable. (opened with fs_open)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t lv_fs_zephyr_close(lv_fs_drv_t* drv, void* file_p) {
    LV_UNUSED(drv);
    const int err = fs_close(file_p);
    free(file_p);
    if (err) {
        LOG_ERR("Failed to close file. Error: %d", err);
        return LV_FS_RES_FS_ERR;
    }
    return LV_FS_RES_OK;
}

/**
 * Read data from an opened file
 * @param drv       pointer to a driver where this function belongs
 * @param file_p    pointer to a FILE variable.
 * @param buf       pointer to a memory block where to store the read data
 * @param btr       number of Bytes To Read
 * @param br        the real number of read bytes (Byte Read)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t lv_fs_zephyr_read(lv_fs_drv_t* drv, void* file_p, void* buf, uint32_t btr, uint32_t* br) {
    LV_UNUSED(drv);
    *br = fs_read(file_p, buf, btr);
    if (*br) {
        LOG_ERR("Failed to read file. Error %d", *br);
    }
    return (int32_t)(*br) < 0 ? LV_FS_RES_UNKNOWN : LV_FS_RES_OK;
}

/**
 * Write into a file
 * @param drv       pointer to a driver where this function belongs
 * @param file_p    pointer to a FILE variable
 * @param buf       pointer to a buffer with the bytes to write
 * @param btw       Bytes To Write
 * @param bw        the number of real written bytes (Bytes Written). NULL if unused.
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t lv_fs_zephyr_write(lv_fs_drv_t* drv, void* file_p, const void* buf, uint32_t btw, uint32_t* bw) {
    LV_UNUSED(drv);
    *bw = fs_write(file_p, buf, btw);
    if (*bw) {
        LOG_ERR("Failed to write file. Error %d", *bw);
    }
    return (int32_t)(*bw) < 0 ? LV_FS_RES_UNKNOWN : LV_FS_RES_OK;
}

/**
 * Set the read write pointer. Also expand the file size if necessary.
 * @param drv       pointer to a driver where this function belongs
 * @param file_p    pointer to a FILE variable. (opened with fs_open )
 * @param pos       the new position of read write pointer
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t lv_fs_zephyr_seek(lv_fs_drv_t* drv, void* file_p, uint32_t pos, lv_fs_whence_t whence) {
    LV_UNUSED(drv);
    int w;
    switch (whence) {
        case LV_FS_SEEK_SET:
            w = FS_SEEK_SET;
            break;
        case LV_FS_SEEK_CUR:
            w = FS_SEEK_CUR;
            break;
        case LV_FS_SEEK_END:
            w = FS_SEEK_END;
            break;
        default:
            return LV_FS_RES_INV_PARAM;
    }

    const int err = fs_seek(file_p, pos, w);
    if (err) {
        LOG_ERR("Failed to seek file. Error %d", err);
    }
    return LV_FS_RES_OK;
}

/**
 * Give the position of the read write pointer
 * @param drv       pointer to a driver where this function belongs
 * @param file_p    pointer to a FILE variable
 * @param pos_p     pointer to store the result
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t lv_fs_zephyr_tell(lv_fs_drv_t* drv, void* file_p, uint32_t* pos_p) {
    LV_UNUSED(drv);
    *pos_p = ftell(file_p);
    return LV_FS_RES_OK;
}

/**
 * Initialize a 'DIR' or 'HANDLE' variable for directory reading
 * @param drv   pointer to a driver where this function belongs
 * @param path  path to a directory
 * @return pointer to an initialized 'DIR' or 'HANDLE' variable
 */
static void* lv_fs_zephyr_dir_open(lv_fs_drv_t* drv, const char* path) {
    LV_UNUSED(drv);
    struct fs_dir_t* handle = lv_malloc(sizeof(struct fs_dir_t));
    /*Make the path relative to the current directory (the projects root folder)*/
    char buf[LV_FS_MAX_PATH_LEN];
    lv_snprintf(buf, sizeof(buf), LV_FS_ZEPHYR_PATH "%s", path);
    const int err = fs_opendir(handle, buf);
    if (err) {
        LOG_ERR("Failed to open directory %s. Error: %d", buf, err);
        lv_free(handle);
        return NULL;
    }
    return handle;
}

/**
 * Read the next filename form a directory.
 * The name of the directories will begin with '/'
 * @param drv       pointer to a driver where this function belongs
 * @param dir_p     pointer to an initialized 'DIR' or 'HANDLE' variable
 * @param fn        pointer to a buffer to store the filename
 * @param fn_len    length of the buffer to store the filename
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t lv_fs_zephyr_dir_read(lv_fs_drv_t* drv, void* dir_p, char* fn, uint32_t fn_len) {
    LV_UNUSED(drv);
    if (fn_len == 0) {
        return LV_FS_RES_INV_PARAM;
    }

    struct fs_dir_t* handle = (struct fs_dir_t*)dir_p;
    struct fs_dirent entry;
    do {
        const int err = fs_readdir(handle, &entry);
        if (err < 0) {
            LOG_ERR("Failed to read directory %s. Error %d", fn, err);
        } else if (err || entry.name[0] == 0) {
            lv_strlcpy(fn, "", fn_len);
        } else {
            if (entry.type == FS_DIR_ENTRY_DIR) {
                lv_snprintf(fn, fn_len, "/%s", entry.name);
            } else {
                lv_strlcpy(fn, entry.name, fn_len);
            }
        }
    } while (lv_strcmp(fn, "/.") == 0 || lv_strcmp(fn, "/..") == 0);
    return LV_FS_RES_OK;
}

/**
 * Close the directory reading
 * @param drv   pointer to a driver where this function belongs
 * @param dir_p pointer to an initialized 'DIR' or 'HANDLE' variable
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t lv_fs_zephyr_dir_close(lv_fs_drv_t* drv, void* dir_p) {
    LV_UNUSED(drv);
    struct fs_dir_t* handle = (struct fs_dir_t*)dir_p;
    const int err = fs_closedir(handle);
    if (err) {
        LOG_ERR("Failed to close directory. Error: %d", err);
    }
    lv_free(handle);
    return LV_FS_RES_OK;
}