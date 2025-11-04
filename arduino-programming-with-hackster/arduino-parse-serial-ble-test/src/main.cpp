#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(12, 13); // RX, TX

char message[10];
int i = 0, j = 0;
int x = 0, y = 0;

void setup() {
  mySerial.begin(9600);
  Serial.begin(115200);
}

void loop() {
  // if (Serial.available() > 0) {
  if (mySerial.available() > 0) {
    if (i < 9) {
      // Serial.write(Serial.read());
      message[i] = mySerial.read();//Serial.read();
      i += 1;
      if (i == 8) {
        message[9] = '\0';
      }
    } else {
      // Serial.print("\"");
      // Serial.print(message);
      // Serial.println("\"");
      char* d = strtok(message, "-");
      while (d != NULL) {
        j == 0 ? (x = String(d).toInt()) : (y = String(d).toInt());
        d = strtok(NULL, "-");
        j += 1;
        Serial.println("X: " + String(x) + " Y: " + String(y));
      }
      memset(message, '\0', 10);
      i = 0;
      j = 0;
    }
  }
}
