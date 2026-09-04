
#include "bsp_board.h"
#include "screens.h"
#include "ui.h"
#include "ui_action.h"
#include "pure_pursuit.hpp"

#if defined(ESP_PLATFORM)
#include "esp_log.h"

static const char * TAG = "BSP";
#define REMOTE_SERVICE_UUID         0XFFE0                          //目标蓝牙的service UUID
#define REMOTE_NOTIFY_CHAR_UUID     0XFFE1                          //目标蓝牙的characteristic UUID
static esp_bd_addr_t target_mac={0x3c,0xa5,0x51,0x96,0xfe,0x76};    //目标蓝牙的mac地址
static bool is_ble_connect = false;                                 
static esp_gatt_if_t client_if=ESP_GATT_IF_NONE;                    //GATT Client接口标识符，ESP_GATT_IF_NONE表示未注册
static uint16_t client_conn_id=0;                                   //连接id，每次连接分配的唯一标识
static uint16_t target_char_handle =0;                              //用于记录发送数据的特征值句柄

extern lv_disp_drv_t *g_disp_drv;
esp_lcd_panel_handle_t panel_handle = NULL;                         //屏幕物理管理句柄
esp_lcd_touch_handle_t tp = NULL;                                   //触摸芯片物理管理句柄
static bool screen_is_blk = false;                                  // 静态全局变量，用于记录屏幕当前的背光开关状态

static i2c_master_bus_handle_t i2c_bus_handle = NULL;               // 声明全局的 I2C 主机总线句柄（静态，仅当前文件可见）
static StreamBufferHandle_t xStreamBuffer=xStreamBufferCreate(128,1);
enum class Rxstate{WAIT_HEAD,WAIT_V,WAIT_W,WAIT_YAW,WAIT_BCC,WAIT_TATL};

extern "C" void nvs_load_correct_pin(void);

// 硬件传输完毕回调
static bool on_color_trans_done(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx) {
    if (g_disp_drv) {
        lv_disp_flush_ready(g_disp_drv);
    }
    return false;
}

void disp_flush(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map) 
{
    esp_lcd_panel_draw_bitmap(panel_handle, area->x1, area->y1, area->x2 + 1, area->y2 + 1, color_map);// 调用乐鑫高速 API，通过 DMA 将 color_map（像素数组）直接砸进屏幕对应的矩形区域中
    //lv_disp_flush_ready(drv);// 通知 LVGL 引擎：当前区域已经刷新完毕，你可以准备刷新下一帧了
}

// LVGL 触摸回调 (ESP32)，每隔大约 30ms，LVGL 会来这里“查岗”看有没有被戳
void touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data) 
{
    if (tp == NULL) {
        data->state = LV_INDEV_STATE_REL;
        return;
    }
    esp_lcd_touch_point_data_t point_data[1]; // 用于存储一个点的触摸数据（支持多点，此处我们只需要单点）
    uint8_t touchpad_cnt = 0;// 存放当前触摸点的个数
    esp_lcd_touch_read_data(tp);//命令硬件触摸控制器去读取 FT6336 内部最新的寄存器数据
    esp_err_t err = esp_lcd_touch_get_data(tp, point_data, &touchpad_cnt, 1);
    if (err==ESP_OK && touchpad_cnt > 0) //判断是否真的被按下，且至少有一个点被捕捉到
    {
        data->point.x = point_data[0].x;
        data->point.y = point_data[0].y;// 赋值给 LVGL 的 X,Y 点
        data->state = LV_INDEV_STATE_PR;// 报告状态：PRESSED (处于被按下状态)
    } 
    else data->state = LV_INDEV_STATE_REL;// 报告状态：PRESSED (处于释放状态)

}


//初始化 ili9341 屏幕硬件并注册到 LVGL
void bsp_lcd_ili9341_init(void)
{
    //配置物理SPI总线参数
    spi_bus_config_t buscfg = {};
    buscfg.mosi_io_num = LCD_PIN_NUM_MOSI;
    buscfg.miso_io_num = LCD_PIN_NUM_MISO;                      //单项刷屏，不需要读取，-1禁用
    buscfg.sclk_io_num = LCD_PIN_NUM_CLK;                       
    buscfg.quadwp_io_num = -1;                                  //禁用高级QSPI模式
    buscfg.quadhd_io_num = -1;                                  //禁用高级QSPI模式
    buscfg.max_transfer_sz = H_RES * 80 * sizeof(uint16_t);     //设定单次DMA传输最大允许字节数量

    //启动硬件并开启DMA通道自动关联分配
    spi_bus_initialize(LCD_HOST,&buscfg,SPI_DMA_CH_AUTO);

    //创建专用于屏幕指令控制的物理IO接口
    esp_lcd_panel_io_handle_t io_handle =NULL;
    esp_lcd_panel_io_spi_config_t io_config={};
    io_config.cs_gpio_num = (gpio_num_t)LCD_PIN_NUM_CS;
    io_config.dc_gpio_num = (gpio_num_t)LCD_PIN_NUM_DC;
    io_config.spi_mode=0;
    io_config.pclk_hz = LCD_PIXEL_CLK;
    io_config.trans_queue_depth=10;                                                         //队列深度为10层
    io_config.lcd_cmd_bits=8;                                                               //指令长度为8个位
    io_config.lcd_param_bits=8;                                                             //参数长度为8个位
    io_config.on_color_trans_done = on_color_trans_done;                                    // 挂载回调
    esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST,&io_config,&io_handle);     //初始化 SPI 接口 LCD 面板传输句柄

    //实例化ST7789厂商内置寄存器代码控制器
    esp_lcd_panel_dev_config_t panel_config={};
    panel_config.reset_gpio_num=(gpio_num_t)LCD_PIN_NUM_RST;
    panel_config.rgb_ele_order=LCD_RGB_ELEMENT_ORDER_BGR;               //定义基本颜色空间为RGB
    panel_config.bits_per_pixel =16;                                    //设定每个像素占用16位颜色深度
    esp_lcd_new_panel_ili9341(io_handle,&panel_config,&panel_handle);   //初始化 ST7789 LCD 驱动芯片。该函数负责创建 LCD 面板句柄，配置色彩格式、复位引脚及初始化序列，是驱动屏幕显示的核心步骤。

    //驱动开机四部曲
    esp_lcd_panel_reset(panel_handle);                  //物理拉低复位引脚
    esp_lcd_panel_init(panel_handle);                   //灌入开机底层出厂寄存器指令
    esp_lcd_panel_invert_color(panel_handle,true);      //必须ST7789属于常黑屏幕，必须反色才能正常看
    esp_lcd_panel_disp_on_off(panel_handle,true);       //强行解除休眠，唤醒显示

    esp_lcd_panel_mirror(panel_handle,true,false);      //镜像翻转
    //接管背光引脚
    gpio_set_direction((gpio_num_t)LCD_PIN_NUM_BLK,GPIO_MODE_OUTPUT);   //配置GPIO8为输出模式
    gpio_set_level((gpio_num_t)LCD_PIN_NUM_BLK,1);                       //输出高电平，点亮背光

}

void bsp_touch_ft6336_init(void)
{
    printf("[I2C] start I2C hardware config\n");
    // 2.1 配置并初始化硬件 I2C 通信主机总线
    i2c_master_bus_config_t i2c_conf={};                       
    i2c_conf.i2c_port =I2C_NUM_0;                     //配置ESP32-S3为I2C主机模式
    i2c_conf.sda_io_num = (gpio_num_t)TOUCH_PIN_SDA;    
    i2c_conf.scl_io_num = (gpio_num_t)TOUCH_PIN_SCL;
    i2c_conf.clk_source = I2C_CLK_SRC_DEFAULT;          //使用默认时钟源
    i2c_conf.glitch_ignore_cnt =7;                      //开启滤波，忽略总线毛刺
    i2c_conf.intr_priority=0;                           //默认中断优先级
    i2c_conf.trans_queue_depth = 0;                     //仅作主读
    i2c_conf.flags={.enable_internal_pullup=true};      //开启内部上拉电阻

   // 创建新版 I2C 主机总线
    esp_err_t ret = i2c_new_master_bus(&i2c_conf, &i2c_bus_handle);
    if (ret != ESP_OK) {
        printf("[I2C ERROR] I2C hardware bus create failed! failure: %s\n", esp_err_to_name(ret));
        return;
    }

    printf("[I2C] hardware bus create success\n");
    
    //创建专门针对触摸接口的物理IO映射实例
    esp_lcd_panel_io_handle_t tp_io_handle=NULL;
    esp_lcd_panel_io_i2c_config_t tp_io_config={};
    tp_io_config.dev_addr =ESP_LCD_TOUCH_IO_I2C_FT5x06_ADDRESS;     //挂载通用的FT系列芯片I2C设备地址（0x38）
    tp_io_config.control_phase_bytes =1;                            //控制相位字段占用1个字节
    tp_io_config.lcd_cmd_bits=8;                                    //寄存器地址长度为8位
    tp_io_config.lcd_param_bits=0;                                  //读写数据参数长度为0位
    tp_io_config.flags.disable_control_phase =1;
    tp_io_config.scl_speed_hz =400000;                              //如果总线速度为100000kHz，这里配置为100000


    /*初始化 LCD 面板 I2C 通信接口。该函数创建一个面板 I/O 对象，专门用于通过 I2C 总线向 LCD 驱动芯片发送命令和数据，用于驱动如 GC9A01、ST7789 等支持 I2C 协议的显示屏。*/
    ret = esp_lcd_new_panel_io_i2c((i2c_master_bus_handle_t)i2c_bus_handle, &tp_io_config, &tp_io_handle);
    if (ret != ESP_OK || tp_io_handle == NULL) {
        printf("[I2C ERROR] I2C IO abstract layer bridging failed! failure: %s\n", esp_err_to_name(ret));
        return;
    }
    
    printf("[I2C] IO abstract layer bridging success\n");


    //实例化并正式启动触摸硬件
    esp_lcd_touch_config_t tp_cfg={};
    tp_cfg.y_max=V_RES;                                                 //告诉触摸芯片Y坐标最大限度
    tp_cfg.x_max=H_RES;
    tp_cfg.rst_gpio_num=(gpio_num_t)TOUCH_PIN_RST;                      //激活硬件复位引脚，更稳定
    tp_cfg.int_gpio_num=(gpio_num_t)TOUCH_PIN_INT;                      //激活手势捕捉的外部中断引脚

    tp_cfg.flags.mirror_x=0;                                            //如果屏幕开了左右镜像
    tp_cfg.flags.mirror_y=0;
    ret = esp_lcd_touch_new_i2c_ft5x06(tp_io_handle,&tp_cfg,&tp);       //将它当成通用的FT驱动架构拉起来
    if (ret != ESP_OK) 
    {
        printf("[I2C ERROR] falied to instantiate ft6336 driver! failure: %s\n", esp_err_to_name(ret));
        return;
    }
    
    printf("successed to instantiate ft6336 driver\n");

}

//wifi底层事件状态机监视器
static void wifi_event_handler(void*arg,esp_event_base_t event_base,int32_t event_id,void*event_data)
{
    if(event_base ==WIFI_EVENT&&event_id ==WIFI_EVENT_STA_START)esp_wifi_connect();                     //状态就绪，可以开始连接指定路由
    else if(event_base == WIFI_EVENT&&event_id ==WIFI_EVENT_STA_DISCONNECTED)esp_wifi_connect();        //意外断开，自动发起重连
    else if(event_base == IP_EVENT&&event_id==IP_EVENT_STA_GOT_IP)                                      //成功获取内网ip，拉起网络SNTP时间对时服务
    {
        esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);                                                    //设置SNTP工作模式为轮询模式（客户端定时向服务器请求时间）
        esp_sntp_setservername(0,"ntp.aliyun.com");                                                     //0号服务器挂载阿里云高度时间基准服务器
        esp_sntp_setservername(1,"time.windows.com");                                                   //1号服务器为微软windows NTP服务器
        esp_sntp_init();
    }   
}


//联网组件初始化主配置
void bsp_wifi_sntp_init(void)
{
    ESP_ERROR_CHECK(esp_netif_init());                          //初始化网络栈，ESP_ERROR_CHECK是一个宏，函数返回错误会打印错误信息并触发abort（）重启
    ESP_ERROR_CHECK(esp_event_loop_create_default());           //创建默认事件循环
    esp_netif_create_default_wifi_sta();                        //创建默认的wifi station网络接口（STA模式=作为客户端连接路由）

    wifi_init_config_t cfg =WIFI_INIT_CONFIG_DEFAULT();         //使用默认wifi初始化配置
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));                       //初始化wifi驱动

    esp_event_handler_instance_t instance_any_id;               //事件处理句柄，用于wifi事件
    esp_event_handler_instance_t instance_gop_id;               //用于ip事件
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,ESP_EVENT_ANY_ID,&wifi_event_handler,NULL,&instance_any_id));        //注册wifi事件处理器
    /*参数1：事件基类：wifi相关事件     参数2：事件ID，监听所有wifi事件     参数3：回调函数指针     参数4：用户数据（传递给回调函数的参数）     参数5：输出，事件处理句柄*/
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,IP_EVENT_STA_GOT_IP,&wifi_event_handler,NULL,&instance_gop_id));       //注册ip事件处理器
    /*参数1：事件基类：ip相关事件       参数2：事件id，只监听“获取到ip地址”事件     参数3：回调函数指针     参数4,5：同上*/
    
    wifi_config_t wifi_config={};                                       //定义wif配置结构体
    strcpy((char*)wifi_config.sta.ssid,WIFI_SSID);                      //将宏定义中的wifi的ssid和密码拷贝到结构体中
    strcpy((char*)wifi_config.sta.password,WIFI_PASS);

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));                  //设置wifi工作模式为station模式
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA,&wifi_config));     //将ssid和密码配置应用到wifi驱动
    ESP_ERROR_CHECK(esp_wifi_start());                                  //启动wifi驱动

    //设置中国标准东八区时区偏移（北京时间）
    setenv("TZ","CST-8",1);                                             //CST-8:CST为时区名称，-8表示比UTC快8小时（东八区用负数）
    tzset();                                                            //应用时区设置

}

void bsp_update_time(void)
{
    char timer_str[32]={0};
    char date_str[32]={0};
    char day_str[32]={0};

    static const char* weekday_chinese[]={"星期日","星期一","星期二","星期三","星期四","星期五","星期六"};

    //获取系统实时网络时间并更新到标签上
    time_t now;
    struct tm timeinfo;                 //mday:日  wday:星期（0为周日）    yday:年内第几天     isdst:夏令时标志     year：从1900年开始的年数偏移量
    time(&now);                         //获取当前系统事件，以秒为单位的unix时间戳
    localtime_r(&now,&timeinfo);        //将unix时间戳转换为本地时间结构体
    
    if(timeinfo.tm_year >(1970-1900))
    {
        snprintf(timer_str,sizeof(timer_str),"%02d:%02d:%02d",timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);
        int wday_idx =timeinfo.tm_wday;
        snprintf(date_str,sizeof(date_str),"%d-%02d-%02d",timeinfo.tm_year+1900,timeinfo.tm_mon+1,timeinfo.tm_mday);
        snprintf(day_str,sizeof(day_str),"%s",weekday_chinese[wday_idx]);
        if(objects.time)lv_label_set_text(objects.time,timer_str);
        if(objects.date)lv_label_set_text(objects.date,date_str);
        if(objects.day)lv_label_set_text(objects.day,day_str);
        if(objects.pin_time)lv_label_set_text(objects.pin_time,timer_str);
        if(objects.pin_date)lv_label_set_text(objects.pin_date,date_str);
        if(objects.pin_day)lv_label_set_text(objects.pin_day,day_str);
    }
}

void bsp_screen_dormancy(void)
{
    //无操作1分钟自动息屏唤醒
    uint32_t inactive_ms=lv_disp_get_inactive_time(NULL);//获取默认屏幕的无触摸静止时间（毫秒）
    if(inactive_ms>=60000)
    {
        if(!screen_is_blk)
        {
            gpio_set_level((gpio_num_t)LCD_PIN_NUM_BLK,0);//休眠
            if(correct_pin[0]=='\0')ui_navigate_to(objects.lockscreen);
            else ui_navigate_to(objects.pinscreen);
            screen_is_blk=true;
        }
       
    } 
    else
    {
        if(screen_is_blk)
        {
            gpio_set_level((gpio_num_t)LCD_PIN_NUM_BLK,1);//唤醒
            screen_is_blk=false;
        }
    }
}

void bsp_bt_ble_send(uint8_t *data,size_t len)
{
    if(bsp_bt_is_connect())
    esp_ble_gattc_write_char(client_if,client_conn_id,target_char_handle,len,data,ESP_GATT_WRITE_TYPE_NO_RSP,ESP_GATT_AUTH_REQ_NONE);
    /*参数1：GATT客户端接口标识     参数2：连接id标识，当前链路     参数3：目标特征值的句柄     参数4、5：待写入数据长度和内容*/
    /*参数6：写入类型（是否需要响应）       参数7：请求认证类型：NONE：无需认证  MITM：中间人保护     NO_MITM：需要加密但无需MITM     SIGNED：需要签名写入*/
}

void bsp_bt_ble_receive(void *pvParmeters)
{
    uint8_t rx_byte;
    while(1)
    {
        if(xStreamBufferReceive(xStreamBuffer,&rx_byte,1,portMAX_DELAY))statemachine_parse(rx_byte);
    }
}

void statemachine_parse(uint8_t &data)
{
    static uint32_t last_rx_time=0;
    static Rxstate rx_state=Rxstate::WAIT_HEAD;
    uint32_t now=xTaskGetTickCount();
    if(rx_state!=Rxstate::WAIT_HEAD&&(now-last_rx_time>pdMS_TO_TICKS(50)))rx_state=Rxstate::WAIT_HEAD;//防止丢包卡死
    last_rx_time=now;
    switch(rx_state)
    {
        case Rxstate::WAIT_HEAD:
            if(data==0xaa){rx_state=Rxstate::WAIT_V;g_pure_pursuit_ptr->read_cmd.bcc=0;}
            break;
        case Rxstate::WAIT_V:
            g_pure_pursuit_ptr->read_cmd.v=data;
            g_pure_pursuit_ptr->read_cmd.bcc^=data;
            rx_state=Rxstate::WAIT_W;
            break;
        case Rxstate::WAIT_W:
            g_pure_pursuit_ptr->read_cmd.w=data;
            g_pure_pursuit_ptr->read_cmd.bcc^=data;
            rx_state=Rxstate::WAIT_YAW;
            break;
        case Rxstate::WAIT_YAW:
            g_pure_pursuit_ptr->read_cmd.yaw=data;
            g_pure_pursuit_ptr->read_cmd.bcc^=data;
            rx_state=Rxstate::WAIT_BCC;
            break;
        case Rxstate::WAIT_BCC:
            if(data==g_pure_pursuit_ptr->read_cmd.bcc){
                rx_state=Rxstate::WAIT_TATL;
                last_rx_time=xTaskGetTickCount();
                g_pure_pursuit_ptr->read_cmd.is_read=false;
            }
            else rx_state=Rxstate::WAIT_HEAD;
            break;
        case Rxstate::WAIT_TATL:
            rx_state=Rxstate::WAIT_HEAD;
            break;            
        default:
            rx_state=Rxstate::WAIT_HEAD;
            break;
    }
}

bool bsp_bt_is_connect(void)
{
    return is_ble_connect &&(target_char_handle !=0);
}

// GATTC 回调核心状态机
static void esp_gattc_cb(esp_gattc_cb_event_t event,esp_gatt_if_t gattc_if,esp_ble_gattc_cb_param_t *param)
{
    switch (event)
    {
    case ESP_GATTC_REG_EVT:                                                         //ESP_GATTC_REG_EVT：GATT Client注册成功事件
        client_if =gattc_if;                                                        //保存接口标识符
        esp_ble_gattc_open(client_if,target_mac,BLE_ADDR_TYPE_PUBLIC,true);         //直接连接指定MAC地址，true：不扫描，BLE_ADDR_TYPE_PUBLIC：使用公共地址类型
        break;
    
    case ESP_GATTC_CONNECT_EVT:                                                     //底层物理蓝牙连接已建立
        client_conn_id = param->connect.conn_id;
        is_ble_connect =true;
        break;
    
    case ESP_GATTC_OPEN_EVT:                                                        //GATT层连接成功
        if(param->open.status == ESP_GATT_OK)esp_ble_gattc_search_service(gattc_if,param->open.conn_id,NULL);   
        /*open.status检查状态是否成功，esp_ble_gattc_search_service搜索设备上所有GATT服务*/     
        break;

    case ESP_GATTC_SEARCH_RES_EVT:{
        if (param->search_res.srvc_id.uuid.uuid.uuid16 == REMOTE_SERVICE_UUID)
        {
            printf("find target service\r\n");
        }
        break;
    }//每搜索到一个服务就触发一次

    //所有服务搜索完毕，开始查找具体characteristic
    case ESP_GATTC_SEARCH_CMPL_EVT:{
        esp_bt_uuid_t char_uuid={.len =ESP_UUID_LEN_16,.uuid={.uuid16=REMOTE_NOTIFY_CHAR_UUID}};//定义要查找的characteristic、service的UUID
        esp_bt_uuid_t srvc_uuid={.len=ESP_UUID_LEN_16,.uuid={.uuid16=REMOTE_SERVICE_UUID}};
        esp_gattc_char_elem_t result;           //存储查到的特征值信息
        uint16_t count =1;                      //期望返回的数量

        esp_gatt_status_t status = esp_ble_gattc_get_char_by_uuid(gattc_if,param->search_cmpl.conn_id,0x0001,0xFFFF,char_uuid,&result,&count);
        /*1.GATT客户端访问接口  2.连接id    3.属性起始句柄  4.结束句柄  5.要查找的uuid  6.输出结果  7.检索数量*/
        if (status ==ESP_GATT_OK &&count>0)
        {
            target_char_handle =result.char_handle;
            // ====== 注册接收 Notify 回调 ======
            esp_err_t reg_err=esp_ble_gattc_register_for_notify(gattc_if,target_mac,target_char_handle);
            if (reg_err == ESP_OK) printf("register_for_notify API OK, async in progress\r\n");
            else printf("register_for_notify API FAILED, err=0x%x (%s)\r\n",reg_err, esp_err_to_name(reg_err));
           
        }
        else printf("control characteristic value not found\n");
        break;
    }

    case ESP_GATTC_REG_FOR_NOTIFY_EVT:{
        if(param->reg_for_notify.status==ESP_GATT_OK)
        {
            //CCCD描述符的UUID在蓝牙规范中笃定为0x2902
            uint16_t notify_en=0x0001;
            
            //查找这个特征值下的0x2902描述符句柄
            esp_bt_uuid_t descr_uuid={.len=ESP_UUID_LEN_16,.uuid={.uuid16=ESP_GATT_UUID_CHAR_CLIENT_CONFIG}};
            esp_gattc_descr_elem_t descr_result;
            uint16_t descr_count=1;
            esp_gatt_status_t descr_status=esp_ble_gattc_get_descr_by_char_handle(
                gattc_if,client_conn_id,target_char_handle,descr_uuid,&descr_result,&descr_count
            );
            //如果找到了CCCD描述符，把0x0001写入
            if(descr_status==ESP_GATT_OK&&descr_count>0){
                esp_ble_gattc_write_char_descr(
                    gattc_if,
                    client_conn_id,
                    descr_result.handle,
                    sizeof(notify_en),
                    (uint8_t*)&notify_en,
                    ESP_GATT_WRITE_TYPE_RSP,
                    ESP_GATT_AUTH_REQ_NONE);
                printf("success send cmd to stm32");
            } 
            else printf("fail to find cccd");

        }
        break;
    }

    case ESP_GATTC_DISCONNECT_EVT:
        is_ble_connect=false;
        target_char_handle=0;
        break;
    
    case ESP_GATTC_NOTIFY_EVT:
        if(param->notify.is_notify){
            printf("receive notify,length:%d byte",param->notify.value_len);
            xStreamBufferSend(xStreamBuffer,param->notify.value,param->notify.value_len,portMAX_DELAY);
        }
        break;

    default:
        break;
    }
}


static void ble_start_task(void *pvParameters)
{
    ESP_LOGI(TAG," DMA continuous available memory: %d byte\n", heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA));
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();        //初始化蓝牙控制器结构体配置
    if(esp_bt_controller_init(&bt_cfg)!=ESP_OK)goto end;                              //初始化蓝牙控制器和使能，启用BLE模式
    if(esp_bt_controller_enable(ESP_BT_MODE_BLE)!=ESP_OK)goto end;

    if(esp_bluedroid_init()!=ESP_OK || esp_bluedroid_enable()!=ESP_OK)goto end;       //初始化蓝牙协议栈和使能

    esp_ble_gattc_register_callback(esp_gattc_cb);                                  //注册gattc事件回调函数
    esp_ble_gattc_app_register(0);                                                  //注册 GATT Client 应用（profile ID = 0），触发 ESP_GATTC_REG_EVT 事件，开始连接流程。

end:
    vTaskDelete(NULL);
}

void bsp_bt_ble_start(void)
{
    if(is_ble_connect)return;
    xTaskCreate(ble_start_task,"ble_start_task",4096,NULL,5,NULL);

}


static void ble_stop_task(void *pvParameters)
{
    if(is_ble_connect)esp_ble_gattc_close(client_if,client_conn_id);
    esp_ble_gattc_app_unregister(client_if);
    esp_bluedroid_disable();
    esp_bluedroid_deinit();
    esp_bt_controller_disable();
    esp_bt_controller_deinit();

    is_ble_connect= false;
    target_char_handle =0;
    client_if = ESP_GATT_IF_NONE;

    vTaskDelete(NULL);
}

void bsp_bt_ble_stop(void)
{
    xTaskCreate(ble_stop_task,"ble_stop_task",4096,NULL,5,NULL);
}

#endif

void screen_init()
{
    create_screens();
    if(correct_pin[0]=='\0')loadScreen(SCREEN_ID_LOCKSCREEN);
    else loadScreen(SCREEN_ID_PINSCREEN);
}

