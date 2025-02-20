/*
 * Copyright 2025 Jared Woolston
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT raspberrypi_pico_qdec_pio

#include <stdint.h>
#include <stdlib.h>

#include <zephyr/device.h>
#include <zephyr/drivers/pinctrl.h>
#include <zephyr/input/input.h>
#include <zephyr/kernel.h>
#include <zephyr/pm/device.h>
#include <zephyr/pm/device_runtime.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/sys/util.h>

#include <zephyr/drivers/misc/pio_rpi_pico/pio_rpi_pico.h>

#include <hardware/pio.h>
#include <hardware/clocks.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(raspberrypi_pico_qdec_pio, CONFIG_INPUT_LOG_LEVEL);

struct pio_qdec_config {
    const struct device *piodev;
    const struct pinctrl_dev_config *pcfg;
    const uint32_t clk_pin;
    const uint32_t dt_pin;
    const uint32_t max_step_rate;
    const uint16_t axis;
};

struct pio_qdec_data {
    const struct device *dev;
    size_t qdec_sm;
    struct k_timer sample_timer;
    uint8_t prev_step;
    int32_t acc;
    struct k_work event_work;
    struct k_work_delayable idle_work;
    //struct gpio_callback gpio_cb;
    atomic_t polling;
#ifdef CONFIG_PM_DEVICE
    atomic_t suspended;
#endif
};

/*
 The following PIO program is the unmodified output of picoasm from the
 pico-sdk PIO Quadrature Encoder sample. Its original copyright notice
 is produced below.

 Copyright (c) 2023 Raspberry Pi (Trading) Ltd.

 SPDX-License-Identifier: BSD-3-Clause
 */
RPI_PICO_PIO_DEFINE_PROGRAM(qdec, 15, 23,
    0x000f, //  0: jmp    15
    0x000e, //  1: jmp    14
    0x0015, //  2: jmp    21
    0x000f, //  3: jmp    15
    0x0015, //  4: jmp    21
    0x000f, //  5: jmp    15
    0x000f, //  6: jmp    15
    0x000e, //  7: jmp    14
    0x000e, //  8: jmp    14
    0x000f, //  9: jmp    15
    0x000f, // 10: jmp    15
    0x0015, // 11: jmp    21
    0x000f, // 12: jmp    15
    0x0015, // 13: jmp    21
    0x008f, // 14: jmp    y--, 15
            //     .wrap_target
    0xa0c2, // 15: mov    isr, y
    0x8000, // 16: push   noblock
    0x60c2, // 17: out    isr, 2
    0x4002, // 18: in     pins, 2
    0xa0e6, // 19: mov    osr, isr
    0xa0a6, // 20: mov    pc, isr
    0xa04a, // 21: mov    y, !y
    0x0097, // 22: jmp    y--, 23
    0xa04a, // 23: mov    y, !y
            //     .wrap
);

static int pio_qdec_sm_init(PIO pio, uint32_t sm, uint32_t clk_pin, uint32_t dt_pin, int max_step_rate)
{
    pio_sm_config sm_config;
    uint32_t offset;

    if (!pio_can_add_program(pio, RPI_PICO_PIO_GET_PROGRAM(qdec))) {
        return -EBUSY;
    }

    offset = pio_add_program(pio, RPI_PICO_PIO_GET_PROGRAM(qdec));

    pio_sm_set_consecutive_pindirs(pio, sm, clk_pin, 1, false);
    pio_sm_set_consecutive_pindirs(pio, sm, dt_pin, 1, false);
    pio_gpio_init(pio, clk_pin);
    pio_gpio_init(pio, dt_pin);
    gpio_pull_up(clk_pin);
    gpio_pull_up(dt_pin);
    sm_config = pio_get_default_sm_config();
    sm_config_set_in_pins(&sm_config, clk_pin); // for WAIT, IN
    sm_config_set_jmp_pin(&sm_config, clk_pin); // for JMP
    // shift to left, autopull disabled
    sm_config_set_in_shift(&sm_config, false, false, 32);
    // don't join FIFO's
    sm_config_set_fifo_join(&sm_config, PIO_FIFO_JOIN_NONE);
    // passing "0" as the sample frequency,
    if (max_step_rate == 0) {
        sm_config_set_clkdiv(&sm_config, 1.0);
    } else {
        // one state machine loop takes at most 10 cycles
        float div = (float)clock_get_hz(clk_sys) / (10 * max_step_rate);
        sm_config_set_clkdiv(&sm_config, div);
    }

    sm_config_set_wrap(&sm_config,
                       offset + RPI_PICO_PIO_GET_WRAP_TARGET(qdec),
                       offset + RPI_PICO_PIO_GET_WRAP(qdec));

    pio_sm_init(pio, sm, offset, &sm_config);
    pio_sm_set_enabled(pio, sm, true);

    return 0;
}

static int pio_qdec_init(const struct device *dev)
{
    const struct pio_qdec_config *config = dev->config;
    struct pio_qdec_data *data = dev->data;
    size_t qdec_sm;
    int retval;
    PIO pio;

    pio = pio_rpi_pico_get_pio(config->piodev);

    retval = pio_rpi_pico_allocate_sm(config->piodev, &qdec_sm);

    if (retval < 0) {
        return retval;
    }

    data->qdec_sm = qdec_sm;

    retval = pio_qdec_sm_init(pio, qdec_sm, config->clk_pin, config->dt_pin, config->max_step_rate);
    if (retval < 0) {
        return retval;
    }

    return pinctrl_apply_state(config->pcfg, PINCTRL_STATE_DEFAULT);
}

#define QDEC_PIO_INIT(idx)							        \
        PINCTRL_DT_INST_DEFINE(idx);	                                                \
        static const struct pio_qdec_config pio_qdec##idx##_config = {			\
            .piodev = DEVICE_DT_GET(DT_INST_PARENT(idx)),				\
            .pcfg = PINCTRL_DT_INST_DEV_CONFIG_GET(idx),				\
            .clk_pin = DT_INST_RPI_PICO_PIO_PIN_BY_NAME(idx, default, 0, clk_pin, 0),	\
            .dt_pin = DT_INST_RPI_PICO_PIO_PIN_BY_NAME(idx, default, 0, dt_pin, 0),	\
            .max_step_rate = DT_INST_PROP_OR(idx, max_step_rate, 0),			\
            .axis = DT_INST_PROP(idx, zephyr_axis),			        	\
        };										\
        static struct pio_qdec_data pio_qdec##idx##_data;				\
                                                                                        \
        DEVICE_DT_INST_DEFINE(idx, pio_qdec_init, PM_DEVICE_DT_INST_GET(idx),	        \
                              &pio_qdec##idx##_data, &pio_qdec##idx##_config,		\
                              POST_KERNEL, CONFIG_INPUT_INIT_PRIORITY,		        \
                              NULL);

DT_INST_FOREACH_STATUS_OKAY(QDEC_PIO_INIT)