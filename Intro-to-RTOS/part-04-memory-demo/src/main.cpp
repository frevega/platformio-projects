#include <Arduino.h>

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Task: Perform some mundane task
void testTask(void *parameter) {
  while (true) {
    int a = 1;
    int b[125];

    // Do something with array so it's not optimized out by the compiler
    for (int i = 0; i < 125; i += 1) {
      b[i] = a + 1;
    }
    Serial.println(b[0]);

    // Print out remaining stack memory (words)
    // The value returned is in words, not bytes. On ESP32 (32-bit), multiply by 4 to get bytes.
    // It shows the minimum free stack that has ever been available, not the current free stack.
    // It’s a good tool for debugging and tuning stack sizes.
    Serial.print("High water mark (words): ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));

    // Print out the number of free heap memory bytes before malloc
    Serial.print("Heap before malloc (bytes): ");
    Serial.println(xPortGetFreeHeapSize());

    int *ptr = (int*)pvPortMalloc(1024 * sizeof(int));
    if (ptr == NULL) {
      Serial.println("Not enough heap.");
    } else {
      // Do something with the memory so it's not optimized by the compiler
      for (int i = 0; i < 1024; i += 1) {
        ptr[i] = 3;
      }
    }

    // Print out the number of free heap memory bytes before malloc
    Serial.print("Heap after malloc (bytes): ");
    Serial.println(xPortGetFreeHeapSize());

    // Free up our allocated memory
    vPortFree(ptr);

    // Delay to see what its happening
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  Serial.println("\n--- Part 04 Memory Demo ---");

  // Start the only task 
  xTaskCreatePinnedToCore(
      testTask,         // Task function
      "Test Task",      // Name of the task
      1650,            // Stack size in bytes
      NULL,             // Parameter passed to the task
      1,                // Priority of the task
      NULL,             // Task handle
      app_cpu           // Core where the task should run
  );

  // Delete "setup and "loop" task
  vTaskDelete(NULL);
}

void loop() {
  // Do nothing here
  Serial.println("Looping...");
}