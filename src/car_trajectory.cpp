#include "car_trajectory.hpp"
#include "bezier_curve.hpp"
#include "pure_pursuit.hpp"
#include "screens.h"
#include <stdio.h>
#include <string>
#if defined(ESP_PLATFORM)
#include <esp_heap_caps.h>
#include "esp_log.h"
#endif


lv_color_t* CarTrajectory::canvas_buf = nullptr;
std::vector<Point2D> CarTrajectory::raw_touch_path;
std::vector<Point2D> CarTrajectory::all_points;

CarTrajectory::CarTrajectory()
{
    size_t buf_size=LV_CANVAS_BUF_SIZE_TRUE_COLOR(CANVAS_WIDTH,CANVAS_HEIGHT)*sizeof(lv_color_t);
    #ifdef ESP_PLATFORM
    canvas_buf=(lv_color_t*)heap_caps_malloc(buf_size,MALLOC_CAP_SPIRAM|MALLOC_CAP_8BIT);//分配到psarm
    
    if(canvas_buf==NULL)
    {
        canvas_buf=(lv_color_t*)heap_caps_malloc(buf_size,MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT);
        printf("use dram buffer area\n");
    }
    else printf("use pasrm buffer area\n");
    #else
    canvas_buf=(lv_color_t*)malloc(buf_size);
    if(canvas_buf==NULL)printf("assign fail\n");
    #endif

    memset(canvas_buf,0,buf_size);
    lv_canvas_set_buffer(objects.canvas,canvas_buf,CANVAS_WIDTH,CANVAS_HEIGHT,LV_IMG_CF_TRUE_COLOR);
}

CarTrajectory &CarTrajectory::getinstance()
{
    static CarTrajectory instance;
    return instance;
}


void CarTrajectory::canvas_clear()
{
    CarTrajectory::all_points.clear();
    CarTrajectory::raw_touch_path.clear();
    lv_canvas_fill_bg(objects.canvas,lv_color_hex(0x352C71),LV_OPA_COVER);
}


void CarTrajectory::canvas_drawbezier()
{
    if(CarTrajectory::getinstance().all_points.size()<2)return;

    lv_draw_line_dsc_t line_dsc;
    lv_draw_line_dsc_init(&line_dsc);
    line_dsc.color=lv_color_hex(0x00ff00);
    line_dsc.width=4;                           //线条粗细
    line_dsc.round_start=1;                     //开启圆润端点
    line_dsc.round_end=1;
    line_dsc.opa=LV_OPA_COVER;                  //完全不透明

    for(int i=0;i<all_points.size()-1;++i)
    {
        lv_point_t point[2];
        point[0].x=(lv_coord_t)(all_points[i].x+0.5f);
        point[0].y=(lv_coord_t)(all_points[i].y+0.5f);
        point[1].x=(lv_coord_t)(all_points[i+1].x+0.5f);
        point[1].y=(lv_coord_t)(all_points[i+1].y+0.5f);

        lv_canvas_draw_line(objects.canvas,point,2,&line_dsc);
    }

    lv_obj_invalidate(objects.canvas);
}