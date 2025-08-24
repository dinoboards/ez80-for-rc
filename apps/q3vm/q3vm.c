#include <ez80.h>
#include <stdint.h>
#include <stdio.h>
#include "vm.h"
#include "vm_bytecode.h"


/* Reservice 16K for main stack */
#define RESERVE_SPL_SIZE 16386

uint32_t systemCalls(vm_t *vm, uint8_t *args);
void *VM_ArgPtr(intptr_t vmAddr, vm_t *vm);

extern uint8_t end_of_bss[];


/*
Options

q3vm file.qvm
*/

int main(/*const int argc, const char *argv[]*/) {
  FILE *f = fopen("bytecode.qvm", "r");
  printf("fopen: %p\n", f);
  if (f == NULL) {
    printf("Unable to open file.\n");
    return -1;
  }

  uint8_t * const bytecode_start = end_of_bss+4;

  uint8_t* bytecode_read_ptr = bytecode_start;
  uint24_t bytecode_length = 0;
  size_t n;

  do {
    n = fread(bytecode_read_ptr, 1, 128, f);
    bytecode_length += n;
    bytecode_read_ptr += n;
  } while (n > 0);

  fclose(f);

  uint8_t * const RAM_start = bytecode_start + n;
  const uint24_t RAM_length = get_memory_end() - RAM_start - RESERVE_SPL_SIZE;

  vm_t  vm;
  printf("             VM: %06X\n", (uint24_t)&vm);
  printf(" bytecode_start: %06X\n", (uint24_t)bytecode_start);
  printf("bytecode_length: %06X\n", bytecode_length);
  printf("      RAM_Start: %06X\n", (uint24_t)RAM_start);
  printf("     RAM_length: %06X\n", RAM_length);

  if (VM_Create(&vm, bytecode_start, bytecode_length, RAM_start, RAM_length, systemCalls) != 0) {
    printf("VM Create failed\n");
    return -1;
  }

  printf("VM Created\n");

  int32_t retVal = VM_Call(&vm, 1, 2);
  printf("VM_Called with 1, 2 : result = %ld\n", retVal);

  return 0;
}

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
