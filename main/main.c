#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include "camera_module.h"
#include "display_module.h"
#include "tracker_module.h"
#include "serial_task.h"

SemaphoreHandle_t camera_mutex;
static const char *TAG = "MAIN";

void app_main(void)
{
    vTaskDelay(pdMS_TO_TICKS(3000));

    gpio_install_isr_service(0);
    camera_mutex = xSemaphoreCreateMutex();

    init_display();
    init_tracker();

    if (init_camera() != ESP_OK) {
        ESP_LOGE(TAG, "Camera failed");
        return;
    }

    init_serial_task();

    while(1) {
        if (xSemaphoreTake(camera_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
            camera_fb_t *fb = esp_camera_fb_get();
            if (fb) {
                draw_frame(fb->buf);
                esp_camera_fb_return(fb);
            }
            xSemaphoreGive(camera_mutex);
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}