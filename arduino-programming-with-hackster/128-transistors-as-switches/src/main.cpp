#include <Arduino.h>

#define LED_PIN 6
#define LED_TIME_INTERVAL 1000

unsigned long previous_read = 0;

void setup() {
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    if (millis() - previous_read >= LED_TIME_INTERVAL) {
        previous_read = millis();
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
}