#if !defined(__AUDIO_USER_DSP_H__)
#define __AUDIO_USER_DSP_H__

#include <stdint.h>
#include "stdbool.h"

void AudioUserDsp_ApplyFilterToSamples(uint8_t* dataPointer, uint32_t dataLength, int16_t (*leftChannelFilter)(int16_t, uint32_t), int16_t (*rightChannelFilter)(int16_t, uint32_t));
int16_t AudioUserDsp_ChangeAmplitude(int16_t sample, uint16_t amplitude);
int16_t AudioUserDsp_LowPassFilter(int16_t sample, uint32_t iteration);

#endif // __AUDIO_USER_DSP_H__
