#pragma once

#include <lvgl.h>

#include "touch.h"
#include "backlight.h"

LGFX lcd;

// Display
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *disp_draw_buf;
static lv_disp_drv_t disp_drv;

// Input
static lv_indev_drv_t indev_drv;

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{

  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);


  //lcd.fillScreen(TFT_WHITE);
  lcd.pushImageDMA(area->x1, area->y1, w, h,(lgfx::rgb565_t*)&color_p->full);
  
  lv_disp_flush_ready(disp);
}

void my_touchpad_read(lv_indev_drv_t *drv, lv_indev_data_t *data) {
  lgfx::v1::touch_point_t tp;
  if (lcd.getTouchRaw(&tp, 1) > 0) {
    data->state = LV_INDEV_STATE_PR;
    data->point.x = tp.x;
    data->point.y = tp.y;
  } else {
    data->state = LV_INDEV_STATE_REL;
  }
}

void display_init() {
  touch_init();

  lcd.init();
  lcd.setRotation(0);
  lv_init();

  // Calculate a safe size: 800 pixels * 16 lines * 2 bytes (RGB565) = 25,600 bytes
  // We allocate TWO tracking buffers inside fast internal RAM
  uint32_t buffer_size = 800 * 48 * sizeof(lv_color_t);
  lv_color_t *buf1 = (lv_color_t *)heap_caps_malloc(buffer_size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  lv_color_t *buf2 = (lv_color_t *)heap_caps_malloc(buffer_size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

  if (buf1 == NULL) {
      while(1) delay(1000); // Halt if allocation failed
  }

  // Initialize the v8 display buffer descriptor
  lv_disp_draw_buf_init(&draw_buf, buf1, buf2, 800 * 48);

  
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = SCREEN_WIDTH;
  disp_drv.ver_res = SCREEN_HEIGHT;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  // input device (touch)
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);
}
