#include "signal_handlers.h"

// static const char *TAG = "Signal Handlers";


/// TODO: convert to kissfftr when have time
kiss_fft_cpx* sig_applyFourierTransform(int16_t* sampleVector, int numSamples) 
{
  kiss_fft_cfg cfg = kiss_fft_alloc(numSamples, 0, NULL, NULL);
  if (!cfg) return NULL;

  kiss_fft_cpx* cx_in = (kiss_fft_cpx*)malloc(numSamples * sizeof(kiss_fft_cpx)); 
  if (!cx_in) return NULL;

  kiss_fft_cpx* cx_out = (kiss_fft_cpx*)malloc(numSamples * sizeof(kiss_fft_cpx));
  if (!cx_out) return NULL;
    
  for (int i = 0; i < numSamples; i++) {
    cx_in[i].r = (float)sampleVector[i];
    cx_in[i].i = 0.0f;
  }

  kiss_fft(cfg, cx_in, cx_out); // DO FFT
  
  free(cx_in);
  kiss_fft_free(cfg);
  return cx_out;
}


int16_t* sig_applyHanningWindow(int16_t * inputSignalVector, int vectorLength) 
{  
  int16_t* windowedSignal = (int16_t *)malloc(vectorLength * sizeof(int16_t));
  
  for (int i = 0; i < vectorLength; i++) { 
    float alpha = 0.5 * (1 - cos(2 * M_PI * i / (vectorLength - 1)));
    int16_t conv = (int16_t)(inputSignalVector[i] * alpha);
    windowedSignal[i] = conv;
  }
  return windowedSignal;
}


int sig_frequencyWithHighestMagnitude(kiss_fft_cpx* vector, int numSamples, int sampleRate) 
{ 
  int highestFreq = 0;
  float highestMag = 0;
  
  for (int i = 0; i < numSamples; i++) {
    int freq = i * sampleRate / numSamples;
    float mag = fabs(vector[i].r);

    if (mag > highestMag) {
      highestMag = mag;
      highestFreq = freq;
    }
  }
  return highestFreq;
}
