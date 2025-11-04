#include <Arduino.h>
#include <HTTPClient.h>
#include "ThingSpeak.h"
#include "mywifi.h"
#include "myleds.h"

// WiFi Config
String SSID = "";
String PASSW = "";
WiFiClient client;
bool wiFiSuccess = false;

// ThingSpeak config
const unsigned long CHANNEL_ID = 2911250;
const char WRITE_API_KEY[] = "UZ23HSIJXXOYEKBJ";

int rando = 0;
int responseCode = 0;

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
  randomSeed(micros());
}

void loop() {
  delay(20000);
  // Generate random number 1-100
  rando = random(1, 101);

  // Write value to out ThingSpeak channel
  Serial.println("Posting " + String(rando) + " to ThingSpeak");
  ThingSpeak.setField(1, rando);
  responseCode = ThingSpeak.writeFields(CHANNEL_ID, WRITE_API_KEY);
  if (responseCode == 200) {
    blinkOnboardLed();
    Serial.println("Data posted successfully");
  } else {
    Serial.println("Error posting data: " + String(responseCode));
  }
}