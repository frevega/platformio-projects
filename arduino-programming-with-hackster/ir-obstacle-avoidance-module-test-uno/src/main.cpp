#include <Arduino.h>

#define RIGHT_IR_PIN 12
#define LEFT_IR_PIN 8

void setup() {
  pinMode(RIGHT_IR_PIN, INPUT);
  pinMode(LEFT_IR_PIN, INPUT);
  Serial.begin(115200);
}

void loop() {
  Serial.print("R: ");
  Serial.print(digitalRead(RIGHT_IR_PIN));
  Serial.print(" - L: ");
  Serial.println(digitalRead(LEFT_IR_PIN));
  delay(100);
}