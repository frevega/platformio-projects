#include <Arduino.h>
#include <HTTPClient.h>
#include "mywifi.h"
#include "myleds.h"

// WiFi Config
String SSID = "";
String PASSW = "";
bool wiFiSuccess = false;

const long TEMP_TIME_INTERVAL = 60000;
unsigned long currentMillis = 0, tempPreviousTime = -1;

void iftttRequestHTTPS();

void setup() {
  setUpLeds();

  Serial.begin(115200);

  // Connect to WiFi
  wiFiSuccess = setupWiFi(Serial, SSID, PASSW);
  if (wiFiSuccess) {
    blinkOnboardLed();
  }
  Serial.println(wiFiSuccess ? "WIFI SUCCESS\n" : "WIFI FAILURE\n");
}

void loop() {currentMillis = millis();
  if (currentMillis - tempPreviousTime >= TEMP_TIME_INTERVAL || tempPreviousTime == -1) {
    tempPreviousTime = currentMillis;
    iftttRequestHTTPS();
  }
}

void iftttRequestHTTPS() {
  String URL = "https://maker.ifttt.com/trigger/my_reminder/json/with/key/";

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient https;
  if (!https.begin(client, URL)) {
    Serial.println("Connection failed");
  } else {
    https.addHeader("Content-Type", "application/json");
    
    byte responseCode = https.POST(NULL, 0);
    if (responseCode == HTTP_CODE_OK) {
      blinkOnboardLed();
      Serial.println(String(responseCode) + " " + https.getString());
    } else {
      Serial.println("FAILURE " + String(responseCode));
    }
  }
}