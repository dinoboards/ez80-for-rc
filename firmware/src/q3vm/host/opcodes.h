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
  OP_CI3I1,      /*  24  */
  OP_CI3I2,      /*  25  */
  OP_CI3I4,      /*  26  */
  OP_CI3U4,      /*  27  */
  OP_CI4F4,      /*  28  Convert to integer from float */
  OP_CI4I3,      /*  29  */
  OP_CI4U3,      /*  30  */
  OP_CONSTF4,    /*  31  */
  OP_CONSTGP3,   /*  32  */
  OP_CONSTGP4,   /*  33  Load constant to stack. */
  OP_CONSTI1,    /*  34  */
  OP_CONSTI2,    /*  35  */
  OP_CONSTI3,    /*  36  */
  OP_CONSTI4,    /*  37  */
  OP_CONSTP3,    /*  38  */
  OP_CONSTP4,    /*  39  */
  OP_CONSTU1,    /*  40  */
  OP_CONSTU2,    /*  41  */
  OP_CONSTU3,    /*  42  */
  OP_CONSTU4,    /*  43  */
  OP_CU2I3,      /*  44  */
  OP_CU3U2,      /*  45  */
  OP_CU3U4,      /*  46  */
  OP_CU4I3,      /*  47  */
  OP_CU4U3,      /*  48  */
  OP_CVFI3,      /*  49  */
  OP_CVIU3,      /*  50  */
  OP_CVUI3,      /*  51  */
  OP_DIVF4,      /*  52  Divide floats */
  OP_DIVI,       /*  53  Divide signed integers. */
  OP_DIVI3,      /*  54  */
  OP_DIVU,       /*  55  Divide unsigned integers. */
  OP_DIVU3,      /*  56  */
  OP_ENTER,      /*  57  Begin subroutine. */
  OP_EQ3,        /*  58  */
  OP_EQ4,        /*  59  Compare integers, jump if equal. */
  OP_EQF4,       /*  60  Compare floats, jump if equal */
  OP_EQU3,       /*  61  */
  OP_GEF4,       /*  62  Compare floats, jump if greater-than-or-equal */
  OP_GEI3,       /*  63  */
  OP_GEI4,       /*  64  Compare integers, jump if greater-than-or-equal. */
  OP_GEU3,       /*  65  */
  OP_GEU4,       /*  66  Compare unsigned integers, jump if greater-than-or-equal */
  OP_GTF4,       /*  67  Compare floats, jump if greater-than */
  OP_GTI3,       /*  68  */
  OP_GTI4,       /*  69  Compare integers, jump if greater-than. */
  OP_GTU3,       /*  70  */
  OP_GTU4,       /*  71  Compare unsigned integers, jump if greater-than */
  OP_JUMP,       /*  72  Unconditional jump. */
  OP_LEAVE,      /*  73  End subroutine. */
  OP_LEF,        /*  74  Compare floats, jump if less-than-or-equal */
  OP_LEI3,       /*  75  */
  OP_LEI4,       /*  76  Compare integers, jump if less-than-or-equal. */
  OP_LEU,        /*  77  Compare unsigned integers, jump if less-than-or-equal */
  OP_LEU3,       /*  78  */
  OP_LOAD1,      /*  79  Load 1-byte from memory */
  OP_LOAD2,      /*  80  Load 2-bytes from memory */
  OP_LOAD3,      /*  81  Load 3-bytes from memory */
  OP_LOAD4,      /*  82  Load 4-bytes from memory */
  OP_LOADF4,     /*  83  Load 4 byte float from memory */
  OP_LOCAL,      /*  84  Get local variable. */
  OP_LSH3,       /*  85  */
  OP_LSH4,       /*  86  Left-shift */
  OP_LTF,        /*  87  Compare floats, jump if less-than */
  OP_LTI3,       /*  88  */
  OP_LTI4,       /*  89  Compare integers, jump if less-than. */
  OP_LTU,        /*  90  Compare unsigned integers, jump if less-than */
  OP_LTU3,       /*  91  */
  OP_MODI3,      /*  92  */
  OP_MODI4,      /*  93  Modulus (signed). */
  OP_MODU3,      /*  94  */
  OP_MODU4,      /*  95  Modulus (unsigned). */
  OP_MULF4,      /*  96  Multiply floats */
  OP_MULI3,      /*  97  */
  OP_MULI4,      /*  98  Multiply signed integers. */
  OP_MULU3,      /*  99  */
  OP_MULU4,      /* 100  Multiply unsigned integers. */
  OP_NE3,        /* 101  */
  OP_NE4,        /* 102  Compare integers, jump if not equal. */
  OP_NEF,        /* 103  Compare floats, jump if not-equal */
  OP_NEGF4,      /* 104  Negate float */
  OP_NEGI3,      /* 105  */
  OP_NEGI4,      /* 106  Negate integer. */
  OP_POP,        /* 107  Discard top-of-stack. */
  OP_PUSH,       /* 108  Push to stack. */
  OP_RSHI3,      /* 109  */
  OP_RSHI4,      /* 110  Right-shift (algebraic; preserve sign) */
  OP_RSHU3,      /* 111  */
  OP_RSHU4,      /* 112  Right-shift (bitwise; ignore sign) */
  OP_SEX24,      /* 113  */
  OP_SEX8_3,     /* 114  */
  OP_STORE1,     /* 115  Store 1-byte to memory */
  OP_STORE2,     /* 116  Store 2-byte to memory */
  OP_STORE3,     /* 117  */
  OP_STORE4,     /* 118  *(stack[top-1]) = stack[top] */
  OP_SUB3,       /* 119  */
  OP_SUB4,       /* 120  Subtract integers (two's complement). */
  OP_SUBF4,      /* 121  Subtract floats */
  OP_MAX,        /* Make this the last item */

} opcode_t;

#define OPCODE_TABLE_SIZE OP_MAX

extern const char *const opnames[OPCODE_TABLE_SIZE];

#endif
