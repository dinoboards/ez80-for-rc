#include "dhry.h"

#if defined(MSXDOS2)
#include <stdint.h>
typedef uint16_t ticks_t;

#define JIFFY_ADDR  __at(0xFC9E)
#define RG9SAV_ADDR __at(0xFFE8)

// FC9E-FC9F: software clock, updated at each VDP interrupt
ticks_t JIFFY_ADDR  JIFFY;
uint8_t RG9SAV_ADDR RG9SAV;
#endif
