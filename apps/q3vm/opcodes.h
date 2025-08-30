#ifndef __OPCODES
#define __OPCODES

#include <stdint.h>

/** Enum for the virtual machine op codes */
typedef enum {
  OP_UNDEF,      /*   0  Error: VM halt */
  OP_IGNORE,     /*   1  No operation */
  OP_BREAK,      /*   2  vm->breakCount++ */
  OP_ADD3,       /*   3  Add int24 (two's complement) */
  OP_ADD4,       /*   4  Add int32 (two's complement) */
  OP_ADDF4,      /*   5  Add floats */
  OP_ARG3,       /*   6  Marshal 3 byte argument */
  OP_ARG4,       /*   7  Marshal 4 byte argument */
  OP_BAND3,      /*   8  Bitwise AND int24 */
  OP_BAND4,      /*   9  Bitwise AND int32 */
  OP_BCOM3,      /*  10  Bitwise Complement int24 */
  OP_BCOM4,      /*  11  Bitwise COMplement int32 */
  OP_BLK_CPY,    /*  12  memcpy */
  OP_BLK_CPY_U1, /*  13  */
  OP_BLK_CPY_U2, /*  14  */
  OP_BOR3,       /*  15  Bitwise OR int24 */
  OP_BOR4,       /*  16  Bitwise OR int32 */
  OP_BXOR3,      /*  17  Bitwise eXclusive-OR int24 */
  OP_BXOR4,      /*  18  Bitwise eXclusive-OR int32 */
  OP_CALL,       /*  19  Call subroutine. */
  OP_CF4I3,      /*  20  Convert float to int24 */
  OP_CF4I4,      /*  21  Convert float to int32 */
  OP_CI1I3,      /*  22  Convert int8 to int24 */
  OP_CI2I3,      /*  23  Convert int16 to int24 */
  OP_CI3F4,      /*  24  Convert int24 to float */
  OP_CI3s4,      /*  25  Convert int24 to (u)int32 */
  OP_CI4F4,      /*  26  Convert int32 to float */
  OP_CONSTI3_I1, /*  27  */
  OP_CONSTI3_I2, /*  28  */
  OP_CONSTP3,    /*  29  Push address pointer to stack */
  OP_CONSTs1,    /*  30  Push 1 byte constant */
  OP_CONSTs2,    /*  31  Push 2 byte constant */
  OP_CONSTs3,    /*  32  Push 3 byte constant */
  OP_CONSTs4,    /*  33  Push 4 byte constant */
  OP_CONSTU3_U1, /*  34  */
  OP_CONSTU3_U2, /*  35  */
  OP_CU1I3,      /*  36  Convert uint8 to int24 */
  OP_CU2I3,      /*  37  Convert uint16 to int24 */
  OP_CU3U4,      /*  38  Convert uint24 to uint32 */
  OP_DI,         /*  39  Disable Interrupts */
  OP_DIVF4,      /*  40  Divide floats */
  OP_DIVI3,      /*  41  Divide int24 */
  OP_DIVI4,      /*  42  Divide int32 */
  OP_DIVU3,      /*  43  Divide uint24 */
  OP_DIVU4,      /*  44  Divide uint32 */
  OP_EI,         /*  45  Enable Interrupts */
  OP_ENTER,      /*  46  Begin subroutine */
  OP_EQ3,        /*  47  Compare (u)int24, jump if == */
  OP_EQ4,        /*  48  Compare (u)int32, jump if == */
  OP_GEF4,       /*  49  Compare floats, jump if >= */
  OP_GEI3,       /*  50  Compare int24, jump if > */
  OP_GEI4,       /*  51  Compare int32, jump if >= */
  OP_GEU3,       /*  52  Compare uint24, jump if >= */
  OP_GEU4,       /*  53  Compare uint24, jump if >= */
  OP_GTF4,       /*  54  Compare floats, jump if > */
  OP_GTI3,       /*  55  Compare int24, jump if > */
  OP_GTI4,       /*  56  Compare int32, jump if > */
  OP_GTU3,       /*  57  Compare uint24, jump if > */
  OP_GTU4,       /*  58  Compare uint32, jump if > */
  OP_JUMP,       /*  59  Unconditional jump */
  OP_LEAVE,      /*  60  End subroutine */
  OP_LEF4,       /*  61  Compare floats, jump if <= */
  OP_LEI3,       /*  62  Compare int24, jump if <= */
  OP_LEI4,       /*  63  Compare int32, jump if <= */
  OP_LEU3,       /*  64  Compare uint24, jump if <= */
  OP_LEU4,       /*  65  Compare uint32, jump if <= */
  OP_LOAD1,      /*  66  Load 1-byte from memory */
  OP_LOAD2,      /*  67  Load 2-bytes from memory */
  OP_LOAD3,      /*  68  Load 3-bytes from memory */
  OP_LOAD4,      /*  69  Load 4-bytes from memory */
  OP_LOCAL,      /*  70  Get local variable (8 bit offset) */
  OP_LOCAL_FAR,  /*  71  Get local variable (16 bit offset) */
  OP_LSH3,       /*  72  left shift (u)int24 */
  OP_LSH4,       /*  73  Left-shift (u)int32 */
  OP_LTF4,       /*  74  Compare floats, jump if < */
  OP_LTI3,       /*  75  Compare int24, jump if < */
  OP_LTI4,       /*  76  Compare int32, jump if < */
  OP_LTU3,       /*  77  Compare uint24, jump if < */
  OP_LTU4,       /*  78  Compare uint32, jump if < */
  OP_MODI3,      /*  79  Modulus int24 */
  OP_MODI4,      /*  80  Modulus int32 */
  OP_MODU3,      /*  81  Modulus uint24 */
  OP_MODU4,      /*  82  Modulus uint32 */
  OP_MULF4,      /*  83  Multiply floats */
  OP_MULI3,      /*  84  Multiple int24 */
  OP_MULI4,      /*  85  Multiply int32 */
  OP_MULU3,      /*  86  Multiply uint24 */
  OP_MULU4,      /*  87  Multiply uint32 */
  OP_NE3,        /*  88  Compare (u)int24, jump if != */
  OP_NE4,        /*  89  Compare integers, jump if != */
  OP_NEF4,       /*  90  Compare floats, jump if != */
  OP_NEGF4,      /*  91  Negate float */
  OP_NEGI3,      /*  92  Negate int24 */
  OP_NEGI4,      /*  93  Negate int32 */
  OP_POP,        /*  94  Discard top-of-stack */
  OP_PUSH,       /*  95  Push no-value to stack */
  OP_RSHI3,      /*  96  Right-shift int24 (preserve sign) */
  OP_RSHI4,      /*  97  Right-shift int32 (preserve sign) */
  OP_RSHU3,      /*  98  Right-shift uint24 */
  OP_RSHU4,      /*  99  Right-shift uint32 */
  OP_STORE1,     /* 100  Store 1-byte to memory */
  OP_STORE2,     /* 101  Store 2-byte to memory */
  OP_STORE3,     /* 102  Store 3-byte to memory */
  OP_STORE4,     /* 103  Store 4-byte to memory */
  OP_SUB3,       /* 104  Subtract (u)int24 (two's complement). */
  OP_SUB4,       /* 105  Subtract (u)int32 (two's complement). */
  OP_SUBF4,      /* 106  Subtract floats */
  OP_MAX,        /* Make this the last item */

} opcode_t;

#define OPCODE_TABLE_SIZE OP_MAX

extern const char *const opnames[OPCODE_TABLE_SIZE];

#endif
