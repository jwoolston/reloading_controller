/*
* Copyright 2025 Jared Woolston
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Motor shell commands.
 */


#include <zephyr/shell/shell.h>
#include <app/drivers/motor.h>
#include <stdlib.h>

struct args_index {
    uint8_t device;
    uint8_t channel;
    uint8_t period;
    uint8_t pulse;
    uint8_t flags;
};

static const struct args_index args_indx = {
    .device = 1,
    .channel = 2,
    .period = 3,
    .pulse = 4,
    .flags = 5,
};

static int cmd_speed(const struct shell *sh, size_t argc, char **argv)
{
    /*pwm_flags_t flags = 0;
    const struct device *dev;
    uint32_t period;
    uint32_t pulse;
    uint32_t channel;
    int err;

    dev = shell_device_get_binding(argv[args_indx.device]);
    if (!dev) {
        shell_error(sh, "PWM device not found");
        return -EINVAL;
    }

    channel = strtoul(argv[args_indx.channel], NULL, 0);
    period = strtoul(argv[args_indx.period], NULL, 0);
    pulse = strtoul(argv[args_indx.pulse], NULL, 0);

    if (argc == (args_indx.flags + 1)) {
        flags = strtoul(argv[args_indx.flags], NULL, 0);
    }

    err = pwm_set(dev, channel, period, pulse, flags);
    if (err) {
        shell_error(sh, "failed to setup PWM (err %d)", err);
        return err;
    }*/

    return 0;
}

static bool device_is_motor(const struct device *dev)
{
    return DEVICE_API_IS(motor, dev);
}

static void device_name_get(size_t idx, struct shell_static_entry *entry)
{
    const struct device *dev = shell_device_filter(idx, device_is_motor);

    entry->syntax = (dev != NULL) ? dev->name : NULL;
    entry->handler = NULL;
    entry->help = NULL;
    entry->subcmd = NULL;
}

SHELL_DYNAMIC_CMD_CREATE(dsub_device_name, device_name_get);

SHELL_STATIC_SUBCMD_SET_CREATE(motor_cmds,
        SHELL_CMD_ARG(nsec, &dsub_device_name, "<device> <channel> <period in nsec> "
                      "<pulse width in nsec> [flags]", cmd_speed, 5, 1),
        SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(motor, &motor_cmds, "Motor shell commands", NULL);