#include "../rst-28-vars.h"
#include <stdint.h>

/**
 * @brief Allocate a block of memory in the linear RAM
 *
 * The page alignment can be used to force the resulting memory to be padded such that it
 * always starts on the required boundary.  For example, a value 65356 (0x10000) will result
 * in a new 64K segment aligned block to be allocated
 *
 * @param size The amount of bytes to allocate
 * @param page_alignment the page alignment.  A value of 1 or 0 means normal byte alignment
 * @return uint8_t* the start address of the allocated block of memory
 */
uint8_t *sys_alloc(const uint24_t size, const uint24_t page_alignment) {
  hbios_vars->himem -= size;

  if (page_alignment > 1) {
    uint24_t addr = (uint24_t)hbios_vars->himem;
    hbios_vars->himem -= (addr % page_alignment);
  }

  return hbios_vars->himem;
}
