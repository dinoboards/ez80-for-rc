#ifndef __EZ80_FIRMWARE_H__
#define __EZ80_FIRMWARE_H__

#include <stdint.h>

extern uint32_t ez80_cpu_freq_get(void);
extern uint24_t ez80_mem_bus_cycles_get(void);
extern uint8_t  ez80_io_bus_cycles_get(void);

extern uint8_t ez80_mem0_bus_cycles_set(const uint8_t cycles);

#endif
