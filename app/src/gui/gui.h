//
// Created by Jared Woolston on 3/8/2025.
//

#ifndef GUI_H
#define GUI_H

#include "zephyr/drivers/display.h"
#include "gui_guider.h"
#include "events_init.h"

int reloading_gui(void);

void reloading_gui_loop_handler(void);

#endif //GUI_H
