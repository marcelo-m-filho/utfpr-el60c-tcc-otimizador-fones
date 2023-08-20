#ifndef __LCD_H
#define __LCD_H

// includes ----------------------------------------------------------
#include "stm32f769i_discovery_lcd.h"
#include <stdbool.h>


// defines -----------------------------------------------------------
#define LCD_SCREEN_WIDTH              800
#define LCD_SCREEN_HEIGHT             480
#define ARGB8888_BYTE_PER_PIXEL       4
#define LAYER0_ADDRESS    (LCD_FB_START_ADDRESS)
#define LAYER1_ADDRESS    (LCD_FB_START_ADDRESS + (800 * 480 * ARGB8888_BYTE_PER_PIXEL)) 

/* LTDC foreground layer address 800x480 in ARGB8888 */
#define LCD_FG_LAYER_ADDRESS          LCD_FB_START_ADDRESS

/* LTDC background layer address 800x480 in ARGB8888 following the foreground layer */
#define LCD_BG_LAYER_ADDRESS          LCD_FG_LAYER_ADDRESS + (LCD_SCREEN_WIDTH * LCD_SCREEN_HEIGHT * ARGB8888_BYTE_PER_PIXEL)

#define INTERNAL_BUFFER_START_ADDRESS LCD_BG_LAYER_ADDRESS + (LCD_SCREEN_WIDTH * LCD_SCREEN_HEIGHT * ARGB8888_BYTE_PER_PIXEL)

#define NUMBER_OF_CIRCLE_BUTTONS 1
#define NUMBER_OF_INCREMENT_BUTTONS 1
#define NUMBER_OF_SLIDER_BUTTONS 8

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
void LCD_DisplayKnob(uint8_t knobIndex, uint16_t newKnobY);
void BSP_LCD_DrawPicture(const uint8_t* image, uint32_t width, uint32_t height, uint32_t xPosition, uint32_t yPosition);
void LCD_RelocateKnob(uint8_t knobIndex, uint16_t knobY);

extern CircleButtonTypeDef circleButtons[];
extern IncrementButton plusButtons[];
extern SliderKnob sliderKnobs[];

#endif // __LCD_H