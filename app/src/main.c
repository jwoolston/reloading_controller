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
/* <lvgl.h>
#include <lvgl_mem.h>
#include <lv_demos.h>*/

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

//const struct device *motor = DEVICE_DT_GET(DT_NODELABEL(motor_l298n));

//static uint32_t count;

/*static void lv_btn_click_callback(lv_event_t* e) {
    ARG_UNUSED(e);

    count = 0;
}*/


enum corner {
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_RIGHT,
	BOTTOM_LEFT
};

typedef void (*fill_buffer)(enum corner corner, uint8_t grey, uint8_t *buf,
			    size_t buf_size);


static uint16_t get_rgb565_color(enum corner corner, uint8_t grey)
{
	uint16_t color = 0;
	uint16_t grey_5bit;

	switch (corner) {
	case TOP_LEFT:
		color = 0xF800u;
		break;
	case TOP_RIGHT:
		color = 0x07E0u;
		break;
	case BOTTOM_RIGHT:
		color = 0x001Fu;
		break;
	case BOTTOM_LEFT:
		grey_5bit = grey & 0x1Fu;
		/* shift the green an extra bit, it has 6 bits */
		color = grey_5bit << 11 | grey_5bit << (5 + 1) | grey_5bit;
		break;
	}
	return color;
}

static void fill_buffer_rgb565(enum corner corner, uint8_t grey, uint8_t *buf,
			       size_t buf_size)
{
	uint16_t color = get_rgb565_color(corner, grey);

	for (size_t idx = 0; idx < buf_size; idx += 2) {
		*(buf + idx + 0) = (color >> 8) & 0xFFu;
		*(buf + idx + 1) = (color >> 0) & 0xFFu;
	}
}

int sample(void)
{
	size_t x;
	size_t y;
	size_t rect_w;
	size_t rect_h;
	size_t h_step;
	size_t scale;
	uint8_t bg_color;
	uint8_t *buf;
	const struct device *display_dev;
	struct display_capabilities capabilities;
	struct display_buffer_descriptor buf_desc;
	size_t buf_size = 0;
	fill_buffer fill_buffer_fnc = NULL;

	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		LOG_ERR("Device %s not found. Aborting sample.",
			display_dev->name);
		return 0;
	}

	LOG_INF("Display sample for %s", display_dev->name);
	display_get_capabilities(display_dev, &capabilities);

	if (capabilities.screen_info & SCREEN_INFO_MONO_VTILED) {
		rect_w = 16;
		rect_h = 8;
	} else {
		rect_w = 2;
		rect_h = 1;
	}

	if ((capabilities.x_resolution < 3 * rect_w) ||
	    (capabilities.y_resolution < 3 * rect_h) ||
	    (capabilities.x_resolution < 8 * rect_h)) {
		rect_w = capabilities.x_resolution * 40 / 100;
		rect_h = capabilities.y_resolution * 40 / 100;
		h_step = capabilities.y_resolution * 20 / 100;
		scale = 1;
	} else {
		h_step = rect_h;
		scale = (capabilities.x_resolution / 4) / rect_h;
	}

	rect_w *= scale;
	rect_h *= scale;

	if (capabilities.screen_info & SCREEN_INFO_X_ALIGNMENT_WIDTH) {
		rect_w = capabilities.x_resolution;
	}

	buf_size = rect_w * rect_h;

	if (buf_size < (capabilities.x_resolution * h_step)) {
		buf_size = capabilities.x_resolution * h_step;
	}

	switch (capabilities.current_pixel_format) {
	case PIXEL_FORMAT_RGB_565:
		bg_color = 0xFFu;
		fill_buffer_fnc = fill_buffer_rgb565;
		buf_size *= 2;
		break;
	default:
		LOG_ERR("Unsupported pixel format. Aborting sample.");
		return 0;
	}

	buf = k_malloc(buf_size);

	if (buf == NULL) {
		LOG_ERR("Could not allocate memory. Aborting sample.");
		return 0;
	}

	(void)memset(buf, bg_color, buf_size);

	buf_desc.buf_size = buf_size;
	buf_desc.pitch = capabilities.x_resolution;
	buf_desc.width = capabilities.x_resolution;
	buf_desc.height = h_step;

	/*
	 * The following writes will only render parts of the image,
	 * so turn this option on.
	 * This allows double-buffered displays to hold the pixels
	 * back until the image is complete.
	 */
	buf_desc.frame_incomplete = true;

	for (int idx = 0; idx < capabilities.y_resolution; idx += h_step) {
		/*
		 * Tweaking the height value not to draw outside of the display.
		 * It is required when using a monochrome display whose vertical
		 * resolution can not be divided by 8.
		 */
		if ((capabilities.y_resolution - idx) < h_step) {
			buf_desc.height = (capabilities.y_resolution - idx);
		}
		display_write(display_dev, 0, idx, &buf_desc, buf);
	}

	buf_desc.pitch = rect_w;
	buf_desc.width = rect_w;
	buf_desc.height = rect_h;

	fill_buffer_fnc(TOP_LEFT, 0, buf, buf_size);
	x = 0;
	y = 0;
	display_write(display_dev, x, y, &buf_desc, buf);

	fill_buffer_fnc(TOP_RIGHT, 0, buf, buf_size);
	x = capabilities.x_resolution - rect_w;
	y = 0;
	display_write(display_dev, x, y, &buf_desc, buf);

	/*
	 * This is the last write of the frame, so turn this off.
	 * Double-buffered displays will now present the new image
	 * to the user.
	 */
	buf_desc.frame_incomplete = false;

	fill_buffer_fnc(BOTTOM_RIGHT, 0, buf, buf_size);
	x = capabilities.x_resolution - rect_w;
	y = capabilities.y_resolution - rect_h;
	display_write(display_dev, x, y, &buf_desc, buf);

	display_blanking_off(display_dev);

	LOG_INF("Display starts");
	return 0;
}


int main(void) {
    int err;
    //uint32_t count = 0;
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

    //char count_str[11] = {0};

    /*const struct device* display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
    if (!device_is_ready(display_dev)) {
        LOG_ERR("Device not ready, aborting test");
        return 0;
    }*/

    /*uint16_t Color = 0xFFFF;
    uint16_t j;
    uint16_t Image[240*240];

    Color = ((Color<<8)&0xff00)|(Color>>8);

    for (j = 0; j < 240*240; j++) {
        Image[j] = Color;
    }

    struct display_buffer_descriptor desc;
    desc.buf_size = 240 * 240 * 2;
    desc.width = 240;
    desc.height = 240;
    desc.pitch = 240;
    desc.frame_incomplete = false;

    display_write(display_dev, 0, 0, &desc, (void *)Image);*/

    /*lv_obj_t* hello_world_button = lv_button_create(lv_screen_active());
    lv_obj_align(hello_world_button, LV_ALIGN_CENTER, 0, -15);
    lv_obj_add_event_cb(hello_world_button, lv_btn_click_callback, LV_EVENT_CLICKED, NULL);
    lv_obj_t* hello_world_label = lv_label_create(hello_world_button);

    lv_label_set_text(hello_world_label, "Hello world!");
    lv_obj_align(hello_world_label, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t* count_label = lv_label_create(lv_screen_active());
    lv_obj_align(count_label, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(hello_world_label, lv_color_make(255, 0, 0), LV_STATE_DEFAULT);

    lv_timer_handler();
    display_blanking_on(display_dev);
    k_sleep(K_MSEC(120));
    display_blanking_off(display_dev);
    k_sleep(K_MSEC(1000));
    lvgl_print_heap_info(false);*/
    sample();

    //motor_set_period_ns(motor, 123456);
    while (1) {
        /*f (lv_screen_active() == NULL) {
            LOG_ERR("No display attached");
        }*/
        /*if ((count % 10) == 0U) {
            sprintf(count_str, "%d", count / 100U);
            lv_label_set_text(count_label, count_str);
        }
        lv_timer_handler();*/
        //uint32_t sleep_ms = lv_timer_handler();

        //k_msleep(MIN(sleep_ms, INT32_MAX));
        //++count;
        k_sleep(K_MSEC(10));
        k_sleep(K_MSEC(490));
        //LOG_INF("ADC reading[%u]:\n", count++);
        for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
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
            if (adc_channels[i].channel_cfg.differential) {
                val_mv = (int32_t)((int16_t)buf);
            } else {
                val_mv = (int32_t)buf;
            }
            err = adc_raw_to_millivolts_dt(&adc_channels[i],
                                           &val_mv);
            val_mv = (val_mv * 3) / 2;
            /* conversion to mV may not be supported, skip if not */
            if (err < 0) {
                LOG_ERR(" (value in mV not available)\n");
            /*} else {
                LOG_INF("- %s, channel %d: %"PRId32" mV\n", adc_channels[i].dev->name,
                        adc_channels[i].channel_id, val_mv);*/
            }
        }
    }

    return 0;
}