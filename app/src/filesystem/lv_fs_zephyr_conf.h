//
// Created by Jared Woolston on 3/13/2025.
//

#ifndef LV_FS_ZEPHYR_CONF_H
#define LV_FS_ZEPHYR_CONF_H

#include <zephyr/kernel.h>

#if CONFIG_LV_USE_FS_ZEPHYR
    #ifndef LV_FS_ZEPHYR_LETTER
        #ifdef CONFIG_LV_FS_ZEPHYR_LETTER
            #define LV_FS_ZEPHYR_LETTER CONFIG_LV_FS_ZEPHYR_LETTER
        #else
            #define LV_FS_ZEPHYR_LETTER '\0'     /**< Set an upper cased letter on which the drive will accessible (e.g. 'A') */
        #endif
    #endif
    #ifndef LV_FS_ZEPHYR_PATH
        #ifdef CONFIG_LV_FS_ZEPHYR_PATH
            #define LV_FS_ZEPHYR_PATH CONFIG_LV_FS_ZEPHYR_PATH
        #else
            #define LV_FS_ZEPHYR_PATH ""         /**< Set the working directory. File/directory paths will be appended to it. */
        #endif
    #endif
    #ifndef LV_FS_ZEPHYR_CACHE_SIZE
        #ifdef CONFIG_LV_FS_ZEPHYR_CACHE_SIZE
            #define LV_FS_ZEPHYR_CACHE_SIZE CONFIG_LV_FS_ZEPHYR_CACHE_SIZE
        #else
            #define LV_FS_ZEPHYR_CACHE_SIZE 0    /**< >0 to cache this number of bytes in lv_fs_read() */
        #endif
    #endif

void lv_fs_zephyr_init(void);
#endif

#endif //LV_FS_ZEPHYR_CONF_H
