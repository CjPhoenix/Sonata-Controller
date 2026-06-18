#pragma once

typedef struct {
    int lighting_hue;
    int saturation;
    int brightness;
    int is_lighting_on;
    int animation_index;
} Config;

static Config GLOBAL_CONFIG = {
    0,
    255,
    0,
    1,
    0
};
