# Firmware — Automatic Solar Panel Cleaner

Arduino sketches for the **Auto-Solar-Cleaner** PCB (Arduino Mega + ESP-01, Blynk, RTC, motors, sensors).

## Main sketch: Evoo_Bot_V1.0

- **Path:** `Evoo_Bot_V1.0/`
- **Board:** Arduino Mega (ATmega2560)
- **Features:** Blynk control, RTC scheduling, dry/wet cleaning, 3× sonar positioning, rain and water-level checks, LCD, relay control for pump and brush.

### Dependencies

Install in your Arduino `libraries` folder (or Arduino IDE **Sketch → Include Library → Add .ZIP Library**):

- **Blynk** — from repo `libraries/blynk-library-master.zip` and `libraries/BlynkESP8266-master.zip` (or unzip and copy)
- **ESP8266 for Arduino** — [ESP8266 core](https://github.com/esp8266/Arduino) (for `ESP8266_Lib.h` / Blynk ESP8266)
- **DS3231 RTC** — use repo `libraries/ds3231FS/`
- **Robojax BTS7960** — use repo `libraries/Robojax-BTS7960-Motor-driver(Modified)/`

### Configuration (do not commit secrets)

Set your Blynk auth token and WiFi credentials before compiling:

1. **Option A:** Create a file `Config.h` in the sketch folder (add `#include "Config.h"` in the main `.ino`) with:
   - `char auth[] = "YOUR_BLYNK_AUTH";`
   - `char ssid[] = "YOUR_WIFI_SSID";`
   - `char pass[] = "YOUR_WIFI_PASSWORD";`
2. **Option B:** Edit the placeholders in `Evoo_Bot_V1.0.ino` and **do not commit** those changes (or use a separate private branch).

Add `Config.h` to `.gitignore` (already listed in the repo root `.gitignore`).

### Pin definitions

See `Globals.h`: rain sensor, water level, pump/brush (A1/A2), sonar trigger/echo, motor PWM and enable pins, LCD pins. Matches the PCB design in `hardware/`.

### ESP-01

- Use **Serial1** on the Mega to talk to the ESP-01.
- Flash the ESP-01 with Blynk-compatible AT firmware (e.g. V1.5.4 in `libraries/ESP-At_firmware_1.54/`). See `docs/setup-notes.md`.
- Set ESP-01 baud rate to **9600** (AT command) to match the sketch.

## Other sketches

- **Blynk_RTC** — Blynk + RTC utilities (used or referenced by main logic).
- **SolarCleaner(Original)-Copy** — earlier version of the cleaner logic.
- **NodeMCU** — standalone NodeMCU sketch (optional).

## Building

1. Open `Evoo_Bot_V1.0/Evoo_Bot_V1.0.ino` in the Arduino IDE.
2. Select board **Arduino Mega or Mega 2560** and the correct port.
3. Install all libraries above; add your Blynk/WiFi config.
4. Compile and upload to the Mega.
