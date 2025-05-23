#include "id_mm.h"
#include <ez80.h>

int ChunksInFile;
int PMSpriteStart;
int PMSoundStart;

bool PMSoundInfoPagePadded = false;

// holds the whole VSWAP
uint32_t *PMPageData;
size_t    PMPageDataSize;

// ChunksInFile+1 pointers to page starts.
// The last pointer points one byte after the last page.
uint8_t **PMPages __data_on_chip;

static uint32_t *pageOffsets;
static word     *pageLengths;
static FILE     *file;
static int       preloaded_upto = 0;
static uint8_t  *ptr            = NULL;
static uint8_t   progress       = 0;

void PM_Startup() {
  char fname[13] = "vswap.";
  strcat(fname, extension);

  printf("Reading file %s\r\n", fname);
  file = fopen(fname, "rb");
  if (!file)
    CA_CannotOpen(fname);

  ChunksInFile = 0;
  fread(&ChunksInFile, sizeof(word), 1, file);
  PMSpriteStart = 0;
  fread(&PMSpriteStart, sizeof(word), 1, file);
  PMSoundStart = 0;
  fread(&PMSoundStart, sizeof(word), 1, file);

  MM_GetPtr((memptr *)&pageOffsets, (ChunksInFile + 1) * sizeof(int32_t));
  fread(pageOffsets, sizeof(uint32_t), ChunksInFile, file);

  MM_GetPtr((memptr *)&pageLengths, ChunksInFile * sizeof(word));
  fread(pageLengths, sizeof(word), ChunksInFile, file);

  fseek(file, 0, SEEK_END);
  long fileSize     = ftell(file);
  long pageDataSize = fileSize - pageOffsets[0];
  if (pageDataSize > (size_t)-1)
    Quit("The page file \"%s\" is too large!", fname);

  pageOffsets[ChunksInFile] = fileSize;

  uint32_t dataStart = pageOffsets[0];
  int      i;

  // Check that all pageOffsets are valid
  for (i = 0; i < ChunksInFile; i++) {
    if (!pageOffsets[i])
      continue; // sparse page
    if (pageOffsets[i] < dataStart || pageOffsets[i] >= (size_t)fileSize)
      Quit("Illegal page offset for page %i: %u (filesize: %u)", i, pageOffsets[i], fileSize);
  }

  // Calculate total amount of padding needed for sprites and sound info page
  int alignPadding = 0;
  for (i = PMSpriteStart; i < PMSoundStart; i++) {
    if (!pageOffsets[i])
      continue; // sparse page
    uint32_t offs = pageOffsets[i] - dataStart + alignPadding;
    if (offs & 1)
      alignPadding++;
  }

  if ((pageOffsets[ChunksInFile - 1] - dataStart + alignPadding) & 1)
    alignPadding++;

  PMPageDataSize = (size_t)pageDataSize + alignPadding;
  MM_GetPtr((memptr *)&PMPageData, PMPageDataSize);

  MM_GetPtr((memptr *)&PMPages, (ChunksInFile + 1) * sizeof(uint8_t *));

  PM_Preload(NULL, true);
}
/* if partial, just load for a short period of time */
/* otherwise load everything */

void PM_Preload(chunk_load_progress_t update_fn, bool partial) {
  // Load pages and initialize PMPages pointers
  if (ptr == NULL)
    ptr = (uint8_t *)PMPageData;

  while (preloaded_upto < ChunksInFile) {
    if (preloaded_upto >= PMSpriteStart && (preloaded_upto < PMSoundStart || preloaded_upto == ChunksInFile - 1)) {
      size_t offs = ptr - (uint8_t *)PMPageData;

      // pad with zeros to make it 2-byte aligned
      if (offs & 1) {
        *ptr++ = 0;
        if (preloaded_upto == ChunksInFile - 1)
          PMSoundInfoPagePadded = true;
      }
    }

    PMPages[preloaded_upto] = ptr;

    if (!pageOffsets[preloaded_upto])
      continue; // sparse page

    // Use specified page length, when next page is sparse page.
    // Otherwise, calculate size from the offset difference between this and the next page.
    uint32_t size;
    if (!pageOffsets[preloaded_upto + 1])
      size = pageLengths[preloaded_upto];
    else
      size = pageOffsets[preloaded_upto + 1] - pageOffsets[preloaded_upto];

    if (update_fn) {
      const uint8_t new_p = (preloaded_upto * 100) / ChunksInFile;
      if (new_p != progress) {
        update_fn(new_p);
        progress = new_p;
      }
    }

    fseek(file, pageOffsets[preloaded_upto], SEEK_SET);
    fread(ptr, 1, size, file);
    ptr += size;
    preloaded_upto++;
    if (partial)
      break;
  }

  // last page points after page buffer
  PMPages[ChunksInFile] = ptr;
}

void PM_Shutdown() {
  // MM_FreePtr((memptr *)&pageLengths);
  // MM_FreePtr((memptr *)&pageOffsets);
  // MM_FreePtr((memptr *)&PMPages);
  // MM_FreePtr((memptr *)&PMPageData);
  fclose(file);
}
