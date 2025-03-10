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
    bool enabled;                  // Will default to false
    enum MotorDirection direction; // Will default to FORWARD
    uint32_t pulse_width;          // 100% on when this value is equal to the device
    // tree configured period
};

struct motor_l298n_config {
    //const struct device *motor_dev;
    const struct motor_l298n_channel ch0;
    const struct motor_l298n_channel ch1; // If NULL, this channel is not enabled
};

struct motor_l298n_data {
    struct motor_l298n_channel_state ch0;
    struct motor_l298n_channel_state ch1; // If NULL, this channel is not enabled
};

static int motor_l298n_get_channel_count(const struct device* dev) {
    return 2; //TODO(jwoolston): Make this dynamic
}

static int motor_l298n_channel_set_speed(const struct device* dev, const unsigned int channel, const unsigned int speed) {
    const struct motor_l298n_config* config = dev->config;
    struct motor_l298n_data* data = dev->data;

    LOG_DBG("Setting channel %u motor speed to : %u%%", channel, speed);
    switch (channel) {
        case 0:
            data->ch0.pulse_width = (speed / 100.0f) * config->ch0.enable_pin.period;
            if (data->ch0.enabled) {
                return pwm_set_pulse_dt(&config->ch0.enable_pin, data->ch0.pulse_width);
            }
            return 0;
        case 1:
            data->ch1.pulse_width = (speed / 100.0f) * config->ch1.enable_pin.period;
            if (data->ch1.enabled) {
                return pwm_set_pulse_dt(&config->ch1.enable_pin, data->ch1.pulse_width);
            }
            return 0;
        default:
            LOG_ERR("Invalid motor channel %u", channel);
            return -EINVAL;
    }
}

static int motor_l298n_channel_on(const struct device* dev, const uint32_t channel) {
    const struct motor_l298n_config* config = dev->config;
    struct motor_l298n_data* data = dev->data;

    LOG_DBG("Turning motor channel %d on", channel);
    switch (channel) {
        case 0:
            data->ch0.enabled = true;
            return pwm_set_pulse_dt(&config->ch0.enable_pin, data->ch0.pulse_width);
        case 1:
            data->ch1.enabled = true;
            return pwm_set_pulse_dt(&config->ch1.enable_pin, data->ch1.pulse_width);
        default:
            LOG_ERR("Invalid motor channel %u", channel);
            return -EINVAL;
    }
}

static int motor_l298n_channel_off(const struct device* dev, const uint32_t channel) {
    const struct motor_l298n_config* config = dev->config;
    struct motor_l298n_data* data = dev->data;

    LOG_DBG("Turning motor channel %d off", channel);
    switch (channel) {
        case 0:
            data->ch0.enabled = false;
            return pwm_set_pulse_dt(&config->ch0.enable_pin, 0);
        case 1:
            data->ch1.enabled = false;
            return pwm_set_pulse_dt(&config->ch1.enable_pin, 0);
        default:
            LOG_ERR("Invalid motor channel %u", channel);
            return -EINVAL;
    }
}

static int motor_l298n_channel_set_direction(const struct device* dev, const unsigned int channel,
                                             const enum MotorDirection direction) {
    const struct motor_l298n_config* config = dev->config;
    struct motor_l298n_data* data = dev->data;

    const struct gpio_dt_spec* dira;
    const struct gpio_dt_spec* dirb;
    switch (channel) {
        case 0:
            data->ch0.direction = direction;
            dira = &config->ch0.dir_pins[0];
            dirb = &config->ch0.dir_pins[1];
            break;
        case 1:
            data->ch1.direction = direction;
            dira = &config->ch1.dir_pins[0];
            dirb = &config->ch1.dir_pins[1];
            break;
        default:
            LOG_ERR("Invalid motor channel %u", channel);
            return -EINVAL;
    }

    int dira_retval = 0;
    int dirb_retval = 0;
    switch (direction) {
        case FORWARD:
            dira_retval = gpio_pin_set_dt(dira, 0);
            dirb_retval = gpio_pin_set_dt(dirb, 1);
            break;
        case REVERSE:
            dira_retval = gpio_pin_set_dt(dira, 1);
            dirb_retval = gpio_pin_set_dt(dirb, 0);
            break;
        case BRAKE:
            dira_retval = gpio_pin_set_dt(dira, 1);
            dirb_retval = gpio_pin_set_dt(dirb, 1);
            break;
        case FREEWHEEL:
            dira_retval = gpio_pin_set_dt(dira, 0);
            dirb_retval = gpio_pin_set_dt(dirb, 0);
            break;
    }

    if (dira_retval < 0) {
        LOG_ERR("Channel %d failed to set direction. DIRA (err: %d)", channel, dira_retval);
        return dira_retval;
    }
    if (dirb_retval < 0) {
        LOG_ERR("Channel %d failed to set direction. DIRB (err: %d)", channel, dirb_retval);
        return dirb_retval;
    }
    return 0;
}

static DEVICE_API(motor, motor_api) = {
    .get_channel_count = &motor_l298n_get_channel_count,
    .set_speed = &motor_l298n_channel_set_speed,
    .motor_on = &motor_l298n_channel_on,
    .motor_off = &motor_l298n_channel_off,
    .set_direction = &motor_l298n_channel_set_direction,
};

static int motor_l298n_init_channel(const struct device* dev, const struct motor_l298n_channel* channel,
                                    const unsigned int channel_num) {
    const struct pwm_dt_spec* pwm = &channel->enable_pin;
    const struct gpio_dt_spec* dira = &channel->dir_pins[0];
    const struct gpio_dt_spec* dirb = &channel->dir_pins[1];

    if (!device_is_ready(pwm->dev)) {
        LOG_ERR("Channel %d: %s pwm device not ready", channel_num, pwm->dev->name);
        return -ENODEV;
    }

    int retval = motor_l298n_channel_set_speed(dev, channel_num, 0);
    if (retval) {
        LOG_ERR("Failed to initialize motor channel %d speed", channel_num);
        return retval;
    }

    if (!device_is_ready(dira->port)) {
        LOG_ERR("Channel %d: DIR A device not ready", channel_num);
        return -ENODEV;
    }
    retval = gpio_pin_configure_dt(dira, GPIO_OUTPUT_INACTIVE);
    if (retval) {
        LOG_ERR("Failed to initialize motor channel %d DIR A GPIO", channel_num, retval);
        return retval;
    }

    if (!device_is_ready(dirb->port)) {
        LOG_ERR("Channel %d: DIR B device not ready", channel_num);
        return -ENODEV;
    }
    retval = gpio_pin_configure_dt(dirb, GPIO_OUTPUT_INACTIVE);
    if (retval) {
        LOG_ERR("Failed to initialize motor channel %d DIR B GPIO", channel_num, retval);
        return retval;
    }

    return 0;
}

static int motor_l298n_init(const struct device* dev) {
    const struct motor_l298n_config* config = dev->config;
    struct motor_l298n_data* data = dev->data;

    LOG_DBG("Initializing L298N Motor Driver");

    int retval = motor_l298n_init_channel(dev, &config->ch0, 0);
    if (retval) {
        LOG_ERR("Failed to initialize L298N Motor Driver for channel 0");
        return retval;
    }

    retval = motor_l298n_init_channel(dev, &config->ch1, 1);
    if (retval) {
        LOG_ERR("Failed to initialize L298N Motor Driver for channel 1");
        return retval;
    }

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
    {                                                                               \
        .enable_pin = PWM_DT_SPEC_INST_GET_BY_IDX(idx, channel),                    \
        .dir_pins = {                                                               \
            GPIO_DT_SPEC_INST_GET_BY_IDX(idx, ch_##channel##_dir_gpios, 0),         \
            GPIO_DT_SPEC_INST_GET_BY_IDX(idx, ch_##channel##_dir_gpios, 1)          \
        }                                                                           \
    }
#define MOTOR_L298N_INIT(idx)						            \
        static const struct motor_l298n_config motor_l298n##idx##_config = {        \
            .ch0 = MOTOR_L298N_CONFIGURE_CHANNEL(idx, 0),                           \
            .ch1 = MOTOR_L298N_CONFIGURE_CHANNEL(idx, 1)                            \
        };								            \
                                                                                    \
        static struct motor_l298n_data motor_l298n##idx##_data = {                  \
            .ch0 = {                                                                \
                .enabled = false,                                                   \
                .direction = FORWARD,                                               \
                .pulse_width = 0                                                    \
            },                                                                      \
            .ch1 = {                                                                \
                .enabled = false,                                                   \
                .direction = FORWARD,                                               \
                .pulse_width = 0                                                    \
            }                                                                       \
        };                                                                          \
                                                                                    \
        DEVICE_DT_INST_DEFINE(idx, motor_l298n_init, NULL,                          \
                              &motor_l298n##idx##_data, &motor_l298n##idx##_config, \
                              POST_KERNEL, CONFIG_MOTOR_INIT_PRIORITY, &motor_api);

DT_INST_FOREACH_STATUS_OKAY(MOTOR_L298N_INIT)