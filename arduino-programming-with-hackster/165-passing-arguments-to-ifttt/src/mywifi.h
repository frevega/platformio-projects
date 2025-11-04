#ifndef MYWIFI_H
#define MYWIFI_H
#include <Arduino.h>
#include <WiFi.h>

bool setupWiFi(HardwareSerial &serial, String SSID, String PASSW);

#endif