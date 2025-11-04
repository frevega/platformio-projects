#include <Arduino.h>
#include "ArduinoGraphics.h"
#include <Arduino_LED_Matrix.h>
#include "my_animation.h"

ArduinoLEDMatrix matrix;

void setup() {
  matrix.loadSequence(frames);
  matrix.begin();
  matrix.play();
  delay(2000);
}

void loop() {
  matrix.beginDraw();

  matrix.stroke(0xFFFFFFFF);
  matrix.textScrollSpeed(60);

  const char text[] = "    UNO R4    ";
  matrix.textFont(Font_4x6);
  matrix.beginText(0, 0, 0xFFFFFF);
  matrix.println(text);
  matrix.endText(SCROLL_RIGHT);

  matrix.endDraw();

  delay(1000);

  // Make it scroll!
  matrix.beginDraw();

  matrix.stroke(0xFFFFFFFF);
  matrix.textScrollSpeed(50);

  // add the text
  const char text1[] = "    Hello World!    ";
  matrix.textFont(Font_5x7);
  matrix.beginText(0, 2, 0xFFFFFF);
  matrix.println(text1);
  matrix.endText(SCROLL_LEFT);

  matrix.endDraw();
}