# Sonata Lighting Display

An ESP32-based touchscreen display for vehicle interior / ambient lighting control. The display owns the **LVGL** interface, wireless config updates, and persistent configuration storage via SD card. LED hardware control lives in the sibling `Controller` PlatformIO project.

---

## Features

- **Backlight with automatic sleep**
  - Display backlight turns off after inactivity to reduce power draw.
- **Persistent configuration (SD card)**
  - Lighting settings are saved and restored on boot.
- **LVGL-based UI**
  - Touch-friendly interface with smooth rendering.
- **One-way lighting control**
  - Sends lighting state to the Arduino controller over serial TX on GPIO 5.

---

## Configuration

### Fast Lighting Updates

To update LEDs live while making adjustments in the UI:

```c
#define FAST_LIGHTING_UPDATES
```

### Pin Assignments

**Display serial TX:** Defined in `include/controller.h`

```c
#define CONTROLLER_TX_PIN 5
```

**LED strip pin:** Defined in `../Controller/include/lighting.h`

```c
#define LED_PIN_1 6
```

## Planned Features

### Lighting Patterns

- Time-Based Animations
- Reactive Effects

### Expanded Multi-LED Strip Support

- Independently control multiple LED strips.

*Note: This is currently limited by the Elecrow board this program is designed to run on.*

## Tech Stack

- MCU: ESP32
- UI: LVGL
- Storage: SD card
- LED Control: Addressable LED strips (WS2812-class) via sibling Arduino controller

## Notes

Ensure the ESP32 display and Arduino controller share a common ground. The serial link is one-way from ESP32 GPIO 5 to Arduino pin 5.

Designed with automotive environments in mind (noisy power rails).
