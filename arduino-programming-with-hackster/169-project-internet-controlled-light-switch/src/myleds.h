#ifndef MYLEDS_H
#define MYLEDS_H
#include <Arduino.h>

#define RED_LED_PIN 3
#define GREEN_LED_PIN 4
#define BLUE_LED_PIN 5

// External LEDS
#define EX_GREEN_LED_PIN 0
#define EX_RED_LED_PIN 1
#define EX_YELLOW_LED_PIN 2
#define EX_BLUE_LED_PIN 10

void setUpLeds();
void blinkOnboardLed();
void toggleWhiteLed(boolean isOn);
void toggleExternalLeds(boolean isOn);

#endif