#include <Arduino.h>

#if CONFING_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static QueueHandle_t led_blink_delay_queue;
static QueueHandle_t led_blink_update_queue;

// Blink led task handle
static TaskHandle_t blink_t = NULL;

uint8_t char_size = 20;

// Reads from led_blink_update_queue & prints it
void processLedBlinkUpdateQueue(void *parameter)
{
  char blink_message[char_size] = "";

  while (true) {
    // Read messages from 2nd queue (quantity)
    if (uxQueueMessagesWaiting(led_blink_update_queue) > 0 && xQueueReceive(led_blink_update_queue, (void *)&blink_message, 0) == pdTRUE) {
      Serial.println(blink_message);
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

// Checks for user input, echoes it back & post valid blinking rates to led_blink_delay_queue
void checkForUserInputTask(void *parameter)
{
  char cmd[] = "delay ";
  uint8_t cmd_len = strlen(cmd);
  uint8_t buff_len = 255;
  char buff[buff_len];
  memset(buff, 0, buff_len);
  char *p;
  long delay;

  Serial.println("Enter command 'delay xxxx', where xxx is your blink rate in milliseconds (greater than 0)");
  
  while (true) {
    if (Serial.available()) {
      Serial.readBytesUntil('\n', buff, buff_len);
      if (strstr(buff, cmd) != NULL) {
        // char *tail = &buff[cmd_len];
        // Serial.printf("tail: %s, abs(atoi(tail)): %d\n", tail, abs(atoi(tail)));
        delay = abs(strtol(&buff[cmd_len], &p, 10));
        if (/* *p == '\0' */!*p && (delay > 0 && delay && errno != ERANGE)) {
          // Put message in 1st queue (do not block)
          if (uxQueueSpacesAvailable(led_blink_delay_queue) > 0 && xQueueSend(led_blink_delay_queue, (void *)&delay, 0) != pdTRUE) {
            Serial.println("led_blink_delay_queue full");
          }
        }
      }
      
      errno = 0;
      Serial.println(buff);
      memset(&buff, 0, buff_len);
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

// Updates led blink rate from led_blink_delay_queue & post to led_blink_update_queue every time led blink 100 times
void updateLedBlinkRateTask(void *parameter) 
{
  static int blink_rate = 1000;
  char blink_message[char_size] = "";
  static int blink_counter = 0;

  while (true)
  {
    digitalWrite(GPIO_NUM_26, !digitalRead(GPIO_NUM_26));
    blink_counter += 1;

    if (blink_counter > 0 && blink_counter % 200 == 0) {
      sprintf(blink_message, "Blinked %d", blink_counter / 2);
      
      if (uxQueueSpacesAvailable(led_blink_update_queue) > 0 && xQueueSend(led_blink_update_queue, (void *)&blink_message, 0) != pdTRUE) {
        Serial.println("led_blink_update_queue full");
      }
    }

    // See if there's a message in the queue (do not block) and update blink_rate
    if (uxQueueMessagesWaiting(led_blink_delay_queue) > 0 && xQueueReceive(led_blink_delay_queue, (void *)&blink_rate, 0) == pdTRUE) {
      Serial.printf("Update blink rate to %d milliseconds\n", blink_rate);
    }
    
    vTaskDelay(blink_rate / portTICK_PERIOD_MS);
  }
}

void setup()
{
  pinMode(GPIO_NUM_26, OUTPUT);
  digitalWrite(GPIO_NUM_26, LOW);
  Serial.begin(115200);
  vTaskDelay(3000 / portTICK_PERIOD_MS);
  Serial.println("\n ----- Part 05 Challenge -----");

  // Queues
  static const uint8_t queue_len = 100;
  led_blink_delay_queue = xQueueCreate(queue_len, sizeof(int));
  led_blink_update_queue = xQueueCreate(queue_len, sizeof(char[char_size]));

  // Reads from led_blink_update_queue & prints it
  xTaskCreatePinnedToCore(processLedBlinkUpdateQueue, "Task_0", 2048, NULL, 1, NULL, app_cpu);

  // Checks for user input, echoes it back & post valid blinking rates to led_blink_delay_queue
  xTaskCreatePinnedToCore(checkForUserInputTask, "Task_1", 2048, NULL, 1, NULL, app_cpu);

  // Updates led blink rate from led_blink_delay_queue & post to led_blink_update_queue every time led blink 100 times
  xTaskCreatePinnedToCore(updateLedBlinkRateTask, "Task_2", 2048, NULL, 1, NULL, 0);

  vTaskDelete(NULL);
}

void loop()
{
  // Unreachable code
}