#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"

#define BLINK_GPIO               48 /* GPIO 48 for ESP32-S3 built-in addressable LED */
#define BLINK_LED_RMT_CHANNEL    0
#define BLINK_PERIOD             1000

static const char *TAG = "blink";
static uint8_t s_led_state = 0;
static led_strip_t *pStrip_a;

int randNext(int left, int right)
{
    static unsigned int seed = 0;
    seed++;
    srand((unsigned) time(NULL) + seed * seed);
    return rand() % (right - left + 1) + left;
}

void app_main(void)
{
    ESP_LOGI(TAG, "Example configured to blink addressable LED!");
    pStrip_a = led_strip_init(BLINK_LED_RMT_CHANNEL, BLINK_GPIO, 1); /* LED strip initialization with the GPIO and pixels number*/
    pStrip_a->set_pixel(pStrip_a, 0, 16, 16, 16); /* LED starts with White to indicate power on */
    vTaskDelay(BLINK_PERIOD / portTICK_PERIOD_MS);
    pStrip_a->clear(pStrip_a, 50);                                          /* Set all LED off to clear all pixels */
    int flag_NetConnect = 0;
    int flag_PcConnect = 0;
    while (1)
    {
        //ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
        pStrip_a->clear(pStrip_a, 50);                                          /* Set all LED off to clear all pixels */
        s_led_state = 2 * flag_NetConnect + 1 * flag_PcConnect;

        switch (s_led_state)
        {
            case 0:
                pStrip_a->set_pixel(pStrip_a, 0, 255, 0, 0);                    //red: 00
                ESP_LOGI(TAG, "Network connection missing, PC connection missing!");
                break;
            case 1:
                pStrip_a->set_pixel(pStrip_a, 0, 0, 255, 0);                    //
                ESP_LOGI(TAG, "Network connection done, PC connection missing!"); 
                break;
            case 2:
                pStrip_a->set_pixel(pStrip_a, 0, 0, 0, 255);                    //
                ESP_LOGI(TAG, "Network connection missing, PC connection done!");
                break;
            case 3:
                pStrip_a->set_pixel(pStrip_a, 0, 255, 255, 0);                  //
                ESP_LOGI(TAG, "Network connection done, PC connection done!");
                break;
        }
        pStrip_a->refresh(pStrip_a, 100);
        // if (s_led_state )                                    /* If the addressable LED is enabled */
        // {
        //     pStrip_a->set_pixel(pStrip_a, 0, 116, 16, 16);  /* Set the LED pixel using RGB from 0 (0%) to 255 (100%) for each color */
        //     pStrip_a->refresh(pStrip_a, 100);               /* Refresh the strip to send data */
        // }
        // else
        // {
        //     pStrip_a->clear(pStrip_a, 50);                  /* Set all LED off to clear all pixels */
        // }

                              /* Toggle the LED state */
        flag_NetConnect = randNext(0,1);

        flag_PcConnect = randNext(0,1);
        

        ESP_LOGI(TAG, "flag_NetConnect = %d, flag_PcConnect = %d", flag_NetConnect,flag_PcConnect);

        vTaskDelay(BLINK_PERIOD / portTICK_PERIOD_MS);
    }
}