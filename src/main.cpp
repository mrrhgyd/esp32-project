#include "ui.h"
#include "lvgl.h"
#include <iostream>
#include "ui_action.h"
#include "lockfree_queue.hpp"
#include "bezier_curve.hpp"
#include "pure_pursuit.hpp"
#include "car_comm.hpp"
#include "car_trajectory.hpp"

lv_disp_drv_t *g_disp_drv = NULL; // 暴露给 bsp_board.c
// ==============================================================
// 1. 模拟器 (SDL) 专属包含
// ==============================================================
#if defined(USE_SDL)
    #include "sdl/sdl.h"
    #include <unistd.h>
    #include <time.h>           
    #include <ctime>
    #include "bsp_board.h"


void bsp_update_time(void)
{
    char timer_str[32]={0};
    char date_str[32]={0};
    char day_str[32]={0};

    static const char* weekday_chinese[]={"星期日","星期一","星期二","星期三","星期四","星期五","星期六"};

    //获取系统实时网络时间并更新到标签上
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_s(&timeinfo,&now);

    if(timeinfo.tm_year >(1970-1900))
    {
        snprintf(timer_str,sizeof(timer_str),"%02d:%02d:%02d",timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);
        int wday_idx =timeinfo.tm_wday%7;
        snprintf(date_str,sizeof(date_str),"%d-%02d-%02d",timeinfo.tm_year+1900,timeinfo.tm_mon+1,timeinfo.tm_mday);
        snprintf(day_str,sizeof(day_str),"%s",weekday_chinese[wday_idx]);
        if(objects.time)lv_label_set_text(objects.time,timer_str);
        if(objects.date)lv_label_set_text(objects.date,date_str);
        if(objects.day)lv_label_set_text(objects.day,day_str);
        if(objects.pin_date)lv_label_set_text(objects.pin_time,timer_str);
        if(objects.pin_date)lv_label_set_text(objects.pin_date,date_str);
        if(objects.pin_day)lv_label_set_text(objects.pin_day,day_str);
    }
}

// ==============================================================
// 2. ESP32 硬件专属包含 (ESP-IDF, ST7789, FT6336)
// ==============================================================
#elif defined(ESP_PLATFORM)
    #include "bsp_board.h"
    #include "esp_log.h"
    #include <esp_heap_caps.h>
    #include "rom/ets_sys.h"     // ets_delay_us - 忙等不任务切换
    #include "driver/uart.h"      // uart_tx_wait_idle
    extern "C" void disp_flush(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map);
    extern "C" void touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
    extern "C" void bsp_lcd_ili9341_init(void);
    extern "C" void bsp_touch_ft6336_init(void);
    extern "C" void bsp_wifi_sntp_init(void);
    extern "C" void bsp_update_time(void);
    extern "C" void bsp_screen_dormancy(void);
    extern "C" void screen_init();
    static void lvgl_tick_cb(void *arg) {
    lv_tick_inc(2); // 每次定时器中断，给 LVGL 的寿命增加 2 毫秒
}

    static const char* TAG = "Main";
    
    // ============ Core 1 (PRO_CPU) 任务：Pure Pursuit + 蓝牙下发 ============
    static void PurePursuitTask(void *pvParameters) {
        ESP_LOGI(TAG, "========================================");
        ESP_LOGI(TAG, "Core 1 (PRO_CPU) 启动: Pure Pursuit 任务");
        ESP_LOGI(TAG, "任务核心: PRO_CPU (Core 1)");
        ESP_LOGI(TAG, "频率: 50Hz (20ms)");
        ESP_LOGI(TAG, "========================================");
        
         // 使用全局控制器指针 (已在 app_main 中初始化)
        PurePursuit* controller = g_pure_pursuit_ptr;
        if (!controller) {
            ESP_LOGE(TAG, "控制器指针为空!");
            vTaskDelete(NULL);
            return;
        }
        
        // 主控制循环
        while(1) {
            if(lv_scr_act()==objects.carcontrol)
            {
                // 1. 执行 Pure Pursuit 控制计算
                PurePursuit::ControlCommand cmd = controller->update();
                
                // 2. 发送控制指令给 STM32 (蓝牙)
                if (cmd.is_valid) {
                    car_comm_send_cmd(cmd.mode, cmd.v_cmd, cmd.w_cmd);
                }
            }

            
            // 3. 20ms 周期 (50Hz)
            vTaskDelay(pdMS_TO_TICKS(20));
        }
    }
    

    //============= Core 0 任务：UI界面渲染绘制
    static void UITask(void *pvParameters)
    {
        while(1)
        {
            lv_timer_handler();
            
            bsp_update_time();
            bsp_screen_dormancy();
            vTaskDelay(pdMS_TO_TICKS(10));//强行释放cpu 10ms，防止看门狗咬人
        }
    }
    
#endif



#if defined(USE_SDL)
int main(int argc, char **argv) {
    nvs_load_correct_pin();
    lv_init();
    sdl_init();//初始化SDL视频 + 窗口 + 渲染器 + 纹理 + 事件系统和LVGL
    
    //显示缓冲区设置
    static lv_disp_draw_buf_t draw_buf;//声明显示缓冲区结构体
    static lv_color_t buf1[240*10];//lv_color_t LVGL 的颜色类型
    static lv_color_t buf2[240*10];//两个缓冲区，一个给 GPU 渲染，一个给 CPU 绘制，防撕裂，240*10：缓冲 10 行像素的数据
    lv_disp_draw_buf_init(&draw_buf,buf1,buf2,240*10);
    
    //显示驱动注册
    static lv_disp_drv_t disp_drv;//显示驱动结构体
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res=240;//设置显示分辨率，水平分辨率
    disp_drv.ver_res=320;//垂直分辨率
    disp_drv.flush_cb=sdl_display_flush;//绑定显示刷新回调函数，sdl_display_flush是lv_drive库提供SDL实现，把buf1/buf2的像素复制到SDL窗口
    disp_drv.draw_buf=&draw_buf;//把缓冲区绑定到驱动，让lvgl知道在哪绘制像素
    lv_disp_drv_register(&disp_drv);//向LVGL注册这个驱动
    
    //输入设备注册
    static lv_indev_drv_t indev_dev;//输入设备驱动结构体
    lv_indev_drv_init(&indev_dev);
    indev_dev.type=LV_INDEV_TYPE_POINTER;//设置输入设备类型为指针鼠标，lvgl还支持LV_INDEV_TYPE_KEYPAD(键盘)，~ENCODER(旋转编码器)，~BUTTON（物理按键）
    indev_dev.read_cb = sdl_mouse_read;//绑定输入读取回调函数，SDL鼠标读取回调
    lv_indev_drv_register(&indev_dev);//注册输入设备

    
    screen_init();
    // ============ 模拟器模式：单线程运行所有逻辑 ============
    PurePursuit::Config config;
    config.lookahead_distance = 0.10f;
    config.base_linear_speed = 0.30f;
    config.target_tolerance = 0.02f;
    config.control_frequency = 50.0f;
    config.max_angular_speed = 3.0f;
    config.min_linear_speed = 0.10f;
    config.speed_factor = 0.5f;
    
    PurePursuit controller;
    controller.init(config);
    controller.reset(0.0f, 0.0f, 0.0f);
    
    uint32_t last_control = 0;
    uint32_t last_tick = SDL_GetTicks();
    while(1)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event)) //事件轮询处理
        {

            if(event.type == SDL_QUIT) 
            {
                return 0;  // 点击窗口关闭按钮时退出
            }
            mouse_handler(&event);//自定义鼠标事件处理函数
        }

        uint32_t now = SDL_GetTicks();
        lv_tick_inc(now - last_tick);//告诉 LVGL 过了多少毫秒，更新内部时间基准
        last_tick = now;

        // 执行 Pure Pursuit 控制 (50Hz)
        if (now - last_control >= 20) {
            last_control = now;
            if(lv_scr_act()==objects.carcontrol)
            {
                auto cmd = controller.update();
                if (cmd.is_valid) {
                    car_comm_send_cmd(cmd.mode, cmd.v_cmd, cmd.w_cmd);
                }
            }
        }
        bsp_update_time();
        lv_timer_handler();//lvgl心跳函数，处理lvgl任务(检查输入设备状态，处理所有挂起事件，重新计算受影响对象布局，重绘脏区域)，必须定期调用，否则界面卡死
        SDL_Delay(5);//降低cpu占用
    }

    SDL_Quit();
    return 0;

}



#elif defined(ESP_PLATFORM)
extern "C" void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    
    if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)        //参数1:频繁写入导致NVS页耗尽，垃圾回收失败，擦除整个分区       参数1：固件升级后，NVS格式版本不匹配，擦除旧数据
    {
        ESP_ERROR_CHECK(nvs_flash_erase()); 
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
     
    nvs_load_correct_pin();
    lv_init();
    bsp_lcd_ili9341_init();
    bsp_touch_ft6336_init();
    bsp_wifi_sntp_init();
    car_comm_init();

    //显示缓冲区设置
    static lv_disp_draw_buf_t disp_buf;

    // 缓冲区必须在内部 DRAM 中 (不能在 PSRAM)，且必须是 DMA 可用的
    // 大小: 240 * 20 lines * 2 bytes = 9600 bytes per buffer
    size_t buf_size = H_RES * 20 * sizeof(lv_color_t);

    // 申请第一块支持 DMA 且在高速内部内存中的 40 行缓冲区
    lv_color_t *buf1 = (lv_color_t *)heap_caps_aligned_alloc(4, buf_size, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
    // 申请第二块支持 DMA 且在高速内部内存中的 40 行缓冲区
    lv_color_t *buf2 = (lv_color_t *)heap_caps_aligned_alloc(4, buf_size, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
   
    if (!buf1 || !buf2) {
        ESP_LOGE(TAG, "[FATAL ERROR] 显存分配失败！buf1=%p, buf2=%p", buf1, buf2);
        ESP_LOGE(TAG, "内部 DMA 连续可用内存: %d bytes", heap_caps_get_largest_free_block(MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL));
        ESP_LOGE(TAG, "内部最大连续块: %d bytes", heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL));
        return; 
    }

   
    // 验证缓冲区在合法地址范围
    ESP_LOGI(TAG, "buf1=%p, buf2=%p, size=%d", buf1, buf2, buf_size);
    
    uintptr_t addr = (uintptr_t)buf1;
    if (addr >= 0x42000000 && addr < 0x42800000) {
        ESP_LOGI(TAG, "在 PSRAM 中");
    } else if (addr >= 0x3FCC0000 && addr < 0x3FD00000) {
        ESP_LOGI(TAG, "在内部 DRAM 中");
    }
    // 初始化双缓冲区（buf1, buf2 轮流接力，一个显示，另一个在后台偷偷画，绝无撕裂！）
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, H_RES * 20);
    
    static lv_disp_drv_t disp_drv;//显示驱动结构体
    lv_disp_drv_init(&disp_drv);
    g_disp_drv = &disp_drv; //赋值全局指针
    disp_drv.hor_res=H_RES;//设置显示分辨率，水平分辨率
    disp_drv.ver_res=V_RES;//垂直分辨率
    disp_drv.flush_cb=disp_flush;//绑定显示刷新回调函数，disp_flush是lv_drive库提供SDL实现，把buf1/buf2的像素复制到SDL窗口
    disp_drv.draw_buf=&disp_buf;//把缓冲区绑定到驱动，让lvgl知道在哪绘制像素
    lv_disp_drv_register(&disp_drv);//向LVGL注册这个驱动
    
    //输入设备注册
    static lv_indev_drv_t indev_dev;//输入设备驱动结构体
    lv_indev_drv_init(&indev_dev);
    indev_dev.type=LV_INDEV_TYPE_POINTER;//设置输入设备类型为指针鼠标，lvgl还支持LV_INDEV_TYPE_KEYPAD(键盘)，~ENCODER(旋转编码器)，~BUTTON（物理按键）
    indev_dev.read_cb = touchpad_read;//绑定输入读取回调函数，挂载手势读取查岗函数
    lv_indev_drv_register(&indev_dev);//注册输入设备

    esp_timer_create_args_t lvgl_tick_timer_args={};//为LVGL提供雷打不动的2ms心跳时钟
    lvgl_tick_timer_args.callback=&lvgl_tick_cb;//匿名λ表达式，每次中断自动让LVGL寿命加2ms
    lvgl_tick_timer_args.name="lvgl_tick";//定时器线程名称
    
    esp_timer_handle_t lvgl_tick_timer;
    esp_timer_create(&lvgl_tick_timer_args,&lvgl_tick_timer);
    esp_timer_start_periodic(lvgl_tick_timer,2000);//每2ms触发一次中断

    
    printf("[SYSTEM] physically swig up the backlight GPIO 8\n");
    gpio_set_direction((gpio_num_t)LCD_PIN_NUM_BLK, GPIO_MODE_OUTPUT);
    gpio_set_level((gpio_num_t)LCD_PIN_NUM_BLK, 1);

    printf("[SYSTEM] run ui node init\n");
    
    screen_init();
    printf("[SYSTEM] enter big lop daemon\n");
    
    // ============ 关键：在调度器启动后初始化控制器 (避免静态初始化顺序问题) ============
    static PurePursuit controller_instance;  // 静态局部变量，首次使用时初始化
    g_pure_pursuit_ptr = &controller_instance;  // 设置全局指针
    ESP_LOGI(TAG, "控制器地址: %p", g_pure_pursuit_ptr);
    
    // 初始化控制器配置
    PurePursuit::Config config;
    config.lookahead_distance = 0.10f;      // 预瞄距离 10cm
    config.base_linear_speed = 0.30f;        // 基准线速度 0.3 m/s
    config.target_tolerance = 0.02f;          // 到达容差 2cm
    config.control_frequency = 50.0f;        // 控制频率 50Hz
    config.max_angular_speed = 3.0f;         // 最大角速度 3 rad/s
    config.min_linear_speed = 0.10f;          // 最小速度 0.1 m/s
    config.speed_factor = 0.5f;              // 转弯减速系数
    
    g_pure_pursuit_ptr->init(config);
    g_pure_pursuit_ptr->reset(0.0f, 0.0f, 0.0f);

    // ============ 创建 Core 1 任务 (PRO_CPU) ============
    // 优先级 5 (高于 UI)，运行 Pure Pursuit 高频控制
    ESP_LOGI(TAG, "创建 Core 1 (PRO_CPU) 任务: Pure Pursuit");
    xTaskCreatePinnedToCore(
        PurePursuitTask ,       //任务函数
        "PurePursuit" ,         //任务名称
        8192,                   //栈大小（8kb）
        NULL,                   //参数：不需要，任务内部使用 g_pure_pursuit_ptr
        5,                      //优先级（5）
        NULL,                   //任务句柄
        1                       //核心：PRO_CPU(Core 1)
    );

    // ============ 主循环运行在 Core 0 (APP_CPU) ============
    ESP_LOGI(TAG, "主循环运行在 Core 0 (APP_CPU): UI 渲染 + 贝塞尔曲线");
    xTaskCreatePinnedToCore(
        UITask,
        "UITask",
        8192,
        NULL,
        4,
        NULL,
        0
    );
    
    //============蓝牙信息接收运行在 CORE0 =========================
    xTaskCreatePinnedToCore(
        bsp_bt_ble_receive,
        "bsp_bt_ble_receive",
        4096,
        NULL,
        5,
        NULL,
        0
    );

    vTaskDelete(NULL);

}

#endif