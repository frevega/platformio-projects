/* platformio.ini config
    https://community.platformio.org/t/esp32-s3-devkitc-1-n32r8v-configuration-assistance/31967/15
    
    Take a look on
    https://github.com/platformio/platform-espressif32/issues/837
*/

#include <Arduino.h>

// #define CONFIG_SPIRAM_SUPPORT
#define LED_PIN GPIO_NUM_38

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  neopixelWrite(LED_PIN, 1, 1, 1);
  delay(250);
  neopixelWrite(LED_PIN, 0, 0, 0);
  delay(250);
  Serial.println("Serial output");
}
