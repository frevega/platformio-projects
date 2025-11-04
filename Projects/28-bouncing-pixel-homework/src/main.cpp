#include <Arduino.h>
#include <Arduino_LED_Matrix.h>

ArduinoLEDMatrix matrix;
byte frame[8][12] = {};
int x = 0, y = 0;
bool isDecreasingBrightness = false, yBouncesBack = false;
unsigned long pixelPrevTime = 0, TIME_INTERVAL = 40;

void setup() {
  randomSeed(analogRead(A0));
  x = random(12);
  y = random(8);
  matrix.begin();
}

void loop() {
  if (millis() - pixelPrevTime > TIME_INTERVAL) {
    pixelPrevTime = millis();

    if (x >= 11 || x <= 0 && isDecreasingBrightness) {
      isDecreasingBrightness = !isDecreasingBrightness;
    }
    if (y >= 7 || y <= 0 && yBouncesBack) {
      yBouncesBack = !yBouncesBack;
    }
    isDecreasingBrightness ? x -= 1 : x += 1;
    yBouncesBack ? y -= 1 : y += 1;

    memset(frame, 0, sizeof(frame));
    frame[y][x] = 1;
    matrix.renderBitmap(frame, 8, 12);
  }
}
