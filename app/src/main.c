/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/led.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/sys/util.h>

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL);

#define LED_PWM_NODE_ID	 DT_COMPAT_GET_ANY_STATUS_OKAY(pwm_leds)

const char *led_label[] = {
    DT_FOREACH_CHILD_SEP_VARGS(LED_PWM_NODE_ID, DT_PROP_OR, (,), label, NULL)
};

const int num_leds = ARRAY_SIZE(led_label);

#define MAX_BRIGHTNESS	100

#define NUM_STEPS	50U
#define SLEEP_MSEC	25U

#if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
!DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
#error "No suitable devicetree overlay specified"
#endif

#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

/* Data of ADC io-channels specified in devicetree. */
static const struct adc_dt_spec adc_channels[] = {
    DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels,
                         DT_SPEC_AND_COMMA)
};

const struct device *qdec0 = DEVICE_DT_GET(DT_NODELABEL(pio1_qdec));

int main(void) {
    int err;
    uint32_t count = 0;
    uint16_t buf;
    struct adc_sequence sequence = {
        .buffer = &buf,
        /* buffer size in bytes, not number of samples */
        .buffer_size = sizeof(buf),
    };

    /* Configure channels individually prior to sampling. */
    for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
        if (!adc_is_ready_dt(&adc_channels[i])) {
            printk("ADC controller device %s not ready\n", adc_channels[i].dev->name);
            return 0;
        }

        err = adc_channel_setup_dt(&adc_channels[i]);
        if (err < 0) {
            printk("Could not setup channel #%d (%d)\n", i, err);
            return 0;
        }
    }

    const struct device *led_pwm;
    uint8_t led = 0;

    led_pwm = DEVICE_DT_GET(LED_PWM_NODE_ID);
    if (!device_is_ready(led_pwm)) {
        printk("Device %s is not ready", led_pwm->name);
        return 0;
    }

    if (!num_leds) {
        printk("No LEDs found for %s", led_pwm->name);
        return 0;
    }

    int16_t level = MAX_BRIGHTNESS;

    printk("Testing LED %d - %s", led, led_label[led] ? : "no label");

    /* Turn LED on. */
    err = led_on(led_pwm, led);
    if (err < 0) {
        printk("err=%d", err);
        return -1;
    }
    printk("  Turned on\n");
    k_sleep(K_MSEC(1000));

    err = led_set_brightness(led_pwm, led, level);
    if (err < 0) {
        printk("err=%d brightness=%d\n", err, level);
        return -1;
    }


    while (1) {
        k_sleep(K_MSEC(1000));
        printk("ADC reading[%u]:\n", count++);
        for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
            int32_t val_mv;

            printk("- %s, channel %d: ",
                   adc_channels[i].dev->name,
                   adc_channels[i].channel_id);

            (void)adc_sequence_init_dt(&adc_channels[i], &sequence);

            err = adc_read_dt(&adc_channels[i], &sequence);
            if (err < 0) {
                printk("Could not read (%d)\n", err);
                continue;
            }

            /*
             * If using differential mode, the 16 bit value
             * in the ADC sample buffer should be a signed 2's
             * complement value.
             */
            if (adc_channels[i].channel_cfg.differential) {
                val_mv = (int32_t)((int16_t)buf);
            } else {
                printk("Single ended reading.");
                val_mv = (int32_t)buf;
            }
            printk("%"PRId32, val_mv);
            err = adc_raw_to_millivolts_dt(&adc_channels[i],
                                           &val_mv);
            val_mv = (val_mv * 3) / 2;
            /* conversion to mV may not be supported, skip if not */
            if (err < 0) {
                printk(" (value in mV not available)\n");
            } else {
                printk(" = %"PRId32" mV\n", val_mv);
            }
        }
    }

    return 0;
}