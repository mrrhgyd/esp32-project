#include "bezier_curve.hpp"
#include <stdio.h>
#include <pure_pursuit.hpp>
// 计算二次贝塞尔曲线上某个参数 t 对应的点
Point2D BezierCurve::quadraticPoint(const QuadraticBezier& curve, float t) {
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    
    Point2D result;
    // B(t) = u²·P0 + 2ut·P1 + t²·P2
    result.x = uu * curve.p0.x + 2.0f * u * t * curve.p1.x + tt * curve.p2.x;
    result.y = uu * curve.p0.y + 2.0f * u * t * curve.p1.y + tt * curve.p2.y;
    
    return result;
}

// 计算三次贝塞尔曲线上某个参数 t 对应的点
Point2D BezierCurve::cubicPoint(const CubicBezier& curve, float t) {
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = u * uu;
    float ttt = tt * t;
    
    Point2D result;
    // B(t) = u³·P0 + 3u²t·P1 + 3ut²·P2 + t³·P3
    result.x = uuu * curve.p0.x + 3.0f * uu * t * curve.p1.x + 
               3.0f * u * tt * curve.p2.x + ttt * curve.p3.x;
    result.y = uuu * curve.p0.y + 3.0f * uu * t * curve.p1.y + 
               3.0f * u * tt * curve.p2.y + ttt * curve.p3.y;
    
    return result;
}

// 生成二次贝塞尔曲线的所有轨迹点
void BezierCurve::generateQuadraticPoints(const QuadraticBezier& curve, 
                                            int num_points,
                                            std::vector<Point2D>& points) {
    points.clear();
    points.reserve(num_points);
    
    // 确保至少有 2 个点
    if (num_points < 2) num_points = 2;
    
    float step = 1.0f / (num_points - 1);
    
    for (int i = 0; i < num_points; i++) {
        float t = i * step;
        points.push_back(quadraticPoint(curve, t));
    }
}

// 生成三次贝塞尔曲线的所有轨迹点
void BezierCurve::generateCubicPoints(const CubicBezier& curve, 
                                        int num_points,
                                        std::vector<Point2D>& points) {
    points.clear();
    points.reserve(num_points);
    
    if (num_points < 2) num_points = 2;
    
    float step = 1.0f / (num_points - 1);
    
    for (int i = 0; i < num_points; i++) {
        float t = i * step;
        points.push_back(cubicPoint(curve, t));
    }
}

// 两点距离
float BezierCurve::distance(const Point2D& a, const Point2D& b) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return sqrtf(dx * dx + dy * dy);
}

// 线性插值
Point2D BezierCurve::lerp(const Point2D& a, const Point2D& b, float t) {
    Point2D result;
    result.x = a.x + (b.x - a.x) * t;
    result.y = a.y + (b.y - a.y) * t;
    return result;
}

// 将轨迹点推入无锁队列
int BezierCurve::pushToQueue(const std::vector<Point2D>& points) {
    int pushed_count = 0;
    
    for (const auto& pt : points) {
        if (!g_trajectory_queue.push(pt)) {
            printf(  "queue is full,discard %zu point\n", 
                     points.size() - pushed_count);
            break;
        }
        pushed_count++;
    }
    
    printf( "successfully push %d point into queue\n", pushed_count);
    return pushed_count;
}

// 替换轨迹：清空队列后推入新轨迹（像素 → 米转换 + 控制器重置）
int BezierCurve::replaceTrajectory(const std::vector<Point2D>& points) {
    // 像素坐标 → 物理坐标（米），1px = 1cm = 0.01m
    std::vector<Point2D> physical_points;
    physical_points.reserve(points.size());
    for (const auto& pt : points) {
        physical_points.push_back({pt.x * 0.01f, pt.y * 0.01f});
    }
    
    // 先重置控制器（reset 内部会清空队列），再推入新轨迹
    if (g_pure_pursuit_ptr && !physical_points.empty()) {
        g_pure_pursuit_ptr->reset(physical_points[0].x, physical_points[0].y, 0.0f);
        printf("[Bezier] controller reset to start point: (%.2f, %.2f)\n",
               physical_points[0].x, physical_points[0].y);
    } else {
        g_trajectory_queue.clear();
    }
    
    return pushToQueue(physical_points);
}

/**
 * @brief 从触摸屏路径生成轨迹点（自适应策略）
 *
 * 自适应策略（根据 240×320 屏=2.4m×3.2m → 1px=1cm 设计）：
 *   ① 路径很短 (<20px) 或 点很少 (≤5) → 直接用原始触摸点
 *      （点击一下、画个小拐弯，这种情况贝塞尔纯浪费）
 *   ② 路径中等 → 线性插值补到"每 10px(=10cm=预瞄距离) 一个点"
 *      （PurePursuit 每 20ms 走 ~6mm，10cm 间距正好 1~2 次控制跳一个点）
 *   ③ 路径很长 (>150px) 且点多 (>10) → 才启用贝塞尔平滑
 *      （真正画了大 S 弯、大圆弧时才需要平滑插值）
 *
 * @param touch_points 触摸屏原始点序列
 * @param all_points   输出轨迹点
 * @param points_per_curve 【仅用于情况③】长曲线贝塞尔每段采样数上限（默认不再用硬编码 30）
 * @return 成功推入无锁队列的点数
 */
int BezierCurve::generateFromTouchPath(const std::vector<Point2D>& touch_points,std::vector<Point2D>&all_points,
                                         int points_per_curve) {
    if (touch_points.size() < 2) {
        printf( "insufficient number of touch point (%zu),unable to generate cure\n", touch_points.size());
        return 0;
    }
    
    all_points.clear();
    
    // ========== Step 1: 计算路径总长度 ==========
    float total_length = 0.0f;
    for (size_t i = 1; i < touch_points.size(); i++) {
        total_length += distance(touch_points[i - 1], touch_points[i]);
    }
    
    printf(  "total length of touch path: %.2f, touch_cnt: %zu\n", total_length, touch_points.size());
    
    // ========== Step 2: 情况① → 直接输出原始点 ==========
    if (total_length < 20.0f || touch_points.size() <= 5) {
        all_points = touch_points;
        printf("[Bezier] short trace ,skip bezier\n");
        return replaceTrajectory(all_points);
    }
    
    // ========== Step 3: 计算目标点数（每 10px 一个 = 10cm 实际距离 = 预瞄距离）==========
    // 目标点数最多不超过 min(原始点数 × 3, 总长度/10 + 1)
    int desired_count = std::max((int)touch_points.size(),
                                 (int)(total_length / 10.0f) + 1);
    // 硬上限：防止超大路径爆内存/队列
    desired_count = std::min(desired_count, 200);
    
    // ========== Step 4: 情况② 中等路径 → 线性插值（比贝塞尔便宜 10×，且不扭曲用户画的线）==========
    if (total_length < 150.0f || touch_points.size() <= 10) {
        all_points.reserve(desired_count);
        float accum = 0.0f;           // 已走过的累计长度
        size_t seg_idx = 0;           // 当前在哪两点之间
        float seg_len = 0.0f;         // 当前段长度
        // 初始化第一段
        if (touch_points.size() >= 2) {
            seg_len = distance(touch_points[0], touch_points[1]);
        }
        all_points.push_back(touch_points.front());
        const float step = total_length / (float)(desired_count - 1);
        for (int i = 1; i < desired_count - 1; i++) {
            float target = i * step;
            // 找到目标距离所在的线段
            while (seg_idx + 1 < touch_points.size() - 1 &&
                   accum + seg_len < target) {
                accum += seg_len;
                seg_idx++;
                seg_len = distance(touch_points[seg_idx], touch_points[seg_idx + 1]);
            }
            float local_t = (seg_len > 0.001f) ? (target - accum) / seg_len : 0.0f;
            local_t = local_t < 0.0f ? 0.0f : (local_t > 1.0f ? 1.0f : local_t);
            all_points.push_back(lerp(touch_points[seg_idx],
                                      touch_points[seg_idx + 1], local_t));
        }
        all_points.push_back(touch_points.back());
        printf("[Bezier] mid trace → line interpolation: target%d, real point cnt%zu\n",
               desired_count, all_points.size());
        return replaceTrajectory(all_points);
    }
    
    // ========== Step 5: 情况③ 长路径多拐点 → 贝塞尔平滑（仅这里用贝塞尔）==========
    printf("[Bezier] long trace → use bezier\n");
    int num_segments = std::max(1, (int)(total_length / 80.0f));
    // 每段贝塞尔采样点数：总目标点数 / 段数，且不超过 points_per_curve 上限
    int pts_per_seg = std::min(points_per_curve,std::max(6, desired_count / std::max(1, num_segments)));
    
    if (touch_points.size() == 2) {
        // 只有两个点：生成一条直线
        QuadraticBezier curve = {
            touch_points[0],
            lerp(touch_points[0], touch_points[1], 0.5f),
            touch_points[1]
        };
        generateQuadraticPoints(curve, pts_per_seg, all_points);
    } else {
        // 多个点：生成多段三次贝塞尔曲线
        for (int seg = 0; seg < num_segments; seg++) {
            size_t start_idx = (seg * (touch_points.size() - 1)) / num_segments;
            size_t end_idx = ((seg + 1) * (touch_points.size() - 1)) / num_segments;
            
            // 确保至少 2 个点
            if (end_idx <= start_idx) end_idx = start_idx + 1;
            if (end_idx >= touch_points.size()) end_idx = touch_points.size() - 1;
            
            // 构建三次贝塞尔曲线的 4 个点
            Point2D p0 = touch_points[start_idx];
            Point2D p3 = touch_points[end_idx];
            
            // 计算中间控制点 (基于前一段的斜率)
            Point2D p1, p2;
            
            if (start_idx == 0) {
                // 第一段：控制点 1 为起点，控制点 2 为终点前一点
                p1 = touch_points[start_idx];
                p2 = (end_idx > 0) ? touch_points[end_idx - 1] : lerp(p0, p3, 0.5f);
            } else {
                // 后续段：基于前一段最后一点的切线方向
                size_t prev_end = start_idx - 1;
                Point2D prev_dir = { p0.x - touch_points[prev_end].x,
                                     p0.y - touch_points[prev_end].y };
                
                float dir_len = sqrtf(prev_dir.x * prev_dir.x + prev_dir.y * prev_dir.y);
                if (dir_len > 0.01f) { prev_dir.x /= dir_len; prev_dir.y /= dir_len; }
                
                float ctrl_offset = distance(p0, p3) * 0.3f;
                p1 = { p0.x + prev_dir.x * ctrl_offset,
                       p0.y + prev_dir.y * ctrl_offset };
                p2 = lerp(p0, p3, 0.7f);
            }
            
            CubicBezier curve = {p0, p1, p2, p3};
            std::vector<Point2D> seg_points;
            generateCubicPoints(curve, pts_per_seg, seg_points);
            if (seg > 0 && !seg_points.empty()) seg_points.erase(seg_points.begin());
            all_points.insert(all_points.end(), seg_points.begin(), seg_points.end());
        }
    }
    
    printf("[Bezier] bezier finish: duan=%d, point_cnt=%zu (expect %d)\n",
           num_segments, all_points.size(), desired_count);
    return replaceTrajectory(all_points);
}