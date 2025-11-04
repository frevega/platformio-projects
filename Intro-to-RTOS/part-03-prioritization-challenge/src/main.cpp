#include <Arduino.h>
#include "soc/gpio_num.h"

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

int led_blinking_duration = 1000;
String userInput = "";

bool validDigits(String input) {
    // Serial.println(input.length());
    // Serial.println(input.charAt(input.length() - 1));
    for (byte i = 0; i < input.length(); i += 1) {
        // Serial.println(isdigit(input.charAt(i)));
        if (!isdigit(input.charAt(i))) {
            return false;
        }
    }
    return true;
}

// Wait for user input
void checkForUserInput(void *parameter) {
    Serial.println("Enter a value in milliseconds to change the LED blinkng rate");
    while (true) {
        if (Serial.available()) {
            userInput = Serial.readString();
            if (validDigits(userInput)) {
                led_blinking_duration = userInput.toInt();
                Serial.println("Blinking rate updated to " + String(led_blinking_duration) + " ms");
            } else {
                Serial.println("Please enter a valid number, user input was: " + userInput);
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }
}

// Change led blinking rate
void changeLEDBlinkingRate(void *parameter) {
    Serial.println(xPortGetCoreID());
    while (true) {
        digitalWrite(GPIO_NUM_11, !digitalRead(GPIO_NUM_11)); // ESP32-C6
        // digitalWrite(GPIO_NUM_25, !digitalRead(GPIO_NUM_25)); // ESP32
        vTaskDelay(led_blinking_duration / portTICK_PERIOD_MS);
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(GPIO_NUM_11, OUTPUT); // ESP32-C6
    // pinMode(GPIO_NUM_25, OUTPUT); // ESP32
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    Serial.println("Part 03 challenge");
      // Task check for user input
    xTaskCreatePinnedToCore(
        checkForUserInput,      // Task function
        "Task 1",               // name of task
        1024,                   // Stack size in bytes
        NULL,                   // parameter passed as input to the task
        1,                      // priority of the task
        NULL,                   // Task handle to keep track of created task
        app_cpu                 // pin task to core
    );

      // Task check for user input
    xTaskCreatePinnedToCore(
        changeLEDBlinkingRate,      // Task function
        "Task 2",                   // name of task
        1024,                       // Stack size in bytes
        NULL,                       // parameter passed as input to the task
        1,                          // priority of the task
        NULL,                       // Task handle to keep track of created task
        app_cpu                     // pin task to core
    );

    vTaskDelete(NULL);
}

void loop() {
    // Nothing to do here
}