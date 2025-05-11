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

int SDL_LockSurface(SDL_Surface *surface IG) { return 0; }

int SDL_FillRect(SDL_Surface *dst IG, const SDL_Rect *rect IG, uint32_t color IG) { return 0; }

int SDL_Init(int32_t flags IG) { return 0; }

const char *SDL_GetError(void) { return NULL; }

#ifdef JOYSTICK_SUPPORT
int SDL_NumJoysticks(void) { return 0; }
#endif

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
