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
    "EQ3_1",      /*  48  Compare (u)int24, jump if == */
    "EQ4",        /*  49  Compare (u)int32, jump if == */
    "GEF4",       /*  50  Compare floats, jump if >= */
    "GEI3",       /*  51  Compare int24, jump if > */
    "GEI3_1",     /*  52  */
    "GEI4",       /*  53  Compare int32, jump if >= */
    "GEU3",       /*  54  Compare uint24, jump if >= */
    "GEU3_1",     /*  55  */
    "GEU4",       /*  56  Compare uint24, jump if >= */
    "GTF4",       /*  57  Compare floats, jump if > */
    "GTI3",       /*  58  Compare int24, jump if > */
    "GTI3_1",     /*  59  */
    "GTI4",       /*  60  Compare int32, jump if > */
    "GTU3",       /*  61  Compare uint24, jump if > */
    "GTU3_1",     /*  62  */
    "GTU4",       /*  63  Compare uint32, jump if > */
    "JUMP",       /*  64  Unconditional jump */
    "LEAVE",      /*  65  End subroutine */
    "LEF4",       /*  66  Compare floats, jump if <= */
    "LEI3",       /*  67  Compare int24, jump if <= */
    "LEI3_1",     /*  68  */
    "LEI4",       /*  69  Compare int32, jump if <= */
    "LEU3",       /*  70  Compare uint24, jump if <= */
    "LEU3_1",     /*  71  */
    "LEU4",       /*  72  Compare uint32, jump if <= */
    "LOAD1",      /*  73  Load 1-byte from memory */
    "LOAD2",      /*  74  Load 2-bytes from memory */
    "LOAD3",      /*  75  Load 3-bytes from memory */
    "LOAD4",      /*  76  Load 4-bytes from memory */
    "LOCAL",      /*  77  Get local variable (8 bit offset) */
    "LOCAL_FAR",  /*  78  Get local variable (16 bit offset) */
    "LSH3",       /*  79  left shift (u)int24 */
    "LSH4",       /*  80  Left-shift (u)int32 */
    "LTF4",       /*  81  Compare floats, jump if < */
    "LTI3",       /*  82  Compare int24, jump if < */
    "LTI3_1",     /*  83  */
    "LTI4",       /*  84  Compare int32, jump if < */
    "LTU3",       /*  85  Compare uint24, jump if < */
    "LTU3_1",     /*  86  */
    "LTU4",       /*  87  Compare uint32, jump if < */
    "MODI3",      /*  88  Modulus int24 */
    "MODI4",      /*  89  Modulus int32 */
    "MODU3",      /*  90  Modulus uint24 */
    "MODU4",      /*  91  Modulus uint32 */
    "MULF4",      /*  92  Multiply floats */
    "MULI3",      /*  93  Multiple int24 */
    "MULI4",      /*  94  Multiply int32 */
    "MULU3",      /*  95  Multiply uint24 */
    "MULU4",      /*  96  Multiply uint32 */
    "NE3",        /*  97  Compare (u)int24, jump if != */
    "NE3_1",      /*  98  */
    "NE4",        /*  99  Compare integers, jump if != */
    "NEF4",       /* 100  Compare floats, jump if != */
    "NEGF4",      /* 101  Negate float */
    "NEGI3",      /* 102  Negate int24 */
    "NEGI4",      /* 103  Negate int32 */
    "POP",        /* 104  Discard top-of-stack */
    "PUSH",       /* 105  Push no-value to stack */
    "RSHI3",      /* 106  Right-shift int24 (preserve sign) */
    "RSHI4",      /* 107  Right-shift int32 (preserve sign) */
    "RSHU3",      /* 108  Right-shift uint24 */
    "RSHU4",      /* 109  Right-shift uint32 */
    "STORE1",     /* 110  Store 1-byte to memory */
    "STORE2",     /* 111  Store 2-byte to memory */
    "STORE3",     /* 112  Store 3-byte to memory */
    "STORE4",     /* 113  Store 4-byte to memory */
    "SUB3",       /* 114  Subtract (u)int24 (two's complement). */
    "SUB4",       /* 115  Subtract (u)int32 (two's complement). */
    "SUBF4",      /* 116  Subtract floats */
};
