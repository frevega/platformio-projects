#include <Arduino.h>

unsigned long greenPrevRead = 0, redPrevRead = 0, prevRead = 0, GREEN_TIME_INTERVAL = 2000, RED_TIME_INTERVAL = 4000, TIME_INTERVAL = 2050;
byte red = 0, green = 0;

void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  if (Serial.available() > 0) {
    digitalWrite(LED_BUILTIN, Serial.read()); // FROM UNO
  }

  if (millis() - redPrevRead >= RED_TIME_INTERVAL) {
    redPrevRead = millis();
    // Serial.println("R:" + String(red % 2)); // TO UNO
    red += 1;
  }

  if (millis() - greenPrevRead >= GREEN_TIME_INTERVAL) {
    greenPrevRead = millis();
    // Serial.println("G:" + String(green % 2)); // TO UNO
    green += 1;
  }

  if (millis() - prevRead >= TIME_INTERVAL) {
    prevRead = millis();
    Serial.println("R:" + String(red % 2) + "-G:" + String(green % 2)); // TO UNO
  }
}