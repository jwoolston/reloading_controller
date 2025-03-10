#include "gui.h"

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <lvgl.h>
#include <stdio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(gui, CONFIG_LOG_DEFAULT_LEVEL);

static uint32_t count;
static char count_str[11] = {0};
static lv_obj_t *hello_world_label;
static lv_obj_t *count_label;

/*static void lv_btn_click_callback(lv_event_t *e)
{
    ARG_UNUSED(e);

    count = 0;
}

void test_gui(const struct device* display_dev) {
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
}

void test_gui_loop(void) {
    if ((count % 1) == 0U) {
        sprintf(count_str, "%d", count/1U);
        lv_label_set_text(count_label, count_str);
    }
    lv_timer_handler();
    ++count;
}*/

lv_ui guider_ui;

int reloading_gui(void)
{
    const struct device* display_dev;

    LOG_INF("Getting display");
    display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
    if (!device_is_ready(display_dev)) {
        LOG_ERR("Device %s not found. Aborting sample.",
                display_dev->name);
        return -EINVAL;
    }

    LOG_INF("Display sample for %s", display_dev->name);

    setup_ui(&guider_ui);
    events_init(&guider_ui);

    lv_task_handler();
    display_blanking_off(display_dev);
    return 0;
}

void reloading_gui_loop_handler(void) {
    lv_task_handler();
}
