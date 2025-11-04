#include <Arduino.h>

const int LED = 12;

void blink(short delayTime) {
  for (int i = 0; i < 3; i += 1) {
    Serial.println(i);
    digitalWrite(LED, HIGH);
    delay(delayTime);
    digitalWrite(LED, LOW);
    delay(i == 2 ? 500 : delayTime);
  }
}

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  blink(250);
  blink(125);
  blink(250);
  delay(1000);
}