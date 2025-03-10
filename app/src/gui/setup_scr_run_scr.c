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



void setup_scr_run_scr(lv_ui *ui)
{
    //Write codes run_scr
    ui->run_scr = lv_obj_create(NULL);
    lv_obj_set_size(ui->run_scr, 480, 320);
    lv_obj_set_scrollbar_mode(ui->run_scr, LV_SCROLLBAR_MODE_OFF);

    //Write style for run_scr, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->run_scr, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->run_scr, lv_color_hex(0x4e4e56), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->run_scr, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes run_scr_bullet_status_cont
    ui->run_scr_bullet_status_cont = lv_obj_create(ui->run_scr);
    lv_obj_set_pos(ui->run_scr_bullet_status_cont, 250, 72);
    lv_obj_set_size(ui->run_scr_bullet_status_cont, 210, 230);
    lv_obj_set_scrollbar_mode(ui->run_scr_bullet_status_cont, LV_SCROLLBAR_MODE_OFF);

    //Write style for run_scr_bullet_status_cont, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->run_scr_bullet_status_cont, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->run_scr_bullet_status_cont, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->run_scr_bullet_status_cont, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->run_scr_bullet_status_cont, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->run_scr_bullet_status_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->run_scr_bullet_status_cont, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->run_scr_bullet_status_cont, lv_color_hex(0x4e4e56), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->run_scr_bullet_status_cont, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->run_scr_bullet_status_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->run_scr_bullet_status_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->run_scr_bullet_status_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->run_scr_bullet_status_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->run_scr_bullet_status_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes run_scr_bullet_anim
    /*ui->run_scr_bullet_anim = lv_lott lv_rlottie_create_from_raw(ui->run_scr_bullet_status_cont, 140, 140, (const void *)lottie_running_gear_animation);
    lv_obj_set_pos(ui->run_scr_bullet_anim, 35, 30);
    lv_obj_set_size(ui->run_scr_bullet_anim, 140, 140);

    //Write style for run_scr_bullet_anim, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->run_scr_bullet_anim, 0, LV_PART_MAIN|LV_STATE_DEFAULT);*/

    //Write codes run_scr_bullet_sw
    ui->run_scr_bullet_sw = lv_switch_create(ui->run_scr_bullet_status_cont);
    lv_obj_set_pos(ui->run_scr_bullet_sw, 45, 180);
    lv_obj_set_size(ui->run_scr_bullet_sw, 120, 40);

    //Write style for run_scr_bullet_sw, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->run_scr_bullet_sw, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->run_scr_bullet_sw, lv_color_hex(0x8e8ea3), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->run_scr_bullet_sw, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->run_scr_bullet_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->run_scr_bullet_sw, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->run_scr_bullet_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for run_scr_bullet_sw, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
    lv_obj_set_style_bg_opa(ui->run_scr_bullet_sw, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui->run_scr_bullet_sw, lv_color_hex(0xC06528), LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui->run_scr_bullet_sw, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui->run_scr_bullet_sw, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

    //Write style for run_scr_bullet_sw, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->run_scr_bullet_sw, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->run_scr_bullet_sw, lv_color_hex(0xefe8e4), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->run_scr_bullet_sw, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->run_scr_bullet_sw, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->run_scr_bullet_sw, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes run_scr_bullet_label
    ui->run_scr_bullet_label = lv_label_create(ui->run_scr_bullet_status_cont);
    lv_obj_set_pos(ui->run_scr_bullet_label, 45, 0);
    lv_obj_set_size(ui->run_scr_bullet_label, 120, 20);
    lv_label_set_text(ui->run_scr_bullet_label, "BULLET FEED");
    lv_label_set_long_mode(ui->run_scr_bullet_label, LV_LABEL_LONG_WRAP);

    //Write style for run_scr_bullet_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->run_scr_bullet_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->run_scr_bullet_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->run_scr_bullet_label, lv_color_hex(0xEFE8E4), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->run_scr_bullet_label, &lv_font_Mostra_Nuova_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->run_scr_bullet_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->run_scr_bullet_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->run_scr_bullet_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->run_scr_bullet_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->run_scr_bullet_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->run_scr_bullet_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->run_scr_bullet_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->run_scr_bullet_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->run_scr_bullet_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->run_scr_bullet_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes run_scr_case_status_cont
    ui->run_scr_case_status_cont = lv_obj_create(ui->run_scr);
    lv_obj_set_pos(ui->run_scr_case_status_cont, 20, 70);
    lv_obj_set_size(ui->run_scr_case_status_cont, 210, 230);
    lv_obj_set_scrollbar_mode(ui->run_scr_case_status_cont, LV_SCROLLBAR_MODE_OFF);

    //Write style for run_scr_case_status_cont, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->run_scr_case_status_cont, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->run_scr_case_status_cont, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->run_scr_case_status_cont, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->run_scr_case_status_cont, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->run_scr_case_status_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->run_scr_case_status_cont, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->run_scr_case_status_cont, lv_color_hex(0x4e4e56), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->run_scr_case_status_cont, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->run_scr_case_status_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->run_scr_case_status_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->run_scr_case_status_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->run_scr_case_status_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->run_scr_case_status_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes run_scr_case_anim
    /*ui->run_scr_case_anim = lv_rlottie_create_from_raw(ui->run_scr_case_status_cont, 140, 140, (const void *)lottie_running_gear_animation);
    lv_obj_set_pos(ui->run_scr_case_anim, 35, 30);
    lv_obj_set_size(ui->run_scr_case_anim, 140, 140);

    //Write style for run_scr_case_anim, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->run_scr_case_anim, 0, LV_PART_MAIN|LV_STATE_DEFAULT);*/

    //Write codes run_scr_case_sw
    ui->run_scr_case_sw = lv_switch_create(ui->run_scr_case_status_cont);
    lv_obj_set_pos(ui->run_scr_case_sw, 45, 180);
    lv_obj_set_size(ui->run_scr_case_sw, 120, 40);

    //Write style for run_scr_case_sw, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->run_scr_case_sw, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->run_scr_case_sw, lv_color_hex(0x8e8ea3), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->run_scr_case_sw, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->run_scr_case_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->run_scr_case_sw, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->run_scr_case_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for run_scr_case_sw, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
    lv_obj_set_style_bg_opa(ui->run_scr_case_sw, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui->run_scr_case_sw, lv_color_hex(0xC06528), LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui->run_scr_case_sw, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui->run_scr_case_sw, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

    //Write style for run_scr_case_sw, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->run_scr_case_sw, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->run_scr_case_sw, lv_color_hex(0xefe8e4), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->run_scr_case_sw, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->run_scr_case_sw, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->run_scr_case_sw, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes run_scr_case_label
    ui->run_scr_case_label = lv_label_create(ui->run_scr_case_status_cont);
    lv_obj_set_pos(ui->run_scr_case_label, 50, 0);
    lv_obj_set_size(ui->run_scr_case_label, 110, 20);
    lv_label_set_text(ui->run_scr_case_label, "CASE FEED");
    lv_label_set_long_mode(ui->run_scr_case_label, LV_LABEL_LONG_WRAP);

    //Write style for run_scr_case_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->run_scr_case_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->run_scr_case_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->run_scr_case_label, lv_color_hex(0xEFE8E4), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->run_scr_case_label, &lv_font_Mostra_Nuova_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->run_scr_case_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->run_scr_case_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->run_scr_case_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->run_scr_case_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->run_scr_case_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->run_scr_case_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->run_scr_case_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->run_scr_case_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->run_scr_case_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->run_scr_case_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes run_scr_top_bar
    ui->run_scr_top_bar = lv_obj_create(ui->run_scr);
    lv_obj_set_pos(ui->run_scr_top_bar, 0, 0);
    lv_obj_set_size(ui->run_scr_top_bar, 480, 48);
    lv_obj_set_scrollbar_mode(ui->run_scr_top_bar, LV_SCROLLBAR_MODE_OFF);

    //Write style for run_scr_top_bar, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->run_scr_top_bar, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->run_scr_top_bar, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->run_scr_top_bar, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->run_scr_top_bar, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->run_scr_top_bar, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->run_scr_top_bar, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->run_scr_top_bar, lv_color_hex(0xC06528), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->run_scr_top_bar, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->run_scr_top_bar, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->run_scr_top_bar, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->run_scr_top_bar, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->run_scr_top_bar, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->run_scr_top_bar, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes run_scr_img_1
    /*ui->run_scr_img_1 = lv_image_create(ui->run_scr_top_bar);
    lv_obj_set_pos(ui->run_scr_img_1, 12, 12);
    lv_obj_set_size(ui->run_scr_img_1, 24, 24);
    lv_obj_add_flag(ui->run_scr_img_1, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->run_scr_img_1, &_warning_64dp_EFE8E4_RGB565_24x24);
    lv_image_set_pivot(ui->run_scr_img_1, 50,50);
    lv_image_set_rotation(ui->run_scr_img_1, 0);

    //Write style for run_scr_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->run_scr_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->run_scr_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);*/

    //The custom code of run_scr.


    //Update current screen layout.
    lv_obj_update_layout(ui->run_scr);

    //Init events for screen.
    events_init_run_scr(ui);
}
