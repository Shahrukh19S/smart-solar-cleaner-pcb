# Setup Notes — ESP-01, Bootloader, Blynk

## ESP-01 AT firmware and Blynk

- **Important:** Newer AT firmware versions (e.g. latest from Espressif) may **not** work with Blynk.
- **Tested working:** Firmware **V1.5.4** (included in this repo under `libraries/ESP-At_firmware_1.54/`).
- **Reference:** List of Blynk-supported ESP firmwares is in the project image `Blynk_Supported_Firmwares.JPG` and at:  
  [Blynk: ESP8266 with AT firmware](http://help.blynk.cc/en/articles/605485-esp8266-with-at-firmware).
- **Firmware source (external):**  
  [Edragon esp_firmware — AI-THINKER At_firmware_bin1.54](https://github.com/Edragon/esp_firmware/tree/master/Firmware/AT-other/AI-THINKER/At_firmware_bin1.54).
- **Flashing guide (YouTube):** Nathan Church — [How to flash ESP-01 firmware](https://www.youtube.com/watch?v=YLKEZtLhfZo).
- **Baud rate:** After flashing, set ESP-01 baud rate to **9600** (e.g. via AT commands); the Mega sketch uses 9600 for `Serial1`.

## Arduino Mega bootloader

- **Guide:** Nick Gammon — [Arduino Mega Bootloader and upload](http://www.gammon.com.au/bootloader).
- **Sketches / reference:** [nickgammon/arduino_sketches](https://github.com/nickgammon/arduino_sketches).

## Libraries used in this project

- **Robojax BTS7960** — slightly modified in this repo; see `libraries/Robojax-BTS7960-Motor-driver(Modified)/`.
- **Blynk** — use the Blynk libraries included (or linked) in `libraries/` (e.g. `blynk-library-master.zip`, `BlynkESP8266-master.zip`).

## ESP-01 firmware files (V1.5.4)

Located in `libraries/ESP-At_firmware_1.54/`:

- `AiThinker_ESP8266_DIO_8M_8M_20160615_V1.5.4.bin` — 8 Mbit flash
- `AiThinker_ESP8266_DIO_32M_32M_20160615_V1.5.4.bin` — 32 Mbit flash

Flash at address **0x00000**. Use a tool such as ESP_DOWNLOAD_TOOL_V2.4; if unsure, use the “DoNotChgBin” default configuration.
