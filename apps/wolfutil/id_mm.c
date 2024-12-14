// NEWMM.C

#include <ez80.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "id_defs.h"
#include "id_mm.h"

#define LOCKBIT        0x80 // if set in attributes, block cannot be moved
#define PURGEBITS      3    // 0-3 level, 0= unpurgable, 3= purge first
#define PURGEMASK      0xfffc
#define BASEATTRIBUTES 0 // unlocked, non purgable

#define MAXUMBS 10

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

memptr  bufferseg;
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
  mmblocktype far *scan;

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
= Allocates bufferseg misc buffer
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

  length = 512 * 1024;
  start  = malloc(length);
  if (start == 0) {
    Quit("MM_Startup: Could not allocate heap!");
  }
  memptr end = start + length;

  printf("Start: %X, len: %X, end: %X\r\n", (uint24_t)start, length, (int)end);

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

  MM_GetPtr(&bufferseg, BUFFERSIZE);
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

  free(heap);
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
  mmblocktype far *scan, far *last;

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
  mmblocktype far *start;

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
  mmblocktype far *start;

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
  mmblocktype far *scan, far *last, far *next;
  memptr   start;
  memptr   source;
  memptr   dest;
  uint24_t length;
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
          printf("memcpy: %X, %X, %X\r\n", (int)dest, (int)source, length);
          memcpy(dest, source, length);

          printf("Point 1\r\n");
          scan->start   = start;
          *scan->useptr = start;
        }

        start = scan->start + scan->length;
      }
    }

    last = scan;
    scan = scan->next; // go to next block
    printf("last: %X, scan: %X\r\n", (int)last, (int)scan);
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

void MM_ShowMemory(void) {
  mmblocktype *scan;
  unsigned     color, /*temp,*/ x, y;
  uint24_t     end;
  // int              owner;
  // char scratch[80], str[10];

  // temp      = bufferofs;
  // bufferofs = displayofs;
  scan = mmhead;

  end = (uint24_t)-1;

  while (scan) {
    if (scan->attributes & PURGEBITS)
      color = 5; // dark purple = purgable
    else
      color = 9; // medium blue = non purgable
    if (scan->attributes & LOCKBIT)
      color = 12; // red = locked
    // if (scan->start <= end)
    //   Quit("MM_ShowMemory: Memory block order currupted!");
    end = scan->length - 1;
    y   = (uint24_t)scan->start / 320;
    x   = (uint24_t)scan->start % 320;

    printf("x: %d, y: %d, end: %d, color: %d\r\n", x, y, end, color);

    if (scan->next && (uint24_t)scan->next->start > end + 1)
      printf("free: x: %d, y: %d, end: %d, color: %d\r\n", x + end + 1, x + (scan->next->start - scan->start), y, 0);
    scan = scan->next;
  }

  // VW_FadeIn();
  // IN_Ack();

  // bufferofs = temp;
}

//==========================================================================

/*
=====================
=
= MM_DumpData
=
=====================
*/

// void MM_DumpData(void) {
//   mmblocktype far *scan, far *best;
//   long     lowest, oldlowest;
//   unsigned owner;
//   char     lock, purge;
//   FILE    *dumpfile;

//   free(heap);
//   dumpfile = fopen("MMDUMP.TXT", "w");
//   if (!dumpfile)
//     Quit("MM_DumpData: Couldn't open MMDUMP.TXT!");

//   lowest = -1;
//   do {
//     oldlowest = lowest;
//     lowest    = 0xffff;

//     scan = mmhead;
//     while (scan) {
//       owner = (unsigned)scan->useptr;

//       if (owner && owner < lowest && owner > oldlowest) {
//         best   = scan;
//         lowest = owner;
//       }

//       scan = scan->next;
//     }

//     if (lowest != 0xffff) {
//       if (best->attributes & PURGEBITS)
//         purge = 'P';
//       else
//         purge = '-';
//       if (best->attributes & LOCKBIT)
//         lock = 'L';
//       else
//         lock = '-';
//       fprintf(dumpfile, "0x%p (%c%c) = %u\n", (unsigned)lowest, lock, purge, best->length);
//     }

//   } while (lowest != 0xffff);

//   fclose(dumpfile);
//   Quit("MMDUMP.TXT created.");
// }

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
  unsigned         free;
  mmblocktype far *scan;

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
  unsigned         free;
  mmblocktype far *scan;

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
