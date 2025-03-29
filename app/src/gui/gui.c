#include "gui.h"

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <lvgl.h>
#include <stdio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(gui, CONFIG_LOG_DEFAULT_LEVEL);

lv_ui guider_ui;

static void setup_gui() {
#if LV_USE_THEME_DEFAULT
    lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                          LV_THEME_DEFAULT_DARK,
                          LV_FONT_DEFAULT);
#endif
    lv_obj_t * avatar = lv_image_create(lv_screen_active());
    //lv_obj_set_size(avatar, 480, 320);
    lv_image_set_src(avatar, "A:/ap_logo_min.png");
    lv_obj_center(avatar);
}

int reloading_gui(void) {
    const struct device* display_dev;

    LOG_INF("Getting display");
    display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
    if (!device_is_ready(display_dev)) {
        LOG_ERR("Device %s not found. Aborting sample.",
                display_dev->name);
        return -EINVAL;
    }

    LOG_INF("Creating UI on display %s", display_dev->name);

    //setup_gui();
    setup_ui(&guider_ui);
    events_init(&guider_ui);

    lv_task_handler();
    display_blanking_off(display_dev);
    return 0;
}

void reloading_gui_loop_handler(void) {
    lv_task_handler();
}