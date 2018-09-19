#include "stm32f1xx_hal.h"
#include "usbd_dfu_if.h"
#include "bootloader.h"

//#define FLASH_DESC_STR      "@Internal Flash   /0x08000000/03*016Ka,01*016Kg,01*064Kg,07*128Kg,04*016Kg,01*064Kg,07*128Kg"
//
#define FLASH_DESC_STR      "@Internal Flash   /0x08000000/16*01Ka,48*01Kg"

#define FLASH_ERASE_TIME    (uint16_t)50
#define FLASH_PROGRAM_TIME  (uint16_t)50

extern USBD_HandleTypeDef hUsbDeviceFS;

static uint16_t MEM_If_Init_FS(void);
static uint16_t MEM_If_Erase_FS(uint32_t Add);
static uint16_t MEM_If_Write_FS(uint8_t *src, uint8_t *dest, uint32_t Len);
static uint8_t *MEM_If_Read_FS(uint8_t *src, uint8_t *dest, uint32_t Len);
static uint16_t MEM_If_DeInit_FS(void);
static uint16_t MEM_If_GetStatus_FS(uint32_t Add, uint8_t Cmd, uint8_t *buffer);

#if defined ( __ICCARM__ ) /* IAR Compiler */
  #pragma data_alignment=4
#endif
__ALIGN_BEGIN USBD_DFU_MediaTypeDef USBD_DFU_fops_FS __ALIGN_END =
{
  (uint8_t*)FLASH_DESC_STR,
  MEM_If_Init_FS,
  MEM_If_DeInit_FS,
  MEM_If_Erase_FS,
  MEM_If_Write_FS,
  MEM_If_Read_FS,
  MEM_If_GetStatus_FS
};

uint16_t MEM_If_Init_FS(void)
{
  HAL_FLASH_Unlock();
  return (USBD_OK);
}

uint16_t MEM_If_DeInit_FS(void)
{
  HAL_FLASH_Lock();
  return (USBD_OK);
}

uint16_t MEM_If_Erase_FS(uint32_t Add)
{
  uint32_t NbOfPages = 0;
  uint32_t PageError = 0;
  HAL_StatusTypeDef status;
  FLASH_EraseInitTypeDef eraseinitstruct;

  /* Get the number of sector to erase from 1st sector */
  NbOfPages = ((BOOTLOADER_APP_END_ADDRESS - BOOTLOADER_APP_START_ADDRESS) / FLASH_PAGE_SIZE) + 1;

  eraseinitstruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  eraseinitstruct.PageAddress = BOOTLOADER_APP_START_ADDRESS;
  eraseinitstruct.NbPages     = NbOfPages;

  status = HAL_FLASHEx_Erase(&eraseinitstruct, &PageError);

  if(status != HAL_OK)
  {
    return !USBD_OK;
  }
  return (USBD_OK);
}

uint16_t MEM_If_Write_FS(uint8_t *src, uint8_t *dest, uint32_t Len)
{
  uint32_t    i = 0;

  if(dest < (uint8_t*)BOOTLOADER_APP_START_ADDRESS)
  {
    return 3;
  }

  for(i = 0; i < Len; i += 4)
  {
    if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t) (dest + i), *(uint32_t *) (src + i)) == HAL_OK)
    {
      // verify
      if(*(uint32_t *) (src + i) != *(uint32_t *) (dest + i))
      {
        /* Flash content doesn't match SRAM content */
        return 2;
      }
    }
    else
    {
      // error while writing
      return 1;
    }
  }
  return (USBD_OK);
}

uint8_t *MEM_If_Read_FS(uint8_t *src, uint8_t *dest, uint32_t Len)
{
  uint32_t i = 0;
  uint8_t *psrc = src;

  for (i = 0; i < Len; i++)
  {
    dest[i] = *psrc++;
  }
  /* Return a valid address to avoid HardFault */
  return (uint8_t *) (dest);

}

uint16_t MEM_If_GetStatus_FS(uint32_t Add, uint8_t Cmd, uint8_t *buffer)
{
  switch (Cmd)
  {
  case DFU_MEDIA_PROGRAM:
    buffer[1] = (uint8_t) FLASH_PROGRAM_TIME;
    buffer[2] = (uint8_t) (FLASH_PROGRAM_TIME << 8);
    buffer[3] = 0;
    break;

  case DFU_MEDIA_ERASE:
  default:
    buffer[1] = (uint8_t) FLASH_ERASE_TIME;
    buffer[2] = (uint8_t) (FLASH_ERASE_TIME << 8);
    buffer[3] = 0;
    break;
  }
  return (USBD_OK);
}
