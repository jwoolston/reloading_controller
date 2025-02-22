# Copyright (c) 2025 Jared Woolston
# SPDX-License-Identifier: Apache-2.0

zephyr_library()
zephyr_library_sources_ifdef(CONFIG_MOTOR_L298N motor_l298n.c)