# Hardware — Auto-Solar-Cleaner PCB

Altium Designer project for the **Automatic Solar Panel Cleaner** PCB.

## Contents

- **Source**
  - `Auto-Solar-Cleaner.PrjPcb` — project file (open this in Altium)
  - `*.SchDoc` — schematic sheets (ATmega2560, CH340, ESP-01, DS3231, Sensors)
  - `SOLAR_CLEANER_PCB.PcbDoc` — PCB layout

- **Manufacturing outputs** (`Project Outputs for Auto-Solar-Cleaner/`)
  - **Gerbers** — `.GTL`, `.GBL`, `.GTS`, `.GBS`, `.GTO`, `.GBO`, `.GPT`, `.GPB`, `.GKO`, `.GM*`
  - **Drill** — `.TXT` (round/rect/slot holes), `.LDP`, `.DRR`
  - **Assembly** — `Pick n Place (Centroid).csv`
  - **BOM** — `Parts(BOM).xlsx`
  - **DRC** — `Design Rule Check - SOLAR_CLEANER_PCB.html` / `.drc`
  - **Archive** — `Final PCB (V1).zip`

## Main parts (summary)

- Arduino Mega (ATmega2560), CH340 USB-serial
- ESP-01 (ESP8266), DS3231 RTC
- 16×2 LCD, 3× ultrasonic sensors, rain sensor
- 4× motors (BTS7960 drivers), water pump, brush motor
- 2× relays (pump, brush); 7–12 V DC input

## Viewing without Altium

- Open the **Design Rule Check** report (`.html`) for a net/spec overview.
- View **schematic images** in the repo under `docs/schematics/` (e.g. rain sensor).
- **Pinout:** `docs/pinout/PCB-Pinout.png`
