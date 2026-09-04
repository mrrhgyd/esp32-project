#ifndef _BSP_BOARD_H_
#define _BSP_BOARD_H_


#ifdef __cplusplus
extern "C" {
#endif
#if defined(ESP_PLATFORM)

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"          //ESP-IDF官方底层的SPI主机驱动头文件
#include "driver/i2c_master.h"          //ESP-IDF官方底层的I2C总线驱动头文件
#include "driver/i2c.h"
#include "driver/gpio.h"                //ESP_IDF基础GPIO驱动，用于控制复位，中断，背光等普通引脚
#include "esp_timer.h"          
#include "esp_lcd_panel_io.h"           //ESP-IDF全新液晶屏IO抽象层，封装了发送指令和数据的底层细节
#include "esp_lcd_panel_vendor.h"       //提供各大屏幕厂商驱动支持
#include "esp_lcd_panel_ops.h"          //提供对液晶屏的标准操作API
#include "esp_lcd_touch_ft5x06.h"       //ESP-IDF 框架中用于驱动 FT5x06 系列电容触摸芯片 的官方驱动头文件（FT6336兼容 FT5x06）
#include "esp_lcd_ili9341.h"            //ESP-IDF 框架中用于驱动 ili9341屏幕驱动 的官方驱动头文件
#include <time.h>                       //C 标准时间库，提供 time()、clock()、struct tm 等基础时间函数
#include <sys/time.h>                   //POSIX 扩展时间库，提供 gettimeofday()、settimeofday()、struct timeval（含秒+微秒的高精度时间）
#include "freertos/event_groups.h"      //FreeRTOS 事件组，用于任务间同步。一个 32 位标志位集合，多个任务可以设置/等待不同比特位，实现复杂的状态通知（如"WiFi 已连接" + "SNTP 已同步"同时满足后才继续）
#include "esp_timer.h"                  //ESP-IDF 高精度软件定时器（基于系统滴答，非硬件定时器）。用于创建单次/周期回调，比 FreeRTOS 软件定时器更轻量、更精确
#include "esp_wifi.h"                   //引入乐鑫官方wifi状态机库
#include "esp_event.h"                  //核心事件分发网关
#include "nvs_flash.h"        
#include "esp_sntp.h"                   //引入网络对时系统（SNTP）服务组件
#include "lvgl.h"
#include "stdbool.h"
#include "esp_bt.h"                     //蓝牙控制器和vhci设置头文件
#include "esp_bt_main.h"                //蓝牙栈空间初始化头文件
#include "esp_gap_ble_api.h"            //GAP设置头文件，广播和连接相关参数配置
#include "esp_gattc_api.h"              //GATT配置头文件，创建service和characteristic
#include "esp_gatt_common_api.h"        //GATT通用定义

// --- 请根据你的硬件修改以下引脚 ---
#define LCD_HOST       SPI2_HOST                //使用ESP32-S3的SPI独立硬件控制屏（也叫GPSPI）
#define LCD_PIXEL_CLK  (40 * 1000 * 1000)       //液晶屏SPI时钟频率，配置SPI总线传输速率40MHz
#define LCD_PIN_NUM_MOSI 11                     //主出从入
#define LCD_PIN_NUM_MISO 13
#define LCD_PIN_NUM_CLK  12
#define LCD_PIN_NUM_CS   10
#define LCD_PIN_NUM_DC   46
#define LCD_PIN_NUM_RST  -1                     // 如果接到硬件复位填-1
#define LCD_PIN_NUM_BLK  45                      //背光电平，控制屏幕亮度

#define I2C_HOST       I2C_NUM_0
#define TOUCH_PIN_SDA  16
#define TOUCH_PIN_SCL  15
#define TOUCH_PIN_INT  17
#define TOUCH_PIN_RST  18

#define H_RES 240                               //水平像素
#define V_RES 320                               //垂直像素

#define WIFI_SSID           "PRK66"              //wifi名称
#define WIFI_PASS           "13808850203"        //wifi密码

/**
 * @brief 刷屏回调 (ESP32)，当 UI 有变化时，LVGL 会自动把算好的点阵推送到这里
 * @param drv 显示驱动
 * @param area 矩形区域结构体指针
 * @param color_map 像素颜色数组指针
 */
void disp_flush(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map);

/**
 * @brief LVGL 触摸回调 (ESP32)，每隔大约 30ms，LVGL 会来这里“查岗”看有没有被戳
 */
void touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);

/**
 * @brief 初始化 ili9341 屏幕硬件并注册到 LVGL
 */
void bsp_lcd_ili9341_init(void);

/**
 * @brief 初始化 FT6336 屏幕硬件并注册到 LVGL
 */
void bsp_touch_ft6336_init(void);

/**
 * @brief 联网组件初始化主配置
 */
void bsp_wifi_sntp_init(void);

/**
 * @brief 自动获取 SNTP 网络时间并格式化刷新到 UI，日期后自动附带中文星期几
 */
void bsp_update_time(void);

/**
 * @brief 屏幕休眠和唤醒
 */
void bsp_screen_dormancy(void);

/**
 * @brief //开启蓝牙
 */
void bsp_bt_ble_start(void);

 /**
 * @brief //关闭蓝牙
 */
void bsp_bt_ble_stop(void);

/**
 * @brief 发送信息
 * @param data 发送数据
 * @param len 发送数据长度
 */
void bsp_bt_ble_send(uint8_t *data,size_t len);

/**
 * @brief 检测蓝牙是否连接成功
 * @return 
 *  -ture 连接成功 
 *  -false 连接失败
 */
bool bsp_bt_is_connect(void);

/**
 * @brief 接收信息
 */
void bsp_bt_ble_receive(void *pvParmeters);

/**
 * @brief 状态机解析
 * @param data 需要解析的数据
 */
void statemachine_parse(uint8_t &data);
#endif

/**
 * @brief 初始化和建立组件
 */
void screen_init();

#ifdef __cplusplus
}
#endif



#endif