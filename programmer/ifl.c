#include "ifl.h"

int8_t IFL_IsAddrValid(const int32_t hAddr, const uint16_t Len) {
  if (hAddr < IFL_BIOS_START || hAddr > IFL_BIOS_END)
    return ZFL_ERR_ADDRESS;

  if (hAddr + Len > IFL_BIOS_END)
    return ZFL_ERR_ADDRESS;

  return ZFL_ERR_SUCCESS;
}
