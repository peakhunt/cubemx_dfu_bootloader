#include "usbd_dfu_if.h"

#define FLASH_DESC_STR      "@Internal Flash   /0x08000000/03*016Ka,01*016Kg,01*064Kg,07*128Kg,04*016Kg,01*064Kg,07*128Kg"

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
  return (USBD_OK);
}

uint16_t MEM_If_DeInit_FS(void)
{
  return (USBD_OK);
}

uint16_t MEM_If_Erase_FS(uint32_t Add)
{
  return (USBD_OK);
}

uint16_t MEM_If_Write_FS(uint8_t *src, uint8_t *dest, uint32_t Len)
{
  return (USBD_OK);
}

uint8_t *MEM_If_Read_FS(uint8_t *src, uint8_t *dest, uint32_t Len)
{
  return (uint8_t*)(USBD_OK);
}

uint16_t MEM_If_GetStatus_FS(uint32_t Add, uint8_t Cmd, uint8_t *buffer)
{
  switch (Cmd)
  {
  case DFU_MEDIA_PROGRAM:
    break;

  case DFU_MEDIA_ERASE:
  default:
    break;
  }
  return (USBD_OK);
}
