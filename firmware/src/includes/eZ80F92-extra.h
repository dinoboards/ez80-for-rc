#ifndef _EZ80F92EXTRA_H_
#define _EZ80F92EXTRA_H_

#include <eZ80F92.h>

/*
 * Register bit values
 */
#define BIT7 (1 << 7)
#define BIT6 (1 << 6)
#define BIT5 (1 << 5)
#define BIT4 (1 << 4)
#define BIT3 (1 << 3)
#define BIT2 (1 << 2)
#define BIT1 (1 << 1)
#define BIT0 (1 << 0)

/*
 * Flash Program Control register (FLASH_PGCTL) bit definitions
 */
#define FLASH_PGCTL_ROW_PGM    BIT2
#define FLASH_PGCTL_PG_ERASE   BIT1
#define FLASH_PGCTL_MASS_ERASE BIT0

/*
 * Flash Interrupt Control register (FLASH_IRQ) bit definitions
 */
#define FLASH_IRQ_DONE_IEN BIT7
#define FLASH_IRQ_ERR_IEN  BIT6
#define FLASH_IRQ_DONE     BIT5
#define FLASH_IRQ_WR_VIO   BIT3
#define FLASH_IRQ_RP_TMO   BIT2
#define FLASH_IRQ_PG_VIO   BIT1
#define FLASH_IRQ_MASS_VIO BIT0

/*
 * Flash Page Select register (FLASH_PAGE) bit definitions
 * The eZ80F92 has a 1024-byte page size with 128 total pages.
 */
#define FLASH_PAGE_INFO_EN BIT7
#define FLASH_PAGE_MASK    (IFL_PAGE_SIZE - 1)

/*
 * Flash Key register (FLASH_KEY) unlock codes
 */
#define FLASH_KEY_UNLOCK_1 0xB6
#define FLASH_KEY_UNLOCK_2 0x49

#define di_and_save()                                                                                                              \
  {                                                                                                                                \
    asm("LD    a, i");                                                                                                             \
    asm("PUSH  af");                                                                                                               \
    asm("DI");                                                                                                                     \
  }

#define restore_ei()                                                                                                               \
  {                                                                                                                                \
    asm("POP   hl");                                                                                                               \
    asm("BIT   #2, l");                                                                                                            \
    asm("JR    Z, $F");                                                                                                            \
    asm("EI");                                                                                                                     \
    asm("$$:");                                                                                                                    \
  }
#endif
