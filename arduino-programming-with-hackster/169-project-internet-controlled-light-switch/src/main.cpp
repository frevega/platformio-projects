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
const unsigned long CHANNEL_ID = 2916946;
const char READ_API_KEY[] = "TEOBCDQ728BNIGKY";

// Reading config
const long TIME_INTERVAL = 1000;
unsigned long currentMillis = 0, tempPreviousTime = 0;

int ledValue = 0;

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
}

void loop() {
  currentMillis = millis();
  if (currentMillis - tempPreviousTime >= TIME_INTERVAL) {
    tempPreviousTime = currentMillis;
    ledValue = ThingSpeak.readIntField(CHANNEL_ID, 1, READ_API_KEY);
    Serial.println("Retrieved value: " + String(ledValue));
    // toggleWhiteLed(ledValue == 1 ? true : false);
    toggleExternalLeds(ledValue == 1 ? true : false);
  }
}