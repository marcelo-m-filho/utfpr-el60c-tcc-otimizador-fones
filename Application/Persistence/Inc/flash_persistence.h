#ifndef __FLASH_PERSISTENCE_H
#define __FLASH_PERSISTENCE_H

// includes 
#include <stdint.h>
#include "stm32f7xx_hal.h"

#define FLASH_USER_START_ADDR    FLASH_SECTOR_20

typedef struct {
    int32_t intData;
    double doubleData;
} SavedData;

void FlashPersistence_Write();
SavedData FlashPersistence_Read();


#endif // __FLASH_PERSISTENCE_H