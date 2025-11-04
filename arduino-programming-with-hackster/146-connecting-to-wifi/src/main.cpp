#include <Arduino.h>
#include <ESP8266WiFi.h>

// WiFi Config
String SSID = "";
String PASSW = "";

void setup() {
  // Initialize Serial
  Serial.begin(9600);
  delay(100);
}

void setup_() {

  // Connect to WiFi
  Serial.println("Connecting to " + SSID);
  WiFi.begin(SSID, PASSW);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  // Show that we are connected
  Serial.println("Connected " + String(WiFi.status()));
  Serial.print("localIP: ");
  Serial.println(WiFi.localIP());
  Serial.print("macAddress: ");
  Serial.println(WiFi.macAddress());
  Serial.print("subnetMask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("gatewayIP: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("dnsIP: ");
  Serial.println(WiFi.dnsIP());
  Serial.print("broadcastIP: ");
  Serial.println(WiFi.broadcastIP());
  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  for (int i = 0; i < 10; i += 1) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(25);
  }
}

byte b;
void loop() {
  Serial.println(b += 1);

  if (b >= 30) {
    b = 0;
  }
  delay(1000);
}
