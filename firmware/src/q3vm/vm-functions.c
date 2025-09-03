#include "vm-functions.h"
#include "bytecode.h"
#include "host/vm.h"

extern vm_t vm;

void vm_call_void(const uint24_t pc) {
  uint8_t stack[200];
  VM_SetStackStore(&vm, stack, sizeof(stack));
  VM_Call2(&vm, pc, 0);
  VM_SetStackStore(&vm, NULL, 0);
}

void vm_call_1(const uint24_t pc, uint24_t arg) {
  uint8_t stack[200];
  VM_SetStackStore(&vm, stack, sizeof(stack));
  VM_Call2(&vm, pc, arg);
  VM_SetStackStore(&vm, NULL, 0);
}
