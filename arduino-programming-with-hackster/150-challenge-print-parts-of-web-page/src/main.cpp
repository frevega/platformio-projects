#include <Arduino.h>
#include <ESP8266WiFi.h>

// WiFi Config
String SSID = "";
String PASSW = "";

void getWebPageContents();

void setup() {
  // Initialize Serial
  Serial.begin(9600);
  delay(100);

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
  Serial.println();

  getWebPageContents();
}

void loop() {
  
}


void getWebPageContents() {
  // Server, file and port
  String hostname = "wifitest.adafruit.com";
  String uri = "/testwifi/index.html";
  int port = 80;

  // WiFi client
  WiFiClient client;

  // Connect to server
  Serial.println("Connecting to " + hostname);
  if (client.connect(hostname, port) == 0) {
    Serial.println("Connection failed");
  }

  // Send request for file
  client.println(
    "GET " + uri + " HTTP/1.1\n" +
    "Host: " + hostname + "\n" +
    "Connection: close\n"
  );
  delay(500);
  Serial.println("\n---- Reply from server ----\n");

  // Print reply from server
  while (client.available()) {
    if (client.find("this, ")) {
      Serial.println(client.readStringUntil(':'));
    }
  }
  Serial.println("\n---- Reply from server ----\n");

  // Close TCP connection
  client.stop();
  Serial.println("Connection closed");
}