#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "soc/gpio_num.h"
#include "esp_log.h"
#include "led_strip.h"

#define GPIO_LED_EXTERNAL_PIN GPIO_NUM_11
#define GPIO_LED_EXTERNAL_PIN_SEL (1ULL << GPIO_LED_EXTERNAL_PIN)

static bool led_external_state = true;
static bool led_builtin_state = false;


static led_strip_handle_t led_strip;


static const char *TAG = "example";

static void configure_builtin_led(void)
{
    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {
        .strip_gpio_num = GPIO_NUM_38,
        .max_leds = 1, // at least one LED on board
    };
    
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
        .flags.with_dma = false,
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    
    /* Set all LED off to clear all pixels */
    led_strip_clear(led_strip);
}

// Our task: blink an LED
/* A task is simply a function that gets called during settup,
    or from another task. However we have to tell the scheduler 
    about it so that it can prioritize another and time-slice 
    it appropiately. For FreeRTOS, the function should return
    nothing, and accepts one void pointer as parameter. This
    will allow you to pass in arguments if needed. However, it
    can sometimes be tricky if your setup or calling task goes
    out of scope, which may remove the memory allocated for the
    argument. For simplicity, we won't pass any arguments here.
*/
void toggleLED(void *parameter)
{
    while(1)
    {
        led_external_state = !led_external_state;
        gpio_set_level(GPIO_LED_EXTERNAL_PIN, led_external_state);
        /* (Take a look if Arduino's delay function is not blocking in ESP32).
            * A tick timer is one of the MCU's hardware timers (1 ms in FreeRTOS by default).
            * Use vTaskDelay function, non-blocking and works in vanilla FreRTOS. It 
            expects the number of ticks, not ms, that's why is required to divide the ms by
            the tick period for the argument.
        */
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}

// Experiment: another task for LED_BUILTIN (8) in ESP32-C3
void toggleLEDBuiltin(void *parameter)
{
    while(1)
    {
        led_builtin_state = !led_builtin_state;
        if (led_builtin_state) {
            /* Set the LED pixel using RGB from 0 (0%) to 255 (100%) for each color */
            led_strip_set_pixel(led_strip, 0, 0, 1, 0);
            /* Refresh the strip to send data */
            led_strip_refresh(led_strip);
        } else {
            /* Set all LED off to clear all pixels */
            led_strip_clear(led_strip);
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void setup()
{
    configure_builtin_led();

    gpio_config_t io_conf;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = GPIO_LED_EXTERNAL_PIN_SEL;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_conf);

    // Task to run forever
    xTaskCreatePinnedToCore(    // Use xTaskCreate in vanilla FreeRTOS
                toggleLED,    // Function to be called
                "Toggle LED", // Name of task
                1024,         // Stack size (bytes in ESP32, words in FreeRTOS)
                NULL,         // Parameter to pass to function
                1,            // Task priority (o to configMAX_PRIORITIES - 1)
                                // Take a look on configMAX_PRIORITIES (25) in /Users/miguel/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.4-2f7dcd86-v1/esp32c6/include/freertos/config/include/freertos/FreeRTOSConfig.h
                NULL,         // Task handle
                0);     // Run on one core for demo purposes (ESP32 only)

    // If this was vanilla FreeRTOS, you'd want to call vTaskStartScheduler() in 
    // man after setting up your task.


    // Experiment: another task for LED_BUILTIN (38) in ESP32-S3-DEVKITC-1 (N32R8V)
    xTaskCreatePinnedToCore(
                toggleLEDBuiltin,
                "Toggle LED Builtin",
                1024,
                NULL,
                1,
                NULL, 
                1);
}

void app_main()
{
    setup();

    while (1) 
    {
        ESP_LOGI(TAG, "Turning the EXTERNAL LED %s!", led_external_state == true ? "ON" : "OFF");
        ESP_LOGI(TAG, "Turning the BUILTIN LED %s!", led_builtin_state == true ? "ON" : "OFF");
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}
