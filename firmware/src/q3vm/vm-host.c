#include "vm.h"

/* The compiled bytecode calls native functions, defined in this file.
 * Read README.md section "How to add a custom native function" for
 * details.
 * @param[in,out] vm Pointer to virtual machine, prepared by VM_Create.
 * @param[in,out] args Array with arguments of function call.
 * @return Return value handed back to virtual machine. */
intptr_t systemCalls(vm_t *vm, intptr_t *args);

extern const uint8_t  __example_bytecode_qvm[];
extern const uint24_t __example_bytecode_qvm_len;

uint8_t data[256];

void spike() {
  vm_t vm;

  if (VM_Create(&vm, __example_bytecode_qvm, __example_bytecode_qvm_len, data, sizeof(data), systemCalls) == 0) {
    VM_Call(&vm, 0);
  }
}

/* Callback from the VM that something went wrong
 * @param[in] level Error id, see vmErrorCode_t definition.
 * @param[in] error Human readable error text. */
void Com_Error(vmErrorCode_t level, const char *error) {
  // what to do if we have an error!
  //  fprintf(stderr, "Err (%i): %s\n", level, error);
  //  exit(level);
}

void print_string(const char *str);

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
