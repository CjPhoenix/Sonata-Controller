#include <lvgl.h>
#include <Wire.h>

// #define WIDGETS_OLD

#include "widgets.h"
#include "display.h"
#include "wireless.h"
#include "controller.h"

// Screen size information
#define LV_HOR_RES_MAX 800
#define LV_VER_RES_MAX 480

void screen_init();
void startup();

lv_obj_t* screen;

void GuiTask(void *pvParameters);

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting setup...");

  if (!config_init())
    // write_config_to_file();
    update_config_from_file();

  wireless_init();

  screen_init();

  xTaskCreatePinnedToCore(
    GuiTask,     /* Task function */
    "GuiTask",   /* Name of task */
    8192,        /* Stack size in words */
    NULL,        /* Task input parameter */
    2,           /* Priority of the task */
    NULL,        /* Task handle */
    1            /* Pin to Core 1 */
  );

  controller_init();

  startup();

  Serial.println("Setup complete. Starting loop.");

  update_lighting(1, 1, 0, 255, 255, 1);
}

void GuiTask(void *pvParameters) {
  while (1) {
    lv_timer_handler();
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

void loop()
{
  touch_touched();
  sleep_backlight_if_inactive(touch_has_signal());

  delay(30);
}

// -------------------
//      Initialisers
// -------------------
void screen_init()
{
  display_init();

  screen = lv_scr_act();
  widgets_init(screen);

  backlight_init();
}

void startup()
{
  // update_lighting_from_config();
}
