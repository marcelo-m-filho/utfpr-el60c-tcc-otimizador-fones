#include "audio_user_dsp.h"

extern uint32_t xDebug[40];

void AudioUserDsp_FrameToSamples(uint8_t*, int16_t* leftSamplePointer, int16_t* rightSamplePointer);
void AudioUserDsp_SamplesToFrame(uint8_t*, int16_t* leftSamplePointer, int16_t* rightSamplePointer);

uint32_t divider = 1;

float a0, a1, a2, b1, b2, in_z1, in_z2, out_z1, out_z2;

void AudioUserDsp_ApplyFilterToSamples(uint8_t* dataPointer, uint32_t dataLength, int16_t (*leftChannelFilter)(int16_t), int16_t (*rightChannelFilter)(int16_t))
{
	int16_t leftSample, rightSample;
	for (uint32_t i = 0; i < dataLength; i++)
	{

		uint8_t* framePointer = dataPointer + (4 * i);

		AudioUserDsp_FrameToSamples(framePointer, &leftSample, &rightSample);


		int16_t originalLeftSample = leftSample;

		if(leftChannelFilter)
			leftSample = leftChannelFilter(leftSample);

		if(rightChannelFilter)
			rightSample = rightChannelFilter(rightSample);

		AudioUserDsp_SamplesToFrame(framePointer, &leftSample, &rightSample);

		if(i < 20)
		{
			xDebug[2 * i + 0] = originalLeftSample;
			xDebug[2 * i + 1] = leftSample;
		}
		// xDebug[19] = divider;
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
	return sample / divider;
}

int16_t AudioUserDsp_LowPassFilter(int16_t sample)
{

	// a0 = 0.7014532025132417f;
	// a1 = -1.4029064050264834f;
	// a2 = 0.7014532025132417f;

	// b1 = -1.3262717309424847f;
	// b2 = 0.47954107911048205f;

	a0 = 0.7414416903620761f;
	a1 = -1.4828833807241522f;
  	a2 = 0.7414416903620761f;
  	b1 = -1.414890450876696f;
  	b2 = 0.5508763105716084f;

	double fInSample = (float)(sample >> 2);
	double fOutSample = 
		a0 * fInSample 
		+ a1 * in_z1 +
		+ a2 * in_z2 +
		- b1 * out_z1 +
		- b2 * out_z2;


	in_z2 = in_z1;
	in_z1 = fInSample;
	out_z2 = out_z1;
	out_z1 = fOutSample;

	if(fOutSample > 32767)
		fOutSample = 32767;
	if(fOutSample < -32768)
		fOutSample = -32768;

	// return ((int16_t)fOutSample)/divider;
	return (int16_t)fOutSample;




}
