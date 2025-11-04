#include <Arduino.h>

#if CONFING_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Settings
static const uint8_t msg_queue_len = 5;

// Globals
static QueueHandle_t msg_queue;

void simple_messages(void *parameter) {
  int item;

  while (true) {
    // See if there's a message in the queue (do not block)
    if (xQueueReceive(msg_queue, (void *)&item, 0) == pdTRUE) {
      // Serial.println(item);
    }
    Serial.println(item);

    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  Serial.println("\n ----- FreeRTOS Queue Demo -----");

  // Queue
  msg_queue = xQueueCreate(msg_queue_len, sizeof(int));

  // Task
  xTaskCreatePinnedToCore(
    simple_messages,
    "Task_1",
    2048,
    NULL,
    1,
    NULL,
    app_cpu
  );
}

void loop() {
  static int num = 0;

  // Try to add item to queue for 10 ticks, fail if queue is full
  if (xQueueSend(msg_queue, (void *)&num, 10) != pdTRUE) {
    Serial.println("Queue full");
  }
  num += 1;

  vTaskDelay(1000 / portTICK_PERIOD_MS);
}