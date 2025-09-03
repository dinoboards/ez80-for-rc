#ifndef __HOST_FUNCTIONS
#define __HOST_FUNCTIONS

#include <stdint.h>

extern void print_string(const char *const str);
extern int  putchar(int ch);

#define HOST_VAR_VDP_CURRENT_MODE 1

extern void set_host_var_vdp_current_mode(const uint24_t mode);
extern uint24_t get_host_var_data(const uint24_t index);

#endif
