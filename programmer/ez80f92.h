#ifndef __EZ80F92_H
#define __EZ80F92_H

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

/* General-Purpose Input/Output Ports */
#define PB_DR   0x9A
#define PB_DDR  0x9B
#define PB_ALT1 0x9C
#define PB_ALT2 0x9D
#define PC_DR   0x9E
#define PC_DDR  0x9F
#define PC_ALT1 0xA0
#define PC_ALT2 0xA1
#define PD_DR   0xA2
#define PD_DDR  0xA3
#define PD_ALT1 0xA4
#define PD_ALT2 0xA5

/* On-Chip Flash Memory Control */
#define FLASH_KEY    0xF5
#define FLASH_DATA   0xF6
#define FLASH_ADDR_U 0xF7
#define FLASH_CTRL   0xF8
#define FLASH_FDIV   0xF9
#define FLASH_PROT   0xFA
#define FLASH_IRQ    0xFB
#define FLASH_PAGE   0xFC
#define FLASH_ROW    0xFD
#define FLASH_COL    0xFE
#define FLASH_PGCTL  0xFF

/*
 * Flash Key register (FLASH_KEY) unlock codes
 */
#define FLASH_KEY_UNLOCK_1 0xB6
#define FLASH_KEY_UNLOCK_2 0x49

/*
 * Flash Program Control register (FLASH_PGCTL) bit definitions
 */
#define FLASH_PGCTL_ROW_PGM    BIT2
#define FLASH_PGCTL_PG_ERASE   BIT1
#define FLASH_PGCTL_MASS_ERASE BIT0

#endif
