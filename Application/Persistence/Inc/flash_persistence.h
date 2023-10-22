#ifndef __FLASH_PERSISTENCE_H
#define __FLASH_PERSISTENCE_H

// includes 
#include <stdint.h>
#include "stm32f7xx_hal.h"

#define FLASH_USER_START_ADDR 0x08180000

void FlashPersistence_Write();
void FlashPersistence_Restore();
uint16_t FlashPersistence_Read(uint8_t position);


#endif // __FLASH_PERSISTENCE_H