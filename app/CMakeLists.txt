#-------------------------------------------------------------------------------
# Zephyr Reloading Controller
#
# Copyright (c) 2025 Jared Woolston
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(app LANGUAGES C)

set(LVGL_DIR ${ZEPHYR_LVGL_MODULE_DIR})

target_sources(app PRIVATE src/main.c)
