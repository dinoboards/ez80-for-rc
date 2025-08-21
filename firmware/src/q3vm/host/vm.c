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

static bool VM_ValidateHeader(const vm_t *const vm, const vmHeader_t *const header, const vm_size_t bytecodeLength);

/** Run a function from the virtual machine with the interpreter (i.e. no JIT).
 * @param[in] vm Pointer to initialized virtual machine.
 * @param[in] args Arguments for function call.
 * @return Return value of the function call. */
static ustdint_t VM_CallInterpreted(vm_t *vm, uint32_t *args);

/** Executes a block copy operation (memcpy) within currentVM data space.
 * @param[out] dest Pointer (in VM space).
 * @param[in] src Pointer (in VM space).
 * @param[in] n Number of bytes.
 * @param[in,out] vm Current VM */
static void VM_BlockCopy(vm_size_t dest, const vm_size_t src, const vm_size_t n, const vm_t *vm);

/** Helper function for the _vmf inline function _vmf in vm.h.
 * @param[in] x Number that is actually a IEEE 754 float.
 * @return float number */
extern float _vmf(intptr_t x);

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

bool VM_Create(vm_t                *vm,
               const uint8_t *const bytecode,
               const vm_size_t      length,
               uint8_t *const       workingRAM,
               const vm_size_t      workingRAMLength,
               uint32_t (*systemCalls)(vm_t *, uint8_t *)) {
  if (vm == NULL) {
    Com_Error(VM_INVALID_POINTER, "Invalid vm pointer");
    return -1;
  }
  if (!systemCalls) {
    vm->lastError = VM_NO_SYSCALL_CALLBACK;
    Com_Error(vm->lastError, "No systemcalls provided");
    return -1;
  }

  if (!bytecode) {
    vm->lastError = VM_INVALID_POINTER;
    Com_Error(vm->lastError, "bytecode is NULL");
    return -1;
  }

  {
    const vmHeader_t *const header = (const vmHeader_t *const)bytecode;

    Com_Memset(vm, 0, sizeof(vm_t));
    vm->systemCall       = systemCalls;
    vm->instructionCount = UINT(header->instructionCount);

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

    if (VM_ValidateHeader(vm, header, length))
      return -1;

    /* make sure data section is always initialized with 0
     * (bss would be enough) */
    Com_Memset(workingRAM, 0, workingRAMLength);

    /* copy the intialized data, excluding the lit segment */
    Com_Memcpy(workingRAM, &bytecode[UINT(header->codeLength) + UINT(header->litLength) + sizeof(vmHeader_t)],
               UINT(header->dataLength));

    /* the stack is implicitly at the end of the image */
#ifdef DEBUG_VM
    vm->stackBottom = (vm->dataLength + vm->bssLength);

    printf("Memory Layout:\n");
    printf("  CodeBase Store: %p\n", vm->codeBase);
    printf("  LitBase Store:  %p\n", vm->litBase);
    printf(" dataBase Store:  %p\n", vm->dataBase + UINT(header->litLength));
    printf("  bssBase Store:  %p\n\n", vm->bssBase + UINT(header->litLength));

    printf("  RAM Length:     %06X\n", vm->workingRAMLength);
    printf("  Code Length:    %06X\n", vm->codeLength);
    printf("  Lit Length:     %06X\n", vm->litLength);
    printf("  Data Length:    %06X\n", vm->dataLength);
    printf("  BSS Length:     %06X\n", vm->bssLength);
    printf("  BSS End:        %06X\n", vm->dataLength + vm->bssLength);
    printf("  PS Top:         %06X\n", vm->programStack);
    printf("  PS Bottom:      %06X\n", vm->stackBottom);
#endif
  }
  return 0;
}

#ifdef DEBUG_VM
int VM_LoadDebugInfo(vm_t *vm, char *mapfileImage, uint8_t *debugStorage, int debugStorageLength) {
  if (vm == NULL) {
    Com_Error(VM_INVALID_POINTER, "Invalid vm pointer");
    return -1;
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
  Com_Printf("Instruction count: %i\n", vm->instructionCount);
  return 0;
}
#endif

static bool VM_ValidateHeader(const vm_t *const vm, const vmHeader_t *const header, const vm_size_t bytecodeLength) {

  if (!header || bytecodeLength <= vm_sizeof(vmHeader_t) || bytecodeLength > VM_MAX_IMAGE_SIZE) {
    Com_Printf("Failed.\n");
    return -1;
  }

  if (header->vmMagic == VM_MAGIC) {
    if (UINT(header->codeLength) == 0 || UINT(header->instructionCount) == 0 || UINT(header->bssLength) > VM_MAX_BSS_LENGTH ||
        UINT(header->codeLength) + UINT(header->litLength) + UINT(header->dataLength) > bytecodeLength) {
      Com_Printf("Warning: bad header\n");
      Com_Error(VM_MALFORMED_HEADER, "Malformed bytecode image\n");
      return -1;
    }
  } else {
    Com_Printf("Warning: Invalid magic number in header "
               "Read: 0x%x, expected: 0x%x\n",
               header->vmMagic, VM_MAGIC);
    Com_Error(VM_MALFORMED_HEADER, "Invalid magic number in header\n");
    return -1;
  }

  {
    /* round up to next power of 2 so all data operations can
       be mask protected */
    /*TODO: we can remove need for lit to be included in dataSegment*/
    const vm_size_t dataLength = UINT(header->dataLength) + UINT(header->bssLength);

    if (dataLength > vm->workingRAMLength) {
      Com_Printf("Error: Insufficient ram allocated for VM.  Granted %06X, image requires %06X\n", vm->workingRAMLength,
                 dataLength);
      Com_Error(VM_NOT_ENOUGH_RAM, "Insufficient ram allocated for VM\n");
      return -1;
    }
  }

  return 0;
}

/* FIXME: this needs to be locked to uint24_t to ensure platform agnostic */
intptr_t VM_Call(vm_t *vm, ustdint_t command, ...) {
  intptr_t r;

  if (vm == NULL) {
    Com_Error(VM_INVALID_POINTER, "VM_Call with NULL vm");
    return -1;
  }

  if (vm->codeLength < 1) {
    vm->lastError = VM_NOT_LOADED;
    Com_Error(vm->lastError, "VM not loaded");
    return -1;
  }

  /* FIXME this is not nice. we should check the actual number of arguments */
  {
    uint32_t args[MAX_VMMAIN_ARGS];
    va_list  ap;
    uint8_t  i;

    args[0] = command;
    va_start(ap, command);
    for (i = 1; i < (uint8_t)ARRAY_LEN(args); i++) {
      args[i] = va_arg(ap, uint32_t);
    }
    va_end(ap);

    ++vm->callLevel;
    r = VM_CallInterpreted(vm, args);
    --vm->callLevel;
  }

  return r;
}

void VM_Free(vm_t *vm) {
  if (!vm) {
    return;
  }
  if (vm->callLevel) {
    vm->lastError = VM_FREE_ON_RUNNING_VM;
    Com_Error(vm->lastError, "VM_Free on running vm");
    return;
  }

  Com_Memset(vm, 0, sizeof(*vm));
}

void *VM_ArgPtr(intptr_t vmAddr, vm_t *vm) {
  if (!vmAddr) {
    return NULL;
  }

  if (vm == NULL) {
    Com_Error(VM_INVALID_POINTER, "Invalid VM pointer");
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
  printf("VM_FloatToInt: f: %f, i: %08X\n", fi.f, fi.i);
  return fi.i;
}

bool VM_MemoryRangeValid(const vm_size_t vmAddr, const vm_size_t len, const vm_t *const vm) {
  if (!vmAddr || !vm) {
    return -1;
  }

  if (vmAddr > vm->workingRAMLength || vmAddr + len > vm->workingRAMLength) {
    Com_Error(VM_DATA_OUT_OF_RANGE, "Memory access out of range");
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

const uint8_t *VM_RedirectLit(vm_t *vm, int32_t vaddr, int size) {
  if (vaddr < 0) {
    Com_Printf("Attempted to read at %06X\n", vaddr);
    vm->lastError = VM_RAM_ACCESS_ERROR;
    Com_Error(VM_LIT_ACCESS_ERROR, "Memory Access Error");
    return bad_memory;
  }

  if (vaddr < (vm_operand_t)vm->litLength) {
    if (vaddr + size > (vm_operand_t)vm->litLength) {
      vm->lastError = VM_RAM_ACCESS_ERROR;
      Com_Error(VM_LIT_ACCESS_ERROR, "Memory Access Error");
      return bad_memory;
    }
    return &vm->codeBase[vm->codeLength + vaddr];
  }

  if ((vm_size_t)vaddr + size > vm->workingRAMLength + vm->litLength) {
    Com_Printf("Attempted to read at %06X -- (%06X %06X)\n", vaddr, vm->workingRAMLength, vm->litLength);
    vm->lastError = VM_RAM_ACCESS_ERROR;
    Com_Error(VM_RAM_ACCESS_ERROR, "Memory Access Error");
    return bad_memory;
  }

  return &vm->dataBase[vaddr];
}
#else
#define VM_RedirectLit(vm, a, l) ((a < (vm_operand_t)vm->litLength) ? &vm->codeBase[vm->codeLength + a] : &vm->dataBase[a])
#endif

#define opStack32  ((int32_t *)opStack8)
#define opStackFlt ((float *)opStack8)

#define VM_DataRead_uint32(vm, vaddr) (*((uint32_t *)VM_RedirectLit(vm, UINT(vaddr), 4)))

#define VM_DataRead_uint24(vm, vaddr)                                                                                              \
  (*((uint24_t *)((UINT(vaddr) < vm->litLength) ? &vm->codeBase[vm->codeLength + UINT(vaddr)] : &vm->dataBase[UINT(vaddr)])))

#define VM_DataRead_uint16(vm, vaddr)                                                                                              \
  (*((uint16_t *)((UINT(vaddr) < vm->litLength) ? &vm->codeBase[vm->codeLength + UINT(vaddr)] : &vm->dataBase[UINT(vaddr)])))

#define VM_DataRead_float(vm, vaddr) (*(float *)VM_RedirectLit(vm, vaddr, 4))
#define VM_DataRead_uint8(vm, vaddr) (*(uint8_t *)VM_RedirectLit(vm, vaddr, 1))

#define pop_2_int32()                                                                                                              \
  r0 = *((int32_t *)opStack8);                                                                                                     \
  r1 = *((int32_t *)(opStack8 - 4));                                                                                               \
  log3_3("%08X %08X POP int32\n", r0, r1);                                                                                         \
  opStack8 -= 8;

#define pop_2_uint32()                                                                                                             \
  r0 = *((uint32_t *)opStack8);                                                                                                    \
  r1 = *((uint32_t *)(opStack8 - 4));                                                                                              \
  log3_3("%08X %08X POP uint32\n", r0, r1);                                                                                        \
  opStack8 -= 8;

#define pop_1_uint32()                                                                                                             \
  r0 = *((uint32_t *)opStack8);                                                                                                    \
  log3_2("%08X POP uint32\n", r0);                                                                                                 \
  opStack8 -= 4;

#define pop_1_int32()                                                                                                              \
  r0 = *((int32_t *)opStack8);                                                                                                     \
  log3_2("%08X POP int32\n", r0);                                                                                                  \
  opStack8 -= 4;

#define pop_2_uint24()                                                                                                             \
  r0_uint24 = *((uint24_t *)opStack8);                                                                                             \
  r1_uint24 = *((uint24_t *)(opStack8 - 4));                                                                                       \
  log3_3("%08X %08X POP uint24\n", UINT(r0_uint24), UINT(r1_uint24));                                                              \
  opStack8 -= 8;

#define pop_2_int24()                                                                                                              \
  r0_int24 = *((int24_t *)opStack8);                                                                                               \
  r1_int24 = *((int24_t *)(opStack8 - 4));                                                                                         \
  log3_3("%06X %06X POP int24\n", INT(r0_int24), INT(r1_int24));                                                                   \
  opStack8 -= 8;

#define pop_1_uint24()                                                                                                             \
  r0_uint24 = *((uint24_t *)opStack8);                                                                                             \
  log3_2("%06X POP uint24\n", UINT(r0_uint24));                                                                                    \
  opStack8 -= 4;

#define pop_1_int24()                                                                                                              \
  r0_int24 = *((int24_t *)opStack8);                                                                                               \
  log3_2("%06X POP int24\n", INT(r0_int24));                                                                                       \
  opStack8 -= 4;

#define pop_1_uint16()                                                                                                             \
  r0_uint16 = *((uint16_t *)opStack8);                                                                                             \
  log3_2("%04X POP uint16\n", r0_uint16);                                                                                          \
  opStack8 -= 4;

#define pop_1_int16()                                                                                                              \
  r0_int16 = *((int16_t *)opStack8);                                                                                               \
  log3_2("%04X POP int16\n", r0_int16);                                                                                            \
  opStack8 -= 4;

#define pop_1_int8()                                                                                                               \
  r0_int8 = *((int8_t *)opStack8);                                                                                                 \
  log3_2("%02X POP int8\n", r0_int8);                                                                                              \
  opStack8 -= 4;

#define pop_1_uint8()                                                                                                              \
  r0_uint8 = *((uint8_t *)opStack8);                                                                                               \
  log3_2("%02X POP uint8\n", r0_uint8);                                                                                            \
  opStack8 -= 4;

#define push_1_float(a)                                                                                                            \
  opStack8 += 4;                                                                                                                   \
  *opStack32 = a;

#define push_1_int32(a)                                                                                                            \
  opStack8 += 4;                                                                                                                   \
  *opStack32 = a;                                                                                                                  \
  log3_2("%08X PUSHED int32\n", *opStack32);

#define push_1_uint32(a)                                                                                                           \
  opStack8 += 4;                                                                                                                   \
  *opStack32 = a;                                                                                                                  \
  log3_2("%08X PUSHED uint32\n", *opStack32);

#define push_1_uint24(a)                                                                                                           \
  opStack8 += 4;                                                                                                                   \
  *opStack32 = (uint32_t)UINT(((uint24_t)(a)));                                                                                    \
  log3_2("%08X PUSHED uint24\n", *opStack32);

#define push_1_int24(a)                                                                                                            \
  opStack8 += 4;                                                                                                                   \
  *opStack32 = (int32_t)INT(((int24_t)(a)));                                                                                       \
  log3_2("%08X PUSHED int24\n", *opStack32);

#define push_1_uint16(a)                                                                                                           \
  opStack8 += 4;                                                                                                                   \
  *opStack32 = (uint32_t)(uint16_t)a;                                                                                              \
  log3_2("%08X PUSHED uint16\n", *opStack32);

#define push_1_int16(a)                                                                                                            \
  opStack8 += 4;                                                                                                                   \
  *opStack32 = (int32_t)(int16_t)a;                                                                                                \
  log3_2("%08X PUSHED int16\n", *opStack32);

#define push_1_uint8(a)                                                                                                            \
  opStack8 += 4;                                                                                                                   \
  *opStack32 = (uint8_t)(a);                                                                                                       \
  log3_2("%02X PUSHED uint8\n", *opStack32);

#define push_1_int8(a)                                                                                                             \
  opStack8 += 4;                                                                                                                   \
  *opStack32 = (int8_t)(a);                                                                                                        \
  log3_2("%02X PUSHED int8\n", *opStack32);

/* FIXME: this needs to be locked to uint24_t to ensure platform agnostic */
static ustdint_t VM_CallInterpreted(vm_t *vm, uint32_t *args) {
  uint8_t        _opStack[OPSTACK_SIZE + 4]; /* 256 4 byte double words + 4 safety bytes */
  uint8_t       *opStack8 = &_opStack[4];
  stdint_t       programCounter;
  ustdint_t      programStack;
  ustdint_t      stackOnEntry;
  uint8_t       *dataBase;
  const uint8_t *codeBase;
  ustdint_t      arg;
  uint8_t        opcode;
  vm_operand_t   r0, r1;

  int24_t r0_int24;
  int24_t r1_int24;

  uint24_t r0_uint24;
  uint24_t r1_uint24;
  uint16_t r0_uint16;
  int16_t  r0_int16;
  int8_t   r0_int8;
  uint8_t  r0_uint8;

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

  dataBase = vm->dataBase;
  codeBase = vm->codeBase;

  programCounter = 0;
  programStack -= (6 + 3 * MAX_VMMAIN_ARGS);

  for (arg = 0; arg < MAX_VMMAIN_ARGS; arg++) {
    *(int24_t *)&dataBase[programStack + 6 + arg * 3] = INT24(args[arg]);
  }

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
    r0 = opStack32[0];
    r1 = opStack32[-1];

#ifdef DEBUG_VM
    if (vm_debugLevel > 1) {
      Com_Printf("%06X:\t", programCounter);
    }
#endif

    opcode = codeBase[programCounter++];

#ifdef DEBUG_VM
    if (programCounter >= (stdint_t)vm->codeLength) {
      vm->lastError = VM_PC_OUT_OF_RANGE;
      Com_Error(vm->lastError, "VM pc out of range");
      return -1;
    }

    if (programStack <= vm->stackBottom) {
      vm->lastError = VM_STACK_OVERFLOW;
      Com_Error(vm->lastError, "VM stack overflow");
      return -1;
    }

    if (vm_debugLevel > 1) {
      Com_Printf("%s%i %s\t(%02X %06X);\tSP=%06X,  R0=%06X, R1=%06X \n", VM_Indent(vm), (int)(opStack8 - _opStack), opnames[opcode],
                 opcode, r2, programStack, r0, r1);
    }
    profileSymbol->profileCount++;
#endif /* DEBUG_VM */
    switch (opcode) {
#ifdef DEBUG_VM
    default: /* fall through */
#endif
    case OP_UNDEF:
      Com_Error(vm->lastError = VM_BAD_INSTRUCTION, "Bad VM instruction");
      return -1;
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
      r1 = r0;
      r0 = *opStack32 = r2_int24_old;
      programCounter += INT24_INCREMENT;
      DISPATCH();

    case OP_LOCAL: {
      log3_2("&PS[%04X]", r2_uint16);
      push_1_uint24(UINT24(r2_uint16 + programStack));
      programCounter += INT16_INCREMENT;
      DISPATCH();
    }

    case OP_LOADF4: {
      r0 = *opStack32 = *((uint32_t *)VM_RedirectLit(vm, r0, 4));
      DISPATCH();
    }

    case OP_LOAD4: {
      pop_1_uint24();
      log3_2("R0: %06X", UINT(r0_uint24));
      push_1_uint32(VM_DataRead_uint32(vm, r0_uint24));
      DISPATCH();
    }

    case OP_LOAD3: {
      pop_1_uint24();
      log3_2("RO: %06X", UINT(r0_uint24));
      push_1_uint24(VM_DataRead_uint24(vm, r0_uint24));
      DISPATCH();
    }

    case OP_LOAD2:
      pop_1_uint24();
      log3_2("R0: %06X", UINT(r0_uint24));
      push_1_uint16(VM_DataRead_uint16(vm, r0_uint24));
      DISPATCH();

    case OP_LOAD1: {
      pop_1_uint24();
      log3_2("R0: %06X", UINT(r0_uint24));
      push_1_uint8(*VM_RedirectLit(vm, (vm_operand_t)UINT(r0_uint24), 1));
      DISPATCH();
    }

    case OP_STORE3:
      pop_2_uint24();
      *(uint24_t *)&dataBase[UINT(r1_uint24)] = r0_uint24;
      log3_3("*(%06X) = %08X  MOVE (3 bytes)\n", UINT(r1_uint24), UINT(r0_uint24));
      DISPATCH();

    case OP_STORE4: {
      pop_1_uint32();
      pop_1_uint24();
      *((uint32_t *)&dataBase[UINT(r0_uint24)]) = r0;
      log3_3("*(%06X) = %08X  MOVE\n", UINT(r0_uint24), r0);
      DISPATCH();
    }

      *(vm_operand_t *)&dataBase[r1] = r0;
      opStack8 -= 8;
      DISPATCH();

    case OP_STOREF4:
      *(vm_operand_t *)&dataBase[r1] = r0;
      opStack8 -= 8;
      DISPATCH();

    case OP_STORE2:
      pop_1_uint16();
      pop_1_uint24();
      *((uint16_t *)&dataBase[UINT(r0_uint24)]) = r0_uint16;
      log3_3("*(%06X) = %04X  MOVE\n", UINT(r0_uint24), r0_uint16);
      DISPATCH();

    case OP_STORE1:
      pop_1_uint8();
      pop_1_uint24();
      dataBase[UINT(r0_uint24)] = r0_uint8;
      log3_3("*(%06X) = %02X  MOVE\n", UINT(r0_uint24), r0_uint8);
      DISPATCH();

    case OP_ARG:
      /* single byte offset from programStack */
#ifdef DEBUG_VM
      printf("  ARG *[%06X + %02X (%06X)] = %06X\n", programStack, r2_int8, programStack + r2_int8, r0);
#endif
      *(int24_t *)&dataBase[programStack + r2_int8] = INT24(r0);
      opStack8 -= 4;
      programCounter += 1;
      DISPATCH();

    case OP_ARG4:
      /* single byte offset from programStack */
#ifdef DEBUG_VM
      printf("  ARG4 *[%06X + %02X (%06X)] = %06X\n", programStack, r2_int8, programStack + r2_int8, r0);
#endif
      *(int32_t *)&dataBase[programStack + r2_int8] = r0;
      opStack8 -= 4;
      programCounter += 1;
      DISPATCH();

    case OP_ARGF: {
      /* single byte offset from programStack */
#ifdef DEBUG_VM
      float    f = *((float *)&r0);
      uint32_t i = *((uint32_t *)&r0);

      printf("  ARGF *[%06X + %02X (%06X)] = %f (%08X) (from: %06X)\n", programStack, r2_int8, programStack + r2_int8, f, i,
             (int)(opStack8 - _opStack));
#endif

      *((uint32_t *)&dataBase[programStack + r2_int8]) = r0;
      opStack8 -= 4;
      programCounter += 1;
      DISPATCH();
    }

    case OP_BLOCK_COPY:
      VM_BlockCopy(r1, r0, r2_uint24_old, vm);
      programCounter += INT24_INCREMENT;
      opStack8 -= 8;
      DISPATCH();

    case OP_CALL:
      /* save current program counter */
      *(int24_t *)&dataBase[programStack] = INT24(programCounter);
      pop_1_int24();
      /* jump to the location on the stack */
      programCounter = INT(r0_int24);

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
      } else if ((unsigned)programCounter >= MAX_PROGRAM_COUNTER) {
        vm->lastError = VM_PC_OUT_OF_RANGE;
        Com_Error(vm->lastError, "VM program counter out of range in OP_CALL");
        return -1;
      }
      DISPATCH();
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
      log3_4("FRAME SIZE: %04X (from %06X to %06X)\n", localsAndArgsSize, programStack + localsAndArgsSize, programStack);

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
      log3_4("FRAME SIZE: %06X (from %06X to %06X)\n", r2_int16, programStack, programStack + r2_int16);
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
      if (programCounter == -1) {
        goto done;
      } else if ((unsigned)programCounter >= (unsigned)vm->codeLength) {
        Com_Error(vm->lastError = VM_PC_OUT_OF_RANGE, "VM program counter out of range in OP_LEAVE");
        return -1;
      }
      DISPATCH();
    }
      /*
         ===================================================================
         BRANCHES
         ===================================================================
         */

    case OP_JUMP:
      if ((unsigned)r0 >= MAX_PROGRAM_COUNTER) {
        Com_Error(vm->lastError = VM_PC_OUT_OF_RANGE, "VM program counter out of range in OP_JUMP");
        return -1;
      }

      programCounter = r0;
      opStack8 -= 4;
      DISPATCH();

    case OP_EQ: {
      pop_2_int32();
      log3_3("%08X == %08X\n", r1, r0);
      programCounter = (r1 == r0) ? UINT(r2_uint24) : programCounter + INT24_INCREMENT;
      DISPATCH();
    }

    case OP_EQ3: {
      pop_2_uint24();
      if (UINT(r1_uint24) == UINT(r0_uint24)) {
        programCounter = r2_int24_old;
      } else {
        programCounter += INT24_INCREMENT;
      }
      DISPATCH();
    }

    case OP_NE: {
      pop_2_int32();
      log3_3("%08X != %08X\n", r1, r0);
      programCounter = (r1 != r0) ? UINT(r2_uint24) : programCounter + INT24_INCREMENT;
      DISPATCH();
    }

    case OP_NE3: {
      pop_2_uint24();
      log3_3("%06X != %06X\n", UINT(r1_uint24), UINT(r0_uint24));
      programCounter = (UINT(r1_uint24) != UINT(r0_uint24)) ? UINT(r2_uint24) : programCounter + INT24_INCREMENT;
      DISPATCH();
    }

    case OP_LTI: {
      pop_2_int32();
      log3_3("%08X < %08X\n", r1, r0);
      programCounter = (r1 < r0) ? UINT(r2_uint24) : programCounter + INT24_INCREMENT;
      DISPATCH();
    }

    case OP_LTI3: {
      pop_2_int24();
      log3_4("%06X < %06X = %d\n", INT(r1_int24), INT(r0_int24), (INT(r1_int24) < INT(r0_int24)));
      programCounter = (INT(r1_int24) < INT(r0_int24)) ? UINT(r2_uint24) : programCounter + INT24_INCREMENT;
      DISPATCH();
    }

    case OP_LEI: {
      pop_2_int32();
      log3_3("%08X <= %08X\n", r1, r0);
      programCounter = (r1 <= r0) ? UINT(r2_uint24) : programCounter + INT24_INCREMENT;
      DISPATCH();
    }

    case OP_LEI3: {
      pop_2_int24();
      log3_3("%06X <= %06X\n", INT(r1_int24), INT(r0_int24));
      programCounter = (INT(r1_int24) <= INT(r0_int24)) ? UINT(r2_uint24) : programCounter + INT24_INCREMENT;
      DISPATCH();
    }

    case OP_GTI: {
      pop_2_int32();
      log3_3("%08X > %08X\n", r1, r0);
      programCounter = (r1 > r0) ? UINT(r2_uint24) : programCounter + INT24_INCREMENT;
      DISPATCH();
    }

    case OP_GEI:
      opStack8 -= 8;
      log3_3("%08X > %08X\n", r1, r0);

      if (r1 >= r0) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }

    case OP_GEI3: {
      pop_2_int24();
      log3_3("%06X >= %06X\n", INT(r1_int24), INT(r0_int24));
      programCounter = (INT(r1_int24) >= INT(r0_int24)) ? UINT(r2_uint24) : programCounter + INT24_INCREMENT;
      DISPATCH();
    }

    case OP_GTI3: {
      pop_2_int24();
      log3_3("%06X > %06X\n", INT(r1_int24), INT(r0_int24));
      programCounter = (INT(r1_int24) > INT(r0_int24)) ? UINT(r2_uint24) : programCounter + INT24_INCREMENT;
      DISPATCH();
    }

    case OP_LTU:
      opStack8 -= 8;
      if (((unsigned)r1) < ((unsigned)r0)) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }
    case OP_LEU:
      opStack8 -= 8;
      if (((unsigned)r1) <= ((unsigned)r0)) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }
    case OP_GTU:
      opStack8 -= 8;
      if (((unsigned)r1) > ((unsigned)r0)) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }
    case OP_GEU:
      opStack8 -= 8;
      if (((unsigned)r1) >= ((unsigned)r0)) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }
    case OP_EQF:
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
    case OP_GTF:
      opStack8 -= 8;

      if (opStackFlt[1] > opStackFlt[2]) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }
    case OP_GEF:
      opStack8 -= 8;

      if (opStackFlt[1] >= opStackFlt[2]) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }

      /*===================================================================*/

    case OP_NEGI:
      pop_1_int32();
      push_1_int32(-r0);
      DISPATCH();

    case OP_NEGI3:
      pop_1_int24();
      push_1_int24(INT24(-INT(r0_int24)));
      DISPATCH();

    case OP_ADD3:
      pop_2_int24();
      log3_3("%08X + %08X = ", INT(r1_int24), INT(r0_int24));
      push_1_int24(INT24(INT(r1_int24) + INT(r0_int24)));
      DISPATCH();

    case OP_ADD: {
      pop_2_int32();
      log3_3("%08X + %08X =", r1, r0);
      push_1_int32(r1 + r0);
      DISPATCH();
    }

    case OP_SUB:
      log3_4("%08X - %08X = %08X PUSHED\n", r1, r0, r1 - r0);
      opStack8 -= 4;
      *opStack32 = r1 - r0;
      DISPATCH();

    case OP_SUB3: {
      pop_2_int24();
      log3_3("%08X - %08X = ", INT(r1_int24), INT(r0_int24));
      push_1_int24(INT24(INT(r1_int24) - INT(r0_int24)));
      DISPATCH();
    }

    case OP_DIVI: {
      pop_2_int32();
      log3_3("%08X / %08X =", r1, r0);
      push_1_int32(r1 / r0);
      DISPATCH();
    }

    case OP_DIVI3: {
      pop_2_int24();
      log3_3("%08X / %08X =", INT(r1_int24), INT(r0_int24));
      push_1_int24(INT24(INT(r1_int24) / INT(r0_int24)));
      DISPATCH();
    }

    case OP_DIVU:
      opStack8 -= 4;
      *opStack32 = ((unsigned)r1) / ((unsigned)r0);
      DISPATCH();

    case OP_MODI:
      opStack8 -= 4;
      *opStack32 = r1 % r0;
      DISPATCH();

    case OP_MODI3: {
      pop_2_int24();
      log3_3("%08X %% %08X = ", INT(r1_int24), INT(r0_int24));
      push_1_int24(INT24(INT(r1_int24) % INT(r0_int24)));
      DISPATCH();
    }

    case OP_MODU:
      opStack8 -= 4;
      *opStack32 = ((unsigned)r1) % ((unsigned)r0);
      DISPATCH();

    case OP_MULI: {
      pop_2_int32();
      log3_3("%08X * %08X =", r1, r0);
      push_1_int32(r1 * r0);
      DISPATCH();
    }

    case OP_MULI3: {
      pop_2_int24();
      log3_3("%08X * %08X = ", INT(r1_int24), INT(r0_int24));
      push_1_int24(INT24(INT(r1_int24) * INT(r0_int24)));
      DISPATCH();
    }

    case OP_MULU:
      opStack8 -= 4;
      *opStack32 = ((unsigned)r1) * ((unsigned)r0);
      DISPATCH();

    case OP_BAND: {
      pop_2_uint32();
      log3_3("%08X & %08X =", r1, r0);
      push_1_uint32(r1 & r0);
      DISPATCH();
    }

    case OP_BAND3: {
      pop_2_uint24();
      log3_3("%08X & %08X = ", UINT(r1_uint24), UINT(r0_uint24));
      push_1_uint24(UINT24(UINT(r1_uint24) & UINT(r0_uint24)));
      DISPATCH();
    }

    case OP_BOR: {
      pop_2_uint32();
      log3_3("%08X | %08X =", r1, r0);
      push_1_uint32(r1 | r0);
      DISPATCH();
    }

    case OP_BOR3: {
      pop_2_uint24();
      log3_3("%08X | %08X = ", UINT(r1_uint24), UINT(r0_uint24));
      push_1_uint24(UINT24(UINT(r1_uint24) | UINT(r0_uint24)));
      DISPATCH();
    }

    case OP_BXOR: {
      pop_2_uint32();
      log3_3("%08X ^ %08X =", r1, r0);
      push_1_uint32(r1 ^ r0);
      DISPATCH();
    }

    case OP_BXOR3: {
      pop_2_uint24();
      log3_3("%08X ^ %08X = ", UINT(r1_uint24), UINT(r0_uint24));
      push_1_uint24(UINT24(UINT(r1_uint24) ^ UINT(r0_uint24)));
      DISPATCH();
    }
    case OP_BCOM:
      *opStack32 = ~((unsigned)r0);
      DISPATCH();

    case OP_LSH: {
      pop_2_uint32();
      log3_3("%08X << %08X =", r1, r0);
      push_1_uint32(r1 << r0);
      DISPATCH();
    }

    case OP_LSH3: {
      pop_2_uint24();
      log3_3("%08X << %08X = ", UINT(r1_uint24), UINT(r0_uint24));
      push_1_uint24(UINT24(UINT(r1_uint24) << UINT(r0_uint24)));
      DISPATCH();
    }

    case OP_RSHI: {
      pop_2_int32();
      log3_3("%08X >> %08X =", r1, r0);
      push_1_int32(r1 >> r0);
      DISPATCH();
    }

    case OP_RSHI3: {
      pop_2_int24();
      log3_3("%08X >> %08X = ", INT(r1_int24), INT(r0_int24));
      push_1_int24(INT24(INT(r1_int24) >> INT(r0_int24)));
      DISPATCH();
    }

    case OP_RSHU:
      opStack8 -= 4;
      *opStack32 = ((unsigned)r1) >> r0;
      DISPATCH();
    case OP_NEGF:
      opStackFlt[0] = -opStackFlt[0];
      DISPATCH();
    case OP_ADDF:
      opStack8 -= 4;
      opStackFlt[0] = opStackFlt[0] + opStackFlt[1];
      DISPATCH();
    case OP_SUBF:
      opStack8 -= 4;
      opStackFlt[0] = opStackFlt[0] - opStackFlt[1];
      DISPATCH();
    case OP_DIVF:
      opStack8 -= 4;
      opStackFlt[0] = opStackFlt[0] / opStackFlt[1];
      DISPATCH();
    case OP_MULF:
      opStack8 -= 4;
      opStackFlt[0] = opStackFlt[0] * opStackFlt[1];
      DISPATCH();

    case OP_CVIF:
      opStackFlt[0] = (float)opStack32[0];
      DISPATCH();

    case OP_CVFI:
      *opStack32 = Q_ftol(opStackFlt[0]);
      DISPATCH();

    case OP_CI1I4:
      *opStack32 = (int8_t)*opStack32;
      DISPATCH();

    case OP_CI2I4:
      *opStack32 = (int16_t)*opStack32;
      DISPATCH();

    case OP_CI3I4: {
      pop_1_int24();
      push_1_int32((int32_t)(INT(r0_int24)));
      DISPATCH();
    }

    /* convert I3 to I1*/
    case OP_CI3I1: {
      pop_1_int24();
      push_1_int8((int8_t)(INT(r0_int24)));
      DISPATCH();
    }

      /* convert I3 to I2*/
    case OP_CI3I2: {
      pop_1_int24();
      push_1_int16((int16_t)(INT(r0_int24)));
      DISPATCH();
    }

    /* extend sign I2 to I3*/
    case OP_CI2I3: {
      pop_1_int16();
      push_1_int24(INT24(r0_int16));
      DISPATCH();
    }

    /* reduce I4 to I3 */
    case OP_CI4I3: {
      pop_1_int32();
      push_1_int24(INT24(r0));
      DISPATCH();
    }

      /* extend sign I1 to I3*/
    case OP_CI1I3: {
      pop_1_int8();
      push_1_int24(INT24(r0_int8));
      DISPATCH();
    }

    /* reduce store from U3 to U2 */
    /*TODO: optimse by just remove the high byte stored on stack*/
    case OP_CU3U2: {
      pop_1_uint24();
      push_1_uint16((uint16_t)UINT(r0_uint24));
      DISPATCH();
    }

    case OP_CU2I3: {
      pop_1_uint16();
      push_1_int24(INT24(r0_uint16));
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
      push_1_uint32(r2);
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
      r1 = r0;
      r0 = *opStack32 = r2;
      programCounter += INT32_INCREMENT;
      DISPATCH();
    }

    case OP_CONSTP3: {
      opStack8 += 4;
      r1 = r0;
      r0 = *opStack32 = (vm_operand_t)(int32_t)r2_uint24_old;
      programCounter += INT24_INCREMENT;
      DISPATCH();
    }

    case OP_CONSTP4: {
      opStack8 += 4;
      r1 = r0;
      r0 = *opStack32 = (vm_operand_t)(int32_t)r2_uint24_old;
      programCounter += INT24_INCREMENT;
      DISPATCH();
    }
    }
  }

done:
  if (opStack32[-1] != 0x0000BEEF) {
    Com_Error(vm->lastError = VM_STACK_ERROR, "Operation stack error");
  }

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
      Com_Error(VM_MALLOC_FAILED, "Sym. pointer malloc failed: out of memory?");
      break;
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
    Com_Printf("STACK: %s %06X %06X\n", VM_ValueToSymbol(vm, programCounter), programCounter, programStack);
    programStack   = INT(*(int24_t *)&vm->dataBase[programStack + 3]);
    programCounter = INT(*(int24_t *)&vm->dataBase[programStack]);
  } while (programCounter != -1 && ++count < 5);

  if (programCounter == 0) {
    Com_Printf("STACK: %s %06X %06X\n", VM_ValueToSymbol(vm, programCounter), programCounter, programStack);
    printf("%06X\n", INT(*(int24_t *)&vm->dataBase[programStack - 6]));
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
