#include "opcodes.h"

const char *const opnames[OPCODE_TABLE_SIZE] = {
    "UNDEF",          /*   0  Error: VM halt */
    "IGNORE",         /*   1  No operation */
    "ADD3",           /*   2  Add int24 (two's complement) */
    "ADD4",           /*   3  Add int32 (two's complement) */
    "ADDF4",          /*   4  Add floats */
    "ARG3",           /*   5  Marshal 3 byte argument */
    "ARG4",           /*   6  Marshal 4 byte argument */
    "BAND3",          /*   7  Bitwise AND int24 */
    "BAND4",          /*   8  Bitwise AND int32 */
    "BCOM3",          /*   9  Bitwise Complement int24 */
    "BCOM4",          /*  10  Bitwise COMplement int32 */
    "BLK_CPY",        /*  11  memcpy */
    "BLK_CPY_U1",     /*  12  */
    "BLK_CPY_U2",     /*  13  */
    "BOR3",           /*  14  Bitwise OR int24 */
    "BOR4",           /*  15  Bitwise OR int32 */
    "BREAK",          /*  16  vm->breakCount++ */
    "BXOR3",          /*  17  Bitwise eXclusive-OR int24 */
    "BXOR4",          /*  18  Bitwise eXclusive-OR int32 */
    "CALL",           /*  19  Call subroutine. */
    "CF4I3",          /*  20  Convert float to int24 */
    "CF4I4",          /*  21  Convert float to int32 */
    "CI1I3",          /*  22  Convert int8 to int24 */
    "CI2I3",          /*  23  Convert int16 to int24 */
    "CI3F4",          /*  24  Convert int24 to float */
    "CI3s4",          /*  25  Convert int24 to (u)int32 */
    "CI4F4",          /*  26  Convert int32 to float */
    "CONSTI3_I1",     /*  27  */
    "CONSTI3_I2",     /*  28  */
    "CONSTP3",        /*  29  Push address pointer to stack */
    "CONSTP3_1",      /*  30  */
    "CONSTP3_2",      /*  31  */
    "CONSTP3_FD",     /*  32  */
    "CONSTP3_FFFFF1", /*  33  */
    "CONSTP3_FFFFF2", /*  34  */
    "CONSTP3_FFFFF3", /*  35  */
    "CONSTP3_FFFFF4", /*  36  */
    "CONSTP3_FFFFF5", /*  37  */
    "CONSTP3_FFFFF6", /*  38  */
    "CONSTP3_FFFFF7", /*  39  */
    "CONSTP3_FFFFF8", /*  40  */
    "CONSTP3_FFFFF9", /*  41  */
    "CONSTP3_FFFFFA", /*  42  */
    "CONSTP3_FFFFFB", /*  43  */
    "CONSTP3_FFFFFC", /*  44  */
    "CONSTP3_FFFFFD", /*  45  */
    "CONSTP3_FFFFFE", /*  46  */
    "CONSTP3_FFFFFF", /*  47  */
    "CONSTP3_FFFFnn", /*  48  */
    "CONSTP3_NULL",   /*  49  */
    "CONSTs1",        /*  50  Push 1 byte constant */
    "CONSTs2",        /*  51  Push 2 byte constant */
    "CONSTs3",        /*  52  Push 3 byte constant */
    "CONSTs4",        /*  53  Push 4 byte constant */
    "CONSTU3_U1",     /*  54  */
    "CONSTU3_U2",     /*  55  */
    "CU1I3",          /*  56  Convert uint8 to int24 */
    "CU2I3",          /*  57  Convert uint16 to int24 */
    "CU3U4",          /*  58  Convert uint24 to uint32 */
    "DI",             /*  59  Disable Interrupts */
    "DIVF4",          /*  60  Divide floats */
    "DIVI3",          /*  61  Divide int24 */
    "DIVI4",          /*  62  Divide int32 */
    "DIVU3",          /*  63  Divide uint24 */
    "DIVU4",          /*  64  Divide uint32 */
    "EI",             /*  65  Enable Interrupts */
    "ENTER",          /*  66  Begin subroutine */
    "EQ3",            /*  67  Compare (u)int24, jump if == */
    "EQ3_1",          /*  68  Compare (u)int24, jump if == */
    "EQ4",            /*  69  Compare (u)int32, jump if == */
    "GEF4",           /*  70  Compare floats, jump if >= */
    "GEI3",           /*  71  Compare int24, jump if > */
    "GEI3_1",         /*  72  */
    "GEI4",           /*  73  Compare int32, jump if >= */
    "GEU3",           /*  74  Compare uint24, jump if >= */
    "GEU3_1",         /*  75  */
    "GEU4",           /*  76  Compare uint24, jump if >= */
    "GTF4",           /*  77  Compare floats, jump if > */
    "GTI3",           /*  78  Compare int24, jump if > */
    "GTI3_1",         /*  79  */
    "GTI4",           /*  80  Compare int32, jump if > */
    "GTU3",           /*  81  Compare uint24, jump if > */
    "GTU3_1",         /*  82  */
    "GTU4",           /*  83  Compare uint32, jump if > */
    "JUMP",           /*  84  Unconditional jump */
    "LEAVE",          /*  85  End subroutine */
    "LEF4",           /*  86  Compare floats, jump if <= */
    "LEI3",           /*  87  Compare int24, jump if <= */
    "LEI3_1",         /*  88  */
    "LEI4",           /*  89  Compare int32, jump if <= */
    "LEU3",           /*  90  Compare uint24, jump if <= */
    "LEU3_1",         /*  91  */
    "LEU4",           /*  92  Compare uint32, jump if <= */
    "LOAD1",          /*  93  Load 1-byte from memory */
    "LOAD2",          /*  94  Load 2-bytes from memory */
    "LOAD3",          /*  95  Load 3-bytes from memory */
    "LOAD4",          /*  96  Load 4-bytes from memory */
    "LOCAL",          /*  97  Get local variable (8 bit offset) */
    "LOCAL_FAR",      /*  98  Get local variable (16 bit offset) */
    "LSH3",           /*  99  left shift (u)int24 */
    "LSH4",           /* 100  Left-shift (u)int32 */
    "LTF4",           /* 101  Compare floats, jump if < */
    "LTI3",           /* 102  Compare int24, jump if < */
    "LTI3_1",         /* 103  */
    "LTI4",           /* 104  Compare int32, jump if < */
    "LTU3",           /* 105  Compare uint24, jump if < */
    "LTU3_1",         /* 106  */
    "LTU4",           /* 107  Compare uint32, jump if < */
    "MODI3",          /* 108  Modulus int24 */
    "MODI4",          /* 109  Modulus int32 */
    "MODU3",          /* 110  Modulus uint24 */
    "MODU4",          /* 111  Modulus uint32 */
    "MULF4",          /* 112  Multiply floats */
    "MULI3",          /* 113  Multiple int24 */
    "MULI4",          /* 114  Multiply int32 */
    "MULU3",          /* 115  Multiply uint24 */
    "MULU4",          /* 116  Multiply uint32 */
    "NE3",            /* 117  Compare (u)int24, jump if != */
    "NE3_1",          /* 118  */
    "NE4",            /* 119  Compare integers, jump if != */
    "NEF4",           /* 120  Compare floats, jump if != */
    "NEGF4",          /* 121  Negate float */
    "NEGI3",          /* 122  Negate int24 */
    "NEGI4",          /* 123  Negate int32 */
    "POP",            /* 124  Discard top-of-stack */
    "PUSH",           /* 125  Push no-value to stack */
    "RSHI3",          /* 126  Right-shift int24 (preserve sign) */
    "RSHI4",          /* 127  Right-shift int32 (preserve sign) */
    "RSHU3",          /* 128  Right-shift uint24 */
    "RSHU4",          /* 129  Right-shift uint32 */
    "STORE1",         /* 130  Store 1-byte to memory */
    "STORE2",         /* 131  Store 2-byte to memory */
    "STORE3",         /* 132  Store 3-byte to memory */
    "STORE4",         /* 133  Store 4-byte to memory */
    "SUB3",           /* 134  Subtract (u)int24 (two's complement). */
    "SUB4",           /* 135  Subtract (u)int32 (two's complement). */
    "SUBF4",          /* 136  Subtract floats */
};
