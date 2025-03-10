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
    uint8_t speed;
    uint8_t direction;
};

static const struct args_index args_indx = {
    .device = 1,
    .channel = 2,
    .speed = 3,
    .direction = 3,
};

static int cmd_channel_count(const struct shell* sh, size_t argc, char** argv) {
    const struct device* dev = shell_device_get_binding(argv[args_indx.device]);
    if (!dev) {
        shell_error(sh, "Motor device not found");
        return -EINVAL;
    }

    const int retval = motor_get_channel_count(dev);
    if (retval < 0) {
        shell_error(sh, "Failed to get motor channel count (err %d)", retval);
        return retval;
    }
    shell_print(sh, "Motor channel count: %d", retval);
    return 0;
}

static int cmd_speed(const struct shell* sh, size_t argc, char** argv) {
    const struct device* dev = shell_device_get_binding(argv[args_indx.device]);
    if (!dev) {
        shell_error(sh, "Motor device not found");
        return -EINVAL;
    }

    const uint32_t channel = strtoul(argv[args_indx.channel], NULL, 0);
    const uint32_t speed = strtoul(argv[args_indx.speed], NULL, 0);

    const int err = motor_set_speed(dev, channel, speed);
    if (err) {
        shell_error(sh, "Failed to set motor speed for channel %d (err %d)", channel, err);
        return err;
    }

    return 0;
}

static int cmd_on(const struct shell* sh, size_t argc, char** argv) {
    const struct device* dev = shell_device_get_binding(argv[args_indx.device]);
    if (!dev) {
        shell_error(sh, "Motor device not found");
        return -EINVAL;
    }

    const uint32_t channel = strtoul(argv[args_indx.channel], NULL, 0);

    const int err = motor_on(dev, channel);
    if (err) {
        shell_error(sh, "Failed to turn motor on for channel %d (err %d)", channel, err);
        return err;
    }

    return 0;
}

static int cmd_off(const struct shell* sh, size_t argc, char** argv) {
    const struct device* dev = shell_device_get_binding(argv[args_indx.device]);
    if (!dev) {
        shell_error(sh, "Motor device not found");
        return -EINVAL;
    }

    const uint32_t channel = strtoul(argv[args_indx.channel], NULL, 0);

    const int err = motor_off(dev, channel);
    if (err) {
        shell_error(sh, "Failed to turn motor off for channel %d (err %d)", channel, err);
        return err;
    }

    return 0;
}

static int cmd_dir(const struct shell* sh, size_t argc, char** argv) {
    const struct device* dev = shell_device_get_binding(argv[args_indx.device]);
    if (!dev) {
        shell_error(sh, "Motor device not found");
        return -EINVAL;
    }

    const uint32_t channel = strtoul(argv[args_indx.channel], NULL, 0);
    const uint32_t dir = strtoul(argv[args_indx.direction], NULL, 0);

    enum MotorDirection direction;
    switch (dir) {
        case 0:
            direction = FORWARD;
            break;
        case 1:
            direction = REVERSE;
            break;
        case 2:
            direction = FREEWHEEL;
            break;
        case 3:
            direction = BRAKE;
            break;
        default:
            shell_error(sh, "Invalid direction: %d", dir);
            return -EINVAL;
    }

    const int err = motor_set_direction(dev, channel, direction);
    if (err) {
        shell_error(sh, "Failed to set motor direction for channel %d (err %d)", channel, err);
        return err;
    }

    return 0;
}

static bool device_is_motor(const struct device* dev) {
    return DEVICE_API_IS(motor, dev);
}

static void device_name_get(size_t idx, struct shell_static_entry* entry) {
    const struct device* dev = shell_device_filter(idx, device_is_motor);

    entry->syntax = (dev != NULL) ? dev->name : NULL;
    entry->handler = NULL;
    entry->help = NULL;
    entry->subcmd = NULL;
}

SHELL_DYNAMIC_CMD_CREATE(dsub_device_name, device_name_get);

SHELL_STATIC_SUBCMD_SET_CREATE(motor_cmds,
                               SHELL_CMD_ARG(channels, &dsub_device_name, "<device>", cmd_channel_count, 2, 0),
                               SHELL_CMD_ARG(speed, &dsub_device_name, "<device> <channel> <speed in %>", cmd_speed, 4,
                                   0),
                               SHELL_CMD_ARG(on, &dsub_device_name, "<device> <channel>", cmd_on, 3, 0),
                               SHELL_CMD_ARG(off, &dsub_device_name, "<device> <channel>", cmd_off, 3, 0),
                               SHELL_CMD_ARG(dir, &dsub_device_name,
                                   "<device> <channel> <direction FWD = 0, REV = 1, FREE = 2, BRAKE = 3>", cmd_dir, 4, 0
                               ),
                               SHELL_SUBCMD_SET_END
    );

SHELL_CMD_REGISTER(motor, &motor_cmds, "Motor shell commands", NULL);