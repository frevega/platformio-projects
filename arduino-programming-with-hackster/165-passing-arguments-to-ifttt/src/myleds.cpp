#include "myleds.h"

#define RED_LED_PIN 3
#define GREEN_LED_PIN 4
#define BLUE_LED_PIN 5
int pins[] = { RED_LED_PIN, GREEN_LED_PIN, BLUE_LED_PIN };

void setUpLeds() {
  for (int j = 0; j < 3; j += 1) {
    pinMode(pins[j], OUTPUT);
  }
}

void blinkOnboardLed() {
    for (int i = 0; i < 10; i += 1) {
      for (int j = 0; j < 3; j += 1) {
        digitalWrite(pins[j], !digitalRead(pins[j]));
      }
      delay(25);
    }
}