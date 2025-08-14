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

#endif
