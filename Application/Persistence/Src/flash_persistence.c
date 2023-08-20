#include "flash_persistence.h"

void FlashPersistence_Write()
{
  FLASH_EraseInitTypeDef eraseInitStruct;
  eraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
  eraseInitStruct.Sector = FLASH_USER_START_ADDR;
  eraseInitStruct.NbSectors = 1;
  eraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
  uint32_t sectorError = 0;
    
  HAL_FLASHEx_Erase(&eraseInitStruct, &sectorError);
  SavedData dataToSave;
  dataToSave.intData = 42;
  dataToSave.doubleData = 31.4159;


  HAL_FLASH_Unlock();
  for (uint32_t i = 0; i < sizeof(SavedData); i += 4) {
      HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_USER_START_ADDR + i, *((uint32_t*)((uint8_t*)&dataToSave + i)));
  }
  HAL_FLASH_Lock();

  HAL_FLASHEx_Erase(&eraseInitStruct, &sectorError);

}

SavedData FlashPersistence_Read()
{
    SavedData retrievedData;
    for (uint32_t i = 0; i < sizeof(SavedData); i += 4) {
        *((uint32_t*)((uint8_t*)&retrievedData + i)) = *(__IO uint32_t*)(FLASH_USER_START_ADDR + i);
    }

    return retrievedData;
}
