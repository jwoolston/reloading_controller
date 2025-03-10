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

static int motor_l298n_get_channel_count(const struct device *dev) {
    return 2; //TODO(jwoolston): Make this dynamic
}

static int motor_l298n_set_period_ms(const struct device* dev, unsigned int period_ms) {
    const struct motor_l298n_config* config = dev->config;
    struct motor_l298n_data* data = dev->data;

    LOG_DBG("Setting motor period to : %u", period_ms);

    /*int ret = 0;
    if (config->ch0 != NULL) {
        uint64_t pulse_ms;
        ret = pwm_cycles_to_msec(config->ch0->enable_pin.dev, config->ch0->enable_pin.channel,
                                 data->ch0.pulse_width, &pulse_ms);
        if (ret < 0) {
            LOG_ERR("pwm_cycles_to_msec failed (%d)", ret);
            return ret;
        }
        pwm_set_dt(&config->ch0->enable_pin, period_ms, (uint32_t) pulse_ms);
    }*/

    return 0;
}

static int motor_l298n_set_speed(const struct device* dev, unsigned int channel, unsigned int speed) {
    const struct motor_l298n_config* config = dev->config;
    struct motor_l298n_data* data = dev->data;

    LOG_DBG("Setting channel %u motor speed to : %u%%", channel, speed);
    switch (channel) {
        case 0:
            data->ch0.pulse_width = (speed / 100.0f) * config->ch0.enable_pin.period;
            pwm_set_pulse_dt(&config->ch0.enable_pin, data->ch0.pulse_width);
            break;
        case 1:
            data->ch1.pulse_width = (speed / 100.0f) * config->ch1.enable_pin.period;
            pwm_set_pulse_dt(&config->ch1.enable_pin, data->ch1.pulse_width);
            break;
        default:
            LOG_ERR("Invalid motor channel %u", channel);
            return -EINVAL;
    }
    return 0;
}

static int motor_l298n_on(const struct device *dev, uint32_t led)
{
    LOG_DBG("Turning motor on");
    //return motor_l298n_set_speed(dev, led, 100);
    return 0;
}

static int motor_l298n_off(const struct device *dev, uint32_t led)
{
    LOG_DBG("Turning motor off");
    //return motor_l298n_set_speed(dev, led, 0);
    return 0;
}

static DEVICE_API(motor, motor_api) = {
    .get_channel_count = &motor_l298n_get_channel_count,
    .set_period_ms = &motor_l298n_set_period_ms,
    .set_speed = &motor_l298n_set_speed,
};

static int motor_l298n_init(const struct device* dev) {
    const struct motor_l298n_config* config = dev->config;
    struct motor_l298n_data* data = dev->data;

    LOG_DBG("Initializing L298N Motor Driver");

    const struct pwm_dt_spec* pwm_ch0 = &config->ch0.enable_pin;
    const struct pwm_dt_spec* pwm_ch1 = &config->ch1.enable_pin;

    if (!device_is_ready(pwm_ch0->dev)) {
        LOG_ERR("%s: pwm device not ready", pwm_ch0->dev->name);
        return -ENODEV;
    }

    if (!device_is_ready(pwm_ch1->dev)) {
        LOG_ERR("%s: pwm device not ready", pwm_ch1->dev->name);
        return -ENODEV;
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
    COND_CODE_1(DT_INST_PROP_HAS_IDX(idx, pwms, channel),                           \
    ({                                                                              \
        .enable_pin = PWM_DT_SPEC_INST_GET_BY_IDX_OR(idx, channel, {}),             \
        .dir_pins = {                                                               \
            GPIO_DT_SPEC_INST_GET_BY_IDX_OR(idx, ch_##channel##_dir_gpios, 0, {}),  \
            GPIO_DT_SPEC_INST_GET_BY_IDX_OR(idx, ch_##channel##_dir_gpios, 1, {})   \
        }                                                                           \
    }), NULL)
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