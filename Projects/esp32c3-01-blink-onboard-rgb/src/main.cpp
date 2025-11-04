#include <Arduino.h>

#define MY_SHORT_DELAY 4 // 2
// #define MY_LONG_DELAY 250
#define TIMER_MULTIPLIER 5
#define WARM_LED_TIME_INTERVAL 20 * TIMER_MULTIPLIER
#define COLD_LED_TIME_INTERVAL 40 * TIMER_MULTIPLIER
#define RED_LED_TIME_INTERVAL 60 * TIMER_MULTIPLIER
#define GREEN_LED_TIME_INTERVAL 80 * TIMER_MULTIPLIER
#define BLUE_LED_TIME_INTERVAL 100 * TIMER_MULTIPLIER
#define RGB_PINS_ARRAY_LENGTH 2 //5
#define RGB_PINS_ARRAY_INIT_LENGTH 2 //5
#define MAX_LED_BRIGHTNESS 32 //255

#define RED_PIN 3
#define GREEN_PIN 4
#define BLUE_PIN 5
#define WARM_PIN 18
#define COLD_PIN 19

byte rgbPins[] = { /*RED_PIN, GREEN_PIN, BLUE_PIN,*/ WARM_PIN, COLD_PIN };
byte counter = 0;
byte currentLedBrightness = 0;
bool isDecreasingBrightness = false;
unsigned long warmLedPrevTimeInterval = 0, coldLedPrevTimeInterval = 0, redLedPrevTimeInterval = 0, greenLedPrevTimeInterval = 0, blueLedPrevTimeInterval = 0;

void setup() {
  for (int pin_pos = 0; pin_pos < RGB_PINS_ARRAY_INIT_LENGTH; pin_pos += 1) {
    pinMode(rgbPins[pin_pos], OUTPUT);
    if (pin_pos > 2) {
      digitalWrite(rgbPins[pin_pos], LOW);
    }
  }
  Serial.begin(115200);
}

void serialOutput(byte pin_pos, byte brightness, byte counter);
void individualLeds();
void ledsArray();

void loop() {
  ledsArray();
  Serial.println(counter);
  counter += 1;
  // individualLeds();
}

void individualLeds() {
  if (millis() - redLedPrevTimeInterval >= RED_LED_TIME_INTERVAL) {
    redLedPrevTimeInterval = millis();
    digitalWrite(RED_PIN, !digitalRead(RED_PIN));
  }

  if (millis() - greenLedPrevTimeInterval >= GREEN_LED_TIME_INTERVAL) {
    greenLedPrevTimeInterval = millis();
    digitalWrite(GREEN_PIN, !digitalRead(GREEN_PIN));
  }

  if (millis() - blueLedPrevTimeInterval >= BLUE_LED_TIME_INTERVAL) {
    blueLedPrevTimeInterval = millis();
    digitalWrite(BLUE_PIN, !digitalRead(BLUE_PIN));
  }

  if (millis() - warmLedPrevTimeInterval >= WARM_LED_TIME_INTERVAL) {
    warmLedPrevTimeInterval = millis();
    digitalWrite(WARM_PIN, !digitalRead(WARM_PIN));
  }

  if (!digitalRead(18) && millis() - coldLedPrevTimeInterval >= COLD_LED_TIME_INTERVAL) {
    coldLedPrevTimeInterval = millis();
    digitalWrite(COLD_PIN, !digitalRead(COLD_PIN));
  }
}

void ledsArray() {
  for (int i = 0; i < RGB_PINS_ARRAY_LENGTH; i += 1) {
    for (int j = 0; j <= MAX_LED_BRIGHTNESS; j += 1) {
      analogWrite(rgbPins[i % RGB_PINS_ARRAY_LENGTH], j);
      delay(MY_SHORT_DELAY);
    }
    for (int j = MAX_LED_BRIGHTNESS; j >= 0; j -= 1) {
      analogWrite(rgbPins[i % RGB_PINS_ARRAY_LENGTH], j);
      delay(MY_SHORT_DELAY);
    }
  }

  // serialOutput(counter % RGB_PINS_ARRAY_LENGTH, currentLedBrightness, counter);
  // analogWrite(rgbPins[counter % RGB_PINS_ARRAY_LENGTH], currentLedBrightness);
  // delay(MY_SHORT_DELAY);
  // if (currentLedBrightness >= MAX_LED_BRIGHTNESS || currentLedBrightness <= 0 && isDecreasingBrightness) {
  //   isDecreasingBrightness = !isDecreasingBrightness;
  // }
  // isDecreasingBrightness ? currentLedBrightness -= 1 : currentLedBrightness += 1;
  // counter += 1;
}

void serialOutput(byte pin_pos, byte brightness, byte counter) {
    Serial.print("Pin: ");
    Serial.print(rgbPins[pin_pos]);
    Serial.print(" Brightness: ");
    Serial.print(brightness);
    Serial.print(" Counter: ");
    Serial.println(counter);
}