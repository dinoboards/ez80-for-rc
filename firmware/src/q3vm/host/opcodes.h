#ifndef __OPCODES
#define __OPCODES

#include <stdint.h>

/** Enum for the virtual machine op codes */
typedef enum {
  OP_UNDEF,      /*   1  Error: VM halt */
  OP_IGNORE,     /*   2  No operation */
  OP_BREAK,      /*   3  vm->breakCount++ */
  OP_ADD,        /*   4  Add integers (two's complement). */
  OP_ADD3,       /*   5  */
  OP_ADDF,       /*   6  Add floats */
  OP_ARG,        /*   7  Marshal argument */
  OP_ARG4,       /*   8  */
  OP_ARGF,       /*   9  */
  OP_BAND,       /*  10  Bitwise AND */
  OP_BAND3,      /*  11  */
  OP_BCOM,       /*  12  Bitwise COMplement */
  OP_BCOM3,      /*  13  */
  OP_BLOCK_COPY, /*  14  memcpy */
  OP_BOR,        /*  15  Bitwise OR */
  OP_BOR3,       /*  16  */
  OP_BXOR,       /*  17  Bitwise eXclusive-OR */
  OP_BXOR3,      /*  18  */
  OP_CALL,       /*  19  Call subroutine. */
  OP_CI1I3,      /*  20  */
  OP_CI1I4,      /*  21  Sign-Extend 8-bit */
  OP_CI2I3,      /*  22  */
  OP_CI2I4,      /*  23  Sign-Extend 16-bit */
  OP_CI3I1,      /*  24  */
  OP_CI3I2,      /*  25  */
  OP_CI3I4,      /*  26  */
  OP_CI4I3,      /*  27  */
  OP_CONSTF4,    /*  28  */
  OP_CONSTGP3,   /*  29  */
  OP_CONSTGP4,   /*  30  Load constant to stack. */
  OP_CONSTI1,    /*  31  */
  OP_CONSTI2,    /*  32  */
  OP_CONSTI3,    /*  33  */
  OP_CONSTI4,    /*  34  */
  OP_CONSTP3,    /*  35  */
  OP_CONSTP4,    /*  36  */
  OP_CONSTU1,    /*  37  */
  OP_CONSTU2,    /*  38  */
  OP_CONSTU3,    /*  39  */
  OP_CONSTU4,    /*  40  */
  OP_CU2I3,      /*  41  */
  OP_CU3U2,      /*  42  */
  OP_CVFI,       /*  43  Convert to float from integer */
  OP_CVFI3,      /*  44  */
  OP_CVIF,       /*  45  Convert to integer from float */
  OP_CVIU3,      /*  46  */
  OP_CVUI3,      /*  47  */
  OP_DIVF,       /*  48  Divide floats */
  OP_DIVI,       /*  49  Divide signed integers. */
  OP_DIVI3,      /*  50  */
  OP_DIVU,       /*  51  Divide unsigned integers. */
  OP_DIVU3,      /*  52  */
  OP_ENTER,      /*  53  Begin subroutine. */
  OP_EQ,         /*  54  Compare integers, jump if equal. */
  OP_EQ3,        /*  55  */
  OP_EQF,        /*  56  Compare floats, jump if equal */
  OP_EQU3,       /*  57  */
  OP_GEF,        /*  58  Compare floats, jump if greater-than-or-equal */
  OP_GEI,        /*  59  Compare integers, jump if greater-than-or-equal. */
  OP_GEI3,       /*  60  */
  OP_GEU,        /*  61  Compare unsigned integers, jump if greater-than-or-equal */
  OP_GEU3,       /*  62  */
  OP_GTF,        /*  63  Compare floats, jump if greater-than */
  OP_GTI,        /*  64  Compare integers, jump if greater-than. */
  OP_GTI3,       /*  65  */
  OP_GTU,        /*  66  Compare unsigned integers, jump if greater-than */
  OP_GTU3,       /*  67  */
  OP_JUMP,       /*  68  Unconditional jump. */
  OP_LEAVE,      /*  69  End subroutine. */
  OP_LEF,        /*  70  Compare floats, jump if less-than-or-equal */
  OP_LEI,        /*  71  Compare integers, jump if less-than-or-equal. */
  OP_LEI3,       /*  72  */
  OP_LEU,        /*  73  Compare unsigned integers, jump if less-than-or-equal */
  OP_LEU3,       /*  74  */
  OP_LOAD1,      /*  75  Load 1-byte from memory */
  OP_LOAD2,      /*  76  Load 2-bytes from memory */
  OP_LOAD3,      /*  77  Load 3-bytes from memory */
  OP_LOAD4,      /*  78  Load 4-bytes from memory */
  OP_LOADF4,     /*  79  Load 4 byte float from memory */
  OP_LOCAL,      /*  80  Get local variable. */
  OP_LSH,        /*  81  Left-shift */
  OP_LSH3,       /*  82  */
  OP_LTF,        /*  83  Compare floats, jump if less-than */
  OP_LTI,        /*  84  Compare integers, jump if less-than. */
  OP_LTI3,       /*  85  */
  OP_LTU,        /*  86  Compare unsigned integers, jump if less-than */
  OP_LTU3,       /*  87  */
  OP_MODI,       /*  88  Modulus (signed). */
  OP_MODI3,      /*  89  */
  OP_MODU,       /*  90  Modulus (unsigned). */
  OP_MODU3,      /*  91  */
  OP_MULF,       /*  92  Multiply floats */
  OP_MULI,       /*  93  Multiply signed integers. */
  OP_MULI3,      /*  94  */
  OP_MULU,       /*  95  Multiply unsigned integers. */
  OP_MULU3,      /*  96  */
  OP_NE,         /*  97  Compare integers, jump if not equal. */
  OP_NE3,        /*  98  */
  OP_NEF,        /*  99  Compare floats, jump if not-equal */
  OP_NEGF,       /* 100  Negate float */
  OP_NEGI,       /* 101  Negate integer. */
  OP_NEGI3,      /* 102  */
  OP_POP,        /* 103  Discard top-of-stack. */
  OP_PUSH,       /* 104  Push to stack. */
  OP_RSHI,       /* 105  Right-shift (algebraic; preserve sign) */
  OP_RSHI3,      /* 106  */
  OP_RSHU,       /* 107  Right-shift (bitwise; ignore sign) */
  OP_RSHU3,      /* 108  */
  OP_SEX24,      /* 109  */
  OP_SEX8_3,     /* 110  */
  OP_STORE1,     /* 111  Store 1-byte to memory */
  OP_STORE2,     /* 112  Store 2-byte to memory */
  OP_STORE3,     /* 113  */
  OP_STORE4,     /* 114  *(stack[top-1]) = stack[top] */
  OP_STOREF4,    /* 115  *(stack[top-1]) = stack[top] */
  OP_SUB,        /* 116  Subtract integers (two's complement). */
  OP_SUB3,       /* 117  */
  OP_SUBF,       /* 118  Subtract floats */
  OP_MAX,        /* Make this the last item */

} opcode_t;

#define OPCODE_TABLE_SIZE OP_MAX

extern const char *const opnames[OPCODE_TABLE_SIZE];

#endif
