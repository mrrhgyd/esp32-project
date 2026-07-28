#ifndef LV_DRV_CONF_H
#define LV_DRV_CONF_H

#include <stdint.h>

/* 将 0 改为 1，正式激活整个配置文件 */
#define USE_SDL 1

#if USE_SDL
    #define SDL_APPLE_MONITOR_DIR /System/Library/Fonts
    #define LV_USE_SDL              1
    #define LV_SDL_FULLSCREEN       0
    #define LV_SDL_MOUSEWHEEL_MODE  LV_SDL_MOUSEWHEEL_MODE_ENCODER
    /* 模拟器窗口的缩放比例 (100 代表 1.0 倍) */
    #define SDL_ZOOM              1

    /* 硬件加速与显示配置 */
    #define SDL_HOR_RES           240    // 电脑弹出的模拟器横向分辨率
    #define SDL_VER_RES           320     // 电脑弹出的模拟器纵向分辨率


    /* 像素位深：必须和你们 lv_conf.h 里的色彩深度一致 (常用 16 或 32) */
    #define SDL_BIT_PER_PIXEL     32
#endif

/* 禁用其他没用的硬件驱动（如树莓派、显示屏裸驱动等） */
#define USE_X11       0
#define USE_FBDEV     0
#define USE_EVDEV     0

#endif /*LV_DRV_CONF_H*/