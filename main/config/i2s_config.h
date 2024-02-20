#ifndef I2S_CONFIG_H
#define I2S_CONFIG_H

#include "driver/i2s.h"
#include "esp_intr_alloc.h"
#include "freertos/FreeRTOS.h"

#define I2S_MIC_WS 4
#define I2S_MIC_SD 6
#define I2S_MIC_SCK 5

#define I2S_SAMPLE_RATE 8000
#define I2S_DMA_BUFFER_LENGTH 1024
#define I2S_DMA_BUFFER_COUNT 4

extern i2s_port_t I2S_MIC_PORT;
extern i2s_pin_config_t I2S_MIC_PINS;
extern i2s_config_t I2S_MIC_CONFIG;


#endif