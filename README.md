# Breathe

Breathe is an environmental monitoring project based on an ESP32 and the M5Stack ecosystem. The system is designed to acquire, timestamp, display, and store atmospheric data in a structured way, making it suitable for long-term environmental studies, field measurements, and sensor validation experiments.

The project focuses on collecting meteorological and air-quality-related variables using low-cost embedded hardware, and on comparing those measurements with official reference data from AEMET in order to evaluate sensor reliability under real-world conditions.

## Overview

Breathe combines embedded sensing, local data logging, and time synchronization to build a compact autonomous measurement node. At startup, the device connects to Wi-Fi only to retrieve the current date and time through NTP, then disconnects to reduce power consumption and network dependency during operation.

Once synchronized, the system periodically reads data from the connected sensors, displays the measurements on the M5Stack screen, and stores them on an SD card with accurate timestamps. This allows later processing, visualization, and comparison against official meteorological datasets.

## Objectives

- Build a compact environmental monitoring node based on ESP32 and M5Stack.
- Record atmospheric variables with reliable timestamps.
- Store measurements locally for offline analysis.
- Compare low-cost sensor readings with official AEMET data.
- Evaluate sensor stability, consistency, and practical reliability.

## Measured Variables

The current setup is intended to monitor the following environmental parameters:

- Temperature.
- Relative humidity.
- Atmospheric pressure.
- Particulate matter concentrations such as PM1.0, PM2.5, and PM10.
- Additional air-quality-related indicators depending on the sensor configuration.

## Hardware

The project uses the following components:

- **M5Stack ESP32**: Main processing unit and display interface.
- **ENV III**: Environmental sensor module that integrates:
  - **SHT30** for temperature and humidity.
  - **QMP6988** for atmospheric pressure.
- **SPS30**: Particulate matter sensor for fine dust measurement.
- **SD card**: Local storage for logs and configuration files.

The ENV III module communicates over **I2C** and provides temperature, humidity, and pressure readings. The SPS30 sensor adds particle concentration measurements, enabling a broader characterization of air quality. The design is based on a modular architecture that separates sensing, time synchronization, display, and storage responsibilities.

## Sensor Details

### ENV III

The ENV III sensor integrates the SHT30 and QMP6988 chips. The SHT30 is used for temperature and humidity measurement, while the QMP6988 is an absolute pressure sensor optimized for barometric applications. This makes the module suitable for environmental monitoring and weather-related experiments.

### SPS30

The SPS30 is a particulate matter sensor designed to measure airborne particle concentration. It supports multiple PM channels, including PM1.0, PM2.5, PM4, and PM10, and is widely used for air-quality monitoring applications thanks to its stable optical measurement principle and digital interface.

## System Workflow

1. The device powers on and initializes the M5Stack platform.
2. It connects to Wi-Fi only at startup.
3. The current date and time are obtained from an NTP server.
4. Wi-Fi is disabled after synchronization to reduce power consumption.
5. The system starts sampling environmental data from the connected sensors.
6. Measurements are shown on the M5Stack screen.
7. Each record is stored on the SD card together with its timestamp.
8. The logged data can later be analyzed and compared with AEMET reference values.

## Data Logging

Each measurement is saved locally to ensure that the system can operate autonomously without requiring a continuous network connection. The use of timestamps makes the dataset suitable for time-series analysis, correlation studies, and validation against external sources.

This approach is especially useful when the goal is to study:
- Daily and seasonal environmental trends.
- Sensor drift over time.
- Differences between local measurements and official stations.
- The influence of location, shielding, and installation conditions on sensor performance.

## Configuration

The system reads Wi-Fi credentials from a `config.cfg` file stored on the SD card. This keeps network settings separate from the firmware and makes deployment easier.

Example:

```ini
[WIFI_CREDENTIALS]
SSID=YOUR-WIFI-SSID
PASSWORD=YOUR-WIFI-PASSWORD
```

## Installation

### Hardware Setup

- Connect the ENV III and SPS30 sensors to the M5Stack board.
- Insert an SD card into the device.
- Verify the I2C wiring and power connections.
- Ensure the sensors are mounted in a way that minimizes airflow obstruction and measurement bias.

### Software Setup

1. Install the Arduino IDE.
2. Add ESP32 board support.
3. Install the required libraries for:
   - M5Stack.
   - ENV III.
   - SPS30.
4. Open `Breathe.ino`.
5. Create and edit the `config.cfg` file on the SD card.
6. Upload the sketch to the M5Stack device.

## AEMET Comparison

One of the main goals of this project is to compare the values obtained from the embedded sensor system with official meteorological data provided by AEMET. This comparison is useful to assess how close low-cost or embedded sensors are to reference-grade observations.

This kind of validation is important because low-cost sensors can be affected by:
- Calibration differences.
- Environmental exposure.
- Drift over time.
- Sensor placement and enclosure effects.
- Local microclimate variations.

By combining local logging with official reference data, Breathe can be used as a practical platform for sensor benchmarking and environmental analysis.

## Applications

- Indoor environmental monitoring.
- Outdoor atmospheric observation.
- Air-quality studies.
- Sensor calibration and validation.
- Educational and research projects.
- Comparison against official meteorological datasets.

## Future Improvements

- Cloud synchronization and remote dashboards.
- Export to CSV or JSON formats.
- Web-based visualization tools.
- Automatic calibration support.
- Integration with AEMET or other meteorological APIs.
- Better power management for autonomous deployments.


## Author

- Diego Martinez Megias.
- Javier Garcia Moriche.
