#include "opcodes.h"

const char *const opnames[OPCODE_TABLE_SIZE] = {
    "UNDEF",        /*   0  Error: VM halt */
    "IGNORE",       /*   1  No operation */
    "ADD3",         /*   2  Add int24 (two's complement) */
    "ADD4",         /*   3  Add int32 (two's complement) */
    "ADDF4",        /*   4  Add floats */
    "ARG3",         /*   5  Marshal 3 byte argument */
    "ARG4",         /*   6  Marshal 4 byte argument */
    "BAND3",        /*   7  Bitwise AND int24 */
    "BAND4",        /*   8  Bitwise AND int32 */
    "BCOM3",        /*   9  Bitwise Complement int24 */
    "BCOM4",        /*  10  Bitwise COMplement int32 */
    "BLK_CPY",      /*  11  memcpy */
    "BLK_CPY_U1",   /*  12  */
    "BLK_CPY_U2",   /*  13  */
    "BOR3",         /*  14  Bitwise OR int24 */
    "BOR4",         /*  15  Bitwise OR int32 */
    "BREAK",        /*  16  vm->breakCount++ */
    "BXOR3",        /*  17  Bitwise eXclusive-OR int24 */
    "BXOR4",        /*  18  Bitwise eXclusive-OR int32 */
    "CALL",         /*  19  Call subroutine. */
    "CF4I3",        /*  20  Convert float to int24 */
    "CF4I4",        /*  21  Convert float to int32 */
    "CI1I3",        /*  22  Convert int8 to int24 */
    "CI2I3",        /*  23  Convert int16 to int24 */
    "CI3F4",        /*  24  Convert int24 to float */
    "CI3s4",        /*  25  Convert int24 to (u)int32 */
    "CI4F4",        /*  26  Convert int32 to float */
    "CONSTI3_I1",   /*  27  */
    "CONSTI3_I2",   /*  28  */
    "CONSTP3",      /*  29  Push address pointer to stack */
    "CONSTP3_1",    /*  30  */
    "CONSTP3_2",    /*  31  */
    "CONSTP3_SC1",  /*  32  */
    "CONSTP3_SC2",  /*  33  */
    "CONSTP3_SC3",  /*  34  */
    "CONSTP3_SC4",  /*  35  */
    "CONSTP3_SC5",  /*  36  */
    "CONSTP3_SC6",  /*  37  */
    "CONSTP3_SC7",  /*  38  */
    "CONSTP3_SC8",  /*  39  */
    "CONSTP3_SC9",  /*  40  */
    "CONSTP3_SC10", /*  41  */
    "CONSTP3_SC11", /*  42  */
    "CONSTP3_SC12", /*  43  */
    "CONSTP3_SC13", /*  44  */
    "CONSTP3_SC14", /*  45  */
    "CONSTP3_SC15", /*  46  */
    "CONSTP3_SCn",  /*  47  */
    "CONSTs1",      /*  48  Push 1 byte constant */
    "CONSTs2",      /*  49  Push 2 byte constant */
    "CONSTs3",      /*  50  Push 3 byte constant */
    "CONSTs4",      /*  51  Push 4 byte constant */
    "CONSTU3_U1",   /*  52  */
    "CONSTU3_U2",   /*  53  */
    "CU1I3",        /*  54  Convert uint8 to int24 */
    "CU2I3",        /*  55  Convert uint16 to int24 */
    "CU3U4",        /*  56  Convert uint24 to uint32 */
    "DI",           /*  57  Disable Interrupts */
    "DIVF4",        /*  58  Divide floats */
    "DIVI3",        /*  59  Divide int24 */
    "DIVI4",        /*  60  Divide int32 */
    "DIVU3",        /*  61  Divide uint24 */
    "DIVU4",        /*  62  Divide uint32 */
    "EI",           /*  63  Enable Interrupts */
    "ENTER",        /*  64  Begin subroutine */
    "EQ3",          /*  65  Compare (u)int24, jump if == */
    "EQ3_1",        /*  66  Compare (u)int24, jump if == */
    "EQ4",          /*  67  Compare (u)int32, jump if == */
    "GEF4",         /*  68  Compare floats, jump if >= */
    "GEI3",         /*  69  Compare int24, jump if > */
    "GEI3_1",       /*  70  */
    "GEI4",         /*  71  Compare int32, jump if >= */
    "GEU3",         /*  72  Compare uint24, jump if >= */
    "GEU3_1",       /*  73  */
    "GEU4",         /*  74  Compare uint24, jump if >= */
    "GTF4",         /*  75  Compare floats, jump if > */
    "GTI3",         /*  76  Compare int24, jump if > */
    "GTI3_1",       /*  77  */
    "GTI4",         /*  78  Compare int32, jump if > */
    "GTU3",         /*  79  Compare uint24, jump if > */
    "GTU3_1",       /*  80  */
    "GTU4",         /*  81  Compare uint32, jump if > */
    "JUMP",         /*  82  Unconditional jump */
    "LEAVE",        /*  83  End subroutine */
    "LEF4",         /*  84  Compare floats, jump if <= */
    "LEI3",         /*  85  Compare int24, jump if <= */
    "LEI3_1",       /*  86  */
    "LEI4",         /*  87  Compare int32, jump if <= */
    "LEU3",         /*  88  Compare uint24, jump if <= */
    "LEU3_1",       /*  89  */
    "LEU4",         /*  90  Compare uint32, jump if <= */
    "LOAD1",        /*  91  Load 1-byte from memory */
    "LOAD2",        /*  92  Load 2-bytes from memory */
    "LOAD3",        /*  93  Load 3-bytes from memory */
    "LOAD4",        /*  94  Load 4-bytes from memory */
    "LOCAL",        /*  95  Get local variable (8 bit offset) */
    "LOCAL_FAR",    /*  96  Get local variable (16 bit offset) */
    "LSH3",         /*  97  left shift (u)int24 */
    "LSH4",         /*  98  Left-shift (u)int32 */
    "LTF4",         /*  99  Compare floats, jump if < */
    "LTI3",         /* 100  Compare int24, jump if < */
    "LTI3_1",       /* 101  */
    "LTI4",         /* 102  Compare int32, jump if < */
    "LTU3",         /* 103  Compare uint24, jump if < */
    "LTU3_1",       /* 104  */
    "LTU4",         /* 105  Compare uint32, jump if < */
    "MODI3",        /* 106  Modulus int24 */
    "MODI4",        /* 107  Modulus int32 */
    "MODU3",        /* 108  Modulus uint24 */
    "MODU4",        /* 109  Modulus uint32 */
    "MULF4",        /* 110  Multiply floats */
    "MULI3",        /* 111  Multiple int24 */
    "MULI4",        /* 112  Multiply int32 */
    "MULU3",        /* 113  Multiply uint24 */
    "MULU4",        /* 114  Multiply uint32 */
    "NE3",          /* 115  Compare (u)int24, jump if != */
    "NE3_1",        /* 116  */
    "NE4",          /* 117  Compare integers, jump if != */
    "NEF4",         /* 118  Compare floats, jump if != */
    "NEGF4",        /* 119  Negate float */
    "NEGI3",        /* 120  Negate int24 */
    "NEGI4",        /* 121  Negate int32 */
    "POP",          /* 122  Discard top-of-stack */
    "PUSH",         /* 123  Push no-value to stack */
    "RSHI3",        /* 124  Right-shift int24 (preserve sign) */
    "RSHI4",        /* 125  Right-shift int32 (preserve sign) */
    "RSHU3",        /* 126  Right-shift uint24 */
    "RSHU4",        /* 127  Right-shift uint32 */
    "STORE1",       /* 128  Store 1-byte to memory */
    "STORE2",       /* 129  Store 2-byte to memory */
    "STORE3",       /* 130  Store 3-byte to memory */
    "STORE4",       /* 131  Store 4-byte to memory */
    "SUB3",         /* 132  Subtract (u)int24 (two's complement). */
    "SUB4",         /* 133  Subtract (u)int32 (two's complement). */
    "SUBF4",        /* 134  Subtract floats */
};
