#include "pure_pursuit.hpp"
#include "car_comm.hpp"
#include <stdio.h>
#ifdef ESP_PLATFORM
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#endif
// 全局控制器指针 (在 app_main 中初始化)
PurePursuit* g_pure_pursuit_ptr = nullptr;


// 构造函数
PurePursuit::PurePursuit() 
    : has_target_(false), 
      trajectory_complete_(false),
      target_reached_count_(0) {
    reset();
}

// 初始化配置
void PurePursuit::init(const Config& config) {
    config_ = config;
    printf(  "controller init: Ld=%.3f, v=%.2f, tol=%.2f\n", 
             config_.lookahead_distance, config_.base_linear_speed, 
             config_.target_tolerance);
}

// 重置状态
void PurePursuit::reset(float x, float y, float theta) {
    state_.x = x;
    state_.y = y;
    state_.theta = theta;
    state_.v = 0.0f;
    state_.w = 0.0f;
    has_target_ = false;
    trajectory_complete_ = false;
    target_reached_count_ = 0;
    g_trajectory_queue.clear();
    printf( "status reset: (%.2f, %.2f, %.2f)\n", x, y, theta);
}

// 外部设置状态 (真实里程计回传)
void PurePursuit::setState(float x, float y, float theta) {
    state_.x = x;
    state_.y = y;
    state_.theta = theta;
}


// 角度规范化
float PurePursuit::normalizeAngle(float angle) {
    while (angle > (float)M_PI) angle -= 2.0f * (float)M_PI;
    while (angle < -(float)M_PI) angle += 2.0f * (float)M_PI;
    return angle;
}

/**
 * @brief Pure Pursuit 核心算法
 * 
 * 公式: w = 2*v*sin(alpha) / Ld
 * 
 * 其中:
 * - v: 线速度
 * - alpha: 车辆朝向与目标点的夹角
 * - Ld: 预瞄距离 (使用实际距离作为自适应预瞄)
 */
void PurePursuit::computeControl(const Point2D& target, float& v, float& w) {
    // 计算向量差
    float dx = target.x - state_.x;
    float dy = target.y - state_.y;
    float dist = sqrtf(dx * dx + dy * dy);
    
    // 计算目标相对角度
    float angle_to_target = atan2f(dy, dx);
    float alpha = angle_to_target - state_.theta;
    alpha = normalizeAngle(alpha);
    
    // 自适应预瞄距离: 距离越远，Ld 越大 (但不超过配置值)
    float adaptive_ld = config_.lookahead_distance;
    if (dist > config_.lookahead_distance) {
        adaptive_ld = dist;  // 远距离直接跟随
    }
    
    // Pure Pursuit 角速度计算
    w = (2.0f * config_.base_linear_speed * sinf(alpha)) / adaptive_ld;
    
    // 角速度限幅
    if (w > config_.max_angular_speed) w = config_.max_angular_speed;
    if (w < -config_.max_angular_speed) w = -config_.max_angular_speed;
    
    // 根据转弯幅度调整线速度 (转弯大时减速)
    float v_factor = 1.0f - config_.speed_factor * fabsf(alpha);
    if (v_factor < 0.3f) v_factor = 0.3f;  // 最小 30% 速度
    v = config_.base_linear_speed * v_factor;
    
    // 最小速度限制
    if (v < config_.min_linear_speed) v = config_.min_linear_speed;
    
    // 距离容差内减速
    if (dist < config_.target_tolerance * 3.0f) {
        float slow_factor = dist / (config_.target_tolerance * 3.0f);
        if (slow_factor < 0.2f) slow_factor = 0.2f;
        v *= slow_factor;
    }
}

// 更新虚拟里程计
void PurePursuit::updateOdometry(float v, float w, float dt) {
    state_.theta += w * dt;
    state_.x += v * cosf(state_.theta) * dt;
    state_.y += v * sinf(state_.theta) * dt;
    state_.v = v;
    state_.w = w;
    
    // 角度规范化
    state_.theta = normalizeAngle(state_.theta);
}

/**
 * @brief 主控制循环
 * 
 * 执行顺序:
 * 1. 从无锁队列获取新轨迹点
 * 2. 计算到目标点的距离
 * 3. 到达目标后取下一个点
 * 4. 计算控制指令 (v, w)
 * 5. 虚拟里程计更新
 * 6. 蓝牙发送指令给 STM32
 */
PurePursuit::ControlCommand PurePursuit::update() {
    ControlCommand cmd;
    cmd.is_valid = false;
    
    // 1. 如果没有目标点，尝试从队列获取
    if (!has_target_) {
        Point2D new_target;
        if (g_trajectory_queue.pop(new_target)) {
            current_target_ = new_target;
            has_target_ = true;
            target_reached_count_ = 0;
            trajectory_complete_ = false;  // 轨迹未完成
            printf( "new target: (%.2f, %.2f)\n", new_target.x, new_target.y);
        } else {
            // 队列为空
            trajectory_complete_ = true;
            cmd.mode = 0;  // 停止
            cmd.v_cmd = 0;
            cmd.w_cmd = 128;  // 中心值 (无转向)
            cmd.is_valid = false;
            return cmd;
        }
    }
    
    // 2. 计算当前距离
    #ifdef ESP_PLATFORM
    float dx = current_target_.x - state_.x;
    float dy = current_target_.y - state_.y;
    float dist = sqrtf(dx * dx + dy * dy);
    #elif USE_SDL
    float dx = 0;
    float dy = 0;
    float dist = sqrtf(dx * dx + dy * dy);
    #endif
    
    // 3. 检查是否到达目标
    if (dist < config_.target_tolerance) {
        target_reached_count_++;
        
        if (target_reached_count_ >= 2) {  // 连续 2 次确认到达
            has_target_ = false;
            target_reached_count_ = 0;
            
            // 尝试获取下一个点
            Point2D next_target;
            if (g_trajectory_queue.pop(next_target)) {
                current_target_ = next_target;
                has_target_ = true;
                printf(  "switch next target: (%.2f, %.2f)\n", 
                         next_target.x, next_target.y);
            } else {
                // 轨迹完成
                trajectory_complete_ = true;
                printf( "trace finish\n!");
                
                // 发送停止指令
                cmd.mode = 0;
                cmd.v_cmd = 0;
                cmd.w_cmd = 128;
                cmd.is_valid = true;
                return cmd;
            }
        }
    }
    
    // 4. 计算控制量
    float v, w;
    computeControl(current_target_, v, w);
    
    // 5. 更新虚拟里程计 (20ms 周期)
    float dt = 1.0f / config_.control_frequency;
    updateOdometry(v, w, dt);
    
    // 6. 映射为蓝牙协议格式
    // 协议: mode | v(0-255) | w(128 中心, 0-255)
    // v: 实际范围 0.0 - 0.5 m/s, 映射到 0 - 250
    // w: 实际范围 -3.0 - 3.0 rad/s, 映射到 0 - 255 (128 为中心)
    cmd.mode = (trajectory_complete_) ? 0 : 1;
    cmd.v_cmd = (uint8_t)(v * 500.0f);  // 0-250
    if (cmd.v_cmd > 250) cmd.v_cmd = 250;
    
    // 角速度映射: -max 到 +max -> 0 到 255
    float w_norm = (w + config_.max_angular_speed) / (2.0f * config_.max_angular_speed);
    cmd.w_cmd = (uint8_t)(w_norm * 255.0f);
    
    cmd.is_valid = true;
    
    // 定期打印状态 (每 50 次打印一次)
    static int print_counter = 0;
    if (print_counter++ >= 50) {
        print_counter = 0;
        printf(  "status: pos=(%.2f,%.2f,%.1f) v=%.2f w=%.2f target=(%.2f,%.2f) dist=%.3f\n",
                 state_.x, state_.y, state_.theta * 180.0f / (float)M_PI,
                 v, w, current_target_.x, current_target_.y, dist);
    }
    
    return cmd;
}


