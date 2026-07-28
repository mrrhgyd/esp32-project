#ifndef EEZ_LVGL_UI_IMAGES_H
#define EEZ_LVGL_UI_IMAGES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const lv_img_dsc_t img_image_1;
extern const lv_img_dsc_t img_image_2;
extern const lv_img_dsc_t img_image_3;
extern const lv_img_dsc_t img_image_4;
extern const lv_img_dsc_t img_image_5;
extern const lv_img_dsc_t img_image_6;
extern const lv_img_dsc_t img_image_7;
extern const lv_img_dsc_t img_image_8;
extern const lv_img_dsc_t img_image_9;
extern const lv_img_dsc_t img_image_10;
extern const lv_img_dsc_t img_set;
extern const lv_img_dsc_t img_bluetooth;
extern const lv_img_dsc_t img_car;
extern const lv_img_dsc_t img_dinosaur;

#ifndef EXT_IMG_DESC_T
#define EXT_IMG_DESC_T
typedef struct _ext_img_desc_t {
    const char *name;
    const lv_img_dsc_t *img_dsc;
} ext_img_desc_t;
#endif

extern const ext_img_desc_t images[14];

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_IMAGES_H*/