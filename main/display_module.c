#include "display_module.h"
#include "esp_log.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_ili9341.h" 
#include "driver/spi_master.h"

static const char *TAG = "DISPLAY_MODULE";

#define TFT_HOST       SPI2_HOST 
#define TFT_MOSI       41
#define TFT_SCLK       42
#define TFT_CS         38
#define TFT_DC         40
#define TFT_RST        39
#define TFT_PIXEL_CLOCK_HZ (40 * 1000 * 1000) 

static esp_lcd_panel_handle_t painel_tft = NULL;

void init_display(void) {
    ESP_LOGI(TAG, "Configurando o barramento SPI...");
    spi_bus_config_t buscfg = {
        .sclk_io_num = TFT_SCLK,
        .data0_io_num = TFT_MOSI,
        .data1_io_num = -1, 
        .data2_io_num = -1,
        .data3_io_num = -1,
        .max_transfer_sz = 320 * 240 * 2 + 8
    };
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

void draw_frame(const void *buf) {
    if (painel_tft) {
        esp_lcd_panel_draw_bitmap(painel_tft, 0, 0, 320, 240, buf);
    }
}