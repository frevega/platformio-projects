#include <Arduino.h>
// #include <SoftwareSerial.h>

#define GREEN_LED 11
#define RED_LED 12

unsigned long previousRead = 0, TIME_INTERVAL = 1000;
byte b = 0;
String espValues[2] = {};

// SoftwareSerial ESP8266(9, 10); // RX, TX

void setup() {
  Serial.begin(115200);  // Computer
  delay(200);
  // ESP8266.begin(9600); // ESP8266

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
}

void loop() {
  if (Serial.available()) { // FROM ESP-01S
  // if (ESP8266.available() > 0) {
    espValues[0] = Serial.readStringUntil('-');
    espValues[1] = Serial.readString();
    for (int i = 0; i < 2; i += 1) {
      // Serial.println(String(espValues[0]) + " " + String(espValues[1]));
      // Serial.println(espValues[i].charAt(0));
      // Serial.println(espValues[i].charAt(2));
      digitalWrite(espValues[i].charAt(0) == 'G' ? GREEN_LED : RED_LED, espValues[i].charAt(2) - '0');
    }
    // digitalWrite(GREEN_LED, ESP8266.parseInt());
  }

  if (millis() - previousRead >= TIME_INTERVAL) {
    previousRead = millis();
    Serial.write(b % 2); // TO ESP-01S
    // ESP8266.write(b % 2);
    b += 1;
  }
}