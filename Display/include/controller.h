#pragma once

#include <Arduino.h>

#include "config.h"

#define CONTROLLER_SERIAL_BAUD 9600
// #define CONTROLLER_TX_PIN 5
#define CONTROLLER_TX_PIN 38

#define FAST_LIGHTING_UPDATES 1

#define UPDATE_FLAG_SATURATION  0x1
#define UPDATE_FLAG_BRIGHTNESS  0x2
#define UPDATE_FLAG_HUE         0x4
#define UPDATE_FLAG_ANIMATION   0x8

unsigned int update_flags;

void controller_init();
int update_lighting(int force_all = 0);

void toggle_lighting();
void set_brightness(int level);
void set_hue(int hue);
void set_saturation(int saturation);
void set_animation(int animation_index, const int* params, int params_size);
void set_all_flags();

void controller_init()
{
    Serial1.begin(CONTROLLER_SERIAL_BAUD, SERIAL_8N1, -1, CONTROLLER_TX_PIN);
    set_all_flags();
}

/**
 * Sends the current lighting state to the lighting controller.
 * Returns true when the display preview should be refreshed.
 */
int update_lighting(int force_all)
{
    if (force_all) set_all_flags();

    if (!update_flags) return 0;

    Serial1.printf(
        "L %d %d %d %d %d\n",
        GLOBAL_CONFIG.lighting_hue,
        GLOBAL_CONFIG.saturation,
        GLOBAL_CONFIG.brightness,
        GLOBAL_CONFIG.is_lighting_on,
        GLOBAL_CONFIG.animation_index
    );
    Serial1.flush();

    update_flags = 0;
    return 1;
}

void toggle_lighting()
{
    GLOBAL_CONFIG.is_lighting_on = GLOBAL_CONFIG.is_lighting_on ? 0 : 1;
    update_flags |= UPDATE_FLAG_BRIGHTNESS;
}

void set_brightness(int level)
{
    GLOBAL_CONFIG.brightness = level;
    update_flags |= UPDATE_FLAG_BRIGHTNESS;
}

void set_hue(int hue)
{
    GLOBAL_CONFIG.lighting_hue = hue;
    update_flags |= UPDATE_FLAG_HUE;
}

void set_saturation(int saturation)
{
    GLOBAL_CONFIG.saturation = saturation;
    update_flags |= UPDATE_FLAG_SATURATION;
}

void set_animation(int animation_index, const int*, int)
{
    GLOBAL_CONFIG.animation_index = animation_index;
    update_flags |= UPDATE_FLAG_ANIMATION;
}

void set_all_flags()
{
    update_flags = UPDATE_FLAG_BRIGHTNESS | UPDATE_FLAG_HUE | UPDATE_FLAG_SATURATION | UPDATE_FLAG_ANIMATION;
}
