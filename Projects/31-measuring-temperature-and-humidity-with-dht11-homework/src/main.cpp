#include <Arduino.h>
#include "ArduinoGraphics.h"
#include <Arduino_LED_Matrix.h>
#include <DHT.h>

DHT dht(11, DHT11);
ArduinoLEDMatrix matrix;

// Button config
const byte BUTTON_DEBOUNCE_INTERVAL = 50;
byte buttonState = 0, buttonValue = 0, buttonPreviousValue = 0;
unsigned long currentMillis = 0;

void showTextInLedMatrix();

void setup() {
  Serial.begin(115200);
  dht.begin();
  matrix.begin();
  pinMode(12, INPUT_PULLUP);
}

void loop() {
  buttonValue = digitalRead(12);

  if (buttonValue != buttonPreviousValue) {
    currentMillis = millis();
  }

  if (millis() > (currentMillis + BUTTON_DEBOUNCE_INTERVAL) && (buttonValue != buttonState)) {
    buttonState = buttonValue;
    if (buttonValue == LOW) {
      showTextInLedMatrix();
    }
  }

  buttonPreviousValue = buttonValue;
}

void showTextInLedMatrix() {
  matrix.beginDraw();

  matrix.stroke(0xFFFFFFFF);
  matrix.textScrollSpeed(25);

  // add the text
  String text = "    TEMP: " + String(dht.readTemperature()) + "°C    HUM: " + String(dht.readHumidity()) + "%    HI: " + String(dht.computeHeatIndex()) + "°C    ";
  Serial.println(text);
  matrix.textFont(Font_5x7);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(text);
  matrix.endText(SCROLL_LEFT);

  matrix.endDraw();
}