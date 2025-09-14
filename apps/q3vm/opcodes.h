#ifndef __OPCODES
#define __OPCODES

#include <stdint.h>

/** Enum for the virtual machine op codes */
typedef enum {
  OP_UNDEF,          /*   0  Error: VM halt */
  OP_IGNORE,         /*   1  No operation */
  OP_ADD3,           /*   2  Add int24 (two's complement) */
  OP_ADD4,           /*   3  Add int32 (two's complement) */
  OP_ADDF4,          /*   4  Add floats */
  OP_ARG3,           /*   5  Marshal 3 byte argument */
  OP_ARG4,           /*   6  Marshal 4 byte argument */
  OP_BAND3,          /*   7  Bitwise AND int24 */
  OP_BAND4,          /*   8  Bitwise AND int32 */
  OP_BCOM3,          /*   9  Bitwise Complement int24 */
  OP_BCOM4,          /*  10  Bitwise COMplement int32 */
  OP_BLK_CPY,        /*  11  memcpy */
  OP_BLK_CPY_U1,     /*  12  */
  OP_BLK_CPY_U2,     /*  13  */
  OP_BOR3,           /*  14  Bitwise OR int24 */
  OP_BOR4,           /*  15  Bitwise OR int32 */
  OP_BREAK,          /*  16  vm->breakCount++ */
  OP_BXOR3,          /*  17  Bitwise eXclusive-OR int24 */
  OP_BXOR4,          /*  18  Bitwise eXclusive-OR int32 */
  OP_CALL,           /*  19  Call subroutine. */
  OP_CF4I3,          /*  20  Convert float to int24 */
  OP_CF4I4,          /*  21  Convert float to int32 */
  OP_CI1I3,          /*  22  Convert int8 to int24 */
  OP_CI2I3,          /*  23  Convert int16 to int24 */
  OP_CI3F4,          /*  24  Convert int24 to float */
  OP_CI3s4,          /*  25  Convert int24 to (u)int32 */
  OP_CI4F4,          /*  26  Convert int32 to float */
  OP_CONSTI3_I1,     /*  27  */
  OP_CONSTI3_I2,     /*  28  */
  OP_CONSTP3,        /*  29  Push address pointer to stack */
  OP_CONSTP3_1,      /*  30  */
  OP_CONSTP3_2,      /*  31  */
  OP_CONSTP3_FD,     /*  32  */
  OP_CONSTP3_FFFFF1, /*  33  */
  OP_CONSTP3_FFFFF2, /*  34  */
  OP_CONSTP3_FFFFF3, /*  35  */
  OP_CONSTP3_FFFFF4, /*  36  */
  OP_CONSTP3_FFFFF5, /*  37  */
  OP_CONSTP3_FFFFF6, /*  38  */
  OP_CONSTP3_FFFFF7, /*  39  */
  OP_CONSTP3_FFFFF8, /*  40  */
  OP_CONSTP3_FFFFF9, /*  41  */
  OP_CONSTP3_FFFFFA, /*  42  */
  OP_CONSTP3_FFFFFB, /*  43  */
  OP_CONSTP3_FFFFFC, /*  44  */
  OP_CONSTP3_FFFFFD, /*  45  */
  OP_CONSTP3_FFFFFE, /*  46  */
  OP_CONSTP3_FFFFFF, /*  47  */
  OP_CONSTP3_FFFFnn, /*  48  */
  OP_CONSTP3_NULL,   /*  49  */
  OP_CONSTs1,        /*  50  Push 1 byte constant */
  OP_CONSTs2,        /*  51  Push 2 byte constant */
  OP_CONSTs3,        /*  52  Push 3 byte constant */
  OP_CONSTs4,        /*  53  Push 4 byte constant */
  OP_CONSTU3_U1,     /*  54  */
  OP_CONSTU3_U2,     /*  55  */
  OP_CU1I3,          /*  56  Convert uint8 to int24 */
  OP_CU2I3,          /*  57  Convert uint16 to int24 */
  OP_CU3U4,          /*  58  Convert uint24 to uint32 */
  OP_DI,             /*  59  Disable Interrupts */
  OP_DIVF4,          /*  60  Divide floats */
  OP_DIVI3,          /*  61  Divide int24 */
  OP_DIVI4,          /*  62  Divide int32 */
  OP_DIVU3,          /*  63  Divide uint24 */
  OP_DIVU4,          /*  64  Divide uint32 */
  OP_EI,             /*  65  Enable Interrupts */
  OP_ENTER,          /*  66  Begin subroutine */
  OP_EQ3,            /*  67  Compare (u)int24, jump if == */
  OP_EQ3_1,          /*  68  Compare (u)int24, jump if == */
  OP_EQ4,            /*  69  Compare (u)int32, jump if == */
  OP_GEF4,           /*  70  Compare floats, jump if >= */
  OP_GEI3,           /*  71  Compare int24, jump if > */
  OP_GEI3_1,         /*  72  */
  OP_GEI4,           /*  73  Compare int32, jump if >= */
  OP_GEU3,           /*  74  Compare uint24, jump if >= */
  OP_GEU3_1,         /*  75  */
  OP_GEU4,           /*  76  Compare uint24, jump if >= */
  OP_GTF4,           /*  77  Compare floats, jump if > */
  OP_GTI3,           /*  78  Compare int24, jump if > */
  OP_GTI3_1,         /*  79  */
  OP_GTI4,           /*  80  Compare int32, jump if > */
  OP_GTU3,           /*  81  Compare uint24, jump if > */
  OP_GTU3_1,         /*  82  */
  OP_GTU4,           /*  83  Compare uint32, jump if > */
  OP_JUMP,           /*  84  Unconditional jump */
  OP_LEAVE,          /*  85  End subroutine */
  OP_LEF4,           /*  86  Compare floats, jump if <= */
  OP_LEI3,           /*  87  Compare int24, jump if <= */
  OP_LEI3_1,         /*  88  */
  OP_LEI4,           /*  89  Compare int32, jump if <= */
  OP_LEU3,           /*  90  Compare uint24, jump if <= */
  OP_LEU3_1,         /*  91  */
  OP_LEU4,           /*  92  Compare uint32, jump if <= */
  OP_LOAD1,          /*  93  Load 1-byte from memory */
  OP_LOAD2,          /*  94  Load 2-bytes from memory */
  OP_LOAD3,          /*  95  Load 3-bytes from memory */
  OP_LOAD4,          /*  96  Load 4-bytes from memory */
  OP_LOCAL,          /*  97  Get local variable (8 bit offset) */
  OP_LOCAL_FAR,      /*  98  Get local variable (16 bit offset) */
  OP_LSH3,           /*  99  left shift (u)int24 */
  OP_LSH4,           /* 100  Left-shift (u)int32 */
  OP_LTF4,           /* 101  Compare floats, jump if < */
  OP_LTI3,           /* 102  Compare int24, jump if < */
  OP_LTI3_1,         /* 103  */
  OP_LTI4,           /* 104  Compare int32, jump if < */
  OP_LTU3,           /* 105  Compare uint24, jump if < */
  OP_LTU3_1,         /* 106  */
  OP_LTU4,           /* 107  Compare uint32, jump if < */
  OP_MODI3,          /* 108  Modulus int24 */
  OP_MODI4,          /* 109  Modulus int32 */
  OP_MODU3,          /* 110  Modulus uint24 */
  OP_MODU4,          /* 111  Modulus uint32 */
  OP_MULF4,          /* 112  Multiply floats */
  OP_MULI3,          /* 113  Multiple int24 */
  OP_MULI4,          /* 114  Multiply int32 */
  OP_MULU3,          /* 115  Multiply uint24 */
  OP_MULU4,          /* 116  Multiply uint32 */
  OP_NE3,            /* 117  Compare (u)int24, jump if != */
  OP_NE3_1,          /* 118  */
  OP_NE4,            /* 119  Compare integers, jump if != */
  OP_NEF4,           /* 120  Compare floats, jump if != */
  OP_NEGF4,          /* 121  Negate float */
  OP_NEGI3,          /* 122  Negate int24 */
  OP_NEGI4,          /* 123  Negate int32 */
  OP_POP,            /* 124  Discard top-of-stack */
  OP_PUSH,           /* 125  Push no-value to stack */
  OP_RSHI3,          /* 126  Right-shift int24 (preserve sign) */
  OP_RSHI4,          /* 127  Right-shift int32 (preserve sign) */
  OP_RSHU3,          /* 128  Right-shift uint24 */
  OP_RSHU4,          /* 129  Right-shift uint32 */
  OP_STORE1,         /* 130  Store 1-byte to memory */
  OP_STORE2,         /* 131  Store 2-byte to memory */
  OP_STORE3,         /* 132  Store 3-byte to memory */
  OP_STORE4,         /* 133  Store 4-byte to memory */
  OP_SUB3,           /* 134  Subtract (u)int24 (two's complement). */
  OP_SUB4,           /* 135  Subtract (u)int32 (two's complement). */
  OP_SUBF4,          /* 136  Subtract floats */
  OP_MAX,            /* Make this the last item */

} opcode_t;

#define OPCODE_TABLE_SIZE OP_MAX

extern const char *const opnames[OPCODE_TABLE_SIZE];

#endif
