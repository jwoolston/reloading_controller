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



void setup_scr_home_scr(lv_ui *ui)
{
    //Write codes home_scr
    ui->home_scr = lv_obj_create(NULL);
    lv_obj_set_size(ui->home_scr, 480, 320);
    lv_obj_set_scrollbar_mode(ui->home_scr, LV_SCROLLBAR_MODE_OFF);

    //Write style for home_scr, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->home_scr, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->home_scr, lv_color_hex(0x4e4e56), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->home_scr, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes home_scr_top_bar
    ui->home_scr_top_bar = lv_obj_create(ui->home_scr);
    lv_obj_set_pos(ui->home_scr_top_bar, 0, 0);
    lv_obj_set_size(ui->home_scr_top_bar, 480, 48);
    lv_obj_set_scrollbar_mode(ui->home_scr_top_bar, LV_SCROLLBAR_MODE_OFF);

    //Write style for home_scr_top_bar, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->home_scr_top_bar, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->home_scr_top_bar, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->home_scr_top_bar, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->home_scr_top_bar, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_scr_top_bar, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->home_scr_top_bar, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->home_scr_top_bar, lv_color_hex(0xC06528), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->home_scr_top_bar, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->home_scr_top_bar, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->home_scr_top_bar, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->home_scr_top_bar, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->home_scr_top_bar, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->home_scr_top_bar, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes home_scr_settings_container
    ui->home_scr_settings_container = lv_obj_create(ui->home_scr);
    lv_obj_set_pos(ui->home_scr_settings_container, 325, 85);
    lv_obj_set_size(ui->home_scr_settings_container, 140, 200);
    lv_obj_set_scrollbar_mode(ui->home_scr_settings_container, LV_SCROLLBAR_MODE_OFF);

    //Write style for home_scr_settings_container, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->home_scr_settings_container, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->home_scr_settings_container, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->home_scr_settings_container, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->home_scr_settings_container, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_scr_settings_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->home_scr_settings_container, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->home_scr_settings_container, lv_color_hex(0x4e4e56), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->home_scr_settings_container, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->home_scr_settings_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->home_scr_settings_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->home_scr_settings_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->home_scr_settings_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->home_scr_settings_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes home_scr_settings_label
    ui->home_scr_settings_label = lv_label_create(ui->home_scr_settings_container);
    lv_obj_set_pos(ui->home_scr_settings_label, 0, 142);
    lv_obj_set_size(ui->home_scr_settings_label, 140, 32);
    lv_label_set_text(ui->home_scr_settings_label, "SETTINGS");
    lv_label_set_long_mode(ui->home_scr_settings_label, LV_LABEL_LONG_WRAP);

    //Write style for home_scr_settings_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->home_scr_settings_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_scr_settings_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->home_scr_settings_label, lv_color_hex(0xefe8e4), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->home_scr_settings_label, &lv_font_Mostra_Nuova_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->home_scr_settings_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->home_scr_settings_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->home_scr_settings_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->home_scr_settings_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->home_scr_settings_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->home_scr_settings_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->home_scr_settings_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->home_scr_settings_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->home_scr_settings_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->home_scr_settings_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes home_scr_settings_icon
    /*ui->home_scr_settings_icon = lv_image_create(ui->home_scr_settings_container);
    lv_obj_set_pos(ui->home_scr_settings_icon, 0, 0);
    lv_obj_set_size(ui->home_scr_settings_icon, 140, 140);
    lv_obj_add_flag(ui->home_scr_settings_icon, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->home_scr_settings_icon, &_settings_128dp_EFE8E4_RGB565_140x140);
    lv_image_set_pivot(ui->home_scr_settings_icon, 50,50);
    lv_image_set_rotation(ui->home_scr_settings_icon, 0);

    //Write style for home_scr_settings_icon, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->home_scr_settings_icon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor(ui->home_scr_settings_icon, lv_color_hex(0xefe8e4), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->home_scr_settings_icon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);*/

    //Write codes home_scr_run_container
    ui->home_scr_run_container = lv_obj_create(ui->home_scr);
    lv_obj_set_pos(ui->home_scr_run_container, 170, 85);
    lv_obj_set_size(ui->home_scr_run_container, 140, 200);
    lv_obj_set_scrollbar_mode(ui->home_scr_run_container, LV_SCROLLBAR_MODE_OFF);

    //Write style for home_scr_run_container, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->home_scr_run_container, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->home_scr_run_container, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->home_scr_run_container, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->home_scr_run_container, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_scr_run_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->home_scr_run_container, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->home_scr_run_container, lv_color_hex(0x4e4e56), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->home_scr_run_container, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->home_scr_run_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->home_scr_run_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->home_scr_run_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->home_scr_run_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->home_scr_run_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes home_scr_run_label
    ui->home_scr_run_label = lv_label_create(ui->home_scr_run_container);
    lv_obj_set_pos(ui->home_scr_run_label, 0, 143);
    lv_obj_set_size(ui->home_scr_run_label, 140, 32);
    lv_label_set_text(ui->home_scr_run_label, "RUN");
    lv_label_set_long_mode(ui->home_scr_run_label, LV_LABEL_LONG_WRAP);

    //Write style for home_scr_run_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->home_scr_run_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_scr_run_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->home_scr_run_label, lv_color_hex(0xefe8e4), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->home_scr_run_label, &lv_font_Mostra_Nuova_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->home_scr_run_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->home_scr_run_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->home_scr_run_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->home_scr_run_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->home_scr_run_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->home_scr_run_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->home_scr_run_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->home_scr_run_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->home_scr_run_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->home_scr_run_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes home_scr_run_icon
    /*ui->home_scr_run_icon = lv_image_create(ui->home_scr_run_container);
    lv_obj_set_pos(ui->home_scr_run_icon, 0, 0);
    lv_obj_set_size(ui->home_scr_run_icon, 140, 140);
    lv_obj_add_flag(ui->home_scr_run_icon, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->home_scr_run_icon, &_play_circle_outline_128dp_EFE8E4_RGB565_140x140);
    lv_image_set_pivot(ui->home_scr_run_icon, 50,50);
    lv_image_set_rotation(ui->home_scr_run_icon, 0);

    //Write style for home_scr_run_icon, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->home_scr_run_icon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor(ui->home_scr_run_icon, lv_color_hex(0xefe8e4), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->home_scr_run_icon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);*/

    //Write codes home_scr_stats_container
    ui->home_scr_stats_container = lv_obj_create(ui->home_scr);
    lv_obj_set_pos(ui->home_scr_stats_container, 15, 85);
    lv_obj_set_size(ui->home_scr_stats_container, 140, 200);
    lv_obj_set_scrollbar_mode(ui->home_scr_stats_container, LV_SCROLLBAR_MODE_OFF);

    //Write style for home_scr_stats_container, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->home_scr_stats_container, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->home_scr_stats_container, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->home_scr_stats_container, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->home_scr_stats_container, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_scr_stats_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->home_scr_stats_container, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->home_scr_stats_container, lv_color_hex(0x4e4e56), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->home_scr_stats_container, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->home_scr_stats_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->home_scr_stats_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->home_scr_stats_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->home_scr_stats_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->home_scr_stats_container, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes home_scr_stats_label
    ui->home_scr_stats_label = lv_label_create(ui->home_scr_stats_container);
    lv_obj_set_pos(ui->home_scr_stats_label, 0, 143);
    lv_obj_set_size(ui->home_scr_stats_label, 140, 32);
    lv_label_set_text(ui->home_scr_stats_label, "STATS\n");
    lv_label_set_long_mode(ui->home_scr_stats_label, LV_LABEL_LONG_WRAP);

    //Write style for home_scr_stats_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->home_scr_stats_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_scr_stats_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->home_scr_stats_label, lv_color_hex(0xefe8e4), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->home_scr_stats_label, &lv_font_Mostra_Nuova_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->home_scr_stats_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->home_scr_stats_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->home_scr_stats_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->home_scr_stats_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->home_scr_stats_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->home_scr_stats_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->home_scr_stats_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->home_scr_stats_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->home_scr_stats_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->home_scr_stats_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes home_scr_stats_icon
    /*ui->home_scr_stats_icon = lv_image_create(ui->home_scr_stats_container);
    lv_obj_set_pos(ui->home_scr_stats_icon, 0, 0);
    lv_obj_set_size(ui->home_scr_stats_icon, 140, 140);
    lv_obj_add_flag(ui->home_scr_stats_icon, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->home_scr_stats_icon, &_insights_128dp_EFE8E4_RGB565_140x140);
    lv_image_set_pivot(ui->home_scr_stats_icon, 50,50);
    lv_image_set_rotation(ui->home_scr_stats_icon, 0);

    //Write style for home_scr_stats_icon, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->home_scr_stats_icon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor(ui->home_scr_stats_icon, lv_color_hex(0xefe8e4), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->home_scr_stats_icon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);*/

    //The custom code of home_scr.


    //Update current screen layout.
    lv_obj_update_layout(ui->home_scr);

    //Init events for screen.
    events_init_home_scr(ui);
}
