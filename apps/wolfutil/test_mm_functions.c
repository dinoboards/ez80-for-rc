#include "wolfutil.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "id_defs.h"

#include "id_mm.h"

extern memptr bufferseg;

memptr block1;
memptr block2;
memptr block3;
memptr block4;

void test_mm_functions() {

  MM_Startup();

  printf("bufferseg at: %p\r\n", bufferseg);
  MM_DumpMemoryBlocks();

  printf("\r\nAllocate a 2k block\r\n");
  MM_GetPtr(&block1, 2048);
  printf("block1 at %p\r\n\r\n", block1);

  printf("\r\nAllocate a 32k block\r\n");
  MM_GetPtr(&block2, 32768);
  printf("block2 at %p\r\n\r\n", block2);

  printf("\r\nAllocate a 485300 block\r\n");
  MM_GetPtr(&block3, 485300);
  printf("block3 at %p\r\n\r\n", block3);

  printf("\r\nFree the 1st 2K block\r\n");
  MM_FreePtr(&block1);
  printf("block1 at %p\r\n\r\n", block1);

  MM_DumpMemoryBlocks();

  printf("\r\nallocate (2050bytes) to force sorting..\r\n");
  MM_GetPtr(&block1, 2050);
  printf("block1 at %p\r\n\r\n", block1);
  MM_DumpMemoryBlocks();

  printf("\r\nMark large block3 as purgable (not locked)\r\n");
  MM_SetLock(&block3, false);
  MM_SetPurge(&block3, 3);
  MM_DumpMemoryBlocks();

  printf("\r\nAttempt to allocated block, forcing pruging)\r\n");
  MM_GetPtr(&block4, 10000);
  printf("block4 at %p\r\n\r\n", block4);
  MM_DumpMemoryBlocks();

  printf("bufferseg at: %p\r\n", bufferseg);
  printf("block1 at %p\r\n", block1);
  printf("block2 at %p\r\n", block2);
  printf("block3 at %p\r\n", block3);
  printf("block4 at %p\r\n", block4);

  MM_Shutdown();
}
