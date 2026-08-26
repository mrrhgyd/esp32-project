#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;

//
// Event handlers
//

lv_obj_t *tick_value_change_obj;

//
// Screens
//

void create_screen_lockscreen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.lockscreen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 320);
    lv_obj_add_event_cb(obj, action_change_screen, LV_EVENT_GESTURE, (void *)0);
    lv_obj_set_style_text_letter_space(obj, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(obj, &img_image_2, LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // time
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.time = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 40);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_letter_space(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "time");
        }
        {
            // date
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.date = obj;
            lv_obj_set_pos(obj, 0, 40);
            lv_obj_set_size(obj, 240, 30);
            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_letter_space(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "date");
        }
        {
            // day
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.day = obj;
            lv_obj_set_pos(obj, 0, 70);
            lv_obj_set_size(obj, 240, 30);
            lv_obj_set_style_text_font(obj, &ui_font_ui_font_chinese_16, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_letter_space(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "day");
        }
    }
    
    tick_screen_lockscreen();
}

void tick_screen_lockscreen() {
}

void create_screen_pinscreen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.pinscreen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 320);
    lv_obj_add_event_cb(obj, action_change_screen, LV_EVENT_GESTURE, (void *)0);
    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(obj, &img_image_9, LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // pin_time
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.pin_time = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 40);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "time");
        }
        {
            // pin_date
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.pin_date = obj;
            lv_obj_set_pos(obj, 0, 40);
            lv_obj_set_size(obj, 240, 30);
            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "date");
        }
        {
            // pin_day
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.pin_day = obj;
            lv_obj_set_pos(obj, 0, 70);
            lv_obj_set_size(obj, 240, 30);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &ui_font_ui_font_chinese_16, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Text");
        }
        {
            // input1_keyboard
            lv_obj_t *obj = lv_btnmatrix_create(parent_obj);
            objects.input1_keyboard = obj;
            lv_obj_set_pos(obj, 0, 143);
            lv_obj_set_size(obj, 240, 177);
            static const char *map[16] = {
                "1",
                "2",
                "3",
                "\n",
                "4",
                "5",
                "6",
                "\n",
                "7",
                "8",
                "9",
                "\n",
                "back",
                "0",
                "del",
                NULL,
            };
            static lv_btnmatrix_ctrl_t ctrl_map[12] = {
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
            };
            lv_btnmatrix_set_map(obj, map);
            lv_btnmatrix_set_ctrl_map(obj, ctrl_map);
            lv_obj_add_event_cb(obj, action_get_pin, LV_EVENT_CLICKED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
        {
            // pin_input
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.pin_input = obj;
            lv_obj_set_pos(obj, 0, 111);
            lv_obj_set_size(obj, 240, 32);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_text_color(obj, lv_color_hex(0x212121), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_38, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "******");
        }
    }
    
    tick_screen_pinscreen();
}

void tick_screen_pinscreen() {
}

void create_screen_desktop() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.desktop = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 320);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xf5f5f5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(obj, &img_image_7, LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 320);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // set
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.set = obj;
                    lv_obj_set_pos(obj, 21, 47);
                    lv_obj_set_size(obj, 50, 50);
                    lv_obj_add_event_cb(obj, action_change_screen, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_img_src(obj, &img_set, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_clip_corner(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj0 = obj;
                    lv_obj_set_pos(obj, -74, -54);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_ui_font_chinese_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "设置");
                }
                {
                    // car_control
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.car_control = obj;
                    lv_obj_set_pos(obj, 98, 47);
                    lv_obj_set_size(obj, 50, 50);
                    lv_obj_add_event_cb(obj, action_change_screen, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
                    lv_obj_set_style_radius(obj, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_clip_corner(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_img_src(obj, &img_car, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // app2
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.app2 = obj;
                    lv_obj_set_pos(obj, 3, -54);
                    lv_obj_set_size(obj, 64, 19);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_ui_font_chinese_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "小车控制");
                }
                {
                    // dino_run
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.dino_run = obj;
                    lv_obj_set_pos(obj, 175, 47);
                    lv_obj_set_size(obj, 50, 50);
                    lv_obj_add_event_cb(obj, action_start_dinosaur_run, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_set_style_radius(obj, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_clip_corner(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_img_src(obj, &img_dinosaur, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj1 = obj;
                    lv_obj_set_pos(obj, 80, -54);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_ui_font_chinese_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "恐龙快跑");
                }
                {
                    // car
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.car = obj;
                    lv_obj_set_pos(obj, 21, 122);
                    lv_obj_set_size(obj, 50, 50);
                    lv_obj_add_event_cb(obj, action_change_screen, LV_EVENT_CLICKED, (void *)0);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, -74, 20);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Button");
                }
            }
        }
        {
            // fixed_key
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.fixed_key = obj;
            lv_obj_set_pos(obj, 0, 300);
            lv_obj_set_size(obj, 240, 21);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    lv_obj_set_pos(obj, 160, 0);
                    lv_obj_set_size(obj, 20, 20);
                    static lv_point_t line_points[] = {
                        { 0, 10 },
                        { 20, 20 },
                        { 20, 0 },
                        { 0, 10 }
                    };
                    lv_line_set_points(obj, line_points, 4);
                    lv_line_set_y_invert(obj, true);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    lv_obj_set_style_line_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    lv_obj_t *obj = lv_arc_create(parent_obj);
                    objects.obj2 = obj;
                    lv_obj_set_pos(obj, 80, 0);
                    lv_obj_set_size(obj, 20, 20);
                    lv_arc_set_range(obj, 99, 100);
                    lv_arc_set_value(obj, 100);
                    lv_arc_set_bg_start_angle(obj, 0);
                    lv_arc_set_bg_end_angle(obj, 360);
                    lv_obj_set_style_arc_width(obj, 3, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_color(obj, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // home
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.home = obj;
                    lv_obj_set_pos(obj, 70, -20);
                    lv_obj_set_size(obj, 40, 40);
                    lv_obj_add_event_cb(obj, action_change_screen, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // back
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.back = obj;
                    lv_obj_set_pos(obj, 150, -20);
                    lv_obj_set_size(obj, 40, 40);
                    lv_obj_add_event_cb(obj, action_back_pre_scr, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
    }
    
    tick_screen_desktop();
}

void tick_screen_desktop() {
}

void create_screen_setscreen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.setscreen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 320);
    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_SCROLLED);
    lv_obj_set_style_bg_img_src(obj, &img_image_8, LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 320);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_ACTIVE);
            lv_obj_set_scroll_dir(obj, LV_DIR_VER);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_COLUMN, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // user_but
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.user_but = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, 240, 50);
                    lv_obj_add_event_cb(obj, action_change_screen, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            lv_obj_set_pos(obj, 206, 7);
                            lv_obj_set_size(obj, 10, 20);
                            static lv_point_t line_points[] = {
                                { 0, 20 },
                                { 10, 10 },
                                { 0, 0 }
                            };
                            lv_line_set_points(obj, line_points, 3);
                            lv_line_set_y_invert(obj, true);
                        }
                        {
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            lv_obj_set_pos(obj, -5, 2);
                            lv_obj_set_size(obj, 30, 30);
                            lv_img_set_src(obj, &img_image_1);
                        }
                        {
                            // user_l
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.user_l = obj;
                            lv_obj_set_pos(obj, -58, 5);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_ui_font_chinese_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_ui_font_chinese_16, LV_PART_MAIN | LV_STATE_SCROLLED);
                            lv_label_set_text_static(obj, "用户");
                        }
                    }
                }
                {
                    // wife_b
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.wife_b = obj;
                    lv_obj_set_pos(obj, 0, 50);
                    lv_obj_set_size(obj, 240, 50);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // wife_l
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.wife_l = obj;
                            lv_obj_set_pos(obj, -49, 2);
                            lv_obj_set_size(obj, 45, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "WLAN");
                        }
                        {
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            lv_obj_set_pos(obj, 206, 8);
                            lv_obj_set_size(obj, 10, 20);
                            static lv_point_t line_points[] = {
                                { 0, 20 },
                                { 10, 10 },
                                { 0, 0 }
                            };
                            lv_line_set_points(obj, line_points, 3);
                            lv_line_set_y_invert(obj, true);
                        }
                        {
                            // panel
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.panel = obj;
                            lv_obj_set_pos(obj, 10, 27);
                            lv_obj_set_size(obj, 8, 8);
                            lv_arc_set_range(obj, 0, 20);
                            lv_arc_set_value(obj, 20);
                            lv_arc_set_bg_start_angle(obj, 225);
                            lv_arc_set_bg_end_angle(obj, 315);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
                        }
                        {
                            // arc1
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.arc1 = obj;
                            lv_obj_set_pos(obj, 6, 19);
                            lv_obj_set_size(obj, 15, 15);
                            lv_arc_set_range(obj, 0, 20);
                            lv_arc_set_value(obj, 20);
                            lv_arc_set_bg_start_angle(obj, 225);
                            lv_arc_set_bg_end_angle(obj, 315);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_width(obj, 3, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // arc2
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.arc2 = obj;
                            lv_obj_set_pos(obj, 1, 12);
                            lv_obj_set_size(obj, 25, 25);
                            lv_arc_set_range(obj, 0, 20);
                            lv_arc_set_value(obj, 20);
                            lv_arc_set_bg_start_angle(obj, 225);
                            lv_arc_set_bg_end_angle(obj, 315);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_width(obj, 3, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // arc3
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.arc3 = obj;
                            lv_obj_set_pos(obj, -9, 5);
                            lv_obj_set_size(obj, 45, 45);
                            lv_arc_set_range(obj, 0, 20);
                            lv_arc_set_value(obj, 20);
                            lv_arc_set_bg_start_angle(obj, 225);
                            lv_arc_set_bg_end_angle(obj, 315);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_width(obj, 3, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_arc_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    // bluetooth_t
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.bluetooth_t = obj;
                    lv_obj_set_pos(obj, 0, 100);
                    lv_obj_set_size(obj, 240, 50);
                    lv_obj_add_event_cb(obj, action_change_screen, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            lv_obj_set_pos(obj, -5, 3);
                            lv_obj_set_size(obj, 30, 30);
                            lv_img_set_src(obj, &img_bluetooth);
                            lv_obj_set_style_radius(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_clip_corner(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            lv_obj_set_pos(obj, 206, 9);
                            lv_obj_set_size(obj, 10, 20);
                            static lv_point_t line_points[] = {
                                { 0, 20 },
                                { 10, 10 },
                                { 0, 0 }
                            };
                            lv_line_set_points(obj, line_points, 3);
                            lv_line_set_y_invert(obj, true);
                        }
                        {
                            // bluetooth_l
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.bluetooth_l = obj;
                            lv_obj_set_pos(obj, -58, 1);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_ui_font_chinese_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "蓝牙");
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 150);
                    lv_obj_set_size(obj, 240, 50);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.obj3 = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "user");
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 200);
                    lv_obj_set_size(obj, 240, 50);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.obj4 = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "user");
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 250);
                    lv_obj_set_size(obj, 240, 50);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.obj5 = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "user");
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 300);
                    lv_obj_set_size(obj, 240, 50);
                    lv_obj_set_style_bg_opa(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.obj6 = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "user");
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 350);
                    lv_obj_set_size(obj, 240, 50);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.obj7 = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, 32, 16);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "user");
                        }
                    }
                }
            }
        }
    }
    
    tick_screen_setscreen();
}

void tick_screen_setscreen() {
}

void create_screen_userscreen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.userscreen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 320);
    lv_obj_set_style_bg_img_src(obj, &img_image_3, LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // user_txt
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.user_txt = obj;
            lv_obj_set_pos(obj, 34, 18);
            lv_obj_set_size(obj, 53, 24);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &ui_font_ui_font_chinese_16, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "用户");
        }
        {
            // pin_set
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.pin_set = obj;
            lv_obj_set_pos(obj, 0, 57);
            lv_obj_set_size(obj, 240, 50);
            lv_obj_add_event_cb(obj, action_change_screen, LV_EVENT_CLICKED, (void *)0);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj8 = obj;
                    lv_obj_set_pos(obj, -67, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_ui_font_chinese_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "密码设置");
                }
                {
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    lv_obj_set_pos(obj, 197, 9);
                    lv_obj_set_size(obj, 10, 20);
                    static lv_point_t line_points[] = {
                        { 0, 20 },
                        { 10, 10 },
                        { 0, 0 }
                    };
                    lv_line_set_points(obj, line_points, 3);
                    lv_line_set_y_invert(obj, true);
                }
            }
        }
        {
            // logout
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.logout = obj;
            lv_obj_set_pos(obj, 0, 107);
            lv_obj_set_size(obj, 240, 50);
            lv_obj_add_event_cb(obj, action_change_screen, LV_EVENT_CLICKED, (void *)0);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj9 = obj;
                    lv_obj_set_pos(obj, -80, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_ui_font_chinese_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "注销");
                }
                {
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    lv_obj_set_pos(obj, 197, 9);
                    lv_obj_set_size(obj, 10, 20);
                    static lv_point_t line_points[] = {
                        { 0, 20 },
                        { 10, 10 },
                        { 0, 0 }
                    };
                    lv_line_set_points(obj, line_points, 3);
                    lv_line_set_y_invert(obj, true);
                }
            }
        }
        {
            // user_return
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.user_return = obj;
            lv_obj_set_pos(obj, 10, 12);
            lv_obj_set_size(obj, 30, 30);
            lv_obj_add_event_cb(obj, action_back_pre_scr, LV_EVENT_CLICKED, (void *)0);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
                {
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    lv_obj_set_pos(obj, -13, -8);
                    lv_obj_set_size(obj, 30, 30);
                    static lv_point_t line_points[] = {
                        { 0, 15 },
                        { 15, 30 },
                        { 0, 15 },
                        { 15, 0 },
                        { 0, 15 },
                        { 30, 15 }
                    };
                    lv_line_set_points(obj, line_points, 6);
                    lv_line_set_y_invert(obj, true);
                }
            }
        }
        {
            // logout_text_hide
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.logout_text_hide = obj;
            lv_obj_set_pos(obj, 0, 207);
            lv_obj_set_size(obj, 240, 113);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    lv_obj_set_pos(obj, 120, 63);
                    lv_obj_set_size(obj, 0, 50);
                    static lv_point_t line_points[] = {
                        { 0, 0 },
                        { 0, 50 }
                    };
                    lv_line_set_points(obj, line_points, 2);
                }
                {
                    // cancel_but
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.cancel_but = obj;
                    lv_obj_set_pos(obj, 120, 63);
                    lv_obj_set_size(obj, 120, 50);
                    lv_obj_add_event_cb(obj, action_cancel, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.obj10 = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_ui_font_chinese_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "取消");
                        }
                    }
                }
                {
                    // sure_but
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.sure_but = obj;
                    lv_obj_set_pos(obj, 0, 63);
                    lv_obj_set_size(obj, 120, 50);
                    lv_obj_add_event_cb(obj, action_sure, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.obj11 = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_ui_font_chinese_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "确定");
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, 240, 23);
                    lv_obj_set_style_text_font(obj, &ui_font_ui_font_chinese_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "是否确定注销账号");
                }
            }
        }
        {
            // input2_keyboard
            lv_obj_t *obj = lv_btnmatrix_create(parent_obj);
            objects.input2_keyboard = obj;
            lv_obj_set_pos(obj, 0, 143);
            lv_obj_set_size(obj, 240, 177);
            static const char *map[16] = {
                "1",
                "2",
                "3",
                "\n",
                "4",
                "5",
                "6",
                "\n",
                "7",
                "8",
                "9",
                "\n",
                "back",
                "0",
                "del",
                NULL,
            };
            static lv_btnmatrix_ctrl_t ctrl_map[12] = {
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
            };
            lv_btnmatrix_set_map(obj, map);
            lv_btnmatrix_set_ctrl_map(obj, ctrl_map);
            lv_obj_add_event_cb(obj, action_get_pin, LV_EVENT_CLICKED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_OVERFLOW_VISIBLE|LV_OBJ_FLAG_HIDDEN);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, -15, -95);
                    lv_obj_set_size(obj, 240, 31);
                    lv_obj_set_style_text_font(obj, &ui_font_ui_font_chinese_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "请输入六位数字密码");
                }
                {
                    // pin_input_2
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pin_input_2 = obj;
                    lv_obj_set_pos(obj, -15, -65);
                    lv_obj_set_size(obj, 240, 32);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_38, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "******");
                }
            }
        }
    }
    
    tick_screen_userscreen();
}

void tick_screen_userscreen() {
}

void create_screen_carscreen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.carscreen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 320);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xa9a9b5), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &ui_font_ui_font_chinese_16, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "注意:使用该软件需要打开蓝牙");
        }
        {
            // car_go
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.car_go = obj;
            lv_obj_set_pos(obj, 97, 97);
            lv_obj_set_size(obj, 46, 45);
            lv_obj_add_event_cb(obj, action_car_control, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_car_control, LV_EVENT_RELEASED, (void *)0);
            lv_obj_add_event_cb(obj, action_car_control, LV_EVENT_PRESS_LOST, (void *)0);
            lv_obj_set_style_clip_corner(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 23, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    objects.obj12 = obj;
                    lv_obj_set_pos(obj, -13, -8);
                    lv_obj_set_size(obj, 46, 46);
                    static lv_point_t line_points[] = {
                        { 23, 46 },
                        { 0, 23 },
                        { 23, 46 },
                        { 46, 23 },
                        { 23, 46 },
                        { 23, 0 },
                        { 23, 46 }
                    };
                    lv_line_set_points(obj, line_points, 7);
                    lv_line_set_y_invert(obj, true);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // car_back
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.car_back = obj;
            lv_obj_set_pos(obj, 97, 217);
            lv_obj_set_size(obj, 46, 46);
            lv_obj_add_event_cb(obj, action_car_control, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_car_control, LV_EVENT_RELEASED, (void *)0);
            lv_obj_add_event_cb(obj, action_car_control, LV_EVENT_PRESS_LOST, (void *)0);
            lv_obj_set_style_radius(obj, 23, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_clip_corner(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    objects.obj13 = obj;
                    lv_obj_set_pos(obj, -13, -8);
                    lv_obj_set_size(obj, 46, 46);
                    static lv_point_t line_points[] = {
                        { 23, 46 },
                        { 0, 23 },
                        { 23, 46 },
                        { 46, 23 },
                        { 23, 46 },
                        { 23, 0 },
                        { 23, 46 }
                    };
                    lv_line_set_points(obj, line_points, 7);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // car_left
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.car_left = obj;
            lv_obj_set_pos(obj, 37, 157);
            lv_obj_set_size(obj, 46, 46);
            lv_obj_add_event_cb(obj, action_car_control, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_car_control, LV_EVENT_PRESS_LOST, (void *)0);
            lv_obj_add_event_cb(obj, action_car_control, LV_EVENT_RELEASED, (void *)0);
            lv_obj_set_style_shadow_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 23, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_clip_corner(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    objects.obj14 = obj;
                    lv_obj_set_pos(obj, -13, -8);
                    lv_obj_set_size(obj, 46, 46);
                    static lv_point_t line_points[] = {
                        { 0, 23 },
                        { 23, 46 },
                        { 0, 23 },
                        { 23, 0 },
                        { 0, 23 },
                        { 46, 23 },
                        { 0, 23 }
                    };
                    lv_line_set_points(obj, line_points, 7);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // car_right
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.car_right = obj;
            lv_obj_set_pos(obj, 157, 157);
            lv_obj_set_size(obj, 46, 46);
            lv_obj_add_event_cb(obj, action_car_control, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_car_control, LV_EVENT_PRESS_LOST, (void *)0);
            lv_obj_add_event_cb(obj, action_car_control, LV_EVENT_RELEASED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 23, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_clip_corner(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    objects.obj15 = obj;
                    lv_obj_set_pos(obj, -13, -8);
                    lv_obj_set_size(obj, 46, 46);
                    static lv_point_t line_points[] = {
                        { 46, 23 },
                        { 23, 46 },
                        { 46, 23 },
                        { 23, 0 },
                        { 46, 23 },
                        { 0, 23 },
                        { 46, 23 }
                    };
                    lv_line_set_points(obj, line_points, 7);
                    lv_line_set_y_invert(obj, true);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // car_stop
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.car_stop = obj;
            lv_obj_set_pos(obj, 97, 157);
            lv_obj_set_size(obj, 46, 46);
            lv_obj_add_event_cb(obj, action_car_control, LV_EVENT_PRESSED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 23, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_clip_corner(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "STOP");
                }
            }
        }
        {
            // speed_input
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.speed_input = obj;
            lv_obj_set_pos(obj, 0, 46);
            lv_obj_set_size(obj, 240, 26);
            lv_label_set_text_static(obj, "set_speed:0");
        }
        {
            // set_speed_but
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.set_speed_but = obj;
            lv_obj_set_pos(obj, 0, 46);
            lv_obj_set_size(obj, 240, 26);
            lv_obj_add_event_cb(obj, action_change_screen, LV_EVENT_CLICKED, (void *)0);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // real_speed
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.real_speed = obj;
            lv_obj_set_pos(obj, 0, 72);
            lv_obj_set_size(obj, 240, 25);
            lv_label_set_text_static(obj, "real_speed:");
        }
        {
            // input3_keyboard
            lv_obj_t *obj = lv_btnmatrix_create(parent_obj);
            objects.input3_keyboard = obj;
            lv_obj_set_pos(obj, 0, 143);
            lv_obj_set_size(obj, 240, 177);
            static const char *map[20] = {
                "1",
                "2",
                "3",
                "\n",
                "4",
                "5",
                "6",
                "\n",
                "7",
                "8",
                "9",
                "\n",
                "back",
                "0",
                "del",
                "\n",
                "Btn",
                "sure",
                "Btn",
                NULL,
            };
            static lv_btnmatrix_ctrl_t ctrl_map[15] = {
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_HIDDEN,
                1 | LV_BTNMATRIX_CTRL_CLICK_TRIG,
                1 | LV_BTNMATRIX_CTRL_HIDDEN,
            };
            lv_btnmatrix_set_map(obj, map);
            lv_btnmatrix_set_ctrl_map(obj, ctrl_map);
            lv_obj_add_event_cb(obj, action_get_pin, LV_EVENT_CLICKED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
    }
    
    tick_screen_carscreen();
}

void tick_screen_carscreen() {
}

void create_screen_bluetoothscreen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.bluetoothscreen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 320);
    lv_obj_set_style_bg_img_src(obj, &img_image_5, LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // user_return_blue
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.user_return_blue = obj;
            lv_obj_set_pos(obj, 10, 12);
            lv_obj_set_size(obj, 30, 30);
            lv_obj_add_event_cb(obj, action_back_pre_scr, LV_EVENT_CLICKED, (void *)0);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
                {
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    lv_obj_set_pos(obj, -13, -8);
                    lv_obj_set_size(obj, 30, 30);
                    static lv_point_t line_points[] = {
                        { 0, 15 },
                        { 15, 30 },
                        { 0, 15 },
                        { 15, 0 },
                        { 0, 15 },
                        { 30, 15 }
                    };
                    lv_line_set_points(obj, line_points, 6);
                    lv_line_set_y_invert(obj, true);
                }
            }
        }
        {
            // user_txt_1
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.user_txt_1 = obj;
            lv_obj_set_pos(obj, 34, 18);
            lv_obj_set_size(obj, 53, 24);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &ui_font_ui_font_chinese_16, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "用户");
        }
        {
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.obj16 = obj;
            lv_obj_set_pos(obj, 0, 63);
            lv_obj_set_size(obj, 240, 30);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xf7f7f7), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj17 = obj;
                    lv_obj_set_pos(obj, -84, 0);
                    lv_obj_set_size(obj, 52, 21);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_ui_font_chinese_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "蓝牙");
                }
            }
        }
        {
            // bluetooth_switch
            lv_obj_t *obj = lv_switch_create(parent_obj);
            objects.bluetooth_switch = obj;
            lv_obj_set_pos(obj, 182, 68);
            lv_obj_set_size(obj, 50, 20);
            lv_obj_add_event_cb(obj, action_switch_onoff, LV_EVENT_CLICKED, (void *)0);
        }
    }
    
    tick_screen_bluetoothscreen();
}

void tick_screen_bluetoothscreen() {
}

void create_screen_dinosaurscreen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.dinosaurscreen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 320);
    lv_obj_add_event_cb(obj, action_game_jump_control, LV_EVENT_CLICKED, (void *)0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xf0f0f0), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_line_create(parent_obj);
            objects.obj18 = obj;
            lv_obj_set_pos(obj, 0, 250);
            lv_obj_set_size(obj, 240, 1);
            static lv_point_t line_points[] = {
                { 0, 0 },
                { 240, 0 }
            };
            lv_line_set_points(obj, line_points, 2);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_EVENT_BUBBLE);
            lv_obj_set_style_line_color(obj, lv_color_hex(0x333333), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // dino_obj
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.dino_obj = obj;
            lv_obj_set_pos(obj, 30, 220);
            lv_obj_set_size(obj, 30, 30);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_EVENT_BUBBLE);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x333333), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_clip_corner(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // cacti1
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.cacti1 = obj;
            lv_obj_set_pos(obj, 220, 210);
            lv_obj_set_size(obj, 20, 40);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_EVENT_BUBBLE|LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x228b22), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_clip_corner(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // cacti2
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.cacti2 = obj;
            lv_obj_set_pos(obj, 220, 210);
            lv_obj_set_size(obj, 20, 40);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_EVENT_BUBBLE|LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x228b22), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_clip_corner(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // cacti3
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.cacti3 = obj;
            lv_obj_set_pos(obj, 220, 210);
            lv_obj_set_size(obj, 20, 40);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_EVENT_BUBBLE|LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x228b22), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_clip_corner(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // score_label
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.score_label = obj;
            lv_obj_set_pos(obj, 95, 20);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_EVENT_BUBBLE);
            lv_label_set_text_static(obj, "score:0");
        }
        {
            // game_over_label
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.game_over_label = obj;
            lv_obj_set_pos(obj, 73, 104);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_EVENT_BUBBLE|LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "game over\ntap to restart");
        }
    }
    
    tick_screen_dinosaurscreen();
}

void tick_screen_dinosaurscreen() {
}

void create_screen_carcontrol() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.carcontrol = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 320);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    {
        lv_obj_t *parent_obj = obj;
        {
            // canvas
            lv_obj_t *obj = lv_canvas_create(parent_obj);
            objects.canvas = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 320);
            lv_obj_add_event_cb(obj, action_canvas_touch, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_canvas_touch, LV_EVENT_PRESSING, (void *)0);
            lv_obj_add_event_cb(obj, action_canvas_touch, LV_EVENT_PRESS_LOST, (void *)0);
            lv_obj_add_event_cb(obj, action_canvas_touch, LV_EVENT_RELEASED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x352c71), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
    
    tick_screen_carcontrol();
}

void tick_screen_carcontrol() {
}

typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_lockscreen,
    tick_screen_pinscreen,
    tick_screen_desktop,
    tick_screen_setscreen,
    tick_screen_userscreen,
    tick_screen_carscreen,
    tick_screen_bluetoothscreen,
    tick_screen_dinosaurscreen,
    tick_screen_carcontrol,
};
void tick_screen(int screen_index) {
    if (screen_index >= 0 && screen_index < 9) {
        tick_screen_funcs[screen_index]();
    }
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen(screenId - 1);
}

//
// Fonts
//

ext_font_desc_t fonts[] = {
    { "ui_font_chinese_16", &ui_font_ui_font_chinese_16 },
#if LV_FONT_MONTSERRAT_8
    { "MONTSERRAT_8", &lv_font_montserrat_8 },
#endif
#if LV_FONT_MONTSERRAT_10
    { "MONTSERRAT_10", &lv_font_montserrat_10 },
#endif
#if LV_FONT_MONTSERRAT_12
    { "MONTSERRAT_12", &lv_font_montserrat_12 },
#endif
#if LV_FONT_MONTSERRAT_14
    { "MONTSERRAT_14", &lv_font_montserrat_14 },
#endif
#if LV_FONT_MONTSERRAT_16
    { "MONTSERRAT_16", &lv_font_montserrat_16 },
#endif
#if LV_FONT_MONTSERRAT_18
    { "MONTSERRAT_18", &lv_font_montserrat_18 },
#endif
#if LV_FONT_MONTSERRAT_20
    { "MONTSERRAT_20", &lv_font_montserrat_20 },
#endif
#if LV_FONT_MONTSERRAT_22
    { "MONTSERRAT_22", &lv_font_montserrat_22 },
#endif
#if LV_FONT_MONTSERRAT_24
    { "MONTSERRAT_24", &lv_font_montserrat_24 },
#endif
#if LV_FONT_MONTSERRAT_26
    { "MONTSERRAT_26", &lv_font_montserrat_26 },
#endif
#if LV_FONT_MONTSERRAT_28
    { "MONTSERRAT_28", &lv_font_montserrat_28 },
#endif
#if LV_FONT_MONTSERRAT_30
    { "MONTSERRAT_30", &lv_font_montserrat_30 },
#endif
#if LV_FONT_MONTSERRAT_32
    { "MONTSERRAT_32", &lv_font_montserrat_32 },
#endif
#if LV_FONT_MONTSERRAT_34
    { "MONTSERRAT_34", &lv_font_montserrat_34 },
#endif
#if LV_FONT_MONTSERRAT_36
    { "MONTSERRAT_36", &lv_font_montserrat_36 },
#endif
#if LV_FONT_MONTSERRAT_38
    { "MONTSERRAT_38", &lv_font_montserrat_38 },
#endif
#if LV_FONT_MONTSERRAT_40
    { "MONTSERRAT_40", &lv_font_montserrat_40 },
#endif
#if LV_FONT_MONTSERRAT_42
    { "MONTSERRAT_42", &lv_font_montserrat_42 },
#endif
#if LV_FONT_MONTSERRAT_44
    { "MONTSERRAT_44", &lv_font_montserrat_44 },
#endif
#if LV_FONT_MONTSERRAT_46
    { "MONTSERRAT_46", &lv_font_montserrat_46 },
#endif
#if LV_FONT_MONTSERRAT_48
    { "MONTSERRAT_48", &lv_font_montserrat_48 },
#endif
};

//
// Color themes
//

uint32_t active_theme_index = 0;

//
//
//

void create_screens() {

// Set default LVGL theme
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    // Initialize screens
    // Create screens
    create_screen_lockscreen();
    create_screen_pinscreen();
    create_screen_desktop();
    create_screen_setscreen();
    create_screen_userscreen();
    create_screen_carscreen();
    create_screen_bluetoothscreen();
    create_screen_dinosaurscreen();
    create_screen_carcontrol();
}