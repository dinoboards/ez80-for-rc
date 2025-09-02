#include "../../q3vm/bytecode.h"
#include "../../q3vm/host/vm.h"

extern vm_t vm;

void vdu_init() {
  uint8_t stack[512];

  VM_SetStackStore(&vm, stack, sizeof(stack));
  VM_Call2(&vm, Q3VM_FN_VDU_INIT, 0);
  VM_SetStackStore(&vm, NULL, 0);
}
