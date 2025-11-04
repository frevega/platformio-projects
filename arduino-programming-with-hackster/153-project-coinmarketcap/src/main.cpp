#include "mywifi.h"
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <ArduinoJson.h>

#define GREEN_LED 0
#define RED_LED 2

// WiFi Config
String SSID = "";
String PASSW = "";
String symbols[] = {"BTC"};//, "ETH", "USDT", "BNB"};
String changePeriods[] = {"1h", "24h", "7d", "30d", "60d", "90d"};
bool wiFiSuccess = false;
bool shouldUseLEDS = false;
bool isGoingUp = false;

unsigned long previousRead = 0, TIME_INTERVAL = 30000;

void cmcGetRequest(String symbol);
void blink(JsonDocument data);
void setupLEDS();

void setup() {
  Serial.begin(115200);
  wiFiSuccess = setupWiFi(Serial, SSID, PASSW);
  Serial.println(wiFiSuccess ? "WIFI SUCCESS" : "WIFI FAILURE");
  setupLEDS();
}

void loop() {
  if (wiFiSuccess) {
    // Coinmarketcap GET request
    if (millis() - previousRead >= TIME_INTERVAL) {
      previousRead = millis();
      for (String symbol : symbols) {
        cmcGetRequest(symbol);
      }
      Serial.println("Waiting for 30 seconds...");
    }
  }
}

void cmcGetRequest(String symbol) {
  String URL = "https://pro-api.coinmarketcap.com/v2/cryptocurrency/quotes/latest?aux=cmc_rank&symbol=" + symbol;

  WiFiClientSecure client;
  client.setInsecure();

  // Connect to server
  HTTPClient https;
  if (!https.begin(client, URL)) {
    Serial.println("Connection failed");
  }

  // Add headers
  https.addHeader("Accepts", "application/json");
  https.addHeader("Content-Type", "application/json");
  https.addHeader("X-CMC_PRO_API_KEY", "");
  
  // Send GET request & handling
  byte responseCode = https.GET();
  if (responseCode <= 0) {
    Serial.println("FAILURE " + String(responseCode));
    return;
  }
  
  blinkOnboardLed();
  JsonDocument serverResponse;
  DeserializationError error = deserializeJson(serverResponse, https.getString());

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
  } else if (serverResponse["status"]["error_code"] == 0) {
    JsonDocument data = serverResponse["data"][symbol].as<JsonArray>()[0]["quote"]["USD"];
    Serial.println("\n" + symbol);
    Serial.print("Price: ");
    Serial.println((double)data["price"]);
    Serial.println("% change");
    blink(data);
    for (String period: changePeriods) {
      Serial.println(period + ": " + String((double)data["percent_change_" + period]));
    }
  } else {
    Serial.println("Error: " + String(serverResponse["status"]["error_code"]));
  }

  // Close connection
  https.end();
}

void setupLEDS() {
  if (shouldUseLEDS) {
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
  }
}

void blink(JsonDocument data) {
  if (shouldUseLEDS) {
    isGoingUp = ((double)data["percent_change_1h"]) >= 0;
    Serial.println(isGoingUp ? "GREEN" : "RED");
    digitalWrite(isGoingUp ? RED_LED : GREEN_LED, LOW);
    digitalWrite(isGoingUp ? GREEN_LED : RED_LED, HIGH);
  }
}

/* 
void blink(JsonDocument data) {
  if (shouldUseLEDS) {
    if(((double)data["percent_change_1h"]) < 0) {
      Serial.println("RED");
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED, HIGH);
    } else {
      Serial.println("GREEN");
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(RED_LED, LOW);
    }
  }
} */

// #include <Arduino.h>

// //You can modifiy the next line if your board has the LED connected to another Pin
// #define GREEN_LED 0
// #define RED_LED 2
// // the setup function runs once when you press reset or power the board
// void setup() {
//   // initialize digital pin LED_BUILTIN as an output.
//   pinMode(GREEN_LED, OUTPUT);
//   pinMode(RED_LED, OUTPUT);
// }
// // the loop function runs over and over again forever
// void loop() {
//   digitalWrite(GREEN_LED, HIGH); // turn the LED on (HIGH is the voltage level)
//   delay(1000);
//   digitalWrite(GREEN_LED, LOW); // turn the LED off by making the voltage LOW
//   delay(1000); // wait for a second
//   digitalWrite(RED_LED, HIGH); // turn the LED on (HIGH is the voltage level)
//   delay(1000);
//   digitalWrite(RED_LED, LOW); // turn the LED off by making the voltage LOW
//   delay(1000); // wait for a second
// }