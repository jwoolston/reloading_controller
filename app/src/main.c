/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/sys/util.h>

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL);

#define PWM_LCD_ALIAS(i) DT_ALIAS(_CONCAT(pwm_lcd, i))
#define PWM_LCD_IS_OKAY(i) DT_NODE_HAS_STATUS_OKAY(DT_PARENT(PWM_LCD_ALIAS(i)))
#define PWM_LCD(i, _) IF_ENABLED(PWM_LCD_IS_OKAY(i), (PWM_DT_SPEC_GET(PWM_LCD_ALIAS(i)),))

static const struct pwm_dt_spec pwm_led = PWM_DT_SPEC_GET(DT_ALIAS(pwm_lcd0));

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
    uint32_t pulse_width;
    uint32_t step;
    uint8_t dir = 1U;
    int ret;

    pulse_width = 0;
    step = pwm_led.period / NUM_STEPS;
    if (!pwm_is_ready_dt(&pwm_led)) {
        printk("Error: PWM device %s is not ready\n", pwm_led.dev->name);
        return 0;
    }

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

    while (1) {
        // Loop PWM 1000ms then do ADC reading
        for (int cnt = 0; cnt < 1000 / SLEEP_MSEC; ++cnt) {
                ret = pwm_set_pulse_dt(&pwm_led, pulse_width);
                if (ret) {
                    printk("Error %d: failed to set pulse width for LED\n", ret);
                }
                printk("LED: Using pulse width %d%%\n",
                       100 * pulse_width / pwm_led.period);

                if (dir) {
                    if (pulse_width + step >= pwm_led.period) {
                        pulse_width = pwm_led.period;
                        dir = 0U;
                    } else {
                        pulse_width += step;
                    }
                } else {
                    if (pulse_width <= step) {
                        pulse_width = 0;
                        dir = 1U;
                    } else {
                        pulse_width -= step;
                    }
                }

            k_sleep(K_MSEC(SLEEP_MSEC));
        }

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