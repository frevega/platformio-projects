#include <Arduino.h>

int redPin = 2;
int ledValue = 0;
String userInput;

boolean isStringAllDigits(String input);

void setup() {
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
}

void loop() {
  Serial.println("Enter LED bright value exponent (0-8)");

  while (Serial.available() == 0) {}

  userInput = Serial.readString();
  if (isStringAllDigits(userInput)) {
    if (userInput.toInt() >= 0 && userInput.toInt() <= 8) {
      ledValue = pow(2, userInput.toInt()) - 1;
      analogWrite(redPin, ledValue);
      Serial.println("LED brightness exponent was " + userInput + ", setting value to " + (ledValue == 0 ? "OFF" : ledValue < 255 ? String(ledValue) : "MAX"));
    } else {
      Serial.println("Allowed values for exponent go from 0 to 8, your input was " + userInput);
    }
  } else {
      Serial.println("Only enter numbers, your input was " + userInput);
  }
}

boolean isStringAllDigits(String input) {
  for (int i = 0; i < input.length(); i++) {
    if (!isDigit(input.charAt(i))) {
      return false;
    } 
  }

  return true;
}