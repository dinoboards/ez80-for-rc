#include <stdint.h>
#include <stdio.h>
#include "vm.h"
#include "vm_bytecode.h"

vm_t  vm;

#define STACK_SIZE 4096

uint32_t systemCalls(vm_t *vm, uint8_t *args);
void *VM_ArgPtr(intptr_t vmAddr, vm_t *vm);

#if 0
int main(/*const int argc, const char *argv[]*/) {

  printf("&VM: %p\n", &vm);
  printf("&bytecode: %p\n", vm_bytecode);
  printf("&bytecode_ram: %p\n", vm_bytecode_ram);
  printf("&systemCalls: %p\n", systemCalls);

  if (!VM_Create(&vm, vm_bytecode, VM_BYTECODE_SIZE, vm_bytecode_ram, VM_BYTECODE_RAM_SIZE + STACK_SIZE, systemCalls) == 0) {
    printf("VM Create failed\n");
    return -1;
  }

  printf("VM Created\n");

  int32_t retVal = VM_Call(&vm, 1, 2);
  printf("VM_Called with 1, 2 : result = %ld\n", retVal);

  return 0;
}

#else
#include "tests-byte-code.h"

uint8_t ram[2048];

int main(/*const int argc, const char *argv[]*/) {

  for(int i = 0; i < Q3VM_VERIFICATION_SIZE; i++) {

    if (!VM_Create(&vm, q3vm_verification[i].image, q3vm_verification[i].size, ram, 2048 + STACK_SIZE, systemCalls) == 0) {
      printf("VM %s Create failed\n", q3vm_verification[i].name);
      return -1;
    }

    int32_t retVal = VM_Call(&vm, 0);
    if (retVal == q3vm_verification[i].expected)
      printf("Passed: %s\n", q3vm_verification[i].name);
    else {
      printf("Failed: %s\n", q3vm_verification[i].name);
      printf("   Expected %d but got %ld\n", q3vm_verification[i].expected, retVal);
    }
  }

  return 0;
}
#endif

uint32_t systemCalls(vm_t *vm, uint8_t *args) {
  const int id = -1 - VMA_UINT24(0);

  switch (id) {
  case -1: /* PRINTF */
    return printf("%s", (const char *)VMA_PTR(3, vm));

  case -2: /* ERROR */
    return fprintf(stderr, "%s", (const char *)VMA_PTR(3, vm));

  case -3: /* MEMSET */
    memset(VMA_PTR(3, vm), VMA_UINT24(6), VMA_UINT24(9));
    return VMA_UINT24(3);

  case -4: /* MEMCPY */
    memcpy(VMA_PTR(3, vm), VMA_PTR(6, vm), VMA_UINT24(9));
    return VMA_UINT24(3);

  case -254:
    return putchar(VMA_UINT24(3));

  default:
    fprintf(stderr, "Bad system call: %i\n", id);
  }
  return 0;
}

/*************************** */

void *VM_ArgPtr(intptr_t vmAddr, vm_t *vm) {
  if (!vmAddr) {
    return NULL;
  }

  if (vm == NULL) {
    printf("Invalid VM pointer");
    return NULL;
  }

  if (vmAddr < (intptr_t)vm->litLength)
    return (void *)(&vm->codeBase[vm->codeLength + vmAddr]);

  return (void *)(vm->dataBase + (vmAddr));
}
