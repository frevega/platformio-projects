#include <Arduino.h>
#include "soc/gpio_num.h"

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// ESP32-C6-Devkitc-1
/* #define LED_BUILTIN GPIO_NUM_8
#define LED_PIN GPIO_NUM_11 */
#define LED_BUILTIN GPIO_NUM_25
#define LED_PIN GPIO_NUM_26
#define TASK_DELAY 500

// Our task: blink an LED
/* A task is simply a function that gets called during settu
    or from another task. However we have to tell the scheduler
    about it so that it can prioritize another and time-slice
    it appropiately. For FreeRTOS, the function should return
    nothing, and accepts one void pointer as parameter. This
    will allow you to pass in arguments if needed. However, it
    can sometimes be tricky if your setup or calling task goes
    out of scope, which may remove the memory allocated for the
    argument. For simplicity, we won't pass any arguments here.
*/
void toggleLED(void *parameter) {
  while (true) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    /* (Take a look if Arduino's delay function is not blocking in ESP32).
        * A tick timer is one of the MCU's hardware timers (1 ms in FreeRTOS by default).
        * Use vTaskDelay function, non-blocking and works in vanilla FreRTOS. It
        expects the number of ticks, not ms, that's why is required to divide the ms by
        the tick period for the argument.
    */
    Serial.print(xPortGetCoreID());
    Serial.print(" ");
    Serial.println(digitalRead(LED_PIN) ? "RED\tON" : "RED\tOFF");
    vTaskDelay(TASK_DELAY / portTICK_PERIOD_MS);
  }
}

// Experiment: another task for LED_BUILTIN (8) in ESP32-C6
void toggleLEDBuiltin(void *parameter) {
  while (true) {
    // rgbLedWrite(LED_BUILTIN, random(17), random(17), random(17)); // Randomize RGB values for addressable LED
    // rgbLedWrite(LED_BUILTIN, digitalRead(LED_PIN) ? 0 : 10, 0, 0);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_PIN));
    Serial.print(xPortGetCoreID());
    Serial.print(" ");
    Serial.println(digitalRead(LED_BUILTIN) ? "BLUE\tON" : "BLUE\tOFF");
    vTaskDelay(TASK_DELAY / portTICK_PERIOD_MS);
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  Serial.begin(115200);

  // Task to run forever
  xTaskCreatePinnedToCore( // Use xTaskCreate in vanilla FreeRTOS
      toggleLED,           // Function to be called
      "Toggle LED",        // Name of task
      1024,                // Stack size (bytes in ESP32, words in FreeRTOS)
      NULL,                // Parameter to pass to function
      1,                   // Task priority (o to configMAX_PRIORITIES - 1)
                           // Take a look on configMAX_PRIORITIES (25) in /Users/miguel/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.4-2f7dcd86-v1/esp32c6/include/freertos/config/include/freertos/FreeRTOSConfig.h
      NULL,                // Task handle
      app_cpu);     // Run on one core for demo purposes (ESP32 only)

  // If this was vanilla FreeRTOS, you'd want to call vTaskStartScheduler() in
  // man after setting up your task.

  // Experiment: another task for LED_BUILTIN (8) in ESP32-C6
  xTaskCreatePinnedToCore(
      toggleLEDBuiltin,
      "Toggle LED Builtin",
      1024,
      NULL,
      1,
      NULL,
      app_cpu);

    vTaskDelete(NULL);
}

void loop() {
  // Do nothing
  Serial.println("Looping...");
}
