#ifndef __LCD_H
#define __LCD_H

// includes ----------------------------------------------------------
#include "stm32f769i_discovery_lcd.h"
#include <stdbool.h>

// defines -----------------------------------------------------------
#define LAYER0_ADDRESS    (LCD_FB_START_ADDRESS)
#define NUMBER_OF_CIRCLE_BUTTONS 6

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

// function prototypes -----------------------------------------------
void LCD_Init(void);
void LCD_UpdateWatchdog(uint32_t* watchdogCounter);
void LCD_PrintDebugVariable(uint8_t columns, bool printAsShort);
void LCD_UpdateButton(uint8_t buttonIndex, bool isPressed, bool shouldToggleOtherButtons);
void BSP_LCD_DrawPicture(const uint8_t* image, uint32_t width, uint32_t height, uint32_t xPosition, uint32_t yPosition);

#endif // __LCD_H