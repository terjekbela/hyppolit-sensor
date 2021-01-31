#include "config-secrets.h"

#include <ArduinoLowPower.h>
#include <ECCX08.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <utility/wifi_drv.h>

WiFiClient wifiClient;

#define LED_RED   26
#define LED_GREEN 25
#define LED_BLUE  27

void setup() {
  Serial.begin(115200);
  
  pinMode(LED_BUILTIN, OUTPUT);
  WiFiDrv::pinMode(LED_GREEN, OUTPUT); //green
  WiFiDrv::pinMode(LED_RED,   OUTPUT); //red
  WiFiDrv::pinMode(LED_BLUE,  OUTPUT); //blue

  WiFiDrv::analogWrite(LED_GREEN, 16);
  WiFiDrv::analogWrite(LED_RED, 8);
  WiFiDrv::analogWrite(LED_BLUE, 32);
  delay(5000);
  WiFiDrv::analogWrite(LED_GREEN, 0);
  WiFiDrv::analogWrite(LED_RED, 0);
  WiFiDrv::analogWrite(LED_BLUE, 0);

  WiFi.end();
  ECCX08.begin();
  ECCX08.end();
}

void loop() {
  float batteryVoltage;
  int wifiStatus = WL_IDLE_STATUS;
  IPAddress serverIP(192,168,15,124);
  int serverPort = 1880;
  
  batteryVoltage = analogRead(ADC_BATTERY) * 3.3f / 1023.0f / 1.2f * (1.2f+0.33f);

  WiFiDrv::analogWrite(LED_GREEN,  8);
  wifiStatus = WiFi.begin(NET_CLIENT_SSID, NET_CLIENT_PASS);
  WiFiDrv::analogWrite(LED_GREEN, 0);
  if ( wifiStatus == WL_CONNECTED) {
    if (wifiClient.connect(serverIP, serverPort)) {
      WiFiDrv::analogWrite(LED_BLUE, 16);
      wifiClient.print("GET /test?battery=");
      wifiClient.print(batteryVoltage);
      wifiClient.println(" HTTP/1.0");
      wifiClient.println();
      delay(100);
      WiFiDrv::analogWrite(LED_BLUE, 0);
    } else {
      WiFiDrv::analogWrite(LED_RED,  4); delay(200); WiFiDrv::analogWrite(LED_RED, 0);
    }
  } else {
    WiFiDrv::analogWrite(LED_RED,  4); delay(200); WiFiDrv::analogWrite(LED_RED, 0);
  }
  WiFi.end();

  LowPower.sleep(60000);
  //delay(60000);
}
