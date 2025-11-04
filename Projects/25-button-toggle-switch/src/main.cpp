#include <Arduino.h>

int redPin = 10;
int greenPin = 9;
int bluePin = 8;
int rgbPins[] = {redPin, greenPin, bluePin};
int currentLed = 0;
int buttonPin = 4;
int prevButtonValue = 1;
unsigned long prevButtonTime = 0, BUTTON_TIME_INTERVAL = 50;
byte buttonPushCounter = 0;

void toggleLed(int value);

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  if (millis() - prevButtonTime >= BUTTON_TIME_INTERVAL) {
    prevButtonTime = millis();
    toggleLed(digitalRead(buttonPin));
  }

  if (buttonPushCounter % 2 == 0) {
    currentLed = rgbPins[(buttonPushCounter / 2) % 3];
  }
}

void toggleLed(int value) {
  if (value != prevButtonValue) {
    prevButtonValue = value;
    if (value == 1) {
      digitalWrite(currentLed, digitalRead(currentLed) == LOW ? HIGH : LOW);
      buttonPushCounter += 1;
    }
  }
}