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
    "CI3F4",      /*  24  */
    "CI3I1",      /*  25  */
    "CI3I2",      /*  26  */
    "CI3I4",      /*  27  */
    "CI3U4",      /*  28  */
    "CI4F4",      /*  29  Convert to integer from float */
    "CI4I3",      /*  30  */
    "CI4U3",      /*  31  */
    "CONSTGP3",   /*  32  push address pointer to stack. */
    "CONSTI1",    /*  33  */
    "CONSTI2",    /*  34  */
    "CONSTI3",    /*  35  */
    "CONSTI4",    /*  36  */
    "CONSTP3",    /*  37  */
    "CONSTU1",    /*  38  */
    "CONSTU2",    /*  39  */
    "CONSTU3",    /*  40  */
    "CONSTU4",    /*  41  */
    "CU1I3",      /*  42  */
    "CU2I3",      /*  43  */
    "CU3U2",      /*  44  */
    "CU3U4",      /*  45  */
    "CU4I3",      /*  46  */
    "CU4U3",      /*  47  */
    "CVFI3",      /*  48  */
    "CVIU3",      /*  49  */
    "CVUI3",      /*  50  */
    "DIVF4",      /*  51  Divide floats */
    "DIVI",       /*  52  Divide signed integers. */
    "DIVI3",      /*  53  */
    "DIVU",       /*  54  Divide unsigned integers. */
    "DIVU3",      /*  55  */
    "ENTER",      /*  56  Begin subroutine. */
    "EQ3",        /*  57  */
    "EQ4",        /*  58  Compare integers, jump if equal. */
    "EQF4",       /*  59  Compare floats, jump if equal */
    "EQU3",       /*  60  */
    "GEF4",       /*  61  Compare floats, jump if greater-than-or-equal */
    "GEI3",       /*  62  */
    "GEI4",       /*  63  Compare integers, jump if greater-than-or-equal. */
    "GEU3",       /*  64  */
    "GEU4",       /*  65  Compare unsigned integers, jump if greater-than-or-equal */
    "GTF4",       /*  66  Compare floats, jump if greater-than */
    "GTI3",       /*  67  */
    "GTI4",       /*  68  Compare integers, jump if greater-than. */
    "GTU3",       /*  69  */
    "GTU4",       /*  70  Compare unsigned integers, jump if greater-than */
    "JUMP",       /*  71  Unconditional jump. */
    "LEAVE",      /*  72  End subroutine. */
    "LEF",        /*  73  Compare floats, jump if less-than-or-equal */
    "LEI3",       /*  74  */
    "LEI4",       /*  75  Compare integers, jump if less-than-or-equal. */
    "LEU",        /*  76  Compare unsigned integers, jump if less-than-or-equal */
    "LEU3",       /*  77  */
    "LOAD1",      /*  78  Load 1-byte from memory */
    "LOAD2",      /*  79  Load 2-bytes from memory */
    "LOAD3",      /*  80  Load 3-bytes from memory */
    "LOAD4",      /*  81  Load 4-bytes from memory */
    "LOADF4",     /*  82  Load 4 byte float from memory */
    "LOCAL",      /*  83  Get local variable. */
    "LSH3",       /*  84  */
    "LSH4",       /*  85  Left-shift */
    "LTF",        /*  86  Compare floats, jump if less-than */
    "LTI3",       /*  87  */
    "LTI4",       /*  88  Compare integers, jump if less-than. */
    "LTU",        /*  89  Compare unsigned integers, jump if less-than */
    "LTU3",       /*  90  */
    "MODI3",      /*  91  */
    "MODI4",      /*  92  Modulus (signed). */
    "MODU3",      /*  93  */
    "MODU4",      /*  94  Modulus (unsigned). */
    "MULF4",      /*  95  Multiply floats */
    "MULI3",      /*  96  */
    "MULI4",      /*  97  Multiply signed integers. */
    "MULU3",      /*  98  */
    "MULU4",      /*  99  Multiply unsigned integers. */
    "NE3",        /* 100  */
    "NE4",        /* 101  Compare integers, jump if not equal. */
    "NEF",        /* 102  Compare floats, jump if not-equal */
    "NEGF4",      /* 103  Negate float */
    "NEGI3",      /* 104  */
    "NEGI4",      /* 105  Negate integer. */
    "POP",        /* 106  Discard top-of-stack. */
    "PUSH",       /* 107  Push to stack. */
    "RSHI3",      /* 108  */
    "RSHI4",      /* 109  Right-shift (algebraic; preserve sign) */
    "RSHU3",      /* 110  */
    "RSHU4",      /* 111  Right-shift (bitwise; ignore sign) */
    "STORE1",     /* 112  Store 1-byte to memory */
    "STORE2",     /* 113  Store 2-byte to memory */
    "STORE3",     /* 114  */
    "STORE4",     /* 115  *(stack[top-1]) = stack[top] */
    "SUB3",       /* 116  */
    "SUB4",       /* 117  Subtract integers (two's complement). */
    "SUBF4",      /* 118  Subtract floats */
};
