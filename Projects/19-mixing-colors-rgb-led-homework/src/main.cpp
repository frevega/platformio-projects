#include <Arduino.h>

int redPin = 4;
int greenPin = 3;
int bluePin = 2;
int rgbPins[] = {redPin, greenPin, bluePin};
int blinkLimit = 500;

int rgbPinsSize = 3;
bool isRgbSequenceStopped = false;
bool isShortLightSequenceStopped = false;
int counter = 0;
int blinkCounter = 0;
unsigned long rgbShortTimeInterval = 100, lightSequenceLongTimeInterval[] = {100, 1000}, curTime2 = 0, prevTime2 = 0, curTime = 0, rgbPrevTime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

// void loop() {
//   Serial.println("Enter number of blinks:");
  
//   while (Serial.available() == 0) {}
  
//   blinkLimit = Serial.parseInt();
//   if (blinkLimit <= 0) {
//     Serial.println("Number of blinks shoulld be greater than 0");
//   } else {
//     Serial.println("Will blink " + String(blinkLimit) + " time(s)");
//     for (int i = 0; i < blinkLimit; i += 1) {
//       Serial.println(i + 1);
//       blinkSequence();
//     }
//   }
// }

// void blinkSequence() {
//   for (int i = 0; i < sizeof(rgbPins) / sizeof(int); i += 1) {
//     for (int j = 0; j < 2; j += 1) {
//       digitalWrite(rgbPins[i], digitalRead(rgbPins[i]) == LOW ? HIGH : LOW);
//       delay(50);
//     }
//   }
//   delay(1000);
// }

void loop() {
  if (counter < blinkLimit) {
    curTime = millis();
    if (curTime - rgbPrevTime >= rgbShortTimeInterval) {
      rgbPrevTime = curTime;
      digitalWrite(rgbPins[blinkCounter], digitalRead(rgbPins[blinkCounter]) == LOW ? isRgbSequenceStopped ? LOW : HIGH : LOW);

      Serial.print(rgbPins[blinkCounter]);
      Serial.print(" ");
      Serial.println(blinkCounter);
      blinkCounter += 1;

      if (blinkCounter > 2) {
        blinkCounter = 0;
      }

      curTime2 = millis();
      if (curTime2 - prevTime2 >= lightSequenceLongTimeInterval[counter % 2] && blinkCounter == 0) {
        prevTime2 = curTime2;
        isRgbSequenceStopped = !isRgbSequenceStopped;
        counter += 1;
        blinkCounter = 0;
      }
    }
  }
}