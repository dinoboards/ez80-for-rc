// ID_MM.H

#ifndef __ID_MM__
#define __ID_MM__

#define BUFFERSIZE 0x1000 // miscellaneous, always available buffer

#define MAXBLOCKS 700

//==========================================================================

typedef uint8_t *memptr;

//==========================================================================

extern memptr  bufferseg;
extern boolean mmerror;

extern void (*beforesort)(void);
extern void (*aftersort)(void);

//==========================================================================

void MM_Startup(void);
void MM_Shutdown(void);

void MM_GetPtr(memptr *baseptr, uint24_t size);
void MM_FreePtr(memptr *baseptr);

void MM_SetPurge(memptr *baseptr, int purge);
void MM_SetLock(memptr *baseptr, boolean locked);
void MM_SortMem(void);

void MM_DumpMemoryBlocks(void);
void MM_ShowMemory(void);

long MM_UnusedMemory(void);
long MM_TotalFree(void);

void MM_BombOnError(boolean bomb);

#endif
