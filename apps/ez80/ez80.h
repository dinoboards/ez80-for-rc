#ifndef __APP_EZ80_H
#define __APP_EZ80_H

#include <stdint.h>

typedef enum { WAIT_STATE, BUS_CYCLE } wait_type_t;

typedef struct {
  uint8_t     value;
  wait_type_t type;
} mem_config_t;

extern void find_extended_memory(void);
extern void report_memory_timing(void);
extern void show_help(void);
extern void validate_mem_set_value(const char *value, mem_config_t *mem_config);
extern void validate_wait_only_set_value(const char *value, mem_config_t *mem_config);
extern void config_flash(mem_config_t mem_config);
extern void config_mem0(mem_config_t mem_config);
extern void config_mem(mem_config_t mem_config);
extern void config_io(mem_config_t mem_config);

#endif
