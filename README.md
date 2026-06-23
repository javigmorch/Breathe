# Breathe

![Platform](https://img.shields.io/badge/platform-ESP32-blue)
![Framework](https://img.shields.io/badge/framework-Arduino-green)
![Status](https://img.shields.io/badge/status-in%20development-orange)
![License](https://img.shields.io/badge/license-MIT-lightgrey)

Breathe is an ESP32-based project built on the M5Stack platform to measure, record, and analyze environmental and air quality data. The system captures real-time measurements and stores them with accurate timestamps for later analysis.

This project also aims to compare the performance of low-cost sensors against official meteorological stations (such as AEMET) to evaluate their reliability under real-world conditions.

---

## Features

- Real-time environmental monitoring  
- SD card data logging with timestamps  
- NTP time synchronization at startup  
- Low-power approach (Wi-Fi only used at boot)  
- External configuration via file  

---

## Hardware

- **M5Stack ESP32** — Main controller  
- **ENV III** — Temperature, humidity, and gas sensor (CO2, VOCs)  
- **SPS30** — Particulate matter sensor (PM1.0, PM2.5, PM10)  
- **SD Card** — Data storage  

Communication is handled via **I2C**.

---

## Project Structure
/Breathe
├── Breathe.ino
├── config.cfg
└── /data (SD card logs)

text

---

## How It Works

1. Connects to Wi-Fi at startup  
2. Retrieves time from an NTP server  
3. Disconnects Wi-Fi to save power  
4. Reads data from sensors  
5. Displays data on screen  
6. Stores measurements on SD card  

---

## Configuration

Create a `config.cfg` file in the SD card:
[WIFI_CREDENTIALS]
SSID=YOUR-WIFI-SSID
PASSWORD=YOUR-WIFI-PASSWORD

text

---

## Installation

### 1. Hardware Setup

- Connect ENV III and SPS30 via I2C  
- Insert SD card into M5Stack  

### 2. Software Setup

1. Install Arduino IDE  
2. Add ESP32 board support  
3. Install required libraries:
   - M5Stack
   - ENV III
   - SPS30  
4. Open `Breathe.ino`  
5. Configure `config.cfg`  
6. Upload the code  

---

## Use Cases

- Indoor and outdoor air quality monitoring  
- Educational and research projects  
- Sensor validation against official stations  
- Environmental data collection  

---

## Roadmap

- Cloud integration (IoT platforms)  
- Web dashboard for visualization  
- Sensor calibration system  
- API integration (weather services)  

---

## License

This project is licensed under the MIT License.

---

## Author

Javier Garcia Moriche  
Braunschweig, Germany
