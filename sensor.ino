////////////////////////////////////////////////////////////////////////////////
// HYPPOLITsensor - Sensor library                  http://github.com/terjekbela
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// sensor code
////////////////////////////////////////////////////////////////////////////////

float sensorValueBattery() {
  #if defined(ADC_BATTERY)
    return analogRead(ADC_BATTERY) * 3.3f / 1023.0f / 1.2f * (1.2f + 0.33f);
  #else
    return NAN;
  #endif  
}

float sensorValueMCP9808() {
  float value = NAN;
  Adafruit_MCP9808 sensor = Adafruit_MCP9808();
  if (sensor.begin(0x18)) {
    sensor.wake();
    value = sensor.readTempC();
    sensor.shutdown_wake(1);
  }
  return value;
}

float sensorValueBME280Temperature() {
  float value = NAN;
  Adafruit_BME280 sensor;
  sensor.begin();
  sensor.takeForcedMeasurement();
  value = sensor.readTemperature();
  return value;
}

float sensorValueBME280Humidity() {
  float value = NAN;
  Adafruit_BME280 sensor;
  sensor.begin();
  sensor.takeForcedMeasurement();
  value = sensor.readHumidity();
  return value;
}

float sensorValueBME280Pressure() {
  float value = NAN;
  Adafruit_BME280 sensor;
  sensor.begin();
  sensor.takeForcedMeasurement();
  value = sensor.readPressure() / 100;
  return value;
}
