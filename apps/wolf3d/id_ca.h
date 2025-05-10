#ifndef __ID_CA__
#define __ID_CA__

//===========================================================================

#define NUMMAPS   60
#define MAPPLANES 2

#define UNCACHEGRCHUNK(chunk)                                                                                                      \
  {                                                                                                                                \
    if (grsegs[chunk]) {                                                                                                           \
      MM_FreePtr((memptr *)&grsegs[chunk]);                                                                                        \
    }                                                                                                                              \
  }
#define UNCACHEAUDIOCHUNK(chunk)                                                                                                   \
  {                                                                                                                                \
    if (audiosegs[chunk]) {                                                                                                        \
      MM_FreePtr((memptr *)&audiosegs[chunk]);                                                                                     \
    }                                                                                                                              \
  }

//===========================================================================

typedef struct {
  int32_t planestart[3];
  word    planelength[3];
  word    width, height;
  char    name[16];
} maptype;

//===========================================================================

extern int   mapon;
extern word *mapsegs[MAPPLANES];
extern byte *audiosegs[NUMSNDCHUNKS];
extern byte *grsegs[NUMCHUNKS];

extern char extension[5];
extern char graphext[5];
extern char audioext[5];

//===========================================================================

boolean CA_LoadFile(const char *filename, memptr *ptr);
boolean CA_WriteFile(const char *filename, void *ptr, int32_t length);

int32_t CA_RLEWCompress(word *source, int32_t length, word *dest, word rlewtag);

void CA_RLEWexpand(word *source, word *dest, int32_t length, word rlewtag);

void CA_Startup(void);
void CA_Shutdown(void);

#ifdef SOUND_ENABLED
int32_t CA_CacheAudioChunk(int chunk);
void    CA_LoadAllSounds(void);
#else
#define CA_CacheAudioChunk(chunk) 0
#define CA_LoadAllSounds()
#endif

void CA_CacheGrChunk(int chunk);
void CA_CacheMap(int mapnum);

void CA_CacheScreen(int chunk, uint8_t clip_top);

void CA_CannotOpen(const char *name);

typedef struct {
  word bit0;
  word bit1; // 0-255 is a character, > is a pointer to a node
} huffnode;

extern void CAL_HuffExpand(byte *source, byte *dest, int24_t length /*, huffnode *hufftable*/);

#endif
