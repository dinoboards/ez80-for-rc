#include "../../q3vm/host/vm.h"

extern vm_t vm;

void vdu_init() {
  uint8_t stack[512];

  VM_SetStackStore(&vm, stack, sizeof(stack));
  VM_Call(&vm, 255);
  VM_SetStackStore(&vm, NULL, 0);
}
