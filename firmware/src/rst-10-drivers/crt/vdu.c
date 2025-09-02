#include "../../q3vm/host/target-support.h"
#include "../../q3vm/host/vm.h"
#include "../../q3vm/bytecode.h"

extern vm_t vm;

void vdu(uint8_t code) {
  uint8_t stack[1024];

  VM_SetStackStore(&vm, stack, sizeof(stack));
  VM_Call2(&vm, Q3VM_FN_VDU, code);
  VM_SetStackStore(&vm, NULL, 0);
}
