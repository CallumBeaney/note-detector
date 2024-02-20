#ifndef CLASSES_H
#define CLASSES_H

/// @brief once filled with FFT results this struct can then be passed into datahandling/processing functions. 
/// @param lengthOfArrays will be the length of all arrays in this struct.
/// @param frequencies an array of Hz values for the given indexes
/// @param normalised an array of the normalised amplitudes of each given index's frequencies
/// @param amplitudes an array of the amplitudes of each given index's frequencies
typedef struct ProcessedSignalData {
  int* frequencies;
  float* amplitudes;
  int* normalised; 
  int lengthOfArrays;
} ProcessedSignalData;

#endif