#include <Arduino.h>
#include <HTTPClient.h>
#include "mywifi.h"
#include "myleds.h"
#include "ThingSpeak.h"

// WiFi Config
String SSID = "";
String PASSW = "";
WiFiClient client;
bool wiFiSuccess = false;

// ThingSpeak config
const unsigned long CHANNEL_ID = 2913098;
const char READ_API_KEY[] = "4V10GXNB50RKBJCR";

void setup() {
  setUpLeds();

  Serial.begin(115200);

  // Connect to WiFi
  wiFiSuccess = setupWiFi(Serial, SSID, PASSW);
  if (wiFiSuccess) {
    blinkOnboardLed();
  }
  Serial.println(wiFiSuccess ? "WIFI SUCCESS" : "WIFI FAILURE");

  // Connect to ThingSpeak
  ThingSpeak.begin(client);

  int value = ThingSpeak.readIntField(CHANNEL_ID, 1, READ_API_KEY);
  Serial.println("Retrieved value: " + String(value));
}

void loop() {
  // Do nothing
}