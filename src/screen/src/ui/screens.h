#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Screens

enum ScreensEnum {
    _SCREEN_ID_FIRST = 1,
    SCREEN_ID_LOCKSCREEN = 1,
    SCREEN_ID_PINSCREEN = 2,
    SCREEN_ID_DESKTOP = 3,
    SCREEN_ID_SETSCREEN = 4,
    SCREEN_ID_USERSCREEN = 5,
    SCREEN_ID_CARSCREEN = 6,
    SCREEN_ID_BLUETOOTHSCREEN = 7,
    SCREEN_ID_DINOSAURSCREEN = 8,
    SCREEN_ID_CARCONTROL = 9,
    _SCREEN_ID_LAST = 9
};

typedef struct _objects_t {
    lv_obj_t *lockscreen;
    lv_obj_t *pinscreen;
    lv_obj_t *desktop;
    lv_obj_t *setscreen;
    lv_obj_t *userscreen;
    lv_obj_t *carscreen;
    lv_obj_t *bluetoothscreen;
    lv_obj_t *dinosaurscreen;
    lv_obj_t *carcontrol;
    lv_obj_t *time;
    lv_obj_t *date;
    lv_obj_t *day;
    lv_obj_t *pin_time;
    lv_obj_t *pin_date;
    lv_obj_t *pin_day;
    lv_obj_t *input1_keyboard;
    lv_obj_t *pin_input;
    lv_obj_t *set;
    lv_obj_t *obj0;
    lv_obj_t *car_control;
    lv_obj_t *app2;
    lv_obj_t *dino_run;
    lv_obj_t *obj1;
    lv_obj_t *car;
    lv_obj_t *fixed_key;
    lv_obj_t *obj2;
    lv_obj_t *home;
    lv_obj_t *back;
    lv_obj_t *user_but;
    lv_obj_t *user_l;
    lv_obj_t *wife_b;
    lv_obj_t *wife_l;
    lv_obj_t *panel;
    lv_obj_t *arc1;
    lv_obj_t *arc2;
    lv_obj_t *arc3;
    lv_obj_t *bluetooth_t;
    lv_obj_t *bluetooth_l;
    lv_obj_t *obj3;
    lv_obj_t *obj4;
    lv_obj_t *obj5;
    lv_obj_t *obj6;
    lv_obj_t *obj7;
    lv_obj_t *user_txt;
    lv_obj_t *pin_set;
    lv_obj_t *obj8;
    lv_obj_t *logout;
    lv_obj_t *obj9;
    lv_obj_t *user_return;
    lv_obj_t *logout_text_hide;
    lv_obj_t *cancel_but;
    lv_obj_t *obj10;
    lv_obj_t *sure_but;
    lv_obj_t *obj11;
    lv_obj_t *input2_keyboard;
    lv_obj_t *pin_input_2;
    lv_obj_t *car_go;
    lv_obj_t *obj12;
    lv_obj_t *car_back;
    lv_obj_t *obj13;
    lv_obj_t *car_left;
    lv_obj_t *obj14;
    lv_obj_t *car_right;
    lv_obj_t *obj15;
    lv_obj_t *car_stop;
    lv_obj_t *speed_input;
    lv_obj_t *set_speed_but;
    lv_obj_t *real_speed;
    lv_obj_t *input3_keyboard;
    lv_obj_t *user_return_blue;
    lv_obj_t *user_txt_1;
    lv_obj_t *obj16;
    lv_obj_t *obj17;
    lv_obj_t *bluetooth_switch;
    lv_obj_t *obj18;
    lv_obj_t *dino_obj;
    lv_obj_t *cacti1;
    lv_obj_t *cacti2;
    lv_obj_t *cacti3;
    lv_obj_t *score_label;
    lv_obj_t *game_over_label;
    lv_obj_t *canvas;
} objects_t;

extern objects_t objects;

void create_screen_lockscreen();
void tick_screen_lockscreen();

void create_screen_pinscreen();
void tick_screen_pinscreen();

void create_screen_desktop();
void tick_screen_desktop();

void create_screen_setscreen();
void tick_screen_setscreen();

void create_screen_userscreen();
void tick_screen_userscreen();

void create_screen_carscreen();
void tick_screen_carscreen();

void create_screen_bluetoothscreen();
void tick_screen_bluetoothscreen();

void create_screen_dinosaurscreen();
void tick_screen_dinosaurscreen();

void create_screen_carcontrol();
void tick_screen_carcontrol();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/