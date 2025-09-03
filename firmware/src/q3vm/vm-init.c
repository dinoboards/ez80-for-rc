#include "host/target-support.h"
#include "host/vm.h"
#include "includes/host-functions.h"
#include "vm-promoted-fn.h"
#include "vm_bytecode.h"
#include "bytecode.h"

vm_t vm;

extern void     print_string(const char *str);
extern uint32_t systemCalls(vm_t *vm, uint8_t *args);

uint8_t* registers_mirror;

void vm_init() {
  if (VM_Create(&vm, vm_bytecode, VM_BYTECODE_SIZE, vm_bytecode_ram, VM_BYTECODE_RAM_SIZE, systemCalls, NULL)) {
    print_string("VM Create failed.\n\r");
    return;
  }

  registers_mirror = &vm.dataBase[Q3VM_DATA_REGISTERS_MIRROR];

  print_string("vm_init\r\n");

  // link_vm_globals(&vm);
}
