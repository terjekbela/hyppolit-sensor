#include <WiFiNINA.h>
#include "ArduinoLowPower.h"
#include <utility/wifi_drv.h>

void setup() {
  Serial.begin(115200);
  
  pinMode(LED_BUILTIN, OUTPUT);
  WiFiDrv::pinMode(25, OUTPUT); //green
  WiFiDrv::pinMode(26, OUTPUT); //red
  WiFiDrv::pinMode(27, OUTPUT); //blue

  WiFiDrv::analogWrite(26, 8);
  delay(5000);
  WiFiDrv::analogWrite(26, 0);
  WiFi.end();
}

void loop() {
  float batteryLevel;
  
  LowPower.sleep(10000);
  //delay(10000);
  WiFiDrv::analogWrite(25,  8); delay(200); WiFiDrv::analogWrite(25, 0);
  WiFiDrv::analogWrite(26,  8); delay(200); WiFiDrv::analogWrite(26, 0);
  WiFiDrv::analogWrite(27, 16); delay(200); WiFiDrv::analogWrite(27, 0);

  //batteryLevel = analogRead(ADC_BATTERY) * 3.3f / 1023.0f / 1.2f * (1.2f+0.33f);
  //Serial.print("battery level: ");
  //Serial.print(batteryLevel);
  //Serial.println("V");
}
