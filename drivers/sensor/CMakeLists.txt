# Copyright (c) 2025 Jared Woolston
# SPDX-License-Identifier: Apache-2.0

add_subdirectory_ifdef(CONFIG_EXAMPLE_SENSOR example_sensor)
add_subdirectory_ifdef(CONFIG_QMI8658C qmi8658c)