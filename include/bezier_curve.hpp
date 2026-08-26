#ifndef _BEZIER_CURVE_H_
#define _BEZIER_CURVE_H_

#include <cmath>
#include <vector>
#include "lockfree_queue.hpp"

/**
 * @brief 贝塞尔曲线生成器
 * 
 * 负责在 Core 1 (APP_CPU) 上生成轨迹点，通过无锁队列发送给 Core 0
 * 支持二次贝塞尔曲线 (Quadratic Bezier) 和三次贝塞尔曲线 (Cubic Bezier)
 */
class BezierCurve {
public:
    /**
     * @brief 二次贝塞尔曲线参数
     */
    struct QuadraticBezier {
        Point2D p0;  // 起点
        Point2D p1;  // 控制点
        Point2D p2;  // 终点
    };

    /**
     * @brief 三次贝塞尔曲线参数
     */
    struct CubicBezier {
        Point2D p0;  // 起点
        Point2D p1;  // 控制点 1
        Point2D p2;  // 控制点 2
        Point2D p3;  // 终点
    };

    /**
     * @brief 生成二次贝塞尔曲线的所有轨迹点
     * @param curve 曲线参数
     * @param num_points 采样点数量 (越大越平滑)
     * @param points 输出轨迹点数组
     */
    static void generateQuadraticPoints(const QuadraticBezier& curve, 
                                         int num_points,
                                         std::vector<Point2D>& points);

    /**
     * @brief 生成三次贝塞尔曲线的所有轨迹点
     * @param curve 曲线参数
     * @param num_points 采样点数量
     * @param points 输出轨迹点数组
     */
    static void generateCubicPoints(const CubicBezier& curve, 
                                     int num_points,
                                     std::vector<Point2D>& points);

    /**
     * @brief 计算二次贝塞尔曲线上某个参数 t 对应的点
     * @param curve 曲线参数
     * @param t 参数 [0.0, 1.0]
     * @return 曲线上的点
     */
    static Point2D quadraticPoint(const QuadraticBezier& curve, float t);

    /**
     * @brief 计算三次贝塞尔曲线上某个参数 t 对应的点
     * @param curve 曲线参数
     * @param t 参数 [0.0, 1.0]
     * @return 曲线上的点
     */
    static Point2D cubicPoint(const CubicBezier& curve, float t);

    /**
     * @brief 将生成的轨迹点推入无锁队列
     * @param points 轨迹点数组
     * @return 成功推入的点数量
     */
    static int pushToQueue(const std::vector<Point2D>& points);

    /**
     * @brief 清空轨迹队列并推入新轨迹
     * @param points 新的轨迹点数组
     * @return 成功推入的点数量
     */
    static int replaceTrajectory(const std::vector<Point2D>& points);

    /**
     * @brief 从触摸屏轨迹生成贝塞尔曲线并推送到队列
     * @param touch_points 触摸屏捕捉到的原始点序列
     * @param num_beziers 生成的贝塞尔曲线数量
     * @return 生成的轨迹点总数
     */
    static int generateFromTouchPath(const std::vector<Point2D>& touch_points,std::vector<Point2D>&all_point,
                                      int points_per_curve = 50);

private:
    /**
     * @brief 估算两点之间的距离
     */
    static float distance(const Point2D& a, const Point2D& b);

    /**
     * @brief 在两点之间线性插值
     */
    static Point2D lerp(const Point2D& a, const Point2D& b, float t);
};

#endif