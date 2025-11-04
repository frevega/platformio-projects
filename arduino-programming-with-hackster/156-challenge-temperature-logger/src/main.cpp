#include <Arduino.h>
#include <HTTPClient.h>
#include "DHT.h"
#include "mywifi.h"
#include "myleds.h"
#include "ThingSpeak.h"

// DHT-11
#define DHT_PIN 10
DHT dht(DHT_PIN, DHT11);

// WiFi Config
String SSID = "";
String PASSW = "";
WiFiClient client;
bool wiFiSuccess = false;

// ThingSpeak config
const unsigned long CHANNEL_ID = 2913098;
const char WRITE_API_KEY[] = "SMB5FHLUHQKNQY0C";

// Temperature reading config
const long TEMP_TIME_INTERVAL = 20000;
unsigned long currentMillis = 0, tempPreviousTime = 0;

int tempC = 0;
int humidity = 0;
int responseCode = 0;

void postToThingSpeak(int tempC, int humidity);

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

  // Initialize DHT sensor
  dht.begin();
}

void loop() {
  currentMillis = millis();
  if (currentMillis - tempPreviousTime >= TEMP_TIME_INTERVAL) {
    tempPreviousTime = currentMillis;
    // Read temperature
    postToThingSpeak(dht.readTemperature(), dht.readHumidity());
  }
}

void postToThingSpeak(int tempC, int humidity) {
  // Write value to out ThingSpeak channel
  Serial.println("Posting " + String(tempC) + ", " + String(humidity) + " to ThingSpeak");
  ThingSpeak.setField(1, tempC);
  ThingSpeak.setField(2, humidity);
  responseCode = ThingSpeak.writeFields(CHANNEL_ID, WRITE_API_KEY);
  if (responseCode == 200) {
    blinkOnboardLed();
    Serial.println("Data posted successfully");
  } else {
    Serial.println("Error posting data: " + String(responseCode));
  }
}