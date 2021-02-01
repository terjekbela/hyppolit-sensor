#include "config.h"
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
  WiFiDrv::pinMode(LED_RED,   OUTPUT); //red
  WiFiDrv::pinMode(LED_GREEN, OUTPUT); //green
  WiFiDrv::pinMode(LED_BLUE,  OUTPUT); //blue
  led(4,8,16);
  delay(5000);
  led(0,0,0);

  WiFi.end();
  ECCX08.begin();
  ECCX08.end();
}

void loop() {
  int   wifiStatus     = WL_IDLE_STATUS;
  float batteryVoltage = analogRead(ADC_BATTERY) * 3.3f / 1023.0f / 1.2f * (1.2f+0.33f);

  led(0,8,0);
  wifiStatus = WiFi.begin(NET_CLIENT_SSID, NET_CLIENT_PASS);
  led(0,0,0);
  if ( wifiStatus == WL_CONNECTED) {
    if (wifiClient.connect(IPAddress(NET_SERVER_IP), NET_SERVER_PORT)) {
      led(0,0,16);
      wifiClient.print("GET /test?battery=");
      wifiClient.print(batteryVoltage);
      wifiClient.println(" HTTP/1.0");
      wifiClient.println();
      delay(100);
      led(0,0,0);
    } else {
      led(4,0,0);
      delay(200);
      led(0,0,0);
    }
  } else {
    led(4,0,0); delay(200); led(0,0,0);
  }
  WiFi.end();

  LowPower.sleep(60000);
  //delay(60000);
}

void led(byte red, byte green, byte blue) {
  WiFiDrv::analogWrite(LED_RED,   red);
  WiFiDrv::analogWrite(LED_GREEN, green);
  WiFiDrv::analogWrite(LED_BLUE,  blue);
}
