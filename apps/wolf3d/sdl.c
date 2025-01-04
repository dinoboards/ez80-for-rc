#include <ez80-firmware.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sdl.h"

// empty function
// rewrite consumers of these functions
// to use some equivalent functions

#define IG __attribute__((unused))

#include <stdbool.h>
#include <stdint.h>

#include "wl_def.h"

#include "id_mm.h"

// convert original x [0-320] to scaled x [0-256]
const uint8_t scale_points[320] = {
    0,   0,   1,   2,   3,   4,   4,   5,   6,   7,   8,   8,   9,   10,  11,  12,  12,  13,  14,  15,  16,  16,  17,  18,  19,
    20,  20,  21,  22,  23,  24,  24,  25,  26,  27,  28,  28,  29,  30,  31,  32,  32,  33,  34,  35,  36,  36,  37,  38,  39,
    40,  40,  41,  42,  43,  44,  44,  45,  46,  47,  48,  48,  49,  50,  51,  52,  52,  53,  54,  55,  56,  56,  57,  58,  59,
    60,  60,  61,  62,  63,  64,  64,  65,  66,  67,  68,  68,  69,  70,  71,  72,  72,  73,  74,  75,  76,  76,  77,  78,  79,
    80,  80,  81,  82,  83,  84,  84,  85,  86,  87,  88,  88,  89,  90,  91,  92,  92,  93,  94,  95,  96,  96,  97,  98,  99,
    100, 100, 101, 102, 103, 104, 104, 105, 106, 107, 108, 108, 109, 110, 111, 112, 112, 113, 114, 115, 116, 116, 117, 118, 119,
    120, 120, 121, 122, 123, 124, 124, 125, 126, 127, 128, 128, 129, 130, 131, 132, 132, 133, 134, 135, 136, 136, 137, 138, 139,
    140, 140, 141, 142, 143, 144, 144, 145, 146, 147, 148, 148, 149, 150, 151, 152, 152, 153, 154, 155, 156, 156, 157, 158, 159,
    160, 160, 161, 162, 163, 164, 164, 165, 166, 167, 168, 168, 169, 170, 171, 172, 172, 173, 174, 175, 176, 176, 177, 178, 179,
    180, 180, 181, 182, 183, 184, 184, 185, 186, 187, 188, 188, 189, 190, 191, 192, 192, 193, 194, 195, 196, 196, 197, 198, 199,
    200, 200, 201, 202, 203, 204, 204, 205, 206, 207, 208, 208, 209, 210, 211, 212, 212, 213, 214, 215, 216, 216, 217, 218, 219,
    220, 220, 221, 222, 223, 224, 224, 225, 226, 227, 228, 228, 229, 230, 231, 232, 232, 233, 234, 235, 236, 236, 237, 238, 239,
    240, 240, 241, 242, 243, 244, 244, 245, 246, 247, 248, 248, 249, 250, 251, 252, 252, 253, 254, 255};

void SDL_Delay(uint32_t ms IG) {}

int SDL_LockSurface(SDL_Surface *surface IG) { return 0; }

int SDL_FillRect(SDL_Surface *dst IG, const SDL_Rect *rect IG, uint32_t color IG) { return 0; }

// uint32_t SDL_GetTicks(void) {
//   return = ez80_timers_ticks_get();

//   return milliseconds;
// }

int SDL_Init(int32_t flags IG) { return 0; }

const char *SDL_GetError(void) { return NULL; }

#ifdef JOYSTICK_SUPPORT
int SDL_NumJoysticks(void) { return 0; }
#endif

uint32_t SDL_GetRelativeMouseState(int *x IG, int *y IG) { return 0; }

uint32_t SDL_GetMouseState(int *x IG, int *y IG) { return 0; }

void SDL_GameControllerUpdate(void) {}

int16_t SDL_GameControllerGetAxis(SDL_GameController *gamecontroller IG, SDL_GameControllerAxis axis IG) { return 0; }

uint8_t SDL_GameControllerGetButton(SDL_GameController *gamecontroller IG, SDL_GameControllerButton button IG) { return 0; }

SDL_Keymod SDL_GetModState(void) { return 0; }

int SDL_WaitEvent(SDL_Event *event IG) { return 0; }

int SDL_PollEvent(SDL_Event *event IG) { return 0; }

int SDL_InitSubSystem(uint32_t flags IG) { return 0; }

bool SDL_IsGameController(int joystick_index IG) { return false; }

SDL_GameController *SDL_GameControllerOpen(int joystick_index IG) { return NULL; }

const char *SDL_GameControllerName(SDL_GameController *gamecontroller IG) { return NULL; }

int SDL_GameControllerEventState(int state IG) { return 0; }

uint8_t SDL_EventState(uint32_t type IG, int state IG) { return 0; }

int SDL_SetRelativeMouseMode(bool enabled IG) { return 0; }

void SDL_GameControllerClose(SDL_GameController *gamecontroller IG) {}

int SDL_UpperBlit(SDL_Surface *src IG, const SDL_Rect *srcrect IG, SDL_Surface *dst IG, SDL_Rect *dstrect IG) { return 0; }

SDL_Surface *SDL_CreateRGBSurface(/*uint32_t flags IG,*/
                                  int width      ,
                                  int height
                                  /*int depth      IG,
                                  uint32_t Rmask IG,
                                  uint32_t Gmask IG,
                                  uint32_t Bmask IG,
                                  uint32_t Amask IG*/) {

  int          size = sizeof(SDL_Surface) + width * height;
  SDL_Surface *surface;
  MM_GetPtr((memptr *)&surface, size);
  memset(surface->xpixels, 0, width * height);
  surface->w     = width;
  surface->pitch = width;
  surface->h     = height;
  return surface;
}

uint32_t SDL_MapRGB(/*const SDL_PixelFormat *format IG,*/ uint8_t r IG, uint8_t g IG, uint8_t b IG) { return 0; }

SDL_Texture *SDL_CreateTextureFromSurface(SDL_Renderer *renderer IG, SDL_Surface *surface IG) { return NULL; }

int SDL_RenderCopy(SDL_Renderer *renderer IG, SDL_Texture *texture IG, const SDL_Rect *srcrect IG, const SDL_Rect *dstrect IG) {
  return 0;
}

void SDL_RenderPresent(SDL_Renderer *renderer IG) {}

void SDL_DestroyTexture(SDL_Texture *texture IG) {}

const uint8_t *SDL_GetKeyboardState(int *numkeys IG) { return NULL; }

void Mix_FreeChunk(Mix_Chunk *chunk IG) {}

int Mix_HaltChannel(int channel IG) { return 0; }

int Mix_GroupAvailable(int tag IG) { return 0; }

int Mix_GroupOldest(int tag IG) { return 0; }

int Mix_SetPanning(int channel IG, uint8_t left IG, uint8_t right IG) { return 0; }

Mix_Chunk *Mix_LoadWAV_RW(SDL_RWops *src IG, int freesrc IG) { return NULL; }

int Mix_PlayChannel(int channel IG, Mix_Chunk *chunk IG, int loops IG) { return 0; }

int Mix_OpenAudio(int frequency IG, uint16_t format IG, int channels IG, int chunksize IG) { return 0; }

int Mix_ReserveChannels(int num IG) { return 0; }

int Mix_GroupChannels(int from IG, int to IG, int tag IG) { return 0; }

void Mix_ChannelFinished(Mix_ChannelFinishedCallback channel_finished IG) {}

void Mix_HookMusic(Mix_MixCallback mix_func IG, void *arg IG) {}

SDL_RWops *SDL_RWFromMem(void *mem IG, int size IG) { return NULL; }
