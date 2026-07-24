#include <stdio.h>
#include "esp_log.h"
#include "esp_camera.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_ili9341.h" 
#include "driver/spi_master.h"

static const char *TAG = "CAMERA_TFT";

//camera pins
static camera_config_t camera_config = {
    .pin_pwdn = -1,  .pin_reset = -1, .pin_xclk = 15,
    .pin_sscb_sda = 4, .pin_sscb_scl = 5,
    .pin_d7 = 16, .pin_d6 = 17, .pin_d5 = 18, .pin_d4 = 12,
    .pin_d3 = 10, .pin_d2 = 8,  .pin_d1 = 9,  .pin_d0 = 11,
    .pin_vsync = 6, .pin_href = 7, .pin_pclk = 13,
    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0, .ledc_channel = LEDC_CHANNEL_0,
    
    // QVGA (320x240) resolution
    .pixel_format = PIXFORMAT_RGB565, 
    .frame_size = FRAMESIZE_QVGA,     
    .jpeg_quality = 12,
    .fb_count = 2,
    .grab_mode = CAMERA_GRAB_WHEN_EMPTY
};

//tft screen pins
#define TFT_HOST       SPI2_HOST // channel SPI 2 ESP32-S3
#define TFT_MOSI       41
#define TFT_SCLK       42
#define TFT_CS         38
#define TFT_DC         40
#define TFT_RST        39
#define TFT_PIXEL_CLOCK_HZ (40 * 1000 * 1000) 

esp_lcd_panel_handle_t painel_tft = NULL;

//screen initialization
void init_tela(void) {
    ESP_LOGI(TAG, "Configurando o barramento SPI...");
    spi_bus_config_t buscfg = {
        .sclk_io_num = TFT_SCLK,
        .data0_io_num = TFT_MOSI,
        .data1_io_num = -1, 
        .data2_io_num = -1,
        .data3_io_num = -1,
        .max_transfer_sz = 320 * 240 * 2 + 8
    };
    // spi bus with DMA init
    spi_bus_initialize(TFT_HOST, &buscfg, SPI_DMA_CH_AUTO);

    ESP_LOGI(TAG, "Instalando o painel ILI9341...");
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = TFT_DC,
        .cs_gpio_num = TFT_CS,
        .pclk_hz = TFT_PIXEL_CLOCK_HZ,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        .spi_mode = 0,
        .trans_queue_depth = 10,
    };
    esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)TFT_HOST, &io_config, &io_handle);

    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = TFT_RST,
        .rgb_endian = LCD_RGB_ENDIAN_BGR,
        .bits_per_pixel = 16,
    };
    esp_lcd_new_panel_ili9341(io_handle, &panel_config, &painel_tft);
    
    esp_lcd_panel_reset(painel_tft);
    esp_lcd_panel_init(painel_tft);
    esp_lcd_panel_swap_xy(painel_tft, true); 
    esp_lcd_panel_mirror(painel_tft, false, true); 
    esp_lcd_panel_disp_on_off(painel_tft, true);
}

void app_main(void)
{
    vTaskDelay(pdMS_TO_TICKS(3000));

    //turn the screen on
    init_tela();

    //turn the cam on
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha na câmera: 0x%x", err);
        return;
    }

    while(1) {
        camera_fb_t *fb = esp_camera_fb_get();
        if (!fb) {
            continue;
        }

        esp_lcd_panel_draw_bitmap(painel_tft, 0, 0, 320, 240, fb->buf);

        esp_camera_fb_return(fb);
    }
}