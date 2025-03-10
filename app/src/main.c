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
#include <zephyr/drivers/display.h>
#include <app/drivers/motor.h>

//#include "gui.h"
#include <lvgl.h>

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL);

/* The devicetree node identifier for the "green_led_2" alias. */
#define LED0_NODE DT_ALIAS(led0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

/*#if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
!DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
#error "No suitable devicetree overlay specified"
#endif

#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

/* Data of ADC io-channels specified in devicetree. */
/*static const struct adc_dt_spec adc_channels[] = {
    DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels,
                         DT_SPEC_AND_COMMA)
};*/

int main(void) {

    if (!gpio_is_ready_dt(&led)) {
        return 0;
    }

    int ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        return 0;
    }

    const struct device *motor = DEVICE_DT_GET(DT_NODELABEL(motors));
    if (!device_is_ready(motor)) {
        LOG_ERR("Motor device not ready");
        return 0;
    }

    LOG_INF("Display starts");
    //reloading_gui();

    //test_gui(display_dev);

    while (1) {
        //LOG_INF("Hello World!");
        //reloading_gui_loop_handler();
        ret = gpio_pin_toggle_dt(&led);
        k_sleep(K_MSEC(100));
        //LOG_INF("ADC reading[%u]:\n", count++);
        /*for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
            int32_t val_mv;

            (void)adc_sequence_init_dt(&adc_channels[i], &sequence);

            err = adc_read_dt(&adc_channels[i], &sequence);
            if (err < 0) {
                LOG_ERR("Could not read (%d)\n", err);
                continue;
            }

            /*
             * If using differential mode, the 16 bit value
             * in the ADC sample buffer should be a signed 2's
             * complement value.
             */
        /*if (adc_channels[i].channel_cfg.differential) {
            val_mv = (int32_t)((int16_t)buf);
        } else {
            val_mv = (int32_t)buf;
        }
        err = adc_raw_to_millivolts_dt(&adc_channels[i],
                                       &val_mv);
        val_mv = (val_mv * 3) / 2;
        /* conversion to mV may not be supported, skip if not */
        /*if (err < 0) {
            LOG_ERR(" (value in mV not available)\n");
        /*} else {
            LOG_INF("- %s, channel %d: %"PRId32" mV\n", adc_channels[i].dev->name,
                    adc_channels[i].channel_id, val_mv);*/
        //}
        // }
    }

    return 0;
}