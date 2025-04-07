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
extern void validate_number(const char *value, uint8_t *number);
extern void config_flash(mem_config_t mem_config);
extern void config_mem0(mem_config_t mem_config);
extern void config_mem1(mem_config_t mem_config);
extern void config_mem(mem_config_t mem_config);
extern void config_io(mem_config_t mem_config);
extern void config_set_tick_rate(uint8_t rate);

extern void find_ns_timing_for_memory();

extern uint8_t main_ns;
extern uint8_t cs0_ns;
extern uint8_t cs1_ns;
extern uint8_t io_ns;

#endif
