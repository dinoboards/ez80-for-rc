#ifdef Q3_VM
#include "vdu/variables.h"
#include <host-functions.h>
#include <stdint.h>
#include "../vm-promoted-fn.h"

#else
#include "../../../q3vm/includes/host-functions.h"
#include <stdint.h>
#include "../vm-promoted-fn.h"

#endif

#ifdef ENABLE_FN_HOSTING

void graphic_print_char(uint8_t ch) {
  if (ch >= ' ' && ch < 126)
    putchar(ch);

  printf("\n%d\n", counter++);
}


#endif

