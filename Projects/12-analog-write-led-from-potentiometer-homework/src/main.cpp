#include <Arduino.h>

/*
  
  Equation for linear LED brightness increment
  
  Slope (m) equation
    x, y     x, y
   (0, 0)  (1023, 255)
  m =  (y2 - y1)
        ---------
        (x2 - x1)
  m = (255 - 0) / (1023 - 0)
  m = 255 / 1023

  Equation of the line
  y - y1 = m (x - x1)
  y - 0 = 255 / 1023 (x - 0)
  y - 0 = 255 / 1023 x
  y = 255 / 1023. * x
  x = analog read (potentiometer)

  Equation for exponential LED brightness increment

  brightness = 2 ^ potentiometer / x

  potenciometer / x = 8 
  
  1023 / x = 8
  1023 = 8x
  x = 1023 / 8 = 127.875 - 1
*/

int buttonPin = 4;
int ledState = 1;
int prevButtonValue = 0;
int potPin = A2;
int redPin = 2;
int ledVal = 0;

void linearScaleLedBrightnessIncrement(int reading);
void exponentialScaleLedBrightnessIncrement(int reading);
void toggleLed(int reading);

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(potPin, INPUT);
  pinMode(redPin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  toggleLed(digitalRead(buttonPin));
  if (ledState == 0) {
    linearScaleLedBrightnessIncrement(analogRead(potPin));
  } else {
    exponentialScaleLedBrightnessIncrement(analogRead(potPin));
  }

  delay(10);
}

void linearScaleLedBrightnessIncrement(int reading) {
  ledVal = 255 / 1023. * reading;
  analogWrite(redPin, ledVal);
  Serial.println("(LIN) POT: " + String(reading) + " \tLED: " + String(ledVal));
}

void exponentialScaleLedBrightnessIncrement(int reading) {
  ledVal = pow(2, 8 / 1023. * reading) - 1;
  analogWrite(redPin, ledVal);
  Serial.println("(EXP) POT: " + String(reading) + " \tLED: " + String(ledVal) + " \tEXP: " + String(int(8 / 1023. * reading)));
}

void toggleLed(int reading) {
  if (reading != prevButtonValue) {
    if (reading == 1) {
      ledState = (ledState + 1) % 2;
    }
  }
  
  prevButtonValue = reading;
}