#pragma once

typedef struct {
    int left_hue;
    int left_saturation;
    int left_brightness;
    int left_is_lighting_on;

    int right_hue;
    int right_saturation;
    int right_brightness;
    int right_is_lighting_on;

    int animation_index;
} Config;

static Config GLOBAL_CONFIG = {
    0,
    255,
    0,
    1,

    0,
    255,
    0,
    1,

    0
};
