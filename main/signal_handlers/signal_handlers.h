#ifndef SIGNAL_HANDLERS_H
#define SIGNAL_HANDLERS_H

#include "esp_log.h"
#include "esp_err.h"
#include "classes.h"
#include "../kissfft/kiss_fft.h"

/// If you arn't downsampling or just getting a single read from your mic, your numSamples will be e.g. BUFFER_OF_BUFFERS_LENGTH
/// Remember to free() whatever your cx_out is returned to in your main code body.
kiss_fft_cpx* sig_applyFourierTransform(int16_t* sampleVector, int numSamples);
int sig_frequencyWithHighestMagnitude(kiss_fft_cpx* vector, int numSamples, int sampleRate);
int16_t* sig_applyHanningWindow(int16_t * inputSignalVector, int vectorLength);

#endif