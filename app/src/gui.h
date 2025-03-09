//
// Created by Jared Woolston on 3/8/2025.
//

#ifndef GUI_H
#define GUI_H

#include "zephyr/drivers/display.h"

void test_gui(const struct device* display_dev);

void test_gui_loop(void);

void reloading_gui(void);

#endif //GUI_H
