#ifndef _CAR_TRAJECTORY_H_
#define _CAR_TRAJECTORY_H_

#include <lvgl.h>
#include <vector>
#include "lockfree_queue.hpp"

/**
 * @brief 车辆轨迹绘制管理器
 * 
 * Canvas 已在 screens.c 中创建 (objects.obj19)
 * 屏幕 240x320 像素映射为 2.4m x 3.2m 物理空间
 * 
 * 使用：
 * 1.初始化画布，绑定缓存区
 * 2.获取触摸屏上的坐标
 * 3.通过贝塞尔函数生成平滑的坐标点
 * 4.绘制贝塞尔曲线
 * 5. CarTrajectory::processTouch(indev) - 在画布事件中调用
 * 6. CarTrajectory::clear() - 清空画布
 */
class CarTrajectory {
private: 
    #define CANVAS_WIDTH  240
    #define CANVAS_HEIGHT 320
    
/**
 * @brief 构造函数，初始化画布，绑定缓冲区
 */    
    CarTrajectory();
    
    static lv_color_t *canvas_buf;


public:

/**
 * @brief 获取全局单例
 */
    static CarTrajectory &getinstance();

    static std::vector<Point2D> raw_touch_path;//从触摸屏中获取的坐标点

    static std::vector<Point2D>all_points;//经过抽稀算法和贝塞尔曲线得出来的坐标点


/**
 * @brief 清空画布和缓冲区
 */
    void canvas_clear();

    
    void canvas_drawbezier();
};

#endif