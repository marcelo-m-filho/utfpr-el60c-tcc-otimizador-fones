#include "audio_user_dsp.h"
#include "usart.h"
#include "usb_audio.h"
#include <math.h>

extern UART_HandleTypeDef UART1_Handle;

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

#define DSP_SERIAL_BUFFER_SIZE 100
#define PI 3.14159265358979323846f

BiquadFilter biquadFilters[NUMBER_OF_BANDS];

void AudioUserDsp_ApplyFilterToSamples(uint8_t* dataPointer, uint32_t dataLength, int16_t (*leftChannelFilter)(int16_t, uint8_t), int16_t (*rightChannelFilter)(int16_t, uint8_t), uint8_t filterIndex)
{
  int16_t leftSample, rightSample;
 
  for (uint8_t i = 0; i < dataLength; i += 4)
  {
    uint8_t* framePointer = dataPointer + i;

    AudioUserDsp_FrameToSamples(framePointer, &leftSample, &rightSample);

    if(leftChannelFilter)
      leftSample = leftChannelFilter(leftSample, filterIndex);

    if(rightChannelFilter)
      rightSample = rightChannelFilter(rightSample, filterIndex);

    AudioUserDsp_SamplesToFrame(framePointer, &leftSample, &rightSample);
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

int16_t AudioUserDsp_ChangeAmplitude(int16_t sample, uint8_t filterIndex)
{
  return sample / 4;
}

int16_t AudioUserDsp_LowPassFilter(int16_t sample, uint8_t filterIndex)
{
  a0 = 0.011050373933114971f;
  a1 = 0.022100747866229942f;
  a2 = 0.011050373933114971f;
  b1 = -1.3368583644305965f;
  b2 = 0.3810598601630564f;

  double fInSample = (float)(sample);
  double fOutSample = 
      a0 * fInSample 
    + a1 * in_z1 
    + a2 * in_z2
    - b1 * out_z1
    - b2 * out_z2;


  in_z2   = in_z1;
  in_z1   = fInSample;
  out_z2  = out_z1;
  out_z1  = fOutSample;


  return (int16_t)fOutSample;
}

int16_t AudioUserDsp_BiquadFilter(int16_t sample, uint8_t filterIndex)
{
  BiquadFilter* filter = &biquadFilters[filterIndex];

  double fInSample = (float)(sample); // TODO: vers e é necessário o shift
  double fOutSample =
      filter->b0 * fInSample
    + filter->b1 * filter->in_z1
    + filter->b2 * filter->in_z2
    - filter->a1 * filter->out_z1
    - filter->a2 * filter->out_z2;

  filter->in_z2   = filter->in_z1;
  filter->in_z1   = fInSample;
  filter->out_z2  = filter->out_z1;
  filter->out_z1  = fOutSample;

  if(fOutSample > 32767)
    fOutSample = 32767;
  else if(fOutSample < -32768)
    fOutSample = -32768;

  return (int16_t)fOutSample;
}

int16_t AudioUserDsp_BiquadFilterConfig(BiquadFilter* filter, int16_t gain, int16_t frequency, int16_t bandwidth)
{
  int32_t stringSize;
  char result[100];

  double A = pow(10.0, gain / 40.0);
  double omega = 2.0 * PI * frequency / USB_AUDIO_CONFIG_PLAY_DEF_FREQ;
  double alpha = sin(omega) * sinh(log(2) / 2.0 * bandwidth * omega / sin(omega));

  double b0 = 1.0 + alpha * A;
  double b1 = -2.0 * cos(omega);
  double b2 = 1.0 - alpha * A;
  double a0 = 1.0 + alpha / A;
  double a1 = -2.0 * cos(omega);
  double a2 = 1.0 - alpha / A;

  filter->b0 = (float)(b0 / a0);
  filter->b1 = (float)(b1 / a0);
  filter->b2 = (float)(b2 / a0);
  filter->a1 = (float)(a1 / a0);
  filter->a2 = (float)(a2 / a0);

  filter->gain = gain;
  filter->frequency = frequency;
  filter->bandwidth = bandwidth;
  
  if(!filter->isInitialized)
  {
    filter->in_z1 = 0;
    filter->in_z2 = 0;
    filter->out_z1 = 0;
    filter->out_z2 = 0;
    filter->isInitialized = true;
  }
}