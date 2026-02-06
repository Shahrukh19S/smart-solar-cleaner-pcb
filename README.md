# Automatic Solar Panel Cleaner — PCB & Firmware

A smart PCB for an **automatic solar panel cleaning system**, designed in Altium Designer. The board has been **manufactured, assembled, and tested** in the field.

## Overview

The system cleans solar panels using a brush and water pump, with motion provided by four motors (forward/back, horizontal, brush). It uses three ultrasonic (sonar) sensors for positioning, a rain sensor, WiFi (ESP-01) and Blynk for control, a 16×2 LCD, RTC, and an Arduino Mega as the main controller. Power input is 7–12 V DC; two relays drive the water pump and brush motor.

## 📹 Demo

### SMART SOLAR CLEANER PCB PINOUT:

[3D View of PCB Board on Altium Designer](resources/2021-06-28%20(1).png)

## Features

| Category | Details |
|----------|---------|
| **Controller** | Arduino Mega (ATmega2560) |
| **Connectivity** | ESP-01 (ESP8266) — Blynk app control |
| **Display** | 16×2 LCD |
| **Sensors** | 3× ultrasonic (sonar), rain sensor |
| **Actuators** | 4× motors (2 F/B, 2 horizontal, 1 brush), water pump |
| **Power** | 7–12 V DC input; 2 relays (pump, brush) |
| **Other** | DS3231 RTC, CH340 USB-serial |

## Repository Structure

```
├── README.md                 # This file
├── .gitignore
├── docs/                     # Documentation, schematics, pinout, setup notes
├── hardware/                 # Altium Designer PCB project
│   └── Auto-Solar-Cleaner/   # Source (SchDoc, PcbDoc, PrjPcb) + Gerbers, BOM, DRC
├── firmware/                 # Arduino sketches for this PCB
│   └── Evoo_Bot_V1.0/        # Main firmware (Blynk, RTC, cleaning logic)
├── libraries/                # Arduino/ESP libraries and ESP-01 firmware
└── resources/                # Manuals, reference images
```

## Hardware (PCB)

- **Design tool:** Altium Designer  
- **Location:** `hardware/Auto-Solar-Cleaner/`
- **Source files:** `*.SchDoc`, `*.PcbDoc`, `*.PrjPcb`
- **Manufacturing outputs:** Gerbers, drill files, and assembly data in `Project Outputs for Auto-Solar-Cleaner/`
- **BOM:** `Parts(BOM).xlsx` in the same outputs folder
- **Design rule check:** `Design Rule Check - SOLAR_CLEANER_PCB.html` / `.drc`

Schematic sheets include: ATmega2560, CH340, ESP-01, DS3231, and sensors.

## Firmware

- **Main sketch:** `firmware/Evoo_Bot_V1.0/` — Blynk + RTC + cleaning sequences (dry/wet), sonar-based positioning, rain and water-level checks.
- **Target:** Arduino Mega; serial to ESP-01 for Blynk.
- **Dependencies:** See `firmware/Evoo_Bot_V1.0/README.md` and `libraries/` (Blynk, ESP8266, DS3231, Robojax BTS7960).

Before building, set your Blynk auth token and WiFi credentials. Use `firmware/Evoo_Bot_V1.0/Config.example.h` as a template (copy to `Config.h` and do not commit `Config.h`).

## Libraries

In `libraries/`:

- **Blynk** — `blynk-library-master.zip`, `BlynkESP8266-master.zip`
- **DS3231** — `ds3231FS/` (RTC)
- **Robojax BTS7960** — `Robojax-BTS7960-Motor-driver(Modified)/` (motor driver)
- **ESP-01 AT firmware** — `ESP-At_firmware_1.54/` (Blynk-compatible AT firmware; see `docs/setup-notes.md`)

Install the Arduino libraries and flash the ESP-01 with the provided AT firmware as per the setup notes.

## Documentation

- **Setup and flashing:** `docs/setup-notes.md` (ESP-01 firmware, bootloader, Blynk)
- **Schematics / reference:** `docs/schematics/`, `docs/pinout/`
- **Project overview:** `docs/project-overview.md`

## Bill of Materials (BOM)

- **Location:** `hardware/Auto-Solar-Cleaner/Project Outputs for Auto-Solar-Cleaner/Parts(BOM).xlsx`
- Pick-and-place (centroid) file: `Pick n Place (Centroid).csv` in the same folder.

## Manufacturing & Testing

This PCB has been **manufactured, assembled (hand-soldered), and tested** with the firmware in this repository. Behavior has been verified in real use.

## License

MIT License — see [LICENSE](LICENSE).


## Quick Start

1. Clone the repo and open `hardware/Auto-Solar-Cleaner/Auto-Solar-Cleaner.PrjPcb` in Altium for design review or manufacturing.
2. Install Arduino libraries from `libraries/` and configure Blynk/WiFi (see firmware README and `docs/setup-notes.md`).
3. Flash ESP-01 with `libraries/ESP-At_firmware_1.54/` AT firmware, then build and upload `firmware/Evoo_Bot_V1.0/` to the Arduino Mega.
