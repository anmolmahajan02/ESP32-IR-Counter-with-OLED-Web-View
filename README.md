# ESP32 IR Object Counter with OLED Display and Web Interface

A real-time object counter using an IR sensor, displayed on an OLED screen and accessible through a live web interface hosted by the ESP32 in Access Point (AP) mode. Includes a touch-based reset feature using GPIO 4.

## 📌 Features

- IR sensor-based object counting (GPIO 23)
- Live count display on a 128x64 OLED screen (I2C)
- Web interface to view the counter remotely via WiFi
- ESP32 runs in Access Point mode (no external WiFi required)
- Touch-based reset pin using GPIO 4
- Responsive web UI with real-time counter updates

---

## 🛠️ Hardware Used

| Component          | Description                            |
|-------------------|----------------------------------------|
| ESP32             | Main microcontroller                   |
| IR Sensor Module  | Detects object interruptions (GPIO 23) |
| OLED Display      | 128x64, I2C interface (SDA: 21, SCL: 22)|
| Touch Sensor Pin  | GPIO 4 (onboard touch) for reset       |
| Jumper Wires      | For connections                        |

---

## 🔌 Wiring Guide

| ESP32 Pin | Component Function      |
|-----------|-------------------------|
| 3V3       | VCC of IR sensor + OLED |
| GND       | GND of IR sensor + OLED |
| GPIO 21   | SDA (OLED)              |
| GPIO 22   | SCL (OLED)              |
| GPIO 23   | IR sensor output        |
| GPIO 4    | Touch Reset input       |

---

## 🌐 Web Interface

Once powered on, the ESP32 creates a WiFi access point:

- **SSID**: `MyESP`
- **Password**: `9103157011`
- **Connect via browser**: `http://192.168.4.1`

The web page shows the live count and auto-refreshes every 100ms.

---

## 🧠 How It Works

1. IR sensor detects object passing (beam breaks → LOW signal).
2. Counter increases and updates:
   - OLED screen
   - Web interface
3. Touch GPIO 4 to reset the count to `0`.

---

## 📷 Demo

![image](https://github.com/user-attachments/assets/dd1af003-ef0d-4e68-ba74-8fa1d621ae11)



