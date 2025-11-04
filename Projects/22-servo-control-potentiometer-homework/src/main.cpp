#include <Arduino.h>
#include <Servo.h>

/*
  Slope (m) equation
    x, y     x, y
   (0, 180)  (1023, 0)
  m =  (y2 - y1)
        ---------
        (x2 - x1)
  m = (0 - 180) / (1023 - 0)
  m = -180 / 1023

  Equation of the line
  y - y1 = m (x - x1)
  y - 180 = -180 / 1023 (x - 0)
  y - 180 = -180 / 1023 x
  y = -180 / 1023. * x + 180
  x = analog read (potentiometer)
*/

int potPin = A2;
int servoPin = 9;
Servo servo;
int servoPos = 0;

void setup() {
  Serial.begin(115200);
  pinMode(potPin, INPUT);
  servo.attach(servoPin);
}

void loop() {
  servoPos = -180 / 1023. * analogRead(potPin) + 180;
  Serial.println(servoPos);
  servo.write(servoPos);
}