#include "serial_task.h"
#include "esp_log.h"
#include "img_converters.h"
#include "camera_module.h"
#include "tracker_module.h"

extern SemaphoreHandle_t camera_mutex;

static const char *TAG = "SERIAL_TASK";

static void photo_processing_task(void *pvParameters) {
    while(1) {
        if (tracker_detected) {
            tracker_detected = false;

            if (xSemaphoreTake(camera_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
                camera_fb_t *fb = esp_camera_fb_get();
                if (fb) {
                    unsigned char *jpg_buf = NULL;
                    size_t jpg_len = 0;

                    bool converted = fmt2jpg(fb->buf, fb->len, fb->width, fb->height, PIXFORMAT_RGB565, 80, &jpg_buf, &jpg_len);

                    if (converted && jpg_buf != NULL) {
                        ESP_LOGI(TAG, "Sucessfully converted to JPEG: %u bytes", jpg_len);

                        uart_write_bytes(UART_NUM_0, (const char *)jpg_buf, jpg_len);

                        free(jpg_buf);
                    } else {
                        ESP_LOGE(TAG, "Failed to convert to JPEG");
                    }

                    esp_camera_fb_return(fb);
                }
                xSemaphoreGive(camera_mutex);
            }
        } 

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void init_serial_task(void) {
    xTaskCreatePinnedToCore(
        photo_processing_task,  
        "Task_Photo_Serial",       
        4096,                    
        NULL,                     
        5,                       
        NULL,                    
        0               // core 0 will take care of the photo processing to jpeg and send it serial
    );
}