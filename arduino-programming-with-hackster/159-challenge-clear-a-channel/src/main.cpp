#include <Arduino.h>
#include <HTTPClient.h>
#include "mywifi.h"
#include "myleds.h"

// WiFi Config
String SSID = "";
String PASSW = "";
bool wiFiSuccess = false;

String body = "api_key=L7AWAUAWUNCL3UFB";

void thinkSpeakClearChannelHTTP();
void thinkSpeakClearChannelHTTPS();

void setup() {
  setUpLeds();

  Serial.begin(115200);

  // Connect to WiFi
  wiFiSuccess = setupWiFi(Serial, SSID, PASSW);
  if (wiFiSuccess) {
    blinkOnboardLed();
  }
  Serial.println(wiFiSuccess ? "WIFI SUCCESS\n" : "WIFI FAILURE\n");

  Serial.println("thinkSpeakClearChannelHTTP()");
  thinkSpeakClearChannelHTTP();
  delay(2000);
  Serial.println("\nthinkSpeakClearChannelHTTPS()");
  thinkSpeakClearChannelHTTPS();
}

void loop() {
  // Do nothing
}


void thinkSpeakClearChannelHTTP() {
  // Server, file and port
  char hostname[] = "api.thingspeak.com";
  String uri = "/channels/2911250/feeds.json";

  // WiFi client
  WiFiClient client;

  // Connect to server
  Serial.println("Connecting to " + String(hostname));
  if (client.connect(hostname, 80, 1000) == 0) {
    Serial.println("Connection failed");
  } else {
    // Send request for file
    client.println(
      "DELETE " + uri + " HTTP/1.1\n" +
      "Host: " + hostname + "\n" +
      "Content-Type: application/x-www-form-urlencoded\n" +
      "Content-Length: " + String(body.length()) + "\n" +
      "Connection: close\n\n" +
      body + "\n"
    );
    delay(500);

    // Print reply from server
    while (client.available()) {
      Serial.print(client.readStringUntil('\r'));
    }

    // Close TCP connection
    client.stop();
    Serial.println();
    Serial.println("Connection closed");
  }
}

void thinkSpeakClearChannelHTTPS() {
  String URL = "https://api.thingspeak.com/channels/2911250/feeds.json";

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient https;
  if (!https.begin(client, URL)) {
    Serial.println("Connection failed");
  } else {
    https.addHeader("Content-Type", "application/x-www-form-urlencoded");
    https.addHeader("Content-Length", String(body.length()));
    
    byte responseCode = https.sendRequest("DELETE", body);
    if (responseCode <= 0) {
      Serial.println("FAILURE " + String(responseCode));
    } else {
      Serial.println(String(responseCode) + " " + https.getString());
    }
  }
}