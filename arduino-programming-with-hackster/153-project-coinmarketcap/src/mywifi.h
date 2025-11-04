#ifndef MYWIFI_H
#define MYWIFI_H
#include <Arduino.h>
#include <ESP8266WiFi.h>

bool setupWiFi(HardwareSerial &serial, String SSID, String PASSW);
void blinkOnboardLed();

#endif