#include "../../q3vm/includes/host-functions.h"
#include "../../q3vm/host/target-support.h"
#include "../../q3vm/host/vm.h"
#include "vdu_vm_bytecode.h"
#include "vm-promoted-fn.h"

vm_t vm;

uint32_t systemCalls(vm_t *vm, uint8_t *args);
void     print_string(const char *str);

void vdu_init() {
  if (VM_Create(&vm, vdu_vm_bytecode, VDU_VM_BYTECODE_SIZE, vdu_vm_bytecode_ram, VDU_VM_BYTECODE_RAM_SIZE, systemCalls)) {
    print_string("VM Create failed.\n\r");
    return;
  }

  link_vm_globals(&vm);

  VM_Call(&vm, 255);
}
