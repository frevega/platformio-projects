#include <Arduino.h>

/**
 * FreeRTOS Task Demo
 * 
 * Toggles LED and prints "Hello, Wolrd" to console in separate tasks.alignas
 */

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

const char msg[] = "Barkadeer brig Arr booty rum.";

// Task handles
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

// Tasks

// Task: print to Serial Terminal with lower priority
void startTask1(void *parameter) {
  /* Serial.print("startTask1 running on core ");
  Serial.print(xPortGetCoreID());
  Serial.print(" with priority ");
  Serial.println(uxTaskPriorityGet(NULL)); */

  // Count numeber of characters in string
  int msg_len = strlen(msg);

  // Print string to terminal
  while (true) {
    Serial.println();
    for (int i = 0; i < msg_len; i += 1) {
      Serial.print(msg[i]);
    }
    Serial.println();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Task: print to Serial Terminal with higher priority
void startTask2(void *parameter) {
  /* Serial.print("startTask2 running on core ");
  Serial.print(xPortGetCoreID());
  Serial.print(" with priority ");
  Serial.println(uxTaskPriorityGet(NULL)); */
  
  while (true) {
    Serial.print(xPortGetCoreID());
    // Serial.print("*");
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(300);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("\n--- FreeRTOS Task Demo ---");

  // Print self priority
  Serial.print("Setup and loop task running on core ");
  Serial.print(xPortGetCoreID());
  Serial.print(" with priority ");
  Serial.println(uxTaskPriorityGet(NULL));

  // Task to run forever
  xTaskCreatePinnedToCore(
    startTask1,       /* Task function */
    "Task 1",         /* name of task */
    1024,             /* Stack size in bytes */
    NULL,             /* parameter passed as input to the task */
    1,                /* priority of the task */
    &task_1,          /* Task handle to keep track of created task */
    tskNO_AFFINITY    /* pin task to core */
    // app_cpu
  );

  // Task to run once with higher priority
  xTaskCreatePinnedToCore(
    startTask2,       /* Task function */
    "Task 2",         /* name of task */
    1024,             /* Stack size in bytes */
    NULL,             /* parameter passed as input to the task */
    2,                /* priority of the task */
    &task_2,          /* Task handle to keep track of created task */
    tskNO_AFFINITY    /* pin task to core */
    // app_cpu
  );
}

void loop() {
  // Suspend the higher priority task for some intervals
  for (int i = 0; i < 3; i += 1) {
    vTaskSuspend(task_2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    vTaskResume(task_2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }

  // Delete the lower priority task
  if (task_1 != NULL) {
    vTaskDelete(task_1);
    task_1 = NULL;
  }
}