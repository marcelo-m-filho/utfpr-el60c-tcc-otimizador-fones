#if !defined(__AUDIO_USER_DSP_H__)
#define __AUDIO_USER_DSP_H__

#include <stdint.h>
#include "stdbool.h"

#define NUMBER_OF_BANDS 8


typedef struct BiquadFilter {
  float b0, b1, b2, a1, a2;
  float in_z1, in_z2, out_z1, out_z2;
  int32_t gain, frequency, bandwidth;
  bool isInitialized;
} BiquadFilter;

void AudioUserDsp_ApplyFilterToSamples(
    uint8_t* dataPointer, 
    uint32_t dataLength,
    int16_t (*leftChannelFilter)(int16_t, uint8_t), 
    int16_t (*rightChannelFilter)(int16_t, uint8_t),
    uint8_t filterIndex
    );
int16_t AudioUserDsp_ChangeAmplitude(int16_t sample, uint8_t filterIndex);
int16_t AudioUserDsp_LowPassFilter(int16_t sample, uint8_t filterIndex);
int16_t AudioUserDsp_BiquadFilter(int16_t sample, uint8_t filterIndex);
int16_t AudioUserDsp_BiquadFilterConfig(BiquadFilter* filter, int16_t gain, int16_t frequency, int16_t bandwidth);

extern BiquadFilter biquadFilters[NUMBER_OF_BANDS];

#endif // __AUDIO_USER_DSP_H__
