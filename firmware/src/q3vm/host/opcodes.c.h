#include "opcodes.h"

const char *const opnames[OPCODE_TABLE_SIZE] = {
    "UNDEF",      /*   0  Error: VM halt */
    "IGNORE",     /*   1  No operation */
    "BREAK",      /*   2  vm->breakCount++ */
    "ADD3",       /*   3  */
    "ADD4",       /*   4  Add integers (two's complement). */
    "ADDF4",      /*   5  Add floats */
    "ARG3",       /*   6  Marshal argument */
    "ARG4",       /*   7  */
    "ARGF4",      /*   8  */
    "BAND3",      /*   9  */
    "BAND4",      /*  10  Bitwise AND */
    "BCOM3",      /*  11  */
    "BCOM4",      /*  12  Bitwise COMplement */
    "BLOCK_COPY", /*  13  memcpy */
    "BOR3",       /*  14  */
    "BOR4",       /*  15  Bitwise OR */
    "BXOR3",      /*  16  */
    "BXOR4",      /*  17  Bitwise eXclusive-OR */
    "CALL",       /*  18  Call subroutine. */
    "CF4I4",      /*  19  Convert to float from integer */
    "CI1I3",      /*  20  */
    "CI1I4",      /*  21  Sign-Extend 8-bit */
    "CI2I3",      /*  22  */
    "CI2I4",      /*  23  Sign-Extend 16-bit */
    "CI3I1",      /*  24  */
    "CI3I2",      /*  25  */
    "CI3I4",      /*  26  */
    "CI3U4",      /*  27  */
    "CI4F4",      /*  28  Convert to integer from float */
    "CI4I3",      /*  29  */
    "CI4U3",      /*  30  */
    "CONSTF4",    /*  31  */
    "CONSTGP3",   /*  32  */
    "CONSTGP4",   /*  33  Load constant to stack. */
    "CONSTI1",    /*  34  */
    "CONSTI2",    /*  35  */
    "CONSTI3",    /*  36  */
    "CONSTI4",    /*  37  */
    "CONSTP3",    /*  38  */
    "CONSTP4",    /*  39  */
    "CONSTU1",    /*  40  */
    "CONSTU2",    /*  41  */
    "CONSTU3",    /*  42  */
    "CONSTU4",    /*  43  */
    "CU2I3",      /*  44  */
    "CU3U2",      /*  45  */
    "CU3U4",      /*  46  */
    "CU4I3",      /*  47  */
    "CU4U3",      /*  48  */
    "CVFI3",      /*  49  */
    "CVIU3",      /*  50  */
    "CVUI3",      /*  51  */
    "DIVF4",      /*  52  Divide floats */
    "DIVI",       /*  53  Divide signed integers. */
    "DIVI3",      /*  54  */
    "DIVU",       /*  55  Divide unsigned integers. */
    "DIVU3",      /*  56  */
    "ENTER",      /*  57  Begin subroutine. */
    "EQ3",        /*  58  */
    "EQ4",        /*  59  Compare integers, jump if equal. */
    "EQF4",       /*  60  Compare floats, jump if equal */
    "EQU3",       /*  61  */
    "GEF4",       /*  62  Compare floats, jump if greater-than-or-equal */
    "GEI3",       /*  63  */
    "GEI4",       /*  64  Compare integers, jump if greater-than-or-equal. */
    "GEU3",       /*  65  */
    "GEU4",       /*  66  Compare unsigned integers, jump if greater-than-or-equal */
    "GTF4",       /*  67  Compare floats, jump if greater-than */
    "GTI3",       /*  68  */
    "GTI4",       /*  69  Compare integers, jump if greater-than. */
    "GTU3",       /*  70  */
    "GTU4",       /*  71  Compare unsigned integers, jump if greater-than */
    "JUMP",       /*  72  Unconditional jump. */
    "LEAVE",      /*  73  End subroutine. */
    "LEF",        /*  74  Compare floats, jump if less-than-or-equal */
    "LEI3",       /*  75  */
    "LEI4",       /*  76  Compare integers, jump if less-than-or-equal. */
    "LEU",        /*  77  Compare unsigned integers, jump if less-than-or-equal */
    "LEU3",       /*  78  */
    "LOAD1",      /*  79  Load 1-byte from memory */
    "LOAD2",      /*  80  Load 2-bytes from memory */
    "LOAD3",      /*  81  Load 3-bytes from memory */
    "LOAD4",      /*  82  Load 4-bytes from memory */
    "LOADF4",     /*  83  Load 4 byte float from memory */
    "LOCAL",      /*  84  Get local variable. */
    "LSH3",       /*  85  */
    "LSH4",       /*  86  Left-shift */
    "LTF",        /*  87  Compare floats, jump if less-than */
    "LTI3",       /*  88  */
    "LTI4",       /*  89  Compare integers, jump if less-than. */
    "LTU",        /*  90  Compare unsigned integers, jump if less-than */
    "LTU3",       /*  91  */
    "MODI3",      /*  92  */
    "MODI4",      /*  93  Modulus (signed). */
    "MODU3",      /*  94  */
    "MODU4",      /*  95  Modulus (unsigned). */
    "MULF4",      /*  96  Multiply floats */
    "MULI3",      /*  97  */
    "MULI4",      /*  98  Multiply signed integers. */
    "MULU3",      /*  99  */
    "MULU4",      /* 100  Multiply unsigned integers. */
    "NE3",        /* 101  */
    "NE4",        /* 102  Compare integers, jump if not equal. */
    "NEF",        /* 103  Compare floats, jump if not-equal */
    "NEGF4",      /* 104  Negate float */
    "NEGI3",      /* 105  */
    "NEGI4",      /* 106  Negate integer. */
    "POP",        /* 107  Discard top-of-stack. */
    "PUSH",       /* 108  Push to stack. */
    "RSHI3",      /* 109  */
    "RSHI4",      /* 110  Right-shift (algebraic; preserve sign) */
    "RSHU3",      /* 111  */
    "RSHU4",      /* 112  Right-shift (bitwise; ignore sign) */
    "SEX24",      /* 113  */
    "SEX8_3",     /* 114  */
    "STORE1",     /* 115  Store 1-byte to memory */
    "STORE2",     /* 116  Store 2-byte to memory */
    "STORE3",     /* 117  */
    "STORE4",     /* 118  *(stack[top-1]) = stack[top] */
    "SUB3",       /* 119  */
    "SUB4",       /* 120  Subtract integers (two's complement). */
    "SUBF4",      /* 121  Subtract floats */
};
