#ifndef __ID_PM__
#define __ID_PM__

#include <ez80.h>

#define PMPageSize 4096

extern int ChunksInFile;
extern int PMSpriteStart;
extern int PMSoundStart;

extern bool PMSoundInfoPagePadded;

// ChunksInFile+1 pointers to page starts.
// The last pointer points one byte after the last page.
extern uint8_t **PMPages;

extern void PM_Startup();
extern void PM_Shutdown();
typedef void (*chunk_load_progress_t)(uint8_t current);
extern void PM_Preload(chunk_load_progress_t update_fn, bool partial);

static inline uint32_t PM_GetPageSize(int page) {
  if (page < 0 || page >= ChunksInFile)
    Quit("PM_GetPageSize: Tried to access illegal page: %i", page);
  return (uint32_t)(PMPages[page + 1] - PMPages[page]);
}

static inline uint8_t *PM_GetPage(int page) {
#if 0
  if (page < 0 || page >= ChunksInFile)
    Quit("PM_GetPage: Tried to access illegal page: %i", page);
#endif

  if (PMPages[page] == NULL) {
    printf("Page %d Missing!!\r\n", page);
    return NULL;
  }

  return PMPages[page];
}

static inline uint8_t *PM_GetEnd() { return PMPages[ChunksInFile]; }

static inline byte *PM_GetTexture(int wallpic) { return PM_GetPage(wallpic); }

static inline uint16_t *PM_GetSprite(int shapenum) {
  // correct alignment is enforced by PM_Startup()
  return (uint16_t *)(void *)PM_GetPage(PMSpriteStart + shapenum);
}

static inline byte *PM_GetSound(int soundpagenum) { return PM_GetPage(PMSoundStart + soundpagenum); }

#endif
