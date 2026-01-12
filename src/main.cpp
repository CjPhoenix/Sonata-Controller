#include <lvgl.h>
#include <Wire.h>
#include <FastLED.h>

#include "widgets.h"
#include "display.h"

// Screen size information
#define LV_HOR_RES_MAX 800
#define LV_VER_RES_MAX 480

void update_from_file();

lv_obj_t* screen;

void setup() 
{
  Serial.begin(115200);
  Serial.println("Starting setup...");
  // Wait 800ms to wait for voltage to level out when system starts
  // delay(800);

  if (config_init()) update_config_from_file();

  lighting_init();

  display_init();

  // Main screen object
  screen = lv_scr_act();
  lv_obj_set_style_bg_color(screen, C_BACKGROUND, LV_PART_MAIN);

  widgets_init(screen);

  // Enable backlight and set LEDs to finish setup
  backlight_init();
  update_lighting(1);
  Serial.println("Setup complete. Starting loop.");
}

char *preview_text = (char*) malloc(sizeof(char) * 128);
void loop() 
{
  // Update led strip lighting and display preview to match
  if (update_lighting()) lv_obj_set_style_shadow_color(toggle_button, lv_color_hsv_to_rgb(GLOBAL_CONFIG.lighting_hue * 360 / 255, GLOBAL_CONFIG.saturation * 100 / 255, GLOBAL_CONFIG.brightness * GLOBAL_CONFIG.is_lighting_on * 100 / 255), LV_PART_MAIN);

  lv_timer_handler();

  ts.read();
  sleep_backlight_if_inactive(ts.isTouched);

  delay(5);
}

// -------------------
//      Utility
// -------------------
void update_from_file()
{
  // Update local data
  update_config_from_file();

  // Update UI to match new local data
  lv_slider_set_value(brightness_slider, GLOBAL_CONFIG.brightness, LV_ANIM_OFF);
  lv_slider_set_value(hue_slider, GLOBAL_CONFIG.lighting_hue, LV_ANIM_OFF);
  lv_slider_set_value(saturation_slider, GLOBAL_CONFIG.saturation, LV_ANIM_OFF);

  // Update lighting to match new local data
  update_lighting(1);
  lv_obj_set_style_shadow_color(toggle_button, lv_color_hsv_to_rgb(GLOBAL_CONFIG.lighting_hue * 360 / 255, GLOBAL_CONFIG.saturation * 100 / 255, GLOBAL_CONFIG.brightness * GLOBAL_CONFIG.is_lighting_on * 100 / 255), LV_PART_MAIN);
}