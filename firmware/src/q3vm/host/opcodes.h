#ifndef __OPCODES
#define __OPCODES

#include <stdint.h>

/** Enum for the virtual machine op codes */
typedef enum {
  OP_UNDEF,        /*   0  Error: VM halt */
  OP_IGNORE,       /*   1  No operation */
  OP_ADD3,         /*   2  Add int24 (two's complement) */
  OP_ADD4,         /*   3  Add int32 (two's complement) */
  OP_ADDF4,        /*   4  Add floats */
  OP_ARG3,         /*   5  Marshal 3 byte argument */
  OP_ARG4,         /*   6  Marshal 4 byte argument */
  OP_BAND3,        /*   7  Bitwise AND int24 */
  OP_BAND4,        /*   8  Bitwise AND int32 */
  OP_BCOM3,        /*   9  Bitwise Complement int24 */
  OP_BCOM4,        /*  10  Bitwise COMplement int32 */
  OP_BLK_CPY,      /*  11  memcpy */
  OP_BLK_CPY_U1,   /*  12  */
  OP_BLK_CPY_U2,   /*  13  */
  OP_BOR3,         /*  14  Bitwise OR int24 */
  OP_BOR4,         /*  15  Bitwise OR int32 */
  OP_BREAK,        /*  16  vm->breakCount++ */
  OP_BXOR3,        /*  17  Bitwise eXclusive-OR int24 */
  OP_BXOR4,        /*  18  Bitwise eXclusive-OR int32 */
  OP_CALL,         /*  19  Call subroutine. */
  OP_CF4I3,        /*  20  Convert float to int24 */
  OP_CF4I4,        /*  21  Convert float to int32 */
  OP_CI1I3,        /*  22  Convert int8 to int24 */
  OP_CI2I3,        /*  23  Convert int16 to int24 */
  OP_CI3F4,        /*  24  Convert int24 to float */
  OP_CI3s4,        /*  25  Convert int24 to (u)int32 */
  OP_CI4F4,        /*  26  Convert int32 to float */
  OP_CONSTI3_I1,   /*  27  */
  OP_CONSTI3_I2,   /*  28  */
  OP_CONSTP3,      /*  29  Push address pointer to stack */
  OP_CONSTP3_1,    /*  30  */
  OP_CONSTP3_2,    /*  31  */
  OP_CONSTP3_SC1,  /*  32  */
  OP_CONSTP3_SC2,  /*  33  */
  OP_CONSTP3_SC3,  /*  34  */
  OP_CONSTP3_SC4,  /*  35  */
  OP_CONSTP3_SC5,  /*  36  */
  OP_CONSTP3_SC6,  /*  37  */
  OP_CONSTP3_SC7,  /*  38  */
  OP_CONSTP3_SC8,  /*  39  */
  OP_CONSTP3_SC9,  /*  40  */
  OP_CONSTP3_SC10, /*  41  */
  OP_CONSTP3_SC11, /*  42  */
  OP_CONSTP3_SC12, /*  43  */
  OP_CONSTP3_SC13, /*  44  */
  OP_CONSTP3_SC14, /*  45  */
  OP_CONSTP3_SC15, /*  46  */
  OP_CONSTP3_SCn,  /*  47  */
  OP_CONSTs1,      /*  48  Push 1 byte constant */
  OP_CONSTs2,      /*  49  Push 2 byte constant */
  OP_CONSTs3,      /*  50  Push 3 byte constant */
  OP_CONSTs4,      /*  51  Push 4 byte constant */
  OP_CONSTU3_U1,   /*  52  */
  OP_CONSTU3_U2,   /*  53  */
  OP_CU1I3,        /*  54  Convert uint8 to int24 */
  OP_CU2I3,        /*  55  Convert uint16 to int24 */
  OP_CU3U4,        /*  56  Convert uint24 to uint32 */
  OP_DI,           /*  57  Disable Interrupts */
  OP_DIVF4,        /*  58  Divide floats */
  OP_DIVI3,        /*  59  Divide int24 */
  OP_DIVI4,        /*  60  Divide int32 */
  OP_DIVU3,        /*  61  Divide uint24 */
  OP_DIVU4,        /*  62  Divide uint32 */
  OP_EI,           /*  63  Enable Interrupts */
  OP_ENTER,        /*  64  Begin subroutine */
  OP_EQ3,          /*  65  Compare (u)int24, jump if == */
  OP_EQ3_1,        /*  66  Compare (u)int24, jump if == */
  OP_EQ4,          /*  67  Compare (u)int32, jump if == */
  OP_GEF4,         /*  68  Compare floats, jump if >= */
  OP_GEI3,         /*  69  Compare int24, jump if > */
  OP_GEI3_1,       /*  70  */
  OP_GEI4,         /*  71  Compare int32, jump if >= */
  OP_GEU3,         /*  72  Compare uint24, jump if >= */
  OP_GEU3_1,       /*  73  */
  OP_GEU4,         /*  74  Compare uint24, jump if >= */
  OP_GTF4,         /*  75  Compare floats, jump if > */
  OP_GTI3,         /*  76  Compare int24, jump if > */
  OP_GTI3_1,       /*  77  */
  OP_GTI4,         /*  78  Compare int32, jump if > */
  OP_GTU3,         /*  79  Compare uint24, jump if > */
  OP_GTU3_1,       /*  80  */
  OP_GTU4,         /*  81  Compare uint32, jump if > */
  OP_JUMP,         /*  82  Unconditional jump */
  OP_LEAVE,        /*  83  End subroutine */
  OP_LEF4,         /*  84  Compare floats, jump if <= */
  OP_LEI3,         /*  85  Compare int24, jump if <= */
  OP_LEI3_1,       /*  86  */
  OP_LEI4,         /*  87  Compare int32, jump if <= */
  OP_LEU3,         /*  88  Compare uint24, jump if <= */
  OP_LEU3_1,       /*  89  */
  OP_LEU4,         /*  90  Compare uint32, jump if <= */
  OP_LOAD1,        /*  91  Load 1-byte from memory */
  OP_LOAD2,        /*  92  Load 2-bytes from memory */
  OP_LOAD3,        /*  93  Load 3-bytes from memory */
  OP_LOAD4,        /*  94  Load 4-bytes from memory */
  OP_LOCAL,        /*  95  Get local variable (8 bit offset) */
  OP_LOCAL_FAR,    /*  96  Get local variable (16 bit offset) */
  OP_LSH3,         /*  97  left shift (u)int24 */
  OP_LSH4,         /*  98  Left-shift (u)int32 */
  OP_LTF4,         /*  99  Compare floats, jump if < */
  OP_LTI3,         /* 100  Compare int24, jump if < */
  OP_LTI3_1,       /* 101  */
  OP_LTI4,         /* 102  Compare int32, jump if < */
  OP_LTU3,         /* 103  Compare uint24, jump if < */
  OP_LTU3_1,       /* 104  */
  OP_LTU4,         /* 105  Compare uint32, jump if < */
  OP_MODI3,        /* 106  Modulus int24 */
  OP_MODI4,        /* 107  Modulus int32 */
  OP_MODU3,        /* 108  Modulus uint24 */
  OP_MODU4,        /* 109  Modulus uint32 */
  OP_MULF4,        /* 110  Multiply floats */
  OP_MULI3,        /* 111  Multiple int24 */
  OP_MULI4,        /* 112  Multiply int32 */
  OP_MULU3,        /* 113  Multiply uint24 */
  OP_MULU4,        /* 114  Multiply uint32 */
  OP_NE3,          /* 115  Compare (u)int24, jump if != */
  OP_NE3_1,        /* 116  */
  OP_NE4,          /* 117  Compare integers, jump if != */
  OP_NEF4,         /* 118  Compare floats, jump if != */
  OP_NEGF4,        /* 119  Negate float */
  OP_NEGI3,        /* 120  Negate int24 */
  OP_NEGI4,        /* 121  Negate int32 */
  OP_POP,          /* 122  Discard top-of-stack */
  OP_PUSH,         /* 123  Push no-value to stack */
  OP_RSHI3,        /* 124  Right-shift int24 (preserve sign) */
  OP_RSHI4,        /* 125  Right-shift int32 (preserve sign) */
  OP_RSHU3,        /* 126  Right-shift uint24 */
  OP_RSHU4,        /* 127  Right-shift uint32 */
  OP_STORE1,       /* 128  Store 1-byte to memory */
  OP_STORE2,       /* 129  Store 2-byte to memory */
  OP_STORE3,       /* 130  Store 3-byte to memory */
  OP_STORE4,       /* 131  Store 4-byte to memory */
  OP_SUB3,         /* 132  Subtract (u)int24 (two's complement). */
  OP_SUB4,         /* 133  Subtract (u)int32 (two's complement). */
  OP_SUBF4,        /* 134  Subtract floats */
  OP_MAX,          /* Make this the last item */

} opcode_t;

#define OPCODE_TABLE_SIZE OP_MAX

extern const char *const opnames[OPCODE_TABLE_SIZE];

#endif
