#include "dinosaur_run.hpp"
#include <stdlib.h>                     //使用rand()和srand()：伪随机数函数
#include "ui_action.h"
#include "ui.h"
dinosaurrun &dinosaurrun::getinstance()
{
    static dinosaurrun instance;
    return instance;
}
dinosaurrun::dinosaurrun()
{
    dino_y=GROUND_Y-DINO_SIZE;
    dino_vy=0;
    score=0;
    is_jump = false;
    is_game_over=false;
    spawn_timer=0;
    next_spawn_timer =30;           //第一个仙人掌在30帧后出现
    game_timer = nullptr; 

    for(int i=0;i<MAX_CACTI;i++)
    {
        cacti[i].x=240;
        cacti[i].active =false;
        cacti[i].obj=nullptr;
    }
}

void dinosaurrun::reset_game()
{
    dino_y=GROUND_Y-DINO_SIZE;
    dino_vy=0;
    score=0;
    is_jump = false;
    is_game_over=false;
    spawn_timer=0;
    next_spawn_timer =30; 
    

    lv_obj_add_flag(objects.game_over_label,LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text_fmt(objects.score_label,"score:%d",score);
    for(int i=0;i<MAX_CACTI;i++)
    {
        cacti[i].active=false;
        lv_obj_add_flag(cacti[i].obj,LV_OBJ_FLAG_HIDDEN);
    }
    if(game_timer!=nullptr)lv_timer_resume(game_timer);
}

void dinosaurrun::game_loop()
{
    if(is_game_over)return;

    //恐龙
    dino_vy+=GRAVITY;
    dino_y+=dino_vy;
    if(dino_y >=GROUND_Y-DINO_SIZE)
    {
        dino_y=GROUND_Y-DINO_SIZE;
        dino_vy=0;
        is_jump=false;
    }

    lv_obj_set_y(objects.dino_obj,dino_y);


    //随机刷新仙人掌
    if(++spawn_timer>=next_spawn_timer)
    {
        spawn_cactus();
        spawn_timer=0;
        next_spawn_timer=(rand()%40)+25;        //rand（）生成随机数（根据一个"种子"（seed）通过固定算法生成一串数字），和srand（）配合使用，下一个仙人掌在25-65帧间随机出现
    }

    //碰撞检测和仙人掌移动
    int dino_x =30;                             //固定恐龙横坐标
    for (int i = 0; i < MAX_CACTI; i++)
    {
        if(cacti[i].active)
        {
            cacti[i].x-=CACTUS_SPEED;
            lv_obj_set_x(cacti[i].obj,cacti[i].x);

            if(cacti[i].x<=-CACTUS_W)
            {
                lv_obj_add_flag(cacti[i].obj,LV_OBJ_FLAG_HIDDEN);
                cacti[i].active=false;
                score+=10;
                lv_label_set_text_fmt(objects.score_label,"score:%d",score);
            }

            //碰撞检测
            int hit_margin=0;
            int dino_left=dino_x+hit_margin;
            int dino_right=dino_x+DINO_SIZE-hit_margin;
            int dino_top =dino_y + hit_margin;
            int dino_bottom = dino_y+DINO_SIZE-hit_margin;

            int cactus_left=cacti[i].x;
            int cactus_right=cacti[i].x+CACTUS_W;
            int cactus_top = GROUND_Y-CACTUS_H;
            int cactus_bottom=GROUND_Y;

            if(dino_left<cactus_right && dino_right >cactus_left && dino_top < cactus_bottom && dino_bottom >cactus_top)
            {
                is_game_over =true;
                lv_obj_clear_flag(objects.game_over_label,LV_OBJ_FLAG_HIDDEN);
                lv_timer_pause(game_timer);
            }
        }
    }
    
}

void dinosaurrun::input_event()
{
    if(is_game_over)
    {
        reset_game();
        
        return;
    }
    if(!is_jump)
    {
        dino_vy+=JUMP_VELOCITY;
        is_jump=true;
    }
}

void dinosaurrun::spawn_cactus()
{
    for(int i=0;i<MAX_CACTI;i++)
    {
        if(!cacti[i].active)
        {
            cacti[i].active=true;
            cacti[i].x=240;
            lv_obj_clear_flag(cacti[i].obj,LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_pos(cacti[i].obj,cacti[i].x,GROUND_Y-CACTUS_H);
            break;
        }
    }
}

void dinosaurrun::game_input_event_cb(dinosaurrun*e)
{
    if(e)e->input_event();
}


void dinosaurrun::start_game()
{
    srand(lv_tick_get());       //随机数初始化种子，和rand（）配合使用

    cacti[0].obj=objects.cacti1;
    cacti[1].obj=objects.cacti2;
    cacti[2].obj=objects.cacti3;
    
    if (game_timer == nullptr) {
        game_timer = lv_timer_create(game_loop_cb, 30, this);
    }
    reset_game();
}


void dinosaurrun::game_loop_cb(lv_timer_t *game_time)
{
    dinosaurrun *game=static_cast<dinosaurrun*>(game_time->user_data);
    if(game)game->game_loop();
}


void dinosaurrun::stop_game()
{
    if(game_timer!=nullptr)lv_timer_pause(game_timer);
}
