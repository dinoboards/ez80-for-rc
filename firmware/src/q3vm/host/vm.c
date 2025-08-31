/*
 __    _  ___  _____    ___   _______     ____  __
|  \  | |/ _ \|_   _|  / _ \ |___ /\ \   / /  \/  |
| | \ | | | | | | |   | | | |  |_ \ \ \ / /| |\/| |
| |\ \| | |_| | | |   | |_| |____) | \ V / | |  | |
|_| \___|\___/  |_|    \__\_______/   \_/  |_|  |_|

   NOT Quake III Arena Virtual Machine
*/

/******************************************************************************
 * SYSTEM INCLUDE FILES
 ******************************************************************************/

#include <stdarg.h>
#include <stdbool.h>

/******************************************************************************
 * PROJECT INCLUDE FILES
 ******************************************************************************/

#include "opcodes.h"
#include "vm.h"

#define FADE   "\033[2m"
#define NORMAL "\033[0m"

/* debug and tracing logging */
#ifdef DEBUG_VM
#define log_1(a)                                  printf(a)
#define log_2(a, b)                               printf(a, b)
#define log_2(a, b)                               printf(a, b)
#define log_3(a, b, c)                            printf(a, b, c)
#define log_4(a, b, c, d)                         printf(a, b, c, d)
#define log_5(a, b, c, d, e)                      printf(a, b, c, d, e)
#define log_6(a1, a2, a3, a4, a5, a6, a7, a8, a9) printf(a1, a2, a3, a4, a5, a6)
#define log_7(a1, a2, a3, a4, a5, a6, a7, a8, a9) printf(a1, a2, a3, a4, a5, a6, a7)
#define log_8(a1, a2, a3, a4, a5, a6, a7, a8, a9) printf(a1, a2, a3, a4, a5, a6, a7, a8)
#define log_9(a1, a2, a3, a4, a5, a6, a7, a8, a9) printf(a1, a2, a3, a4, a5, a6, a7, a8, a9)

#define log1_2(a, b)                                                                                                               \
  if (vm_debugLevel > 1)                                                                                                           \
  printf(a, b)

#define log1_9(a1, a2, a3, a4, a5, a6, a7, a8, a9)                                                                                 \
  if (vm_debugLevel > 1)                                                                                                           \
  printf(a1, a2, a3, a4, a5, a6, a7, a8, a9)

#define log3_2(a, b)                                                                                                               \
  if (vm_debugLevel > 3)                                                                                                           \
  printf(FADE "\t" a NORMAL, b)

#define log3_3(a, b, c)                                                                                                            \
  if (vm_debugLevel > 3)                                                                                                           \
  printf(FADE "\t" a NORMAL, b, c)

#define log3_4(a, b, c, d)                                                                                                         \
  if (vm_debugLevel > 3)                                                                                                           \
  printf(FADE "\t" a NORMAL, b, c, d)

#else

#define log_1(a)
#define log_2(a, b)
#define log_3(a, b, c)
#define log_4(a, b, c, d)
#define log_5(a, b, c, d, e)
#define log_6(a1, a2, a3, a4, a5, a6, a7, a8, a9)
#define log_7(a1, a2, a3, a4, a5, a6, a7, a8, a9)
#define log_8(a1, a2, a3, a4, a5, a6, a7, a8, a9)
#define log_9(a1, a2, a3, a4, a5, a6, a7, a8, a9)

#define log1_2(a, b)

#define log3_2(a, b)
#define log3_3(a, b, c)
#define log3_4(a, b, c, d)

#endif

/** Error console reporting */
#ifdef LOGGING_ENABLED
#define Com_Printf_1(a)                                  printf(a)
#define Com_Printf_2(a, b)                               printf(a, b)
#define Com_Printf_3(a, b, c)                            printf(a, b, c)
#define Com_Printf_4(a, b, c, d)                         printf(a, b, c, d)
#define Com_Printf_5(a, b, c, d, e)                      printf(a, b, c, d, e)
#define Com_Printf_6(a1, a2, a3, a4, a5, a6, a7, a8, a9) printf(a1, a2, a3, a4, a5, a6)
#define Com_Printf_7(a1, a2, a3, a4, a5, a6, a7, a8, a9) printf(a1, a2, a3, a4, a5, a6, a7)
#define Com_Printf_8(a1, a2, a3, a4, a5, a6, a7, a8, a9) printf(a1, a2, a3, a4, a5, a6, a7, a8)
#define Com_Printf_9(a1, a2, a3, a4, a5, a6, a7, a8, a9) printf(a1, a2, a3, a4, a5, a6, a7, a8, a9)
#else
#define Com_Printf_1(a)
#define Com_Printf_2(a, b)
#define Com_Printf_3(a, b, c)
#define Com_Printf_4(a, b, c, d)
#define Com_Printf_5(a, b, c, d, e)
#define Com_Printf_6(a1, a2, a3, a4, a5, a6, a7, a8, a9)
#define Com_Printf_7(a1, a2, a3, a4, a5, a6, a7, a8, a9)
#define Com_Printf_8(a1, a2, a3, a4, a5, a6, a7, a8, a9)
#define Com_Printf_9(a1, a2, a3, a4, a5, a6, a7, a8, a9)
#endif

/******************************************************************************
 * DEFINES
 ******************************************************************************/

/** Virtual machine op stack size in bytes */
#define OPSTACK_SIZE 1024

/** Max number of arguments to pass from a vm to engine's syscall handler
 * function for the vm.
 * syscall number + 15 arguments */
#define MAX_VMSYSCALL_ARGS 16

/** Max number of arguments to pass from engine to vm's vmMain function.
 * command number + 3 arguments */
#define MAX_VMMAIN_ARGS 4

/** Max. size of BSS section */
#define VM_MAX_BSS_LENGTH 10485760

/******************************************************************************
 * TYPEDEFS
 ******************************************************************************/

/******************************************************************************
 * LOCAL DATA DEFINITIONS
 ******************************************************************************/

#ifdef DEBUG_VM

static uint8_t vm_debugLevel; /**< 0: be quiet, 1: debug msgs, 2: print op codes */

#include "opcodes.c.h"

#endif

/******************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 *******************
 ***********************************************************/
#ifdef MEMORY_SAFE
static bool VM_ValidateHeader(vm_t *const vm, const vmHeader_t *const header, const vm_size_t bytecodeLength);
#endif

/** Run a function from the virtual machine with the interpreter (i.e. no JIT).
 * @param[in] vm Pointer to initialized virtual machine.
 * @param[in] args Arguments for function call.
 * @return Return value of the function call. */
static ustdint_t VM_CallInterpreted(const vm_t _vm, int24_t *args, uint8_t *_opStack);

/******************************************************************************
 * DEBUG FUNCTIONS (only used if DEBUG_VM is defined)
 ******************************************************************************/

#ifdef DEBUG_VM
#include <stdio.h>           /* fopen to read symbols */
#include <stdlib.h>          /* qsort */
#define MAX_TOKEN_CHARS 1024 /**< max length of an individual token */
/* WARNING: DEBUG_VM is not thread safe */
static char  com_token[MAX_TOKEN_CHARS]; /**< helper for COM_Parse */
static int   com_lines;                  /**< helper for COM_Parse */
static int   com_tokenline;              /**< helper for COM_Parse */
static int   ParseHex(const char *text); /**< helper for VM_LoadSymbols */
static char *VM_Indent(vm_t *vm);
/** For profiling, find the symbol behind this value */
static vmSymbol_t *VM_ValueToFunctionSymbol(vm_t *vm, int value);
static const char *VM_ValueToSymbol(vm_t *vm, int value);
/** Load a .map file for the virtual machine. The .map file
 * should have the same path as the .qvm file. */
static void VM_LoadSymbols(vm_t *vm, char *mapfile, uint8_t *debugStorage, int debugStorageLength);
/** Print a stack trace on OP_ENTER if vm_debugLevel is > 0 */
static void VM_StackTrace(vm_t *vm, int vPC, int programStack);

#endif

/******************************************************************************
 * LOCAL INLINE FUNCTIONS AND FUNCTION MACROS
 ******************************************************************************/

#define ARRAY_LEN(x) (sizeof(x) / sizeof(*(x)))
#define Q_ftol(v)    ((long)(v))

/******************************************************************************
 * FUNCTION BODIES
 ******************************************************************************/

#if defined(__clang__) || defined(__GNUC__)
#define UNUSED_ARG __attribute__((unused))
#else
#define UNUSED_ARG
#endif

int VM_Create(vm_t                      *vm,
              const uint8_t *const       bytecode,
              const vm_size_t UNUSED_ARG length,
              uint8_t *const             workingRAM,
              const vm_size_t            workingRAMLength,
              uint32_t (*systemCalls)(vm_t *, uint8_t *),
              vm_aborted_t vm_aborted) {

#ifdef MEMORY_SAFE
  if (vm == NULL) {
    return VM_INVALID_POINTER;
  }

  if (!systemCalls)
    return VM_NO_SYSCALL_CALLBACK;

  if (!bytecode)
    return VM_INVALID_POINTER;
#endif

  {
    const vmHeader_t *const header = (const vmHeader_t *const)bytecode;

    Com_Memset(vm, 0, sizeof(vm_t));
    vm->systemCall       = systemCalls;
    vm->vm_aborted       = vm_aborted;
    vm->codeBase         = &bytecode[sizeof(vmHeader_t)];
    vm->codeLength       = UINT(header->codeLength);
    vm->litBase          = vm->codeBase + vm->codeLength;
    vm->litLength        = UINT(header->litLength);
    vm->dataBase         = workingRAM - UINT(header->litLength);
    vm->dataLength       = UINT(header->dataLength);
    vm->bssBase          = vm->dataBase + UINT(header->dataLength);
    vm->bssLength        = UINT(header->bssLength);
    vm->programStack     = workingRAMLength + vm->litLength - 3;
    vm->workingRAMLength = workingRAMLength;
    vm->vm               = vm;

#ifdef MEMORY_SAFE
    if (VM_ValidateHeader(vm, header, length))
      return -1;
#endif

    /* make sure data section is always initialized with 0
     * (bss would be enough) */
    Com_Memset(workingRAM, 0, workingRAMLength);

    /* copy the intialized data, excluding the lit segment */
    Com_Memcpy(workingRAM, &bytecode[UINT(header->codeLength) + UINT(header->litLength) + sizeof(vmHeader_t)],
               UINT(header->dataLength));

    /* the stack is implicitly at the end of the image */
#ifdef MEMORY_SAFE
    vm->stackBottom = (vm->litLength + vm->dataLength + vm->bssLength + 3);
    vm->stackTop    = vm->programStack;
#endif

#ifdef DEBUG_VM

    printf("Memory Layout:\n");
    printf("  CodeBase Store: %p\n", vm->codeBase);
    printf("  LitBase Store:  %p\n", vm->litBase);
    printf(" dataBase Store:  %p\n", vm->dataBase + UINT(header->litLength));
    printf("  bssBase Store:  %p\n\n", vm->bssBase + UINT(header->litLength));

    printf("  RAM Length:     " FMT_INT24 "\n", vm->workingRAMLength);
    printf("  Code Length:    " FMT_INT24 "\n", vm->codeLength);
    printf("  vData Length:   " FMT_INT24 "\n", vm->litLength + vm->workingRAMLength);
    printf("  Lit Length:     " FMT_INT24 "\n", vm->litLength);
    printf("  Data SEG Start  " FMT_INT24 "\n", vm->litLength);
    printf("  Data SEG Length:" FMT_INT24 "\n", vm->dataLength);
    printf("  Data SEG End:   " FMT_INT24 "\n", vm->litLength + vm->dataLength);
    printf("  BSS Start       " FMT_INT24 "\n", vm->litLength + vm->dataLength);
    printf("  BSS Length:     " FMT_INT24 "\n", vm->bssLength);
    printf("  BSS End:        " FMT_INT24 "\n", vm->litLength + vm->dataLength + vm->bssLength);
#ifdef MEMORY_SAFE
    printf("  PS Bottom:      " FMT_INT24 "\n", vm->stackBottom);
#endif
    printf("  PS Top:         " FMT_INT24 "\n", vm->programStack);
#endif
  }
  return 0;
}

#ifdef DEBUG_VM
int VM_LoadDebugInfo(vm_t *vm, char *mapfileImage, uint8_t *debugStorage, int debugStorageLength) {
  if (vm == NULL) {
    log_1("Invalid vm pointer\n");
    return VM_INVALID_POINTER;
  }
  /* load the map file */
  VM_LoadSymbols(vm, mapfileImage, debugStorage, debugStorageLength);

  log_1("VM:\n");
  log_2(".code length: %6i bytes\n", vm->codeLength);
  log_2("Stack size:   %6i bytes\n", VM_PROGRAM_STACK_SIZE);
  log_2("Allocated memory: %6i bytes\n", vm->workingRAMLength);
  return 0;
}
#endif

#ifdef MEMORY_SAFE
static bool VM_ValidateHeader(vm_t *const vm, const vmHeader_t *const header, const vm_size_t bytecodeLength) {

  if (!header || bytecodeLength <= vm_sizeof(vmHeader_t) || bytecodeLength > VM_MAX_IMAGE_SIZE) {
    Com_Printf_1("Failed.\n");
    return -1;
  }

  if (header->vmMagic == VM_MAGIC) {
    if (UINT(header->codeLength) == 0 || UINT(header->bssLength) > VM_MAX_BSS_LENGTH ||
        UINT(header->codeLength) + UINT(header->litLength) + UINT(header->dataLength) > bytecodeLength) {
      Com_Printf_1("Warning: bad header\n");
      vm->lastError = VM_MALFORMED_HEADER;
      return -1;
    }
  } else {
    Com_Printf_3("Warning: Invalid magic number in header "
                 "Read: 0x%x, expected: 0x%x\n",
                 header->vmMagic, VM_MAGIC);
    vm->lastError = VM_MALFORMED_HEADER;
    return -1;
  }

  {
    /* round up to next power of 2 so all data operations can
       be mask protected */
    /*TODO: we can remove need for lit to be included in dataSegment*/
    const vm_size_t dataLength = UINT(header->dataLength) + UINT(header->bssLength);

    if (dataLength > vm->workingRAMLength) {
      Com_Printf_3("Error: Insufficient ram allocated for VM.  Granted " FMT_INT24 ", image requires " FMT_INT24 "\n",
                   vm->workingRAMLength, dataLength);
      vm->lastError = VM_NOT_ENOUGH_RAM;
      return -1;
    }
  }

  return 0;
}
#endif

/* TODO: Build ez80 platform specific version to just pass through args */
intptr_t VM_Call(vm_t *vm, ustdint_t command, ...) {
  intptr_t r;

#ifdef MEMORY_SAFE
  if (vm == NULL)
    return VM_INVALID_POINTER;

  if (vm->codeLength < 1)
    return VM_NOT_LOADED;

#endif

  vm->lastError = 0;
  {
    int24_t args[MAX_VMMAIN_ARGS];
    va_list ap;
    uint8_t i;
    uint8_t _opStack[OPSTACK_SIZE + 4]; /* 256 4 byte double words + 4 safety bytes */

    // TODO: for ez80 build, this can probably be a memcpy
    args[0] = INT24(command);
    va_start(ap, command);
    for (i = 1; i < (uint8_t)ARRAY_LEN(args); i++) {
      args[i] = va_arg(ap, int24_t);
    }
    va_end(ap);

#ifdef DEBUG_VM
    ++vm->callLevel;
#endif
    r = VM_CallInterpreted(*vm, args, _opStack);
#ifdef DEBUG_VM
    --vm->callLevel;
#endif
  }

  return r;
}

void *VM_ArgPtr(intptr_t vmAddr, vm_t *const vm) {
  if (!vmAddr)
    return NULL;

#ifdef MEMORY_SAFE
  if (vm == NULL) {
    return NULL;
  }
#endif

  if (vmAddr < vm->litLength)
    return (void *)(&vm->codeBase[vm->codeLength + vmAddr]);

  return (void *)(vm->dataBase + (vmAddr));
}

float VM_IntToFloat(int32_t x) {
  union {
    float    f;  /**< float IEEE 754 32-bit single */
    int32_t  i;  /**< int32 part */
    uint32_t ui; /**< unsigned int32 part */
  } fi;
  fi.i = x;

  return fi.f;
}

int32_t VM_FloatToInt(float f) {
  union {
    float    f;  /**< float IEEE 754 32-bit single */
    int32_t  i;  /**< int32 part */
    uint32_t ui; /**< unsigned int32 part */
  } fi;
  fi.f = f;
  return fi.i;
}

#define R2 (*((stack_entry_t *)PC))

#define INT_INCREMENT       sizeof(uint32_t)
#define INT8_INCREMENT      sizeof(uint8_t)
#define INT16_INCREMENT     sizeof(uint16_t)
#define INT24_INCREMENT     sizeof(uint24_t)
#define INT32_INCREMENT     sizeof(uint32_t)
#define MAX_PROGRAM_COUNTER ((unsigned)_vm.vm->codeLength)
#define DISPATCH()          break;

#ifdef MEMORY_SAFE
uint8_t bad_memory[16];
#else
const uint8_t *bad_memory = 0;
#endif

#ifdef MOCK_IO_ACCESS
uint8_t mock_io[4] = {1, 2, 3, 4};
#else
// uint8_t *mock_io = (uint8_t *)0xFFFFFF;
#endif

#ifdef MEMORY_SAFE

bool VM_VerifyReadOK(vm_t *vm, vm_size_t vaddr, int size) {
  if (vaddr > 0xFF0000 && size == 1) {
    return true; /* mem and i/o mapping via host */
  }

  if (vaddr < vm->litLength) {
    if (vaddr + size > vm->litLength) {
      vm->lastError = VM_LIT_ACCESS_ERROR;
      Com_Printf_1("Memory Access Read Error - attempt read beyond LIT segment\n");
      return false;
    }
    return true;
  }

  if (vaddr + size > vm->workingRAMLength + vm->litLength) {
    Com_Printf_5("Attempted to read at " FMT_INT24 "+(" FMT_INT24 ") -- (" FMT_INT24 " " FMT_INT24 ")\n", vaddr, size,
                 vm->workingRAMLength, vm->litLength);
    vm->lastError = VM_RAM_ACCESS_ERROR;
    return false;
  }

  return true;
}

bool VM_VerifyWriteOK(vm_t *vm, vm_size_t vaddr, int size) {
  if (vaddr > 0xFF0000 && size == 1) {
    Com_Printf_1("mocking I/O Access\n");
    return true;
  }

  if (vaddr < vm->litLength) {
    Com_Printf_4("Attempted to write at " FMT_INT24 " -- (" FMT_INT24 " " FMT_INT24 ")\n", vaddr, vm->workingRAMLength,
                 vm->litLength);

    vm->lastError = VM_LIT_ACCESS_ERROR;
    Com_Printf_1("Memory Access Write Error - attempt to write to LIT segment\n");
    return false;
  }

  if (vaddr + size > vm->workingRAMLength + vm->litLength) {
    Com_Printf_4("Attempted to write at " FMT_INT24 " -- (" FMT_INT24 " " FMT_INT24 ")\n", vaddr, vm->workingRAMLength,
                 vm->litLength);
    vm->lastError = VM_RAM_ACCESS_ERROR;
    Com_Printf_1("Memory Access Write Error - attempt to write beyond RAM\n");
    return false;
  }

  return true;
}

#else

#define VM_VerifyReadOK(a, b, c) (1)

#define VM_VerifyWriteOK(a, b, c) (1)

#endif

// For 8 bit access:
// If address > FFFF00, map to Z80 I/O
// if address > FF0000, map to a share host memory range (not yet implemented)

#define VM_WriteAddrByte(vaddr, value)                                                                                             \
  {                                                                                                                                \
    if (!VM_VerifyWriteOK(_vm.vm, vaddr, 1)) {                                                                                     \
      ;                                                                                                                            \
    } else {                                                                                                                       \
                                                                                                                                   \
      if (((vm_size_t)(vaddr) >= 0xFF0000)) {                                                                                      \
        io_write(vaddr, value);                                                                                                    \
      } else {                                                                                                                     \
        _vm.dataBase[(vm_size_t)(vaddr)] = value;                                                                                  \
      }                                                                                                                            \
    }                                                                                                                              \
  }

#define VM_WriteAddrUint16(vaddr, value)                                                                                           \
  {                                                                                                                                \
    if (!VM_VerifyWriteOK(_vm.vm, vaddr, 2)) {                                                                                     \
      ;                                                                                                                            \
    } else {                                                                                                                       \
      *((uint16_t *)&_vm.dataBase[(vm_size_t)(vaddr)]) = value;                                                                    \
    }                                                                                                                              \
  }

#define VM_WriteAddrUint24(vaddr, value)                                                                                           \
  {                                                                                                                                \
    if (!VM_VerifyWriteOK(_vm.vm, vaddr, 3)) {                                                                                     \
      ;                                                                                                                            \
    } else {                                                                                                                       \
      *((uint24_t *)&_vm.dataBase[(vm_size_t)(vaddr)]) = value;                                                                    \
    }                                                                                                                              \
  }

#define VM_WriteAddrUint32(vaddr, value)                                                                                           \
  {                                                                                                                                \
    if (!VM_VerifyWriteOK(_vm.vm, vaddr, 4)) {                                                                                     \
      ;                                                                                                                            \
    } else {                                                                                                                       \
      *((uint32_t *)&_vm.dataBase[(vm_size_t)(vaddr)]) = value;                                                                    \
    }                                                                                                                              \
  }

#define opStack32  ((int32_t *)opStack8)
#define opStackFlt ((float *)opStack8)

#define pop_2_floats()                                                                                                             \
  R0.flt = *((float *)opStack8);                                                                                                   \
  R1.flt = *((float *)(opStack8 - 4));                                                                                             \
  log3_3(FMT_FLT " " FMT_FLT " POP int32\n", R0.flt, R1.flt);                                                                      \
  opStack8 -= 8;

#define pop_2_int32()                                                                                                              \
  R0.int32 = *((int32_t *)opStack8);                                                                                               \
  R1.int32 = *((int32_t *)(opStack8 - 4));                                                                                         \
  log3_3(FMT_INT32 " " FMT_INT32 " POP int32\n", R0.int32, R1.int32);                                                              \
  opStack8 -= 8;

#define pop_2_uint32()                                                                                                             \
  R0.uint32 = *((uint32_t *)opStack8);                                                                                             \
  R1.uint32 = *((uint32_t *)(opStack8 - 4));                                                                                       \
  log3_3(FMT_INT32 " " FMT_INT32 " POP uint32\n", R0.int32, R1.int32);                                                             \
  opStack8 -= 8;

#define pop_1_uint32(Rx)                                                                                                           \
  Rx.int32 = *((uint32_t *)opStack8);                                                                                              \
  log3_2(FMT_INT32 " POP uint32\n", Rx.int32);                                                                                     \
  opStack8 -= 4;

#define pop_1_int32(Rx)                                                                                                            \
  Rx.int32 = *((int32_t *)opStack8);                                                                                               \
  log3_2(FMT_INT32 " POP int32\n", Rx.int32);                                                                                      \
  opStack8 -= 4;

#define pop_2_uint24()                                                                                                             \
  R0.uint24 = *((uint24_t *)opStack8);                                                                                             \
  R1.uint24 = *((uint24_t *)(opStack8 - 4));                                                                                       \
  log3_3(FMT_INT32 " " FMT_INT32 " POP uint24\n", UINT(R0.uint24), UINT(R1.uint24));                                               \
  opStack8 -= 8;

#define pop_2_int24()                                                                                                              \
  R0.int24 = *((int24_t *)opStack8);                                                                                               \
  R1.int24 = *((int24_t *)(opStack8 - 4));                                                                                         \
  log3_3(FMT_INT24 " " FMT_INT24 " POP int24\n", INT(R0.int24), INT(R1.int24));                                                    \
  opStack8 -= 8;

#define pop_1_uint24(Rx)                                                                                                           \
  Rx.uint24 = *((uint24_t *)opStack8);                                                                                             \
  log3_2(FMT_INT24 " POP uint24\n", UINT(Rx.uint24));                                                                              \
  opStack8 -= 4;

#define pop_1_int24(Rx)                                                                                                            \
  Rx.int24 = *((int24_t *)opStack8);                                                                                               \
  log3_2(FMT_INT24 " POP int24\n", INT(Rx.int24));                                                                                 \
  opStack8 -= 4;

#define pop_1_uint16(Rx)                                                                                                           \
  Rx.uint16 = *((uint16_t *)opStack8);                                                                                             \
  log3_2(FMT_INT16 " POP uint16\n", Rx.uint16);                                                                                    \
  opStack8 -= 4;

#define pop_1_int16(Rx)                                                                                                            \
  Rx.int16 = *((int16_t *)opStack8);                                                                                               \
  log3_2(FMT_INT16 " POP int16\n", Rx.int16);                                                                                      \
  opStack8 -= 4;

#define pop_1_int8(Rx)                                                                                                             \
  Rx.int8 = *((int8_t *)opStack8);                                                                                                 \
  log3_2(FMT_INT8 " POP int8\n", Rx.int8);                                                                                         \
  opStack8 -= 4;

#define pop_1_uint8(Rx)                                                                                                            \
  Rx.uint8 = *((uint8_t *)opStack8);                                                                                               \
  log3_2(FMT_INT8 " POP uint8\n", Rx.uint8);                                                                                       \
  opStack8 -= 4;

#define pop_1_float(Rx)                                                                                                            \
  Rx.flt = *((float *)opStack8);                                                                                                   \
  log3_2(FMT_FLT " POP float\n", Rx.flt);                                                                                          \
  opStack8 -= 4;

#define retrieve_1_uint24(Rx)                                                                                                      \
  Rx.uint24 = R_uint24;                                                                                                            \
  log3_2(FMT_INT24 " POP uint24\n", UINT(Rx.uint24));

#define retrieve_1_float(Rx)                                                                                                       \
  Rx.flt = *((float *)opStack8);                                                                                                   \
  log3_2(FMT_FLT " POP float\n", Rx.flt);

#define push_1_float(a)                                                                                                            \
  opStack8 += 4;                                                                                                                   \
  *opStackFlt = a;                                                                                                                 \
  log3_2(FMT_FLT " PUSHED float\n", *opStackFlt);

#define push_1_int32(a)                                                                                                            \
  opStack8 += 4;                                                                                                                   \
  *opStack32 = a;                                                                                                                  \
  log3_2(FMT_INT32 " PUSHED int32\n", *opStack32);

#define assign_1_uint8(a)                                                                                                          \
  *opStack32 = (uint8_t)(a);                                                                                                       \
  log3_2(FMT_INT8 " PUSHED uint8\n", *opStack8);

#define assign_1_uint16(a)                                                                                                         \
  *opStack32 = (uint16_t)(a);                                                                                                      \
  log3_2(FMT_INT16 " PUSHED uint16\n", *opStack32 & 0xFFFF);

#define assign_1_uint24(a)                                                                                                         \
  *opStack32 = UINT((uint24_t)(a));                                                                                                \
  log3_2(FMT_INT24 " PUSHED uint24\n", *opStack32 & 0xFFFFFF);

#define assign_1_uint32(a)                                                                                                         \
  *opStack32 = a;                                                                                                                  \
  log3_2(FMT_INT32 " PUSHED uint32\n", *opStack32);

#define push_1_uint32(a)                                                                                                           \
  {                                                                                                                                \
    opStack8 += 4;                                                                                                                 \
    *opStack32 = a;                                                                                                                \
    log3_2(FMT_INT32 " PUSHED uint32\n", *opStack32);                                                                              \
  }

#define push_1_uint24(a)                                                                                                           \
  opStack8 += 4;                                                                                                                   \
  *opStack32 = (uint32_t)UINT(((uint24_t)(a)));                                                                                    \
  log3_2(FMT_INT32 " PUSHED uint24\n", *opStack32);

#define push_1_int24(a)                                                                                                            \
  opStack8 += 4;                                                                                                                   \
  *opStack32 = (int32_t)INT(((int24_t)(a)));                                                                                       \
  log3_2(FMT_INT32 " PUSHED int24\n", *opStack32);

#define push_1_uint16(a)                                                                                                           \
  {                                                                                                                                \
    opStack8 += 4;                                                                                                                 \
    *opStack32 = (uint32_t)(uint16_t)a;                                                                                            \
    log3_2(FMT_INT32 " PUSHED uint16\n", *opStack32);                                                                              \
  }

#define push_1_int16(a)                                                                                                            \
  opStack8 += 4;                                                                                                                   \
  *opStack32 = (int32_t)(int16_t)a;                                                                                                \
  log3_2(FMT_INT32 " PUSHED int16\n", *opStack32);

#define push_1_uint8(a)                                                                                                            \
  {                                                                                                                                \
    opStack8 += 4;                                                                                                                 \
    *opStack32 = (uint8_t)(a);                                                                                                     \
    log3_2(FMT_INT8 " PUSHED uint8\n", *opStack32);                                                                                \
  }

#define push_1_int8(a)                                                                                                             \
  opStack8 += 4;                                                                                                                   \
  *opStack32 = (int8_t)(a);                                                                                                        \
  log3_2(FMT_INT8 " PUSHED int8\n", *opStack32);

#define R_int8     (*((int8_t *)(opStack8)))
#define R0_int8(k) (*((int8_t *)(opStack8 - k)))
#define R1_int8(k) (*((int8_t *)(opStack8 - k - 4)))

#define R_uint8     (*((uint8_t *)(opStack8)))
#define R0_uint8(k) (*((uint8_t *)(opStack8 - k)))
#define R1_uint8(k) (*((uint8_t *)(opStack8 - k - 4)))

#define R_int16     (*((int16_t *)(opStack8)))
#define R0_int16(k) (*((int16_t *)(opStack8 - k)))
#define R1_int16(k) (*((int16_t *)(opStack8 - k - 4)))

#define R_uint16     (*((uint16_t *)(opStack8)))
#define R0_uint16(k) (*((uint16_t *)(opStack8 - k)))
#define R1_uint16(k) (*((uint16_t *)(opStack8 - k - 4)))

#define R_int24     (*((int24_t *)(opStack8)))
#define R0_int24(k) (*((int24_t *)(opStack8 - k)))
#define R1_int24(k) (*((int24_t *)(opStack8 - k - 4)))

#define R_uint24     (*((uint24_t *)(opStack8)))
#define R0_uint24(k) (*((uint24_t *)(opStack8 - k)))
#define R1_uint24(k) (*((uint24_t *)(opStack8 - k - 4)))

#define R_int32     (*((int32_t *)(opStack8)))
#define R0_int32(k) (*((int32_t *)(opStack8 - k)))
#define R1_int32(k) (*((int32_t *)(opStack8 - k - 4)))

#define R_uint32     (*((uint32_t *)(opStack8)))
#define R0_uint32(k) (*((uint32_t *)(opStack8 - k)))
#define R1_uint32(k) (*((uint32_t *)(opStack8 - k - 4)))

#define R_float     (*((float *)(opStack8)))
#define R0_float(k) (*((float *)(opStack8 - k)))
#define R1_float(k) (*((float *)(opStack8 - k - 4)))

#define op_2_int24_to_1_int24(operation)                                                                                           \
  log3_3(FMT_INT24 " " FMT_INT24 " POP int24\n", INT(R1_int24(0)), INT(R0_int24(0)));                                              \
  opStack8 -= 4;                                                                                                                   \
  log3_3(FMT_INT24 " " #operation " " FMT_INT24 " ", INT(R1_int24(-4)), INT(R0_int24(-4)));                                        \
  R_int24 = INT24(INT(R1_int24(-4)) operation INT(R0_int24(-4)));                                                                  \
  log3_2(FMT_INT24 " PUSHED int24\n", INT(R_int24));

#define op_2_uint24_to_1_uint24(operation)                                                                                         \
  log3_3(FMT_INT24 " " FMT_INT24 " POP uint24\n", UINT(R1_uint24(0)), UINT(R0_uint24(0)));                                         \
  opStack8 -= 4;                                                                                                                   \
  log3_3(FMT_INT24 " " #operation " " FMT_INT24 " ", UINT(R1_uint24(-4)), UINT(R0_uint24(-4)));                                    \
  R_uint24 = UINT24(UINT(R1_uint24(-4)) operation UINT(R0_uint24(-4)));                                                            \
  log3_2(FMT_INT24 " PUSHED uint24\n", UINT(R_uint24));

#define op_1_int24_to_1_int24(operation)                                                                                           \
  log3_2(FMT_INT24 " POP int24\n", INT(R0_int24(0)));                                                                              \
  log3_2(#operation FMT_INT24 " =", INT(R0_int24(0)));                                                                             \
  R_int24 = INT24(operation INT(R0_int24(0)));                                                                                     \
  log3_2(FMT_INT24 " PUSHED int24\n", INT(R_int24));

#define op_1_uint24_to_1_uint24(operation)                                                                                         \
  log3_2(FMT_INT24 " POP uint24\n", UINT(R0_uint24(0)));                                                                           \
  log3_2(#operation FMT_INT24 " =", UINT(R0_uint24(0)));                                                                           \
  R_uint24 = UINT24(operation UINT(R0_uint24(0)));                                                                                 \
  log3_2(FMT_INT24 " PUSHED uint24\n", UINT(R_uint24));

#define op_1_int32_to_1_int32(operation)                                                                                           \
  log3_2(FMT_INT32 " POP int32\n", R0_int32(0));                                                                                   \
  log3_2(#operation FMT_INT32 " =", R0_int32(0));                                                                                  \
  R_int32 = operation R0_int32(0);                                                                                                 \
  log3_2(FMT_INT32 " PUSHED int32\n", R_int32);

#define op_1_float_to_1_float(operation)                                                                                           \
  log3_2(FMT_FLT " POP float\n", R0_float(0));                                                                                     \
  log3_2(#operation FMT_FLT " =", R0_float(0));                                                                                    \
  R_float = operation R0_float(0);                                                                                                 \
  log3_2(FMT_FLT " PUSHED float\n", R_float);

#define op_2_int32_to_1_int32(operation)                                                                                           \
  log3_3(FMT_INT32 " " FMT_INT32 " POP int32\n", R1_int32(0), R0_int32(0));                                                        \
  opStack8 -= 4;                                                                                                                   \
  log3_3(FMT_INT32 " " #operation " " FMT_INT32 " =", R1_int32(-4), R0_int32(-4));                                                 \
  R_int32 = R1_int32(-4) operation R0_int32(-4);                                                                                   \
  log3_2(FMT_INT32 " PUSHED int32\n", R_int32);

#define op_2_uint32_to_1_uint32(operation)                                                                                         \
  log3_3(FMT_INT32 " " FMT_INT32 " POP uint32\n", R1_uint32(0), R0_uint32(0));                                                     \
  opStack8 -= 4;                                                                                                                   \
  log3_3(FMT_INT32 " " #operation " " FMT_INT32 " =", R1_uint32(-4), R0_uint32(-4));                                               \
  R_uint32 = R1_uint32(-4) operation R0_uint32(-4);                                                                                \
  log3_2(FMT_INT32 " PUSHED uint32\n", R_uint32);

#define op_1_uint32_to_1_uint32(operation)                                                                                         \
  log3_2(FMT_INT32 " POP uint32\n", R0_uint32(0));                                                                                 \
  log3_2(#operation FMT_INT32 " =", R0_uint32(0));                                                                                 \
  R_uint32 = ~R0_uint32(0);                                                                                                        \
  log3_2(FMT_INT32 " PUSHED uint32\n", R_uint32);

#define op_2_float_to_1_float(operation)                                                                                           \
  log3_3(FMT_FLT " " FMT_FLT " POP float\n", R1_float(0), R0_float(0));                                                            \
  opStack8 -= 4;                                                                                                                   \
  log3_3(FMT_FLT " " #operation " " FMT_FLT " =", R1_float(-4), R0_float(-4));                                                     \
  R_float = R1_float(-4) operation R0_float(-4);                                                                                   \
  log3_2(FMT_FLT " PUSHED float\n", R_float);

#define op_2_int24_branch(operation)                                                                                               \
  log3_3(FMT_INT24 " " #operation " " FMT_INT24 "\n", INT(R1_int24(0)), INT(R0_int24(0)));                                         \
  if ((INT(R1_int24(0)) operation INT(R0_int24(0))))                                                                               \
    PC += R2.int16;                                                                                                                \
  else                                                                                                                             \
    PC += INT16_INCREMENT;                                                                                                         \
  opStack8 -= 8;

#define op_2_int24_1_branch(operation)                                                                                             \
  log3_3(FMT_INT24 " " #operation " " FMT_INT24 "\n", INT(R1_int24(0)), INT(R0_int24(0)));                                         \
  if ((INT(R1_int24(0)) operation INT(R0_int24(0))))                                                                               \
    PC += R2.int8;                                                                                                                 \
  else                                                                                                                             \
    PC += INT8_INCREMENT;                                                                                                          \
  opStack8 -= 8;

#define op_2_uint24_branch(operation)                                                                                              \
  log3_3(FMT_INT24 " " #operation " " FMT_INT24 "\n", UINT(R1_uint24(0)), UINT(R0_uint24(0)));                                     \
  if ((UINT(R1_uint24(0)) operation UINT(R0_uint24(0))))                                                                           \
    PC += R2.int16;                                                                                                                \
  else                                                                                                                             \
    PC += INT16_INCREMENT;                                                                                                         \
  opStack8 -= 8;

#define op_2_uint24_1_branch(operation)                                                                                            \
  log3_3(FMT_INT24 " " #operation " " FMT_INT24 "\n", UINT(R1_uint24(0)), UINT(R0_uint24(0)));                                     \
  if ((UINT(R1_uint24(0)) operation UINT(R0_uint24(0))))                                                                           \
    PC += R2.int8;                                                                                                                 \
  else                                                                                                                             \
    PC += INT8_INCREMENT;                                                                                                          \
  opStack8 -= 8;

#define op_2_int32_branch(operation)                                                                                               \
  log3_3(FMT_INT32 " " #operation " " FMT_INT32 "\n", R1_int32(0), R0_int32(0));                                                   \
  if ((R1_int32(0) operation R0_int32(0)))                                                                                         \
    PC += R2.int16;                                                                                                                \
  else                                                                                                                             \
    PC += INT16_INCREMENT;                                                                                                         \
  opStack8 -= 8;

#define op_2_uint32_branch(operation)                                                                                              \
  log3_3(FMT_INT32 " " #operation " " FMT_INT32 "\n", R1_uint32(0), R0_uint32(0));                                                 \
  if ((R1_uint32(0) operation R0_uint32(0)))                                                                                       \
    PC += R2.int16;                                                                                                                \
  else                                                                                                                             \
    PC += INT16_INCREMENT;                                                                                                         \
  opStack8 -= 8;

#define op_2_float_branch(operation)                                                                                               \
  log3_3(FMT_FLT " " #operation " " FMT_FLT "\n", R1_float(0), R0_float(0));                                                       \
  if ((R1_float(0) operation R0_float(0)))                                                                                         \
    PC += R2.int16;                                                                                                                \
  else                                                                                                                             \
    PC += INT16_INCREMENT;                                                                                                         \
  opStack8 -= 8;

#ifdef MEMORY_SAFE
#define check_stack_overflow()                                                                                                     \
  if (programStack <= _vm.vm->stackBottom || programStack > _vm.vm->stackTop) {                                                    \
    Com_Printf_1("VM stack overflow\n");                                                                                           \
    _vm.vm->lastError = VM_STACK_OVERFLOW;                                                                                         \
    goto done;                                                                                                                     \
  }

#else
#define check_stack_overflow()
#endif

#ifdef MEMORY_SAFE
#define check_pc_overflow()                                                                                                        \
  if (PC >= PC_end) {                                                                                                              \
    Com_Printf_1("VM pc out of range\n");                                                                                          \
    _vm.vm->lastError = VM_PC_OUT_OF_RANGE;                                                                                        \
    goto done;                                                                                                                     \
  }

#else
#define check_pc_overflow()
#endif

typedef union stack_entry_u {
  int8_t   int8;
  uint8_t  uint8;
  int16_t  int16;
  uint16_t uint16;
  int24_t  int24;
  uint24_t uint24;
  int32_t  int32;
  uint32_t uint32;
  float    flt;
} stack_entry_t;

#define vPC ((int)(PC - _vm.codeBase))

typedef const uint8_t *PC_t;

/*
==============
VM_CallInterpreted

Upon a system call, the stack will look like:

sp+32   parm1
sp+28   parm0
sp+24   return stack
sp+20   return address
sp+16   local1
sp+14   local0
sp+12   arg1
sp+8    arg0
sp+4    return stack
sp      return address

An interpreted function will immediately execute
an OP_ENTER instruction, which will subtract space for
locals from sp
==============
*/

/* FIXME: this needs to be locked to uint24_t to ensure platform agnostic */
static ustdint_t VM_CallInterpreted(const vm_t _vm, int24_t *args, uint8_t *_opStack) {
#ifdef MEMORY_SAFE
  const uint8_t *const PC_end = &_vm.codeBase[_vm.codeLength];
#endif

  uint8_t      *opStack8 = &_opStack[4];
  PC_t          PC;
  ustdint_t     programStack;
  ustdint_t     stackOnEntry;
  stack_entry_t R0, R1;

#ifdef DEBUG_VM
  vmSymbol_t *profileSymbol;
  int         stomped;
#endif

  /* we might be called recursively, so this might not be the very top */
  programStack = stackOnEntry = _vm.programStack;

#ifdef DEBUG_VM
  profileSymbol = VM_ValueToFunctionSymbol(_vm.vm, 0);
  /* uncomment this for debugging breakpoints */
  _vm.vm->breakFunction = 0;
#endif

  PC = _vm.codeBase;
  programStack -= (6 + 3 * MAX_VMMAIN_ARGS);

  memcpy(&_vm.dataBase[programStack + 6], args, MAX_VMMAIN_ARGS * 3);

  *(int24_t *)&_vm.dataBase[programStack + 3] = INT24(0);  /* return stack */
  *(int24_t *)&_vm.dataBase[programStack]     = INT24(-1); /* will terminate the loop on return */

  opStack32[0]  = 0x0000BEEF;
  opStack32[-1] = 0x00000000;

  /* main interpreter loop, will exit when a LEAVE instruction
     grabs the -1 program counter */

  while (1) {
    if (_vm.vm->lastError) {
      _vm.vm->programStack = programStack = stackOnEntry;
      opStack8                            = &_opStack[8];
      goto done;
    }

    log1_2(FMT_INT24 ":\t", vPC);

    check_pc_overflow();
    check_stack_overflow();

#ifdef DEBUG_VM
    if (vm_debugLevel > 1) {
      log1_9("%s%i %s\t(" FMT_INT8 " " FMT_INT24 ");\tSP=" FMT_INT24 ",  R0=" FMT_INT24 ", R1=" FMT_INT24 " \n", VM_Indent(_vm.vm),
             (int)(opStack8 - _opStack), opnames[*PC], *PC, R2.int32, programStack, R0.int32, R1.int32);
    }
    profileSymbol->profileCount++;
#endif /* DEBUG_VM */
    switch (*PC++) {
#if defined(DEBUG_VM) || defined(MEMORY_SAFE)
    default: /* fall through */
    case OP_UNDEF:
      Com_Printf_1("Bad VM instruction\n");
      _vm.vm->lastError = VM_BAD_INSTRUCTION;
      goto done;
#endif
#ifdef DEBUG_VM
    case OP_IGNORE:
      DISPATCH();

    case OP_BREAK:
      _vm.vm->breakCount++;
#endif
      DISPATCH();

    case OP_ADD3: {
      op_2_int24_to_1_int24(+);
      DISPATCH();
    }

    case OP_ADD4: {
      op_2_int32_to_1_int32(+);
      DISPATCH();
    }

    case OP_ADDF4: {
      op_2_float_to_1_float(+);
      DISPATCH();
    }

    case OP_ARG3: {
      pop_1_uint24(R0);

      log_5("  ARG3 *[" FMT_INT24 " + " FMT_INT8 " (" FMT_INT24 ")] = " FMT_INT24 "\n", programStack, R2.int8,
            programStack + R2.int8, UINT(R0.uint24));
      *(uint24_t *)&_vm.dataBase[programStack + R2.int8] = R0.uint24;
      PC++;
      DISPATCH();
    }

    case OP_ARG4: {
      pop_1_uint32(R0);
      log_5("  ARG4 *[" FMT_INT24 " + " FMT_INT8 " (" FMT_INT24 ")] = " FMT_INT24 "\n", programStack, R2.int8,
            programStack + R2.int8, R0.uint32);
      *(uint32_t *)&_vm.dataBase[programStack + R2.int8] = R0.uint32;
      PC++;
      DISPATCH();
    }

    case OP_BAND3: {
      op_2_uint24_to_1_uint24(&);
      DISPATCH();
    }

    case OP_BAND4: {
      op_2_uint32_to_1_uint32(&);
      DISPATCH();
    }

    case OP_BCOM3: {
      op_1_uint24_to_1_uint24(~);
      DISPATCH();
    }

    case OP_BCOM4: {
      op_1_uint32_to_1_uint32(~);
      DISPATCH();
    }

    case OP_BLK_CPY: {
      pop_2_uint24();

      log3_4("memcpy(" FMT_INT24 ", " FMT_INT24 ", " FMT_INT24 ")\n", UINT(R1.uint24), UINT(R0.uint24), UINT(R2.uint24));

#ifdef MEMORY_SAFE
      if (!VM_VerifyReadOK(_vm.vm, UINT(R0.uint24), UINT(R2.uint24)))
        DISPATCH();

      if (!VM_VerifyWriteOK(_vm.vm, UINT(R1.uint24), UINT(R2.uint24)))
        DISPATCH();
#endif

      Com_Memcpy(_vm.dataBase + UINT(R1.uint24),
                 (UINT(R0.uint24) < _vm.litLength) ? &_vm.codeBase[_vm.codeLength + UINT(R0.uint24)]
                                                   : &_vm.dataBase[UINT(R0.uint24)],
                 UINT(R2.uint24));

      PC += INT24_INCREMENT;
      DISPATCH();
    }

    case OP_BLK_CPY_U1: {
      pop_2_uint24();

      log3_4("memcpy(" FMT_INT24 ", " FMT_INT24 ", " FMT_INT24 ")\n", UINT(R1.uint24), UINT(R0.uint24), R2.uint8);

#ifdef MEMORY_SAFE
      if (!VM_VerifyReadOK(_vm.vm, UINT(R0.uint24), R2.uint8))
        DISPATCH();

      if (!VM_VerifyWriteOK(_vm.vm, UINT(R1.uint24), R2.uint8))
        DISPATCH();
#endif

      Com_Memcpy(_vm.dataBase + UINT(R1.uint24),
                 (UINT(R0.uint24) < _vm.litLength) ? &_vm.codeBase[_vm.codeLength + UINT(R0.uint24)]
                                                   : &_vm.dataBase[UINT(R0.uint24)],
                 R2.uint8);

      PC += INT8_INCREMENT;
      DISPATCH();
    }

    case OP_BLK_CPY_U2: {
      pop_2_uint24();

      log3_4("memcpy(" FMT_INT24 ", " FMT_INT24 ", " FMT_INT24 ")\n", UINT(R1.uint24), UINT(R0.uint24), R2.uint16);

#ifdef MEMORY_SAFE
      if (!VM_VerifyReadOK(_vm.vm, UINT(R0.uint24), R2.uint16))
        DISPATCH();

      if (!VM_VerifyWriteOK(_vm.vm, UINT(R1.uint24), R2.uint16))
        DISPATCH();
#endif

      Com_Memcpy(_vm.dataBase + UINT(R1.uint24),
                 (UINT(R0.uint24) < _vm.litLength) ? &_vm.codeBase[_vm.codeLength + UINT(R0.uint24)]
                                                   : &_vm.dataBase[UINT(R0.uint24)],
                 R2.uint16);

      PC += INT16_INCREMENT;
      DISPATCH();
    }

    case OP_BOR3: {
      op_2_uint24_to_1_uint24(|);
      DISPATCH();
    }

    case OP_BOR4: {
      op_2_uint32_to_1_uint32(|);
      DISPATCH();
    }

    case OP_BXOR3: {
      op_2_uint24_to_1_uint24(^);
      DISPATCH();
    }

    case OP_BXOR4: {
      op_2_uint32_to_1_uint32(^);
      DISPATCH();
    }

    case OP_CALL: {
      *(int24_t *)&_vm.dataBase[programStack] = INT24(vPC); /* save current program counter */
      pop_1_int24(R0);

      PC = _vm.codeBase + INT(R0.int24); /* jump to the location on the stack */

      if (INT(R0.int24) < 0) /* system call */
      {
        uint32_t r;
        log_4("%s%i---> systemcall(%i)\n", VM_Indent(_vm.vm), (int)(opStack8 - _opStack), -1 - vPC);

        _vm.vm->programStack = programStack - 3; /* save the stack to allow recursive VM entry */

#ifdef DEBUG_VM
        stomped = INT(*(int24_t *)&_vm.dataBase[programStack + 3]);
#endif
        *(int24_t *)&_vm.dataBase[programStack + 3] = INT24(-1 - vPC); /*command*/

        r = _vm.systemCall(_vm.vm, &_vm.dataBase[programStack + 3]);

#ifdef DEBUG_VM
        /* this is just our stack frame pointer, only needed
           for debugging */
        *(int24_t *)&_vm.dataBase[programStack + 3] = INT24(stomped);
#endif

        push_1_uint32(r);
        PC = _vm.codeBase + INT(*(int24_t *)&_vm.dataBase[programStack]);
        log_4("%s%i<--- %s\n", VM_Indent(_vm.vm), (int)(opStack8 - _opStack), VM_ValueToSymbol(_vm.vm, vPC));
      }
      DISPATCH();
    }

    case OP_CF4I3: {
      log3_2(FMT_FLT " POP float\n", R0_float(0));
      R_int24 = INT24(R0_float(0));
      log3_2(FMT_INT24 " PUSH int24\n", INT(R_int24) & 0xFFFFFF);
      DISPATCH();
    }

    case OP_CF4I4: {
      log3_2(FMT_FLT " POP float\n", R0_float(0));
      R_int32 = R0_float(0);
      log3_2(FMT_INT32 " PUSH int32\n", R_int32);
      DISPATCH();
    }
      /* extend sign I1 to I3*/
    case OP_CI1I3: {
      log3_2(FMT_INT8 " POP int8\n", R0_int8(0) & 0xFF);
      R_int24 = INT24(R0_int8(0));
      log3_2(FMT_INT24 " PUSH int24\n", INT(R_int24) & 0xFFFFFF);
      DISPATCH();
    }

      /* extend sign I2 to I3*/
    case OP_CI2I3: {
      log3_2(FMT_INT16 " POP int16\n", R0_int16(0) & 0xFFFF);
      R_int24 = INT24(R0_int16(0));
      log3_2(FMT_INT24 " PUSH int24\n", INT(R_int24) & 0xFFFFFF);
      DISPATCH();
    }

    case OP_CI3F4: {
      log3_2(FMT_INT24 " POP int24\n", INT(R0_int24(0)) & 0xFFFFFF);
      R_float = (float)INT(R0_int24(0));
      log3_2(FMT_FLT " PUSH float\n", R_float);
      DISPATCH();
    }

    case OP_CI3s4: {
      log3_2(FMT_INT24 " POP int24\n", INT(R0_int24(0)) & 0xFFFFFF);
      R_int32 = (int32_t)INT(R0_int24(0));
      log3_2(FMT_INT32 " PUSH int32\n", R_int32);
      DISPATCH();
    }

    case OP_CI4F4: {
      log3_2(FMT_INT32 " POP int32\n", R0_int32(0));
      R_float = (float)R0_int32(0);
      log3_2(FMT_FLT " PUSH float\n", R_float);
      DISPATCH();
    }

    case OP_CONSTI3_I1: {
      push_1_int24(INT24(R2.int8));
      PC += INT8_INCREMENT;
      DISPATCH();
    }

    case OP_CONSTI3_I2: {
      push_1_int24(INT24(R2.int16));
      PC += INT16_INCREMENT;
      DISPATCH();
    }

    case OP_CONSTP3: {
      push_1_uint24(R2.uint24);
      PC += INT24_INCREMENT;
      DISPATCH();
    }

    case OP_CONSTP3_1: {
      push_1_uint24(UINT24(vPC + R2.int8));
      PC += INT8_INCREMENT;
      DISPATCH();
    }

    case OP_CONSTP3_2: {
      push_1_uint24(UINT24(vPC + R2.int16));
      PC += INT16_INCREMENT;
      DISPATCH();
    }

    case OP_CONSTP3_SC1: {
      push_1_int24(INT24(-1));
      DISPATCH();
    }

    case OP_CONSTP3_SC2: {
      push_1_int24(INT24(-2));
      DISPATCH();
    }

    case OP_CONSTP3_SC3: {
      push_1_int24(INT24(-3));
      DISPATCH();
    }

    case OP_CONSTP3_SC4: {
      push_1_int24(INT24(-4));
      DISPATCH();
    }

    case OP_CONSTP3_SC5: {
      push_1_int24(INT24(-5));
      DISPATCH();
    }

    case OP_CONSTP3_SC6: {
      push_1_int24(INT24(-6));
      DISPATCH();
    }

    case OP_CONSTP3_SC7: {
      push_1_int24(INT24(-7));
      DISPATCH();
    }

    case OP_CONSTP3_SC8: {
      push_1_int24(INT24(-8));
      DISPATCH();
    }

    case OP_CONSTP3_SC9: {
      push_1_int24(INT24(-9));
      DISPATCH();
    }

    case OP_CONSTP3_SC10: {
      push_1_int24(INT24(-10));
      DISPATCH();
    }

    case OP_CONSTP3_SC11: {
      push_1_int24(INT24(-11));
      DISPATCH();
    }

    case OP_CONSTP3_SC12: {
      push_1_int24(INT24(-12));
      DISPATCH();
    }

    case OP_CONSTP3_SC13: {
      push_1_int24(INT24(-13));
      DISPATCH();
    }

    case OP_CONSTP3_SC14: {
      push_1_int24(INT24(-14));
      DISPATCH();
    }

    case OP_CONSTP3_SC15: {
      push_1_int24(INT24(-15));
      DISPATCH();
    }

    case OP_CONSTP3_SCn: {
      log3_2(FMT_INT8 " POP uint8\n", R2.uint8 & 0xFF);
      push_1_int24(INT24(0 - R2.uint8));
      PC += INT8_INCREMENT;
      DISPATCH();
    }

    case OP_CONSTs1: {
      push_1_int8(R2.int8);
      PC += INT8_INCREMENT;
      DISPATCH();
    }

    case OP_CONSTs2: {
      push_1_int16(R2.int16);
      PC += INT16_INCREMENT;
      DISPATCH();
    }

    case OP_CONSTs3: {
      push_1_int24(R2.int24);
      PC += INT24_INCREMENT;
      DISPATCH();
    }

    case OP_CONSTU3_U1: {
      push_1_uint24(UINT24(R2.uint8));
      PC += INT8_INCREMENT;
      DISPATCH();
    }

    case OP_CONSTU3_U2: {
      push_1_uint24(UINT24(R2.uint16));
      PC += INT16_INCREMENT;
      DISPATCH();
    }

    case OP_CONSTs4: {
      push_1_uint32(R2.uint32);
      PC += INT32_INCREMENT;
      DISPATCH();
    }

    case OP_CU1I3: {
      log3_2(FMT_INT8 " POP uint8\n", R0_uint8(0) & 0xFF);
      R_int24 = INT24(R0_uint8(0));
      log3_2(FMT_INT24 " PUSH int24\n", INT(R_int24) & 0xFFFFFF);
      DISPATCH();
    }

    case OP_CU2I3: {
      log3_2(FMT_INT16 " POP uint16\n", R0_uint16(0) & 0xFFFF);
      R_int24 = INT24(R0_uint16(0));
      log3_2(FMT_INT24 " PUSH int24\n", INT(R_int24) & 0xFFFFFF);
      DISPATCH();
    }

    case OP_CU3U4: {
      log3_2(FMT_INT24 " POP uint24\n", UINT(R0_uint24(0)) & 0xFFFFFF);
      R_int32 = UINT(R0_uint24(0));
      log3_2(FMT_INT32 " PUSH int32\n", R_int32);
      DISPATCH();
    }

    case OP_DI: {
      DI();
      DISPATCH();
    }

    case OP_DIVF4:
      opStack8 -= 4;
      opStackFlt[0] = opStackFlt[0] / opStackFlt[1];
      DISPATCH();

    case OP_DIVI3: {
      // clang-format off
      op_2_int24_to_1_int24( / );
      // clang-format on
      DISPATCH();
    }

    case OP_DIVI4: {
      // clang-format off
      op_2_int32_to_1_int32( / );
      // clang-format on
      DISPATCH();
    }

    case OP_DIVU3: {
      // clang-format off
      op_2_uint24_to_1_uint24( / );
      // clang-format on
      DISPATCH();
    }

    case OP_DIVU4: {
      // clang-format off
      op_2_uint32_to_1_uint32( / );
      // clang-format on
      DISPATCH();
    }

    case OP_EI: {
      EI();
      DISPATCH();
    }

    case OP_ENTER: {
#ifdef DEBUG_VM
      uint16_t localsAndArgsSize = R2.int16;
#endif
      programStack -= R2.int16;
      log3_4("FRAME SIZE: " FMT_INT16 " (from " FMT_INT24 " to " FMT_INT24 ")\n", R2.int16, programStack + R2.int16, programStack);

      check_stack_overflow();

      PC += INT16_INCREMENT;

#ifdef DEBUG_VM
      profileSymbol = VM_ValueToFunctionSymbol(_vm.vm, vPC - 3);
      /* save old stack frame for debugging traces */
      *(int24_t *)&_vm.dataBase[programStack + 3] = INT24(programStack + localsAndArgsSize);
      log_4("%s%i---> %s\n", VM_Indent(_vm.vm), (int)(opStack8 - _opStack), VM_ValueToSymbol(_vm.vm, vPC - 3));

      if (vm_debugLevel) {
        {
          /* this is to allow setting breakpoints here in the
           * debugger */
          _vm.vm->breakCount++;
          VM_StackTrace(_vm.vm, vPC - 3, programStack);
        }
      }
#endif
      DISPATCH();
    }

    case OP_EQ3: {
      op_2_uint24_branch(==);
      DISPATCH();
    }

    case OP_EQ3_1: {
      op_2_uint24_1_branch(==);
      DISPATCH();
    }

    case OP_EQ4: {
      op_2_uint32_branch(==);
      DISPATCH();
    }

    case OP_GEF4: {
      op_2_float_branch(>=);
      DISPATCH();
    }

    case OP_GEI3: {
      op_2_int24_branch(>=);
      DISPATCH();
    }

    case OP_GEI3_1: {
      op_2_int24_1_branch(>=);
      DISPATCH();
    }

    case OP_GEI4: {
      op_2_int32_branch(>=);
      DISPATCH();
    }

    case OP_GEU3: {
      op_2_uint24_branch(>=);
      DISPATCH();
    }

    case OP_GEU3_1: {
      op_2_uint24_1_branch(>=);
      DISPATCH();
    }

    case OP_GEU4: {
      op_2_uint32_branch(>=);
      DISPATCH();
    }

    case OP_GTF4: {
      op_2_float_branch(>);
      DISPATCH();
    }

    case OP_GTI3: {
      op_2_int24_branch(>);
      DISPATCH();
    }

    case OP_GTI3_1: {
      op_2_int24_1_branch(>);
      DISPATCH();
    }

    case OP_GTI4: {
      op_2_int32_branch(>);
      DISPATCH();
    }

    case OP_GTU3: {
      op_2_uint24_branch(>);
      DISPATCH();
    }

    case OP_GTU3_1: {
      op_2_uint24_1_branch(>);
      DISPATCH();
    }

    case OP_GTU4: {
      op_2_uint32_branch(>);
      DISPATCH();
    }

    case OP_JUMP: {
      opStack8 -= 4;
      PC = _vm.codeBase + INT(R0_int24(-4));
      log3_3("JUMP by " FMT_INT24 " to " FMT_INT24 "\n", UINT(R0_uint24(-4)), vPC);
      DISPATCH();
    }

    case OP_LEAVE: {
      /* remove our stack frame */
      log3_4("FRAME SIZE: " FMT_INT24 " (from " FMT_INT24 " to " FMT_INT24 ")\n", R2.int16, programStack, programStack + R2.int16);
      programStack += R2.int16;

      /* grab the saved program counter */
      PC = _vm.codeBase + INT(*(int24_t *)&_vm.dataBase[programStack]);
#ifdef DEBUG_VM
      profileSymbol = VM_ValueToFunctionSymbol(_vm.vm, vPC);
#endif
      log_4("%s%i<--- %s\n", VM_Indent(_vm.vm), (int)(opStack8 - _opStack), VM_ValueToSymbol(_vm.vm, vPC));
      /* check for leaving the VM */
      if (vPC == -1)
        goto done;

      DISPATCH();
    }

    case OP_LEF4: {
      op_2_float_branch(<=);
      DISPATCH();
    }

    case OP_LEI3: {
      op_2_int24_branch(<=);
      DISPATCH();
    }

    case OP_LEI3_1: {
      op_2_int24_1_branch(<=);
      DISPATCH();
    }

    case OP_LEI4: {
      op_2_int32_branch(<=);
      DISPATCH();
    }

    case OP_LEU3: {
      op_2_uint24_branch(<=);
      DISPATCH();
    }

    case OP_LEU3_1: {
      op_2_uint24_1_branch(<=);
      DISPATCH();
    }

    case OP_LEU4: {
      op_2_uint32_branch(<=);
      DISPATCH();
    }

    case OP_LOAD1: {
      log3_2("*(" FMT_INT24 ") = ", UINT(R0_uint24(0)));
      if (!VM_VerifyReadOK(_vm.vm, UINT(R0_uint24(0)), 1))
        R_uint8 = 0;
      else {
        if (UINT(R0_uint24(0)) >= 0xFF0000)
          R_uint8 = io_read(UINT(R0_uint24(0)));
        else if (UINT(R0_uint24(0)) < _vm.litLength)
          R_uint8 = *(uint8_t *)&_vm.litBase[UINT(R0_uint24(0))];
        else
          R_uint8 = *(uint8_t *)&_vm.dataBase[UINT(R0_uint24(0))];
      }
      log3_2(FMT_INT8 " PUSHED uint8\n", R_uint16 & 0xFF);
      DISPATCH();
    }

    case OP_LOAD2: {
      log3_2("*(" FMT_INT24 ") = ", UINT(R0_uint24(0)));
      if (!VM_VerifyReadOK(_vm.vm, UINT(R0_uint24(0)), 2))
        R_uint16 = 0;
      else {
        if (UINT(R0_uint24(0)) < _vm.litLength)
          R_uint16 = *(uint16_t *)&_vm.litBase[UINT(R0_uint24(0))];
        else
          R_uint16 = *(uint16_t *)&_vm.dataBase[UINT(R0_uint24(0))];
      }
      log3_2(FMT_INT16 " PUSHED uint16\n", R_uint16 & 0xFFFF);
      DISPATCH();
    }

    case OP_LOAD3: {
      log3_2("*(" FMT_INT24 ") = ", UINT(R0_uint24(0)));
      if (!VM_VerifyReadOK(_vm.vm, UINT(R0_uint24(0)), 3))
        R_uint24 = UINT24(0);
      else {
        if (UINT(R0_uint24(0)) < _vm.litLength)
          R_uint24 = *(uint24_t *)&_vm.litBase[UINT(R0_uint24(0))];
        else
          R_uint24 = *(uint24_t *)&_vm.dataBase[UINT(R0_uint24(0))];
      }
      log3_2(FMT_INT24 " PUSHED uint24\n", UINT(R_uint24));
      DISPATCH();
    }

    case OP_LOAD4: {
      log3_2("*(" FMT_INT24 ") = ", UINT(R0_uint24(0)));
      if (!VM_VerifyReadOK(_vm.vm, UINT(R0_uint24(0)), 4))
        R_uint32 = 0;
      else {
        if (UINT(R0_uint24(0)) < _vm.litLength)
          R_uint32 = *(uint32_t *)&_vm.litBase[UINT(R0_uint24(0))];
        else
          R_uint32 = *(uint32_t *)&_vm.dataBase[UINT(R0_uint24(0))];
      }
      log3_2(FMT_INT32 " PUSHED uint32\n", R_uint32);
      DISPATCH();
    }

    case OP_LOCAL: {
      log3_2("&PS[" FMT_INT8 "]", R2.uint8);
      push_1_uint24(UINT24(R2.uint8 + programStack));
      PC += INT8_INCREMENT;
      DISPATCH();
    }

    case OP_LOCAL_FAR: {
      log3_2("&PS[" FMT_INT16 "]", R2.uint16);
      push_1_uint24(UINT24(R2.uint16 + programStack));
      PC += INT16_INCREMENT;
      DISPATCH();
    }

    case OP_LSH3: {
      op_2_uint24_to_1_uint24(<<);
      DISPATCH();
    }

    case OP_LSH4: {
      op_2_uint32_to_1_uint32(<<);
      DISPATCH();
    }

    case OP_LTF4: {
      op_2_float_branch(<);
      DISPATCH();
    }

    case OP_LTI3: {
      op_2_int24_branch(<);
      DISPATCH();
    }

    case OP_LTI3_1: {
      op_2_int24_1_branch(<);
      DISPATCH();
    }

    case OP_LTI4: {
      op_2_int32_branch(<);
      DISPATCH();
    }

    case OP_LTU3: {
      op_2_uint24_branch(<);
      DISPATCH();
    }

    case OP_LTU3_1: {
      op_2_uint24_1_branch(<);
      DISPATCH();
    }

    case OP_LTU4: {
      op_2_uint32_branch(<);
      DISPATCH();
    }

    case OP_MODI3: {
      /* op_2_int24_to_1_int24(%); */
      log3_3(FMT_INT24 " " FMT_INT24 " POP int24\n", INT(R1_int24(0)), INT(R0_int24(0)));
      opStack8 -= 4;
      log3_3(FMT_INT24 " %% " FMT_INT24 " ", INT(R1_int24(-4)), INT(R0_int24(-4)));
      R_int24 = INT24(INT(R1_int24(-4)) % INT(R0_int24(-4)));
      log3_2(FMT_INT24 " PUSHED int24\n", INT(R_int24));
      DISPATCH();
    }

    case OP_MODI4: {
      /* op_2_int32_to_1_int32(%); */
      log3_3(FMT_INT32 " " FMT_INT32 " POP int32\n", R1_int32(0), R0_int32(0));
      opStack8 -= 4;
      log3_3(FMT_INT32 " %% " FMT_INT32 " =", R1_int32(-4), R0_int32(-4));
      R_int32 = R1_int32(-4) % R0_int32(-4);
      log3_2(FMT_INT32 " PUSHED int32\n", R_int32);
      DISPATCH();
    }

    case OP_MODU3: {
      /* op_2_uint24_to_1_uint24(%); */
      log3_3(FMT_INT24 " " FMT_INT24 " POP uint24\n", UINT(R1_uint24(0)), UINT(R0_uint24(0)));
      opStack8 -= 4;
      log3_3(FMT_INT24 " %% " FMT_INT24 " ", UINT(R1_uint24(-4)), UINT(R0_uint24(-4)));
      R_uint24 = UINT24(UINT(R1_uint24(-4)) % UINT(R0_uint24(-4)));
      log3_2(FMT_INT24 " PUSHED uint24\n", UINT(R_uint24));
      DISPATCH();
    }

    case OP_MODU4: {
      /* op_2_uint32_to_1_uint32(%); */
      log3_3(FMT_INT32 " " FMT_INT32 " POP uint32\n", R1_uint32(0), R0_uint32(0));
      opStack8 -= 4;
      log3_3(FMT_INT32 " %% " FMT_INT32 " =", R1_uint32(-4), R0_uint32(-4));
      R_uint32 = R1_uint32(-4) % R0_uint32(-4);
      log3_2(FMT_INT32 " PUSHED uint32\n", R_uint32);
      DISPATCH();
    }

    case OP_MULF4: {
      op_2_float_to_1_float(*);
      DISPATCH();
    }

    case OP_MULI3: {
      op_2_int24_to_1_int24(*);
      DISPATCH();
    }

    case OP_MULI4: {
      op_2_int32_to_1_int32(*);
      DISPATCH();
    }

    case OP_MULU3: {
      op_2_uint24_to_1_uint24(*);
      DISPATCH();
    }

    case OP_MULU4: {
      op_2_uint32_to_1_uint32(*);
      DISPATCH();
    }

    case OP_NE3: {
      op_2_uint24_branch(!=);
      DISPATCH();
    }

    case OP_NE3_1: {
      op_2_uint24_1_branch(!=);
      DISPATCH();
    }

    case OP_NE4: {
      op_2_uint32_branch(!=);
      DISPATCH();
    }

    case OP_NEF4: {
      op_2_float_branch(!=);
      DISPATCH();
    }

    case OP_NEGF4: {
      op_1_float_to_1_float(-);
      DISPATCH();
    }

    case OP_NEGI3: {
      op_1_int24_to_1_int24(-);
      DISPATCH();
    }

    case OP_NEGI4: {
      op_1_int32_to_1_int32(-);
      DISPATCH();
    }

    case OP_POP: {
      opStack8 -= 4;
      DISPATCH();
    }

    case OP_PUSH: {
      opStack8 += 4;
      DISPATCH();
    }

    case OP_RSHI3: {
      op_2_int24_to_1_int24(>>);
      DISPATCH();
    }

    case OP_RSHI4: {
      op_2_int32_to_1_int32(>>);
      DISPATCH();
    }

    case OP_RSHU3: {
      op_2_uint24_to_1_uint24(>>);
      DISPATCH();
    }

    case OP_RSHU4: {
      op_2_uint32_to_1_uint32(>>);
      DISPATCH();
    }

    case OP_STORE1: {
      VM_WriteAddrByte((vm_operand_t)UINT(R1_uint24(0)), R0_uint8(0));
      log3_3("*(" FMT_INT24 ") = " FMT_INT8 "  MOVE\n", UINT(R1_uint24(0)), R0_uint8(0));
      opStack8 -= 8;
      DISPATCH();
    }

    case OP_STORE2: {
      VM_WriteAddrUint16((vm_operand_t)UINT(R1_uint24(0)), R0_uint16(0));
      log3_3("*(" FMT_INT24 ") = " FMT_INT16 "  MOVE\n", UINT(R1_uint24(0)), R0_uint16(0));
      opStack8 -= 8;
      DISPATCH();
    }

    case OP_STORE3: {
      R0.uint24 = R0_uint24(0);
      VM_WriteAddrUint24((vm_operand_t)UINT(R1_uint24(0)), R0.uint24);
      log3_3("*(" FMT_INT24 ") = " FMT_INT32 "  MOVE (3 bytes)\n", UINT(R1_uint24(0)), UINT(R0_uint24(0)));
      opStack8 -= 8;
      DISPATCH();
    }

    case OP_STORE4: {
      R0.uint32 = R0_uint32(0);
      VM_WriteAddrUint32((vm_operand_t)UINT(R1_uint24(0)), R0.uint32);
      log3_3("*(" FMT_INT24 ") = " FMT_INT32 "  MOVE\n", UINT(R1_uint24(0)), R0.uint32);
      opStack8 -= 8;
      DISPATCH();
    }

    case OP_SUB3: {
      op_2_int24_to_1_int24(-);
      DISPATCH();
    }

    case OP_SUB4: {
      op_2_int32_to_1_int32(-);
      DISPATCH();
    }

    case OP_SUBF4: {
      op_2_float_to_1_float(-);
      DISPATCH();
    }
    }
  }

done:
#ifdef MEMORY_SAFE
  if (!_vm.vm->lastError && opStack32[-1] != 0x0000BEEF) {
    Com_Printf_1("Operation stack error\n");
    _vm.vm->lastError = VM_STACK_ERROR;
  }

  if (_vm.vm->lastError && _vm.vm_aborted)
    _vm.vm_aborted(_vm.vm->lastError);

#endif
  _vm.vm->programStack = stackOnEntry;

  /* return the result of the bytecode computations */

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
  return *opStack32;
#pragma GCC diagnostic pop
#else
  return *opStack32;

#endif
}

/* DEBUG FUNCTIONS */
/* --------------- */

#ifdef DEBUG_VM

size_t strlcpy(char *dest, const char *src, size_t size) {
  size_t ret = strlen(src);

  if (size) {
    size_t len = (ret >= size) ? size - 1 : ret;
    memcpy(dest, src, len);
    dest[len] = '\0';
  }
  return ret;
}

void VM_Debug(const uint8_t level) { vm_debugLevel = level; }

static char *VM_Indent(vm_t *vm) {
  static char *string = "                                        ";
  if (vm->callLevel > 20) {
    return string;
  }
  return string + 2 * (20 - vm->callLevel);
}

static const char *VM_ValueToSymbol(vm_t *vm, int value) {
  vmSymbol_t *sym;
  static char text[MAX_TOKEN_CHARS];

  sym = vm->symbols;
  if (!sym) {
    return "NO SYMBOLS";
  }

  /* find the symbol */
  while (sym->next && sym->next->symValue <= value) {
    sym = sym->next;
  }

  if (value == sym->symValue) {
    return sym->symName;
  }

  sprintf(text, "%s+%i", sym->symName, value - sym->symValue);

  return text;
}

static vmSymbol_t *VM_ValueToFunctionSymbol(vm_t *vm, int value) {
  vmSymbol_t       *sym;
  static vmSymbol_t nullSym;

  sym = vm->symbols;
  if (!sym) {
    return &nullSym;
  }

  while (sym->next && sym->next->symValue <= value) {
    sym = sym->next;
  }

  return sym;
}

static int ParseHex(const char *text) {
  int value;
  int c;

  value = 0;
  while ((c = *text++) != 0) {
    if (c >= '0' && c <= '9') {
      value = value * 16 + c - '0';
      continue;
    }
    if (c >= 'a' && c <= 'f') {
      value = value * 16 + 10 + c - 'a';
      continue;
    }
    if (c >= 'A' && c <= 'F') {
      value = value * 16 + 10 + c - 'A';
      continue;
    }
  }

  return value;
}

static char *SkipWhitespace(char *data, int *hasNewLines) {
  int c;

  while ((c = *data) <= ' ') {
    if (!c) {
      return NULL;
    }
    if (c == '\n') {
      com_lines++;
      *hasNewLines = 1;
    }
    data++;
  }

  return data;
}

static char *COM_Parse(char **data_p) {
  int   c           = 0, len;
  int   hasNewLines = 0;
  char *data;
  int   allowLineBreaks = 1;

  data          = *data_p;
  len           = 0;
  com_token[0]  = 0;
  com_tokenline = 0;

  /* make sure incoming data is valid */
  if (!data) {
    *data_p = NULL;
    return com_token;
  }

  while (1) {
    /* skip whitespace */
    data = SkipWhitespace(data, &hasNewLines);
    if (!data) {
      *data_p = NULL;
      return com_token;
    }
    if (hasNewLines && !allowLineBreaks) {
      *data_p = data;
      return com_token;
    }

    c = *data;

    /* skip double slash comments */
    if (c == '/' && data[1] == '/') {
      data += 2;
      while (*data && *data != '\n') {
        data++;
      }
    }
    /* skip comments */
    else if (c == '/' && data[1] == '*') {
      data += 2;
      while (*data && (*data != '*' || data[1] != '/')) {
        if (*data == '\n') {
          com_lines++;
        }
        data++;
      }
      if (*data) {
        data += 2;
      }
    } else {
      break;
    }
  }

  /* token starts on this line */
  com_tokenline = com_lines;

  /* handle quoted strings */
  if (c == '\"') {
    data++;
    while (1) {
      c = *data++;
      if (c == '\"' || !c) {
        com_token[len] = 0;
        *data_p        = (char *)data;
        return com_token;
      }
      if (c == '\n') {
        com_lines++;
      }
      if (len < MAX_TOKEN_CHARS - 1) {
        com_token[len] = c;
        len++;
      }
    }
  }

  /* parse a regular word */
  do {
    if (len < MAX_TOKEN_CHARS - 1) {
      com_token[len] = c;
      len++;
    }
    data++;
    c = *data;
  } while (c > 32);

  com_token[len] = 0;

  *data_p = (char *)data;
  return com_token;
}

static void VM_LoadSymbols(vm_t *vm, char *mapfile, uint8_t *debugStorage, int debugStorageLength) {
  char        *text_p;
  char        *token;
  vmSymbol_t **prev, *sym;
  int          count;
  int          value;
  int          chars;
  int          segment;

  vm->debugStorage       = debugStorage;
  vm->debugStorageLength = debugStorageLength;

  /* parse the symbols */
  text_p = mapfile;
  prev   = &vm->symbols;
  count  = 0;

  while (1) {
    token = COM_Parse(&text_p);
    if (!token[0]) {
      break;
    }
    segment = ParseHex(token);
    if (segment) {
      COM_Parse(&text_p);
      COM_Parse(&text_p);
      continue; /* only load code segment values */
    }

    token = COM_Parse(&text_p);
    if (!token[0]) {
      log_1("WARNING: incomplete line at end of file\n");
      break;
    }
    value = ParseHex(token);

    token = COM_Parse(&text_p);
    if (!token[0]) {
      log_1("WARNING: incomplete line at end of file\n");
      break;
    }
    chars = strlen(token);
    vm->debugStorageLength -= sizeof(vmSymbol_t) + chars;
    if (vm->debugStorageLength < 0) {
      vm->lastError = VM_MALLOC_FAILED;
      log_1("Sym. pointer malloc failed: out of memory?\n");
      return;
    }

    sym = (vmSymbol_t *)vm->debugStorage;
    vm->debugStorage += sizeof(vmSymbol_t) + chars;

    *prev = sym;

    Com_Memset(sym, 0, sizeof(*sym) + chars);
    prev      = &sym->next;
    sym->next = NULL;

    sym->symValue = value;
    strlcpy(sym->symName, token, chars + 1);

    count++;
  }

  vm->numSymbols = count;
  log_2("%i symbols parsed\n", count);
}

static void VM_StackTrace(vm_t *vm, int _program_counter, int programStack) {
  int count;

  count = 0;
  do {
    log_4("STACK: %s " FMT_INT24 " " FMT_INT24 "\n", VM_ValueToSymbol(vm, _program_counter), _program_counter, programStack);
    programStack     = INT(*(int24_t *)&vm->dataBase[programStack + 3]);
    _program_counter = INT(*(int24_t *)&vm->dataBase[programStack]);
  } while (_program_counter != -1 && ++count < 5);

  if (_program_counter == 0) {
    log_4("STACK: %s " FMT_INT24 " " FMT_INT24 "\n", VM_ValueToSymbol(vm, _program_counter), _program_counter, programStack);
    log_2(FMT_INT24 "\n", INT(*(int24_t *)&vm->dataBase[programStack - 6]));
  }
}

static int VM_ProfileSort(const void *a, const void *b) {
  vmSymbol_t *sa, *sb;

  sa = *(vmSymbol_t **)a;
  sb = *(vmSymbol_t **)b;

  if (sa->profileCount < sb->profileCount) {
    return -1;
  }
  if (sa->profileCount > sb->profileCount) {
    return 1;
  }
  return 0;
}

void VM_VmProfile_f(vm_t *vm) {
  vmSymbol_t **sorted, *sym;
  int          i;
  float        total;

  if (!vm) {
    return;
  }

  if (vm->numSymbols < 1) {
    return;
  }

  vm->debugStorageLength -= (vm->numSymbols * sizeof(*sorted));
  if (vm->debugStorageLength < 0) {
    vm->lastError = VM_MALLOC_FAILED;
    log_1("Symbol pointer malloc failed: out of memory?");
    return;
  }

  sorted = (vmSymbol_t **)vm->debugStorage;

  vm->debugStorage += (vm->numSymbols * sizeof(*sorted));
  sorted[0] = vm->symbols;
  total     = (float)sorted[0]->profileCount;
  for (i = 1; i < vm->numSymbols; i++) {
    sorted[i] = sorted[i - 1]->next;
    total += sorted[i]->profileCount;
  }

  qsort(sorted, vm->numSymbols, sizeof(*sorted), VM_ProfileSort);

  for (i = 0; i < vm->numSymbols; i++) {
    int perc;

    sym = sorted[i];

    perc = (int)(100 * (float)sym->profileCount / total);
    log_4("%2i%% %9i %s\n", perc, sym->profileCount, sym->symName);
    sym->profileCount = 0;
  }

  log_2("    %9.0f total\n", total);
}
#endif
