#include "audio_user_dsp.h"

extern uint32_t xDebug[100];

void AudioUserDsp_FrameToSamples(uint8_t*, int16_t* leftSamplePointer, int16_t* rightSamplePointer);
void AudioUserDsp_SamplesToFrame(uint8_t*, int16_t* leftSamplePointer, int16_t* rightSamplePointer);

uint32_t divider = 1;
int16_t inconsistencies = 0;
int16_t previousSample = 0;

float a0, a1, a2, b1, b2;
extern float in_z1;
extern float in_z2;
extern float out_z1;
extern float out_z2;

void AudioUserDsp_ApplyFilterToSamples(uint8_t* dataPointer, uint32_t dataLength, int16_t (*leftChannelFilter)(int16_t, uint32_t), int16_t (*rightChannelFilter)(int16_t, uint32_t))
{
	int16_t leftSample, rightSample;
	for (uint32_t i = 0; i < dataLength; i++)
	{

		// uint8_t* framePointer = dataPointer;
		uint8_t* framePointer = dataPointer + (4 * i);

		AudioUserDsp_FrameToSamples(framePointer, &leftSample, &rightSample);


	if(dataLength < 192)
	{
		if(i < 10)
		{
			xDebug[2 * i + 0] = i;
			xDebug[2 * i + 1] = leftSample;
		}

		if(i > (dataLength - 11))
		{
			xDebug[2 * (i - (dataLength - 11) + 9) + 0] = i;
			xDebug[2 * (i - (dataLength - 11) + 9) + 1] = leftSample;
		}
	}

//		int16_t originalLeftSample = leftSample;

		if(leftChannelFilter)
			leftSample = leftChannelFilter(leftSample, i);

		if(rightChannelFilter)
			rightSample = rightChannelFilter(rightSample, i);

		AudioUserDsp_SamplesToFrame(framePointer, &leftSample, &rightSample);

		// xDebug[0] = (int16_t)i;
		// if(i < 40)
		// {
		// 	xDebug[i] = leftSample;
		// }
		// if(i < 40)
		// {
		// 	xDebug[2 * i + 0] = originalLeftSample;
		// 	xDebug[2 * i + 1] = leftSample;
		// }
		// Fazer um iterador que mostre esses valores cada vez que for aplicado em um sample (vai precisar que o i seja acessivel dentro da função de frames!) 
		// xDebug[0] = (int16_t)in_z1;
		// xDebug[1] = (int16_t)in_z2;
		// xDebug[2] = (int16_t)out_z1;
		// xDebug[3] = (int16_t)out_z2;
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

int16_t AudioUserDsp_ChangeAmplitude(int16_t sample, uint32_t amplitude)
{
	return sample / 4;
}

int16_t AudioUserDsp_LowPassFilter(int16_t sample, uint32_t iteration)
{
	a0 = 0.011050373933114971f;
	a1 = 0.022100747866229942f;
  	a2 = 0.011050373933114971f;
  	b1 = -1.3368583644305965f;
  	b2 = 0.3810598601630564f;


//	float fInSample = (float)sample;
	// float fOutSample = (a0 * fInSample) + (a1 * fInSample) + (a2 * fInSample);
	// float fOutSample = (a0 * fInSample) + (a1 * in_z1) + (a2 * in_z2);
	
	// int16_t outSample = (int16_t)(fOutSample);
	int16_t outSample = sample;
		// - b1 * out_z1
		// - b2 * out_z2;

	// if(iteration < 10)
	// {
	// 	xDebug[2 * iteration + 0] = iteration;
	// 	xDebug[2 * iteration + 1] = sample;
	// }

	// if(iteration > 181)
	// {
	// 	xDebug[2 * (iteration-181 + 9) + 0] = iteration;
	// 	xDebug[2 * (iteration-181 + 9) + 1] = sample;
	// }

	// if(iteration == 189)
	// {
	// 	xDebug[0] = in_z1;
	// 	xDebug[1] = in_z2;
	// 	xDebug[2] = out_z1;
	// 	xDebug[3] = out_z2;
	// 	xDebug[4] = sample;
	// 	xDebug[5] = outSample;
	// }

	// if(iteration == 190)
	// {
	// 	xDebug[6] = 555;
	// 	xDebug[7] = 555;
	// 	xDebug[8] = in_z1;
	// 	xDebug[9] = in_z2;
	// 	xDebug[10] = out_z1;
	// 	xDebug[11] = out_z2;
	// 	xDebug[12] = sample;
	// 	xDebug[13] = outSample;
	// }

	// if(iteration == 191)
	// {
	// 	xDebug[14] = 555;
	// 	xDebug[15] = 555;
	// 	xDebug[16] = in_z1;
	// 	xDebug[17] = in_z2;
	// 	xDebug[18] = out_z1;
	// 	xDebug[19] = out_z2;
	// 	xDebug[20] = sample;
	// 	xDebug[21] = outSample;
	// }

	// if(iteration == 0)
	// {
	// 	xDebug[22] = 555;
	// 	xDebug[23] = 555;
	// 	xDebug[24] = in_z1;
	// 	xDebug[25] = in_z2;
	// 	xDebug[26] = out_z1;
	// 	xDebug[27] = out_z2;
	// 	xDebug[28] = sample;
	// 	xDebug[29] = outSample;
	// }

	// if(iteration == 1)
	// {
	// 	xDebug[30] = 555;
	// 	xDebug[31] = 555;
	// 	xDebug[32] = in_z1;
	// 	xDebug[33] = in_z2;
	// 	xDebug[34] = out_z1;
	// 	xDebug[35] = out_z2;
	// 	xDebug[36] = sample;
	// 	xDebug[37] = outSample;
	// }

	// if(sample == 0)
	// {
	// 	inconsistencies++;
	// 	if(++inconsistencies > 32765)
	// 		inconsistencies = 0;
	// 	xDebug[38] = inconsistencies;
	// }
	
	// xDebug[39] = sample;





	// if(iteration >= (192 - 6))
	// {
	// 	xDebug[6 * iteration + 0 - (192 - 6)] = in_z1;
	// 	xDebug[6 * iteration + 1 - (192 - 6)] = in_z2;
	// 	xDebug[6 * iteration + 2 - (192 - 6)] = out_z1;
	// 	xDebug[6 * iteration + 3 - (192 - 6)] = out_z2;
	// 	xDebug[6 * iteration + 4 - (192 - 6)] = sample;
	// 	xDebug[6 * iteration + 5 - (192 - 6)] = outSample;
	// }


//	in_z2 = in_z1;
//	in_z1 = fInSample;
//	out_z2 = out_z1;
//	out_z1 = fOutSample;
//
//	previousSample = sample;

	// if(iteration < 40)
	// 	xDebug[iteration] = in_z2;
	



	// xDebug[0] = sample;
	// xDebug[1] = outSample;
	// xDebug[2] = 35;

//	fOutSample = fOutSample * 32767;

//	int16_t iOutSample = (int16_t)(fOutSample * 32767);

	// if(fOutSample > 32767)
	// 	fOutSample = 32767;
	// if(fOutSample < -32768)
	// 	fOutSample = -32768;

	// return ((int16_t)fOutSample)/divider;
	return outSample;




}
