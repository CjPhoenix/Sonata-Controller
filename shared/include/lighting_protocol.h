#pragma once

#include <stdint.h>
#include <stdbool.h>

// Shared packet framing for the display/controller link.
#define LIGHTING_PACKET_START_BYTE 0xA5
#define LIGHTING_PACKET_TYPE_MASK 0x03
#define LIGHTING_PACKET_TYPE_COLOR 0x00
#define LIGHTING_PACKET_TYPE_BRIGHTNESS 0x01
#define LIGHTING_PACKET_ENABLED 0x04
#define LIGHTING_PACKET_TARGET_LEFT 0x08
#define LIGHTING_PACKET_TARGET_RIGHT 0x10
#define LIGHTING_PACKET_SIZE 5

inline void lighting_packet_encode(
    uint8_t packet[LIGHTING_PACKET_SIZE],
    uint8_t packet_type,
    bool enabled,
    uint8_t target_mask,
    uint8_t value1,
    uint8_t value2,
    uint8_t value3)
{
    packet[0] = LIGHTING_PACKET_START_BYTE;
    packet[1] = (packet_type & LIGHTING_PACKET_TYPE_MASK) |
                (enabled ? LIGHTING_PACKET_ENABLED : 0x00) |
                (target_mask & (LIGHTING_PACKET_TARGET_LEFT | LIGHTING_PACKET_TARGET_RIGHT));
    packet[2] = value1;
    packet[3] = value2;
    packet[4] = value3;
}

inline void lighting_packet_decode(
    const uint8_t packet[LIGHTING_PACKET_SIZE],
    uint8_t& packet_type,
    bool& enabled,
    uint8_t& target_mask,
    uint8_t& value1,
    uint8_t& value2,
    uint8_t& value3)
{
    packet_type = packet[1] & LIGHTING_PACKET_TYPE_MASK;
    enabled = (packet[1] & LIGHTING_PACKET_ENABLED) != 0;
    target_mask = packet[1] & (LIGHTING_PACKET_TARGET_LEFT | LIGHTING_PACKET_TARGET_RIGHT);
    value1 = packet[2];
    value2 = packet[3];
    value3 = packet[4];
}
