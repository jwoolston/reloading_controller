#-------------------------------------------------------------------------------
# Zephyr Reloading Controller
#
# Copyright (c) 2025 Jared Woolston
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(app LANGUAGES C)

set(LVGL_DIR ${ZEPHYR_LVGL_MODULE_DIR})

target_sources(app PRIVATE
        src/main.c
        src/gui.c
        src/gui.h
        src/gui/events_init.c
        src/gui/events_init.h
        src/gui/gui_guider.c
        src/gui/gui_guider.h
        src/gui/setup_scr_home_scr.c
        src/gui/setup_scr_run_scr.c
        src/gui/setup_scr_settings_scr.c
        src/gui/setup_scr_splash_scr.c
        src/gui/widgets_init.c
        src/gui/widgets_init.h
        src/gui/fonts/lv_font_montserratMedium_16.c
        src/gui/fonts/lv_font_Mostra_Nuova_24.c
        #src/gui/fonts/lv_font_Mostra_Nuova_32.c
        #src/gui/images/_AP_Logo_RGB565_480x320.c
        #src/gui/images/_insights_128dp_EFE8E4_RGB565_140x140.c
        #src/gui/images/_play_circle_outline_128dp_EFE8E4_RGB565_140x140.c
        #src/gui/images/_settings_128dp_EFE8E4_RGB565_140x140.c
        #src/gui/images/_warning_64dp_EFE8E4_RGB565_24x24.c
        #src/gui/images/images_md5.json
        #src/gui/images/lottie_running_gear_animation.c
)
