#ifndef UI_ACTION_H
#define UI_ACTION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "screens.h"

/* =========================================================
 * 在这里声明你在 EEZ GUI 中配置的自定义 Action 函数
 * ========================================================= */
void action_change_screen(lv_event_t *e);//切换屏幕
void action_get_pin(lv_event_t *e);//获取数字按键输入
void ui_toggle(lv_obj_t *parent);//翻转父组件下所有子件的显隐状态
void display_update(lv_obj_t *e);//更新输出显示
void action_sure(lv_event_t *e);//确认
void action_cancel(lv_event_t *e);//取消
void ui_navigate_to(lv_obj_t *curscreen);
void action_back_pre_scr(lv_event_t *e);//返回上一级页面
void nvs_save_correct_pin(const char *pin);//保存密码到nvs
void nvs_load_correct_pin(void);//从nvs中读取密码
void action_car_control(lv_event_t *e);//小车控制
void action_switch_onoff(lv_event_t *e);//switch开关控制
void action_start_dinosaur_run(lv_event_t *e);//打开恐龙快跑游戏
void action_game_jump_control(lv_event_t *e);//可控制恐龙起跳
extern char correct_pin[7];

#ifdef __cplusplus
}
#endif


#endif