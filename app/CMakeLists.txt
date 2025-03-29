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
        src/filesystem/filesystem.c
        src/filesystem/filesystem.h
        src/filesystem/lv_fs_zephyr.c
        src/filesystem/lv_fs_zephyr_conf.h
        src/gui/gui.c
        src/gui/gui.h
        src/feeders/feeder_subsystem.c
        src/feeders/feeder_subsystem.h
        #src/gui/splash_screen.c
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
        src/gui/fonts/lv_font_Mostra_Nuova_24.c
        src/gui/fonts/lv_font_Mostra_Nuova_32.c
        #src/gui/images/images_md5.json
        #src/gui/images/lottie_running_gear_animation.c
)
