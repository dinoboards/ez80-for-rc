#ifndef __TARGET_SUPPORT
#define __TARGET_SUPPORT

/* The target environment has 24 bit for standard int type
 *
 * To support typical PC environment of 32 bit, a type and associated conversion functions are needed
 *
 * On the target environment these can be 'nop-op' #defines
 *
 **/

#include <stdint.h>

typedef int24_t  stdint_t;  /* can be a 32 or 24 bit number - depending on target CPU */
typedef uint24_t ustdint_t; /* can be a 32 or 24 bit number - depending on target CPU */

typedef uint24_t vm_size_t; /* type to represent size of a vm image (32bits or 24 bits) */

#define vm_sizeof(x) ((vm_size_t)(sizeof(x)))

#define to_ustdint(x) (x)
#define to_stdint(x)  (x)

#define to_uint24(x) (x)
#define to_int24(x)  (x)

#define UINT   to_ustdint
#define INT    to_stdint
#define UINT24 to_uint24
#define INT24  to_int24

#include <eZ80F92-extra.h>

// for ZDS to access an IO port
#define io_read(vaddr) (*((XSFR)vaddr))
#define io_write(vaddr, value)                                                                                                     \
  { (*((XSFR)vaddr)) = value; }

#define FMT_INT32 "%08lX"
#define FMT_INT24 "%06X"
#define FMT_INT16 "%04X"
#define FMT_INT8  "%02X"
#define FMT_FLT   "%f"

#include <defines.h>
#include <Format.h>
#include <stdarg.h>
#include <stdbool.h>

void                   _u_ultoa(unsigned long n);
extern struct fmt_type __print_fmt;

#define MEMORY_SAFE
// #define DEBUG_VM

#endif
