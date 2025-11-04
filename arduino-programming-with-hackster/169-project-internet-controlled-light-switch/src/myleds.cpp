#include "myleds.h"

int pins[] = { RED_LED_PIN, GREEN_LED_PIN, BLUE_LED_PIN };
int exPins[] = { EX_GREEN_LED_PIN, EX_RED_LED_PIN, EX_YELLOW_LED_PIN, EX_BLUE_LED_PIN };

void setUpLeds() {
  for (int j = 0; j < 3; j += 1) {
    pinMode(pins[j], OUTPUT);
  }
  for (int j = 0; j < 4; j += 1) {
    pinMode(exPins[j], OUTPUT);
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

void toggleWhiteLed(boolean isOn) {
  for (int j = 0; j < 3; j += 1) {
    digitalWrite(pins[j], isOn ? HIGH : LOW);
  }
}

void toggleExternalLeds(boolean isOn) {
  for (int j = 0; j < 4; j += 1) {
    digitalWrite(exPins[j], isOn ? HIGH : LOW);
  }
}