#include <Arduino.h>
#include <Servo.h>

// RGB LED
int redPin = 10;
int greenPin = 9;
int bluePin = 8;
int rgbPins[] = {redPin, greenPin, bluePin};
bool isRgbSequenceStopped = false;
byte blinkCounter = 0;
unsigned long rgbShortTimeInterval = 50, rgbLongTimeInterval = 1000, rgbPrevTime = 0;

// SERVO
Servo servo;
int servoPos = 0;
bool goingForward = true;
bool isServoSequenceStopped = false;
unsigned long servoShortTimeInterval = 10, servoLongTimeInterval = 1000, servoPrevTime = 0;

bool shouldContinueOnLongRgbDelay();
bool shouldContinueOnServoLimitDelay();

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  servo.attach(6);
}

void loop() {
  if (shouldContinueOnLongRgbDelay() && millis() - rgbPrevTime >= rgbShortTimeInterval) {
    rgbPrevTime = millis();
    
    digitalWrite(rgbPins[(blinkCounter / 2) % 3], digitalRead(rgbPins[(blinkCounter / 2) % 3]) == LOW ? HIGH : LOW);
    blinkCounter += 1;

    if (blinkCounter % 6 == 0) {
      isRgbSequenceStopped = true;
    }
  }

  if (shouldContinueOnServoLimitDelay() && millis() - servoPrevTime >= servoShortTimeInterval) {
    servoPrevTime = millis();
    servo.write(goingForward ? servoPos += 1 : servoPos -= 1);

    if (servoPos == 180 || servoPos == 0) {
      goingForward = !goingForward;
      isServoSequenceStopped = true;
    }
  }
}

bool shouldContinueOnLongRgbDelay() {
  if (isRgbSequenceStopped && millis() - rgbPrevTime >= rgbLongTimeInterval) {
    rgbPrevTime = millis();
    isRgbSequenceStopped = false;
  }

  return !isRgbSequenceStopped;
}

bool shouldContinueOnServoLimitDelay() {
  if (isServoSequenceStopped && millis() - servoPrevTime >= servoLongTimeInterval) {
    servoPrevTime = millis();
    isServoSequenceStopped = false;
  }

  return !isServoSequenceStopped;
}