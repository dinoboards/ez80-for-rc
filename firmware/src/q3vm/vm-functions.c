#include "vm-functions.h"
#include "bytecode.h"
#include "host/vm.h"

extern vm_t vm;

uint24_t vm_call_void(const uint24_t pc) {
  uint8_t stack[200];
  VM_SetStackStore(&vm, stack, sizeof(stack));
  {
    uint8_t r = VM_Call2(&vm, pc, 0);
    VM_SetStackStore(&vm, NULL, 0);
    return r;
  }
}

// void vm_call_1(const uint24_t pc, uint24_t arg) {
//   uint8_t stack[200];
//   VM_SetStackStore(&vm, stack, sizeof(stack));
//   VM_Call2(&vm, pc, arg);
//   VM_SetStackStore(&vm, NULL, 0);
// }

// void vm_call_2(const uint24_t pc, uint24_t arg, uint24_t arg2) {
//   uint8_t stack[200];
//   VM_SetStackStore(&vm, stack, sizeof(stack));
//   VM_Call2(&vm, pc, arg, arg2);
//   VM_SetStackStore(&vm, NULL, 0);
// }

void vm_vdu_colour(void) { vm_call_void(Q3VM_FN_VDU_COLOUR); }
void vm_vdu_gcol(void) { vm_call_void(Q3VM_FN_VDU_GCOL); }
void vm_vdu_mode(void) { vm_call_void(Q3VM_FN_VDU_MODE); }

uint8_t vm_vdp_init(void) { return vm_call_void(Q3VM_FN_VDP_INIT); }
