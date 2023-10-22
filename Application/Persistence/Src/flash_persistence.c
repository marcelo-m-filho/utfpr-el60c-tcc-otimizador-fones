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
}

void FlashPersistence_Restore()
{
  for(uint32_t i = 0; i < NUMBER_OF_SLIDER_BUTTONS; i++)
    sliderKnobs[i].knobY = *(volatile int32_t*)(FLASH_USER_START_ADDR + i*4);
}

uint16_t FlashPersistence_Read(uint8_t position)
{
  return *(volatile int32_t*)(FLASH_USER_START_ADDR + position * 4);
}
