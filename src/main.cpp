#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>

constexpr unsigned long LED_INTERVAL_MS = 1500;
constexpr unsigned long ADC_INTERVAL_MS = 500;

unsigned long lastLedChange = 0;
unsigned long lastAdcRead = 0;
bool ledOn = true;
VL53L0X lidar;
bool lidarReady = false;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  Serial.begin(9600);
  Wire.begin();
  Wire.setWireTimeout(25000, true);
  lidar.setTimeout(50);
  lidarReady = lidar.init();
  if (lidarReady) {
    lidar.startContinuous(50);
  } else {
    Serial.println(F("VL53L0X no detectado. Revisar conexion I2C."));
  }
  lastLedChange = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - lastLedChange >= LED_INTERVAL_MS) {
    lastLedChange = now;
    ledOn = !ledOn;
    digitalWrite(LED_BUILTIN, ledOn ? HIGH : LOW);
  }

  if (now - lastAdcRead >= ADC_INTERVAL_MS) {
    lastAdcRead = now;
    const int adcA0 = analogRead(A0);
    const int adcA1 = analogRead(A1);

    Serial.print(F("A0:"));
    Serial.print(adcA0);
    Serial.print(F("\tA1:"));
    Serial.print(adcA1);
    Serial.print(F("\tDistancia_mm:"));
    if (lidarReady) {
      const uint16_t distanceMm = lidar.readRangeContinuousMillimeters();
      if (lidar.timeoutOccurred()) {
        Serial.println(F("TIMEOUT"));
      } else if (distanceMm >= 8190) {
        Serial.println(F("FUERA_DE_RANGO"));
      } else {
        Serial.println(distanceMm);
      }
    } else {
      Serial.println(F("SIN_SENSOR"));
    }
  }
}
