#ifndef TRACKER_MODULE_H
#define TRACKER_MODULE_H

#include "driver/gpio.h"

volatile bool tracker_detected;
void init_tracker(void);

#endif