/***********************************************************************
 *
 * FILE NAME: m5stack_sensor_display.ino
 *
 * PURPOSE: This program reads data from various sensors (SPS30, SHT3X, QMP6988) 
 * and displays the information on an M5Stack screen. It also logs the 
 * sensor data to an SD card.
 *
 * DEVELOPMENT HISTORY:
 *
 * Date                 Name                 Change ID           Release Description
 * --------       ------------------          -------        ---------------------------
 * 20-03-2025       Diego Martínez                                 Initial commit
 * 19-08-2025       Antonio Gordillo                               Use I2C to read SPS30
 *
 ***********************************************************************/

/* ---------------------------- Includes ---------------------------- */
#include <M5Unified.h>
//#include <SD.h>
//#include <sps30.h>

//#include "DHT12.h"

//#include "sensirion_uart.h"

#include "breathe_WiFiUtils.h"
#include "breathe_SensorLogger.h"
#include "breathe_display.h"  

/* ---------------------------- Macros and Defines ---------------------------- */
#define PASCAL_TO_ATM 0.00000986923  // Pascal to Atmospheres conversion factor

// Pin definitions for SD card using SPI protocol
#define SD_SPI_CS_PIN   4
#define SD_SPI_SCK_PIN  18
#define SD_SPI_MISO_PIN 19
#define SD_SPI_MOSI_PIN 23

// Pin definitions for ENVIII sensors
char ENV_SDA = 17;
char ENV_SCL = 16;

/* ---------------------------- Global Variables ---------------------------- */
//DHT12 dht12;
SHT3X sht3x;
QMP6988 qmp;
File dataFile;

byte dmode = 0;  // Default display mode: 0 = info, 1 = temp & hum, 2 = PM

// Sensor readings (no longer needed in this scope)
float temp = 0;
float hum = 0;
float pm1 = 0;
float pm25 = 0;
float pm4 = 0;
float pm10 = 0;
float avp = 0;
float envTemp = 0;
float envHum = 0;
float envPressure = 0;
float envAltitude = 0;

unsigned long lastMillis = 0;             // Store last update time
unsigned long lastButtonPress = 0;        // For debouncing
const unsigned long debounceDelay = 200;  // Debounce delay in ms

uint32_t startTime = 0;  // Stores millis() at sync

const int timeIntervals[] = { 100, 500, 1000, 2000, 3000, 4000, 5000 };
const int numIntervals = sizeof(timeIntervals) / sizeof(timeIntervals[0]);
int currentIntervalIndex = 2;                            // Default index for 1000ms
int timeInterval = timeIntervals[currentIntervalIndex];  // Initialize timeInterval

String wifiSsid = "";
String wifiPassword = "";

/* ---------------------------- Function Prototypes ---------------------------- */

/***********************************************************************
 *
 * FUNCTION NAME: setup
 *
 * @brief Initializes sensors, SD card, and starts measurement.
 *
 ***********************************************************************/
void setup() {
  Serial.begin(115200);

  auto cfg = M5.config();
  cfg.internal_imu = true;
  M5.begin(cfg);
 
  
  // Initialize SD card
  SPI.begin(SD_SPI_SCK_PIN, SD_SPI_MISO_PIN, SD_SPI_MOSI_PIN, SD_SPI_CS_PIN);
  if (!SD.begin(SD_SPI_CS_PIN, SPI, 25000000)) {
  //if (!SD.begin()) {
    Serial.println("SD Card initialization failed!");
    while (1);  // Infinite loop if SD card initialization fails
  }
  else {
    Serial.println("\n SD card detected\n");
  }

  // Load WiFi credentials from config file
  if (loadConfig(wifiSsid, wifiPassword)) {
    Serial.println("Wi-Fi credentials loaded successfully!");
    Serial.print("SSID: ");
    Serial.println(wifiSsid);
  } else {
    Serial.println("Unable to load Wi-Fi credentials.");
    wifiSsid = "defaultSSID";
    wifiPassword = "defaultPASSWORD";
  }

  // Connect to WiFi and get time
  WiFi.begin(wifiSsid, wifiPassword);
  connectWiFiAndGetTime();


 
  // initiallizing I2C for ENVIII sensor
  //Wire.begin(0, ENV_SDA, ENV_SCL);
  Wire.begin();

  // Initialize I2C sensors
  sht3x.begin(&Wire, SHT3X_I2C_ADDR, ENV_SDA, ENV_SCL, 400000U);  // SHT3X initialization
  qmp.begin(&Wire, QMP6988_SLAVE_ADDRESS_L, ENV_SDA, ENV_SCL, 400000U);  // QMP6988 initialization



/// Commented in this version 
/*
  // Initialize SPS30 sensor, also using I2C
  //sensirion_i2c_init();

  while (sps30_probe() != 0) {
    Serial.println("Probe failed");
    delay(500);  // Retry if the probe fails
  }
  sps30_set_fan_auto_cleaning_interval(15 * 60);  // Set auto cleaning interval for SPS30
  sps30_start_measurement();  // Start the SPS30 measurement

*/
  // Create data.csv if it doesn't exist
  if (!SD.exists("/data.csv")) {
    dataFile = SD.open("/data.csv", FILE_WRITE);
    if (dataFile) {
      // Write the header for the CSV file
      dataFile.println("Timestamp,SPS30_PM1_0,SPS30_PM2_5,SPS30_PM4_0,SPS30_PM10_0,"
                       "SPS30_Avg_Particle_Size,SHT3X_Env_Temperature_C,"
                       "SHT3X_Env_Humidity_Percent,QMP6988_Env_Pressure_atm,"
                       "QMP6988_Env_Altitude_m");
      dataFile.close();
    } else {
      Serial.println("Error opening data.csv for writing.");
    }
  }

  // Optional: Print initialization success message
  Serial.println("Setup complete, sensors initialized, and data logging ready.");
}



/***********************************************************************
 *
 * FUNCTION NAME: loop
 *
 * @brief Reads sensor data and updates the display periodically.
 *
 ***********************************************************************/
void loop() {
  M5.update();
  unsigned long currentMillis = millis();

  // Button event handling (with debouncing)
  if (M5.BtnA.wasReleased()) {
    Serial.println("button A");
    dmode = 0;  // Switch to Info mode
  } else if (M5.BtnB.wasReleased()) {
    Serial.println("button B");
    dmode = 1;  // Switch to Config mode
  } else if (M5.BtnC.wasReleased()) {
    // Check if enough time has passed for debouncing
    if (currentMillis - lastButtonPress >= debounceDelay) {
      // Cycle through timeInterval values
      currentIntervalIndex = (currentIntervalIndex + 1) % numIntervals;
      timeInterval = timeIntervals[currentIntervalIndex];  // Update timeInterval

      // Refresh the screen if in the Cfg tab
      if (dmode == 1) {
        displayCfg();
      }

      // Update lastButtonPress time
      lastButtonPress = currentMillis;
    }
  }

  // Mode select and display based on dmode value
  if (dmode == 0) {
    displayInfo();
  } else if (dmode == 1) {
    displayCfg();
  }

  // Check if enough time has passed based on the timeInterval for sensor data update
 // Update sensor data only if the time interval has passed
     if (currentMillis - lastMillis >= timeInterval) {

      // Reading from DHT12
      //temp = dht12.readTemperature();
      //hum = dht12.readHumidity();
      

  // Reading ENVIII sensor (pressure, temperature and humidity)
    if (sht3x.update() && qmp.update()) {
      envTemp = sht3x.cTemp;
      envHum = sht3x.humidity;
      envPressure = qmp.pressure * PASCAL_TO_ATM;
      envAltitude = qmp.altitude;
      logSensorData();  // Log data with timestamp
    } else {
      Serial.println("ENVIII sensor data update failed.");
    }
    delay(100);

/// Commented in this version 
/*
    // Reading from Sensirion SPS30
    struct sps30_measurement measurement;
    uint16_t data_ready;
    int16_t ret;

    do {
      ret = sps30_read_data_ready(&data_ready);
      if (ret < 0) {
        Serial.print("Error reading data-ready flag: ");
        Serial.println(ret);
    } else if (!data_ready)
        Serial.print("No new SPS30 measurement available\n");
    else
      break;
      delay(100); // retry in 100ms 
    } while (1);

    if (sps30_read_measurement(&measurement) >= 0) {
      pm1 = measurement.mc_1p0;
      pm25 = measurement.mc_2p5;
      pm4 = measurement.mc_4p0;
      pm10 = measurement.mc_10p0;
      avp = measurement.typical_particle_size;
    } else {
      Serial.println("Read SPS30 measurement failed");
    }
*/

    logSensorData();  // This will read data from sensors and log it

    // Update the last time the data was acquired
    lastMillis = currentMillis;
  }
}
