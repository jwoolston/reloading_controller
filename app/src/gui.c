#include "gui.h"

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <lvgl.h>
#include <stdio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(gui, CONFIG_LOG_DEFAULT_LEVEL);

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
