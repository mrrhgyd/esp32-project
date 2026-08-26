#ifndef _CAR_COMM_H_
#define _CAR_COMM_H_

#include <cstdint>

/**
 * @brief 车辆通信接口
 * 
 * 封装与 STM32 的蓝牙通信协议
 * 负责将 Pure Pursuit 的控制指令转换为蓝牙数据包
 */

// 通信协议常量
#define CAR_PROTOCOL_HEADER     0xA5    // 帧头
#define CAR_PROTOCOL_TAIL       0x55    // 帧尾
#define CAR_PROTOCOL_MSG_SIZE   5       // 单帧数据长度

// 控制模式
#define CAR_MODE_STOP           0       // 停止
#define CAR_MODE_DRIVE          1       // 正常行驶
#define CAR_MODE_BACKWARD       2       // 倒车
#define CAR_MODE_EMERGENCY      3       // 紧急停止

/**
 * @brief 初始化通信模块
 */
void car_comm_init(void);

/**
 * @brief 发送控制指令
 * @param mode 控制模式
 * @param v_cmd 线速度命令 (0-250)
 * @param w_cmd 角速度命令 (0-255, 128 为中心)
 */
void car_comm_send_cmd(uint8_t mode, uint8_t v_cmd, uint8_t w_cmd);

/**
 * @brief 发送停止指令
 */
void car_comm_send_stop(void);

/**
 * @brief 发送紧急停止指令
 */
void car_comm_send_emergency(void);

/**
 * @brief 发送当前速度
 */
void car_comm_send_speed(uint8_t speed);



#endif