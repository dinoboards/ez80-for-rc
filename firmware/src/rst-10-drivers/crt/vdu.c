#include "../../q3vm/host/target-support.h"
#include "../../q3vm/host/vm.h"

extern vm_t vm;

void vdu(uint8_t code) {
  uint8_t stack[1024];

  VM_SetStackStore(&vm, stack, sizeof(stack));
  VM_Call(&vm, 0, code);
  VM_SetStackStore(&vm, NULL, 0);
}
