#include "opcodes.h"

const char *const opnames[OPCODE_TABLE_SIZE] = {
    "UNDEF",      /*   0  Error: VM halt */
    "IGNORE",     /*   1  No operation */
    "BREAK",      /*   2  vm->breakCount++ */
    "ADD3",       /*   3  Add int24 (two's complement) */
    "ADD4",       /*   4  Add int32 (two's complement) */
    "ADDF4",      /*   5  Add floats */
    "ARG3",       /*   6  Marshal 3 byte argument */
    "ARG4",       /*   7  Marshal 4 byte argument */
    "BAND3",      /*   8  Bitwise AND int24 */
    "BAND4",      /*   9  Bitwise AND int32 */
    "BCOM3",      /*  10  Bitwise Complement int24 */
    "BCOM4",      /*  11  Bitwise COMplement int32 */
    "BLK_CPY",    /*  12  memcpy */
    "BLK_CPY_U1", /*  13  */
    "BLK_CPY_U2", /*  14  */
    "BOR3",       /*  15  Bitwise OR int24 */
    "BOR4",       /*  16  Bitwise OR int32 */
    "BXOR3",      /*  17  Bitwise eXclusive-OR int24 */
    "BXOR4",      /*  18  Bitwise eXclusive-OR int32 */
    "CALL",       /*  19  Call subroutine. */
    "CF4I3",      /*  20  Convert float to int24 */
    "CF4I4",      /*  21  Convert float to int32 */
    "CI1I3",      /*  22  Convert int8 to int24 */
    "CI2I3",      /*  23  Convert int16 to int24 */
    "CI3F4",      /*  24  Convert int24 to float */
    "CI3s4",      /*  25  Convert int24 to (u)int32 */
    "CI4F4",      /*  26  Convert int32 to float */
    "CONSTI3_I1", /*  27  */
    "CONSTI3_I2", /*  28  */
    "CONSTP3",    /*  29  Push address pointer to stack */
    "CONSTs1",    /*  30  Push 1 byte constant */
    "CONSTs2",    /*  31  Push 2 byte constant */
    "CONSTs3",    /*  32  Push 3 byte constant */
    "CONSTs4",    /*  33  Push 4 byte constant */
    "CONSTU3_U1", /*  34  */
    "CONSTU3_U2", /*  35  */
    "CU1I3",      /*  36  Convert uint8 to int24 */
    "CU2I3",      /*  37  Convert uint16 to int24 */
    "CU3U4",      /*  38  Convert uint24 to uint32 */
    "DI",         /*  39  Disable Interrupts */
    "DIVF4",      /*  40  Divide floats */
    "DIVI3",      /*  41  Divide int24 */
    "DIVI4",      /*  42  Divide int32 */
    "DIVU3",      /*  43  Divide uint24 */
    "DIVU4",      /*  44  Divide uint32 */
    "EI",         /*  45  Enable Interrupts */
    "ENTER",      /*  46  Begin subroutine */
    "EQ3",        /*  47  Compare (u)int24, jump if == */
    "EQ4",        /*  48  Compare (u)int32, jump if == */
    "GEF4",       /*  49  Compare floats, jump if >= */
    "GEI3",       /*  50  Compare int24, jump if > */
    "GEI4",       /*  51  Compare int32, jump if >= */
    "GEU3",       /*  52  Compare uint24, jump if >= */
    "GEU4",       /*  53  Compare uint24, jump if >= */
    "GTF4",       /*  54  Compare floats, jump if > */
    "GTI3",       /*  55  Compare int24, jump if > */
    "GTI4",       /*  56  Compare int32, jump if > */
    "GTU3",       /*  57  Compare uint24, jump if > */
    "GTU4",       /*  58  Compare uint32, jump if > */
    "JUMP",       /*  59  Unconditional jump */
    "LEAVE",      /*  60  End subroutine */
    "LEF4",       /*  61  Compare floats, jump if <= */
    "LEI3",       /*  62  Compare int24, jump if <= */
    "LEI4",       /*  63  Compare int32, jump if <= */
    "LEU3",       /*  64  Compare uint24, jump if <= */
    "LEU4",       /*  65  Compare uint32, jump if <= */
    "LOAD1",      /*  66  Load 1-byte from memory */
    "LOAD2",      /*  67  Load 2-bytes from memory */
    "LOAD3",      /*  68  Load 3-bytes from memory */
    "LOAD4",      /*  69  Load 4-bytes from memory */
    "LOCAL",      /*  70  Get local variable (8 bit offset) */
    "LOCAL_FAR",  /*  71  Get local variable (16 bit offset) */
    "LSH3",       /*  72  left shift (u)int24 */
    "LSH4",       /*  73  Left-shift (u)int32 */
    "LTF4",       /*  74  Compare floats, jump if < */
    "LTI3",       /*  75  Compare int24, jump if < */
    "LTI4",       /*  76  Compare int32, jump if < */
    "LTU3",       /*  77  Compare uint24, jump if < */
    "LTU4",       /*  78  Compare uint32, jump if < */
    "MODI3",      /*  79  Modulus int24 */
    "MODI4",      /*  80  Modulus int32 */
    "MODU3",      /*  81  Modulus uint24 */
    "MODU4",      /*  82  Modulus uint32 */
    "MULF4",      /*  83  Multiply floats */
    "MULI3",      /*  84  Multiple int24 */
    "MULI4",      /*  85  Multiply int32 */
    "MULU3",      /*  86  Multiply uint24 */
    "MULU4",      /*  87  Multiply uint32 */
    "NE3",        /*  88  Compare (u)int24, jump if != */
    "NE4",        /*  89  Compare integers, jump if != */
    "NEF4",       /*  90  Compare floats, jump if != */
    "NEGF4",      /*  91  Negate float */
    "NEGI3",      /*  92  Negate int24 */
    "NEGI4",      /*  93  Negate int32 */
    "POP",        /*  94  Discard top-of-stack */
    "PUSH",       /*  95  Push no-value to stack */
    "RSHI3",      /*  96  Right-shift int24 (preserve sign) */
    "RSHI4",      /*  97  Right-shift int32 (preserve sign) */
    "RSHU3",      /*  98  Right-shift uint24 */
    "RSHU4",      /*  99  Right-shift uint32 */
    "STORE1",     /* 100  Store 1-byte to memory */
    "STORE2",     /* 101  Store 2-byte to memory */
    "STORE3",     /* 102  Store 3-byte to memory */
    "STORE4",     /* 103  Store 4-byte to memory */
    "SUB3",       /* 104  Subtract (u)int24 (two's complement). */
    "SUB4",       /* 105  Subtract (u)int32 (two's complement). */
    "SUBF4",      /* 106  Subtract floats */
};
