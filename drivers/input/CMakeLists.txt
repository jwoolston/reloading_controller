# Copyright (c) 2025 Jared Woolston
# SPDX-License-Identifier: Apache-2.0

zephyr_library()
zephyr_library_sources_ifdef(CONFIG_INPUT_RPI_PICO_PIO_QDEC input_rpi_pico_pio_qdec.c)