#include <Arduino.h>
#include <DHT.h>

DHT dht(11, DHT11);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  Serial.println("Temperature\t: " + String(dht.readTemperature()) + "°C");
  Serial.println("Humidity\t: " + String(dht.readHumidity()) + "%");
  Serial.println("Heat index\t: " + String(dht.computeHeatIndex()) + "°C\n");
  delay(2000);
}