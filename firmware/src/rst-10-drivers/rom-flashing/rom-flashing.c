#include "rom-flashing.h"
#include "rom-flashing-calculator.h"
#include <critical-blocks.h>
#include <eZ80F92-extra.h>
#include <stdint.h>

int8_t IFL_Init(void) {
  uint8_t fdiv = calculate_fdiv();
  critical_begin();
  FLASH_KEY  = FLASH_KEY_UNLOCK_1;
  FLASH_KEY  = FLASH_KEY_UNLOCK_2;
  FLASH_FDIV = fdiv;

  FLASH_KEY  = FLASH_KEY_UNLOCK_1;
  FLASH_KEY  = FLASH_KEY_UNLOCK_2;
  FLASH_PROT = IFL_FLASH_ENABLED_NONE;
  critical_end();

  return ZFL_ERR_SUCCESS;
}

int8_t IFL_IsAddrValid(const uint8_t *hAddr, const uint16_t Len) {
  if ((uint24_t)hAddr < IFL_ALT_BIOS_START || (uint24_t)hAddr > IFL_ALT_BIOS_END)
    return ZFL_ERR_ADDRESS;

  return ZFL_ERR_SUCCESS;
}

uint8_t IFL_GetPage(const void *hAddr) {
  return (uint8_t)(((uint32_t)(hAddr) & ((0 - IFL_PAGE_SIZE) & (IFL_FLASH_SIZE_BYTES - 1))) >> IFL_PAGE_SIZE_EXP);
}

/*
 * This command is used to erase the page of internal Flash containing the
 * target address (hAddr).
 */
int8_t IFL_ErasePages(const uint8_t *hAddr, uint8_t NumPages) {
  int8_t  status       = IFL_IsAddrValid(hAddr, 0);
  uint8_t current_page = IFL_GetPage(hAddr);
  uint8_t page;

  critical_begin();
  page = FLASH_PAGE;

  FLASH_KEY  = FLASH_KEY_UNLOCK_1;
  FLASH_KEY  = FLASH_KEY_UNLOCK_2;
  FLASH_PROT = IFL_FLASH_ENABLED_ALT_BIOS;

  while ((status == ZFL_ERR_SUCCESS) && NumPages--) {
    FLASH_PAGE  = current_page;
    FLASH_PGCTL = FLASH_PGCTL_PG_ERASE;

    while (FLASH_PGCTL & FLASH_PGCTL_PG_ERASE)
      ;

    if (FLASH_IRQ & FLASH_IRQ_PG_VIO)
      status = ZFL_ERR_ERASE;

    current_page++;
    if (current_page > IFL_TOTAL_PAGES)
      status = ZFL_ERR_ADDRESS;
  }

  FLASH_KEY  = FLASH_KEY_UNLOCK_1;
  FLASH_KEY  = FLASH_KEY_UNLOCK_2;
  FLASH_PROT = IFL_FLASH_ENABLED_NONE;

  FLASH_PAGE = page;
  critical_end();
  return status;
}

int8_t IFL_Program(uint8_t *pDst, uint8_t *pSrc, const uint16_t Len) {
  uint16_t i;
  int8_t   status = IFL_IsAddrValid(pDst, Len);

  if (status != ZFL_ERR_SUCCESS)
    return status;

  critical_begin();

  FLASH_KEY  = FLASH_KEY_UNLOCK_1;
  FLASH_KEY  = FLASH_KEY_UNLOCK_2;
  FLASH_PROT = IFL_FLASH_ENABLED_ALT_BIOS;

  for (i = 0; i < Len; i++) {
    *pDst = *pSrc;
    if (*pDst != *pSrc) {
      status = (FLASH_IRQ & FLASH_IRQ_WR_VIO) ? ZFL_ERR_WRITE : ZFL_ERR_VERIFY;
      break;
    }
    pDst++;
    pSrc++;
  }

  FLASH_KEY  = FLASH_KEY_UNLOCK_1;
  FLASH_KEY  = FLASH_KEY_UNLOCK_2;
  FLASH_PROT = IFL_FLASH_ENABLED_NONE;

  critical_end();

  return status;
}
