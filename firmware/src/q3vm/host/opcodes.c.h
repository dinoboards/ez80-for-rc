#include "opcodes.h"

const char *const opnames[OPCODE_TABLE_SIZE] = {
    "UNDEF",    "IGNORE",  "BREAK",   "ENTER",   "LEAVE",   "CALL",    "PUSH",       "POP",     "CONSTGP4", "LOCAL",
    "JUMP",     "EQ",      "NE",      "LTI",     "LEI",     "GTI",     "GEI",        "LTU",     "LEU",      "GTU",
    "GEU",      "EQF",     "NEF",     "LTF",     "LEF",     "GTF",     "GEF",        "LOAD1",   "LOAD2",    "LOAD4",
    "LOADF4",   "STORE1",  "STORE2",  "STORE4",  "STOREF4", "ARG",     "BLOCK_COPY", "SEX8",    "SEX16",    "NEGI",
    "ADD",      "SUB",     "DIVI",    "DIVU",    "MODI",    "MODU",    "MULI",       "MULU",    "BAND",     "BOR",
    "BXOR",     "BCOM",    "LSH",     "RSHI",    "RSHU",    "NEGF",    "ADDF",       "SUBF",    "DIVF",     "MULF",
    "CVIF",     "CVFI",    "CONSTU1", "CONSTI1", "CONSTU2", "CONSTI2", "CONSTU4",    "CONSTI4", "CONSTF4",  "CONSTP4",

    "LOAD3",    "ADD3",    "BAND3",   "BCOM3",   "BOR3",    "BXOR3",   "CONSTI3",    "CONSTU3", "CVFI3",    "CVIU3",
    "CVUI3",    "DIVI3",   "DIVU3",   "EQ3",     "GEI3",    "GEU3",    "GTI3",       "GTU3",    "LEI3",     "LEU3",
    "LSH3",     "LTI3",    "LTU3",    "MODI3",   "MODU3",   "MULI3",   "MULU3",      "NE3",     "NEGI3",    "RSHI3",
    "RSHU3",    "SEX24",   "SEX8_3",  "STORE3",  "SUB3",

    "CONSTGP3", "CONSTP3", "ARGF",    "ARG4"};
