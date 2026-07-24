#include "tracker_module.h"
#include "esp_attr.h"

volatile bool tracker_detected = false;

static void IRAM_ATTR trackerISR(void *arg) {
    tracker_detected = true;
}

void init_tracker(void) {
    gpio_config_t io_config = {
        .pin_bit_mask = (1ULL << GPIO_NUM_48),
        .mode = GPIO_MODE_INPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE,
    };

    gpio_config(&io_config);

    gpio_isr_handler_add(GPIO_NUM_48, trackerISR, NULL);
}
