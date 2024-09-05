#ifndef __ROM_FLASHING_H__
#define __ROM_FLASHING_H__

#include <stdint.h>

/*
 * The eZ80F92 microcontroller contains 8 erase-blocks (Block0
 * to Block7) that can be protected from accidental erasure by setting the
 * corresponding bit in the Flash Protect register.
 *
 * Each erase-block is composed of 16 pages.  The page-size is 1024 bytes
 * Each page contains 8 rows.  There are 128 bytes in each row.
 *
 * In addition to the main internal Flash storage area, all eZ80Acclaim!
 * microcontrollers include a secondary area of Flash referred to as the Info
 * page.  The Info Page contains 2 rows of data.
 */
#define IFL_ERASE_BLOCKS     ((uint32_t)8)
#define IFL_PAGE_SIZE_EXP    ((uint8_t)10)
#define IFL_PAGE_SIZE        (1 << IFL_PAGE_SIZE_EXP)
#define IFL_ERASE_BLOCK_SIZE (IFL_PAGE_SIZE * 16)
#define IFL_FLASH_SIZE_BYTES (IFL_ERASE_BLOCK_SIZE * IFL_ERASE_BLOCKS)
#define IFL_TOTAL_PAGES      (IFL_FLASH_SIZE_BYTES / IFL_PAGE_SIZE)
#define IFL_ROW_SIZE         (IFL_PAGE_SIZE >> 3)

#define IFL_ALT_BIOS_START         0x010000
#define IFL_ALT_BIOS_END           0x01FFFF
#define IFL_FLASH_ENABLED_ALT_BIOS 0xF0
#define IFL_FLASH_ENABLED_NONE     0xFF

/*
IFL_ERASE_BLOCKS	8
IFL_PAGE_SIZE_EXP	10
IFL_PAGE_SIZE	1024
IFL_ERASE_BLOCK_SIZE	16384
IFL_FLASH_SIZE_BYTES	16777216
IFL_TOTAL_PAGES	16384
IFL_ROW_SIZE	128

*/
/*
 * Flash Library status codes
 */
#define ZFL_ERR_SUCCESS               0
#define ZFL_ERR_FAILURE               -1
#define ZFL_ERR_VPP                   -2
#define ZFL_ERR_WRITE                 -3
#define ZFL_ERR_ERASE                 -4
#define ZFL_ERR_SUSPEND               -5
#define ZFL_ERR_ADDRESS               -6
#define ZFL_ERR_VERIFY                -7
#define ZFL_ERR_UNSUPPORTED_CMD_SET   -8
#define ZFL_ERR_UNSUPPORTED_DEVICE    -9
#define ZFL_ERR_INVALID_PARAMETER     -10
#define ZFL_ERR_TOO_MANY_ERASE_BLOCKS -11
#define ZFL_ERR_TOO_MANY_REGIONS      -12

int8_t  IFL_Init(void);
int8_t  IFL_IsAddrValid(const uint8_t *flash_addr, const uint16_t length);
uint8_t IFL_GetPage(const void *flash_addr);
int8_t  IFL_ErasePages(const uint8_t *flash_addr, uint8_t number_of_pages);
int8_t  IFL_EraseInfoPage(void);
int8_t  IFL_Program(uint8_t *flash_addr, uint8_t *source_addr, const uint16_t length);
int8_t  IFL_ProgramInfoPage(uint8_t flash_addr, uint8_t *source_addr, uint8_t length);
int8_t  IFL_ReadInfoPage(uint8_t *destination_addr, uint8_t flash_addr, uint8_t length);

#endif
