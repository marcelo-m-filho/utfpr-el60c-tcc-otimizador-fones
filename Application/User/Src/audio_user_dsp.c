#include "audio_user_dsp.h"

extern uint32_t xDebug[40];

void AudioUserDsp_FrameToSamples(uint8_t*, int16_t* leftSamplePointer, int16_t* rightSamplePointer);
void AudioUserDsp_SamplesToFrame(uint8_t*, int16_t* leftSamplePointer, int16_t* rightSamplePointer);

void AudioUserDsp_ApplyFilterToSamples(uint8_t* dataPointer, uint32_t dataLength, int16_t (*leftChannelFilter)(int16_t), int16_t (*rightChannelFilter)(int16_t))
{
	int16_t leftSample, rightSample;
	for (uint32_t i = 0; i < dataLength; i++)
	{

		uint8_t* framePointer = dataPointer + (4 * i);

		AudioUserDsp_FrameToSamples(framePointer, &leftSample, &rightSample);

		if(leftChannelFilter)
			leftSample = leftChannelFilter(leftSample);

		if(rightChannelFilter)
		rightSample = rightChannelFilter(rightSample);

		AudioUserDsp_SamplesToFrame(framePointer, &leftSample, &rightSample);

		if(i < 20)
		{
			xDebug[2 * i + 0] = leftSample;
			xDebug[2 * i + 1] = rightSample;
		}

	}
}

void AudioUserDsp_FrameToSamples(uint8_t* framePointer, int16_t* leftSamplePointer, int16_t* rightSamplePointer)
{
	*leftSamplePointer  = framePointer[1] * 256 + framePointer[0];
	*rightSamplePointer = framePointer[3] * 256 + framePointer[2];

}

void AudioUserDsp_SamplesToFrame(uint8_t* framePointer, int16_t* leftSamplePointer, int16_t* rightSamplePointer)
{
	framePointer[0] = ((uint16_t)*leftSamplePointer) % 256;
	framePointer[1] = ((uint16_t)*leftSamplePointer) / 256;
	framePointer[2] = ((uint16_t)*rightSamplePointer) % 256;
	framePointer[3] = ((uint16_t)*rightSamplePointer) / 256;
}

int16_t AudioUserDsp_ChangeAmplitude(int16_t sample, uint16_t amplitude)
{
	return sample / 16;
}
