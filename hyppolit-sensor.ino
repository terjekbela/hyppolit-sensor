////////////////////////////////////////////////////////////////////////////////
// HYPPOLITsensor                                   http://github.com/terjekbela
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// Libraries used
////////////////////////////////////////////////////////////////////////////////

#include "config.h"
#include "config-secrets.h"
#include "circuit.h"

#include <Wire.h>
#include <WiFi.h>

#include <Adafruit_MCP9808.h>
#include <Adafruit_BME280.h>



////////////////////////////////////////////////////////////////////////////////
// Runtime variables
////////////////////////////////////////////////////////////////////////////////

WiFiMulti wifiMulti;
WiFiClient wifiClient;

float batteryValue;
float temperatureValue;
float temperatureCore;
float humidityValue;
float pressureValue;



////////////////////////////////////////////////////////////////////////////////
// Setup / main section
////////////////////////////////////////////////////////////////////////////////

// setup routine
void setup() {
  setupSerial();
  setupIO();
  setupNetwork();
}

// main loop
void loop() {
  loopReadSensors();
  loopSendSensors();
  loopLowPower();
}



////////////////////////////////////////////////////////////////////////////////
// Routines
////////////////////////////////////////////////////////////////////////////////

// start serial
void setupSerial() {
  Serial.begin(115200);
}

// set pin io modes
void setupIO() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(DONE_RP2040, OUTPUT);
  digitalWrite(DONE_RP2040, LOW);
}

// connect to wireless network
void setupNetwork() {
  wifiMulti.addAP(NET_CLIENT_SSID1, NET_CLIENT_PASS1);
  if (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("Unable to connect, rebooting...");
    delay(5000);
    rp2040.reboot();
  }
  Serial.print("WiFi connected, ip: ");
  Serial.println(WiFi.localIP());
}

// reading and storing sensor values
void loopReadSensors(){
  batteryValue     = 50;
  temperatureCore  = sensorRP2040CoreTemp();
  temperatureValue = sensorMCP9808();
  humidityValue    = sensorBME280Humidity();
  pressureValue    = sensorBME280Pressure();
}

// sending sensor values to configured server
void loopSendSensors() {
  const char*    host = NET_SERVER_NAME;
  const uint16_t port = NET_SERVER_PORT;
  Serial.println("Connecting to server");
  if (wifiClient.connect(host, port)) {
    Serial.println("Server connected");
    wifiClient.print("GET /sensor/" NET_SENSOR_ID "?battery=");
    wifiClient.print(batteryValue);
    if(!isnan(temperatureValue)) {
      wifiClient.print("&temperature=");
      wifiClient.print(temperatureValue);
    }
    if(!isnan(temperatureCore)) {
      wifiClient.print("&core=");
      wifiClient.print(temperatureCore);
    }
    if(!isnan(humidityValue)) {
      wifiClient.print("&humidity=");
      wifiClient.print(humidityValue);
    }
    if(!isnan(pressureValue)) {
      wifiClient.print("&pressure=");
      wifiClient.print(pressureValue);
    }
    wifiClient.println(" HTTP/1.1");
    wifiClient.print("host: ");
    wifiClient.println(host);
    wifiClient.println();
    Serial.println("Request sent");
  } else {
    Serial.println("Server failed to connect");
  }
  Serial.println();
}

void loopLowPower() {
  digitalWrite(DONE_RP2040, HIGH);
  while (1) {}
}
