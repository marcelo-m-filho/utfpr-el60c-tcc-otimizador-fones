#ifndef __LCD_H
#define __LCD_H

// includes ----------------------------------------------------------
#include "stm32f769i_discovery_lcd.h"

// defines -----------------------------------------------------------
#define LAYER0_ADDRESS    (LCD_FB_START_ADDRESS)

// function prototypes -----------------------------------------------
void LCD_Init(void);
void BSP_LCD_DrawPicture(const uint8_t* image, uint32_t width, uint32_t height, uint32_t xPosition, uint32_t yPosition );

#endif // __LCD_H