#ifndef HEX_RECORD_H
#define HEX_RECORD_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef int8_t (*emit_func_t)(const uint32_t addr, const uint8_t *data, const uint16_t len);

int8_t process_hex_records(const bool internal, const emit_func_t emit);

#endif
