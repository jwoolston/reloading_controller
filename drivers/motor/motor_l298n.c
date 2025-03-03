/*
* Copyright 2025 Jared Woolston
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT motor_l298n

#include <stdint.h>
#include <stdlib.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/logging/log.h>

#include <app/drivers/motor.h>

#if DT_NUM_INST_STATUS_OKAY(DT_DRV_COMPAT) == 0
#error"L298N Motor controller is not defined in DTS"
#endif

LOG_MODULE_REGISTER(motor_l298n, CONFIG_MOTOR_LOG_LEVEL);

struct motor_l298n_channel {
    const struct pwm_dt_spec enable_pin;
    const struct gpio_dt_spec dir_pins[2];
};

struct motor_l298n_channel_state {
    bool enabled; // Will default to false
    bool forward; // Will default to true
    uint32_t pulse_width; // 100% on when this value is equal to the device
    // tree configured period
};

struct motor_l298n_config {
    //const struct device *motor_dev;
    const struct motor_l298n_channel ch1;
    const struct motor_l298n_channel ch2; // If NULL, this channel is not enabled
};

struct motor_l298n_data {
    struct motor_l298n_channel_state ch1;
    struct motor_l298n_channel_state ch2; // If NULL, this channel is not enabled
};

static int motor_l298n_set_period_ns(const struct device *dev, unsigned int period_ns) {
    const struct motor_l298n_config *config = dev->config;
    struct motor_l298n_data *data = dev->data;

    LOG_DBG("Setting motor period to : %u", period_ns);

    /*int ret = 0;
    if (config->ch1 != NULL) {
        uint64_t pulse_ns;
        ret = pwm_cycles_to_nsec(config->ch1->enable_pin.dev, config->ch1->enable_pin.channel,
                                 data->ch1.pulse_width, &pulse_ns);
        if (ret < 0) {
            LOG_ERR("pwm_cycles_to_nsec failed (%d)", ret);
            return ret;
        }
        pwm_set_dt(&config->ch1->enable_pin, period_ns, (uint32_t) pulse_ns);
    }*/

    return 0;
}

static DEVICE_API(motor, motor_api) = {
    .set_period_ns = &motor_l298n_set_period_ns,
};

static int motor_l298n_init(const struct device *dev)
{
    const struct motor_l298n_config *config = dev->config;
    struct motor_l298n_data *data = dev->data;

    LOG_DBG("Initializing L298N Motor Driver");

    return 0;
}

/**
 * Create a channel instance or return NULL based on the existance of the channel
 * in the devicetree.
 *
 * @param idx The instance index of the Motor L298N driver.
 * @param channel The channel being configured, 0 based.
 * @return A configured motor_l298n_channel struct or NULL
 */
#define MOTOR_L298N_CONFIGURE_CHANNEL(idx, channel)                                 \
    COND_CODE_1(DT_INST_PROP_HAS_IDX(idx, pwms, channel),                           \
    ({                                                                              \
        .enable_pin = PWM_DT_SPEC_INST_GET_BY_IDX_OR(idx, channel, NULL),           \
        .dir_pins = {                                                               \
            GPIO_DT_SPEC_INST_GET_BY_IDX_OR(idx, ch_##channel##_dir_gpios, 0, NULL),\
            GPIO_DT_SPEC_INST_GET_BY_IDX_OR(idx, ch_##channel##_dir_gpios, 1, NULL) \
        }                                                                           \
    }), NULL)                                                                       \

#define MOTOR_L298N_INIT(idx)						            \
        static const struct motor_l298n_config motor_l298n##idx##_config = {        \
            .ch1 = MOTOR_L298N_CONFIGURE_CHANNEL(idx, 0),                           \
            .ch2 = MOTOR_L298N_CONFIGURE_CHANNEL(idx, 1)                            \
        };								            \
                                                                                    \
        static struct motor_l298n_data motor_l298n##idx##_data = {                  \
            .ch1 = {                                                                \
                .enabled = false,                                                   \
                .forward = true,                                                    \
                .pulse_width = 0                                                    \
            },                                                                      \
            .ch2 = {                                                                \
                .enabled = false,                                                   \
                .forward = true,                                                    \
                .pulse_width = 0                                                    \
            }                                                                       \
        };                                                                          \
                                                                                    \
        DEVICE_DT_INST_DEFINE(idx, motor_l298n_init, NULL,                          \
                              &motor_l298n##idx##_data, &motor_l298n##idx##_config, \
                              POST_KERNEL, CONFIG_MOTOR_INIT_PRIORITY, NULL);

DT_INST_FOREACH_STATUS_OKAY(MOTOR_L298N_INIT)
