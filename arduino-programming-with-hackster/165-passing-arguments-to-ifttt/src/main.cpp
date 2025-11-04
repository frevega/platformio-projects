#include <Arduino.h>
#include <HTTPClient.h>
#include "DHT.h"
#include "mywifi.h"
#include "myleds.h"

// DHT-11
#define DHT_PIN 10
DHT dht(DHT_PIN, DHT11);

// Temperature reading config
const long TEMP_TIME_INTERVAL = 20000;
unsigned long currentMillis = 0, tempPreviousTime = -1;

int tempC = 0;
int humidity = 0;
int responseCode = 0;

// WiFi Config
String SSID = "";
String PASSW = "";
bool wiFiSuccess = false;

void postToIftttHTTPS(int tempC, int humidity);

void setup() {
  setUpLeds();

  Serial.begin(115200);

  // Connect to WiFi
  wiFiSuccess = setupWiFi(Serial, SSID, PASSW);
  if (wiFiSuccess) {
    blinkOnboardLed();
  }
  Serial.println(wiFiSuccess ? "WIFI SUCCESS\n" : "WIFI FAILURE\n");

  // Initialize DHT sensor
  dht.begin();
}

void loop() {
  currentMillis = millis();
  if (currentMillis - tempPreviousTime >= TEMP_TIME_INTERVAL || tempPreviousTime == -1) {
    tempPreviousTime = currentMillis;
    // Read temperature
    postToIftttHTTPS(dht.readTemperature(), dht.readHumidity());
  }
}

void postToIftttHTTPS(int tempC, int humidity) {
  String URL = "https://maker.ifttt.com/trigger/my_reminder/with/key/";

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient https;
  if (!https.begin(client, URL)) {
    Serial.println("Connection failed");
  } else {
    String payload = "{\"value1\":\"" + String(tempC) + "\",\"value2\":\"" + String(humidity) + "\"}";
    Serial.println(payload);

    https.addHeader("Content-Type", "application/json");
    https.addHeader("Content-Length", String(payload.length()));
    
    byte responseCode = https.POST(payload);
    if (responseCode == HTTP_CODE_OK) {
      blinkOnboardLed();
      Serial.println(String(responseCode) + " " + https.getString());
    } else {
      Serial.println("FAILURE " + String(responseCode));
    }
  }
}