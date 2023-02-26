/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  */

// define to prevent recursive inclusion ---------------------------------------
#ifndef __USART_H__
#define __USART_H__

// includes --------------------------------------------------------------------
#include "stm32f7xx.h"

extern UART_HandleTypeDef UART1_Handle;

void USART1_UART_Init(void);

#endif // __USART_H__