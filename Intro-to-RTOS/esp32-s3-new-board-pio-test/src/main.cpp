#include <Arduino.h>
// #include "esp32-hal-rgb-led.h"

#define LED_PIN GPIO_NUM_38

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  neopixelWrite(LED_PIN, 1, 0, 0);
  Serial.println("RED LED ON");
  delay(250);
  neopixelWrite(LED_PIN, 0, 1, 0);
  Serial.println("GREEN LED ON");
  delay(250);
  neopixelWrite(LED_PIN, 0, 0, 1);
  Serial.println("BLUE LED ON");
  delay(250);
  neopixelWrite(LED_PIN, 1, 1, 1);
  Serial.println("WHITE LED ON");
  delay(250);
  neopixelWrite(LED_PIN, 0, 0, 0);
  Serial.println("LED OFF");
  delay(250);
}
