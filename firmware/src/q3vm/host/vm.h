/*
 __    _  ___  _____    ___   _______     ____  __
|  \  | |/ _ \|_   _|  / _ \ |___ /\ \   / /  \/  |
| | \ | | | | | | |   | | | |  |_ \ \ \ / /| |\/| |
| |\ \| | |_| | | |   | |_| |____) | \ V / | |  | |
|_| \___|\___/  |_|    \__\_______/   \_/  |_|  |_|

   NOT Quake III Arena Virtual Machine
*/

#ifndef __Q3VM_H
#define __Q3VM_H

#include "target-support.h"

/******************************************************************************
 * SYSTEM INCLUDE FILES
 ******************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>  /* remove this if Com_Printf does not point to printf */
#include <string.h> /* remove this if Com_Mem*** does not point to memcpy */

/******************************************************************************
 * DEFINES
 ******************************************************************************/
#if 0
#define MEMORY_SAFE
#endif

#if 0
#define DEBUG_VM /**< ifdef: enable debug functions and additional checks */
#endif

/** File start magic number for .qvm files (4 bytes, little endian) */
#define VM_MAGIC 0x44494E4F

/** Don't change stack size: Hardcoded in q3asm and reserved at end of BSS */
#define VM_PROGRAM_STACK_SIZE 0x1000

/** Max. number of bytes in .qvm */
#define VM_MAX_IMAGE_SIZE 0x400000

/**< Maximum length of a pathname, 64 to be Q3 compatible */
#define VM_MAX_QPATH 64

/** Redirect memset() calls with this macro */
#define Com_Memset memset

/** Redirect memcpy() calls with this macro */
#define Com_Memcpy memcpy

/** Translate from virtual machine memory to real machine memory. */
/*#define VMA(x, vm) VM_ArgPtr(args[x], vm)*/

/** Translate from virtual machine memory to real machine memory. */
#define VMA_PTR(x, vm) VM_ArgPtr(to_ustdint((*(uint24_t *)&(args[x]))), vm)

#define VMA_UINT24(x) to_ustdint((*(uint24_t *)&(args[x])))

#define VMA_F4(x) (*(float *)&(args[x]))

/** Get argument in syscall and interpret it bit by bit as IEEE 754 float */

typedef int std_int; /* can be a 32 or 24 bit number - depending on target CPU */

typedef int32_t vm_operand_t; /* int to store registers and instruction operand values */

/******************************************************************************
 * TYPEDEFS
 ******************************************************************************/

/** VM error codes */
typedef enum {
  VM_NO_ERROR                    = 0,   /**< 0 = OK */
  VM_INVALID_POINTER             = -1,  /**< NULL pointer for vm_t */
  VM_FAILED_TO_LOAD_BYTECODE     = -2,  /**< Invalid byte code */
  VM_NO_SYSCALL_CALLBACK         = -3,  /**< Syscall pointer missing */
  VM_FREE_ON_RUNNING_VM          = -4,  /**< Call to VM_Free while running */
  VM_BLOCKCOPY_OUT_OF_RANGE      = -5,  /**< VM tries to escape sandbox */
  VM_PC_OUT_OF_RANGE             = -6,  /**< Program counter out of range */
  VM_JUMP_TO_INVALID_INSTRUCTION = -7,  /**< VM tries to escape sandbox */
  VM_STACK_OVERFLOW              = -8,  /**< Only in DEBUG_VM mode */
  VM_STACK_ERROR                 = -11, /**< Stack corrupted after call */
  VM_DATA_OUT_OF_RANGE           = -12, /**< Syscall pointer not in sandbox */
  VM_MALLOC_FAILED               = -13, /**< Not enough memory */
  VM_BAD_INSTRUCTION             = -14, /**< Unknown OP code in bytecode */
  VM_NOT_LOADED                  = -15, /**< VM not loaded */
  VM_NOT_ENOUGH_RAM              = -16, /**< insufficient ram allocated for VM */
  VM_MALFORMED_HEADER            = -17,
  VM_ILLEGAL_OPCODE              = -18,
  VM_LIT_ACCESS_ERROR            = -19,
  VM_RAM_ACCESS_ERROR            = -20
} vmErrorCode_t;

/** File header of a bytecode .qvm file. Can be directly mapped to the start of
 *  the file. This is always little endian encoded in the file. */
typedef struct {
  uint32_t vmMagic;    /**< 00: Bytecode image shall start with VM_MAGIC */
  uint24_t codeLength; /**< 07: Bytes in code segment */
  uint24_t litLength;  /**< 0A: Bytes in strings segment (after .data segment) */
  uint24_t dataLength; /**< 0D: Bytes in .data segment */
  uint24_t bssLength;  /**< 10: How many bytes should be used for .bss segment */
} vmHeader_t;

#ifdef DEBUG_VM
/** For debugging (DEBUG_VM): symbol list */
typedef struct vmSymbol_s {
  struct vmSymbol_s *next; /**< Linked list of symbols */

  int symValue;      /**< Value of symbol that we want to have the ASCII name for
                      */
  int  profileCount; /**< For the runtime profiler. +1 for each call. */
  char symName[1];   /**< Variable sized symbol name. Space is reserved by
                        malloc at load time. */
} vmSymbol_t;
#endif

typedef void (*vm_aborted_t)(vmErrorCode_t);

/** Main struct (think of a kind of a main class) to keep all information of
 * the virtual machine together. Has pointer to the bytecode, the stack and
 * everything. Call VM_Create(...) to initialize this struct. */
typedef struct vm_s {
  /** Function pointer to callback function for native functions called by
   * the bytecode. The function is identified by an integer id that
   * corresponds to the bytecode function ids defined in g_syscalls.asm.
   * Note however that parms equals to (-1 - function_id).
   * So -1 in g_syscalls.asm equals to 0 in the systemCall parms argument,
   *    -2 in g_syscalls.asm equals to 1 in the systemCall parms argument,
   *    -3 in g_syscalls.asm equals to 2 in the systemCall parms argument
   * and so on. You can convert it back to -1, -2, -3, but the 0 based
   * index might help a lookup table. */
  uint32_t (*systemCall)(struct vm_s *vm, uint8_t *parms);
  vm_aborted_t vm_aborted;

  /*------------------------------------*/

  const uint8_t *codeBase;   /**< Bytecode code segment in ROM */
  vm_size_t      codeLength; /**< Number of bytes in code segment */

  const uint8_t *litBase;   /* Start of the lit segment in ROM */
  vm_size_t      litLength; /* length of the lit segment*/

  uint8_t  *dataBase; /* Not the actual start location.  Its a calculated offset - workingRam - litLength */
  vm_size_t dataLength;

  uint8_t  *bssBase; /* Calculated offset of BSS base in RAM */
  vm_size_t bssLength;

  vm_size_t programStack; /* Index of current stack in RAM - initialised at top of RAM */

  vm_size_t workingRAMLength; /**< Number of bytes allocated for dataBase */

  vmErrorCode_t lastError; /**< Last known error */

  struct vm_s *vm;

#ifdef MEMORY_SAFE
  vm_size_t stackTop;    /**< If programStack < stackBottom, error */
  vm_size_t stackBottom; /**< If programStack < stackBottom, error */
#endif

  /*------------------------------------*/

#ifdef DEBUG_VM
  uint8_t    *debugStorage;
  stdint_t    debugStorageLength;
  std_int     numSymbols;    /**< Number of symbols from VM_LoadSymbols */
  vmSymbol_t *symbols;       /**< By VM_LoadSymbols: names for debugging */
  std_int     breakFunction; /**< For debugging: break at this function */
  std_int     breakCount;    /**< Used for breakpoints, triggered by OP_BREAK */
  ustdint_t   callLevel;     /**< Counts recursive VM_Call */
#endif

} vm_t;

typedef uint32_t (*systemCalls_t)(vm_t *, uint8_t *);

/******************************************************************************
 * FUNCTION PROTOTYPES
 ******************************************************************************/

/** Initialize a virtual machine.
 * @param[out] vm Pointer to a virtual machine to initialize.
 * @param[in] module Path to the bytecode file. Used to load the
 *                   symbols. Otherwise not strictly required.
 * @param[in] bytecode Pointer to the bytecode. Directly byte by byte
 *                     the content of the .qvm file.
 * @param[in] length Number of bytes in the bytecode array.
 * @param[in] systemCalls Function pointer to callback function for native
 *   functions called by the bytecode. The function is identified by an integer
 *   id that corresponds to the bytecode function ids defined in g_syscalls.asm.
 *   Note however that parms equals to (-1 - function_id). So -1 in
 *   g_syscalls.asm equals to 0 in the systemCall parms argument, -2 in
 *   g_syscalls.asm is 1 in parms, -3 is 2 and so on.
 * @return 0 if everything is OK. negative if something went wrong. */
int VM_Create(vm_t                *vm,
              const uint8_t *const bytecode,
              const vm_size_t      length,
              uint8_t *const       dataSegment,
              const vm_size_t      dataSegmentLength,
              systemCalls_t        systemCalls,
              vm_aborted_t         vm_aborted);

#ifdef DEBUG_VM
int VM_LoadDebugInfo(vm_t *vm, char *mapfileImage, uint8_t *debugStorage, int debugStorageLength);
#endif

/** Run a function from the virtual machine.
 * Use the command argument to tell the VM what to do.
 * You can supply additional (up to (`MAX_VMMAIN_ARGS`-1)) parameters to pass to the bytecode.
 * @param[in] vm Pointer to initialized virtual machine.
 * @param[in] command Basic parameter passed to the bytecode.
 * @return Return value of the function call by the VM. */
intptr_t VM_Call(vm_t *vm, ustdint_t command, ...);

/** Helper function for syscalls VMA(x) macro:
 * Translate from virtual machine memory to real machine memory.
 * @param[in] vmAddr Address in virtual machine memory
 * @param[in,out] vm Current VM
 * @return translated address. */
void *VM_ArgPtr(intptr_t vmAddr, vm_t *const vm);

/** Helper function for syscalls VMF(x) macro:
 * Get argument in syscall and interpret it bit by bit as IEEE 754 float.
 * That is: just put the int32_t in a float/int32_t union and return the float.
 * If the VM calls a native function with a float argument: don't
 * cast the in32_t argument to a float, but rather interpret it directly
 * as a floating point variable.
 * @param[in] x Argument on call stack.
 * @return Value as float. */
float VM_IntToFloat(int32_t x);

/** Helper function for syscalls:
 * Just put the float in a float/int32_t union and return the int32_t.
 * @param[in] f Floating point number.
 * @return Floating point number as integer */
int32_t VM_FloatToInt(float f);

#ifdef MEMORY_SAFE

bool VM_VerifyWriteOK(vm_t *vm, vm_size_t vaddr, int size);
bool VM_VerifyReadOK(vm_t *vm, vm_size_t vaddr, int size);

#endif

#ifdef DEBUG_VM
/** Print call statistics for every function. Only works with DEBUG_VM.
 * Does nothing if DEBUG_VM is not defined.
 * @param[in] vm VM to profile */
void VM_VmProfile_f(vm_t *vm);

/** Set the printf debug level. Only useful with DEBUG_VM.
 * Set to 1 for general informations and 2 to output every opcode name.
 * @param[in] level If level is 0: be quiet (default). */
void VM_Debug(uint8_t level);
#else
#define VM_Debug(a)
#define VM_VmProfile_f(vm)
#endif

#endif /* __Q3VM_H */
