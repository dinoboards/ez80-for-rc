// NEWMM.C

#include <ez80.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wl_def.h"

#include "id_mm.h"

typedef struct mmblockstruct {
  memptr                start;
  uint24_t              length;
  unsigned              attributes; // 8bits?
  memptr               *useptr;     // pointer to the segment start
  struct mmblockstruct *next;
} mmblocktype;

#define GETNEWBLOCK                                                                                                                \
  {                                                                                                                                \
    if (!mmfree)                                                                                                                   \
      MML_ClearBlock();                                                                                                            \
    mmnew  = mmfree;                                                                                                               \
    mmfree = mmnew->next;                                                                                                          \
  }

#define FREEBLOCK(x)                                                                                                               \
  {                                                                                                                                \
    *x->useptr = NULL;                                                                                                             \
    x->next    = mmfree;                                                                                                           \
    mmfree     = x;                                                                                                                \
  }

/*
=============================================================================

                                                 GLOBAL VARIABLES

=============================================================================
*/

boolean mmerror;

void (*beforesort)(void);
void (*aftersort)(void);

/*
=============================================================================

                                                 LOCAL VARIABLES

=============================================================================
*/

boolean mmstarted;

void *heap;

mmblocktype mmblocks[MAXBLOCKS], *mmhead, *mmfree, *mmrover, *mmnew;

boolean bombonerror;

//==========================================================================

/*
====================
=
= MML_ClearBlock
=
= We are out of blocks, so free a purgable block
=
====================
*/

void MML_ClearBlock(void) {
  mmblocktype *scan;

  scan = mmhead->next;

  while (scan) {
    if (!(scan->attributes & LOCKBIT) && (scan->attributes & PURGEBITS)) {
      MM_FreePtr(scan->useptr);
      return;
    }
    scan = scan->next;
  }

  Quit("MM_ClearBlock: No purgable blocks!");
}

//==========================================================================

/*
===================
=
= MM_Startup
=
===================
*/

void MM_Startup(void) {
  int      i;
  uint24_t length;
  uint8_t *start;

  // lets just use the last 512K or memory for this testing

  if (mmstarted)
    MM_Shutdown();

  mmstarted   = true;
  bombonerror = true;
  //
  // set up the linked list (everything in the free list;
  //
  mmhead = NULL;
  mmfree = &mmblocks[0];
  for (i = 0; i < MAXBLOCKS - 1; i++)
    mmblocks[i].next = &mmblocks[i + 1];
  mmblocks[i].next = NULL;

  printf("extended_memory_start: %p\r\n", get_memory_start());
  printf("extended_memory_end: %p\r\n", get_memory_end());

  length = (uint24_t)get_memory_end() - (uint24_t)_heap;
  start  = _heap;
  printf("Heap size: %d\r\n", length);

  if (start == 0) {
    Quit("MM_Startup: Could not allocate heap!");
  }
  memptr end = start + length;
  //
  // locked block of all memory until we punch out free space
  //
  GETNEWBLOCK;
  mmhead             = mmnew; // this will always be the first node
  mmhead->start      = start;
  mmhead->length     = 0;
  mmhead->attributes = LOCKBIT;
  mmhead->useptr     = NULL;
  mmrover            = mmhead;

  GETNEWBLOCK;
  mmnew->start      = end - 0;
  mmnew->length     = 0;
  mmnew->attributes = LOCKBIT;
  mmnew->next       = NULL;
  mmnew->useptr     = NULL;
  mmhead->next      = mmnew;

  mmrover = mmhead; // start looking for space after low block
}

/*
====================
=
= MM_Shutdown
=
= Frees all conventional, EMS, and XMS allocated
=
====================
*/

void MM_Shutdown(void) {
  if (!mmstarted)
    return;

  // free(heap);
}

//==========================================================================

/*
====================
=
= MM_GetPtr
=
= Allocates an unlocked, unpurgable block
=
====================
*/

void MM_GetPtr(memptr *baseptr, uint24_t size) {
  mmblocktype *scan, *lastscan, *endscan, *purge, *next;
  int          search;
  uint24_t     needed;
  memptr       startseg;

  needed = size;

  GETNEWBLOCK; // fill in start and next after a spot is found
  mmnew->length     = needed;
  mmnew->useptr     = baseptr;
  mmnew->attributes = BASEATTRIBUTES;

  for (search = 0; search < 3; search++) {

    //
    // first search:	try to allocate right after the rover, then on up
    // second search: 	search from the head pointer up to the rover
    // third search:	compress memory, then scan from start
    if (search == 1 && mmrover == mmhead)
      search++;

    switch (search) {
    case 0:
      lastscan = mmrover;
      scan     = mmrover->next;
      endscan  = NULL;
      break;
    case 1:
      lastscan = mmhead;
      scan     = mmhead->next;
      endscan  = mmrover;
      break;
    case 2:
      MM_SortMem();
      lastscan = mmhead;
      scan     = mmhead->next;
      endscan  = NULL;
      break;
    }

    startseg = lastscan->start + lastscan->length;

    while (scan != endscan) {
      if ((uint24_t)(scan->start - startseg) >= needed) {
        //
        // got enough space between the end of lastscan and
        // the start of scan, so throw out anything in the middle
        // and allocate the new block
        //
        purge          = lastscan->next;
        lastscan->next = mmnew;
        mmnew->start   = startseg;
        *baseptr       = startseg;
        mmnew->next    = scan;
        while (purge != scan) { // free the purgable block
          next = purge->next;
          FREEBLOCK(purge);
          purge = next; // purge another if not at scan
        }
        mmrover = mmnew;
        return; // good allocation!
      }

      //
      // if this block is purge level zero or locked, skip past it
      //
      if ((scan->attributes & LOCKBIT) || !(scan->attributes & PURGEBITS)) {
        lastscan = scan;
        startseg = lastscan->start + lastscan->length;
      }

      scan = scan->next; // look at next line
    }
  }

  if (bombonerror) {
    MM_DumpMemoryBlocks();
    Quit("MM_GetPtr: Out of memory!");
  } else
    mmerror = true;
}

//==========================================================================

/*
====================
=
= MM_FreePtr
=
= Deallocates an unlocked, purgable block
=
====================
*/

void MM_FreePtr(memptr *baseptr) {
  mmblocktype *scan, *last;

  last = mmhead;
  scan = last->next;

  if (baseptr == mmrover->useptr) // removed the last allocated block
    mmrover = mmhead;

  while (scan->useptr != baseptr && scan) {
    last = scan;
    scan = scan->next;
  }

  if (!scan)
    Quit("MM_FreePtr: Block not found!");

  last->next = scan->next;

  *baseptr = NULL;
  FREEBLOCK(scan);
}
//==========================================================================

/*
=====================
=
= MM_SetPurge
=
= Sets the purge level for a block (locked blocks cannot be made purgable)
=
=====================
*/

void MM_SetPurge(memptr *baseptr, int purge) {
  mmblocktype *start;

  start = mmrover;

  do {
    if (mmrover->useptr == baseptr)
      break;

    mmrover = mmrover->next;

    if (!mmrover)
      mmrover = mmhead;
    else if (mmrover == start)
      Quit("MM_SetPurge: Block not found!");

  } while (1);

  mmrover->attributes &= ~PURGEBITS;
  mmrover->attributes |= purge;
}

//==========================================================================

/*
=====================
=
= MM_SetLock
=
= Locks / unlocks the block
=
=====================
*/

void MM_SetLock(memptr *baseptr, boolean locked) {
  mmblocktype *start;

  start = mmrover;

  do {
    if (mmrover->useptr == baseptr)
      break;

    mmrover = mmrover->next;

    if (!mmrover)
      mmrover = mmhead;
    else if (mmrover == start)
      Quit("MM_SetLock: Block not found!");

  } while (1);

  mmrover->attributes &= ~LOCKBIT;
  mmrover->attributes |= locked * LOCKBIT;
}

//==========================================================================

/*
=====================
=
= MM_SortMem
=
= Throws out all purgable stuff and compresses movable blocks
=
=====================
*/

void MM_SortMem(void) {
  mmblocktype *scan, *last, *next;
  memptr       start;
  memptr       source;
  memptr       dest;
  uint24_t     length;
  // int      playing;

  //
  // lock down a currently playing sound
  //
  // playing = SD_SoundPlaying();
  // if (playing) {
  //   switch (SoundMode) {
  //   case sdm_PC:
  //     playing += STARTPCSOUNDS;
  //     break;
  //   case sdm_AdLib:
  //     playing += STARTADLIBSOUNDS;
  //     break;
  //   }
  //   MM_SetLock(&(memptr)audiosegs[playing], true);
  // }

  // SD_StopSound();

  if (beforesort)
    beforesort();

  scan = mmhead;

  last = NULL; // shut up compiler warning

  while (scan) {
    if (scan->attributes & LOCKBIT) {
      //
      // block is locked, so try to pile later blocks right after it
      //
      start = scan->start + scan->length;
    } else {
      if (scan->attributes & PURGEBITS) {
        //
        // throw out the purgable block
        //
        next = scan->next;
        FREEBLOCK(scan);
        last->next = next;
        scan       = next;
        continue;
      } else {
        //
        // push the non purgable block on top of the last moved block
        //
        if (scan->start != start) {
          length = scan->length;
          source = scan->start;
          dest   = start;
          printf("MM_SortMem: moving %d bytes from %p to %p\r\n", length, source, dest);
          memcpy(dest, source, length);
          scan->start   = start;
          *scan->useptr = start;
        }

        start = scan->start + scan->length;
      }
    }

    last = scan;
    scan = scan->next; // go to next block
  }

  mmrover = mmhead;

  if (aftersort)
    aftersort();

  // if (playing)
  //   MM_SetLock(&(memptr)audiosegs[playing], false);
}

//==========================================================================

/*
=====================
=
= MM_ShowMemory
=
=====================
*/

void MM_DumpMemoryBlocks(void) {
  mmblocktype *scan = mmhead;
  while (scan) {
    memptr next = scan->start + scan->length;
    printf("start: %X, length: %X, next: %X, attributes: %X, useptr: %X, next: %X\r\n", (int)scan->start, scan->length, (int)next,
           scan->attributes, (int)scan->useptr, (int)scan->next);
    scan = scan->next;
  }

  printf("--Free Blocks--\r\n");
  mmblocktype *prev = mmhead;
  scan              = mmhead->next;

  while (scan) {
    memptr   end_of_previous = prev->start + prev->length;
    uint24_t gap             = scan->start - end_of_previous;
    if (gap > 0)
      printf("start: %X, length: %X, end: %X\r\n", (int)end_of_previous, gap, (int)scan->start);
    prev = scan;
    scan = scan->next;
  }
  printf("---\r\n");
}

//==========================================================================

/*
======================
=
= MM_UnusedMemory
=
= Returns the total free space without purging
=
======================
*/

long MM_UnusedMemory(void) {
  unsigned     free;
  mmblocktype *scan;

  free = 0;
  scan = mmhead;

  while (scan->next) {
    free += scan->next->start - (scan->start + scan->length);
    scan = scan->next;
  }

  return free * 16l;
}

//==========================================================================

/*
======================
=
= MM_TotalFree
=
= Returns the total free space with purging
=
======================
*/

long MM_TotalFree(void) {
  unsigned     free;
  mmblocktype *scan;

  free = 0;
  scan = mmhead;

  while (scan->next) {
    if ((scan->attributes & PURGEBITS) && !(scan->attributes & LOCKBIT))
      free += scan->length;
    free += scan->next->start - (scan->start + scan->length);
    scan = scan->next;
  }

  return free * 16l;
}

//==========================================================================

/*
=====================
=
= MM_BombOnError
=
=====================
*/

void MM_BombOnError(boolean bomb) { bombonerror = bomb; }
