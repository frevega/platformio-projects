#include <Arduino.h>
#include <HTTPClient.h>
#include "DHT.h"
#include "mywifi.h"
#include "myleds.h"

// BUTTON
#define BUTTON_PIN 1

// Button config
const byte BUTTON_DEBOUNCE_INTERVAL = 50;
byte buttonState = 0, buttonValue = 0, buttonPreviousValue = 0;

// DHT-11
#define DHT_PIN 10
DHT dht(DHT_PIN, DHT11);

int tempC = 0;
int humidity = 0;
int responseCode = 0;

unsigned long currentMillis = 0;

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

  // Button
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  buttonValue = digitalRead(BUTTON_PIN);

  if (buttonValue != buttonPreviousValue) {
    currentMillis = millis();
  }

  if (millis() > (currentMillis + BUTTON_DEBOUNCE_INTERVAL) && (buttonValue != buttonState)) {
    buttonState = buttonValue;
    if (buttonValue == LOW) {
      postToIftttHTTPS(dht.readTemperature(), dht.readHumidity());
    }
  }

  buttonPreviousValue = buttonValue;
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