#ifndef __OPCODES
#define __OPCODES

#include <stdint.h>

/** Enum for the virtual machine op codes */
typedef enum {
  OP_UNDEF,      /*   0  Error: VM halt */
  OP_IGNORE,     /*   1  No operation */
  OP_BREAK,      /*   2  vm->breakCount++ */
  OP_ADD3,       /*   3  */
  OP_ADD4,       /*   4  Add integers (two's complement). */
  OP_ADDF4,      /*   5  Add floats */
  OP_ARG3,       /*   6  Marshal argument */
  OP_ARG4,       /*   7  */
  OP_ARGF4,      /*   8  */
  OP_BAND3,      /*   9  */
  OP_BAND4,      /*  10  Bitwise AND */
  OP_BCOM3,      /*  11  */
  OP_BCOM4,      /*  12  Bitwise COMplement */
  OP_BLOCK_COPY, /*  13  memcpy */
  OP_BOR3,       /*  14  */
  OP_BOR4,       /*  15  Bitwise OR */
  OP_BXOR3,      /*  16  */
  OP_BXOR4,      /*  17  Bitwise eXclusive-OR */
  OP_CALL,       /*  18  Call subroutine. */
  OP_CF4I4,      /*  19  Convert to float from integer */
  OP_CI1I3,      /*  20  */
  OP_CI1I4,      /*  21  Sign-Extend 8-bit */
  OP_CI2I3,      /*  22  */
  OP_CI2I4,      /*  23  Sign-Extend 16-bit */
  OP_CI3F4,      /*  24  */
  OP_CI3I1,      /*  25  */
  OP_CI3I2,      /*  26  */
  OP_CI3I4,      /*  27  */
  OP_CI3U4,      /*  28  */
  OP_CI4F4,      /*  29  Convert to integer from float */
  OP_CI4I3,      /*  30  */
  OP_CI4U3,      /*  31  */
  OP_CONSTGP3,   /*  32  push address pointer to stack. */
  OP_CONSTI1,    /*  33  */
  OP_CONSTI2,    /*  34  */
  OP_CONSTI3,    /*  35  */
  OP_CONSTI4,    /*  36  */
  OP_CONSTP3,    /*  37  */
  OP_CONSTU1,    /*  38  */
  OP_CONSTU2,    /*  39  */
  OP_CONSTU3,    /*  40  */
  OP_CONSTU4,    /*  41  */
  OP_CU1I3,      /*  42  */
  OP_CU2I3,      /*  43  */
  OP_CU3U2,      /*  44  */
  OP_CU3U4,      /*  45  */
  OP_CU4I3,      /*  46  */
  OP_CU4U3,      /*  47  */
  OP_CVFI3,      /*  48  */
  OP_CVIU3,      /*  49  */
  OP_CVUI3,      /*  50  */
  OP_DIVF4,      /*  51  Divide floats */
  OP_DIVI,       /*  52  Divide signed integers. */
  OP_DIVI3,      /*  53  */
  OP_DIVU,       /*  54  Divide unsigned integers. */
  OP_DIVU3,      /*  55  */
  OP_ENTER,      /*  56  Begin subroutine. */
  OP_EQ3,        /*  57  */
  OP_EQ4,        /*  58  Compare integers, jump if equal. */
  OP_EQF4,       /*  59  Compare floats, jump if equal */
  OP_EQU3,       /*  60  */
  OP_GEF4,       /*  61  Compare floats, jump if greater-than-or-equal */
  OP_GEI3,       /*  62  */
  OP_GEI4,       /*  63  Compare integers, jump if greater-than-or-equal. */
  OP_GEU3,       /*  64  */
  OP_GEU4,       /*  65  Compare unsigned integers, jump if greater-than-or-equal */
  OP_GTF4,       /*  66  Compare floats, jump if greater-than */
  OP_GTI3,       /*  67  */
  OP_GTI4,       /*  68  Compare integers, jump if greater-than. */
  OP_GTU3,       /*  69  */
  OP_GTU4,       /*  70  Compare unsigned integers, jump if greater-than */
  OP_JUMP,       /*  71  Unconditional jump. */
  OP_LEAVE,      /*  72  End subroutine. */
  OP_LEF4,       /*  73  Compare floats, jump if less-than-or-equal */
  OP_LEI3,       /*  74  */
  OP_LEI4,       /*  75  Compare integers, jump if less-than-or-equal. */
  OP_LEU,        /*  76  Compare unsigned integers, jump if less-than-or-equal */
  OP_LEU3,       /*  77  */
  OP_LOAD1,      /*  78  Load 1-byte from memory */
  OP_LOAD2,      /*  79  Load 2-bytes from memory */
  OP_LOAD3,      /*  80  Load 3-bytes from memory */
  OP_LOAD4,      /*  81  Load 4-bytes from memory */
  OP_LOADF4,     /*  82  Load 4 byte float from memory */
  OP_LOCAL,      /*  83  Get local variable. */
  OP_LSH3,       /*  84  */
  OP_LSH4,       /*  85  Left-shift */
  OP_LTF,        /*  86  Compare floats, jump if less-than */
  OP_LTI3,       /*  87  */
  OP_LTI4,       /*  88  Compare integers, jump if less-than. */
  OP_LTU,        /*  89  Compare unsigned integers, jump if less-than */
  OP_LTU3,       /*  90  */
  OP_MODI3,      /*  91  */
  OP_MODI4,      /*  92  Modulus (signed). */
  OP_MODU3,      /*  93  */
  OP_MODU4,      /*  94  Modulus (unsigned). */
  OP_MULF4,      /*  95  Multiply floats */
  OP_MULI3,      /*  96  */
  OP_MULI4,      /*  97  Multiply signed integers. */
  OP_MULU3,      /*  98  */
  OP_MULU4,      /*  99  Multiply unsigned integers. */
  OP_NE3,        /* 100  */
  OP_NE4,        /* 101  Compare integers, jump if not equal. */
  OP_NEF,        /* 102  Compare floats, jump if not-equal */
  OP_NEGF4,      /* 103  Negate float */
  OP_NEGI3,      /* 104  */
  OP_NEGI4,      /* 105  Negate integer. */
  OP_POP,        /* 106  Discard top-of-stack. */
  OP_PUSH,       /* 107  Push to stack. */
  OP_RSHI3,      /* 108  */
  OP_RSHI4,      /* 109  Right-shift (algebraic; preserve sign) */
  OP_RSHU3,      /* 110  */
  OP_RSHU4,      /* 111  Right-shift (bitwise; ignore sign) */
  OP_STORE1,     /* 112  Store 1-byte to memory */
  OP_STORE2,     /* 113  Store 2-byte to memory */
  OP_STORE3,     /* 114  */
  OP_STORE4,     /* 115  *(stack[top-1]) = stack[top] */
  OP_SUB3,       /* 116  */
  OP_SUB4,       /* 117  Subtract integers (two's complement). */
  OP_SUBF4,      /* 118  Subtract floats */
  OP_MAX,        /* Make this the last item */

} opcode_t;

#define OPCODE_TABLE_SIZE OP_MAX

extern const char *const opnames[OPCODE_TABLE_SIZE];

#endif
