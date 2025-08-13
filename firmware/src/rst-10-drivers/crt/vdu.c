#include "../../q3vm/host/target-support.h"
#include "../../q3vm/host/vm.h"
#include "vdu_vm_bytecode.h"

static vm_t vm;

intptr_t systemCalls(vm_t *vm, intptr_t *args);
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

/* Callback from the VM that something went wrong
 * @param[in] level Error id, see vmErrorCode_t definition.
 * @param[in] error Human readable error text. */
void Com_Error(vmErrorCode_t level, const char *error) {
  // what to do if we have an error!
  //  fprintf(stderr, "Err (%i): %s\n", level, error);
  //  exit(level);
}

intptr_t systemCalls(vm_t *vm, intptr_t *args) {
  const int id = -1 - args[0];

  switch (id) {
  case -1: /* PRINT_STRING */
    print_string((const char *)VMA(1, vm));
    return 0;

  case -2: /* ERROR */
    return 0;

  case -3: /* MEMSET */
    if (VM_MemoryRangeValid(args[1] /*addr*/, args[3] /*len*/, vm) == 0) {
      memset(VMA(1, vm), args[2], args[3]);
    }
    return args[1];

  case -4: /* MEMCPY */
    if (VM_MemoryRangeValid(args[1] /*addr*/, args[3] /*len*/, vm) == 0 &&
        VM_MemoryRangeValid(args[2] /*addr*/, args[3] /*len*/, vm) == 0) {
      memcpy(VMA(1, vm), VMA(2, vm), args[3]);
    }
    return args[1];

  default:
    return -1;
  }
  return 0;
}
