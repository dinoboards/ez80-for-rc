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

void vdp_set_super_graphic_1(void) { vm_call_void(Q3VM_FN_VDP_SET_SUPER_GRAPHIC_1); }

void vdp_set_super_graphic_2(void) { vm_call_void(Q3VM_FN_VDP_SET_SUPER_GRAPHIC_2); }

void vdp_set_graphic_4(void) { vm_call_void(Q3VM_FN_VDP_SET_GRAPHIC_4); }

void vdp_set_super_graphic_21(void) { vm_call_void(Q3VM_FN_VDP_SET_SUPER_GRAPHIC_21); }

void vdp_set_super_graphic_26(void) { vm_call_void(Q3VM_FN_VDP_SET_SUPER_GRAPHIC_26); }

void vdp_set_lines(const uint8_t lines) { vm_call_1(Q3VM_FN_VDP_SET_LINES, lines); }

void vdp_set_refresh(const uint8_t refresh_rate) { vm_call_1(Q3VM_FN_VDP_SET_REFRESH, refresh_rate); }
