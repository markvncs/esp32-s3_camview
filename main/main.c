#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "camera_module.h"
#include "display_module.h"

static const char *TAG = "MAIN";

void app_main(void)
{
    vTaskDelay(pdMS_TO_TICKS(3000));

    init_display();

    if (init_camera() != ESP_OK) {
        ESP_LOGE(TAG, "Camera failed");
        return;
    }

    while(1) {
        camera_fb_t *fb = esp_camera_fb_get();
        if (!fb) {
            continue;
        }

        draw_frame(fb->buf);

        esp_camera_fb_return(fb);
    }
}