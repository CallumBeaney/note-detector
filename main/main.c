#include <stdio.h>

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"

#include "../config/i2s_config.h"
#include "i2s_mic.h"
#include "signal_handlers.h"
#include "notes.h"


void mainTask(void * params);
void gracefulShutdown(void);
int listenToMic(i2s_port_t port, int sampleRate, int samplesLen);

// local variables
static TaskHandle_t mainTaskHandler = NULL; 
static const char* TAG = "MAIN";

void app_main(void)
{
    esp_log_level_set("*", ESP_LOG_VERBOSE);

    ESP_ERROR_CHECK(
      mic_init(I2S_MIC_PORT, I2S_MIC_PINS, I2S_MIC_CONFIG)
    );

    xTaskCreate(&mainTask, "mainTask", 4096, NULL, 2, &mainTaskHandler);
}



void mainTask(void * params) {

  while (1) 
  {
    int dominantFrequency = listenToMic(I2S_MIC_PORT, I2S_SAMPLE_RATE, I2S_DMA_BUFFER_LENGTH);
    
    int buffLen = 100;
    char* buffer = malloc(sizeof(char) * buffLen);
    if (buffer == NULL) {
      ESP_LOGE(TAG, "Failed to allocate memory for buffer");
      ESP_LOGI(TAG, "FREQUENCY IS: %d", dominantFrequency);
      continue;
    }

    note_parseNote(buffer, buffLen, dominantFrequency);
    ESP_LOGI(TAG, "%s\n", buffer); // todo: OLED Screen or something idk
    free(buffer);

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}


int listenToMic(i2s_port_t port, int sampleRate, int samplesLen) 
{
  const char* MIC = "listenToMic";
  
  int16_t* audioSamples = mic_allocateHeapBuffer(samplesLen);
  /* int samplesRead = */ mic_read(port, audioSamples, samplesLen);
  if (audioSamples == NULL) {
    ESP_LOGE(MIC, "Audio samples buffer NULL after sampling microphone.");
    return 0;
  }

  // int16_t* windowedSamples = sig_applyHanningWindow(audioSamples, samplesLen);
  // free(audioSamples);
  // kiss_fft_cpx* frequencyDomain = sig_applyFourierTransform(windowedSamples, samplesLen);  
  // free(windowedSamples);

  kiss_fft_cpx* frequencyDomain = sig_applyFourierTransform(audioSamples, samplesLen);  
  free(audioSamples);
  

  if (frequencyDomain == NULL) {
    ESP_LOGE(MIC, "Pointer to frequency domain buffer NULL after applying FFT to audio samples.");
    return 0;
  }

  int dominantFrequency = sig_frequencyWithHighestMagnitude(frequencyDomain, samplesLen, sampleRate);
  free(frequencyDomain);  

  return dominantFrequency;
}


void gracefulShutdown(void) 
{ 
  ESP_LOGI(TAG, "Deleting main task.");  
  vTaskDelete(mainTaskHandler);
  ESP_LOGI(TAG, "Uninstalling i2s driver.");  
  ESP_ERROR_CHECK_WITHOUT_ABORT(
    i2s_driver_uninstall(I2S_MIC_PORT)
  );
  ESP_LOGI(TAG, "Restarting now.");
  esp_restart();
}


// void checkDynamicMemory(void) 
// {
//   /// N.B. Some ESP boards don't have non-static IRAM
//   /// Use heap_caps_get_largest_free_block() to get block of free memory; heap frag prevents allocating via heap_caps_GFS()

//   ESP_LOGI(TAG, "xPortGetFreeHeapSize %d = DRAM", xPortGetFreeHeapSize()); /// Alternative way to get heap size

//   /// heap_caps_GFS() takes all regions capable of having the given capabilities allocated in them and adds up the free space they have.
//   int dataRam = heap_caps_get_free_size(MALLOC_CAP_8BIT);
//   int instructionRam = heap_caps_get_free_size(MALLOC_CAP_32BIT) - heap_caps_get_free_size(MALLOC_CAP_8BIT);

//   ESP_LOGI(TAG, "DRAM \t\t %d (%d Kb)", dataRam, dataRam/1024);
//   ESP_LOGI(TAG, "IRam \t\t %d (%d Kb)", instructionRam, instructionRam/1024);
// }
