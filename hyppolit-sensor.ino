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



////////////////////////////////////////////////////////////////////////////////
// Setup / main section
////////////////////////////////////////////////////////////////////////////////

// setup routine
void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN,  OUTPUT);
  pinMode(SENSOR_POWER, OUTPUT);
  digitalWrite(SENSOR_POWER, LOW);

  wifiMulti.addAP(NET_CLIENT_SSID1, NET_CLIENT_PASS1);
  if (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("Unable to connect, rebooting...");
    delay(5000);
    rp2040.reboot();
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// main loop
void loop() {
  const char*    host = NET_SERVER_NAME;
  const uint16_t port = NET_SERVER_PORT;
  
  float batteryValue     = 50;
  float temperatureValue = sensorValueMCP9808();                       // getting sensor values
  float humidityValue    = sensorValueBME280Humidity();
  float pressureValue    = sensorValueBME280Pressure();

  Serial.println("Server connecting");
  if (wifiClient.connect(host, port)) {
    Serial.println("Server connected");
    wifiClient.print("GET /sensor/1?battery=");
    wifiClient.print(batteryValue);
    if(!isnan(temperatureValue)) {
      wifiClient.print("&temperature=");
      wifiClient.print(temperatureValue);
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

  Serial.println("Delaying 10s...");
  delay(10000);
}
