/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/led.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/sys/util.h>

#include "feeders/feeder_subsystem.h"
#include "filesystem/filesystem.h"
#include "gui.h"

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL);

#define LED0_NODE DT_ALIAS(led0)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

int main(void) {

    if (!gpio_is_ready_dt(&led)) {
        return 0;
    }

    int ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if (ret) {
        LOG_ERR("Failed to configure heartbeat LED: %d", ret);
        return ret;
    }

    LOG_INF("Initializing Filesystem");
    mount_filesystem();

    LOG_INF("Initializing GUI");
    ret = reloading_gui();
    if (ret) {
        LOG_ERR("Failed to to initialize GUI. Error: %d", ret);
        return ret;
    }

    LOG_INF("Initializing feeder subsystem");
    ret = feeder_subsystem_init();
    if (ret) {
        LOG_ERR("Failed to initialize feeder_subsystem: %d", ret);
        return ret;
    }

    int loop_count = 0;
    while (1) {
        //reloading_gui_loop_handler();

        if (loop_count == 10) {
            // Toggle heartbeat LED
            ret = gpio_pin_toggle_dt(&led);
            if (ret) {
                LOG_ERR("Failed to toggle heartbeat LED: %d", ret);
            }
            loop_count = 0;
        } else {
            ++loop_count;
        }

        k_sleep(K_MSEC(10));
    }
    return 0;
}