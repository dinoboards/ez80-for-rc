/*
      ___   _______     ____  __
     / _ \ |___ /\ \   / /  \/  |
    | | | |  |_ \ \ \ / /| |\/| |
    | |_| |____) | \ V / | |  | |
     \__\_______/   \_/  |_|  |_|


   Quake III Arena Virtual Machine
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

#ifdef DEBUG_VM

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
#define log3_2(a, b)
#define log3_3(a, b, c)
#define log3_4(a, b, c, d)

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

/* GCC can do "computed gotos" instead of a traditional switch/case
 * interpreter, this speeds up the execution.
 *
 * The following section is from Python's ceval.c file:
 *
 *   Computed GOTOs, or
 *       the-optimization-commonly-but-improperly-known-as-"threaded code"
 *   using gcc's labels-as-values extension
 *   (http://gcc.gnu.org/onlinedocs/gcc/Labels-as-Values.html).
 *
 *   The traditional bytecode evaluation loop uses a "switch" statement, which
 *   decent compilers will optimize as a single indirect branch instruction
 *   combined with a lookup table of jump addresses. However, since the
 *   indirect jump instruction is shared by all opcodes, the CPU will have a
 *   hard time making the right prediction for where to jump next (actually, it
 *   will be always wrong except in the uncommon case of a sequence of several
 *   identical opcodes).
 *   "Threaded code" in contrast, uses an explicit jump table and an explicit
 *   indirect jump instruction at the end of each opcode. Since the jump
 *   instruction is at a different address for each opcode, the CPU will make a
 *   separate prediction for each of these instructions, which is equivalent to
 *   predicting the second opcode of each opcode pair. These predictions have a
 *   much better chance to turn out valid, especially in small bytecode loops.
 *   A mispredicted branch on a modern CPU flushes the whole pipeline and can
 *   cost several CPU cycles (depending on the pipeline depth), and potentially
 *   many more instructions (depending on the pipeline width).  A correctly
 *   predicted branch, however, is nearly free.
 * */
#ifdef __GNUC__
#ifndef DEBUG_VM /* can't use computed gotos in debug mode */
#endif
#endif

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
static ustdint_t VM_CallInterpreted(vm_t *vm, int24_t *args);

/** Executes a block copy operation (memcpy) within currentVM data space.
 * @param[out] dest Pointer (in VM space).
 * @param[in] src Pointer (in VM space).
 * @param[in] n Number of bytes.
 * @param[in,out] vm Current VM */
static void VM_BlockCopy(vm_size_t dest, const vm_size_t src, const vm_size_t n, const vm_t *vm);

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
static void VM_StackTrace(vm_t *vm, int programCounter, int programStack);

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
              uint32_t (*systemCalls)(vm_t *, uint8_t *)) {

#ifdef MEMORY_SAFE
  if (vm == NULL) {
    Com_Error(VM_INVALID_POINTER, "Missing VM");
    return VM_INVALID_POINTER;
  }

  if (!systemCalls)
    VM_AbortError(VM_NO_SYSCALL_CALLBACK, "No systemcalls provided");

  if (!bytecode)
    VM_AbortError(VM_INVALID_POINTER, "bytecode is NULL");
#endif

  {
    const vmHeader_t *const header = (const vmHeader_t *const)bytecode;

    Com_Memset(vm, 0, sizeof(vm_t));
    vm->systemCall       = systemCalls;
    vm->codeBase         = &bytecode[sizeof(vmHeader_t)];
    vm->codeLength       = UINT(header->codeLength);
    vm->litBase          = vm->codeBase + vm->codeLength;
    vm->litLength        = UINT(header->litLength);
    vm->dataBase         = workingRAM - UINT(header->litLength);
    vm->dataLength       = UINT(header->dataLength);
    vm->bssBase          = vm->dataBase + UINT(header->dataLength);
    vm->bssLength        = UINT(header->bssLength);
    vm->programStack     = workingRAMLength - 3;
    vm->workingRAMLength = workingRAMLength;

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
    vm->stackBottom = (vm->dataLength + vm->bssLength);
#endif

#ifdef DEBUG_VM

    printf("Memory Layout:\n");
    printf("  CodeBase Store: %p\n", vm->codeBase);
    printf("  LitBase Store:  %p\n", vm->litBase);
    printf(" dataBase Store:  %p\n", vm->dataBase + UINT(header->litLength));
    printf("  bssBase Store:  %p\n\n", vm->bssBase + UINT(header->litLength));

    printf("  RAM Length:     " FMT_INT24 "\n", vm->workingRAMLength);
    printf("  Code Length:    " FMT_INT24 "\n", vm->codeLength);
    printf("  Lit Length:     " FMT_INT24 "\n", vm->litLength);
    printf("  Data Length:    " FMT_INT24 "\n", vm->dataLength);
    printf("  BSS Length:     " FMT_INT24 "\n", vm->bssLength);
    printf("  BSS End:        " FMT_INT24 "\n", vm->dataLength + vm->bssLength);
    printf("  PS Top:         " FMT_INT24 "\n", vm->programStack);
#ifdef MEMORY_SAFE
    printf("  PS Bottom:      " FMT_INT24 "\n", vm->stackBottom);
#endif
#endif
  }
  return 0;
}

#ifdef DEBUG_VM
int VM_LoadDebugInfo(vm_t *vm, char *mapfileImage, uint8_t *debugStorage, int debugStorageLength) {
  if (vm == NULL) {
    VM_AbortError(VM_INVALID_POINTER, "Invalid vm pointer");
  }
  /* load the map file */
  VM_LoadSymbols(vm, mapfileImage, debugStorage, debugStorageLength);

  Com_Printf("VM:\n");
  Com_Printf(".code length: %6i bytes\n", vm->codeLength);
  /*Com_Printf(".data length: %6i bytes\n", vm->workingRAMLength);
  Com_Printf(".lit  length: %6i bytes\n", vm->litLength);
  Com_Printf(".bss  length: %6i bytes\n", vm->bssLength);*/
  Com_Printf("Stack size:   %6i bytes\n", VM_PROGRAM_STACK_SIZE);
  Com_Printf("Allocated memory: %6i bytes\n", vm->workingRAMLength);
  return 0;
}
#endif

#ifdef MEMORY_SAFE
static bool VM_ValidateHeader(vm_t *const vm, const vmHeader_t *const header, const vm_size_t bytecodeLength) {

  if (!header || bytecodeLength <= vm_sizeof(vmHeader_t) || bytecodeLength > VM_MAX_IMAGE_SIZE) {
    Com_Printf("Failed.\n");
    return -1;
  }

  if (header->vmMagic == VM_MAGIC) {
    if (UINT(header->codeLength) == 0 || UINT(header->bssLength) > VM_MAX_BSS_LENGTH ||
        UINT(header->codeLength) + UINT(header->litLength) + UINT(header->dataLength) > bytecodeLength) {
      Com_Printf("Warning: bad header\n");
      VM_AbortError(VM_MALFORMED_HEADER, "Malformed bytecode image\n");
    }
  } else {
    Com_Printf("Warning: Invalid magic number in header "
               "Read: 0x%x, expected: 0x%x\n",
               header->vmMagic, VM_MAGIC);
    VM_AbortError(VM_MALFORMED_HEADER, "Invalid magic number in header\n");
  }

  {
    /* round up to next power of 2 so all data operations can
       be mask protected */
    /*TODO: we can remove need for lit to be included in dataSegment*/
    const vm_size_t dataLength = UINT(header->dataLength) + UINT(header->bssLength);

    if (dataLength > vm->workingRAMLength) {
      Com_Printf("Error: Insufficient ram allocated for VM.  Granted " FMT_INT24 ", image requires " FMT_INT24 "\n",
                 vm->workingRAMLength, dataLength);
      VM_AbortError(VM_NOT_ENOUGH_RAM, "Insufficient ram allocated for VM\n");
    }
  }

  return 0;
}
#endif

/* TODO: Build ez80 platform specific version to just pass through args */
intptr_t VM_Call(vm_t *vm, ustdint_t command, ...) {
  intptr_t r;

#ifdef MEMORY_SAFE
  if (vm == NULL) {
    Com_Error(VM_INVALID_POINTER, "Missing VM Pointe");
    return VM_INVALID_POINTER;
  }

  if (vm->codeLength < 1)
    VM_AbortError(VM_NOT_LOADED, "VM not loaded");
#endif

  vm->lastError = 0;
  {
    int24_t args[MAX_VMMAIN_ARGS];
    va_list ap;
    uint8_t i;

    args[0] = INT24(command);
    va_start(ap, command);
    for (i = 1; i < (uint8_t)ARRAY_LEN(args); i++) {
      args[i] = va_arg(ap, int24_t);
    }
    va_end(ap);

    ++vm->callLevel;
    r = VM_CallInterpreted(vm, args);
    --vm->callLevel;
  }

  return r;
}

void *VM_ArgPtr(intptr_t vmAddr, vm_t *const vm) {
  if (!vmAddr)
    return NULL;

  if (vm == NULL) {
    VM_Error(VM_INVALID_POINTER, "Invalid VM pointer");
    return NULL;
  }

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

bool VM_MemoryRangeValid(const vm_size_t vmAddr, const vm_size_t len, const vm_t *const vm) {
  if (!vmAddr || !vm)
    return -1;

  if (vmAddr > vm->workingRAMLength || vmAddr + len > vm->workingRAMLength) {
    VM_Error(VM_DATA_OUT_OF_RANGE, "Memory access out of range");
    return -1;
  }

  return 0;
}

static void VM_BlockCopy(vm_size_t dest, const vm_size_t src, const vm_size_t n, const vm_t *vm) {

  if (VM_MemoryRangeValid(src, n, vm))
    return;

  if (VM_MemoryRangeValid(src, n, vm))
    return;

  {
    const uint8_t *true_src = (src < vm->litLength) ? &vm->codeBase[vm->codeLength + src] : &vm->dataBase[src];

    Com_Memcpy(vm->dataBase + dest, true_src, n);
  }
}
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

#define r2 (*((vm_operand_t *)&codeBase[programCounter]))

#define r2_int8  (*((int8_t *)&codeBase[programCounter]))
#define r2_int16 (*((int16_t *)&codeBase[programCounter]))
#define r2_int32 (*((int32_t *)&codeBase[programCounter]))

#define r2_uint8  (codeBase[programCounter])
#define r2_uint16 (*((uint16_t *)&codeBase[programCounter]))
#define r2_uint32 (*((uint32_t *)&codeBase[programCounter]))

#define r2_int24_old  INT(*((int24_t *)&codeBase[programCounter]))
#define r2_uint24_old UINT(*((uint24_t *)&codeBase[programCounter]))

#define r2_int24  (*((int24_t *)&codeBase[programCounter]))
#define r2_uint24 (*((uint24_t *)&codeBase[programCounter]))

#define INT_INCREMENT       sizeof(uint32_t)
#define INT8_INCREMENT      sizeof(uint8_t)
#define INT16_INCREMENT     sizeof(uint16_t)
#define INT24_INCREMENT     sizeof(uint24_t)
#define INT32_INCREMENT     sizeof(uint32_t)
#define MAX_PROGRAM_COUNTER ((unsigned)vm->codeLength)
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
    printf("mocking I/O Access\n");
    return true;
  }

  if (vaddr < vm->litLength) {
    if (vaddr + size > vm->litLength) {
      vm->lastError = VM_LIT_ACCESS_ERROR;
      Com_Error(VM_LIT_ACCESS_ERROR, "Memory Access Read Error - attempt read beyond LIT segment");
      return false;
    }
    return true;
  }

  if (vaddr + size > vm->workingRAMLength + vm->litLength) {
    Com_Printf("Attempted to read at " FMT_INT24 "+(" FMT_INT24 ") -- (" FMT_INT24 " " FMT_INT24 ")\n", vaddr, size,
               vm->workingRAMLength, vm->litLength);
    vm->lastError = VM_RAM_ACCESS_ERROR;
    Com_Error(VM_RAM_ACCESS_ERROR, "Memory Access Read Error - attempt read beyond RAM");
    return false;
  }

  return true;
}

bool VM_VerifyWriteOK(vm_t *vm, vm_size_t vaddr, int size) {
  if (vaddr > 0xFF0000 && size == 1) {
    printf("mocking I/O Access\n");
    return true;
  }

  if (vaddr < vm->litLength) {
    Com_Printf("Attempted to write at " FMT_INT24 " -- (" FMT_INT24 " " FMT_INT24 ")\n", vaddr, vm->workingRAMLength,
               vm->litLength);

    vm->lastError = VM_LIT_ACCESS_ERROR;
    Com_Error(VM_LIT_ACCESS_ERROR, "Memory Access Write Error - attempt to write to LIT segment");
    return false;
  }

  if (vaddr + size > vm->workingRAMLength + vm->litLength) {
    Com_Printf("Attempted to write at " FMT_INT24 " -- (" FMT_INT24 " " FMT_INT24 ")\n", vaddr, vm->workingRAMLength,
               vm->litLength);
    vm->lastError = VM_RAM_ACCESS_ERROR;
    Com_Error(VM_RAM_ACCESS_ERROR, "Memory Access Write Error - attempt to write beyond RAM");
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
#define VM_ReadAddrByte(vaddr, fn)                                                                                                 \
  {                                                                                                                                \
    if (!VM_VerifyReadOK(vm, vaddr, 1)) {                                                                                          \
      fn(*bad_memory);                                                                                                             \
    } else {                                                                                                                       \
                                                                                                                                   \
      if (((vm_size_t)(vaddr) > 0xFF0000)) {                                                                                       \
        fn(io_read(vaddr));                                                                                                        \
      } else {                                                                                                                     \
        fn(*(((vm_size_t)(vaddr) < litLength) ? &codeBase[codeLength + (vm_size_t)(vaddr)] : &dataBase[(vm_size_t)(vaddr)]));      \
      }                                                                                                                            \
    }                                                                                                                              \
  }

#define VM_GetReadAddr(vaddr, size)                                                                                                \
  (!VM_VerifyReadOK(vm, vaddr, size)                                                                                               \
       ? bad_memory                                                                                                                \
       : (((((vm_size_t)(vaddr) < litLength) ? &codeBase[codeLength + (vm_size_t)(vaddr)] : &dataBase[(vm_size_t)(vaddr)]))))

#define VM_WriteAddrByte(vaddr, value)                                                                                             \
  {                                                                                                                                \
    if (!VM_VerifyWriteOK(vm, vaddr, 1)) {                                                                                         \
      ;                                                                                                                            \
    } else {                                                                                                                       \
                                                                                                                                   \
      if (((vm_size_t)(vaddr) > 0xFF0000)) {                                                                                       \
        io_write(vaddr, value);                                                                                                    \
      } else {                                                                                                                     \
        dataBase[(vm_size_t)(vaddr)] = value;                                                                                      \
      }                                                                                                                            \
    }                                                                                                                              \
  }

#define VM_GetWriteAddr(vaddr, size) (!VM_VerifyWriteOK(vm, vaddr, size) ? bad_memory : &dataBase[(vm_size_t)(vaddr)])

#define opStack32  ((int32_t *)opStack8)
#define opStackFlt ((float *)opStack8)

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

#define push_1_float(a)                                                                                                            \
  opStack8 += 4;                                                                                                                   \
  *opStack32 = a;

#define push_1_int32(a)                                                                                                            \
  opStack8 += 4;                                                                                                                   \
  *opStack32 = a;                                                                                                                  \
  log3_2(FMT_INT32 " PUSHED int32\n", *opStack32);

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

typedef union stack_entry_u {
  int8_t   int8;
  uint8_t  uint8;
  int16_t  int16;
  uint16_t uint16;
  int24_t  int24;
  uint24_t uint24;
  int32_t  int32;
  uint32_t uint32;
} stack_entry_t;

/* FIXME: this needs to be locked to uint24_t to ensure platform agnostic */
static ustdint_t VM_CallInterpreted(vm_t *vm, int24_t *args) {
  uint8_t       _opStack[OPSTACK_SIZE + 4] = {0}; /* 256 4 byte double words + 4 safety bytes */
  uint8_t      *opStack8                   = &_opStack[4];
  stdint_t      programCounter;
  ustdint_t     programStack;
  ustdint_t     stackOnEntry;
  uint8_t       opcode;
  stack_entry_t R0, R1;

  uint8_t *const       dataBase   = vm->dataBase;
  const uint8_t *const codeBase   = vm->codeBase;
  const vm_size_t      litLength  = vm->litLength;
  const vm_size_t      codeLength = vm->codeLength;

#ifdef DEBUG_VM
  vmSymbol_t *profileSymbol;
  int         stomped;
#endif

  /* we might be called recursively, so this might not be the very top */
  programStack = stackOnEntry = vm->programStack;

#ifdef DEBUG_VM
  profileSymbol = VM_ValueToFunctionSymbol(vm, 0);
  /* uncomment this for debugging breakpoints */
  vm->breakFunction = 0;
#endif

  programCounter = 0;
  programStack -= (6 + 3 * MAX_VMMAIN_ARGS);

  memcpy(&dataBase[programStack + 6], args, MAX_VMMAIN_ARGS * 3);

  *(int24_t *)&dataBase[programStack + 3] = INT24(0);  /* return stack */
  *(int24_t *)&dataBase[programStack]     = INT24(-1); /* will terminate the loop on return */

  opStack32[0]  = 0x0000BEEF;
  opStack32[-1] = 0x00000000;

  /* main interpreter loop, will exit when a LEAVE instruction
     grabs the -1 program counter */

  while (1) {
    if (vm->lastError) {
      vm->programStack = programStack = stackOnEntry;
      opStack8                        = &_opStack[8];
      goto done;
    }
    R0.int32 = opStack32[0];
    R1.int32 = opStack32[-1];

#ifdef DEBUG_VM
    if (vm_debugLevel > 1) {
      Com_Printf(FMT_INT24 ":\t", programCounter);
    }
#endif

#ifdef MEMORY_SAFE
    if (programCounter >= (stdint_t)vm->codeLength)
      VM_AbortError(VM_PC_OUT_OF_RANGE, "VM pc out of range");

    if (programStack <= vm->stackBottom)
      VM_AbortError(VM_STACK_OVERFLOW, "VM stack overflow");
#endif

    opcode = codeBase[programCounter++];

#ifdef DEBUG_VM
    if (vm_debugLevel > 1) {
      Com_Printf("%s%i %s\t(" FMT_INT8 " " FMT_INT24 ");\tSP=" FMT_INT24 ",  R0=" FMT_INT24 ", R1=" FMT_INT24 " \n", VM_Indent(vm),
                 (int)(opStack8 - _opStack), opnames[opcode], opcode, r2, programStack, R0.int32, R1.int32);
    }
    profileSymbol->profileCount++;
#endif /* DEBUG_VM */
    switch (opcode) {
#if defined(DEBUG_VM) || defined(MEMORY_SAFE)
    default: /* fall through */
#endif
    case OP_UNDEF:
      VM_AbortError(VM_BAD_INSTRUCTION, "Bad VM instruction");

    case OP_IGNORE:
      DISPATCH();
    case OP_BREAK:
#ifdef DEBUG_VM
      vm->breakCount++;
#endif
      DISPATCH();

    case OP_CONSTGP3:
      push_1_uint24(r2_uint24);
      programCounter += INT24_INCREMENT;
      DISPATCH();

    case OP_CONSTGP4:
      opStack8 += 4;
      R1       = R0;
      R0.int32 = *opStack32 = r2_int24_old;
      programCounter += INT24_INCREMENT;
      DISPATCH();

    case OP_LOCAL: {
      log3_2("&PS[" FMT_INT16 "]", r2_uint16);
      push_1_uint24(UINT24(r2_uint16 + programStack));
      programCounter += INT16_INCREMENT;
      DISPATCH();
    }

    case OP_LOAD4: {
      pop_1_uint24(R0);
      log3_2("R0: " FMT_INT24 "", UINT(R0.uint24));
      push_1_uint32(*((uint32_t *)VM_GetReadAddr(UINT(R0.uint24), 4)));
      DISPATCH();
    }

    case OP_LOAD3: {
      pop_1_uint24(R0);
      log3_2("RO: " FMT_INT24 "", UINT(R0.uint24));
      push_1_uint24(*((uint24_t *)VM_GetReadAddr(UINT(R0.uint24), 3)));
      DISPATCH();
    }

    case OP_LOAD2:
      pop_1_uint24(R0);
      log3_2("R0: " FMT_INT24 "", UINT(R0.uint24));
      push_1_uint16(*((uint16_t *)VM_GetReadAddr(UINT(R0.uint24), 2)));
      DISPATCH();

    case OP_LOAD1: {
      pop_1_uint24(R0);
      log3_2("R0: " FMT_INT24 "", UINT(R0.uint24));
      VM_ReadAddrByte((vm_operand_t)UINT(R0.uint24), push_1_uint8);
      DISPATCH();
    }

    case OP_STORE4: {
      pop_1_uint32(R0);
      pop_1_uint24(R1);
      *((uint32_t *)VM_GetWriteAddr(UINT(R1.uint24), 4)) = R0.uint32;
      log3_3("*(" FMT_INT24 ") = " FMT_INT32 "  MOVE\n", UINT(R1.uint24), R0.uint32);
      DISPATCH();
    }

    case OP_STORE3: {
      pop_2_uint24();
      *((uint24_t *)VM_GetWriteAddr(UINT(R1.uint24), 3)) = R0.uint24;
      log3_3("*(" FMT_INT24 ") = " FMT_INT32 "  MOVE (3 bytes)\n", UINT(R1.uint24), UINT(R0.uint24));
      DISPATCH();
    }

    case OP_STORE2:
      pop_1_uint16(R0);
      pop_1_uint24(R1);
      *((uint16_t *)VM_GetWriteAddr(UINT(R1.uint24), 2)) = R0.uint16;
      log3_3("*(" FMT_INT24 ") = " FMT_INT16 "  MOVE\n", UINT(R1.uint24), R0.uint16);
      DISPATCH();

    case OP_STORE1:
      pop_1_uint8(R0);
      pop_1_uint24(R1);
      VM_WriteAddrByte((vm_operand_t)UINT(R1.uint24), R0.uint8);
      log3_3("*(" FMT_INT24 ") = " FMT_INT8 "  MOVE\n", UINT(R1.uint24), R0.uint8);
      DISPATCH();

    case OP_ARG3:
      /* single byte offset from programStack */
#ifdef DEBUG_VM
      printf("  ARG *[" FMT_INT24 " + " FMT_INT8 " (" FMT_INT24 ")] = " FMT_INT24 "\n", programStack, r2_int8,
             programStack + r2_int8, R0.int32);
#endif
      *(int24_t *)&dataBase[programStack + r2_int8] = INT24(R0.uint32);
      opStack8 -= 4;
      programCounter += 1;
      DISPATCH();

    case OP_ARG4:
      /* single byte offset from programStack */
#ifdef DEBUG_VM
      printf("  ARG4 *[" FMT_INT24 " + " FMT_INT8 " (" FMT_INT24 ")] = " FMT_INT24 "\n", programStack, r2_int8,
             programStack + r2_int8, R0.int32);
#endif
      *(int32_t *)&dataBase[programStack + r2_int8] = R0.int32;
      opStack8 -= 4;
      programCounter += 1;
      DISPATCH();

    case OP_ARGF4: {
      /* single byte offset from programStack */
#ifdef DEBUG_VM
      float    f = *((float *)&R0.int32);
      uint32_t i = *((uint32_t *)&R0.int32);

      printf("  ARGF *[" FMT_INT24 " + " FMT_INT8 " (" FMT_INT24 ")] = %f (" FMT_INT32 ") (from: " FMT_INT24 ")\n", programStack,
             r2_int8, programStack + r2_int8, f, i, (int)(opStack8 - _opStack));
#endif

      *((uint32_t *)&dataBase[programStack + r2_int8]) = R0.int32;
      opStack8 -= 4;
      programCounter += 1;
      DISPATCH();
    }

    case OP_BLOCK_COPY:
      VM_BlockCopy(R1.int32, R0.int32, r2_uint24_old, vm);
      programCounter += INT24_INCREMENT;
      opStack8 -= 8;
      DISPATCH();

    case OP_CALL: {
      /* save current program counter */
      *(int24_t *)&dataBase[programStack] = INT24(programCounter);
      pop_1_int24(R0);
      /* jump to the location on the stack */
      programCounter = INT(R0.int24);

      if ((programCounter) < 0) /* system call */
      {
        vm_operand_t r;
#ifdef DEBUG_VM
        if (vm_debugLevel) {
          Com_Printf("%s%i---> systemcall(%i)\n", VM_Indent(vm), (int)(opStack8 - _opStack), -1 - programCounter);
        }
#endif
        /* save the stack to allow recursive VM entry */
        vm->programStack = programStack - 3;

#ifdef DEBUG_VM
        stomped = INT(*(int24_t *)&dataBase[programStack + 3]);
#endif
        *(int24_t *)&dataBase[programStack + 3] = INT24(-1 - programCounter); /*command*/
        r                                       = vm->systemCall(vm, &dataBase[programStack + 3]);

#ifdef DEBUG_VM
        /* this is just our stack frame pointer, only needed
           for debugging */
        *(int24_t *)&dataBase[programStack + 3] = INT24(stomped);
#endif

        /* save return value */
        opStack8 += 4;
        *opStack32     = r;
        programCounter = INT(*(int24_t *)&dataBase[programStack]);
#ifdef DEBUG_VM
        if (vm_debugLevel) {
          Com_Printf("%s%i<--- %s\n", VM_Indent(vm), (int)(opStack8 - _opStack), VM_ValueToSymbol(vm, programCounter));
        }
#endif
      }
#ifdef MEMORY_SAFE
      else if ((unsigned)programCounter >= MAX_PROGRAM_COUNTER)
        VM_AbortError(VM_PC_OUT_OF_RANGE, "VM program counter out of range in OP_CALL");
#endif
      DISPATCH();
    }
    /* push and pop are only needed for discarded or bad function return
       values */
    case OP_PUSH:
      opStack8 += 4;
      DISPATCH();
    case OP_POP:
      opStack8 -= 4;
      DISPATCH();

    case OP_ENTER: {
      const uint16_t localsAndArgsSize = r2_int16;
      programCounter += INT16_INCREMENT;
      programStack -= localsAndArgsSize;
      log3_4("FRAME SIZE: " FMT_INT16 " (from " FMT_INT24 " to " FMT_INT24 ")\n", localsAndArgsSize,
             programStack + localsAndArgsSize, programStack);

#ifdef DEBUG_VM
      profileSymbol = VM_ValueToFunctionSymbol(vm, programCounter - 3);
      /* save old stack frame for debugging traces */
      *(int24_t *)&dataBase[programStack + 3] = INT24(programStack + localsAndArgsSize);
      if (vm_debugLevel) {
        Com_Printf("%s%i---> %s\n", VM_Indent(vm), (int)(opStack8 - _opStack), VM_ValueToSymbol(vm, programCounter - 3));
        {
          /* this is to allow setting breakpoints here in the
           * debugger */
          vm->breakCount++;
          VM_StackTrace(vm, programCounter - 3, programStack);
        }
      }
#endif
      DISPATCH();
    }

    case OP_LEAVE: {
      /* remove our stack frame */
      log3_4("FRAME SIZE: " FMT_INT24 " (from " FMT_INT24 " to " FMT_INT24 ")\n", r2_int16, programStack, programStack + r2_int16);
      programStack += r2_int16;

      /* grab the saved program counter */
      programCounter = INT(*(int24_t *)&dataBase[programStack]);
#ifdef DEBUG_VM
      profileSymbol = VM_ValueToFunctionSymbol(vm, programCounter);
      if (vm_debugLevel) {
        Com_Printf("%s%i<--- %s\n", VM_Indent(vm), (int)(opStack8 - _opStack), VM_ValueToSymbol(vm, programCounter));
      }
#endif
      /* check for leaving the VM */
      if (programCounter == -1)
        goto done;

#ifdef MEMORY_SAFE
      if ((unsigned)programCounter >= (unsigned)vm->codeLength)
        VM_AbortError(VM_PC_OUT_OF_RANGE, "VM program counter out of range in OP_LEAVE");
#endif

      DISPATCH();
    }
      /*
         ===================================================================
         BRANCHES
         ===================================================================
         */

    case OP_JUMP:
#ifdef MEMORY_SAFE
      if ((unsigned)R0.int32 >= MAX_PROGRAM_COUNTER)
        VM_AbortError(VM_PC_OUT_OF_RANGE, "VM program counter out of range in OP_JUMP");
#endif

      programCounter = R0.int32;
      opStack8 -= 4;
      DISPATCH();

    case OP_EQ4: {
      pop_2_int32();
      log3_3(FMT_INT32 " == " FMT_INT32 "\n", R1.int32, R0.int32);
      programCounter = (R1.int32 == R0.int32) ? UINT(r2_uint24) : programCounter + INT24_INCREMENT;
      DISPATCH();
    }

    case OP_EQ3: {
      pop_2_uint24();
      log3_3(FMT_INT24 " == " FMT_INT24 "\n", UINT(R1.uint24), UINT(R0.uint24));
      programCounter = (UINT(R1.uint24) == UINT(R0.uint24)) ? UINT(r2_uint24) : programCounter + INT24_INCREMENT;
      DISPATCH();
    }

    case OP_NE4: {
      pop_2_int32();
      log3_3(FMT_INT32 " != " FMT_INT32 "\n", R1.int32, R0.int32);
      programCounter = (R1.int32 != R0.int32) ? UINT(r2_uint24) : programCounter + INT24_INCREMENT;
      DISPATCH();
    }

    case OP_NE3: {
      pop_2_uint24();
      log3_3(FMT_INT24 " != " FMT_INT24 "\n", UINT(R1.uint24), UINT(R0.uint24));
      programCounter = (UINT(R1.uint24) != UINT(R0.uint24)) ? UINT(r2_uint24) : programCounter + INT24_INCREMENT;
      DISPATCH();
    }

    case OP_LTI4: {
      pop_2_int32();
      log3_3(FMT_INT32 " < " FMT_INT32 "\n", R1.int32, R0.int32);
      programCounter = (R1.int32 < R0.int32) ? UINT(r2_uint24) : programCounter + INT24_INCREMENT;
      DISPATCH();
    }

    case OP_LTI3: {
      pop_2_int24();
      log3_4(FMT_INT24 " < " FMT_INT24 " = %d\n", INT(R1.int24), INT(R0.int24), (INT(R1.int24) < INT(R0.int24)));
      programCounter = (INT(R1.int24) < INT(R0.int24)) ? UINT(r2_uint24) : programCounter + INT24_INCREMENT;
      DISPATCH();
    }

    case OP_LEI4: {
      pop_2_int32();
      log3_3(FMT_INT32 " <= " FMT_INT32 "\n", R1.int32, R0.int32);
      programCounter = (R1.int32 <= R0.int32) ? UINT(r2_uint24) : programCounter + INT24_INCREMENT;
      DISPATCH();
    }

    case OP_LEI3: {
      pop_2_int24();
      log3_3(FMT_INT24 " <= " FMT_INT24 "\n", INT(R1.int24), INT(R0.int24));
      programCounter = (INT(R1.int24) <= INT(R0.int24)) ? UINT(r2_uint24) : programCounter + INT24_INCREMENT;
      DISPATCH();
    }

    case OP_GTI4: {
      pop_2_int32();
      log3_3(FMT_INT32 " > " FMT_INT32 "\n", R1.int32, R0.int32);
      programCounter = (R1.int32 > R0.int32) ? UINT(r2_uint24) : programCounter + INT24_INCREMENT;
      DISPATCH();
    }

    case OP_GEI4: {
      pop_2_int32();
      log3_3(FMT_INT32 " >= " FMT_INT32 "\n", R1.int32, R0.int32);
      programCounter = (R1.int32 >= R0.int32) ? UINT(r2_uint24) : programCounter + INT24_INCREMENT;
      DISPATCH();
    }

    case OP_GEI3: {
      pop_2_int24();
      log3_3(FMT_INT24 " >= " FMT_INT24 "\n", INT(R1.int24), INT(R0.int24));
      programCounter = (INT(R1.int24) >= INT(R0.int24)) ? UINT(r2_uint24) : programCounter + INT24_INCREMENT;
      DISPATCH();
    }

    case OP_GTI3: {
      pop_2_int24();
      log3_3(FMT_INT24 " > " FMT_INT24 "\n", INT(R1.int24), INT(R0.int24));
      programCounter = (INT(R1.int24) > INT(R0.int24)) ? UINT(r2_uint24) : programCounter + INT24_INCREMENT;
      DISPATCH();
    }

    case OP_LTU:
      opStack8 -= 8;
      if (((unsigned)R1.int32) < ((unsigned)R0.int32)) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }
    case OP_LEU:
      opStack8 -= 8;
      if (((unsigned)R1.int32) <= ((unsigned)R0.int32)) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }
    case OP_GTU4:
      opStack8 -= 8;
      if (((unsigned)R1.int32) > ((unsigned)R0.int32)) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }
    case OP_GEU4:
      opStack8 -= 8;
      if (((unsigned)R1.int32) >= ((unsigned)R0.int32)) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }

    case OP_EQF4:
      opStack8 -= 8;

      if (opStackFlt[1] == opStackFlt[2]) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }
    case OP_NEF:
      opStack8 -= 8;

      if (opStackFlt[1] != opStackFlt[2]) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }
    case OP_LTF:
      opStack8 -= 8;

      if (opStackFlt[1] < opStackFlt[2]) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }
    case OP_LEF:
      opStack8 -= 8;

      if (opStackFlt[1] <= opStackFlt[2]) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }
    case OP_GTF4:
      opStack8 -= 8;

      if (opStackFlt[1] > opStackFlt[2]) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }
    case OP_GEF4:
      opStack8 -= 8;

      if (opStackFlt[1] >= opStackFlt[2]) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }

      /*===================================================================*/

    case OP_NEGI4:
      pop_1_int32(R0);
      push_1_int32(-R0.int32);
      DISPATCH();

    case OP_NEGI3:
      pop_1_int24(R0);
      push_1_int24(INT24(-INT(R0.int24)));
      DISPATCH();

    case OP_ADD3:
      pop_2_int24();
      log3_3(FMT_INT32 " + " FMT_INT32 " = ", INT(R1.int24), INT(R0.int24));
      push_1_int24(INT24(INT(R1.int24) + INT(R0.int24)));
      DISPATCH();

    case OP_ADD4: {
      pop_2_int32();
      log3_3(FMT_INT32 " + " FMT_INT32 " =", R1.int32, R0.int32);
      push_1_int32(R1.int32 + R0.int32);
      DISPATCH();
    }

    case OP_SUB4:
      log3_4(FMT_INT32 " - " FMT_INT32 " = " FMT_INT32 " PUSHED\n", R1.int32, R0.int32, R1.int32 - R0.int32);
      opStack8 -= 4;
      *opStack32 = R1.int32 - R0.int32;
      DISPATCH();

    case OP_SUB3: {
      pop_2_int24();
      log3_3(FMT_INT32 " - " FMT_INT32 " = ", INT(R1.int24), INT(R0.int24));
      push_1_int24(INT24(INT(R1.int24) - INT(R0.int24)));
      DISPATCH();
    }

    case OP_DIVI: {
      pop_2_int32();
      log3_3(FMT_INT32 " / " FMT_INT32 " =", R1.int32, R0.int32);
      push_1_int32(R1.int32 / R0.int32);
      DISPATCH();
    }

    case OP_DIVI3: {
      pop_2_int24();
      log3_3(FMT_INT32 " / " FMT_INT32 " =", INT(R1.int24), INT(R0.int24));
      push_1_int24(INT24(INT(R1.int24) / INT(R0.int24)));
      DISPATCH();
    }

    case OP_DIVU:
      opStack8 -= 4;
      *opStack32 = ((unsigned)R1.int32) / ((unsigned)R0.int32);
      DISPATCH();

    case OP_MODI4:
      opStack8 -= 4;
      *opStack32 = R1.int32 % R0.int32;
      DISPATCH();

    case OP_MODI3: {
      pop_2_int24();
      log3_3(FMT_INT32 " %% " FMT_INT32 " = ", INT(R1.int24), INT(R0.int24));
      push_1_int24(INT24(INT(R1.int24) % INT(R0.int24)));
      DISPATCH();
    }

    case OP_MODU4:
      opStack8 -= 4;
      *opStack32 = ((unsigned)R1.int32) % ((unsigned)R0.int32);
      DISPATCH();

    case OP_MULI4: {
      pop_2_int32();
      log3_3(FMT_INT32 " * " FMT_INT32 " =", R1.int32, R0.int32);
      push_1_int32(R1.int32 * R0.int32);
      DISPATCH();
    }

    case OP_MULI3: {
      pop_2_int24();
      log3_3(FMT_INT32 " * " FMT_INT32 " = ", INT(R1.int24), INT(R0.int24));
      push_1_int24(INT24(INT(R1.int24) * INT(R0.int24)));
      DISPATCH();
    }

    case OP_MULU4:
      opStack8 -= 4;
      *opStack32 = ((unsigned)R1.int32) * ((unsigned)R0.int32);
      DISPATCH();

    case OP_BAND4: {
      pop_2_uint32();
      log3_3(FMT_INT32 " & " FMT_INT32 " =", R1.int32, R0.int32);
      push_1_uint32(R1.int32 & R0.int32);
      DISPATCH();
    }

    case OP_BAND3: {
      pop_2_uint24();
      log3_3(FMT_INT32 " & " FMT_INT32 " = ", UINT(R1.uint24), UINT(R0.uint24));
      push_1_uint24(UINT24(UINT(R1.uint24) & UINT(R0.uint24)));
      DISPATCH();
    }

    case OP_BOR4: {
      pop_2_uint32();
      log3_3(FMT_INT32 " | " FMT_INT32 " =", R1.int32, R0.int32);
      push_1_uint32(R1.int32 | R0.int32);
      DISPATCH();
    }

    case OP_BOR3: {
      pop_2_uint24();
      log3_3(FMT_INT32 " | " FMT_INT32 " = ", UINT(R1.uint24), UINT(R0.uint24));
      push_1_uint24(UINT24(UINT(R1.uint24) | UINT(R0.uint24)));
      DISPATCH();
    }

    case OP_BXOR4: {
      pop_2_uint32();
      log3_3(FMT_INT32 " ^ " FMT_INT32 " =", R1.int32, R0.int32);
      push_1_uint32(R1.int32 ^ R0.int32);
      DISPATCH();
    }

    case OP_BXOR3: {
      pop_2_uint24();
      log3_3(FMT_INT32 " ^ " FMT_INT32 " = ", UINT(R1.uint24), UINT(R0.uint24));
      push_1_uint24(UINT24(UINT(R1.uint24) ^ UINT(R0.uint24)));
      DISPATCH();
    }

    case OP_BCOM3: {
      pop_1_uint24(R0);
      log3_2("~" FMT_INT24 " =", UINT24(~UINT((R0.uint24))));
      push_1_uint24(UINT24(~UINT((R0.uint24))));
      DISPATCH();
    }

    case OP_BCOM4: {
      pop_1_uint32(R0);
      log3_2("~" FMT_INT32 " =", R0.int32);
      push_1_uint32(~R0.int32);
      DISPATCH();
    }

    case OP_LSH4: {
      pop_2_uint32();
      log3_3(FMT_INT32 " << " FMT_INT32 " =", R1.int32, R0.int32);
      push_1_uint32(R1.int32 << R0.int32);
      DISPATCH();
    }

    case OP_LSH3: {
      pop_2_uint24();
      log3_3(FMT_INT32 " << " FMT_INT32 " = ", UINT(R1.uint24), UINT(R0.uint24));
      push_1_uint24(UINT24(UINT(R1.uint24) << UINT(R0.uint24)));
      DISPATCH();
    }

    case OP_RSHI4: {
      pop_2_int32();
      log3_3(FMT_INT32 " >> " FMT_INT32 " =", R1.int32, R0.int32);
      push_1_int32(R1.int32 >> R0.int32);
      DISPATCH();
    }

    case OP_RSHI3: {
      pop_2_int24();
      log3_3(FMT_INT32 " >> " FMT_INT32 " = ", INT(R1.int24), INT(R0.int24));
      push_1_int24(INT24(INT(R1.int24) >> INT(R0.int24)));
      DISPATCH();
    }

    case OP_RSHU4:
      opStack8 -= 4;
      *opStack32 = ((unsigned)R1.int32) >> R0.int32;
      DISPATCH();
    case OP_NEGF4:
      opStackFlt[0] = -opStackFlt[0];
      DISPATCH();
    case OP_ADDF4:
      opStack8 -= 4;
      opStackFlt[0] = opStackFlt[0] + opStackFlt[1];
      DISPATCH();
    case OP_SUBF4:
      opStack8 -= 4;
      opStackFlt[0] = opStackFlt[0] - opStackFlt[1];
      DISPATCH();
    case OP_DIVF4:
      opStack8 -= 4;
      opStackFlt[0] = opStackFlt[0] / opStackFlt[1];
      DISPATCH();
    case OP_MULF4:
      opStack8 -= 4;
      opStackFlt[0] = opStackFlt[0] * opStackFlt[1];
      DISPATCH();

    case OP_CI4F4:
      opStackFlt[0] = (float)opStack32[0];
      DISPATCH();

    case OP_CF4I4:
      *opStack32 = Q_ftol(opStackFlt[0]);
      DISPATCH();

    case OP_CI1I4:
      *opStack32 = (int8_t)*opStack32;
      DISPATCH();

    case OP_CI2I4:
      *opStack32 = (int16_t)*opStack32;
      DISPATCH();

    case OP_CI3I4: {
      pop_1_int24(R0);
      push_1_int32((int32_t)(INT(R0.int24)));
      DISPATCH();
    }

    /* convert I3 to I1*/
    case OP_CI3I1: {
      pop_1_int24(R0);
      push_1_int8((int8_t)(INT(R0.int24)));
      DISPATCH();
    }

      /* convert I3 to I2*/
    case OP_CI3I2: {
      pop_1_int24(R0);
      push_1_int16((int16_t)(INT(R0.int24)));
      DISPATCH();
    }

    /* extend sign I2 to I3*/
    case OP_CI2I3: {
      pop_1_int16(R0);
      push_1_int24(INT24(R0.int16));
      DISPATCH();
    }

    /* reduce I4 to I3 */
    case OP_CI4I3: {
      pop_1_int32(R0);
      push_1_int24(INT24(R0.int32));
      DISPATCH();
    }

      /* extend sign I1 to I3*/
    case OP_CI1I3: {
      pop_1_int8(R0);
      push_1_int24(INT24(R0.int8));
      DISPATCH();
    }

    /* reduce store from U3 to U2 */
    /*TODO: optimse by just remove the high byte stored on stack*/
    case OP_CU3U2: {
      pop_1_uint24(R0);
      push_1_uint16((uint16_t)UINT(R0.uint24));
      DISPATCH();
    }

    case OP_CU2I3: {
      pop_1_uint16(R0);
      push_1_int24(INT24(R0.uint16));
      DISPATCH();
    }

    case OP_CI4U3: {
      pop_1_int32(R0);
      push_1_uint24(UINT24((uint32_t)R0.int32));
      DISPATCH();
    }

    case OP_CI3U4: {
      pop_1_int24(R0);
      push_1_uint32((uint32_t)INT(R0.int24));
      DISPATCH();
    }

    case OP_CU4U3: {
      pop_1_uint32(R0);
      push_1_uint24(UINT24((uint32_t)R0.int32));
      DISPATCH();
    }

    case OP_CU3U4: {
      pop_1_uint24(R0);
      push_1_uint32((uint32_t)UINT(R0.uint24));
      DISPATCH();
    }

    case OP_CU4I3: {
      pop_1_uint32(R0);
      push_1_int24(INT24(R0.int32));
      DISPATCH();
    }

    case OP_CONSTU1: {
      push_1_int8(r2_uint8);
      programCounter += INT8_INCREMENT;
      DISPATCH();
    }

    case OP_CONSTI1: {
      push_1_int8(r2_int8);
      programCounter += INT8_INCREMENT;
      DISPATCH();
    }

    case OP_CONSTU2: {
      push_1_uint16(r2_uint16);
      programCounter += INT16_INCREMENT;
      DISPATCH();
    }

    case OP_CONSTI2: {
      push_1_int16(r2_int16);
      programCounter += INT16_INCREMENT;
      DISPATCH();
    }

    case OP_CONSTI3: {
      push_1_int24(r2_int24);
      programCounter += INT32_INCREMENT;
      DISPATCH();
    }

    case OP_CONSTU3: {
      push_1_uint24(r2_uint24);
      programCounter += INT24_INCREMENT;
      DISPATCH();
    }

    case OP_CONSTU4: {
      push_1_uint32(r2_uint32);
      programCounter += INT32_INCREMENT;
      DISPATCH();
    }

    case OP_CONSTI4: {
      push_1_int32(r2);
      programCounter += INT32_INCREMENT;
      DISPATCH();
    }

    case OP_CONSTF4: {
      opStack8 += 4;
      R1       = R0;
      R0.int32 = *opStack32 = r2;
      programCounter += INT32_INCREMENT;
      DISPATCH();
    }

    case OP_CONSTP3: {
      opStack8 += 4;
      R1       = R0;
      R0.int32 = *opStack32 = (vm_operand_t)(int32_t)r2_uint24_old;
      programCounter += INT24_INCREMENT;
      DISPATCH();
    }

    case OP_CONSTP4: {
      opStack8 += 4;
      R1       = R0;
      R0.int32 = *opStack32 = (vm_operand_t)(int32_t)r2_uint24_old;
      programCounter += INT24_INCREMENT;
      DISPATCH();
    }
    }
  }

done:
#ifdef MEMORY_SAFE
  if (opStack32[-1] != 0x0000BEEF)
    VM_AbortError(VM_STACK_ERROR, "Operation stack error");
#endif
  vm->programStack = stackOnEntry;

  /* return the result of the bytecode computations */
  return *opStack32;
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
      Com_Printf("WARNING: incomplete line at end of file\n");
      break;
    }
    value = ParseHex(token);

    token = COM_Parse(&text_p);
    if (!token[0]) {
      Com_Printf("WARNING: incomplete line at end of file\n");
      break;
    }
    chars = strlen(token);
    vm->debugStorageLength -= sizeof(vmSymbol_t) + chars;
    if (vm->debugStorageLength < 0) {
      vm->lastError = VM_MALLOC_FAILED;
      Com_Error(VM_MALLOC_FAILED, "Sym. pointer malloc failed: out of memory?");
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
  Com_Printf("%i symbols parsed\n", count);
}

static void VM_StackTrace(vm_t *vm, int programCounter, int programStack) {
  int count;

  count = 0;
  do {
    Com_Printf("STACK: %s " FMT_INT24 " " FMT_INT24 "\n", VM_ValueToSymbol(vm, programCounter), programCounter, programStack);
    programStack   = INT(*(int24_t *)&vm->dataBase[programStack + 3]);
    programCounter = INT(*(int24_t *)&vm->dataBase[programStack]);
  } while (programCounter != -1 && ++count < 5);

  if (programCounter == 0) {
    Com_Printf("STACK: %s " FMT_INT24 " " FMT_INT24 "\n", VM_ValueToSymbol(vm, programCounter), programCounter, programStack);
    printf(FMT_INT24 "\n", INT(*(int24_t *)&vm->dataBase[programStack - 6]));
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
    Com_Error(VM_MALLOC_FAILED, "Symbol pointer malloc failed: out of memory?");
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
    Com_Printf("%2i%% %9i %s\n", perc, sym->profileCount, sym->symName);
    sym->profileCount = 0;
  }

  Com_Printf("    %9.0f total\n", total);
}
#endif
