#include "flash_persistence.h"
#include "user_lcd.h"
#include "usbd_core.h"
extern USBD_HandleTypeDef USBD_Device;
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
  // HAL_FLASH_Lock();

//   for (uint32_t i = 0; i < sizeof(SavedData); i += 4) {
    //   HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_USER_START_ADDR + i, 32);
  
  USBD_LL_Suspend(&USBD_Device);
  USBD_Stop(&USBD_Device);
  HAL_FLASH_Unlock();
  HAL_FLASHEx_Erase(&eraseInitStruct, &sectorError);
  for(uint32_t i = 0; i < NUMBER_OF_SLIDER_BUTTONS; i++)
  {
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_USER_START_ADDR + i*4, sliderKnobs[i].knobY);
  }
  HAL_FLASH_Lock();
  USBD_LL_Resume(&USBD_Device);
  USBD_Start(&USBD_Device);
    //   HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_USER_START_ADDR + i, *((uint32_t*)((uint8_t*)&dataToSave + i)));

//   }
}

SavedData FlashPersistence_Read()
{
    SavedData retrievedData;
    // for (uint32_t i = 0; i < sizeof(SavedData); i += 4) {
        // *((uint32_t*)((uint8_t*)&retrievedData + i)) = *(__IO uint32_t*)(FLASH_USER_START_ADDR + i);
        // *((uint32_t*)((uint8_t*)&retrievedData + i)) = *(__IO uint32_t*)(FLASH_USER_START_ADDR + i);
    // }
    // retrievedData.doubleData = 123.4;
    // retrievedData.intData = *(volatile int32_t*)(FLASH_USER_START_ADDR);
    for(uint32_t i = 0; i < NUMBER_OF_SLIDER_BUTTONS; i++)
    {
      LCD_DisplayKnob(i, *(volatile int32_t*)(FLASH_USER_START_ADDR + i*4));
    }
    return retrievedData;
}
