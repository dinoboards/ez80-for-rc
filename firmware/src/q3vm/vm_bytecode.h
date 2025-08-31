#ifndef __VM_BYTECODE
#define __VM_BYTECODE

#include <stdint.h>

#define VM_BYTECODE_SIZE     9681
#define VM_BYTECODE_RAM_SIZE 734

extern const uint8_t vm_bytecode[VM_BYTECODE_SIZE];

extern uint8_t vm_bytecode_ram[VM_BYTECODE_RAM_SIZE];

#endif
