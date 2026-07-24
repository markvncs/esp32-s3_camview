#include "camera_module.h"

static camera_config_t camera_config = {
    .pin_pwdn = -1,  .pin_reset = -1, .pin_xclk = 15,
    .pin_sscb_sda = 4, .pin_sscb_scl = 5,
    .pin_d7 = 16, .pin_d6 = 17, .pin_d5 = 18, .pin_d4 = 12,
    .pin_d3 = 10, .pin_d2 = 8,  .pin_d1 = 9,  .pin_d0 = 11,
    .pin_vsync = 6, .pin_href = 7, .pin_pclk = 13,
    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0, .ledc_channel = LEDC_CHANNEL_0,
    
    .pixel_format = PIXFORMAT_RGB565, 
    .frame_size = FRAMESIZE_QVGA,     
    .jpeg_quality = 12,
    .fb_count = 2,
    .grab_mode = CAMERA_GRAB_WHEN_EMPTY
};

esp_err_t init_camera(void) {
    return esp_camera_init(&camera_config);
}