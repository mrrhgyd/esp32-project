#include "ui_action.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include "dinosaur_run.hpp"
#include "car_trajectory.hpp"
#include "lockfree_queue.hpp"
#include "bezier_curve.hpp"
#include "car_comm.hpp"
#include "pure_pursuit.hpp"
#if defined(ESP_PLATFORM)
    #include "nvs_flash.h"
    #include "nvs.h"
    #include "driver/uart.h"
    #include "bsp_board.h"
#endif


extern "C"  {
    // TODO: Implement action change_screen here
    extern objects_t objects;
    lv_obj_t *screenstack[50];              //栈，存放上一级页面
    uint8_t stack_len=0;
    
    char pin_buffer[7]={0};                 //锁屏界面输入的密码
    uint8_t pin_len=0;
    char correct_pin[7]={'\0'};             //自己设置的密码
    char cur_pin[7]={0};
    
    char cur_speed_str[4]={0};              //输入的速度值
    uint8_t speed_len=0;
    int global_car_speed=0;                 //确定后的速度值


    void nvs_save_correct_pin(const char *pin)//保存密码到nvs
    {   
        #if defined(ESP_PLATFORM) 
        nvs_handle_t my_handle;
        //打开名为sys_pin的空间
        if(nvs_open("sys_pin",NVS_READWRITE,&my_handle)==ESP_OK)
        {
            nvs_set_str(my_handle,"user_pin",pin);//写入键值对
            nvs_commit(my_handle);//提交保存的闪存
            nvs_close(my_handle);//关闭句柄   
        }
        #endif

        #if defined(USE_SDL)
            FILE *file=fopen("D:\\ESP32\\ESP_BASIC\\nvs_sim.txt","w");
            fprintf(file,"%s",pin);
            fclose(file);


        #endif
    }

    void nvs_load_correct_pin(void)
    {
        #if defined(ESP_PLATFORM) 
            nvs_handle_t my_handle;
            if(nvs_open("sys_pin",NVS_READONLY,&my_handle)==ESP_OK)
            {
                size_t required_size=sizeof(correct_pin);
                //尝试读取user_pin键的值
                nvs_get_str(my_handle,"user_pin",correct_pin,&required_size);
                nvs_close(my_handle);
            }
        #endif

        #if defined(USE_SDL)
            FILE *file =fopen("D:\\ESP32\\ESP_BASIC\\nvs_sim.txt","r");
            if(fscanf(file,"%7s",correct_pin)!=1)correct_pin[0]='\0';//文件为空
            fclose(file);

        #endif
    }

    void send_car_cmd(uint8_t type, uint8_t speed)
    {
        uint8_t tx_buf[4]={0XAA,type,speed,0X55};
        #if defined(ESP_PLATFORM)
            if (bsp_bt_is_connect())
            {
                bsp_bt_ble_send(tx_buf,4);
            }
            
        #endif
        #if defined(USE_SDL)
            printf("type=0x%02X vlaue = %d",type,global_car_speed);

        #endif
    }


    void display_update(lv_obj_t *e)
    {
        char display[7]={0};
        if(e==objects.input1_keyboard)
        {
            for(int i=0;i<pin_len;i++)
            {
                display[i]='*';
            }
            lv_label_set_text(objects.pin_input,display);
        }

        else if(e==objects.input2_keyboard)
        {
            for(int i=0;i<pin_len;i++)
            {
                display[i]=cur_pin[i];
            }
            lv_label_set_text(objects.pin_input_2,display);
        }
        else if(e==objects.input3_keyboard)
        {
            char buffer[32];
            for(int i=0;i<speed_len;i++)
            {
                display[i]=cur_speed_str[i];
            }
            snprintf(buffer,sizeof(buffer),"set_speed:%s",cur_speed_str);
            lv_label_set_text(objects.speed_input,buffer);
        }
    }

    void ui_toggle(lv_obj_t *parent)
    {
        if (parent==NULL)return;

        uint32_t child_cnt=lv_obj_get_child_cnt(parent);//获取当前父对象下所有子组件数量
        //循环遍历每一个子组件
        for(uint32_t i=0;i<child_cnt;i++)
        {
            lv_obj_t*child=lv_obj_get_child(parent,i);
            if(child!=NULL)
            {
                //检查子组件是否包含隐藏标志
                if(lv_obj_has_flag(child,LV_OBJ_FLAG_HIDDEN))lv_obj_clear_flag(child,LV_OBJ_FLAG_HIDDEN);//如果隐藏，变显示
                else lv_obj_add_flag(child,LV_OBJ_FLAG_HIDDEN);
            }
        }
    }

    void ui_navigate_to(lv_obj_t *tarscreen)
    {
        if(lv_scr_act()==objects.desktop)stack_len=0;
        screenstack[stack_len++]=lv_scr_act();
        if(tarscreen!=objects.pinscreen && tarscreen != objects.lockscreen)
        {
            lv_obj_set_parent(objects.fixed_key,tarscreen);
            lv_obj_move_foreground(objects.fixed_key);
        }

        lv_scr_load(tarscreen);
    }

    void action_change_screen(lv_event_t *e)
    {
        lv_event_code_t code=lv_event_get_code(e);//获取当前事件类型代码
        lv_obj_t *currentscr=lv_event_get_target(e);//获取当前触发事件屏幕
        if (code == LV_EVENT_GESTURE)
        {
            lv_dir_t dir =lv_indev_get_gesture_dir(lv_indev_get_act());//获取手势方向
            lv_obj_t *targetscr=nullptr;//目标屏幕
            lv_scr_load_anim_t anim_type=LV_SCR_LOAD_ANIM_NONE;//动画
            uint32_t time_ms=300;

            if (currentscr == objects.lockscreen)
            {
                switch (dir)
                {
                    case LV_DIR_LEFT:
                        anim_type =LV_SCR_LOAD_ANIM_MOVE_LEFT;//左滑，屏幕从右边推入
                        break;
                    case LV_DIR_RIGHT:
                        anim_type =LV_SCR_LOAD_ANIM_MOVE_RIGHT;//右滑，屏幕从左边推入
                        break;
                    case LV_DIR_TOP:
                        anim_type =LV_SCR_LOAD_ANIM_MOVE_TOP;//上滑，屏幕从下边推入
                        break;
                    case LV_DIR_BOTTOM:
                        anim_type =LV_SCR_LOAD_ANIM_MOVE_BOTTOM;//下滑，屏幕从上边推入
                        break;
                    default:
                        break;
                }
                if(stack_len!=0)
                {
                    targetscr =screenstack[--stack_len];
                    lv_obj_set_parent(objects.fixed_key,targetscr);
                    lv_obj_move_foreground(objects.fixed_key);
                    lv_scr_load(targetscr);
                    if(targetscr==objects.desktop)stack_len=0;
                }
                else targetscr=objects.desktop;
                if(targetscr!=nullptr)lv_scr_load_anim(targetscr, anim_type, time_ms, 0,  false);
            }
            
            else if(currentscr == objects.pinscreen)
            {
                if(dir!= LV_DIR_NONE)
                {
                    ui_toggle(objects.pinscreen);
                    display_update(objects.input1_keyboard);
                }
            }

        }
        if(code == LV_EVENT_CLICKED)
        {
            if(currentscr == objects.set )ui_navigate_to(objects.setscreen);//加载到设置界面
            if(currentscr == objects.user_but)ui_navigate_to(objects.userscreen);//加载到用户界面
            if(currentscr == objects.pin_set)//显示键盘输入
            {
                ui_toggle(objects.userscreen);
                lv_obj_add_flag(objects.logout_text_hide,LV_OBJ_FLAG_HIDDEN);
                display_update(objects.input2_keyboard);

            }
            if(currentscr == objects.logout)//显示确定取消键
            {
                lv_obj_add_flag(objects.fixed_key,LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(objects.logout_text_hide,LV_OBJ_FLAG_HIDDEN);
            }
            if(currentscr == objects.home)
            {
                if(lv_scr_act()==objects.dinosaurscreen)dinosaurrun::getinstance().stop_game();

                if(lv_scr_act()==objects.carcontrol)
                {
                    car_comm_send_cmd(0,0,128);
                    CarTrajectory::getinstance().canvas_clear();
                    if(g_pure_pursuit_ptr)g_pure_pursuit_ptr->reset(0.0f,0.0f,0.0f);
                    g_trajectory_queue.clear();
                }

                ui_navigate_to(objects.desktop);//返回桌面
                stack_len=0;
            }
            if(currentscr == objects.set_speed_but)
            {
                lv_obj_clear_flag(objects.input3_keyboard,LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(objects.fixed_key,LV_OBJ_FLAG_HIDDEN);
            }
            if(currentscr == objects.car_control)ui_navigate_to(objects.carscreen);
            if(currentscr == objects.bluetooth_t)ui_navigate_to(objects.bluetoothscreen);
            if(currentscr==objects.car){ui_navigate_to(objects.carcontrol);car_comm_send_cmd(0,0,128);}
        }
        
    }

    void action_get_pin(lv_event_t *e)
    {
        lv_event_code_t code= lv_event_get_code(e);
        if(code!=LV_EVENT_CLICKED)return;

        lv_obj_t *btnm=lv_event_get_target(e);//获取触发事件的控件
        uint32_t id=lv_btnmatrix_get_selected_btn(btnm);//获取被点击按钮的索引
        const char *txt=lv_btnmatrix_get_btn_text(btnm,id);//获取被点击按钮文本
        if(btnm==objects.input1_keyboard)
        {
            if(txt==NULL)return;
            else if(strcmp(txt,"back")==0)
            {
                pin_len=0;
                ui_toggle(objects.pinscreen);
            }
            else if(strcmp(txt,"del")==0&&pin_len>0)
            {
                --pin_len;
                pin_buffer[pin_len]='\0';
            }
            else if(txt[0]>='0'&&txt[0]<='9')
            {
                if(pin_len<6)
                {
                    pin_buffer[pin_len++]=txt[0];
                    pin_buffer[pin_len]='\0';
                }
                
            }
            display_update(objects.input1_keyboard);
            if(pin_len==6)
            {
                if(strcmp(pin_buffer,correct_pin)==0)
                {
                    lv_obj_t *targetscr;
                    if(stack_len!=0)
                    {
                        targetscr =screenstack[--stack_len];
                        lv_obj_set_parent(objects.fixed_key,targetscr);
                        lv_obj_move_foreground(objects.fixed_key);
                        lv_scr_load(targetscr);
                        if(targetscr==objects.desktop)stack_len=0;
                    }
                    else targetscr=objects.desktop;
                    lv_scr_load(targetscr);
                    pin_len=0;
                    pin_buffer[0]='\0';
                    display_update(objects.input1_keyboard);
                }
                else
                {
                    pin_len=0;
                    pin_buffer[0]='\0';
                    display_update(objects.input1_keyboard);
                }
            }
        }
        if(btnm==objects.input2_keyboard)
        {
            if(txt==NULL)return;
            else if(strcmp(txt,"del")==0&&pin_len>0)
            {
                --pin_len;
                cur_pin[pin_len]='\0';
            }
            else if(strcmp(txt,"back")==0)
            {
                pin_len=0;
                ui_toggle(objects.userscreen);
                lv_obj_add_flag(objects.logout_text_hide,LV_OBJ_FLAG_HIDDEN);
            }
            else if(txt[0]>='0'&&txt[0]<='9')
            {
                if(pin_len<6)
                {
                    cur_pin[pin_len++]=txt[0];
                    cur_pin[pin_len]='\0';
                }
            }
            display_update(objects.input2_keyboard);
            if(pin_len==6)
            {
                ui_toggle(objects.userscreen);
                lv_obj_add_flag(objects.logout_text_hide,LV_OBJ_FLAG_HIDDEN);                
                strcpy(correct_pin,cur_pin);
                nvs_save_correct_pin(correct_pin);//存入nvs中防止断电丢失
                pin_len=0;
                display_update(objects.input2_keyboard);
            }
            
        }
        if(btnm==objects.input3_keyboard)
        {
            if(txt==NULL)return;
            else if(strcmp(txt,"back")==0)
            {
                char buffer[32];
                snprintf(buffer,sizeof(buffer),"set_speed:%d",global_car_speed);
                lv_label_set_text(objects.speed_input,buffer);
                speed_len=0;
                cur_speed_str[0]='\0';
                lv_obj_add_flag(objects.input3_keyboard,LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(objects.fixed_key,LV_OBJ_FLAG_HIDDEN);
            }
            else if(strcmp(txt,"del")==0&&speed_len>0)
            {
                --speed_len;
                cur_speed_str[speed_len]='\0';
                display_update(objects.input3_keyboard);
            }
            else if(strcmp(txt,"sure")==0)
            {

                int temp_speed = 0;
                if(speed_len>0)sscanf(cur_speed_str, "%d", &temp_speed);
                if(temp_speed > 100) temp_speed = 100;
                if(temp_speed < 0) temp_speed = 0;
                global_car_speed =temp_speed;

                char buffer[32];
                snprintf(buffer,sizeof(buffer),"set_speed:%d",global_car_speed);
                lv_label_set_text(objects.speed_input,buffer);

                lv_obj_add_flag(objects.input3_keyboard,LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(objects.fixed_key,LV_OBJ_FLAG_HIDDEN);
                speed_len =0;
                cur_speed_str[0]='\0';
            }
            else if(txt[0]>='0'&&txt[0]<='9')
            {
                if(speed_len<3)
                {
                    cur_speed_str[speed_len++]=txt[0];
                    cur_speed_str[speed_len]='\0';
                    display_update(objects.input3_keyboard);
                }
                if(speed_len==3)
                {
                    int speed=0;
                    sscanf(cur_speed_str,"%d",&speed);
                    if(speed > 100) speed = 100;
                    if(speed < 0) speed = 0;
                    snprintf(cur_speed_str, sizeof(cur_speed_str), "%d", speed);
                    display_update(objects.input3_keyboard);

                }    
            }
        
        }

    }

    void action_sure(lv_event_t *e)
    {
        correct_pin[0]='\0';
        nvs_save_correct_pin(correct_pin);
        lv_obj_add_flag(objects.logout_text_hide,LV_OBJ_FLAG_HIDDEN); 
        lv_obj_clear_flag(objects.fixed_key,LV_OBJ_FLAG_HIDDEN);
    }

    void action_cancel(lv_event_t *e)
    {
        lv_obj_add_flag(objects.logout_text_hide,LV_OBJ_FLAG_HIDDEN); 
        lv_obj_clear_flag(objects.fixed_key,LV_OBJ_FLAG_HIDDEN);
    }

    void action_back_pre_scr(lv_event_t *e)
    {
        lv_event_code_t code =lv_event_get_code(e);
        lv_obj_t *but =lv_event_get_target(e);
        if(code == LV_EVENT_CLICKED)
        {
            if(but==objects.back || but == objects.user_return || but == objects.user_return_blue)
            {
                if(stack_len==0)return;
                
                if(lv_scr_act()==objects.dinosaurscreen)dinosaurrun::getinstance().stop_game();
                
                if(lv_scr_act()==objects.carcontrol)
                {
                    car_comm_send_cmd(0,0,128);
                    CarTrajectory::getinstance().canvas_clear();
                    if(g_pure_pursuit_ptr)g_pure_pursuit_ptr->reset(0.0f,0.0f,0.0f);
                    g_trajectory_queue.clear();
                }

                lv_obj_t *tarscreen =screenstack[--stack_len];
                lv_obj_set_parent(objects.fixed_key,tarscreen);
                lv_obj_move_foreground(objects.fixed_key);
                lv_scr_load(tarscreen);

                if(tarscreen==objects.desktop)stack_len=0;


            }
        }

    }

    void action_car_control(lv_event_t *e)
    {
        lv_event_code_t code = lv_event_get_code(e);
        lv_obj_t *but = lv_event_get_target(e);
        

        int cur_speed =global_car_speed;

        
        if(code == LV_EVENT_PRESSED)
        {
            if(but == objects.car_go)
            {
                send_car_cmd(0x01,cur_speed);
            }
            if(but == objects.car_back)
            {
                send_car_cmd(0x02,cur_speed);
            }
            if(but == objects.car_left)
            {
                send_car_cmd(0x03,cur_speed);
            }
            if(but == objects.car_right)
            {
                send_car_cmd(0x04,cur_speed);
            }
            if(but == objects.car_stop)send_car_cmd(0x05, 0);
        }
        else if(code == LV_EVENT_PRESS_LOST || code == LV_EVENT_RELEASED)
        {
            send_car_cmd(0x05, 0);
        }

    }

    void action_switch_onoff(lv_event_t *e)
    {
        lv_event_code_t code = lv_event_get_code(e);
        lv_obj_t *switchbut = lv_event_get_target(e);
        if(code == LV_EVENT_CLICKED)
        {
            if(switchbut == objects.bluetooth_switch)
            {
                if(lv_obj_has_state(switchbut,LV_STATE_CHECKED))
                {
                    #if defined(USE_SDL)
                        printf("open bluetooth\n");
                    #endif  

                    #if defined(ESP_PLATFORM)
                        bsp_bt_ble_start();
                    #endif
                }
                else 
                {
                    #if defined(USE_SDL)
                        printf("close bluetooth\n");
                    #endif

                    #if defined(ESP_PLATFORM)
                        bsp_bt_ble_stop();
                    #endif
                }
            }
        }

    }

    void action_game_jump_control(lv_event_t *e)
    {
        lv_event_code_t code = lv_event_get_code(e);
        lv_obj_t *obj=lv_event_get_target(e);
        if(code == LV_EVENT_CLICKED)
        {
            if(obj == objects.dinosaurscreen)dinosaurrun::game_input_event_cb(&dinosaurrun::getinstance());
        }
        
    }

    void action_start_dinosaur_run(lv_event_t *e)
    {
        lv_event_code_t code = lv_event_get_code(e);
        if(code == LV_EVENT_CLICKED)
        {
            ui_navigate_to(objects.dinosaurscreen);
            dinosaurrun::getinstance().start_game();
        
            
        }
    }

    void action_canvas_touch(lv_event_t * e)
    {
        lv_event_code_t code=lv_event_get_code(e);
        lv_indev_t *indev=lv_indev_get_act();
        if(!indev)return;

        lv_point_t point;
        lv_indev_get_point(indev,&point);
        if(code==LV_EVENT_PRESSED)
        {
            //起始，清空画布，获取坐标
            CarTrajectory::getinstance().canvas_clear();
            CarTrajectory::raw_touch_path.push_back(Point2D{(float)point.x,(float)point.y});
        }
        if(code==LV_EVENT_PRESSING)
        {
            //滑动中，获取坐标
            CarTrajectory::raw_touch_path.push_back(Point2D{(float)point.x,(float)point.y});
        }
        if(code==LV_EVENT_PRESS_LOST||code==LV_EVENT_RELEASED)
        {
            if(CarTrajectory::raw_touch_path.size()>=2)
            {
                BezierCurve::generateFromTouchPath(CarTrajectory::raw_touch_path,CarTrajectory::all_points,30);
                CarTrajectory::getinstance().canvas_drawbezier();               
            }
        }
    }
}




