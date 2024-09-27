#ifndef __READ_LINE_H
#define __READ_LINE_H

#include <stdbool.h>

#define BUFFER_SIZE 133

extern char input_buffer[BUFFER_SIZE];

typedef bool (*pool_status_fn_t)(void);

extern bool read_line(pool_status_fn_t);

#endif
