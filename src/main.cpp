#include <Arduino.h>

constexpr unsigned long LED_INTERVAL_MS = 1500;
constexpr unsigned long ADC_INTERVAL_MS = 500;

unsigned long lastLedChange = 0;
unsigned long lastAdcRead = 0;
bool ledOn = true;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  Serial.begin(9600);
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
    Serial.println(adcA1);
  }
}
