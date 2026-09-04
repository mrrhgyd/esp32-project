#ifndef _PURE_PURSUIT_H_
#define _PURE_PURSUIT_H_

#include <cmath>
#include "lockfree_queue.hpp"

/**
 * @brief Pure Pursuit (纯跟踪) 控制器
 * 
 * 在 Core 0 (PRO_CPU) 上运行，频率 50Hz
 * 从无锁队列中获取轨迹点，计算车辆的控制指令 (线速度 v, 角速度 w)
 * 
 * 控制原理:
 * - 从轨迹队列中依次取出目标点
 * - 计算车辆当前位置到目标点的距离和方向
 * - 使用纯跟踪公式计算需要的角速度
 * - 通过蓝牙将控制指令发送给 STM32
 */
class PurePursuit {
public:
    /**
     * @brief 控制参数
     */
    struct Config {
        float lookahead_distance;  // 预瞄距离 Ld (米)
        float base_linear_speed;   // 基准线速度 v (m/s)
        float target_tolerance;    // 到达目标的容差半径 (米)
        float control_frequency;  // 控制频率 (Hz)
        float max_angular_speed;  // 最大角速度限制 (rad/s)
        float min_linear_speed;   // 最小线速度 (m/s)
        float speed_factor;       // 根据转弯半径调整速度的系数
    };

    /**
     * @brief 车辆状态
     */
    struct VehicleState {
        float x;      // 位置 x (米)
        float y;      // 位置 y (米)
        float theta;  // 朝向角度 (弧度)
        float v;      // 当前线速度 (m/s)
        float w;      // 当前角速度 (rad/s)
    };

    /**
     * @brief 车辆控制指令
     */
    struct ControlCommand {
        uint8_t mode;    // 控制模式 (0: 停止, 1: 行驶, 2: 倒车)
        uint8_t v_cmd;   // 线速度命令 (0-255, 映射自速度)
        uint8_t w_cmd;   // 角速度命令 (0-255, 128 为中心, 映射自角速度)
        bool is_valid;   // 指令是否有效
    };
    /**
     * @brief stm32回传的数据
     * @param bcc 异或校验位
     */
    struct receivecmd
    {
       uint8_t v;
       uint8_t w;
       uint8_t yaw;
       uint8_t bcc;
       bool is_read;
    };
    
    PurePursuit();
    
    /**
     * @brief 初始化控制器
     * @param config 控制参数配置
     */
    void init(const Config& config);

    /**
     * @brief 重置车辆状态
     * @param x 初始位置 x
     * @param y 初始位置 y
     * @param theta 初始朝向
     */
    void reset(float x = 0.0f, float y = 0.0f, float theta = 0.0f);

    /**
     * @brief 执行一次控制周期 (在 Core 0 上以 50Hz 调用)
     * @return 本次的控制指令
     */
    ControlCommand update();

    /**
     * @brief 获取当前车辆状态
     */
    const VehicleState& getState() const { return state_; }

    /**
     * @brief 设置车辆状态 (用于外部里程计数据回传)
     */
    void setState(float dt);


    /**
     * @brief 检查是否已完成所有轨迹点
     */
    bool isTrajectoryComplete() const { return trajectory_complete_; }
    receivecmd read_cmd;

private:
    /**
     * @brief 计算 Pure Pursuit 控制输出
     * @param target 目标点坐标
     * @param v 输出线速度
     * @param w 输出角速度
     */
    void computeControl(const Point2D& target, float& v, float& w);

    /**
     * @brief 更新虚拟里程计 (Dead Reckoning)
     * @param v 线速度
     * @param w 角速度
     * @param dt 时间步长
     */
    void updateOdometry(float v, float w, float dt);

    /**
     * @brief 角度规范化到 [-PI, PI]
     */
    static float normalizeAngle(float angle);

    Config config_;
    VehicleState state_;
    Point2D current_target_;
    bool has_target_;
    bool trajectory_complete_;
    int target_reached_count_;  // 连续到达计数
};

// 全局控制器指针 (在 app_main 中初始化，其他文件通过此指针访问)
extern PurePursuit* g_pure_pursuit_ptr;

#endif