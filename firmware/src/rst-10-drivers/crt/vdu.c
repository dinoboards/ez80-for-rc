#include "../../q3vm/host/target-support.h"
#include "../../q3vm/host/vm.h"
#include "vdu_vm_bytecode.h"

extern vm_t vm;

void vdu(uint8_t code) { VM_Call(&vm, 0, code); }

