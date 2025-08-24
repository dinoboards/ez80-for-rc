#include "../../q3vm/host/target-support.h"
#include "../../q3vm/host/vm.h"
#include "vdu_vm_bytecode.h"

static vm_t vm;

uint32_t systemCalls(vm_t *vm, uint8_t *args);
void     print_string(const char *str);

void vdu_init() {
  if (VM_Create(&vm, vdu_vm_bytecode, VDU_VM_BYTECODE_SIZE, vdu_vm_bytecode_ram, VDU_VM_BYTECODE_RAM_SIZE, systemCalls)) {
    print_string("VM Create failed.\n\r");
    return;
  }

  VM_Call(&vm, 255);
}

void vdu(uint8_t code) { VM_Call(&vm, 0, code); }

/*   VM HOST INTERFACE   */

#ifdef DEBUG_VM
/* Callback from the VM that something went wrong
 * @param[in] level Error id, see vmErrorCode_t definition.
 * @param[in] error Human readable error text. */
void Com_Error(vmErrorCode_t level, const char *error) {
  // what to do if we have an error!
  //  fprintf(stderr, "Err (%i): %s\n", level, error);
  //  exit(level);
}
#endif

uint32_t systemCalls(vm_t *vm, uint8_t *args) {
  const int id = -1 - args[0];

  switch (id) {
  case -1: /* PRINTF */
    print_string((const char *)VMA_PTR(3, vm));
    return 0;

  case -2: /* ERROR */
    print_string((const char *)VMA_PTR(3, vm));
    return 0;

  case -3: /* MEMSET */
    if (VM_MemoryRangeValid(VMA_UINT24(3) /*addr*/, VMA_UINT24(9) /*len*/, vm) == 0) {
      memset(VMA_PTR(3, vm), VMA_UINT24(6), VMA_UINT24(9));
    }
    return VMA_UINT24(3);

  case -4: /* MEMCPY */
    if (VM_MemoryRangeValid(VMA_UINT24(3) /*addr*/, VMA_UINT24(9) /*len*/, vm) == 0 &&
        VM_MemoryRangeValid(VMA_UINT24(6) /*addr*/, VMA_UINT24(9) /*len*/, vm) == 0) {
      memcpy(VMA_PTR(3, vm), VMA_PTR(6, vm), VMA_UINT24(9));
    }
    return VMA_UINT24(3);

  default:
    return -1;
  }
  return 0;
}
