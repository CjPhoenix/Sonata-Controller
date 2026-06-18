#pragma once

#include <Arduino.h>

#include "config.h"

#define CONTROLLER_SERIAL_BAUD 115200
#define CONTROLLER_TX_PIN 38

#define FAST_LIGHTING_UPDATES 1

#define ANIM_STARTUP    1

void controller_init();

void fire_animation(int index);
void sync();
void toggle_lighting();
void set_brightness(int level);
void set_hue(int hue);
void set_saturation(int saturation);
void set_animation(int animation_index, const int* params, int params_size);

void controller_init()
{
    Serial1.begin(CONTROLLER_SERIAL_BAUD, SERIAL_8N1, -1, CONTROLLER_TX_PIN);
}

void fire_animation(int index)
{
    Serial1.printf(
        "F %d\n",
        index
    );
    Serial1.flush();
}

void sync()
{
    Serial1.printf(
        "L %d %d %d %d %d\n",
        GLOBAL_CONFIG.lighting_hue,
        GLOBAL_CONFIG.saturation,
        GLOBAL_CONFIG.brightness,
        GLOBAL_CONFIG.is_lighting_on,
        GLOBAL_CONFIG.animation_index
    );
    Serial1.flush();
}

void toggle_lighting()
{
    GLOBAL_CONFIG.is_lighting_on = GLOBAL_CONFIG.is_lighting_on ? 0 : 1;
    Serial1.printf(
        "V %d\n",
        GLOBAL_CONFIG.is_lighting_on
    );
    Serial1.flush();
}

void set_brightness(int level)
{
    GLOBAL_CONFIG.brightness = level;
    Serial1.printf(
        "B %d\n",
        level
    );
    Serial1.flush();
}

void set_hue(int hue)
{
    GLOBAL_CONFIG.lighting_hue = hue;
    Serial1.printf(
        "H %d\n",
        hue
    );
    Serial1.flush();
}

void set_saturation(int saturation)
{
    GLOBAL_CONFIG.saturation = saturation;
    Serial1.printf(
        "S %d\n",
        saturation
    );
    Serial1.flush();
}

void set_animation(int animation_index, const int*, int)
{
    GLOBAL_CONFIG.animation_index = animation_index;
    Serial1.printf(
        "A %d\n",
        animation_index
    );
    Serial1.flush();
}
