# Libraries & ESP-01 Firmware

Arduino libraries and ESP-01 AT firmware used by the **Auto-Solar-Cleaner** firmware.

## Arduino libraries

- **blynk-library-master.zip** — Blynk (unzip and install via Sketch → Include Library → Add .ZIP Library).
- **BlynkESP8266-master.zip** — Blynk ESP8266 shield support.
- **ds3231FS/** — DS3231 RTC library (copy the folder into your Arduino `libraries` directory).
- **Robojax-BTS7960-Motor-driver(Modified)/** — BTS7960 motor driver (modified for this project; copy into Arduino `libraries`).

You also need the **ESP8266 core for Arduino** (from [esp8266/Arduino](https://github.com/esp8266/Arduino)) for the Blynk ESP8266 library.

## ESP-01 AT firmware (Blynk-compatible)

- **ESP-At_firmware_1.54/** — AT firmware V1.5.4 for Ai-Thinker ESP-01. Flash at **0x00000**.
- Newer AT versions may not work with Blynk; see `docs/setup-notes.md` and `docs/Blynk_Supported_Firmwares.JPG`.

## Installation

1. Unzip the `.zip` libraries and place them (or their contents) in your Arduino `libraries` folder.
2. Copy `ds3231FS` and `Robojax-BTS7960-Motor-driver(Modified)` into the same `libraries` folder.
3. Install the ESP8266 core via Boards Manager if you have not already.
4. Flash the ESP-01 with a `.bin` from `ESP-At_firmware_1.54/` using a tool such as ESP flash download utility; set baud to 9600 (e.g. AT command) for use with the Mega sketch.
