#include <Arduino.h>
#include "ArduinoGraphics.h"
#include <Arduino_LED_Matrix.h>

using namespace std;

typedef enum {
  UP   = 1,
  DOWN = 0,
} PaddleDirection;

ArduinoLEDMatrix matrix;
byte frame[8][12] = {};
byte x = 11, y = 0, paddlePos = 3;
bool isDecreasingBrightness = false, yBouncesBack = false, isPlaying = true;
const char text[] = "    GAME OVER";
byte upButtonPin = 2, downButtonPin = 3;
byte prevUpButtonRead = 0, prevDownButtonRead = 0;
unsigned long pixelPrevTime = 0, PIXEL_TIME_INTERVAL = 200, buttonPrevTime = 0, BUTTON_TIME_INTERVAL = 50;

void buttonPush(byte value, byte &previousRead, PaddleDirection direction);
void gameOverMessage();

void setup() {
  pinMode(downButtonPin, INPUT_PULLUP);
  pinMode(upButtonPin, INPUT_PULLUP);
  randomSeed(analogRead(A0));
  y = random(7);
  matrix.begin();
}

void loop() {
  if (!isPlaying) {
    gameOverMessage();
  } else {
    if (millis() - buttonPrevTime > BUTTON_TIME_INTERVAL) {
        buttonPrevTime = millis();
        buttonPush(digitalRead(upButtonPin), prevUpButtonRead, UP);
        buttonPush(digitalRead(downButtonPin), prevDownButtonRead, DOWN);
    }

    if (millis() - pixelPrevTime > PIXEL_TIME_INTERVAL) {
      pixelPrevTime = millis();

      if (x >= 11 || x <= 0 && isDecreasingBrightness) {
        isDecreasingBrightness = !isDecreasingBrightness;
      }
      if (y >= 7 || y <= 0 && yBouncesBack) {
        yBouncesBack = !yBouncesBack;
      }
      isDecreasingBrightness ? x -= 1 : x += 1;
      yBouncesBack ? y -= 1 : y += 1;
      
      if (x == 0 && (y >= paddlePos && y <= paddlePos + 2)) {
        PIXEL_TIME_INTERVAL -= 10;
      }

      memset(frame, 0, sizeof(frame));
    }

    frame[y][x] = 1;
    for (int i = 0; i < 3; i += 1) {
      frame[paddlePos + i][0] = 1;
    }
    matrix.renderBitmap(frame, 8, 12);

    if (x == 0 && (y < paddlePos || y > paddlePos + 2)) {
      isPlaying = !isPlaying;
      delay(1000);
    }
  }
}

void buttonPush(byte value, byte &previousRead, PaddleDirection direction) {
  if (value != previousRead) {
    previousRead = value;
    if (value == 1) {
      if (direction ? paddlePos > 0 : paddlePos < 5) {
        direction ? paddlePos -= 1 : paddlePos += 1;
      }
    }
  }
}

void gameOverMessage() {
  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);
  matrix.textScrollSpeed(80);
  matrix.textFont(Font_4x6);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(text);
  matrix.endText(SCROLL_LEFT);
  matrix.endDraw();
}