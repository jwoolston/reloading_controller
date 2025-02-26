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
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/sys/util.h>
#include <zephyr/drivers/display.h>
#include <lvgl.h>
#include <lvgl_input_device.h>

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL);

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

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

static uint32_t count;

#ifdef CONFIG_LV_Z_ENCODER_INPUT
static const struct device *lvgl_encoder =
        DEVICE_DT_GET(DT_COMPAT_GET_ANY_STATUS_OKAY(zephyr_lvgl_encoder_input));
#endif /* CONFIG_LV_Z_ENCODER_INPUT */

#ifdef CONFIG_LV_Z_KEYPAD_INPUT
static const struct device *lvgl_keypad =
        DEVICE_DT_GET(DT_COMPAT_GET_ANY_STATUS_OKAY(zephyr_lvgl_keypad_input));
#endif /* CONFIG_LV_Z_KEYPAD_INPUT */

static void lv_btn_click_callback(lv_event_t *e)
{
    ARG_UNUSED(e);

    count = 0;
}

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
            LOG_ERR("ADC controller device %s not ready\n", adc_channels[i].dev->name);
            return 0;
        }

        err = adc_channel_setup_dt(&adc_channels[i]);
        if (err < 0) {
            LOG_ERR("Could not setup channel #%d (%d)\n", i, err);
            return 0;
        }
    }

    if (!gpio_is_ready_dt(&led)) {
        return 0;
    }

    int ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        return 0;
    }
    //ret = gpio_pin_set_dt(&led, true);

    char count_str[11] = {0};
    const struct device *display_dev;
    lv_obj_t *hello_world_label;
    lv_obj_t *count_label;

    display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
    if (!device_is_ready(display_dev)) {
        LOG_ERR("Device not ready, aborting test");
        return 0;
    }

#ifdef CONFIG_LV_Z_ENCODER_INPUT
    lv_obj_t *arc;
    lv_group_t *arc_group;

    arc = lv_arc_create(lv_screen_active());
    lv_obj_align(arc, LV_ALIGN_CENTER, 0, -15);
    lv_obj_set_size(arc, 150, 150);

    arc_group = lv_group_create();
    lv_group_add_obj(arc_group, arc);
    lv_indev_set_group(lvgl_input_get_indev(lvgl_encoder), arc_group);
#endif /* CONFIG_LV_Z_ENCODER_INPUT */

#ifdef CONFIG_LV_Z_KEYPAD_INPUT
    lv_obj_t *btn_matrix;
    lv_group_t *btn_matrix_group;
    static const char *const btnm_map[] = {"1", "2", "3", "4", ""};

    btn_matrix = lv_buttonmatrix_create(lv_screen_active());
    lv_obj_align(btn_matrix, LV_ALIGN_CENTER, 0, 70);
    lv_buttonmatrix_set_map(btn_matrix, (const char **)btnm_map);
    lv_obj_set_size(btn_matrix, 100, 50);

    btn_matrix_group = lv_group_create();
    lv_group_add_obj(btn_matrix_group, btn_matrix);
    lv_indev_set_group(lvgl_input_get_indev(lvgl_keypad), btn_matrix_group);
#endif /* CONFIG_LV_Z_KEYPAD_INPUT */

    if (IS_ENABLED(CONFIG_LV_Z_POINTER_INPUT)) {
        lv_obj_t *hello_world_button;

        hello_world_button = lv_button_create(lv_screen_active());
        lv_obj_align(hello_world_button, LV_ALIGN_CENTER, 0, -15);
        lv_obj_add_event_cb(hello_world_button, lv_btn_click_callback, LV_EVENT_CLICKED,
                            NULL);
        hello_world_label = lv_label_create(hello_world_button);
    } else {
        hello_world_label = lv_label_create(lv_screen_active());
    }

    lv_label_set_text(hello_world_label, "Hello world!");
    lv_obj_align(hello_world_label, LV_ALIGN_CENTER, 0, 0);

    count_label = lv_label_create(lv_screen_active());
    lv_obj_align(count_label, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_timer_handler();
    display_blanking_off(display_dev);

    while (1) {
        if ((count % 100) == 0U) {
            sprintf(count_str, "%d", count/100U);
            lv_label_set_text(count_label, count_str);
        }
        lv_timer_handler();
        ++count;
        k_sleep(K_MSEC(10));
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