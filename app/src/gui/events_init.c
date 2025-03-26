/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif


static void splash_scr_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.home_scr, guider_ui.home_scr_del, &guider_ui.splash_scr_del, setup_scr_home_scr, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, true);
        break;
    }
    case LV_EVENT_PRESSED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.home_scr, guider_ui.home_scr_del, &guider_ui.splash_scr_del, setup_scr_home_scr, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

static void splash_scr_splash_logo_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.home_scr, guider_ui.home_scr_del, &guider_ui.splash_scr_del, setup_scr_home_scr, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, true);
        break;
    }
    case LV_EVENT_PRESSED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.home_scr, guider_ui.home_scr_del, &guider_ui.splash_scr_del, setup_scr_home_scr, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

void events_init_splash_scr (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->splash_scr, splash_scr_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->splash_scr_splash_logo, splash_scr_splash_logo_event_handler, LV_EVENT_ALL, ui);
}

static void home_scr_run_container_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.run_scr, guider_ui.run_scr_del, &guider_ui.home_scr_del, setup_scr_run_scr, LV_SCR_LOAD_ANIM_NONE, 200, 200, false, true);
        break;
    }
    case LV_EVENT_PRESSED:
    {
        break;
    }
    default:
        break;
    }
}

static void home_scr_run_label_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.run_scr, guider_ui.run_scr_del, &guider_ui.home_scr_del, setup_scr_run_scr, LV_SCR_LOAD_ANIM_NONE, 200, 200, false, true);
        ui_load_scr_animation(&guider_ui, &guider_ui.run_scr, guider_ui.run_scr_del, &guider_ui.home_scr_del, setup_scr_run_scr, LV_SCR_LOAD_ANIM_NONE, 200, 200, false, true);
        break;
    }
    case LV_EVENT_PRESSED:
    {
        break;
    }
    default:
        break;
    }
}

static void home_scr_run_icon_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.run_scr, guider_ui.run_scr_del, &guider_ui.home_scr_del, setup_scr_run_scr, LV_SCR_LOAD_ANIM_NONE, 200, 200, false, true);
        break;
    }
    case LV_EVENT_PRESSED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.run_scr, guider_ui.run_scr_del, &guider_ui.home_scr_del, setup_scr_run_scr, LV_SCR_LOAD_ANIM_NONE, 200, 200, false, true);
        break;
    }
    default:
        break;
    }
}

void events_init_home_scr (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->home_scr_run_container, home_scr_run_container_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->home_scr_run_label, home_scr_run_label_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->home_scr_run_icon, home_scr_run_icon_event_handler, LV_EVENT_ALL, ui);
}

static void run_scr_case_anim_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_VALUE_CHANGED:
    {

        break;
    }
    default:
        break;
    }
}

void events_init_run_scr (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->run_scr_case_anim, run_scr_case_anim_event_handler, LV_EVENT_ALL, ui);
}


void events_init(lv_ui *ui)
{

}
