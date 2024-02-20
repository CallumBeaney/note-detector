#include "i2s_config.h"

i2s_port_t I2S_MIC_PORT = I2S_NUM_0;

i2s_pin_config_t I2S_MIC_PINS = {
  .bck_io_num = I2S_MIC_SCK,
  .ws_io_num = I2S_MIC_WS,
  .data_out_num = I2S_PIN_NO_CHANGE,
  .data_in_num = I2S_MIC_SD,
};

i2s_config_t I2S_MIC_CONFIG = {
  .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
  .sample_rate = (uint32_t) I2S_SAMPLE_RATE,
  .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
  .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
  .communication_format = I2S_COMM_FORMAT_STAND_I2S,
  .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
  .dma_buf_count = I2S_DMA_BUFFER_COUNT,
  .dma_buf_len = I2S_DMA_BUFFER_LENGTH,
  .use_apll = false, /* "Audio Phase-Locked Loop" */
  .tx_desc_auto_clear = false, /* helps in avoiding noise in case of underflow/data unavailability */
  .fixed_mclk = 0,
};
