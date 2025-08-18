#ifndef __OPCODES
#define __OPCODES

#include <stdint.h>

/** Enum for the virtual machine op codes */
typedef enum {
  OP_UNDEF,      /* 00: Error: VM halt */
  OP_IGNORE,     /* 01: No operation */
  OP_BREAK,      /* 02: vm->breakCount++ */
  OP_ENTER,      /* 03: Begin subroutine. */
  OP_LEAVE,      /* 04: End subroutine. */
  OP_CALL,       /* 05: Call subroutine. */
  OP_PUSH,       /* 06: Push to stack. */
  OP_POP,        /* 07: Discard top-of-stack. */
  OP_CONSTGP4,   /* 08: Load constant to stack. */
  OP_LOCAL,      /* 09: Get local variable. */
  OP_JUMP,       /* 0A: Unconditional jump. */
  OP_EQ,         /* 0B: Compare integers, jump if equal. */
  OP_NE,         /* 0C: Compare integers, jump if not equal. */
  OP_LTI,        /* 0D: Compare integers, jump if less-than. */
  OP_LEI,        /* OE: Compare integers, jump if less-than-or-equal. */
  OP_GTI,        /* 0F: Compare integers, jump if greater-than. */
  OP_GEI,        /* 10: Compare integers, jump if greater-than-or-equal. */
  OP_LTU,        /* 11: Compare unsigned integers, jump if less-than */
  OP_LEU,        /* 12: Compare unsigned integers, jump if less-than-or-equal */
  OP_GTU,        /* 13: Compare unsigned integers, jump if greater-than */
  OP_GEU,        /* 14: Compare unsigned integers, jump if greater-than-or-equal */
  OP_EQF,        /* 15: Compare floats, jump if equal */
  OP_NEF,        /* 16: Compare floats, jump if not-equal */
  OP_LTF,        /* 17: Compare floats, jump if less-than */
  OP_LEF,        /* 18: Compare floats, jump if less-than-or-equal */
  OP_GTF,        /* 19: Compare floats, jump if greater-than */
  OP_GEF,        /* 1A: Compare floats, jump if greater-than-or-equal */
  OP_LOAD1,      /* 1B: Load 1-byte from memory */
  OP_LOAD2,      /* 1C: Load 2-bytes from memory */
  OP_LOAD4,      /* 1D: Load 4-bytes from memory */
  OP_LOADF4,     /* 1E: Load 4 byte float from memory */
  OP_STORE1,     /* 1F: Store 1-byte to memory */
  OP_STORE2,     /* 20: Store 2-byte to memory */
  OP_STORE4,     /* 21: *(stack[top-1]) = stack[top] */
  OP_STOREF4,    /* 22: *(stack[top-1]) = stack[top] */
  OP_ARG,        /* 23: Marshal argument */
  OP_BLOCK_COPY, /* 24: memcpy */
  OP_SEX8,       /* 25: Sign-Extend 8-bit */
  OP_SEX16,      /* 26: Sign-Extend 16-bit */
  OP_NEGI,       /* 27: Negate integer. */
  OP_ADD,        /* 28: Add integers (two's complement). */
  OP_SUB,        /* 29: Subtract integers (two's complement). */
  OP_DIVI,       /* 2A: Divide signed integers. */
  OP_DIVU,       /* 2B: Divide unsigned integers. */
  OP_MODI,       /* 2C: Modulus (signed). */
  OP_MODU,       /* 2D: Modulus (unsigned). */
  OP_MULI,       /* 2E: Multiply signed integers. */
  OP_MULU,       /* 2F: Multiply unsigned integers. */
  OP_BAND,       /* 30: Bitwise AND */
  OP_BOR,        /* 31: Bitwise OR */
  OP_BXOR,       /* 32: Bitwise eXclusive-OR */
  OP_BCOM,       /* 33: Bitwise COMplement */
  OP_LSH,        /* 34: Left-shift */
  OP_RSHI,       /* 35: Right-shift (algebraic; preserve sign) */
  OP_RSHU,       /* 36: Right-shift (bitwise; ignore sign) */
  OP_NEGF,       /* 37: Negate float */
  OP_ADDF,       /* 38: Add floats */
  OP_SUBF,       /* 39: Subtract floats */
  OP_DIVF,       /* 3A: Divide floats */
  OP_MULF,       /* 3B: Multiply floats */
  OP_CVIF,       /* 3C: Convert to integer from float */
  OP_CVFI,       /* 3D: Convert to float from integer */
  OP_CONSTU1,    /* 3E: */
  OP_CONSTI1,    /* 3F: */
  OP_CONSTU2,    /* 40: */
  OP_CONSTI2,    /* 41: */
  OP_CONSTU4,    /* 42: */
  OP_CONSTI4,    /* 43: */
  OP_CONSTF4,    /* 44: */
  OP_CONSTP4,    /* 45: */
  OP_LOAD3,      /* 46: Load 3-bytes from memory */
  OP_ADD3,
  OP_BAND3,
  OP_BCOM3,
  OP_BOR3,
  OP_BXOR3,
  OP_CONSTI3,
  OP_CONSTU3,
  OP_CVFI3,
  OP_CVIU3,
  OP_CVUI3,
  OP_DIVI3,
  OP_DIVU3,
  OP_EQ3,
  OP_GEI3,
  OP_GEU3,
  OP_GTI3,
  OP_GTU3,
  OP_LEI3,
  OP_LEU3,
  OP_LSH3,
  OP_LTI3,
  OP_LTU3,
  OP_MODI3,
  OP_MODU3,
  OP_MULI3,
  OP_MULU3,
  OP_NE3,
  OP_NEGI3,
  OP_RSHI3,
  OP_RSHU3,
  OP_SEX24,
  OP_SEX8_3,
  OP_STORE3,
  OP_SUB3,
  OP_CONSTGP3,
  OP_CONSTP3,
  OP_ARGF,
  OP_ARG4,

  OP_MAX /* ??: Make this the last item */
} opcode_t;

#define OPCODE_TABLE_SIZE OP_MAX

extern const char *const opnames[OPCODE_TABLE_SIZE];

#endif
