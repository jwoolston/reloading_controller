/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"



void setup_scr_splash_scr(lv_ui *ui)
{
    //Write codes splash_scr
    ui->splash_scr = lv_obj_create(NULL);
    lv_obj_set_size(ui->splash_scr, 480, 320);
    lv_obj_set_scrollbar_mode(ui->splash_scr, LV_SCROLLBAR_MODE_OFF);

    //Write style for splash_scr, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->splash_scr, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->splash_scr, lv_color_hex(0xff0000), LV_PART_MAIN|LV_STATE_DEFAULT); // 0x4e4e56
    lv_obj_set_style_bg_grad_dir(ui->splash_scr, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes splash_scr_splash_logo
    ui->splash_scr_splash_logo = lv_image_create(ui->splash_scr);
    lv_obj_set_pos(ui->splash_scr_splash_logo, 0, 0);
    lv_obj_set_size(ui->splash_scr_splash_logo, 480, 320);
    lv_obj_align(ui->splash_scr_splash_logo, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(ui->splash_scr_splash_logo, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->splash_scr_splash_logo, "A:/ap_logo_min.png");
    lv_image_set_pivot(ui->splash_scr_splash_logo, 50,50);
    lv_image_set_rotation(ui->splash_scr_splash_logo, 0);

    //Write style for splash_scr_splash_logo, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->splash_scr_splash_logo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->splash_scr_splash_logo, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of splash_scr.


    //Update current screen layout.
    lv_obj_update_layout(ui->splash_scr);

    //Init events for screen.
    events_init_splash_scr(ui);
}
