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
  OP_BLOCK_COPY, /*  12  memcpy */
  OP_BOR3,       /*  13  Bitwise OR int24 */
  OP_BOR4,       /*  14  Bitwise OR int32 */
  OP_BXOR3,      /*  15  Bitwise eXclusive-OR int24 */
  OP_BXOR4,      /*  16  Bitwise eXclusive-OR int32 */
  OP_CALL,       /*  17  Call subroutine. */
  OP_CF4I3,      /*  18  Convert float to int24 */
  OP_CF4I4,      /*  19  Convert float to int32 */
  OP_CI1I3,      /*  20  Convert int8 to int24 */
  OP_CI2I3,      /*  21  Convert int16 to int24 */
  OP_CI3F4,      /*  22  Convert int24 to float */
  OP_CI3s4,      /*  23  Convert int24 to (u)int32 */
  OP_CI4F4,      /*  24  Convert int32 to float */
  OP_CONSTP3,    /*  25  Push address pointer to stack */
  OP_CONSTs1,    /*  26  Push 1 byte constant */
  OP_CONSTs2,    /*  27  Push 2 byte constant */
  OP_CONSTs3,    /*  28  Push 3 byte constant */
  OP_CONSTs4,    /*  29  Push 4 byte constant */
  OP_CU1I3,      /*  30  Convert uint8 to int24 */
  OP_CU2I3,      /*  31  Convert uint16 to int24 */
  OP_CU3U4,      /*  32  Convert uint24 to uint32 */
  OP_DIVF4,      /*  33  Divide floats */
  OP_DIVI3,      /*  34  Divide int24 */
  OP_DIVI4,      /*  35  Divide int32 */
  OP_DIVU3,      /*  36  Divide uint24 */
  OP_DIVU4,      /*  37  Divide uint32 */
  OP_ENTER,      /*  38  Begin subroutine */
  OP_EQ3,        /*  39  Compare (u)int24, jump if == */
  OP_EQ4,        /*  40  Compare (u)int32, jump if == */
  OP_GEF4,       /*  41  Compare floats, jump if >= */
  OP_GEI3,       /*  42  Compare int24, jump if > */
  OP_GEI4,       /*  43  Compare int32, jump if >= */
  OP_GEU3,       /*  44  Compare uint24, jump if >= */
  OP_GEU4,       /*  45  Compare uint24, jump if >= */
  OP_GTF4,       /*  46  Compare floats, jump if > */
  OP_GTI3,       /*  47  Compare int24, jump if > */
  OP_GTI4,       /*  48  Compare int32, jump if > */
  OP_GTU3,       /*  49  Compare uint24, jump if > */
  OP_GTU4,       /*  50  Compare uint32, jump if > */
  OP_JUMP,       /*  51  Unconditional jump */
  OP_LEAVE,      /*  52  End subroutine */
  OP_LEF4,       /*  53  Compare floats, jump if <= */
  OP_LEI3,       /*  54  Compare int24, jump if <= */
  OP_LEI4,       /*  55  Compare int32, jump if <= */
  OP_LEU3,       /*  56  Compare uint24, jump if <= */
  OP_LEU4,       /*  57  Compare uint32, jump if <= */
  OP_LOAD1,      /*  58  Load 1-byte from memory */
  OP_LOAD2,      /*  59  Load 2-bytes from memory */
  OP_LOAD3,      /*  60  Load 3-bytes from memory */
  OP_LOAD4,      /*  61  Load 4-bytes from memory */
  OP_LOCAL,      /*  62  Get local variable */
  OP_LSH3,       /*  63  left shift (u)int24 */
  OP_LSH4,       /*  64  Left-shift (u)int32 */
  OP_LTF4,       /*  65  Compare floats, jump if < */
  OP_LTI3,       /*  66  Compare int24, jump if < */
  OP_LTI4,       /*  67  Compare int32, jump if < */
  OP_LTU3,       /*  68  Compare uint24, jump if < */
  OP_LTU4,       /*  69  Compare uint32, jump if < */
  OP_MODI3,      /*  70  Modulus int24 */
  OP_MODI4,      /*  71  Modulus int32 */
  OP_MODU3,      /*  72  Modulus uint24 */
  OP_MODU4,      /*  73  Modulus uint32 */
  OP_MULF4,      /*  74  Multiply floats */
  OP_MULI3,      /*  75  Multiple int24 */
  OP_MULI4,      /*  76  Multiply int32 */
  OP_MULU3,      /*  77  Multiply uint24 */
  OP_MULU4,      /*  78  Multiply uint32 */
  OP_NE3,        /*  79  Compare (u)int24, jump if != */
  OP_NE4,        /*  80  Compare integers, jump if != */
  OP_NEF4,       /*  81  Compare floats, jump if != */
  OP_NEGF4,      /*  82  Negate float */
  OP_NEGI3,      /*  83  Negate int24 */
  OP_NEGI4,      /*  84  Negate int32 */
  OP_POP,        /*  85  Discard top-of-stack */
  OP_PUSH,       /*  86  Push no-value to stack */
  OP_RSHI3,      /*  87  Right-shift int24 (preserve sign) */
  OP_RSHI4,      /*  88  Right-shift int32 (preserve sign) */
  OP_RSHU3,      /*  89  Right-shift uint24 */
  OP_RSHU4,      /*  90  Right-shift uint32 */
  OP_STORE1,     /*  91  Store 1-byte to memory */
  OP_STORE2,     /*  92  Store 2-byte to memory */
  OP_STORE3,     /*  93  Store 3-byte to memory */
  OP_STORE4,     /*  94  Store 4-byte to memory */
  OP_SUB3,       /*  95  Subtract (u)int24 (two's complement). */
  OP_SUB4,       /*  96  Subtract (u)int32 (two's complement). */
  OP_SUBF4,      /*  97  Subtract floats */
  OP_MAX,        /* Make this the last item */

} opcode_t;

#define OPCODE_TABLE_SIZE OP_MAX

extern const char *const opnames[OPCODE_TABLE_SIZE];

#endif
