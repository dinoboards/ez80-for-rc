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
    "BAND3",      /*   8  */
    "BAND4",      /*   9  Bitwise AND */
    "BCOM3",      /*  10  */
    "BCOM4",      /*  11  Bitwise COMplement */
    "BLOCK_COPY", /*  12  memcpy */
    "BOR3",       /*  13  */
    "BOR4",       /*  14  Bitwise OR */
    "BXOR3",      /*  15  */
    "BXOR4",      /*  16  Bitwise eXclusive-OR */
    "CALL",       /*  17  Call subroutine. */
    "CF4I3",      /*  18  */
    "CF4I4",      /*  19  Convert to float from integer */
    "CI1I3",      /*  20  */
    "CI2I3",      /*  21  */
    "CI3F4",      /*  22  */
    "CI3s4",      /*  23  */
    "CI4F4",      /*  24  Convert to integer from float */
    "CONSTGP3",   /*  25  push address pointer to stack. */
    "CONSTP3",    /*  26  */
    "CONSTs1",    /*  27  */
    "CONSTs2",    /*  28  */
    "CONSTs3",    /*  29  */
    "CONSTs4",    /*  30  */
    "CU1I3",      /*  31  */
    "CU2I3",      /*  32  */
    "CU3U4",      /*  33  */
    "DIVF4",      /*  34  Divide floats */
    "DIVI3",      /*  35  */
    "DIVI4",      /*  36  Divide signed integers. */
    "DIVU3",      /*  37  */
    "DIVU4",      /*  38  Divide unsigned integers. */
    "ENTER",      /*  39  Begin subroutine. */
    "EQ3",        /*  40  */
    "EQ4",        /*  41  Compare integers, jump if equal. */
    "EQF4",       /*  42  Compare floats, jump if equal */
    "GEF4",       /*  43  Compare floats, jump if greater-than-or-equal */
    "GEI3",       /*  44  */
    "GEI4",       /*  45  Compare integers, jump if greater-than-or-equal. */
    "GEU3",       /*  46  */
    "GEU4",       /*  47  Compare unsigned integers, jump if greater-than-or-equal */
    "GTF4",       /*  48  Compare floats, jump if greater-than */
    "GTI3",       /*  49  */
    "GTI4",       /*  50  Compare integers, jump if greater-than. */
    "GTU3",       /*  51  */
    "GTU4",       /*  52  Compare unsigned integers, jump if greater-than */
    "JUMP",       /*  53  Unconditional jump. */
    "LEAVE",      /*  54  End subroutine. */
    "LEF4",       /*  55  Compare floats, jump if less-than-or-equal */
    "LEI3",       /*  56  */
    "LEI4",       /*  57  Compare integers, jump if less-than-or-equal. */
    "LEU3",       /*  58  */
    "LEU4",       /*  59  Compare unsigned integers, jump if less-than-or-equal */
    "LOAD1",      /*  60  Load 1-byte from memory */
    "LOAD2",      /*  61  Load 2-bytes from memory */
    "LOAD3",      /*  62  Load 3-bytes from memory */
    "LOAD4",      /*  63  Load 4-bytes from memory */
    "LOCAL",      /*  64  Get local variable. */
    "LSH3",       /*  65  */
    "LSH4",       /*  66  Left-shift */
    "LTF4",       /*  67  Compare floats, jump if less-than */
    "LTI3",       /*  68  */
    "LTI4",       /*  69  Compare integers, jump if less-than. */
    "LTU3",       /*  70  */
    "LTU4",       /*  71  Compare unsigned integers, jump if less-than */
    "MODI3",      /*  72  */
    "MODI4",      /*  73  Modulus (signed). */
    "MODU3",      /*  74  */
    "MODU4",      /*  75  Modulus (unsigned). */
    "MULF4",      /*  76  Multiply floats */
    "MULI3",      /*  77  */
    "MULI4",      /*  78  Multiply signed integers. */
    "MULU3",      /*  79  */
    "MULU4",      /*  80  Multiply unsigned integers. */
    "NE3",        /*  81  */
    "NE4",        /*  82  Compare integers, jump if not equal. */
    "NEF4",       /*  83  Compare floats, jump if not-equal */
    "NEGF4",      /*  84  Negate float */
    "NEGI3",      /*  85  */
    "NEGI4",      /*  86  Negate integer. */
    "POP",        /*  87  Discard top-of-stack. */
    "PUSH",       /*  88  Push to stack. */
    "RSHI3",      /*  89  */
    "RSHI4",      /*  90  Right-shift (algebraic; preserve sign) */
    "RSHU3",      /*  91  */
    "RSHU4",      /*  92  Right-shift (bitwise; ignore sign) */
    "STORE1",     /*  93  Store 1-byte to memory */
    "STORE2",     /*  94  Store 2-byte to memory */
    "STORE3",     /*  95  */
    "STORE4",     /*  96  *(stack[top-1]) = stack[top] */
    "SUB3",       /*  97  */
    "SUB4",       /*  98  Subtract integers (two's complement). */
    "SUBF4",      /*  99  Subtract floats */
};
