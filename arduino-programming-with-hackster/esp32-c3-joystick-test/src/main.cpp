#include <Arduino.h>
#include "pins_arduino.h"

#define xPin A0
#define yPin A1
int xValue = 0;
int yValue = 0;

void setup() {
  Serial.begin(115200);
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  analogReadResolution(10); // Like Arduino Uno
}

void loop() {
  xValue = analogRead(xPin);
  yValue = analogRead(yPin);
  Serial.print("X: " + String(xValue) + " Y: " + String(yValue) + "                \r");
  delay(150);
}