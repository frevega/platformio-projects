#include <Arduino.h>

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

char *ptr = NULL;
bool flag = false;

// Wait for user input
void checkForUserInputTask(void *parameter) {
  Serial.println("Enter a string");
  while (true) {
    if (Serial.available()) {
      // Print out the number of free heap memory bytes before malloc
      Serial.printf("Heap before malloc (bytes): %zu\n", xPortGetFreeHeapSize());

      String userInput = Serial.readStringUntil('\n');
      // Serial.printf("User input: %s\n", userInput);
      // Serial.printf("userInput.length(): %d\n", userInput.length());
      // Serial.printf("sizeof(userInput): %zu\n", sizeof(userInput));
      Serial.printf("strlen(userInput.c_str()): %zu\n", strlen(userInput.c_str()));

      ptr = (char *) pvPortMalloc(strlen(userInput.c_str()) + 1);
      if (ptr == NULL) {
        Serial.println("\n******* NOT ENOUGH HEAP *******");
        Serial.println("******* Restarting Board ******\n");
        ESP.restart();
      } else {
        strcpy(ptr, userInput.c_str());
        // Serial.printf("User input: %s, address: %p\n", ptr, (void *)ptr);
        Serial.printf("ptr address: %p\n", ptr, (void *)ptr);
      }

      // Print out the number of free heap memory bytes before malloc
      Serial.printf("Heap after malloc (bytes): %zu\n", xPortGetFreeHeapSize());

      // Set flag true to show user input and free memory in second task
      flag = true;
    }
  }
}

// Print user input and free memory
void printUserInputAndFreeMemTask(void *parameter) {
  while (true) {
    if (flag == true) {
      Serial.println("--- task_2 ---");
      Serial.printf("User input: %s, address: %p\n", ptr, (void *)ptr);

      // Print out the number of free heap memory bytes before malloc
      Serial.printf("Heap before free (bytes): %zu\n", xPortGetFreeHeapSize());
      
      // Free up allocated memory
      vPortFree(ptr);
      ptr = NULL;

      // Print out the number of free heap memory bytes before malloc
      Serial.printf("Heap after free (bytes): %zu\n\n", xPortGetFreeHeapSize());

      // Set flag false to stop second task
      flag = false;
      Serial.println("Enter a string");
    }

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  Serial.println("\n--- Part 04 Memory Demo Challenge ---");

  // Start tasks
  xTaskCreatePinnedToCore(
      checkForUserInputTask,    // Task function
      "task_1",                 // Name of the task
      2048,                     // Stack size in bytes
      NULL,                     // Parameter passed to the task
      1,                        // Priority of the task
      NULL,                     // Task handle
      app_cpu                   // Core where the task should run
  );

  xTaskCreatePinnedToCore(
      printUserInputAndFreeMemTask, // Task function
      "task_2",                     // Name of the task
      2048,                         // Stack size in bytes
      NULL,                         // Parameter passed to the task
      1,                            // Priority of the task
      NULL,                         // Task handle
      0                             // Core where the task should run
  );

  // Delete "setup and "loop" task
  vTaskDelete(NULL);
}

void loop() {
  // Do nothing here
  Serial.println("Looping...");
}