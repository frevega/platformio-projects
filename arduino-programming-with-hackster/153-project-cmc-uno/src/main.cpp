#include <Arduino.h>

#define GREEN_LED 11
#define RED_LED 12

String strValue = "";
bool isGoingUp = false;

void makeLedBlinky(int LED);

void setup() {
  Serial.begin(115200);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
}

void loop() {
  if (Serial.available() > 0) {
    strValue = Serial.readString();
    Serial.println(strValue);
    if (strValue.indexOf("1h: ") >= 0) {
      strValue = strValue.substring(strValue.indexOf("1h: "), strValue.indexOf("24h: ") -1);
      strValue.remove(0, 4);
      isGoingUp = strValue.toFloat() > 0;
      makeLedBlinky(isGoingUp ? GREEN_LED : RED_LED);
      digitalWrite(isGoingUp ? RED_LED : GREEN_LED, LOW);
      digitalWrite(isGoingUp ? GREEN_LED : RED_LED, HIGH);
    }
  }

  // val = Serial.parseInt();

  // if (val == 7) {
  //   digitalWrite(RED_LED, HIGH);
  //   digitalWrite(GREEN_LED, LOW);
  // } else if (val == 14) {
  //   digitalWrite(RED_LED, LOW);
  //   digitalWrite(GREEN_LED, HIGH);
  // } else if (val == 24) {
  //   digitalWrite(RED_LED, LOW);
  //   digitalWrite(GREEN_LED, LOW);
  // }
}

void makeLedBlinky(int LED) {
  for (int i = 0; i < 10; i += 1) {
    digitalWrite(LED, !digitalRead(LED));
    delay(30);
  }
}