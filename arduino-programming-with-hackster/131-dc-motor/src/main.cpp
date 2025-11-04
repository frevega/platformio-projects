#include <Arduino.h>

/*
  Slope (m) equation
       x, y    x, y
   (1023, 0)  (0, 255)
  m =  (y2 - y1)
        ---------
        (x2 - x1)
  m = (255 - 0) / (0 - 1023)
  m = - 255 / 1023

  Equation of the line
  y - y1 = m (x - x1)
  y - 0 = -255 / 1023 (x - 1023)
  y = -255 / 1023. * (x - 1023)
  x = analog read (potentiometer)
*/

#define MOTOR_PIN 6
#define POT_PIN A5
#define MOTOR_INTERVAL_DIGITAL 1000
#define MOTOR_INTERVAL_ANALOG 2000
#define MOTOR_INTERVAL_POT 100
unsigned long previous_read = 0;
byte exponentValue = 5;
byte motorValue = 0;

void digital();
void analog();
void potentiometer();

void setup() {
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  // digital();
  // analog();
  potentiometer();
}

void digital() {
  if (millis() - previous_read >= MOTOR_INTERVAL_DIGITAL) {
    previous_read = millis();
    digitalWrite(MOTOR_PIN, !digitalRead(MOTOR_PIN));
  }
}

// Lesson challenge
void analog() {
  if (millis() - previous_read >= MOTOR_INTERVAL_ANALOG) {
    previous_read = millis();
    if (exponentValue > 8) {
      exponentValue = 5;
    }

    analogWrite(MOTOR_PIN, pow(2, exponentValue));
    Serial.println("exp value: " + String(pow(2, exponentValue)));
    exponentValue += 1;
  }
}

void potentiometer() {
  if (millis() - previous_read >= MOTOR_INTERVAL_POT) {
    previous_read = millis();
    motorValue = -255 / 910. * (analogRead(POT_PIN) - 910);
    Serial.print(motorValue);
    Serial.print(" ");
    Serial.println(analogRead(POT_PIN));
    analogWrite(MOTOR_PIN, motorValue);
  }
}