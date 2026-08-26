#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_change_screen(lv_event_t * e);
extern void action_get_pin(lv_event_t * e);
extern void action_sure(lv_event_t * e);
extern void action_cancel(lv_event_t * e);
extern void action_back_pre_scr(lv_event_t * e);
extern void action_car_control(lv_event_t * e);
extern void action_switch_onoff(lv_event_t * e);
extern void action_game_jump_control(lv_event_t * e);
extern void action_start_dinosaur_run(lv_event_t * e);
extern void action_canvas_touch(lv_event_t * e);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/