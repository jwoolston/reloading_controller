/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef APP_DRIVERS_MOTOR_H_
#define APP_DRIVERS_MOTOR_H_

#include <zephyr/device.h>
#include <zephyr/toolchain.h>

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
     * @brief Configure the motor pwm period.
         * NOTE: This will adjust the period of both channels, if present.
     *
     * @param dev Motor device instance.
     * @param period_ms Period of the motor PWM in milliseconds, 0 to
     * disable drive.
     *
     * @retval 0 if successful.
     * @retval -EINVAL if @p period_ms can not be set.
     * @retval -errno Other negative errno code on failure.
     */
    int (*set_period_ms)(const struct device* dev, unsigned int period_ms);

    /**
 * @brief Set the current motor speed (percentage of full speed).
 * NOTE: This will adjust the speed of both channels, if present.
 *
 * @param dev Motor device instance.
 * @param speed The motor speed (percentage of full speed). Valid range is [0-100].
 *
 * @retval 0 if successful.
 * @retval -EINVAL if @p speed can not be set.
 * @retval -errno Other negative errno code on failure.
 */
    int (*set_speed)(const struct device* dev, unsigned int speed);
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
 * @brief Configure the motor PWM period.
 * NOTE: This will adjust the period of both channels, if present.
 *
 * @param dev Motor device instance.
 * @param period_ms Period of the motor PWM in milliseconds.
 *
 * @retval 0 if successful.
 * @retval -EINVAL if @p period_ms can not be set.
 * @retval -errno Other negative errno code on failure.
 */
__syscall int motor_set_period_ms(const struct device* dev,
                                  unsigned int period_ms);

static inline int z_impl_motor_set_period_ms(const struct device* dev,
                                             unsigned int period_ms)
{
    __ASSERT_NO_MSG(DEVICE_API_IS(motor, dev));

    return DEVICE_API_GET(motor, dev)->set_period_ms(dev, period_ms);
}

/**
 * @brief Set the current motor speed (percentage of full speed).
 * NOTE: This will adjust the speed of both channels, if present.
 *
 * @param dev Motor device instance.
 * @param speed The motor speed (percentage of full speed). Valid range is [0-100].
 *
 * @retval 0 if successful.
 * @retval -EINVAL if @p speed can not be set.
 * @retval -errno Other negative errno code on failure.
 */
__syscall int motor_set_speed(const struct device* dev, unsigned int speed);

static inline int z_impl_motor_set_speed(const struct device* dev, unsigned int speed)
{
    __ASSERT_NO_MSG(DEVICE_API_IS(motor, dev));

    return DEVICE_API_GET(motor, dev)->set_speed(dev, speed);
}

/**
 * @brief Turn all motor outputs off, without braking.
 *
 * This is a convenience function to turn off the motor outputs. No
 * braking action will be applied.
 *
 * @param dev Motor device instance.
 *
 * @return See motor_set_period_ms().
 */
static inline int motor_off(const struct device* dev)
{
    return motor_set_period_ms(dev, 0);
}

#include <syscalls/motor.h>

/** @} */

/** @} */

#endif /* APP_DRIVERS_MOTOR_H_ */
