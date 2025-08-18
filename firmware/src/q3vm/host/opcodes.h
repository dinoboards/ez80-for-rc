#ifndef __OPCODES
#define __OPCODES

#include <stdint.h>

/** Enum for the virtual machine op codes */
typedef enum {
  _OP_UNDEF,      /* 00: Error: VM halt */
  _OP_IGNORE,     /* 01: No operation */
  _OP_BREAK,      /* 02: vm->breakCount++ */
  _OP_ENTER,      /* 03: Begin subroutine. */
  _OP_LEAVE,      /* 04: End subroutine. */
  _OP_CALL,       /* 05: Call subroutine. */
  _OP_PUSH,       /* 06: Push to stack. */
  _OP_POP,        /* 07: Discard top-of-stack. */
  _OP_CONSTGP4,   /* 08: Load constant to stack. */
  _OP_LOCAL,      /* 09: Get local variable. */
  _OP_JUMP,       /* 0A: Unconditional jump. */
  _OP_EQ,         /* 0B: Compare integers, jump if equal. */
  _OP_NE,         /* 0C: Compare integers, jump if not equal. */
  _OP_LTI,        /* 0D: Compare integers, jump if less-than. */
  _OP_LEI,        /* OE: Compare integers, jump if less-than-or-equal. */
  _OP_GTI,        /* 0F: Compare integers, jump if greater-than. */
  _OP_GEI,        /* 10: Compare integers, jump if greater-than-or-equal. */
  _OP_LTU,        /* 11: Compare unsigned integers, jump if less-than */
  _OP_LEU,        /* 12: Compare unsigned integers, jump if less-than-or-equal */
  _OP_GTU,        /* 13: Compare unsigned integers, jump if greater-than */
  _OP_GEU,        /* 14: Compare unsigned integers, jump if greater-than-or-equal */
  _OP_EQF,        /* 15: Compare floats, jump if equal */
  _OP_NEF,        /* 16: Compare floats, jump if not-equal */
  _OP_LTF,        /* 17: Compare floats, jump if less-than */
  _OP_LEF,        /* 18: Compare floats, jump if less-than-or-equal */
  _OP_GTF,        /* 19: Compare floats, jump if greater-than */
  _OP_GEF,        /* 1A: Compare floats, jump if greater-than-or-equal */
  _OP_LOAD1,      /* 1B: Load 1-byte from memory */
  _OP_LOAD2,      /* 1C: Load 2-bytes from memory */
  _OP_LOAD4,      /* 1D: Load 4-bytes from memory */
  _OP_LOADF4,     /* 1E: Load 4 byte float from memory */
  _OP_STORE1,     /* 1F: Store 1-byte to memory */
  _OP_STORE2,     /* 20: Store 2-byte to memory */
  _OP_STORE4,     /* 21: *(stack[top-1]) = stack[top] */
  _OP_STOREF4,    /* 22: *(stack[top-1]) = stack[top] */
  _OP_ARG,        /* 23: Marshal argument */
  _OP_BLOCK_COPY, /* 24: memcpy */
  _OP_SEX8,       /* 25: Sign-Extend 8-bit */
  _OP_SEX16,      /* 26: Sign-Extend 16-bit */
  _OP_NEGI,       /* 27: Negate integer. */
  _OP_ADD,        /* 28: Add integers (two's complement). */
  _OP_SUB,        /* 29: Subtract integers (two's complement). */
  _OP_DIVI,       /* 2A: Divide signed integers. */
  _OP_DIVU,       /* 2B: Divide unsigned integers. */
  _OP_MODI,       /* 2C: Modulus (signed). */
  _OP_MODU,       /* 2D: Modulus (unsigned). */
  _OP_MULI,       /* 2E: Multiply signed integers. */
  _OP_MULU,       /* 2F: Multiply unsigned integers. */
  _OP_BAND,       /* 30: Bitwise AND */
  _OP_BOR,        /* 31: Bitwise OR */
  _OP_BXOR,       /* 32: Bitwise eXclusive-OR */
  _OP_BCOM,       /* 33: Bitwise COMplement */
  _OP_LSH,        /* 34: Left-shift */
  _OP_RSHI,       /* 35: Right-shift (algebraic; preserve sign) */
  _OP_RSHU,       /* 36: Right-shift (bitwise; ignore sign) */
  _OP_NEGF,       /* 37: Negate float */
  _OP_ADDF,       /* 38: Add floats */
  _OP_SUBF,       /* 39: Subtract floats */
  _OP_DIVF,       /* 3A: Divide floats */
  _OP_MULF,       /* 3B: Multiply floats */
  _OP_CVIF,       /* 3C: Convert to integer from float */
  _OP_CVFI,       /* 3D: Convert to float from integer */
  _OP_CONSTU1,    /* 3E: */
  _OP_CONSTI1,    /* 3F: */
  _OP_CONSTU2,    /* 40: */
  _OP_CONSTI2,    /* 41: */
  _OP_CONSTU4,    /* 42: */
  _OP_CONSTI4,    /* 43: */
  _OP_CONSTF4,    /* 44: */
  _OP_CONSTP4,    /* 45: */
  _OP_LOAD3,      /* 46: Load 3-bytes from memory */
  _OP_ADD3,
  _OP_BAND3,
  _OP_BCOM3,
  _OP_BOR3,
  _OP_BXOR3,
  _OP_CONSTI3,
  _OP_CONSTU3,
  _OP_CVFI3,
  _OP_CVIU3,
  _OP_CVUI3,
  _OP_DIVI3,
  _OP_DIVU3,
  _OP_EQ3,
  _OP_GEI3,
  _OP_GEU3,
  _OP_GTI3,
  _OP_GTU3,
  _OP_LEI3,
  _OP_LEU3,
  _OP_LSH3,
  _OP_LTI3,
  _OP_LTU3,
  _OP_MODI3,
  _OP_MODU3,
  _OP_MULI3,
  _OP_MULU3,
  _OP_NE3,
  _OP_NEGI3,
  _OP_RSHI3,
  _OP_RSHU3,
  _OP_SEX24,
  _OP_SEX8_3,
  _OP_STORE3,
  _OP_SUB3,
  _OP_CONSTGP3,
  _OP_CONSTP3,
  _OP_ARGF,
  _OP_ARG4,

  _OP_MAX /* ??: Make this the last item */
} opcode_t;

#define OPCODE_TABLE_SIZE _OP_MAX

#define OP_UNDEF      ((uint8_t)_OP_UNDEF)      /* 00: Error: VM halt */
#define OP_IGNORE     ((uint8_t)_OP_IGNORE)     /* 01: No operation */
#define OP_BREAK      ((uint8_t)_OP_BREAK)      /* 02: vm->breakCount++ */
#define OP_ENTER      ((uint8_t)_OP_ENTER)      /* 03: Begin subroutine. */
#define OP_LEAVE      ((uint8_t)_OP_LEAVE)      /* 04: End subroutine. */
#define OP_CALL       ((uint8_t)_OP_CALL)       /* 05: Call subroutine. */
#define OP_PUSH       ((uint8_t)_OP_PUSH)       /* 06: Push to stack. */
#define OP_POP        ((uint8_t)_OP_POP)        /* 07: Discard top-of-stack. */
#define OP_CONSTGP4   ((uint8_t)_OP_CONSTGP4)   /* 08: Load constant to stack. */
#define OP_LOCAL      ((uint8_t)_OP_LOCAL)      /* 09: Get local variable. */
#define OP_JUMP       ((uint8_t)_OP_JUMP)       /* 0A: Unconditional jump. */
#define OP_EQ         ((uint8_t)_OP_EQ)         /* 0B: Compare integers, jump if equal. */
#define OP_NE         ((uint8_t)_OP_NE)         /* 0C: Compare integers, jump if not equal. */
#define OP_LTI        ((uint8_t)_OP_LTI)        /* 0D: Compare integers, jump if less-than. */
#define OP_LEI        ((uint8_t)_OP_LEI)        /* OE: Compare integers, jump if less-than-or-equal. */
#define OP_GTI        ((uint8_t)_OP_GTI)        /* 0F: Compare integers, jump if greater-than. */
#define OP_GEI        ((uint8_t)_OP_GEI)        /* 10: Compare integers, jump if greater-than-or-equal. */
#define OP_LTU        ((uint8_t)_OP_LTU)        /* 11: Compare unsigned integers, jump if less-than */
#define OP_LEU        ((uint8_t)_OP_LEU)        /* 12: Compare unsigned integers, jump if less-than-or-equal */
#define OP_GTU        ((uint8_t)_OP_GTU)        /* 13: Compare unsigned integers, jump if greater-than */
#define OP_GEU        ((uint8_t)_OP_GEU)        /* 14: Compare unsigned integers, jump if greater-than-or-equal */
#define OP_EQF        ((uint8_t)_OP_EQF)        /* 15: Compare floats, jump if equal */
#define OP_NEF        ((uint8_t)_OP_NEF)        /* 16: Compare floats, jump if not-equal */
#define OP_LTF        ((uint8_t)_OP_LTF)        /* 17: Compare floats, jump if less-than */
#define OP_LEF        ((uint8_t)_OP_LEF)        /* 18: Compare floats, jump if less-than-or-equal */
#define OP_GTF        ((uint8_t)_OP_GTF)        /* 19: Compare floats, jump if greater-than */
#define OP_GEF        ((uint8_t)_OP_GEF)        /* 1A: Compare floats, jump if greater-than-or-equal */
#define OP_LOAD1      ((uint8_t)_OP_LOAD1)      /* 1B: Load 1-byte from memory */
#define OP_LOAD2      ((uint8_t)_OP_LOAD2)      /* 1C: Load 2-bytes from memory */
#define OP_LOAD4      ((uint8_t)_OP_LOAD4)      /* 1D: Load 4-bytes from memory */
#define OP_LOADF4     ((uint8_t)_OP_LOADF4)     /* 1E: Load 4 byte float from memory */
#define OP_STORE1     ((uint8_t)_OP_STORE1)     /* 1F: Store 1-byte to memory */
#define OP_STORE2     ((uint8_t)_OP_STORE2)     /* 20: Store 2-byte to memory */
#define OP_STORE4     ((uint8_t)_OP_STORE4)     /* 21: *(stack[top-1]) = stack[top] */
#define OP_STOREF4    ((uint8_t)_OP_STOREF4)    /* 22: *(stack[top-1]) = stack[top] */
#define OP_ARG        ((uint8_t)_OP_ARG)        /* 23: Marshal argument */
#define OP_BLOCK_COPY ((uint8_t)_OP_BLOCK_COPY) /* 24: memcpy */
#define OP_SEX8       ((uint8_t)_OP_SEX8)       /* 25: Sign-Extend 8-bit */
#define OP_SEX16      ((uint8_t)_OP_SEX16)      /* 26: Sign-Extend 16-bit */
#define OP_NEGI       ((uint8_t)_OP_NEGI)       /* 27: Negate integer. */
#define OP_ADD        ((uint8_t)_OP_ADD)        /* 28: Add integers (two's complement). */
#define OP_SUB        ((uint8_t)_OP_SUB)        /* 29: Subtract integers (two's complement). */
#define OP_DIVI       ((uint8_t)_OP_DIVI)       /* 2A: Divide signed integers. */
#define OP_DIVU       ((uint8_t)_OP_DIVU)       /* 2B: Divide unsigned integers. */
#define OP_MODI       ((uint8_t)_OP_MODI)       /* 2C: Modulus (signed). */
#define OP_MODU       ((uint8_t)_OP_MODU)       /* 2D: Modulus (unsigned). */
#define OP_MULI       ((uint8_t)_OP_MULI)       /* 2E: Multiply signed integers. */
#define OP_MULU       ((uint8_t)_OP_MULU)       /* 2F: Multiply unsigned integers. */
#define OP_BAND       ((uint8_t)_OP_BAND)       /* 30: Bitwise AND */
#define OP_BOR        ((uint8_t)_OP_BOR)        /* 31: Bitwise OR */
#define OP_BXOR       ((uint8_t)_OP_BXOR)       /* 32: Bitwise eXclusive-OR */
#define OP_BCOM       ((uint8_t)_OP_BCOM)       /* 33: Bitwise COMplement */
#define OP_LSH        ((uint8_t)_OP_LSH)        /* 34: Left-shift */
#define OP_RSHI       ((uint8_t)_OP_RSHI)       /* 35: Right-shift (algebraic; preserve sign) */
#define OP_RSHU       ((uint8_t)_OP_RSHU)       /* 36: Right-shift (bitwise; ignore sign) */
#define OP_NEGF       ((uint8_t)_OP_NEGF)       /* 37: Negate float */
#define OP_ADDF       ((uint8_t)_OP_ADDF)       /* 38: Add floats */
#define OP_SUBF       ((uint8_t)_OP_SUBF)       /* 39: Subtract floats */
#define OP_DIVF       ((uint8_t)_OP_DIVF)       /* 3A: Divide floats */
#define OP_MULF       ((uint8_t)_OP_MULF)       /* 3B: Multiply floats */
#define OP_CVIF       ((uint8_t)_OP_CVIF)       /* 3C: Convert to integer from float */
#define OP_CVFI       ((uint8_t)_OP_CVFI)       /* 3D: Convert to float from integer */
#define OP_CONSTU1    ((uint8_t)_OP_CONSTU1)    /* 3E: */
#define OP_CONSTI1    ((uint8_t)_OP_CONSTI1)    /* 3F: */
#define OP_CONSTU2    ((uint8_t)_OP_CONSTU2)    /* 40: */
#define OP_CONSTI2    ((uint8_t)_OP_CONSTI2)    /* 41: */
#define OP_CONSTU4    ((uint8_t)_OP_CONSTU4)    /* 42: */
#define OP_CONSTI4    ((uint8_t)_OP_CONSTI4)    /* 43: */
#define OP_CONSTF4    ((uint8_t)_OP_CONSTF4)    /* 44: */
#define OP_CONSTP4    ((uint8_t)_OP_CONSTP4)    /* 45: */
#define OP_LOAD3      ((uint8_t)_OP_LOAD3)      /* 46: Load 3-bytes from memory */
#define OP_ADD3       ((uint8_t)_OP_ADD3)
#define OP_BAND3      ((uint8_t)_OP_BAND3)
#define OP_BCOM3      ((uint8_t)_OP_BCOM3)
#define OP_BOR3       ((uint8_t)_OP_BOR3)
#define OP_BXOR3      ((uint8_t)_OP_BXOR3)
#define OP_CONSTI3    ((uint8_t)_OP_CONSTI3)
#define OP_CONSTU3    ((uint8_t)_OP_CONSTU3)
#define OP_CVFI3      ((uint8_t)_OP_CVFI3)
#define OP_CVIU3      ((uint8_t)_OP_CVIU3)
#define OP_CVUI3      ((uint8_t)_OP_CVUI3)
#define OP_DIVI3      ((uint8_t)_OP_DIVI3)
#define OP_DIVU3      ((uint8_t)_OP_DIVU3)
#define OP_EQ3        ((uint8_t)_OP_EQ3)
#define OP_GEI3       ((uint8_t)_OP_GEI3)
#define OP_GEU3       ((uint8_t)_OP_GEU3)
#define OP_GTI3       ((uint8_t)_OP_GTI3)
#define OP_GTU3       ((uint8_t)_OP_GTU3)
#define OP_LEI3       ((uint8_t)_OP_LEI3)
#define OP_LEU3       ((uint8_t)_OP_LEU3)
#define OP_LSH3       ((uint8_t)_OP_LSH3)
#define OP_LTI3       ((uint8_t)_OP_LTI3)
#define OP_LTU3       ((uint8_t)_OP_LTU3)
#define OP_MODI3      ((uint8_t)_OP_MODI3)
#define OP_MODU3      ((uint8_t)_OP_MODU3)
#define OP_MULI3      ((uint8_t)_OP_MULI3)
#define OP_MULU3      ((uint8_t)_OP_MULU3)
#define OP_NE3        ((uint8_t)_OP_NE3)
#define OP_NEGI3      ((uint8_t)_OP_NEGI3)
#define OP_RSHI3      ((uint8_t)_OP_RSHI3)
#define OP_RSHU3      ((uint8_t)_OP_RSHU3)
#define OP_SEX24      ((uint8_t)_OP_SEX24)
#define OP_SEX8_3     ((uint8_t)_OP_SEX8_3)
#define OP_STORE3     ((uint8_t)_OP_STORE3)
#define OP_SUB3       ((uint8_t)_OP_SUB3)
#define OP_CONSTGP3   ((uint8_t)_OP_CONSTGP3)
#define OP_CONSTP3    ((uint8_t)_OP_CONSTP3)
#define OP_ARGF       ((uint8_t)_OP_ARGF)
#define OP_ARG4       ((uint8_t)_OP_ARG4)
#define OP_MAX        ((uint8_t)_OP_MAX) /* ??: Make this the last item */

extern const char *const opnames[OPCODE_TABLE_SIZE];

#endif
