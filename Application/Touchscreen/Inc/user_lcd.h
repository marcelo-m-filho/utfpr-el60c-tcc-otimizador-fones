#ifndef __LCD_H
#define __LCD_H

// includes ----------------------------------------------------------
#include "stm32f769i_discovery_lcd.h"
#include <stdbool.h>

// defines -----------------------------------------------------------
#define LAYER0_ADDRESS    (LCD_FB_START_ADDRESS)
#define NUMBER_OF_CIRCLE_BUTTONS 6
#define NUMBER_OF_INCREMENT_BUTTONS 1
#define NUMBER_OF_SLIDER_BUTTONS 5

// typedefs ----------------------------------------------------------
typedef struct CircleButton
{
    uint16_t x;
    uint16_t y;
    uint16_t radius;
    uint32_t color;
    uint32_t textColor;
    char* onText;
    char* offText;
    bool isPressed;
    bool isIndependent;
    bool isActive;
} CircleButtonTypeDef;

typedef struct IncrementButton
{
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    uint32_t color;
    uint32_t textColor;
    char* text;
    bool isPressed;
    int16_t incrementValue;
} IncrementButton;

typedef struct SliderKnob
{
    uint16_t sliderX;
    uint16_t sliderY;
    uint16_t sliderWidth;
    uint16_t sliderHeight;
    uint32_t sliderColor;
    uint16_t knobY;
    uint16_t knobRadius;
    bool isPressed;
    uint32_t debounceCount;
    uint32_t debouceLimit;
} SliderKnob;

// function prototypes -----------------------------------------------
void LCD_Init(void);
void LCD_UpdateWatchdog(uint32_t* watchdogCounter);
void LCD_UpdateButton(uint8_t buttonIndex, bool isPressed, bool shouldToggleOtherButtons);
void LCD_DisplayKnob(uint8_t knobIndex);
void BSP_LCD_DrawPicture(const uint8_t* image, uint32_t width, uint32_t height, uint32_t xPosition, uint32_t yPosition);

extern CircleButtonTypeDef circleButtons[];
extern IncrementButton plusButtons[];
extern SliderKnob sliderKnobs[];

#endif // __LCD_H