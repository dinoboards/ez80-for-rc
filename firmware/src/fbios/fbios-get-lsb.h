#ifndef __FBIOS_GET_LSB
#define __FBIOS_GET_LSB

#include <stdint.h>

/**
 * @brief Calculates the Logical Sector Block (LSB) for the current fbios
 * track and 128 byte sector.
 *
 * @return uint24_t The calculated LSB.
 */
uint24_t get_lsb();

/**
 * @brief Calculates the byte offset within a 512-byte sector for the current
 * sector.
 *
 * @return uint24_t The calculated offset.
 */
uint24_t get_offset();

#endif
