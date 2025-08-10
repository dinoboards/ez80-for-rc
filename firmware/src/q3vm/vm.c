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

#include "vm.h"

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
 * command number + 12 arguments */
#define MAX_VMMAIN_ARGS 13

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

/** Max. number of op codes in op codes table */
#define OPCODE_TABLE_SIZE 64
/** Mask for a valid opcode (so no one can escape the sandbox) */
#define OPCODE_TABLE_MASK (OPCODE_TABLE_SIZE - 1)

/** Max. size of BSS section */
#define VM_MAX_BSS_LENGTH 10485760

/******************************************************************************
 * TYPEDEFS
 ******************************************************************************/

/** Enum for the virtual machine op codes */
typedef enum {
  OP_UNDEF, /* Error: VM halt */

  OP_IGNORE, /* No operation */

  OP_BREAK, /* vm->breakCount++ */

  OP_ENTER, /* Begin subroutine. */
  OP_LEAVE, /* End subroutine. */
  OP_CALL,  /* Call subroutine. */
  OP_PUSH,  /* Push to stack. */
  OP_POP,   /* Discard top-of-stack. */

  OP_CONST, /* Load constant to stack. */
  OP_LOCAL, /* Get local variable. */

  OP_JUMP, /* Unconditional jump. */

  /*-------------------*/

  OP_EQ, /* Compare integers, jump if equal. */
  OP_NE, /* Compare integers, jump if not equal. */

  OP_LTI, /* Compare integers, jump if less-than. */
  OP_LEI, /* Compare integers, jump if less-than-or-equal. */
  OP_GTI, /* Compare integers, jump if greater-than. */
  OP_GEI, /* Compare integers, jump if greater-than-or-equal. */

  OP_LTU, /* Compare unsigned integers, jump if less-than */
  OP_LEU, /* Compare unsigned integers, jump if less-than-or-equal */
  OP_GTU, /* Compare unsigned integers, jump if greater-than */
  OP_GEU, /* Compare unsigned integers, jump if greater-than-or-equal */

  OP_EQF, /* Compare floats, jump if equal */
  OP_NEF, /* Compare floats, jump if not-equal */

  OP_LTF, /* Compare floats, jump if less-than */
  OP_LEF, /* Compare floats, jump if less-than-or-equal */
  OP_GTF, /* Compare floats, jump if greater-than */
  OP_GEF, /* Compare floats, jump if greater-than-or-equal */

  /*-------------------*/

  OP_LOAD1,  /* Load 1-byte from memory */
  OP_LOAD2,  /* Load 2-bytes from memory */
  OP_LOAD4,  /* Load 4-bytes from memory */
  OP_STORE1, /* Store 1-byte to memory */
  OP_STORE2, /* Store 2-byte to memory */
  OP_STORE4, /* *(stack[top-1]) = stack[top] */
  OP_ARG,    /* Marshal argument */

  OP_BLOCK_COPY, /* memcpy */

  /*-------------------*/

  OP_SEX8,  /* Sign-Extend 8-bit */
  OP_SEX16, /* Sign-Extend 16-bit */

  OP_NEGI, /* Negate integer. */
  OP_ADD,  /* Add integers (two's complement). */
  OP_SUB,  /* Subtract integers (two's complement). */
  OP_DIVI, /* Divide signed integers. */
  OP_DIVU, /* Divide unsigned integers. */
  OP_MODI, /* Modulus (signed). */
  OP_MODU, /* Modulus (unsigned). */
  OP_MULI, /* Multiply signed integers. */
  OP_MULU, /* Multiply unsigned integers. */

  OP_BAND, /* Bitwise AND */
  OP_BOR,  /* Bitwise OR */
  OP_BXOR, /* Bitwise eXclusive-OR */
  OP_BCOM, /* Bitwise COMplement */

  OP_LSH,  /* Left-shift */
  OP_RSHI, /* Right-shift (algebraic; preserve sign) */
  OP_RSHU, /* Right-shift (bitwise; ignore sign) */

  OP_NEGF, /* Negate float */
  OP_ADDF, /* Add floats */
  OP_SUBF, /* Subtract floats */
  OP_DIVF, /* Divide floats */
  OP_MULF, /* Multiply floats */

  OP_CVIF, /* Convert to integer from float */
  OP_CVFI, /* Convert to float from integer */

  OP_MAX /* Make this the last item */
} opcode_t;

/* for the the computed gotos we need labels,
 * but for the normal switch case we need the cases */
#define goto_OP_UNDEF      case OP_UNDEF
#define goto_OP_IGNORE     case OP_IGNORE
#define goto_OP_BREAK      case OP_BREAK
#define goto_OP_ENTER      case OP_ENTER
#define goto_OP_LEAVE      case OP_LEAVE
#define goto_OP_CALL       case OP_CALL
#define goto_OP_PUSH       case OP_PUSH
#define goto_OP_POP        case OP_POP
#define goto_OP_CONST      case OP_CONST
#define goto_OP_LOCAL      case OP_LOCAL
#define goto_OP_JUMP       case OP_JUMP
#define goto_OP_EQ         case OP_EQ
#define goto_OP_NE         case OP_NE
#define goto_OP_LTI        case OP_LTI
#define goto_OP_LEI        case OP_LEI
#define goto_OP_GTI        case OP_GTI
#define goto_OP_GEI        case OP_GEI
#define goto_OP_LTU        case OP_LTU
#define goto_OP_LEU        case OP_LEU
#define goto_OP_GTU        case OP_GTU
#define goto_OP_GEU        case OP_GEU
#define goto_OP_EQF        case OP_EQF
#define goto_OP_NEF        case OP_NEF
#define goto_OP_LTF        case OP_LTF
#define goto_OP_LEF        case OP_LEF
#define goto_OP_GTF        case OP_GTF
#define goto_OP_GEF        case OP_GEF
#define goto_OP_LOAD1      case OP_LOAD1
#define goto_OP_LOAD2      case OP_LOAD2
#define goto_OP_LOAD4      case OP_LOAD4
#define goto_OP_STORE1     case OP_STORE1
#define goto_OP_STORE2     case OP_STORE2
#define goto_OP_STORE4     case OP_STORE4
#define goto_OP_ARG        case OP_ARG
#define goto_OP_BLOCK_COPY case OP_BLOCK_COPY
#define goto_OP_SEX8       case OP_SEX8
#define goto_OP_SEX16      case OP_SEX16
#define goto_OP_NEGI       case OP_NEGI
#define goto_OP_ADD        case OP_ADD
#define goto_OP_SUB        case OP_SUB
#define goto_OP_DIVI       case OP_DIVI
#define goto_OP_DIVU       case OP_DIVU
#define goto_OP_MODI       case OP_MODI
#define goto_OP_MODU       case OP_MODU
#define goto_OP_MULI       case OP_MULI
#define goto_OP_MULU       case OP_MULU
#define goto_OP_BAND       case OP_BAND
#define goto_OP_BOR        case OP_BOR
#define goto_OP_BXOR       case OP_BXOR
#define goto_OP_BCOM       case OP_BCOM
#define goto_OP_LSH        case OP_LSH
#define goto_OP_RSHI       case OP_RSHI
#define goto_OP_RSHU       case OP_RSHU
#define goto_OP_NEGF       case OP_NEGF
#define goto_OP_ADDF       case OP_ADDF
#define goto_OP_SUBF       case OP_SUBF
#define goto_OP_DIVF       case OP_DIVF
#define goto_OP_MULF       case OP_MULF
#define goto_OP_CVIF       case OP_CVIF
#define goto_OP_CVFI       case OP_CVFI

/******************************************************************************
 * LOCAL DATA DEFINITIONS
 ******************************************************************************/

#ifdef DEBUG_VM
static uint8_t vm_debugLevel; /**< 0: be quiet, 1: debug msgs, 2: print op codes */

/** Table to convert op codes to readable names */
static const char *opnames[OPCODE_TABLE_SIZE] = {
    "OP_UNDEF",  "OP_IGNORE", "OP_BREAK",  "OP_ENTER", "OP_LEAVE",      "OP_CALL", "OP_PUSH",  "OP_POP",   "OP_CONST", "OP_LOCAL",
    "OP_JUMP",   "OP_EQ",     "OP_NE",     "OP_LTI",   "OP_LEI",        "OP_GTI",  "OP_GEI",   "OP_LTU",   "OP_LEU",   "OP_GTU",
    "OP_GEU",    "OP_EQF",    "OP_NEF",    "OP_LTF",   "OP_LEF",        "OP_GTF",  "OP_GEF",   "OP_LOAD1", "OP_LOAD2", "OP_LOAD4",
    "OP_STORE1", "OP_STORE2", "OP_STORE4", "OP_ARG",   "OP_BLOCK_COPY", "OP_SEX8", "OP_SEX16", "OP_NEGI",  "OP_ADD",   "OP_SUB",
    "OP_DIVI",   "OP_DIVU",   "OP_MODI",   "OP_MODU",  "OP_MULI",       "OP_MULU", "OP_BAND",  "OP_BOR",   "OP_BXOR",  "OP_BCOM",
    "OP_LSH",    "OP_RSHI",   "OP_RSHU",   "OP_NEGF",  "OP_ADDF",       "OP_SUBF", "OP_DIVF",  "OP_MULF",  "OP_CVIF",  "OP_CVFI",
    "OP_UNDEF",  "OP_UNDEF",  "OP_UNDEF",  "OP_UNDEF",
};
#endif

/******************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 ******************************************************************************/

/** Helper function for VM_Create: Set up the virtual machine during loading.
 * Copy the data from the file input (bytecode) to the vm.
 * @param[in,out] vm Pointer to virtual machine, prepared by VM_Create.
 * @param[in] bytecode Pointer to bytecode.
 * @param[in] length Number of bytes in bytecode array.
 * @return Pointer to start/header of vm bytecode. */
static const vmHeader_t *VM_LoadQVM(
    vm_t *const vm, const uint8_t *bytecode, vm_size_t length, uint8_t *const dataSegment, const vm_size_t dataSegmentLength);

/** Helper function for VM_Create: Set up the virtual machine during loading.
 * Ensure consistency and prepare the jumps.
 * @param[in,out] vm Pointer to virtual machine, prepared by VM_Create.
 * @param[in] header Header of .qvm bytecode.
 * @return 0 if everything is OK. -1 otherwise. */
static bool VM_PrepareInterpreter(vm_t *vm, const vmHeader_t *header);

/** Run a function from the virtual machine with the interpreter (i.e. no JIT).
 * @param[in] vm Pointer to initialized virtual machine.
 * @param[in] args Arguments for function call.
 * @return Return value of the function call. */
static std_int VM_CallInterpreted(vm_t *vm, std_int *args);

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

/** @brief Safe strncpy that ensures a trailing zero.
 * @param[out] dest Output string.
 * @param[in] src Input string.
 * @param[in] destsize Number of free bytes in dest. */
static void Q_strncpyz(char *dest, const char *src, int destsize);
#endif

/******************************************************************************
 * LOCAL INLINE FUNCTIONS AND FUNCTION MACROS
 ******************************************************************************/

#define ARRAY_LEN(x)            (sizeof(x) / sizeof(*(x)))
#define PAD(base, alignment)    (((base) + (alignment)-1) & ~((alignment)-1))
#define PADLEN(base, alignment) (PAD((base), (alignment)) - (base))
#define PADP(base, alignment)   ((void *)PAD((intptr_t)(base), (alignment)))
#define Q_ftol(v)               ((long)(v))

/******************************************************************************
 * FUNCTION BODIES
 ******************************************************************************/

bool VM_Create(vm_t                *vm,
               const uint8_t *const bytecode,
               const vm_size_t      length,
               uint8_t *const       dataSegment,
               const vm_size_t      dataSegmentLength,
               intptr_t (*systemCalls)(vm_t *, intptr_t *)) {
  if (vm == NULL) {
    Com_Error(VM_INVALID_POINTER, "Invalid vm pointer");
    return -1;
  }
  if (!systemCalls) {
    vm->lastError = VM_NO_SYSCALL_CALLBACK;
    Com_Error(vm->lastError, "No systemcalls provided");
    return -1;
  }

  Com_Memset(vm, 0, sizeof(vm_t));
  {
    const vmHeader_t *header = VM_LoadQVM(vm, bytecode, length, dataSegment, dataSegmentLength);
    if (!header) {
      vm->lastError = VM_FAILED_TO_LOAD_BYTECODE;
      Com_Error(vm->lastError, "Failed to load bytecode");
      VM_Free(vm);
      return -1;
    }

    vm->systemCall = systemCalls;

    /* allocate space for the jump targets, which will be filled in by the
       compile/prep functions */
    vm->instructionCount = header->instructionCount;

    vm->codeLength = header->codeLength;

    if (VM_PrepareInterpreter(vm, header) != 0) {
      VM_Free(vm);
      return -1;
    }
  }

  /* the stack is implicitly at the end of the image */
  vm->programStack = vm->dataAlloc - 4;
#ifdef DEBUG_VM
  vm->stackBottom = vm->programStack - VM_PROGRAM_STACK_SIZE;
#endif

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
  /*Com_Printf(".data length: %6i bytes\n", vm->dataAlloc);
  Com_Printf(".lit  length: %6i bytes\n", vm->litLength);
  Com_Printf(".bss  length: %6i bytes\n", vm->bssLength);*/
  Com_Printf("Stack size:   %6i bytes\n", VM_PROGRAM_STACK_SIZE);
  Com_Printf("Allocated memory: %6i bytes\n", vm->dataAlloc);
  Com_Printf("Instruction count: %i\n", vm->instructionCount);

  return 0;
}
#endif

static const vmHeader_t *VM_LoadQVM(
    vm_t *const vm, const uint8_t *bytecode, vm_size_t length, uint8_t *const dataSegment, const vm_size_t dataSegmentLength) {
  union {
    vmHeader_t    *h;
    const uint8_t *v;
  } header;

  header.v = bytecode;

  Com_Printf("Loading vm\n");

  if (!header.h || !bytecode || length <= vm_sizeof(vmHeader_t) || length > VM_MAX_IMAGE_SIZE) {
    Com_Printf("Failed.\n");
    return NULL;
  }

  if (header.h->vmMagic == VM_MAGIC) {
    /* validate */
    if (header.h->codeLength == 0 || header.h->instructionCount == 0 || header.h->bssLength > VM_MAX_BSS_LENGTH ||
        header.h->codeOffset + header.h->codeLength > (uint32_t)length ||
        header.h->dataOffset + header.h->dataLength + header.h->litLength > (uint32_t)length) {
      Com_Printf("Warning: bad header\n");
      return NULL;
    }
  } else {
    Com_Printf("Warning: Invalid magic number in header "
               "Read: 0x%x, expected: 0x%x\n",
               header.h->vmMagic, VM_MAGIC);
    return NULL;
  }

  {
    vm_size_t dataLength;

    /* round up to next power of 2 so all data operations can
       be mask protected */
    dataLength = header.h->dataLength + header.h->litLength + header.h->bssLength;

    if (dataLength > dataSegmentLength) {
      Com_Error(VM_NOT_ENOUGH_RAM, "Insufficient ram allocated for VM\n");
      return NULL;
    }
  }

  vm->dataBase  = dataSegment;
  vm->dataAlloc = dataSegmentLength;

  /* make sure data section is always initialized with 0
   * (bss would be enough) */
  Com_Memset(vm->dataBase, 0, vm->dataAlloc);

  /* copy the intialized data */
  Com_Memcpy(vm->dataBase, header.v + header.h->dataOffset, header.h->dataLength + header.h->litLength);

  return header.h;
}

intptr_t VM_Call(vm_t *vm, std_int command, ...) {
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
    std_int args[MAX_VMMAIN_ARGS];
    va_list ap;
    uint8_t i;

    args[0] = command;
    va_start(ap, command);
    for (i = 1; i < (uint8_t)ARRAY_LEN(args); i++) {
      args[i] = va_arg(ap, std_int);
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
  if (!vmAddr || !vm) {
    return -1;
  }

  if (vmAddr > vm->dataAlloc || vmAddr + len > vm->dataAlloc) {
    Com_Error(VM_DATA_OUT_OF_RANGE, "Memory access out of range");
    return -1;
  }

  return 0;
}

#ifdef DEBUG_VM
static void Q_strncpyz(char *dest, const char *src, int destsize) {
  if (!dest || !src || destsize < 1) {
    return;
  }
  strlcpy(dest, src, destsize - 1);
  dest[destsize - 1] = 0;
}
#endif

static void VM_BlockCopy(vm_size_t dest, const vm_size_t src, const vm_size_t n, const vm_t *vm) {

  if (VM_MemoryRangeValid(src, n, vm))
    return;

  if (VM_MemoryRangeValid(src, n, vm))
    return;

  Com_Memcpy(vm->dataBase + dest, vm->dataBase + src, n);
}

static bool VM_PrepareInterpreter(vm_t *vm, const vmHeader_t *header) {

  vm->codeBase = (uint8_t *)header + header->codeOffset;

  return false;
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

#define r2                  (*((vm_operand_t *)&codeImage[programCounter]))
#define INT_INCREMENT       sizeof(uint32_t)
#define MAX_PROGRAM_COUNTER ((unsigned)vm->codeLength)
#define DISPATCH2()         goto nextInstruction2
#define DISPATCH()          goto nextInstruction

static std_int VM_CallInterpreted(vm_t *vm, std_int *args) {
  uint8_t       stack[OPSTACK_SIZE + 15];
  vm_operand_t *opStack;
  uint8_t       opStackOfs;
  std_int       programCounter;
  std_int       programStack;
  std_int       stackOnEntry;
  uint8_t      *image;
  uint8_t      *codeImage;
  vm_operand_t  v1;
  std_int       arg;
  uint8_t       opcode;
  vm_operand_t  r0, r1;

#ifdef DEBUG_VM
  std_int     prevProgramCounter;
  vmSymbol_t *profileSymbol;
#endif

  /* we might be called recursively, so this might not be the very top */
  programStack = stackOnEntry = vm->programStack;

#ifdef DEBUG_VM
  profileSymbol = VM_ValueToFunctionSymbol(vm, 0);
  /* uncomment this for debugging breakpoints */
  vm->breakFunction = 0;
#endif

  image          = vm->dataBase;
  codeImage      = vm->codeBase;
  programCounter = 0;
#ifdef DEBUG_VM
  prevProgramCounter = 0;
#endif
  programStack -= (8 + 4 * MAX_VMMAIN_ARGS);

  for (arg = 0; arg < MAX_VMMAIN_ARGS; arg++) {
    *(vm_operand_t *)&image[programStack + 8 + arg * 4] = args[arg];
  }

  *(vm_operand_t *)&image[programStack + 4] = 0;  /* return stack */
  *(vm_operand_t *)&image[programStack]     = -1; /* will terminate the loop on return */

  /* leave a free spot at start of stack so
     that as long as opStack is valid, opStack-1 will
     not corrupt anything */
  opStack    = PADP(stack, 16);
  *opStack   = 0x0000BEEF;
  opStackOfs = 0;

  /* main interpreter loop, will exit when a LEAVE instruction
     grabs the -1 program counter */

  while (1) {
  nextInstruction:
    r0 = opStack[opStackOfs];
    r1 = opStack[(uint8_t)(opStackOfs - 1)];
  nextInstruction2:

#ifdef DEBUG_VM
    if (vm_debugLevel > 1) {
      std_int diff       = programCounter - prevProgramCounter;
      prevProgramCounter = programCounter;
      Com_Printf("%08X[%d]:\t", programCounter, diff);
    }
#endif

    opcode = codeImage[programCounter++];

#ifdef DEBUG_VM
    if (programCounter >= vm->codeLength) {
      vm->lastError = VM_PC_OUT_OF_RANGE;
      Com_Error(vm->lastError, "VM pc out of range");
      return -1;
    }

    if (programStack <= vm->stackBottom) {
      vm->lastError = VM_STACK_OVERFLOW;
      Com_Error(vm->lastError, "VM stack overflow");
      return -1;
    }

    if (programStack & 3) {
      vm->lastError = VM_STACK_MISALIGNED;
      Com_Error(vm->lastError, "VM program stack misaligned");
      return -1;
    }

    if (vm_debugLevel > 1) {
      Com_Printf("%s%i %s\t(%02X %08X);\tSP=%d, R0=%08X, R1=%08X \n", VM_Indent(vm), opStackOfs,
                 opnames[opcode & OPCODE_TABLE_MASK], opcode, r2, programStack, r0, r1);
    }
    profileSymbol->profileCount++;
#endif /* DEBUG_VM */
    switch (opcode) {
#ifdef DEBUG_VM
    default: /* fall through */
#endif
    goto_OP_UNDEF:
      Com_Error(vm->lastError = VM_BAD_INSTRUCTION, "Bad VM instruction");
      return -1;
    goto_OP_IGNORE:
      DISPATCH2();
    goto_OP_BREAK:
#ifdef DEBUG_VM
      vm->breakCount++;
#endif
      DISPATCH2();
    goto_OP_CONST:
      opStackOfs++;
      r1 = r0;
      r0 = opStack[opStackOfs] = r2;

      programCounter += INT_INCREMENT;
      DISPATCH2();
    goto_OP_LOCAL:
      opStackOfs++;
      r1 = r0;
      r0 = opStack[opStackOfs] = r2 + programStack;

      programCounter += INT_INCREMENT;
      DISPATCH2();
    goto_OP_LOAD4:
#ifdef DEBUG_VM
      if (opStack[opStackOfs] & 3) {
        vm->lastError = VM_OP_LOAD4_MISALIGNED;
        Com_Error(vm->lastError, "OP_LOAD4 misaligned");
        return -1;
      }
#endif
      r0 = opStack[opStackOfs] = *(vm_operand_t *)&image[r0];
      DISPATCH2();
    goto_OP_LOAD2:
      r0 = opStack[opStackOfs] = *(unsigned short *)&image[r0];
      DISPATCH2();
    goto_OP_LOAD1:
      r0 = opStack[opStackOfs] = image[r0];
      DISPATCH2();

    goto_OP_STORE4:
      *(vm_operand_t *)&image[r1] = r0;
      opStackOfs -= 2;
      DISPATCH();
    goto_OP_STORE2:
      *(short *)&image[r1] = r0;
      opStackOfs -= 2;
      DISPATCH();
    goto_OP_STORE1:
      image[r1] = r0;
      opStackOfs -= 2;
      DISPATCH();
    goto_OP_ARG:
      /* single byte offset from programStack */
      *(vm_operand_t *)&image[(codeImage[programCounter] + programStack)] = r0;
      opStackOfs--;
      programCounter += 1;
      DISPATCH();
    goto_OP_BLOCK_COPY:
      VM_BlockCopy(r1, r0, r2, vm);
      programCounter += INT_INCREMENT;
      opStackOfs -= 2;
      DISPATCH();
    goto_OP_CALL:
      /* save current program counter */
      *(vm_operand_t *)&image[programStack] = (vm_operand_t)programCounter;

      /* jump to the location on the stack */
      programCounter = r0;
      opStackOfs--;
      if (programCounter < 0) /* system call */
      {
        vm_operand_t r;
#ifdef DEBUG_VM
        if (vm_debugLevel) {
          Com_Printf("%s%i---> systemcall(%i)\n", VM_Indent(vm), opStackOfs, -1 - programCounter);
        }
#endif
        /* save the stack to allow recursive VM entry */
        vm->programStack = programStack - 4;
#ifdef DEBUG_VM
        int stomped = *(int *)&image[programStack + 4];
#endif
        *(vm_operand_t *)&image[programStack + 4] = -1 - programCounter;

        {
          intptr_t      argarr[MAX_VMSYSCALL_ARGS];
          vm_operand_t *imagePtr = (vm_operand_t *)&image[programStack];
          std_int       i;

          for (i = 0; i < (std_int)ARRAY_LEN(argarr); ++i)
            argarr[i] = *(++imagePtr);

          r = vm->systemCall(vm, argarr);
        }

#ifdef DEBUG_VM
        /* this is just our stack frame pointer, only needed
           for debugging */
        *(int *)&image[programStack + 4] = stomped;
#endif

        /* save return value */
        opStackOfs++;
        opStack[opStackOfs] = r;
        programCounter      = *(vm_operand_t *)&image[programStack];
#ifdef DEBUG_VM
        if (vm_debugLevel) {
          Com_Printf("%s%i<--- %s\n", VM_Indent(vm), opStackOfs, VM_ValueToSymbol(vm, programCounter));
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
    goto_OP_PUSH:
      opStackOfs++;
      DISPATCH();
    goto_OP_POP:
      opStackOfs--;
      DISPATCH();
    goto_OP_ENTER:
      /* get size of stack frame */
      v1 = r2;
      programCounter += INT_INCREMENT;
      programStack -= v1;
#ifdef DEBUG_VM
      profileSymbol = VM_ValueToFunctionSymbol(vm, programCounter - 3);
      /* save old stack frame for debugging traces */
      *(int *)&image[programStack + 4] = programStack + v1;
      if (vm_debugLevel) {
        Com_Printf("%s%i---> %s\n", VM_Indent(vm), opStackOfs, VM_ValueToSymbol(vm, programCounter - 5 - 3));
        if (vm->breakFunction && programCounter - 5 - 3 == vm->breakFunction) {
          /* this is to allow setting breakpoints here in the
           * debugger */
          vm->breakCount++;
          VM_StackTrace(vm, programCounter - 3, programStack);
        }
      }
#endif
      DISPATCH();
    goto_OP_LEAVE:
      /* remove our stack frame */
      v1 = r2;

      programStack += v1;

      /* grab the saved program counter */
      programCounter = *(vm_operand_t *)&image[programStack];
#ifdef DEBUG_VM
      profileSymbol = VM_ValueToFunctionSymbol(vm, programCounter);
      if (vm_debugLevel) {
        Com_Printf("%s%i<--- %s\n", VM_Indent(vm), opStackOfs, VM_ValueToSymbol(vm, programCounter));
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

      /*
         ===================================================================
         BRANCHES
         ===================================================================
         */

    goto_OP_JUMP:
      if ((unsigned)r0 >= MAX_PROGRAM_COUNTER) {
        Com_Error(vm->lastError = VM_PC_OUT_OF_RANGE, "VM program counter out of range in OP_JUMP");
        return -1;
      }

      programCounter = r0;

      opStackOfs--;
      DISPATCH();
    goto_OP_EQ:
      opStackOfs -= 2;
      if (r1 == r0) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }
    goto_OP_NE:
      opStackOfs -= 2;
      if (r1 != r0) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }
    goto_OP_LTI:
      opStackOfs -= 2;
      if (r1 < r0) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }
    goto_OP_LEI:
      opStackOfs -= 2;
      if (r1 <= r0) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }
    goto_OP_GTI:
      opStackOfs -= 2;
      if (r1 > r0) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }
    goto_OP_GEI:
      opStackOfs -= 2;
      if (r1 >= r0) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }
    goto_OP_LTU:
      opStackOfs -= 2;
      if (((unsigned)r1) < ((unsigned)r0)) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }
    goto_OP_LEU:
      opStackOfs -= 2;
      if (((unsigned)r1) <= ((unsigned)r0)) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }
    goto_OP_GTU:
      opStackOfs -= 2;
      if (((unsigned)r1) > ((unsigned)r0)) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }
    goto_OP_GEU:
      opStackOfs -= 2;
      if (((unsigned)r1) >= ((unsigned)r0)) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }
    goto_OP_EQF:
      opStackOfs -= 2;

      if (((float *)opStack)[(uint8_t)(opStackOfs + 1)] == ((float *)opStack)[(uint8_t)(opStackOfs + 2)]) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }
    goto_OP_NEF:
      opStackOfs -= 2;

      if (((float *)opStack)[(uint8_t)(opStackOfs + 1)] != ((float *)opStack)[(uint8_t)(opStackOfs + 2)]) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }
    goto_OP_LTF:
      opStackOfs -= 2;

      if (((float *)opStack)[(uint8_t)(opStackOfs + 1)] < ((float *)opStack)[(uint8_t)(opStackOfs + 2)]) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }
    goto_OP_LEF:
      opStackOfs -= 2;

      if (((float *)opStack)[(uint8_t)((uint8_t)(opStackOfs + 1))] <= ((float *)opStack)[(uint8_t)((uint8_t)(opStackOfs + 2))]) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }
    goto_OP_GTF:
      opStackOfs -= 2;

      if (((float *)opStack)[(uint8_t)(opStackOfs + 1)] > ((float *)opStack)[(uint8_t)(opStackOfs + 2)]) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }
    goto_OP_GEF:
      opStackOfs -= 2;

      if (((float *)opStack)[(uint8_t)(opStackOfs + 1)] >= ((float *)opStack)[(uint8_t)(opStackOfs + 2)]) {
        programCounter = r2;
        DISPATCH();
      } else {
        programCounter += INT_INCREMENT;
        DISPATCH();
      }

      /*===================================================================*/

    goto_OP_NEGI:
      opStack[opStackOfs] = -r0;
      DISPATCH();
    goto_OP_ADD:
      opStackOfs--;
      opStack[opStackOfs] = r1 + r0;
      DISPATCH();
    goto_OP_SUB:
      opStackOfs--;
      opStack[opStackOfs] = r1 - r0;
      DISPATCH();
    goto_OP_DIVI:
      opStackOfs--;
      opStack[opStackOfs] = r1 / r0;
      DISPATCH();
    goto_OP_DIVU:
      opStackOfs--;
      opStack[opStackOfs] = ((unsigned)r1) / ((unsigned)r0);
      DISPATCH();
    goto_OP_MODI:
      opStackOfs--;
      opStack[opStackOfs] = r1 % r0;
      DISPATCH();
    goto_OP_MODU:
      opStackOfs--;
      opStack[opStackOfs] = ((unsigned)r1) % ((unsigned)r0);
      DISPATCH();
    goto_OP_MULI:
      opStackOfs--;
      opStack[opStackOfs] = r1 * r0;
      DISPATCH();
    goto_OP_MULU:
      opStackOfs--;
      opStack[opStackOfs] = ((unsigned)r1) * ((unsigned)r0);
      DISPATCH();
    goto_OP_BAND:
      opStackOfs--;
      opStack[opStackOfs] = ((unsigned)r1) & ((unsigned)r0);
      DISPATCH();
    goto_OP_BOR:
      opStackOfs--;
      opStack[opStackOfs] = ((unsigned)r1) | ((unsigned)r0);
      DISPATCH();
    goto_OP_BXOR:
      opStackOfs--;
      opStack[opStackOfs] = ((unsigned)r1) ^ ((unsigned)r0);
      DISPATCH();
    goto_OP_BCOM:
      opStack[opStackOfs] = ~((unsigned)r0);
      DISPATCH();
    goto_OP_LSH:
      opStackOfs--;
      opStack[opStackOfs] = r1 << r0;
      DISPATCH();
    goto_OP_RSHI:
      opStackOfs--;
      opStack[opStackOfs] = r1 >> r0;
      DISPATCH();
    goto_OP_RSHU:
      opStackOfs--;
      opStack[opStackOfs] = ((unsigned)r1) >> r0;
      DISPATCH();
    goto_OP_NEGF:
      ((float *)opStack)[opStackOfs] = -((float *)opStack)[opStackOfs];
      DISPATCH();
    goto_OP_ADDF:
      opStackOfs--;
      ((float *)opStack)[opStackOfs] = ((float *)opStack)[opStackOfs] + ((float *)opStack)[(uint8_t)(opStackOfs + 1)];
      DISPATCH();
    goto_OP_SUBF:
      opStackOfs--;
      ((float *)opStack)[opStackOfs] = ((float *)opStack)[opStackOfs] - ((float *)opStack)[(uint8_t)(opStackOfs + 1)];
      DISPATCH();
    goto_OP_DIVF:
      opStackOfs--;
      ((float *)opStack)[opStackOfs] = ((float *)opStack)[opStackOfs] / ((float *)opStack)[(uint8_t)(opStackOfs + 1)];
      DISPATCH();
    goto_OP_MULF:
      opStackOfs--;
      ((float *)opStack)[opStackOfs] = ((float *)opStack)[opStackOfs] * ((float *)opStack)[(uint8_t)(opStackOfs + 1)];
      DISPATCH();
    goto_OP_CVIF:
      ((float *)opStack)[opStackOfs] = (float)opStack[opStackOfs];
      DISPATCH();
    goto_OP_CVFI:
      opStack[opStackOfs] = Q_ftol(((float *)opStack)[opStackOfs]);
      DISPATCH();
    goto_OP_SEX8:
      opStack[opStackOfs] = (int8_t)opStack[opStackOfs];
      DISPATCH();
    goto_OP_SEX16:
      opStack[opStackOfs] = (int16_t)opStack[opStackOfs];
      DISPATCH();
    }
  }

done:
  if (opStackOfs != 1 || *opStack != 0x0000BEEF) {
    Com_Error(vm->lastError = VM_STACK_ERROR, "Interpreter stack error");
  }

  vm->programStack = stackOnEntry;

  /* return the result of the bytecode computations */
  return opStack[opStackOfs];
}

/* DEBUG FUNCTIONS */
/* --------------- */

#ifdef DEBUG_VM

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

  snprintf(text, sizeof(text), "%s+%i", sym->symName, value - sym->symValue);

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
  char         symbols[VM_MAX_QPATH];
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
    Q_strncpyz(sym->symName, token, chars + 1);

    count++;
  }

  vm->numSymbols = count;
  Com_Printf("%i symbols parsed from %s\n", count, symbols);
}

static void VM_StackTrace(vm_t *vm, int programCounter, int programStack) {
  int count;

  count = 0;
  do {
    Com_Printf("%s\n", VM_ValueToSymbol(vm, programCounter));
    programStack   = *(int *)&vm->dataBase[programStack + 4];
    programCounter = *(int *)&vm->dataBase[programStack];
  } while (programCounter != -1 && ++count < 32);
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
