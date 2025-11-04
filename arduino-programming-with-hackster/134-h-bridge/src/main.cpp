#include <Arduino.h>

#define L_PWM_PIN 9
#define DIR_1A_PIN 8
#define DIR_2A_PIN 7

void setup() {
  pinMode(L_PWM_PIN, OUTPUT);
  pinMode(DIR_1A_PIN, OUTPUT);
  pinMode(DIR_2A_PIN, OUTPUT);
}

void loop() {
  // ONE DIRECTION AT HALF SPEED
  digitalWrite(DIR_1A_PIN, HIGH);
  digitalWrite(DIR_2A_PIN, LOW);
  analogWrite(L_PWM_PIN, 160);
  delay(2000);

  // STOP SPINNING
  analogWrite(L_PWM_PIN, 0);
  delay(2000);

  // OTHER DIRECTION AT FULL SPEED
  digitalWrite(DIR_1A_PIN, LOW);
  digitalWrite(DIR_2A_PIN, HIGH);
  analogWrite(L_PWM_PIN, 255);
  delay(2000);

  // BRAKE
  digitalWrite(DIR_1A_PIN, LOW);
  digitalWrite(DIR_2A_PIN, LOW);
  analogWrite(L_PWM_PIN, 0);
  delay(2000);
}