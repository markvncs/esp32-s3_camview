#ifndef SERIAL_TASK_H
#define SERIAL_TASK_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "mbedtls/base64.h"

void init_serial_task(void);

#endif