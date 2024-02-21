#ifndef I2S_MIC_H
#define I2S_MIC_H

#include <string.h>
#include <math.h>

#include <driver/i2s.h>
#include <esp_log.h>
#include <esp_err.h>


/// @brief Convenience wrapper to initialise I2S microphone. Pass in pre-defined config & pins from your config header file.
esp_err_t mic_init(i2s_port_t port, i2s_pin_config_t pins, i2s_config_t config);

esp_err_t mic_deinit(i2s_port_t port);

/// @brief get samples from the microphone using DMA.
/// @param samples This will be passed into and written to by i2s_read().
/// @param count the number of samples to write.
/// @return the number of samples read.
int mic_read(i2s_port_t port, int16_t *samples, int count);

/// @return returns a pointer to a zeroed array that must be manually freed
int16_t* mic_allocateHeapBuffer(int sizeOfBuffer);

// int16_t* mic_readDMAmicInputIntoBufferTillFull(i2s_port_t port, int16_t* buffer, int repeatCount, int micInputBufferLength, bool applyWindow);



#endif