#include "car_comm.hpp"
#include "bsp_board.h"
#include "screens.h"
#include <stdio.h>

// 初始化
void car_comm_init(void) {
    printf("vehicle communication interface has init\n");
}

/**
 * @brief 发送控制指令给 STM32
 * 
 * 数据包格式:
 * | 字节 | 内容 | 说明 |
 * |------|------|------|
 * | 0    | 0xAA | 帧头 |
 * | 1    | mode | 控制模式 |
 * | 2    | v_cmd| 线速度 |
 * | 3    | w_cmd| 角速度 |
 * | 4    | 0x55 | 帧尾 (可选，简化协议) |
 */
void car_comm_send_cmd(uint8_t mode, uint8_t v_cmd, uint8_t w_cmd) {
    uint8_t tx_buf[CAR_PROTOCOL_MSG_SIZE] = {
        CAR_PROTOCOL_HEADER,
        mode,
        v_cmd,
        w_cmd,
        CAR_PROTOCOL_TAIL
    };
    
#if defined(ESP_PLATFORM)
    bsp_bt_ble_send(tx_buf, CAR_PROTOCOL_MSG_SIZE);
    printf( "send: mode=%d v=%d w=%d\n", mode, v_cmd, w_cmd);
#elif defined(USE_SDL)
    printf("[BT] mode=%d v=%d w=%d\n", mode, v_cmd, w_cmd);
#endif
}

// 发送停止指令
void car_comm_send_stop(void) {
    car_comm_send_cmd(CAR_MODE_STOP, 0, 128);
    printf( "send stop cmd\n");
}

// 发送紧急停止
void car_comm_send_emergency(void) {
    car_comm_send_cmd(CAR_MODE_EMERGENCY, 0, 128);
    printf( "send urgency stop cmd\n!");
}

// 发送速度
void car_comm_send_speed(uint8_t speed) {
    car_comm_send_cmd(CAR_MODE_DRIVE, speed, 128);
}

