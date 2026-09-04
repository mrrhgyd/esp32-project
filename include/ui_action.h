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
/**
 * @brief 切换屏幕事件
 * @param e 传递事件信息
 */
void action_change_screen(lv_event_t *e);

/**
 * @brief 获取数字按键输入
 * @param e 传递事件信息
 */
void action_get_pin(lv_event_t *e);

/**
 * @brief 翻转父组件下所有子件的显隐状态
 * @param parent 父组件
 */
void ui_toggle(lv_obj_t *parent);

/**
 * @brief 更新输出显示
 * @param e 传递事件信息
 */
void display_update(lv_obj_t *e);

/**
 * @brief 确认
 * @param e 传递事件信息
 */
void action_sure(lv_event_t *e);

/**
 * @brief 取消
 * @param e 传递事件信息
 */
void action_cancel(lv_event_t *e);

/**
 * @brief 加载界面，将当前界面放入栈中
 * @param curscreen 需要加载的界面
 */
void ui_navigate_to(lv_obj_t *curscreen);

/**
 * @brief 返回上一级页面
 * @param e 传递事件信息
 */
void action_back_pre_scr(lv_event_t *e);

/**
 * @brief 保存密码到nvs
 * @param pin 需要保存的密码
 */
void nvs_save_correct_pin(const char *pin);

/**
 * @brief 从nvs中读取密码
 */
void nvs_load_correct_pin(void);

/**
 * @brief //小车控制
 * @param e 传递事件信息
 */
void action_car_control(lv_event_t *e);

/**
 * @brief //switch开关控制
 * @param e 传递事件信息
 */
void action_switch_onoff(lv_event_t *e);

/**
 * @brief //打开恐龙快跑游戏
 * @param e 传递事件信息
 */
void action_start_dinosaur_run(lv_event_t *e);

/**
 * @brief //可控制恐龙起跳
 * @param e 传递事件信息
 */
void action_game_jump_control(lv_event_t *e);

/**
 * @brief 绘制曲线，传送命令
 * @param e 传递事件信息
 */
void action_canvas_touch(lv_event_t * e);

/**
 * 密码
 */
extern char correct_pin[7];

#ifdef __cplusplus
}
#endif


#endif