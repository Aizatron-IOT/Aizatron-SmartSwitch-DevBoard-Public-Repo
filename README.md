# Aizatron SmartSwitch DevBoard

A compact and powerful ESP32-based smart switch development board built for makers, hobbyists, tinkerers, and DIY automation projects.

The SmartSwitch DevBoard combines relay control, GPIO expansion, USB programming, and wireless web-based control into a single easy-to-use platform.

Designed for home automation systems, IoT projects, robotics, lighting controllers, or custom smart devices, this board gives you a flexible starting point with minimal setup.

---

## Features

### Hardware Features

* ESP32 dual-core WiFi + Bluetooth microcontroller
* On-board optocoupler-driven relays
* Relay status indicator LEDs
* Built-in user LED
* Power status LED
* USB Type-C connector
* On-board CP2102 USB-to-Serial programmer
* GPIO breakout headers
* Power (5V, 3.3V, GND) breakout pins

---

## Perfect For

* Home automation
* Smart lighting
* Garage automation
* Relay control projects
* Sensor interfacing
* Robotics
* ESP32 learning
* IoT dashboards
* MQTT projects
* Web-controlled devices
* DIY electronics experiments

---

# Built-In Web Demo

The included demo firmware turns the board into a standalone WiFi access point with a real-time web dashboard.

No router required.

Simply:

1. Power the board
2. Connect to the WiFi network
3. Open the web dashboard
4. Control relays and GPIOs instantly

---

## Default WiFi Credentials

| Setting  | Value                       |
| -------- | --------------------------- |
| SSID     | `Aizatron_SmartSwitch_Demo` |
| Password | `SmartSwitch`               |

---

## Web Dashboard Features

* Real-time GPIO control
* Relay toggling
* Live WebSocket updates
* Mobile-friendly interface
* Dark mode and light mode
* GPIO grouping panels
* Visual ON/OFF indicators
* Automatic reconnect handling

---

# GPIO Overview

## Relay Outputs

The SmartSwitch DevBoard is available in multiple variants:

- 2 Channel
- 4 Channel
- 8 Channel

All board versions share the same GPIO layout and firmware compatibility.

The included demo firmware and web dashboard are designed around the full 8-channel pinout. On smaller board variants (such as the 2CH and 4CH models), the unused relay GPIOs are still broken out to header pins and remain fully usable for custom applications.

This means you can use those GPIOs for:

- Sensors
- LEDs
- Buttons
- UART devices
- I2C peripherals
- SPI peripherals
- Custom outputs
- General-purpose IO

### Relay / GPIO Pin Mapping

| Channel  | GPIO    | Available On |
|----------|----------|---------------|
| Relay 1  | GPIO 4   | 2CH / 4CH / 8CH |
| Relay 2  | GPIO 16  | 2CH / 4CH / 8CH |
| Relay 3  | GPIO 17  | 4CH / 8CH |
| Relay 4  | GPIO 23  | 4CH / 8CH |
| Relay 5  | GPIO 27  | 8CH |
| Relay 6  | GPIO 26  | 8CH |
| Relay 7  | GPIO 33  | 8CH |
| Relay 8  | GPIO 19  | 8CH |

> Example: On the 2CH board, GPIO 17, 23, 27, 26, 33, and 19 are still accessible through the breakout headers even though onboard relays are not populated.

## GPIO Breakout Headers

Additional GPIOs are available for:

* Sensors
* LEDs
* Buttons
* Displays
* I2C devices
* SPI devices
* UART communication
* Custom peripherals

> Note: The ESP module used is an ESP32. Most pins can be used for any function, e.g. I2C, UART, SPI, etc. However, only some pins are usable as analog sensing pins, and others are only capable of being input pins. Please refer to the official ESP32 datasheet for guidance.

---

# GPIO Usage Notes

Some ESP32 pins have special functions during boot or programming.

| GPIO | Notes |
|---|---|
| GPIO 0 | Boot mode pin |
| GPIO 1 | UART TX |
| GPIO 3 | UART RX |

These pins are still available for use, but care should be taken when connecting external hardware.

Improper use of boot-related pins may prevent the ESP32 from starting correctly.

---

# Software Stack

The demo firmware uses:

* Arduino Framework for ESP32
* WiFi Access Point mode
* Async-style WebSocket communication
* Embedded HTML/CSS/JavaScript UI
* Real-time browser updates

---

# Getting Started

## Requirements

* Arduino IDE or PlatformIO
* ESP32 Board Package
* USB Type-C cable

---

## Install ESP32 Board Package

In Arduino IDE:

1. Open Preferences
2. Add the ESP32 board manager URL:

```text
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

3. Open Board Manager
4. Install:

```text
esp32 by Espressif Systems
```

---

## Required Libraries

Install these libraries:

* WiFi
* WebServer
* WebSocketsServer

---

# Uploading Firmware

1. Connect the SmartSwitch DevBoard via USB-C
2. Select the correct COM port
3. Select the "ESP32 Dev Module" board type
4. Upload the firmware

---

# Connecting To The Dashboard

When powered on, the SmartSwitch DevBoard automatically creates its own WiFi Access Point (AP). This allows you to connect directly to the board without needing an internet connection or external router.

1. Connect to:

```text
Aizatron_SmartSwitch_Demo
```

2. Open your browser and go to:

```text
192.168.4.1
```

3. Use the toggle buttons on the web dashboard to control the SmartSwitch.

---

# Example Applications

* Smart room controller
* WiFi relay board
* Home Assistant integration
* Wireless GPIO controller
* LED automation
* IoT sensor hub
* Smart workshop tools
* Remote power switching
* Educational ESP32 platform

---

# Safety Notes

* Relays switch external loads — always use proper electrical safety precautions
* Do not exceed relay voltage/current ratings
* Use isolation and protection circuits where necessary
* Intended for development and prototyping use

---

# Custom Firmware Development

The SmartSwitch DevBoard is designed to be more than just a relay controller — it is a flexible ESP32 development platform built for custom firmware and DIY projects.

You can:

- Modify the included demo firmware
- Build your own web dashboard
- Create custom automation systems
- Integrate with MQTT or Home Assistant
- Develop standalone ESP32 applications
- Use the board as a general-purpose ESP32 dev platform

The included demo project is intended as a starting point for experimentation and development.

---

## Easy USB Programming

The board includes an onboard CP2102 USB-to-Serial programmer, allowing firmware uploads directly over USB-C.

No external programmer is required.

Simply:

1. Connect a USB-C cable
2. Select the correct COM port
3. Upload your firmware

This makes development fast and beginner-friendly while still being powerful enough for advanced projects.

---

## GPIO Access

All relay control pins and additional ESP32 GPIOs are broken out to headers for easy access.

Depending on the board variant:

- Some GPIOs may drive onboard relays
- Additional relay GPIOs may still be exposed even if relays are not populated

Refer to the GPIO mapping section of this README when developing custom firmware.

---

## Compatible Development Environments

The SmartSwitch DevBoard can be programmed using:

- Arduino IDE
- PlatformIO
- ESP-IDF
- MicroPython

---

## Ideas For Custom Projects

- Smart home controllers
- Sensor monitoring systems
- Web dashboards
- MQTT automation nodes
- Home Assistant devices
- ESP-NOW networks
- Robotics controllers
- LED lighting systems
- Industrial monitoring
- Remote relay controllers
- IoT gateways

---

# Aizatron SmartSwitch DevBoard

Built for makers. Designed for experimentation. Powered by ESP32.
