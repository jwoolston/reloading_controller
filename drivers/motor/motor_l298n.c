/*
* Copyright 2025 Jared Woolston
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT motor_l298n

#include <stdint.h>
#include <stdlib.h>

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(motor_l298n, CONFIG_MOTOR_LOG_LEVEL);

struct motor_l298n_channel {
    const struct pwm_dt_spec *enable_pin;
    const struct gpio_dt_spec dir_pins[2];
};

struct motor_l298n_channel_state {
    bool enabled; // Will default to false
    bool forward; // Will default to true
    uint32_t pulse_width; // 100% on when this value is equal to the device
    // tree configured period
};

struct motor_l298n_config {
    const struct device *motor_dev;
    const struct gpio_dt_spec *ch1;
    const struct gpio_dt_spec *ch2; // If NULL, this channel is not enabled
};

struct motor_l298n_data {
    struct motor_l298n_channel_state *ch1;
    struct motor_l298n_channel_state *ch2; // If NULL, this channel is not enabled
};

#define MOTOR_L298N_CONFIGURE_CHANNEL(idx, channel)                           \
    {                                                                         \
        .enable_pin = PWM_DT_SPEC_INST_GET_BY_IDX_OR(idx, channel, NULL),     \
        .dir_pins = {                                                         \
            GPIO_DT_SPEC_INST_GET_BY_IDX_OR(idx, ch_1_dir_gpios, 0, NULL),    \
            GPIO_DT_SPEC_INST_GET_BY_IDX_OR(idx, ch_1_dir_gpios, 0, NULL)     \
        }                                                                     \
    }

#define MOTOR_L298N_INIT(idx)						            \
        static const struct motor_l298n_config motor_l298n##idx##_config = {        \
            .motordev = DEVICE_DT_GET(DT_INST_PARENT(idx)),		            \
            .ch1 = MOTOR_L298N_CONFIGURE_CHANNEL(idx, 0),                           \
            .ch2 = MOTOR_L298N_CONFIGURE_CHANNEL(idx, 1)                            \
        };								            \
                                                                                    \
        static struct motor_l298n_data motor_l298n##idx##_data = {                  \
            .ch1 = {                                                                \
                .enabled = false,                                                   \
                .forward = true,                                                    \
                .pulse_width = 0,                                                   \
            },                                                                      \
            .ch2 = {                                                                \
                .enabled = false,                                                   \
                .forward = true,                                                    \
                .pulse_width = 0,                                                   \
            }                                                                       \
        };                                                                          \
                                                                                    \
        DEVICE_DT_INST_DEFINE(idx, motor_l298n_init, PM_DEVICE_DT_INST_GET(idx),    \
                              &motor_l298n##idx##_data, &motor_l298n##idx##_config, \
                              POST_KERNEL, CONFIG_MOTOR_INIT_PRIORITY, NULL);

DT_INST_FOREACH_STATUS_OKAY(MOTOR_L298N_INIT)