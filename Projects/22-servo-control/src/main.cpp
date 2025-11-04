#include <Arduino.h>
#include <Servo.h>

Servo servo;
int servoPos = 0;
bool goingForward = true;
bool sequenceStopped = false;

void setup() {
  Serial.begin(115200);
  servo.attach(9);
}

unsigned long servoShortTimeInterval = 10, servoLongTimeInterval = 1000, curTime2 = 0, prevTime2 = 0;

bool shouldContinueOnServoLimitDelay();

void loop() {
  curTime2 = millis();
  if (shouldContinueOnServoLimitDelay() && curTime2 - prevTime2 >= servoShortTimeInterval) {
    prevTime2 = curTime2;
    servo.write(goingForward ? servoPos += 1 : servoPos -= 1);
    Serial.print("servo.read(): " + String(servo.read()) + " \r");

    if (servoPos == 180 || servoPos == 0) {
        goingForward = !goingForward;
        Serial.println("\nstop");
        sequenceStopped = true;
    }
  }
}

bool shouldContinueOnServoLimitDelay() {
  curTime2 = millis();
  if (sequenceStopped && curTime2 - prevTime2 >= servoLongTimeInterval) {
    prevTime2 = curTime2;
    sequenceStopped = false;
    Serial.println("continue");
  }

  return !sequenceStopped;
}