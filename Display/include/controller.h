#pragma once

#include <Arduino.h>

#include "config.h"
#include "lighting_protocol.h"

#define CONTROLLER_SERIAL_BAUD 38400
#define CONTROLLER_TX_PIN 38

#define FAST_LIGHTING_UPDATES 1

#define ANIM_STARTUP    1

void toggle_lighting();

void controller_init()
{
    Serial1.begin(CONTROLLER_SERIAL_BAUD, SERIAL_8N1, -1, CONTROLLER_TX_PIN);
}

void update_lighting(int left, int right, uint8_t hue, uint8_t sat, uint8_t val, int enabled)
{
    uint8_t packet[5];
    lighting_packet_encode(packet, LIGHTING_PACKET_TYPE_COLOR, enabled, LIGHTING_PACKET_TARGET_LEFT | LIGHTING_PACKET_TARGET_RIGHT, hue, sat, val);

    Serial1.write(packet, 5);
}

void update_brightness(int left, int right, uint8_t val)
{
    uint8_t packet[5];
    lighting_packet_encode(packet, LIGHTING_PACKET_TYPE_BRIGHTNESS, true, LIGHTING_PACKET_TARGET_LEFT | LIGHTING_PACKET_TARGET_RIGHT, 0, 0, val);

    Serial1.write(packet, 5);
}

void update_lighting_from_config()
{
    update_lighting(1, 0, GLOBAL_CONFIG.hue_l, GLOBAL_CONFIG.sat_l, GLOBAL_CONFIG.val_l, GLOBAL_CONFIG.is_lighting_on);
    update_lighting(0, 1, GLOBAL_CONFIG.hue_r, GLOBAL_CONFIG.sat_r, GLOBAL_CONFIG.val_r, GLOBAL_CONFIG.is_lighting_on);
}