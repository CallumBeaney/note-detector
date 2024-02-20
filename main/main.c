#include <stdio.h>

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"

#include "../config/i2s_config.h"
#include "classes.h"
#include "i2s_mic.h"
#include "signal_handlers.h"
#include "notes.h"


void mainTask(void * params);
int listenToMic(i2s_port_t port, int sampleRate, int samplesLen) ;

// local variables
static TaskHandle_t mainTaskHandler = NULL; 


void app_main(void)
{
    ESP_ERROR_CHECK(mic_init(I2S_MIC_PORT, I2S_MIC_PINS, I2S_MIC_CONFIG));  

    xTaskCreate(&mainTask, "mainTask", 2048, NULL, 2, &mainTaskHandler);

    // ESP_ERROR_CHECK(mic_deinit(I2S_MIC_PORT));     

    // printf("Restarting now.\n");
    // fflush(stdout);
    // esp_restart();
}



void mainTask(void * params) {

  while (1) 
  {
    int dominantFrequency = listenToMic(I2S_MIC_PORT, I2S_SAMPLE_RATE, I2S_DMA_BUFFER_LENGTH);
    const char* noteData = note_parseNote(dominantFrequency);
    // todo: OLED Screen or something idk
    fprintf(stdout, "%s", noteData);
  }
}



int listenToMic(i2s_port_t port, int sampleRate, int samplesLen) 
{
  const char* TAG = "listenToMic";
  
  int16_t* audioSamples = mic_allocateHeapBuffer(samplesLen);
  int samplesRead = mic_read(port, audioSamples, samplesLen);

  if (audioSamples == NULL) {
    fprintf(stdout, "Audio samples buffer NULL after sampling microphone.\n");
    return 0;
  }

  int16_t* windowedSamples = sig_applyHanningWindow(audioSamples, samplesLen);
  free(audioSamples);

  kiss_fft_cpx* frequencyDomain = sig_applyFourierTransform(windowedSamples, samplesLen);  
  free(windowedSamples);

  if (frequencyDomain == NULL) {
    fprintf(stdout, "Pointer to frequency domain buffer NULL after applying FFT to audio samples.\n");
    // ESP_LOGI("LISTENING FOR HORNETS", "Pointer to frequency domain buffer NULL after applying FFT to audio samples.\n");
    return 0;
  }

  int dominantFrequency = sig_frequencyWithHighestMagnitude(frequencyDomain, samplesLen, sampleRate);
  free(frequencyDomain);  

  return dominantFrequency;
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
