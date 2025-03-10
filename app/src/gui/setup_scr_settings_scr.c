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



void setup_scr_settings_scr(lv_ui *ui)
{
    //Write codes settings_scr
    ui->settings_scr = lv_obj_create(NULL);
    lv_obj_set_size(ui->settings_scr, 480, 320);
    lv_obj_set_scrollbar_mode(ui->settings_scr, LV_SCROLLBAR_MODE_OFF);

    //Write style for settings_scr, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->settings_scr, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->settings_scr, lv_color_hex(0x4e4e56), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->settings_scr, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes settings_scr_top_bar
    ui->settings_scr_top_bar = lv_obj_create(ui->settings_scr);
    lv_obj_set_pos(ui->settings_scr_top_bar, 0, 0);
    lv_obj_set_size(ui->settings_scr_top_bar, 480, 48);
    lv_obj_set_scrollbar_mode(ui->settings_scr_top_bar, LV_SCROLLBAR_MODE_OFF);

    //Write style for settings_scr_top_bar, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->settings_scr_top_bar, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->settings_scr_top_bar, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->settings_scr_top_bar, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->settings_scr_top_bar, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->settings_scr_top_bar, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->settings_scr_top_bar, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->settings_scr_top_bar, lv_color_hex(0xC06528), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->settings_scr_top_bar, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->settings_scr_top_bar, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->settings_scr_top_bar, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->settings_scr_top_bar, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->settings_scr_top_bar, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->settings_scr_top_bar, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of settings_scr.


    //Update current screen layout.
    lv_obj_update_layout(ui->settings_scr);

}
