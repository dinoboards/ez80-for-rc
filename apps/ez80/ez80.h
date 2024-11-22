#ifndef __APP_EZ80_H
#define __APP_EZ80_H

#include <stdint.h>

extern void find_extended_memory(void);
extern void report_memory_timing(void);
extern void show_help(void);

typedef enum { WAIT_STATE, BUS_CYCLE } wait_type_t;

typedef struct {
  uint8_t     value;
  wait_type_t type;
} mem_config_t;

void config_mem0(mem_config_t mem_config);

#endif
