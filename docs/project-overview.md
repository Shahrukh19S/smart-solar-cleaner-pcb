# Project Overview — Automatic Solar Panel Cleaner

## System Summary

This project implements a **smart PCB** for an automatic solar panel cleaning robot. The hardware and firmware in this repository have been **manufactured, assembled, and tested** successfully.

## Main Components (from project spec)

1. **Ultrasonic (waterproof) sensors** — 3× (positioning / boundaries)
2. **Rain sensor** — for weather-aware operation
3. **WiFi** — ESP-01 (ESP8266) for Blynk app control
4. **Solar charge controller** — system power management
5. **Automatic battery charging and discharging**
6. **Motors** — 5 in total: 2 forward/back, 2 horizontal, 1 brush
7. **Blynk** — mobile app for scheduling and control
8. **Real-time clock (RTC)** — DS3231 for alarms and scheduling
9. **Target deployment** — e.g. 22 m × 6 m solar panel array; one dedicated panel for the cleaner
10. **SIM module** — (optional) for SMS alerts
11. **Arduino Mega** — main controller
12. **LCD** — 16×2 display
13. **Water level sensor** — tank level for wet cleaning
14. **Water pump** — wet cleaning; controlled via relay

## PCB Highlights

- **Input:** 7–12 V DC  
- **Relays:** 2 — one for water pump, one for brush motor  
- **Design:** Altium Designer (see `hardware/`)

## Documentation Map

- **Setup (ESP-01, bootloader, Blynk):** [setup-notes.md](setup-notes.md)
- **Schematic images:** `docs/schematics/`
- **Pinout:** `docs/pinout/PCB-Pinout.png`
- **Blynk-supported ESP firmwares:** `docs/Blynk_Supported_Firmwares.JPG`
