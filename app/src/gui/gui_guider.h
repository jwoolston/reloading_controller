/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"


typedef struct
{
  
	lv_obj_t *splash_scr;
	bool splash_scr_del;
	lv_obj_t *splash_scr_splash_logo;
	lv_obj_t *home_scr;
	bool home_scr_del;
	lv_obj_t *home_scr_top_bar;
	lv_obj_t *home_scr_settings_container;
	lv_obj_t *home_scr_settings_label;
	lv_obj_t *home_scr_settings_icon;
	lv_obj_t *home_scr_run_container;
	lv_obj_t *home_scr_run_label;
	lv_obj_t *home_scr_run_icon;
	lv_obj_t *home_scr_stats_container;
	lv_obj_t *home_scr_stats_label;
	lv_obj_t *home_scr_stats_icon;
	lv_obj_t *settings_scr;
	bool settings_scr_del;
	lv_obj_t *settings_scr_top_bar;
	lv_obj_t *run_scr;
	bool run_scr_del;
	lv_obj_t *run_scr_bullet_status_cont;
	lv_obj_t *run_scr_bullet_anim;
	lv_obj_t *run_scr_bullet_sw;
	lv_obj_t *run_scr_bullet_label;
	lv_obj_t *run_scr_case_status_cont;
	lv_obj_t *run_scr_case_anim;
	lv_obj_t *run_scr_case_sw;
	lv_obj_t *run_scr_case_label;
	lv_obj_t *run_scr_top_bar;
	lv_obj_t *run_scr_img_1;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_screen_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_animation(void * var, uint32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                  uint32_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                  lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_completed_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);


void init_scr_del_flag(lv_ui *ui);

void setup_bottom_layer(void);

void setup_ui(lv_ui *ui);

void video_play(lv_ui *ui);

void init_keyboard(lv_ui *ui);

extern lv_ui guider_ui;


void setup_scr_splash_scr(lv_ui *ui);
void setup_scr_home_scr(lv_ui *ui);
void setup_scr_settings_scr(lv_ui *ui);
void setup_scr_run_scr(lv_ui *ui);
LV_IMAGE_DECLARE(_AP_Logo_RGB565_480x320);
LV_IMAGE_DECLARE(_settings_128dp_EFE8E4_RGB565_140x140);
LV_IMAGE_DECLARE(_play_circle_outline_128dp_EFE8E4_RGB565_140x140);
LV_IMAGE_DECLARE(_insights_128dp_EFE8E4_RGB565_140x140);
#define LV_LOT_DECLARE(array_name) extern const uint8_t array_name[];
LV_LOT_DECLARE(lottie_running_gear_animation);
#define LV_LOT_DECLARE(array_name) extern const uint8_t array_name[];
LV_LOT_DECLARE(lottie_running_gear_animation);
LV_IMAGE_DECLARE(_warning_64dp_EFE8E4_RGB565_24x24);

LV_FONT_DECLARE(lv_font_Mostra_Nuova_32)
LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_Mostra_Nuova_24)


#ifdef __cplusplus
}
#endif
#endif
