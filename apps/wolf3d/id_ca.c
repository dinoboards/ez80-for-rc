// ID_CA.C

// this has been customized for WOLF

/*
=============================================================================

Id Software Caching Manager
---------------------------

Must be started BEFORE the memory manager, because it needs to get the headers
loaded into the data segment

=============================================================================
*/

#include "ez80-vdp.h"
#include "v99x8-hdmi/v99x8-wolf3d.h"
#include <sys/types.h>
#include <unistd.h>

#include "wl_def.h"

#include "id_mm.h"

#include "id_ca.h"

#define THREEBYTEGRSTARTS

/*
=============================================================================

                                                         LOCAL CONSTANTS

=============================================================================
*/

#pragma pack(push, 1)
typedef struct {
  word    RLEWtag;
  int32_t headeroffsets[100];
} mapfiletype;
#pragma pack(pop)

/*
=============================================================================

                                                         GLOBAL VARIABLES

=============================================================================
*/

#define BUFFERSIZE 0x1000
static int32_t bufferseg[BUFFERSIZE / 4];

int mapon;

word           *mapsegs[MAPPLANES];
static maptype *mapheaderseg[NUMMAPS];
byte           *audiosegs[NUMSNDCHUNKS];
byte           *grsegs[NUMCHUNKS];

word RLEWtag;

int numEpisodesMissing = 0;

/*
=============================================================================

                                                         LOCAL VARIABLES

=============================================================================
*/

char              extension[5]; // Need a string, not constant to change cache files
char              graphext[5];
char              audioext[5];
static const char gheadname[] = "vgahead.";
static const char gfilename[] = "vgagraph.";
static const char gdictname[] = "vgadict.";
static const char mheadname[] = "maphead.";

#ifdef SOUND_ENABLED
static const char aheadname[] = "audiohed.";
static const char afilename[] = "audiot.";
#endif

void CA_CannotOpen(const char *string);

static int32_t grstarts[NUMCHUNKS + 1];
#ifdef SOUND_ENABLED
static int32_t *audiostarts; // array of offsets in audio / audiot
#endif

#ifdef GRHEADERLINKED
huffnode *grhuffman;
#else
huffnode grhuffman[255];
#endif

int grhandle    = -1; // handle to EGAGRAPH
int maphandle   = -1; // handle to MAPTEMP / GAMEMAPS
int audiohandle = -1; // handle to AUDIOT / AUDIO

int32_t chunkcomplen, chunkexplen;

SDMode oldsoundmode;

static int32_t GRFILEPOS(const size_t idx) {
  assert(idx < lengthof(grstarts));
  return grstarts[idx];
}

/*
=============================================================================

                                                        LOW LEVEL ROUTINES

=============================================================================
*/

/*
============================
=
= CAL_GetGrChunkLength
=
= Gets the length of an explicit length chunk (not tiles)
= The file pointer is positioned so the compressed data can be read in next.
=
============================
*/

void CAL_GetGrChunkLength(int chunk) {
  lseek(grhandle, GRFILEPOS(chunk), SEEK_SET);
  read(grhandle, &chunkexplen, sizeof(chunkexplen));
  chunkcomplen = GRFILEPOS(chunk + 1) - GRFILEPOS(chunk) - 4;
}

/*
==========================
=
= CA_WriteFile
=
= Writes a file from a memory buffer
=
==========================
*/

boolean CA_WriteFile(const char *filename, void *ptr, int32_t length) {
  const int handle = open(filename, O_CREAT | O_WRONLY | O_BINARY, 0644);
  printf("Save file %s\r\n", filename);

  if (handle == -1)
    return false;

  if (!write(handle, ptr, length)) {
    close(handle);
    return false;
  }
  close(handle);
  return true;
}

/*
==========================
=
= CA_LoadFile
=
= Allocate space for and load a file
=
==========================
*/

boolean CA_LoadFile(const char *filename, memptr *ptr) {
  int32_t size;

  printf("Load File: %s\r\n", filename);
  const int handle = open(filename, O_RDONLY | O_BINARY);
  if (handle == -1)
    return false;

  size = lseek(handle, 0, SEEK_END);
  lseek(handle, 0, SEEK_SET);
  MM_GetPtr((memptr *)&ptr, size);
  if (!read(handle, *ptr, size)) {
    close(handle);
    return false;
  }
  close(handle);
  return true;
}

/*
======================
=
= CAL_CarmackExpand
=
= Length is the length of the EXPANDED data
=
======================
*/

#define NEARTAG 0xa7
#define FARTAG  0xa8

void CAL_CarmackExpand(byte *source, word *dest, int length) {
  word  ch, chhigh, count, offset;
  byte *inptr;
  word *copyptr, *outptr;

  length /= 2;

  inptr  = (byte *)source;
  outptr = dest;

  while (length > 0) {
    ch     = READWORD(&inptr);
    chhigh = ch >> 8;
    if (chhigh == NEARTAG) {
      count = ch & 0xff;
      if (!count) { // have to insert a word containing the tag byte
        ch |= *inptr++;
        *outptr++ = ch;
        length--;
      } else {
        offset  = *inptr++;
        copyptr = outptr - offset;
        length -= count;
        if (length < 0)
          return;
        while (count--)
          *outptr++ = *copyptr++;
      }
    } else if (chhigh == FARTAG) {
      count = ch & 0xff;
      if (!count) { // have to insert a word containing the tag byte
        ch |= *inptr++;
        *outptr++ = ch;
        length--;
      } else {
        offset  = READWORD(&inptr);
        copyptr = dest + offset;
        length -= count;
        if (length < 0)
          return;
        while (count--)
          *outptr++ = *copyptr++;
      }
    } else {
      *outptr++ = ch;
      length--;
    }
  }
}

/*
======================
=
= CA_RLEWcompress
=
======================
*/

int32_t CA_RLEWCompress(word *source, int32_t length, word *dest, word rlewtag) {
  word     value, count;
  unsigned i;
  word    *start, *end;

  start = dest;

  end = source + (length + 1) / 2;

  //
  // compress it
  //
  do {
    count = 1;
    value = *source++;
    while (*source == value && source < end) {
      count++;
      source++;
    }
    if (count > 3 || value == rlewtag) {
      //
      // send a tag / count / value string
      //
      *dest++ = rlewtag;
      *dest++ = count;
      *dest++ = value;
    } else {
      //
      // send word without compressing
      //
      for (i = 1; i <= count; i++)
        *dest++ = value;
    }
  } while (source < end);

  return (int32_t)(2 * (dest - start));
}

/*
======================
=
= CA_RLEWexpand
= length is EXPANDED length
=
======================
*/

void CA_RLEWexpand(word *source, word *dest, int32_t length, word rlewtag) {
  word  value, count, i;
  word *end = dest + length / 2;

  //
  // expand it
  //
  do {
    value = *source++;
    if (value != rlewtag)
      //
      // uncompressed
      //
      *dest++ = value;
    else {
      //
      // compressed string
      //
      count = *source++;
      value = *source++;
      for (i = 1; i <= count; i++)
        *dest++ = value;
    }
  } while (dest < end);
}

/*
=============================================================================

                                                                                 CACHE MANAGER ROUTINES

=============================================================================
*/

/*
======================
=
= CAL_SetupGrFile
=
======================
*/

void CAL_SetupGrFile(void) {
  char  fname[13];
  int   handle;
  byte *compseg;

#ifdef GRHEADERLINKED

  grhuffman = (huffnode *)&EGAdict;
  grstarts  = (int32_t _seg *)FP_SEG(&EGAhead);

#else

  //
  // load ???dict.ext (huffman dictionary for graphics files)
  //

  strcpy(fname, gdictname);
  strcat(fname, graphext);

  handle = open(fname, O_RDONLY | O_BINARY);
  printf("Open file %s\r\n", fname);

  if (handle == -1)
    CA_CannotOpen(fname);

  read(handle, grhuffman, sizeof(grhuffman));
  close(handle);

  // load the data offsets from ???head.ext
  strcpy(fname, gheadname);
  strcat(fname, graphext);

  handle = open(fname, O_RDONLY | O_BINARY);
  printf("Open file %s\r\n", fname);

  if (handle == -1)
    CA_CannotOpen(fname);

  //   long headersize = lseek(handle, 0, SEEK_END);
  //   lseek(handle, 0, SEEK_SET);

  // #ifndef APOGEE_1_0
  //   int expectedsize = lengthof(grstarts) - numEpisodesMissing;
  // #else
  //   int expectedsize = lengthof(grstarts);
  // #endif

  //   if (!param_ignorenumchunks && headersize / 3 != expectedsize)
  //     Quit("Wolf3D was not compiled for these data files:\r\n"
  //          "%s contains a wrong number of offsets (%d instead of %d)!\r\n\n"
  //          "Please check whether you are using the right executable!\r\n"
  //          "(For mod developers: perhaps you forgot to update NUMCHUNKS?)",
  //          fname, (int)headersize / 3, (int)expectedsize);

  byte data[lengthof(grstarts) * 3];
  read(handle, data, sizeof(data));
  close(handle);

  const byte *d = data;
  for (int32_t *i = grstarts; i != endof(grstarts); ++i) {
    const int32_t val = d[0] | d[1] << 8 | d[2] << 16;
    *i                = (val == 0x00FFFFFF ? -1 : val);
    d += 3;
  }
#endif

  //
  // Open the graphics file
  //
  strcpy(fname, gfilename);
  strcat(fname, graphext);

  grhandle = open(fname, O_RDONLY | O_BINARY);
  printf("Open file %s\r\n", fname);

  if (grhandle == -1)
    CA_CannotOpen(fname);

  //
  // load the pic and sprite headers into the arrays in the data segment
  //
  MM_GetPtr((memptr *)&pictable, NUMPICS * sizeof(pictabletype));
  CAL_GetGrChunkLength(STRUCTPIC); // position file pointer

  MM_GetPtr((memptr *)&compseg, chunkcomplen);
  read(grhandle, compseg, chunkcomplen);
  CAL_HuffExpand(compseg, (byte *)pictable, NUMPICS * sizeof(pictabletype) /*, grhuffman*/);
  MM_FreePtr((memptr *)&compseg);
}

//==========================================================================

/*
======================
=
= CAL_SetupMapFile
=
======================
*/

void CAL_SetupMapFile(void) {
  int     i;
  int     handle;
  int32_t length, pos;
  char    fname[13];

  //
  // load maphead.ext (offsets and tileinfo for map file)
  //
  strcpy(fname, mheadname);
  strcat(fname, extension);

  handle = open(fname, O_RDONLY | O_BINARY);
  printf("Open file %s\r\n", fname);

  if (handle == -1)
    CA_CannotOpen(fname);

  length = NUMMAPS * 4 + 2; // used to be "filelength(handle);"
  mapfiletype *tinf;
  MM_GetPtr((memptr *)&tinf, sizeof(mapfiletype));

  read(handle, tinf, length);
  close(handle);

  RLEWtag = tinf->RLEWtag;

  //
  // open the data file
  //
#ifdef CARMACIZED
  strcpy(fname, "gamemaps.");
  strcat(fname, extension);

  maphandle = open(fname, O_RDONLY | O_BINARY);
  if (maphandle == -1)
    CA_CannotOpen(fname);
#else
  strcpy(fname, mfilename);
  strcat(fname, extension);

  maphandle = open(fname, O_RDONLY | O_BINARY);
  printf("Open file %s\r\n", fname);

  if (maphandle == -1)
    CA_CannotOpen(fname);
#endif

  //
  // load all map header
  //
  for (i = 0; i < NUMMAPS; i++) {
    // if mapfiletype not pragma packed, member headeroffsets will access incorrect memory
    pos = tinf->headeroffsets[i];
    if (pos < 0) // $FFFFFFFF start is a sparse map
      continue;

    MM_GetPtr((memptr *)&mapheaderseg[i], sizeof(maptype));
    lseek(maphandle, pos, SEEK_SET);
    read(maphandle, (memptr)mapheaderseg[i], sizeof(maptype));
  }

  MM_FreePtr((memptr *)&tinf);

  //
  // allocate space for 3 64*64 planes
  //
  for (i = 0; i < MAPPLANES; i++) {
    MM_GetPtr((memptr *)&mapsegs[i], maparea * 2);
  }
}

//==========================================================================

/*
======================
=
= CAL_SetupAudioFile
=
======================
*/

#ifdef SOUND_ENABLED
void CAL_SetupAudioFile(void) {
  char fname[13];

  //
  // load audiohed.ext (offsets for audio file)
  //
  strcpy(fname, aheadname);
  strcat(fname, audioext);

  void *ptr;
  if (!CA_LoadFile(fname, &ptr))
    CA_CannotOpen(fname);
  audiostarts = (int32_t *)ptr;

  //
  // open the data file
  //
  strcpy(fname, afilename);
  strcat(fname, audioext);

  audiohandle = open(fname, O_RDONLY | O_BINARY);
  printf("Open file %s\r\n", fname);

  if (audiohandle == -1)
    CA_CannotOpen(fname);
}
#endif

//==========================================================================

/*
======================
=
= CA_Startup
=
= Open all files and load in headers
=
======================
*/

void CA_Startup(void) {
#ifdef PROFILE
  unlink("PROFILE.TXT");
  profilehandle = open("PROFILE.TXT", O_CREAT | O_WRONLY | O_TEXT);
  printf("Open file PROFILE.TXT\r\n");

#endif

  CAL_SetupMapFile();
  CAL_SetupGrFile();
#ifdef SOUND_ENABLED
  CAL_SetupAudioFile();
#endif
  mapon = -1;
}

//==========================================================================

/*
======================
=
= CA_Shutdown
=
= Closes all files
=
======================
*/

void CA_Shutdown(void) {
  int i, start;

  if (maphandle != -1)
    close(maphandle);
  if (grhandle != -1)
    close(grhandle);
  if (audiohandle != -1)
    close(audiohandle);

  for (i = 0; i < NUMCHUNKS; i++)
    UNCACHEGRCHUNK(i);
  MM_FreePtr((memptr *)&pictable);

  switch (oldsoundmode) {
  case sdm_Off:
    return;
  case sdm_PC:
    start = STARTPCSOUNDS;
    break;
  case sdm_AdLib:
    start = STARTADLIBSOUNDS;
    break;
  }

  for (i = 0; i < NUMSOUNDS; i++, start++)
    UNCACHEAUDIOCHUNK(start);
}

//===========================================================================

/*
======================
=
= CA_CacheAudioChunk
=
======================
*/
#ifdef SOUND_ENABLED
int32_t CA_CacheAudioChunk(int chunk) {
  int32_t pos  = audiostarts[chunk];
  int32_t size = audiostarts[chunk + 1] - pos;

  if (audiosegs[chunk])
    return size; // already in memory

  MM_GetPtr((memptr *)&audiosegs[chunk], size);

  lseek(audiohandle, pos, SEEK_SET);
  read(audiohandle, audiosegs[chunk], size);

  return size;
}

void CA_CacheAdlibSoundChunk(int chunk) {
  int32_t pos  = audiostarts[chunk];
  int32_t size = audiostarts[chunk + 1] - pos;

  if (audiosegs[chunk])
    return; // already in memory

  lseek(audiohandle, pos, SEEK_SET);
  read(audiohandle, bufferseg, ORIG_ADLIBSOUND_SIZE - 1); // without data[1]

  AdLibSound *sound;
  MM_GetPtr((memptr *)&sound, size + sizeof(AdLibSound) - ORIG_ADLIBSOUND_SIZE);

  byte *ptr              = (byte *)bufferseg;
  sound->common.length   = READLONGWORD(&ptr);
  sound->common.priority = READWORD(&ptr);
  sound->inst.mChar      = *ptr++;
  sound->inst.cChar      = *ptr++;
  sound->inst.mScale     = *ptr++;
  sound->inst.cScale     = *ptr++;
  sound->inst.mAttack    = *ptr++;
  sound->inst.cAttack    = *ptr++;
  sound->inst.mSus       = *ptr++;
  sound->inst.cSus       = *ptr++;
  sound->inst.mWave      = *ptr++;
  sound->inst.cWave      = *ptr++;
  sound->inst.nConn      = *ptr++;
  sound->inst.voice      = *ptr++;
  sound->inst.mode       = *ptr++;
  sound->inst.unused[0]  = *ptr++;
  sound->inst.unused[1]  = *ptr++;
  sound->inst.unused[2]  = *ptr++;
  sound->block           = *ptr++;

  read(audiohandle, sound->data, size - ORIG_ADLIBSOUND_SIZE + 1); // + 1 because of byte data[1]

  audiosegs[chunk] = (byte *)sound;
}
#endif

//===========================================================================

/*
======================
=
= CA_LoadAllSounds
=
= Purges all sounds, then loads all new ones (mode switch)
=
======================
*/
#ifdef SOUND_ENABLED
void CA_LoadAllSounds(void) {
  unsigned start, i;

  switch (oldsoundmode) {
  case sdm_Off:
    goto cachein;
  case sdm_PC:
    start = STARTPCSOUNDS;
    break;
  case sdm_AdLib:
    start = STARTADLIBSOUNDS;
    break;
  }

  for (i = 0; i < NUMSOUNDS; i++, start++)
    UNCACHEAUDIOCHUNK(start);

cachein:

  oldsoundmode = SoundMode;

  switch (SoundMode) {
  case sdm_Off:
    start = STARTADLIBSOUNDS; // needed for priorities...
    break;
  case sdm_PC:
    start = STARTPCSOUNDS;
    break;
  case sdm_AdLib:
    start = STARTADLIBSOUNDS;
    break;
  }

  if (start == STARTADLIBSOUNDS) {
    for (i = 0; i < NUMSOUNDS; i++, start++)
      CA_CacheAdlibSoundChunk(start);
  } else {
    for (i = 0; i < NUMSOUNDS; i++, start++)
      CA_CacheAudioChunk(start);
  }
}
#endif
//===========================================================================

/*
======================
=
= CAL_ExpandGrChunk
=
= Does whatever is needed with a pointer to a compressed chunk
=
======================
*/

void CAL_ExpandGrChunk(int chunk, int32_t *source) {
  uint24_t expanded;

  if (chunk >= STARTTILE8 && chunk < STARTEXTERNS) {
    //
    // expanded sizes of tile8/16/32 are implicit
    //

#define BLOCK     64
#define MASKBLOCK 128

    if (chunk < STARTTILE8M) // tile 8s are all in one chunk!
      expanded = BLOCK * NUMTILE8;
    else if (chunk < STARTTILE16)
      expanded = MASKBLOCK * NUMTILE8M;
    else if (chunk < STARTTILE16M) // all other tiles are one/chunk
      expanded = BLOCK * 4;
    else if (chunk < STARTTILE32)
      expanded = MASKBLOCK * 4;
    else if (chunk < STARTTILE32M)
      expanded = BLOCK * 16;
    else
      expanded = MASKBLOCK * 16;
  } else {
    //
    // everything else has an explicit size longword
    //
    expanded = *source++;
  }

  //
  // allocate final space, decompress it, and free bigbuffer
  // Sprites need to have shifts made and various other junk
  //
  MM_GetPtr((memptr *)&grsegs[chunk], expanded);
  CAL_HuffExpand((byte *)source, grsegs[chunk], expanded /*, grhuffman*/);
}

/*
======================
=
= CA_CacheGrChunk
=
= Makes sure a given chunk is in memory, loading it if needed
=
======================
*/

void CA_CacheGrChunk(int chunk) {
  int32_t  pos, compressed;
  int32_t *source;
  int      next;

  if (grsegs[chunk])
    return; // already in memory

  //
  // load the chunk into a buffer, either the miscbuffer if it fits, or allocate
  // a larger buffer
  //

  pos = GRFILEPOS(chunk);

  if (pos < 0) // $FFFFFFFF start is a sparse tile
    return;

  next = chunk + 1;
  while (GRFILEPOS(next) == -1) // skip past any sparse tiles
    next++;

  compressed = GRFILEPOS(next) - pos;

  lseek(grhandle, pos, SEEK_SET);

  if (compressed <= BUFFERSIZE) {
    read(grhandle, bufferseg, compressed);
    source = bufferseg;
  } else {
    MM_GetPtr((memptr *)&source, compressed);
    read(grhandle, source, compressed);
  }

  CAL_ExpandGrChunk(chunk, source);

  if (compressed > BUFFERSIZE)
    MM_FreePtr((memptr *)&source);
}

//==========================================================================

/*
======================
=
= CA_CacheScreen
=
= Decompresses a chunk from disk straight onto the screen
=
======================
// */
void CA_CacheScreen(int chunk, uint8_t clip_top) {
  int32_t  pos, compressed, expanded;
  memptr   bigbufferseg;
  int32_t *source;
  int      next;

  //
  // load the chunk into a buffer
  //
  pos  = GRFILEPOS(chunk);
  next = chunk + 1;
  while (GRFILEPOS(next) == -1) // skip past any sparse tiles
    next++;
  compressed = GRFILEPOS(next) - pos;
  lseek(grhandle, pos, SEEK_SET);

  MM_GetPtr(&bigbufferseg, compressed);
  read(grhandle, bigbufferseg, compressed);
  source = (int32_t *)bigbufferseg;

  expanded = *source++;

  //
  // allocate final space, decompress it, and free bigbuffer
  // Sprites need to have shifts made and various other junk
  //
  byte *pic;
  MM_GetPtr((memptr *)&pic, 320 * 200);

  CAL_HuffExpand((byte *)source, pic, expanded /*, grhuffman*/);

#define CLIP_LEFT 8
#define CLIP_SKIP 5

  vdp_scn_write_init();

  for (int y = 0; y < 192; y++) {
    int cx = CLIP_LEFT;

    for (int x = 0; x < 256; x++) {
      const byte col = gamepal[pic[((y + clip_top) * 80 + (cx >> 2)) + (cx & 3) * 80 * 200]];
      vdp_scn_write_pixel(col, 0, 256, SCREEN_WIDTH, SCREEN_HEIGHT);

      cx++;
      if (x % CLIP_SKIP == 0)
        cx++;
    }
  }
  MM_FreePtr((memptr *)&pic);
  MM_FreePtr((memptr *)&bigbufferseg);

  vdp_scn_copy_y(0, 256, 0, SCREEN_HEIGHT);
}

//==========================================================================

/*
======================
=
= CA_CacheMap
=
= WOLF: This is specialized for a 64*64 map size
=
======================
*/

void CA_CacheMap(int mapnum) {
  int32_t  pos, compressed;
  int      plane;
  word    *dest;
  memptr   bigbufferseg;
  unsigned size;
  word    *source;
#ifdef CARMACIZED
  word   *buffer2seg;
  int32_t expanded;
#endif

  mapon = mapnum;

  //
  // load the planes into the allready allocated buffers
  //
  size = maparea * 2;

  for (plane = 0; plane < MAPPLANES; plane++) {
    pos        = mapheaderseg[mapnum]->planestart[plane];
    compressed = mapheaderseg[mapnum]->planelength[plane];

    dest = mapsegs[plane];

    lseek(maphandle, pos, SEEK_SET);
    if (compressed <= BUFFERSIZE)
      source = (word *)bufferseg;
    else {
      MM_GetPtr(&bigbufferseg, compressed);
      source = (word *)bigbufferseg;
    }

    read(maphandle, source, compressed);
#ifdef CARMACIZED
    //
    // unhuffman, then unRLEW
    // The huffman'd chunk has a two byte expanded length first
    // The resulting RLEW chunk also does, even though it's not really
    // needed
    //
    expanded = *source;
    source++;
    MM_GetPtr((memptr *)&buffer2seg, expanded);
    CAL_CarmackExpand((byte *)source, buffer2seg, expanded);
    CA_RLEWexpand(buffer2seg + 1, dest, size, RLEWtag);
    MM_FreePtr((memptr *)&buffer2seg);

#else
    //
    // unRLEW, skipping expanded length
    //
    CA_RLEWexpand(source + 1, dest, size, RLEWtag);
#endif

    if (compressed > BUFFERSIZE)
      MM_FreePtr((memptr *)&bigbufferseg);
  }
}

//===========================================================================

void CA_CannotOpen(const char *string) {
  char str[30];

  strcpy(str, "Can't open ");
  strcat(str, string);
  strcat(str, "!\n");
  Quit(str);
}
