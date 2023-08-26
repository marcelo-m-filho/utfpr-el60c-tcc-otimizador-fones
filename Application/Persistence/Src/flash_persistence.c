#include "flash_persistence.h"

void FlashPersistence_Write()
{
  FLASH_EraseInitTypeDef eraseInitStruct;
  eraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
  eraseInitStruct.Sector = FLASH_SECTOR_10;
  eraseInitStruct.NbSectors = 1;
  eraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
  uint32_t sectorError = 0;
    
  SavedData dataToSave;
  dataToSave.intData = 42;
  dataToSave.doubleData = 31.4159;


// clear all flags before you write it to flash
//   __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
//                          FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

  // HAL_FLASH_Unlock();
  // HAL_FLASH_OB_Unlock();
  // HAL_FLASHEx_Erase(&eraseInitStruct, &sectorError);
  // HAL_FLASH_Lock();

//   for (uint32_t i = 0; i < sizeof(SavedData); i += 4) {
    //   HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_USER_START_ADDR + i, 32);
  HAL_FLASH_Unlock();
  HAL_FLASH_OB_Unlock();
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_USER_START_ADDR, 32);
  HAL_FLASH_Lock();
//   }
}

SavedData FlashPersistence_Read()
{
    SavedData retrievedData;
    // for (uint32_t i = 0; i < sizeof(SavedData); i += 4) {
        // *((uint32_t*)((uint8_t*)&retrievedData + i)) = *(__IO uint32_t*)(FLASH_USER_START_ADDR + i);
        // *((uint32_t*)((uint8_t*)&retrievedData + i)) = *(__IO uint32_t*)(FLASH_USER_START_ADDR + i);
    // }
    retrievedData.doubleData = 123.4;
    retrievedData.intData = *(volatile uint32_t*)(FLASH_USER_START_ADDR);
    return retrievedData;
}
