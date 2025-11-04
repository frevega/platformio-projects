#include "mywifi.h"

bool setupWiFi(HardwareSerial &serial, String SSID, String PASSW) {
  // Connect to WiFi
  serial.println("\nConnecting to " + SSID);
  WiFi.begin(SSID, PASSW);

  // while (WiFi.status() != WL_CONNECTED) {
  for (int i = 0; i < 20 && WiFi.status() != WL_CONNECTED; i += 1) {
    delay(500);
    serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    // Show that we are connected
    serial.println("\nConnected " + String(WiFi.status()));
    serial.print("localIP: ");
    serial.println(WiFi.localIP());
    serial.print("macAddress: ");
    serial.println(WiFi.macAddress());
    serial.print("subnetMask: ");
    serial.println(WiFi.subnetMask());
    serial.print("gatewayIP: ");
    serial.println(WiFi.gatewayIP());
    serial.print("dnsIP: ");
    serial.println(WiFi.dnsIP());
    serial.print("broadcastIP: ");
    serial.println(WiFi.broadcastIP());
    serial.print("RSSI: ");
    serial.println(WiFi.RSSI());

    pinMode(LED_BUILTIN, OUTPUT);
    blinkOnboardLed();
    return true;
  } else {
    return false;
  }
}

void blinkOnboardLed() {
    digitalWrite(LED_BUILTIN, HIGH);
    for (int i = 0; i < 10; i += 1) {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      delay(25);
    }
}