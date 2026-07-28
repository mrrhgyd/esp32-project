#ifndef DINOSAUR_RUN_H
#define DINOSAUR_RUN_H

#include "lvgl.h"


// 仙人掌对象结构体
struct Cactus {
    int x;
    bool active;
    lv_obj_t *obj;
};

class dinosaurrun
{
private:
    static constexpr int GRAVITY = 2;             // 重力加速度
    static constexpr int JUMP_VELOCITY = -20;     // 起跳初速度（向上为负）
    static constexpr int CACTUS_SPEED = 8;        // 仙人掌移动速度
    static constexpr int GROUND_Y = 250;          // 地面 Y 坐标
    static constexpr int DINO_SIZE = 30;          // 恐龙大小
    static constexpr int CACTUS_W = 20;           // 仙人掌宽度
    static constexpr int CACTUS_H = 40;           // 仙人掌高度
    static constexpr int MAX_CACTI = 3;           // 屏幕上最多同时存在的仙人掌数量

    int dino_y;                                   //实际坐标
    int dino_vy;                                  //应下落多少
    int score;
    bool is_jump;
    bool is_game_over;

    //随机生成机制的变量
    int spawn_timer;
    int next_spawn_timer;

    Cactus cacti[MAX_CACTI];                        //仙人掌对象池子

    lv_timer_t *game_timer=nullptr;


    dinosaurrun();
    ~dinosaurrun()=default;

    //禁止拷贝构造和赋值操作符
    dinosaurrun(const dinosaurrun&)=delete;
    dinosaurrun& operator=(const dinosaurrun&)=delete;

    void reset_game();                                  //重置
    void game_loop();                                   //游戏内部数据更新
    void input_event();                                 //内部输入时间处理
    void spawn_cactus();                                //激活一个仙人掌
public:
    static dinosaurrun& getinstance();
    static void game_input_event_cb(dinosaurrun *e);     //屏幕触摸回调事件
    void start_game();                                  //开始游戏
    static void game_loop_cb(lv_timer_t *game_time);    //游戏心跳，每30ms执行一次帧循环
    void stop_game();
};

















#endif