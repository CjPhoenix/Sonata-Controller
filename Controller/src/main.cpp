#include <Arduino.h>
#include <SoftwareSerial.h>

#include "lighting.h"

#define DISPLAY_SERIAL_BAUD 115200
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
  int is_lighting_on;
  int animation_index;

  if (line[0] == 'L')
  {
    sscanf(line, "L %d %d %d %d %d", &hue, &saturation, &brightness, &is_lighting_on, &animation_index);

    set_hue(hue);
    set_saturation(saturation);
    set_brightness(brightness);
    set_visible(is_lighting_on);
    // set_animation(animation_index);
  }
  else if (line[0] == 'H')
  {
    sscanf(line, "H %d", &hue);
    set_hue(hue);
  }
  else if (line[0] == 'S')
  {
    sscanf(line, "S %d", &saturation);
    set_saturation(saturation);
  }
  else if (line[0] == 'B')
  {
    sscanf(line, "B %d", &brightness);
    set_brightness(brightness);
  }
  else if (line[0] == 'V')
  {
    sscanf(line, "V %d", &is_lighting_on);
    set_visible(is_lighting_on);
  }
  else if (line[0] == 'F')
  {
    sscanf(line, "F %d", &animation_index);
    fire_animation(animation_index);
  }
  else if (line[0] == 'A')
  {
    // Start continuous animation
  }

  // if (sscanf(line, "L %d %d %d %d %d", &hue, &saturation, &brightness, &is_lighting_on, &animation_index) != 5)
  // {
  //   return;
  // }

  // GLOBAL_CONFIG.lighting_hue = constrain(hue, 0, 255);
  // GLOBAL_CONFIG.saturation = constrain(saturation, 0, 255);
  // GLOBAL_CONFIG.brightness = constrain(brightness, 0, 255);
  // GLOBAL_CONFIG.is_lighting_on = is_lighting_on ? 1 : 0;
  // GLOBAL_CONFIG.animation_index = animation_index;

  // set_all_flags();
  // update_lighting();
}
