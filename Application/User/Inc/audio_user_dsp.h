#if !defined(__AUDIO_USER_DSP_H__)
#define __AUDIO_USER_DSP_H__

#include <stdint.h>

void AudioUserDsp_ApplyFilterToSamples(int8_t* dataPointer, uint32_t dataLength, int16_t (*leftChannelFilter)(int16_t), int16_t (*rightChannelFilter)(int16_t));
int16_t AudioUserDsp_ChangeAmplitude(int16_t sample, uint16_t amplitude);

#endif // __AUDIO_USER_DSP_H__
