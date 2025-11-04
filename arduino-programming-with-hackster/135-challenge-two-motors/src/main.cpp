#include <Arduino.h>

#define L_PWM_PIN 9
#define DIR_1A_PIN 8
#define DIR_2A_PIN 7
#define R_PWM_PIN 3
#define DIR_3A_PIN 5
#define DIR_4A_PIN 4
byte pins[] = { L_PWM_PIN, R_PWM_PIN, DIR_1A_PIN, DIR_2A_PIN, DIR_3A_PIN, DIR_4A_PIN };

void motorAction(byte enOnePin, byte enOneValue, byte enTwoPin, byte enTwoValue, byte pwmPin, byte pwmValue);

void setup() {
  for (byte pin : pins) {
    pinMode(pin, OUTPUT);
  }
}

void loop() {
  // Drive forward (both motors spin one direction) at full speed for 1 second
  motorAction(DIR_1A_PIN, LOW, DIR_2A_PIN, HIGH, L_PWM_PIN, 255);
  motorAction(DIR_3A_PIN, LOW, DIR_4A_PIN, HIGH, R_PWM_PIN, 255);
  delay(2000);
  // Turn left at half speed for 1 second
  motorAction(DIR_1A_PIN, HIGH, DIR_2A_PIN, LOW, L_PWM_PIN, 160);
  motorAction(DIR_3A_PIN, LOW, DIR_4A_PIN, HIGH, R_PWM_PIN, 160);
  delay(2000);
  // Turn right at half speed for 1 second
  motorAction(DIR_1A_PIN, LOW, DIR_2A_PIN, HIGH, L_PWM_PIN, 160);
  motorAction(DIR_3A_PIN, HIGH, DIR_4A_PIN, LOW, R_PWM_PIN, 160);
  delay(2000);
  // Drive backward at half speed for 1 second
  motorAction(DIR_1A_PIN, HIGH, DIR_2A_PIN, LOW, L_PWM_PIN, 160);
  motorAction(DIR_3A_PIN, HIGH, DIR_4A_PIN, LOW, R_PWM_PIN, 160);
  delay(2000);
  // Stop for 1 second
  motorAction(DIR_1A_PIN, LOW, DIR_2A_PIN, LOW, L_PWM_PIN, 0);
  motorAction(DIR_3A_PIN, LOW, DIR_4A_PIN, LOW, R_PWM_PIN, 0);
  delay(2000);
}

void motorAction(byte enOnePin, byte enOneValue, byte enTwoPin, byte enTwoValue, byte pwmPin, byte pwmValue) {
  digitalWrite(enOnePin, enOneValue);
  digitalWrite(enTwoPin, enTwoValue);
  analogWrite(pwmPin, pwmValue);
}