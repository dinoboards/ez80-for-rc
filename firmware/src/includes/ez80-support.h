#ifndef __EZ80_SUPPORT
#define __EZ80_SUPPORT

#include <stdint.h>

/**
 * @brief extend the 16 bit address to the full
 * 24 bit address of the current MBASE segment
 *
 * @param address a 16 bit address within the current MBASE segment
 *
 * @return uint24_t the full 24bit address
 */
extern uint24_t extend_mbase(uint16_t address);

#endif
