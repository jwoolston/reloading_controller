# Copyright (c) 2025 Jared Woolston
# SPDX-License-Identifier: Apache-2.0

# Out-of-tree drivers for existing driver classes
add_subdirectory_ifdef(CONFIG_INPUT input)
add_subdirectory_ifdef(CONFIG_MOTOR motor)
add_subdirectory_ifdef(CONFIG_SENSOR sensor)
