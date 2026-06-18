#include <Arduino.h>
#include <SoftwareSerial.h>

#include "lighting.h"

#define DISPLAY_SERIAL_BAUD 9600
#define DISPLAY_RX_PIN 5
#define UNUSED_TX_PIN 4
#define SERIAL_BUFFER_SIZE 64

SoftwareSerial displaySerial(DISPLAY_RX_PIN, UNUSED_TX_PIN);

char serialBuffer[SERIAL_BUFFER_SIZE];
uint8_t serialBufferIndex = 0;

void handle_display_line(const char* line);
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
  update_lighting();
}

void read_display_serial()
{
  while (displaySerial.available())
  {
    char c = displaySerial.read();

    if (c == '\r') continue;

    if (c == '\n')
    {
      serialBuffer[serialBufferIndex] = '\0';
      handle_display_line(serialBuffer);
      serialBufferIndex = 0;
      continue;
    }

    if (serialBufferIndex < SERIAL_BUFFER_SIZE - 1)
    {
      serialBuffer[serialBufferIndex++] = c;
    }
    else
    {
      serialBufferIndex = 0;
    }
  }
}

void handle_display_line(const char* line)
{
  int hue;
  int saturation;
  int brightness;
  int isLightingOn;
  int animationIndex;

  if (sscanf(line, "L %d %d %d %d %d", &hue, &saturation, &brightness, &isLightingOn, &animationIndex) != 5)
  {
    return;
  }

  GLOBAL_CONFIG.lighting_hue = constrain(hue, 0, 255);
  GLOBAL_CONFIG.saturation = constrain(saturation, 0, 255);
  GLOBAL_CONFIG.brightness = constrain(brightness, 0, 255);
  GLOBAL_CONFIG.is_lighting_on = isLightingOn ? 1 : 0;
  GLOBAL_CONFIG.animation_index = animationIndex;

  set_all_flags();
  update_lighting();
}
