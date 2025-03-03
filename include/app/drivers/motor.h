/*
 * Copyright (c) 2025 Jared Woolston
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef APP_DRIVERS_MOTOR_H_
#define APP_DRIVERS_MOTOR_H_

#include <zephyr/device.h>
#include <zephyr/toolchain.h>

/**
 * @defgroup drivers_motor Motor drivers
 * @ingroup drivers
 * @{
 *
 * @brief A custom driver class to control DC motors
 *
 * This driver class provides a means to control DC motors via PWM with
 * optional direction control. Depending on configuration, the driver
 * can have no direction control, single pin or dual pin direction control.
 * In the case of dual direction control, braking options are additionally
 * enabled.
 */

/**
 * @defgroup drivers_motor_ops Motor driver operations
 * @{
 *
 * @brief Operations of the motor driver class.
 *
 * Each driver class typically provides a set of operations that need to be
 * implemented by each driver. These are used to implement the public API. If
 * support for system calls is needed, the operations structure must be tagged
 * with `__subsystem` and follow the `${class}_driver_api` naming scheme.
 */

/** @brief Motor driver class operations */
__subsystem struct motor_driver_api {
	/**
	 * @brief Configure the motor PWM period.
	 *
	 * @param dev Motor device instance.
	 * @param period_ns Period of the motor PWM in nanoseconds, 0 to
	 * disable drive.
	 *
	 * @retval 0 if successful.
	 * @retval -EINVAL if @p period_ns can not be set.
	 * @retval -errno Other negative errno code on failure.
	 */
	int (*set_period_ns)(const struct device *dev, unsigned int period_ns);
};

/** @} */

/**
 * @defgroup drivers_motor_api Motor driver API
 * @{
 *
 * @brief Public API provided by the motor driver class.
 *
 * The public API is the interface that is used by applications to interact with
 * devices that implement the motor driver class. If support for system calls is
 * needed, functions accessing device fields need to be tagged with `__syscall`
 * and provide an implementation that follows the `z_impl_${function_name}`
 * naming scheme.
 */

/**
 * @brief Configure the Motor PWM period.
 *
 *
 * @param dev Motor device instance.
 * @param period_ns Period of the motor PWM in nanoseconds.
 *
 * @retval 0 if successful.
 * @retval -EINVAL if @p period_ns can not be set.
 * @retval -errno Other negative errno code on failure.
 */
__syscall int motor_set_period_ns(const struct device *dev,
				  unsigned int period_ns);

static inline int z_impl_motor_set_period_ns(const struct device *dev,
					     unsigned int period_ns)
{
	__ASSERT_NO_MSG(DEVICE_API_IS(motor, dev));

	return DEVICE_API_GET(motor, dev)->set_period_ns(dev, period_ns);
}

/**
 * @brief Turn Motor drive off.
 *
 * This is a convenience function to turn off the motor.
 *
 * @param dev Motor device instance.
 *
 * @return See motor_set_period_ns().
 */
static inline int motor_off(const struct device *dev)
{
	return motor_set_period_ns(dev, 0);
}

#include <syscalls/motor.h>

/** @} */

/** @} */

#endif /* APP_DRIVERS_BLINK_H_ */