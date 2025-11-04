#include <Arduino.h>
#include "soc/gpio_num.h"

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

#define LED_PIN GPIO_NUM_11
#define TASK_DELAY_1000 1000
#define TASK_DELAY_500 500
#define TASK_DELAY_300 300
#define TASK_DELAY_200 200

// Challenge, blink external led at different rates
void toggleLED1000(void *parameter) {
  while (true) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    vTaskDelay(TASK_DELAY_1000 / portTICK_PERIOD_MS);
  }
}

void toggleLED500(void *parameter) {
  while (true) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    vTaskDelay(TASK_DELAY_500 / portTICK_PERIOD_MS);
  }
}

void toggleLED300(void *parameter) {
  while (true) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    vTaskDelay(TASK_DELAY_300 / portTICK_PERIOD_MS);
  }
}

void toggleLED200(void *parameter) {
  while (true) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    vTaskDelay(TASK_DELAY_200 / portTICK_PERIOD_MS);
  }
}

// Experiment: another task for PIN_RGB_LED (8) in ESP32-C3
void toggleLEDBuiltin(void *parameter) {
  while (true) {
    rgbLedWrite(PIN_RGB_LED, 10, 10, 10);
    vTaskDelay(TASK_DELAY_500 / portTICK_PERIOD_MS);
    rgbLedWrite(PIN_RGB_LED, 0, 0, 0);
    vTaskDelay(TASK_DELAY_500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  pinMode(PIN_RGB_LED, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  // Challenge, blink external led at different rates
  // Task to run forever
  xTaskCreatePinnedToCore(
      toggleLED1000,
      "Toggle LED1000",
      1024,
      NULL,
      1,
      NULL,
      app_cpu);

  xTaskCreatePinnedToCore(
      toggleLED500,
      "Toggle LED500",
      1024,
      NULL,
      1,
      NULL,
      app_cpu);

  xTaskCreatePinnedToCore(
      toggleLED300,
      "Toggle LED300",
      1024,
      NULL,
      1,
      NULL,
      app_cpu);

  xTaskCreatePinnedToCore(
      toggleLED200,
      "Toggle LED200",
      1024,
      NULL,
      1,
      NULL,
      app_cpu);

  // Experiment: another task for PIN_RGB_LED (8) in ESP32-C3
  xTaskCreatePinnedToCore(
      toggleLEDBuiltin,
      "Toggle LED Builtin",
      1024,
      NULL,
      1,
      NULL,
      app_cpu);
}

void loop() {
  // Do nothing
}
