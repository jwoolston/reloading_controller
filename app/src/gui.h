//
// Created by Jared Woolston on 3/8/2025.
//

#ifndef GUI_H
#define GUI_H

#include "zephyr/drivers/display.h"
#include "gui/gui_guider.h"
#include "gui/events_init.h"

/*void test_gui(const struct device* display_dev);

void test_gui_loop(void);*/

int reloading_gui(void);

void reloading_gui_loop_handler(void);

#endif //GUI_H
