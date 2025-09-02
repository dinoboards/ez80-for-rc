#include "vm-promoted-fn.h"
#include "includes/host-functions.h"
#include <stdint.h>

/*
This file is to manage and enable the debugging of function host within the vm

This is done by 'promoting' it to live on the host, through the use of conditional compile switches.

To promote a function from vm to host do the following (assuming name of file is vm/file.c):

1. wrap the condition `#ifdef Q3_VM` around the main includes of your file vm/file.c
2. Add an else condition, to include required files  "../../../q3vm/includes/host-functions.h", "../vm-promoted-fn.h" and any others
required
3. For the function of interest, wrap the condition #ifdef ENABLE_FN_HOSTING
4. In `g_syscalls.asm`, add your function name with unique dispatching number

Within this file:
4. in `dispatch_hosted_fn`, create a dispatching case (for number from step 3), calling function of interest.
5. Define pointers to any required globals
6.   in the vm-promoted-fn.h add extern for global pointers and #defines to present same identity to your function of interest
7. In link_vm_globals, assign global pointers to the correct mapped address (see generated vm/bytecode.map file for vm virtaul
address to be applied)

8. A rebuild may be required for ZDS to update dependencies correctly.

*/

void graphic_print_char(uint24_t ch);

#if defined(__FN_ON_HOST__)
uint32_t dispatch_hosted_fn(vm_t *vm, uint8_t *args) {
  const int id = -1 - args[0];

  switch (id) {
  case -128:
    graphic_print_char((VMA_UINT24(3)));
    return 0;

  default:
    return -1;
  }
}

uint8_t *_counter;

void link_vm_globals(vm_t *vm) { _counter = &vm->dataBase[0x191]; }

#endif
