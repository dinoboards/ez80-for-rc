#ifndef __EZ80_FIRMWARE_H__
#define __EZ80_FIRMWARE_H__

#include <stdint.h>

extern uint32_t ez80_cpu_freq_get(void);
extern uint8_t  ez80_mem_bus_cycles_get(void);
extern uint8_t  ez80_io_bus_cycles_get(void);

#endif
