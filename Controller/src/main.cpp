#include <Arduino.h>
#include <SoftwareSerial.h>

#include "lighting.h"
#include "lighting_protocol.h"

#define DISPLAY_SERIAL_BAUD 38400
#define DISPLAY_RX_PIN 5
#define UNUSED_TX_PIN 4

SoftwareSerial displaySerial(DISPLAY_RX_PIN, UNUSED_TX_PIN);

uint8_t serialPacket[4];
uint8_t serialPacketIndex = 0;
bool serialPacketActive = false;
bool pendingPacketRender = false;
unsigned long lastPacketTime = 0;

void read_display_serial();

void setup()
{
  Serial.begin(115200);
  displaySerial.begin(DISPLAY_SERIAL_BAUD);

  lighting_init();
  update_lighting(1);

  Serial.println("Lighting controller ready.");
}

void loop()
{
  read_display_serial();

  if (pendingPacketRender && (millis() - lastPacketTime) >= 16)
  {
    render_leds();
    pendingPacketRender = false;
  }

  update_lighting();
}

void read_display_serial()
{
  while (displaySerial.available())
  {
    uint8_t byte = (uint8_t)displaySerial.read();

    if (!serialPacketActive)
    {
      if (byte == LIGHTING_PACKET_START_BYTE)
      {
        serialPacketIndex = 0;
        serialPacketActive = true;
      }
      continue;
    }

    serialPacket[serialPacketIndex++] = byte;

    if (serialPacketIndex >= 4)
    {
      apply_lighting_packet(serialPacket-1);
      serialPacketIndex = 0;
      serialPacketActive = false;
      pendingPacketRender = true;
      lastPacketTime = millis();
    }
  }
}
