////////////////////////////////////////////////////////////////////////////////
// HYPPOLITsensor                                   http://github.com/terjekbela
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// Libraries used
////////////////////////////////////////////////////////////////////////////////

#include "config.h"
#include "config-secrets.h"
#include "circuit.h"

#include <ArduinoLowPower.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <ECCX08.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_MCP9808.h>
#include <Adafruit_BME280.h>


////////////////////////////////////////////////////////////////////////////////
// Runtime variables
////////////////////////////////////////////////////////////////////////////////

WiFiClient wifiClient;
#if defined(NET_CLIENT_IP)
IPAddress ipIP (NET_CLIENT_IP);
IPAddress ipDNS(NET_CLIENT_DNS);
IPAddress ipGW (NET_CLIENT_GW);
IPAddress ipNET(NET_CLIENT_NET);
#endif



////////////////////////////////////////////////////////////////////////////////
// Setup / main section
////////////////////////////////////////////////////////////////////////////////

// setup routine
void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN,  OUTPUT);
  pinMode(SENSOR_POWER, OUTPUT);
  digitalWrite(SENSOR_POWER, LOW);

  WiFiDrv::pinMode(LED_RED,   OUTPUT); //red
  WiFiDrv::pinMode(LED_GREEN, OUTPUT); //green
  WiFiDrv::pinMode(LED_BLUE,  OUTPUT); //blue
  led(2,4,8,5000);

  WiFi.end();
  ECCX08.begin();
  ECCX08.end();
}

// main loop
void loop() {
  int   wifiStatus       = WL_IDLE_STATUS;
  pinMode(SENSOR_POWER, OUTPUT);
  digitalWrite(SENSOR_POWER, HIGH);
  delay(100);
  
  float batteryValue     = sensorValueBattery();
  float temperatureValue = sensorValueMCP9808();
  if(isnan(temperatureValue)) {
    temperatureValue = sensorValueBME280Temperature();
  }
  float humidityValue    = sensorValueBME280Humidity();
  float pressureValue    = sensorValueBME280Pressure();
  digitalWrite(SENSOR_POWER, LOW);
  pinMode(SENSOR_POWER, INPUT);
  led(0,0,8);
  Serial.print("Connecting: ");
  Serial.println(millis());
#if defined(NET_CLIENT_IP)
  WiFi.config(ipIP, ipDNS, ipGW, ipNET);
#endif
  wifiStatus = WiFi.begin(NET_CLIENT_SSID, NET_CLIENT_PASS);
  Serial.print("Connected: ");
  Serial.println(millis());
  if ( wifiStatus == WL_CONNECTED) {
    if (wifiClient.connect(IPAddress(NET_SERVER_IP), NET_SERVER_PORT)) {
      led(0,4,0);
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
      wifiClient.println(" HTTP/1.0");
      wifiClient.println();
      delay(200);
      led(0,0,0);
    } else {
      led(2,0,0,200);
    }
  } else {
    led(2,0,0,200);
  }
  WiFi.end();

  digitalWrite(SENSOR_SCL, HIGH);
  digitalWrite(SENSOR_SDA, HIGH);
  LowPower.sleep(56000);
  //delay(60000);
}

////////////////////////////////////////////////////////////////////////////////
// LED library
////////////////////////////////////////////////////////////////////////////////

void led(byte red, byte green, byte blue) {
  WiFiDrv::analogWrite(LED_RED,   red);
  WiFiDrv::analogWrite(LED_GREEN, green);
  WiFiDrv::analogWrite(LED_BLUE,  blue);
}

void led(byte red, byte green, byte blue, int wait) {
  led(red, green, blue);
  delay(wait);
  led(0,0,0);
}
