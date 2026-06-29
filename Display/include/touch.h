#pragma once

#include <Wire.h>
#include <lgfx_user/LGFX_Elecrow_ESP32_Display_WZ8048C050.h>

#define TOUCH_SDA 19
#define TOUCH_SCL 20
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480

int touch_last_x, touch_last_y;

extern LGFX lcd;

void touch_init()
{
  Wire.begin(TOUCH_SDA, TOUCH_SCL);
  Wire.setClock(400000);
}

bool touch_has_signal()
{
  lgfx::v1::touch_point_t tp;
  return lcd.getTouchRaw(&tp, 1) > 0;
}

bool touch_touched()
{
  lgfx::v1::touch_point_t tp;
  if (lcd.getTouchRaw(&tp, 1) > 0)
  {
    touch_last_x = tp.x;
    touch_last_y = tp.y;
    return true;
  }
  return false;
}

bool touch_released()
{
  return !touch_has_signal();
}