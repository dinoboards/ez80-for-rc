#ifndef __CRITICAL_BLOCKS_H__
#define __CRITICAL_BLOCKS_H__

/*
 * Function implemented by macros
 */
#define critical_begin()                                                                                                           \
  {                                                                                                                                \
    asm("LD    a, i");                                                                                                             \
    asm("PUSH  af");                                                                                                               \
    asm("DI");                                                                                                                     \
  }

#define critical_end()                                                                                                             \
  {                                                                                                                                \
    asm("POP   hl");                                                                                                               \
    asm("BIT   #2, l");                                                                                                            \
    asm("JR    Z, $F");                                                                                                            \
    asm("EI");                                                                                                                     \
    asm("$$:");                                                                                                                    \
  }

#endif
