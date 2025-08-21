#include "opcodes.h"

const char *const opnames[OPCODE_TABLE_SIZE] = {
    "UNDEF",      /*   1  Error: VM halt */
    "IGNORE",     /*   2  No operation */
    "BREAK",      /*   3  vm->breakCount++ */
    "ADD",        /*   4  Add integers (two's complement). */
    "ADD3",       /*   5  */
    "ADDF",       /*   6  Add floats */
    "ARG",        /*   7  Marshal argument */
    "ARG4",       /*   8  */
    "ARGF",       /*   9  */
    "BAND",       /*  10  Bitwise AND */
    "BAND3",      /*  11  */
    "BCOM",       /*  12  Bitwise COMplement */
    "BCOM3",      /*  13  */
    "BLOCK_COPY", /*  14  memcpy */
    "BOR",        /*  15  Bitwise OR */
    "BOR3",       /*  16  */
    "BXOR",       /*  17  Bitwise eXclusive-OR */
    "BXOR3",      /*  18  */
    "CALL",       /*  19  Call subroutine. */
    "CI1I3",      /*  20  */
    "CI1I4",      /*  21  Sign-Extend 8-bit */
    "CI2I3",      /*  22  */
    "CI2I4",      /*  23  Sign-Extend 16-bit */
    "CI3I1",      /*  24  */
    "CI3I2",      /*  25  */
    "CI3I4",      /*  26  */
    "CI4I3",      /*  27  */
    "CONSTF4",    /*  28  */
    "CONSTGP3",   /*  29  */
    "CONSTGP4",   /*  30  Load constant to stack. */
    "CONSTI1",    /*  31  */
    "CONSTI2",    /*  32  */
    "CONSTI3",    /*  33  */
    "CONSTI4",    /*  34  */
    "CONSTP3",    /*  35  */
    "CONSTP4",    /*  36  */
    "CONSTU1",    /*  37  */
    "CONSTU2",    /*  38  */
    "CONSTU3",    /*  39  */
    "CONSTU4",    /*  40  */
    "CU2I3",      /*  41  */
    "CU3U2",      /*  42  */
    "CVFI",       /*  43  Convert to float from integer */
    "CVFI3",      /*  44  */
    "CVIF",       /*  45  Convert to integer from float */
    "CVIU3",      /*  46  */
    "CVUI3",      /*  47  */
    "DIVF",       /*  48  Divide floats */
    "DIVI",       /*  49  Divide signed integers. */
    "DIVI3",      /*  50  */
    "DIVU",       /*  51  Divide unsigned integers. */
    "DIVU3",      /*  52  */
    "ENTER",      /*  53  Begin subroutine. */
    "EQ",         /*  54  Compare integers, jump if equal. */
    "EQ3",        /*  55  */
    "EQF",        /*  56  Compare floats, jump if equal */
    "EQU3",       /*  57  */
    "GEF",        /*  58  Compare floats, jump if greater-than-or-equal */
    "GEI",        /*  59  Compare integers, jump if greater-than-or-equal. */
    "GEI3",       /*  60  */
    "GEU",        /*  61  Compare unsigned integers, jump if greater-than-or-equal */
    "GEU3",       /*  62  */
    "GTF",        /*  63  Compare floats, jump if greater-than */
    "GTI",        /*  64  Compare integers, jump if greater-than. */
    "GTI3",       /*  65  */
    "GTU",        /*  66  Compare unsigned integers, jump if greater-than */
    "GTU3",       /*  67  */
    "JUMP",       /*  68  Unconditional jump. */
    "LEAVE",      /*  69  End subroutine. */
    "LEF",        /*  70  Compare floats, jump if less-than-or-equal */
    "LEI",        /*  71  Compare integers, jump if less-than-or-equal. */
    "LEI3",       /*  72  */
    "LEU",        /*  73  Compare unsigned integers, jump if less-than-or-equal */
    "LEU3",       /*  74  */
    "LOAD1",      /*  75  Load 1-byte from memory */
    "LOAD2",      /*  76  Load 2-bytes from memory */
    "LOAD3",      /*  77  Load 3-bytes from memory */
    "LOAD4",      /*  78  Load 4-bytes from memory */
    "LOADF4",     /*  79  Load 4 byte float from memory */
    "LOCAL",      /*  80  Get local variable. */
    "LSH",        /*  81  Left-shift */
    "LSH3",       /*  82  */
    "LTF",        /*  83  Compare floats, jump if less-than */
    "LTI",        /*  84  Compare integers, jump if less-than. */
    "LTI3",       /*  85  */
    "LTU",        /*  86  Compare unsigned integers, jump if less-than */
    "LTU3",       /*  87  */
    "MODI",       /*  88  Modulus (signed). */
    "MODI3",      /*  89  */
    "MODU",       /*  90  Modulus (unsigned). */
    "MODU3",      /*  91  */
    "MULF",       /*  92  Multiply floats */
    "MULI",       /*  93  Multiply signed integers. */
    "MULI3",      /*  94  */
    "MULU",       /*  95  Multiply unsigned integers. */
    "MULU3",      /*  96  */
    "NE",         /*  97  Compare integers, jump if not equal. */
    "NE3",        /*  98  */
    "NEF",        /*  99  Compare floats, jump if not-equal */
    "NEGF",       /* 100  Negate float */
    "NEGI",       /* 101  Negate integer. */
    "NEGI3",      /* 102  */
    "POP",        /* 103  Discard top-of-stack. */
    "PUSH",       /* 104  Push to stack. */
    "RSHI",       /* 105  Right-shift (algebraic; preserve sign) */
    "RSHI3",      /* 106  */
    "RSHU",       /* 107  Right-shift (bitwise; ignore sign) */
    "RSHU3",      /* 108  */
    "SEX24",      /* 109  */
    "SEX8_3",     /* 110  */
    "STORE1",     /* 111  Store 1-byte to memory */
    "STORE2",     /* 112  Store 2-byte to memory */
    "STORE3",     /* 113  */
    "STORE4",     /* 114  *(stack[top-1]) = stack[top] */
    "STOREF4",    /* 115  *(stack[top-1]) = stack[top] */
    "SUB",        /* 116  Subtract integers (two's complement). */
    "SUB3",       /* 117  */
    "SUBF",       /* 118  Subtract floats */
};
