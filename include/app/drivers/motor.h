/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef APP_DRIVERS_MOTOR_H_
#define APP_DRIVERS_MOTOR_H_

#include <zephyr/device.h>

enum MotorDirection { FORWARD = 0, REVERSE = 1, FREEWHEEL = 2, BRAKE = 3 };

/**
 * @defgroup drivers_blink Motor drivers
 * @ingroup drivers
 * @{
 *
 * @brief A custom Motor/H-Bridge driver class.
 *
 */

/**
 * @defgroup drivers_motor_ops Motor driver operations
 * @{
 *
 * @brief Operations of the motor driver class.
 *
 */

/** @brief Motor driver class operations */
__subsystem struct motor_driver_api
{
    /**
     * @brief Get the number of channels (motors) this device controls.
     *
     * @param dev Motor device instance.
     *
     * @retval The number of controlled channels, if successful.
     * @retval -errno Other negative errno code on failure.
     */
    int (*get_channel_count)(const struct device* dev);

    /**
     * @brief Set the current motor speed (percentage of full speed).
     * NOTE: This will adjust the speed of both channels, if present.
     *
     * @param dev Motor device instance.
     * @param channel The 0-based channel on this instance to adjust.
     * @param speed The motor speed (percentage of full speed). Valid range is [0-100].
     *
     * @retval 0 if successful.
     * @retval -EINVAL if @p speed can not be set.
     * @retval -errno Other negative errno code on failure.
     */
    int (*set_speed)(const struct device* dev, unsigned int channel, unsigned int speed);

    int (*motor_on)(const struct device* dev, unsigned int channel);

    int (*motor_off)(const struct device* dev, unsigned int channel);

    int (*set_direction)(const struct device* dev, unsigned int channel, enum MotorDirection direction);
};

/** @} */

/**
 * @defgroup drivers_motor_api Motor driver API
 * @{
 *
 * @brief Public API provided by the motor driver class.
 *
 */

/**
 * @brief Get the number of channels (motors) this device controls.
 *
 * @param dev Motor device instance.
 *
 * @retval The number of controlled channels, if successful.
 * @retval -errno Other negative errno code on failure.
 */
__syscall int motor_get_channel_count(const struct device* dev);

static inline int z_impl_motor_get_channel_count(const struct device* dev)
{
    __ASSERT_NO_MSG(DEVICE_API_IS(motor, dev));

    return DEVICE_API_GET(motor, dev)->get_channel_count(dev);
}

/**
 * @brief Set the current motor speed (percentage of full speed).
 *
 * @param dev Motor device instance.
 * @param channel The 0-based channel on this instance to adjust.
 * @param speed The motor speed (percentage of full speed). Valid range is [0-100].
 *
 * @retval 0 if successful.
 * @retval -EINVAL if @p speed can not be set.
 * @retval -errno Other negative errno code on failure.
 */
__syscall int motor_set_speed(const struct device* dev, unsigned int channel, unsigned int speed);

static inline int z_impl_motor_set_speed(const struct device* dev, unsigned int channel, unsigned int speed)
{
    __ASSERT_NO_MSG(DEVICE_API_IS(motor, dev));

    return DEVICE_API_GET(motor, dev)->set_speed(dev, channel, speed);
}

__syscall int motor_on(const struct device* dev, unsigned int channel);

static inline int z_impl_motor_on(const struct device* dev, unsigned int channel)
{
    __ASSERT_NO_MSG(DEVICE_API_IS(motor, dev));

    return DEVICE_API_GET(motor, dev)->motor_on(dev, channel);
}

__syscall int motor_off(const struct device* dev, unsigned int channel);

static inline int z_impl_motor_off(const struct device* dev, unsigned int channel)
{
    __ASSERT_NO_MSG(DEVICE_API_IS(motor, dev));

    return DEVICE_API_GET(motor, dev)->motor_off(dev, channel);
}

__syscall int motor_set_direction(const struct device* dev, unsigned int channel, enum MotorDirection direction);

static inline int z_impl_motor_set_direction(const struct device* dev, unsigned int channel,
                                             enum MotorDirection direction)
{
    __ASSERT_NO_MSG(DEVICE_API_IS(motor, dev));

    return DEVICE_API_GET(motor, dev)->set_direction(dev, channel, direction);
}

#include <syscalls/motor.h>

/** @} */

/** @} */

#endif /* APP_DRIVERS_MOTOR_H_ */
