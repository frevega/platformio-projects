#ifndef MYLEDS_H
#define MYLEDS_H
#include <Arduino.h>

#define RED_LED_PIN 3
#define GREEN_LED_PIN 4
#define BLUE_LED_PIN 5

void setUpLeds();
void blinkOnboardLed();

#endif