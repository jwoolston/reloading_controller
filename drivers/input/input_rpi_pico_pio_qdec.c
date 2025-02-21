/*
 * Copyright 2025 Jared Woolston
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT raspberrypi_pico_qdec_pio

#include <stdint.h>
#include <stdlib.h>

#include <zephyr/device.h>
#include <zephyr/drivers/dma.h>
#include <zephyr/drivers/pinctrl.h>
#include <zephyr/input/input.h>
#include <zephyr/kernel.h>
#include <zephyr/pm/device.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/sys/util.h>

#include <zephyr/drivers/misc/pio_rpi_pico/pio_rpi_pico.h>

#include <hardware/dma.h>
#include <hardware/pio.h>
#include <hardware/clocks.h>

#include <zephyr/logging/log.h>

#include "zephyr/drivers/gpio.h"
LOG_MODULE_REGISTER(raspberrypi_pico_qdec_pio, CONFIG_INPUT_LOG_LEVEL);

#define DMA_MAX_TRANSFER_COUNT 0xFFFFFFFF
#define DMA_REFRESH_THRESHOLD  0x80000000

struct pio_qdec_config {
    const struct device *piodev;
    const struct pinctrl_dev_config *pcfg;
    struct gpio_dt_spec btn_pin;
    const uint32_t clk_pin;
    const uint32_t dt_pin;
    const uint16_t axis;
    const uint16_t button;
    const int max_step_rate;
};

struct pio_qdec_data {
    const struct device *dev;
    const struct device *dma_dev;
    struct dma_config dma_cfg;
    struct dma_block_config dma_block_cfg;
    struct gpio_callback button_cb_data;
    const uint8_t channel;
    const uint8_t slot;
    const uint8_t channel_config;
    size_t qdec_sm;
    volatile uint32_t qdec_count;
    volatile uint32_t qdec_count_prev;
    volatile uint32_t qdec_transfer_count;
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

static int pio_qdec_sm_init(PIO pio, uint32_t sm, uint32_t clk_pin, uint32_t dt_pin, int max_update_rate)
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
    if (max_update_rate == 0) {
        sm_config_set_clkdiv(&sm_config, 1.0f);
    } else {
        // one state machine loop takes at most 10 cycles
        float div = (float)clock_get_hz(clk_sys) / (10.0f * max_update_rate);
        sm_config_set_clkdiv(&sm_config, div);
    }

    sm_config_set_wrap(&sm_config,
                       offset + RPI_PICO_PIO_GET_WRAP_TARGET(qdec),
                       offset + RPI_PICO_PIO_GET_WRAP(qdec));

    pio_sm_init(pio, sm, offset, &sm_config);
    return 0;
}

static void dma_user_callback(const struct device *dev, void *user_data,
                              uint32_t channel, int status)
{
    const struct pio_qdec_config *config = dev->config;
    struct pio_qdec_data *data = dev->data;
    if (channel != data->channel) {
        return;
    }

    const int32_t delta = data->qdec_count - data->qdec_count_prev;
    data->qdec_count_prev = data->qdec_count;

    // Report the input event
    input_report_rel(data->dev, config->axis, delta, true, K_FOREVER);

    data->qdec_transfer_count++;
    if (data->qdec_transfer_count >= DMA_REFRESH_THRESHOLD) {
        // Restart the DMA with the original configuration
        dma_stop(data->dma_dev, data->channel);
        dma_config(data->dma_dev, data->channel, &data->dma_cfg);
        data->qdec_transfer_count = 0;
        dma_start(data->dma_dev, data->channel);
    }
}

void button_pressed(const struct device *dev, struct gpio_callback *cb,
                    uint32_t pins)
{
    const struct pio_qdec_config *config = dev->config;
    struct pio_qdec_data *data = dev->data;

    printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
    // Report the input event
    input_report_key(data->dev, config->button, 1, true, K_FOREVER);
}

static int pio_qdec_btn_init(const struct device *dev) {
    const struct pio_qdec_config *config = dev->config;
    struct pio_qdec_data *data = dev->data;

    if (!gpio_is_ready_dt(&config->btn_pin)) {
        printk("Error: button device %s is not ready\n",
               config->btn_pin.port->name);
        return -EAGAIN;
    }

    int retval = gpio_pin_configure_dt(&config->btn_pin, GPIO_INPUT);
    if (retval != 0) {
        printk("Error %d: failed to configure %s pin %d\n",
               retval, config->btn_pin.port->name, config->btn_pin.pin);
        return 0;
    }

    retval = gpio_pin_interrupt_configure_dt(&config->btn_pin,
                                          GPIO_INT_EDGE_TO_ACTIVE);
    if (retval != 0) {
        printk("Error %d: failed to configure interrupt on %s pin %d\n",
                retval, config->btn_pin.port->name, config->btn_pin.pin);
        return 0;
    }

    gpio_init_callback(&data->button_cb_data, button_pressed, BIT(config->btn_pin.pin));
    gpio_add_callback(config->btn_pin.port, &data->button_cb_data);
    printk("Set up button at %s pin %d\n", config->btn_pin.port->name, config->btn_pin.pin);

    return 0;
}

static int pio_qdec_init(const struct device *dev)
{
    const struct pio_qdec_config *config = dev->config;
    struct pio_qdec_data *data = dev->data;
    size_t qdec_sm;

    // Ensure the DMA is available
    if (data->dma_dev == NULL || !device_is_ready(data->dma_dev)) {
        return  -ENODEV;
    }

    if (dma_channel_is_claimed(data->channel)) {
        return -EBUSY;
    }

    PIO pio = pio_rpi_pico_get_pio(config->piodev);

    if (pio_rpi_pico_allocate_sm(config->piodev, &qdec_sm) != 0) {
        return -EBUSY;
    }

    data->qdec_sm = qdec_sm;

    if (pio_qdec_sm_init(pio, qdec_sm, config->clk_pin, config->dt_pin, config->max_step_rate) != 0) {
        return -EBUSY;
    }

    // Configure DMA to read the latest count from the state machine's RX FIFO and place it in the driver's data count
    data->dma_cfg.channel_direction = MEMORY_TO_MEMORY;
    data->dma_cfg.source_data_size = sizeof(uint32_t);
    data->dma_cfg.dest_data_size = sizeof(uint32_t);
    data->dma_cfg.source_burst_length = 1U;
    data->dma_cfg.dest_burst_length = 1U;
    data->dma_cfg.user_data = NULL;
    data->dma_cfg.dma_callback = dma_user_callback;
    data->dma_cfg.block_count = DMA_MAX_TRANSFER_COUNT;
    data->dma_cfg.head_block = &data->dma_block_cfg;
    data->dma_block_cfg.block_size = sizeof(uint32_t);
    data->dma_block_cfg.source_address = (uint32_t)&pio->rxf;
    data->dma_block_cfg.dest_address = (uint32_t)&data->qdec_count;
    data->qdec_transfer_count = 0;
    data->qdec_count = 0;
    data->qdec_count_prev = 0;

    int retval = dma_config(data->dma_dev, data->channel, &data->dma_cfg);
    if (retval < 0) {
        return retval;
    }

    retval = dma_start(data->dma_dev, data->channel);
    if (retval < 0) {
        return retval;
    }

    /*dma_channel_claim(config->channel);
    dma_channel_config cfg = dma_channel_get_default_config(config->channel);
    channel_config_set_read_increment(&cfg, false);
    channel_config_set_write_increment(&cfg, false);
    // This will pace the DMA transfers on the SM updates
    channel_config_set_dreq(&cfg, pio_get_dreq(pio, qdec_sm, false));
    dma_channel_configure(config->channel, &cfg,
                          &data->qdec_count,      // Destination
                          pio->rxf,               // Source
                          DMA_MAX_TRANSFER_COUNT, // Transfer count
                          true                    // Start immediately
        );*/

    // Initialize the state machine registers
    pio_sm_exec(pio, qdec_sm, pio_encode_set(pio_x, 0));
    // Initialize the Y register to the current value of the pins.
    pio_sm_exec(pio, qdec_sm, pio_encode_mov(pio_y, pio_pins));
    // Finally, start the state machine to start capturing ticks
    pio_sm_set_enabled(pio, qdec_sm, true);
    retval = pinctrl_apply_state(config->pcfg, PINCTRL_STATE_DEFAULT);
    if (retval < 0) {
        return retval;
    }

    return pio_qdec_btn_init(dev);
}

#define QDEC_PIO_INIT(idx)							        \
        PINCTRL_DT_INST_DEFINE(idx);	                                                \
        static const struct pio_qdec_config pio_qdec##idx##_config = {			\
            .piodev = DEVICE_DT_GET(DT_INST_PARENT(idx)),				\
            .pcfg = PINCTRL_DT_INST_DEV_CONFIG_GET(idx),				\
            .clk_pin = DT_INST_RPI_PICO_PIO_PIN_BY_NAME(idx, default, 0, clk_pin, 0),	\
            .dt_pin = DT_INST_RPI_PICO_PIO_PIN_BY_NAME(idx, default, 0, dt_pin, 0),	\
            .btn_pin = GPIO_DT_SPEC_INST_GET(idx, btn_gpios),                           \
            .max_step_rate = DT_INST_PROP_OR(idx, max_step_rate, 0),			\
            .axis = DT_INST_PROP(idx, zephyr_axis),			        	\
            .button = DT_INST_PROP(idx, zephyr_key),			        	\
        };										\
        static struct pio_qdec_data pio_qdec##idx##_data = {       			\
            .channel = DT_INST_DMAS_CELL_BY_IDX(idx, 0, channel),                       \
            .slot = DT_INST_DMAS_CELL_BY_IDX(idx, 0, slot),                             \
            .channel_config = DT_INST_DMAS_CELL_BY_IDX(idx, 0, channel_config)          \
        };                                                                              \
                                                                                        \
        PM_DEVICE_DT_INST_DEFINE(idx, pio_qdec_pm_action);			        \
                                                                                        \
        DEVICE_DT_INST_DEFINE(idx, pio_qdec_init, PM_DEVICE_DT_INST_GET(idx),	        \
                              &pio_qdec##idx##_data, &pio_qdec##idx##_config,		\
                              POST_KERNEL, CONFIG_INPUT_INIT_PRIORITY,		        \
                              NULL);

DT_INST_FOREACH_STATUS_OKAY(QDEC_PIO_INIT)