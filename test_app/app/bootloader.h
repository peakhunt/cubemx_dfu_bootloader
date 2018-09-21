#ifndef __BOOTLOADER_DEF_H__
#define __BOOTLOADER_DEF_H__

// bootloader size is 12K
#define BOOTLOADER_FLASH_ADDRESS            0x08000000U
#define BOOTLOADER_FLASH_PAGE_SIZE          1024
#define BOOTLOADER_APP_START_ADDRESS        (BOOTLOADER_FLASH_ADDRESS + 1024 * 16)
#define BOOTLOADER_APP_END_ADDRESS          (BOOTLOADER_FLASH_ADDRESS + 1024 * 64)

extern int bootloader_check_bootmode(void);
extern void bootloader_set_bootmode(void);
extern void bootloader_jump_to_app(void);

#endif /* !__BOOTLOADER_DEF_H__ */
