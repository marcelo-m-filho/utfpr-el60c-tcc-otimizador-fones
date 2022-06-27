/**
  ******************************************************************************
  * @file    USB_Device/AUDIO_EXT_Advanced_Player_Recorder/Inc/main.h 
  * @author  MCD Application Team 
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019  STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stdbool.h"
#include "stm32f7xx_hal.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_audio.h"
#include "usbd_audio_if.h"
#include "stm32f769i_discovery_lcd.h"
#include "stm32f769i_discovery_audio_ex.h"
#include "stm32f769i_discovery_ts.h"
#include "touchscreen.h"

#define ARGB8888_BYTE_PER_PIXEL  4
#define RGB565_BYTE_PER_PIXEL    2
#define LCD_FRAME_BUFFER  0xC0000000
#define LAYER_SIZE_X          240
#define LAYER_SIZE_Y          130
#define LAYER_BYTE_PER_PIXEL  RGB565_BYTE_PER_PIXEL

#ifdef USE_FULL_ASSERT
/* Assert activated */
#define BSP_TEST_APPLI_ASSERT(__error_condition__)    do { if(__error_condition__) \
                                                           {  while(1);  \
                                                           } \
                                                          } while(0)
#else
/* Assert not activated : macro has no effect */
#define BSP_TEST_APPLI_ASSERT(__error_condition__)    do { if(__error_condition__) \
                                                           {  ;  \
                                                           } \
                                                         } while(0)
#endif /* USE_FULL_ASSERT */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* User can use this section to tailor TIM_VolumeChange instance used and associated
   resources */
/* Definition for TIM_VolumeChange clock resources */
#define TIM_VolumeChange                           TIM3
#define TIM_VolumeChangeCLK_ENABLE()              __HAL_RCC_TIM3_CLK_ENABLE()


/* Definition for TIM_VolumeChange's NVIC */
#define TIM_VolumeChangeIRQn                      TIM3_IRQn
#define TIM_VolumeChangeIRQHandler                TIM3_IRQHandler

/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
