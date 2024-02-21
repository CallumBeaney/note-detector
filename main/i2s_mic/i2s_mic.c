#include "i2s_mic.h"



static const char* TAG = "I2S MIC";

static i2s_port_t m_micPort; // store port number locally upon initialisation.



esp_err_t mic_init(i2s_port_t port, i2s_pin_config_t pins, i2s_config_t config) 
{
  esp_err_t ret;
  
  ret = i2s_driver_install(port, &config, 0, NULL);
  if (ret != ESP_OK) return ret;
  vTaskDelay(100 / portTICK_PERIOD_MS);
  
  ret = i2s_set_pin(port, &pins);
  if (ret != ESP_OK) return ret;
  vTaskDelay(100 / portTICK_PERIOD_MS);

  ret = i2s_start(port);
  if (ret != ESP_OK) return ret;

  m_micPort = port;
  return ret;
}

// you can just call the uninstall fn directly
esp_err_t mic_deinit(i2s_port_t port) 
{ 
  esp_err_t ret = i2s_driver_uninstall(m_micPort);
  if (ret != ESP_OK) ESP_ERROR_CHECK_WITHOUT_ABORT(ret);
  return ret;
}


int mic_read(i2s_port_t port, int16_t *samples, int count)
{
  int32_t *raw_samples = (int32_t *)malloc(sizeof(int32_t) * count);
  size_t bytes_read = 0;
  /* esp_err_t result = */ i2s_read(port, raw_samples, sizeof(int32_t) * count, &bytes_read, portMAX_DELAY);

  int samples_read = bytes_read / sizeof(int32_t);
  ESP_LOGI(TAG, "SRead: %d", samples_read);
  for (int i = 0; i < count; i++) {
    samples[i] = (raw_samples[i] & 0xFFFFFFF0) >> 11;
  }

  free(raw_samples);
  return samples_read;
}


int16_t* mic_allocateHeapBuffer(int sizeOfBuffer) 
{
    int16_t* buffer = (int16_t *)malloc(sizeOfBuffer * sizeof(int16_t));
    if (!buffer) {
      // a failed malloc returns null pointer; the only false pointer value.
      ESP_LOGE(TAG, "mic_allocateHeapBuffer: Failed to allocate memory.");
      return NULL;
    }
    memset(buffer, 0, sizeOfBuffer * sizeof(int16_t));
    return buffer;
}


// static void applyHanningWindow(int16_t* inputSignalVector, int vectorLength);
// static void insertOneArrayIntoAnotherAtIndex(int16_t* smallerBuffer, int srcBufferSize, int16_t* destBuffer, int memoryIndex);


// int16_t* mic_readDMAmicInputIntoBufferTillFull(i2s_port_t port, int16_t* buffer, int repeatCount, int micInputBufferLength, bool applyWindow) 
// { 
//   size_t numberOfBytesReadFromDMA = 0;
//   int16_t tempBuffer[micInputBufferLength];

//   for (int i = 0; i < repeatCount; i++) {
//     int memPtr = i * micInputBufferLength; 
//     i2s_read(port, &tempBuffer, micInputBufferLength, &numberOfBytesReadFromDMA, portMAX_DELAY);
    
//     if (applyWindow == true){
//       applyHanningWindow(tempBuffer, micInputBufferLength);
//     }

//     insertOneArrayIntoAnotherAtIndex(tempBuffer, micInputBufferLength, buffer, memPtr);
//   }
//   return buffer;
// }


// /// TODO: maybe write frankenFunction that integrates everything but is faster
// static void applyHanningWindow(int16_t* inputSignalVector, int vectorLength) 
// {
//   for (int i = 0; i < vectorLength; i++) {
//     float alpha = 0.5 * (1 - cos(2 * M_PI * i / (vectorLength - 1)));
//     int result = (int16_t)(inputSignalVector[i] * alpha);
//     inputSignalVector[i] = result;
//   }
//   return;
// }


// static void insertOneArrayIntoAnotherAtIndex(int16_t* smallerBuffer, int srcBufferSize, int16_t* destBuffer, int memoryIndex) 
// {
//     memcpy(destBuffer + memoryIndex, smallerBuffer, srcBufferSize);
// }




