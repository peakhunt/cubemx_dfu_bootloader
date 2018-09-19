#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_rtc.h"
#include "stm32f1xx_hal_rtc_ex.h"
#include "gpio.h"

#include "bootloader.h"

int
bootloader_check_bootmode(void)
{
  uint32_t    d;

  d = HAL_RTCEx_BKUPRead(NULL,  RTC_BKP_DR1);
  if(d != 0)
  {
    // bootloader enter set in backup register
    HAL_RTCEx_BKUPWrite(NULL, RTC_BKP_DR1, 0);
    return 1;
  }
  return 0;
}

void
bootloader_jump_to_app(void)
{
  volatile uint32_t*     pApp = (volatile uint32_t*)BOOTLOADER_APP_START_ADDRESS;

  // check if valid image is installed in the flash
  // MSP is supposed to set to sram area
  if((pApp[0] & 0x2FFE0000) != 0x20000000)
  {
    return;
  }
  
  // disable systick interrupt
  // not necessary since this is called before SystemClock_Config()
  // SysTick->CTRL &= (~SysTick_CTRL_ENABLE_Msk);

  // setup 
  SCB->VTOR = BOOTLOADER_APP_START_ADDRESS;

  // initialize MSP
  asm volatile ("msr msp, %0"::"g" (*(volatile uint32_t*)pApp));

  // jump to application
  (*(void(**)())(pApp + 4))();
}
