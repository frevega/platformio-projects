#include <Arduino.h>

int intValue = 0;
float floatValue = 0;
String stringValue;

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("What's your number?");

  while (Serial.available() == 0) {}

  intValue = Serial.parseInt();
  Serial.println("Your number is " + String(intValue));

  Serial.println("What's your float?");

  while (Serial.available() == 0) {}

  floatValue = Serial.parseFloat();
  Serial.println("Your float is " + String(floatValue));

  Serial.println("What's your name?");

  while (Serial.available() == 0) {}

  stringValue = Serial.readString();
  Serial.println("Your name is " + String(stringValue));
}