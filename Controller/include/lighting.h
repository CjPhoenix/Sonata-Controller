#pragma once

#include <FastLED.h>
#include <stdbool.h>

#include "config.h"
#include "lighting_protocol.h"

// ----------------------------------------
// HARDWARE DEFINITIONS
// ----------------------------------------
#define LED_PIN_1   6
#define LED_COUNT_1 144

#define LED_PIN_2   7
#define LED_COUNT_2 144

#define FAST_LIGHTING_UPDATES 1

// ----------------------------------------
// UPDATE FLAGS
// ----------------------------------------
#define UPDATE_FLAG_SATURATION  0x1
#define UPDATE_FLAG_BRIGHTNESS  0x2
#define UPDATE_FLAG_HUE         0x4

// ----------------------------------------
// ANIMATION INDICES
// ----------------------------------------
#define ANIM_STARTUP    1

// Lighting updates and timestamps
bool first_update_called;
unsigned int update_flags;

// Lighting hardware references
CRGB strip1[LED_COUNT_1];
CRGB strip2[LED_COUNT_2];

// ----------------------------------------
// FUNCTION DEFINITIONS
// ----------------------------------------
void lighting_init();
int update_lighting(int);
void render_leds();
void apply_lighting_packet(const uint8_t packet[LIGHTING_PACKET_SIZE]);

void fire_animation(int index);
void set_brightness(int level);
void set_visible(int);
void set_hue(int hue);
void set_saturation(int saturation);

void set_all_flags();


void lighting_init()
{
    FastLED.addLeds<WS2812, LED_PIN_1, GRB>(strip1, LED_COUNT_1);
    FastLED.addLeds<WS2812, LED_PIN_2, GRB>(strip2, LED_COUNT_2);

    set_all_flags();
    render_leds();
}

/**
 * Returns true if any lighting data was changed
 */
int update_lighting(int force_all = 0)
{
    if (force_all) set_all_flags();

    if (update_flags)
    {
        first_update_called = 1;
        if (update_flags & (UPDATE_FLAG_HUE | UPDATE_FLAG_SATURATION | UPDATE_FLAG_BRIGHTNESS))
        {
            render_leds();
            update_flags &= ~(UPDATE_FLAG_HUE | UPDATE_FLAG_SATURATION | UPDATE_FLAG_BRIGHTNESS);
        }

        return 1;
    }
    else
    {
        return 0;
    }
}

void render_leds()
{
    for (int i = 0; i < LED_COUNT_1; i++)
    {
        strip1[i] = CHSV(
            (uint8_t)GLOBAL_CONFIG.left_hue,
            (uint8_t)GLOBAL_CONFIG.left_saturation,
            GLOBAL_CONFIG.left_is_lighting_on ? (uint8_t)GLOBAL_CONFIG.left_brightness : 0
        );
    }

    for (int i = 0; i < LED_COUNT_2; i++)
    {
        strip2[i] = CHSV(
            (uint8_t)GLOBAL_CONFIG.right_hue,
            (uint8_t)GLOBAL_CONFIG.right_saturation,
            GLOBAL_CONFIG.right_is_lighting_on ? (uint8_t)GLOBAL_CONFIG.right_brightness : 0
        );
    }

    FastLED.show();
}

void apply_lighting_packet(const uint8_t packet[LIGHTING_PACKET_SIZE])
{
    uint8_t packet_type;
    bool enabled;
    uint8_t target_mask;
    uint8_t hue;
    uint8_t sat;
    uint8_t val;
    lighting_packet_decode(packet, packet_type, enabled, target_mask, hue, sat, val);

    if (target_mask == 0)
    {
        target_mask = LIGHTING_PACKET_TARGET_LEFT | LIGHTING_PACKET_TARGET_RIGHT;
    }

    if (packet_type == LIGHTING_PACKET_TYPE_COLOR)
    {
        if (target_mask & LIGHTING_PACKET_TARGET_LEFT)
        {
            GLOBAL_CONFIG.left_hue = hue;
            GLOBAL_CONFIG.left_saturation = sat;
            GLOBAL_CONFIG.left_brightness = val;
            GLOBAL_CONFIG.left_is_lighting_on = enabled;
        }

        if (target_mask & LIGHTING_PACKET_TARGET_RIGHT)
        {
            GLOBAL_CONFIG.right_hue = hue;
            GLOBAL_CONFIG.right_saturation = sat;
            GLOBAL_CONFIG.right_brightness = val;
            GLOBAL_CONFIG.right_is_lighting_on = enabled;
        }
    }
    else if (packet_type == LIGHTING_PACKET_TYPE_BRIGHTNESS)
    {
        if (target_mask & LIGHTING_PACKET_TARGET_LEFT)
        {
            GLOBAL_CONFIG.left_brightness = val;
        }

        if (target_mask & LIGHTING_PACKET_TARGET_RIGHT)
        {
            GLOBAL_CONFIG.right_brightness = val;
        }
    }
}

void toggle_lighting()
{
    GLOBAL_CONFIG.left_is_lighting_on = GLOBAL_CONFIG.left_is_lighting_on ? 0 : 1;
    GLOBAL_CONFIG.right_is_lighting_on = GLOBAL_CONFIG.right_is_lighting_on ? 0 : 1;
    update_flags |= UPDATE_FLAG_BRIGHTNESS;
}

void fire_animation(int index)
{
    if (index == ANIM_STARTUP)
    {
        for (int i = 0; i < 255; i++)
        {
            strip1[i] = CHSV(0, 1, 255);
            strip2[LED_COUNT_2-(i+1)] = CHSV(0, 1, 255);
            FastLED.show();
            delay(2);
        }
    }
}

void set_visible(int visible)
{
    GLOBAL_CONFIG.left_is_lighting_on = visible;
    GLOBAL_CONFIG.right_is_lighting_on = visible;
    update_flags |= UPDATE_FLAG_BRIGHTNESS;
}

void set_brightness(int level)
{
    GLOBAL_CONFIG.left_brightness = level;
    GLOBAL_CONFIG.right_brightness = level;
    update_flags |= UPDATE_FLAG_BRIGHTNESS;
}

void set_hue(int hue)
{
    GLOBAL_CONFIG.left_hue = hue;
    GLOBAL_CONFIG.right_hue = hue;
    update_flags |= UPDATE_FLAG_HUE;
}

void set_saturation(int saturation)
{
    GLOBAL_CONFIG.left_saturation = saturation;
    GLOBAL_CONFIG.right_saturation = saturation;
    update_flags |= UPDATE_FLAG_SATURATION;
}

void set_all_flags()
{
    update_flags = UPDATE_FLAG_BRIGHTNESS | UPDATE_FLAG_HUE | UPDATE_FLAG_SATURATION;
}
