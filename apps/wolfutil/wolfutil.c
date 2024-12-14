#include <cpm.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <v99x8.h>

#include "id_defs.h"

#include "id_mm.h"

#define LEFT   0
#define RIGHT  256
#define TOP    0
#define BOTTOM (212)
#define WIDTH  (RIGHT - LEFT)
#define HEIGHT (BOTTOM - TOP)

extern uint8_t signon[];

typedef struct SDL_Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} SDL_Color;

#define RGB(r, g, b)                                                                                                               \
  { (r), (g), (b), 0 }

SDL_Color gamepal[] = {
#include "wolfpal.inc"
};

void erase_page_0() { vdp_cmd_vdp_to_vram(LEFT, TOP, WIDTH, HEIGHT, 0, 0); }

uint8_t temp[320 * 200];

void VL_ConvertForV9958(uint8_t *surface) {

  // reduce the horizontal resolution of the image from 320x200 to 256x200

  const uint8_t *c     = surface;
  uint8_t       *dest  = surface;
  uint8_t        sum_r = 0;
  uint8_t        sum_g = 0;
  uint8_t        sum_b = 0;

  for (uint8_t y = 0; y < 200; y++) {
    for (uint16_t x = 0; x < 320; x++) {
      const uint8_t  i     = *c++;
      const uint16_t red   = (gamepal[i].r / 8);
      const uint16_t green = (gamepal[i].g / 8);
      const uint16_t blue  = (gamepal[i].b / 16);
      const uint8_t  rgb   = green << 5 | red << 2 | blue;

      const uint8_t s = x % 5;
      switch (s) {
      case 0:
      case 1:
        *dest++ = rgb;
        break;
      case 2:
        *dest++ = rgb;
        sum_r   = red;
        sum_g   = green;
        sum_b   = blue;
        break;

      case 3:
        sum_r += red;
        sum_g += green;
        sum_b += blue;
        break;

      case 4:
        sum_r = (sum_r + red) / 3;
        if (sum_r > 7)
          sum_r = 7;
        sum_g = (sum_g + green) / 3;
        if (sum_g > 7)
          sum_g = 7;
        sum_b = (sum_b + blue) / 3;
        if (sum_b > 3)
          sum_b = 3;
        *dest++ = (sum_g) << 5 | (sum_r) << 2 | (sum_b);
        break;
      }
    }
  }
}

void show_help() {
  printf("Usage: wolfutil [options]\r\n");
  printf("Common utility for converting/porting wolf3d assets\r\n");
  printf("Options:\r\n");
  printf("  -I=SIGNON        Convert and create the SIGNON.IMG\r\n");
  printf("  -S=<filename>    Display a converted IMG file\r\n");
  printf("  -P=<swapfile>    Parse and test loading swap file\r\n");
  printf("  -T=MM            Test MM functions\r\n");
  printf("  -? /?            Show this help message\r\n");
}

bool argument_help(const char *arg) {
  if (strcmp(arg, "-?") == 0 || strcmp(arg, "/?") == 0) {
    show_help();
    exit(0);
  }

  return false;
}
typedef enum { CMD_NONE, CMD_HELP, CMD_I_SIGNON, CMD_S, CMD_P, CMD_T_MM } command_type_t;
static command_type_t cmd = CMD_NONE;
static const char    *filename;

#if 0
void create_signon_image(void) {
  VL_ConvertForV9958(signon);

  FILE *f = fopen("SIGNON.IMG", "wb");
  fwrite(signon, 1, 256 * 200, f);
  fclose(f);

  printf("SIGNON.IMG image created\r\n");
}
#endif

void display_img_file() {

  uint8_t *buffer = malloc(256 * 200);
  if (buffer == NULL) {
    printf("Error: Unable to allocate buffer\r\n");
    return;
  }

  FILE *f = fopen(filename, "rb");
  if (f == NULL) {
    printf("Error: Unable to open file %s\r\n", filename);
    return;
  }

  int r = fread(buffer, 1, 256 * 200, f);
  if (r != 256 * 200) {
    printf("Error: Unable to read file %s\r\n", filename);
    fclose(f);
    return;
  }
  fclose(f);

  vdp_set_mode(7, 212, PAL);
  erase_page_0();

  vdp_cpu_to_vram0(buffer, 256 * 200);

  free(buffer);
}

void parse_test_swap_file() {
  FILE *f = fopen(filename, "rb");
  if (f == NULL) {
    printf("Error: Unable to open file %s\r\n", filename);
    return;
  }

  printf("TODO\r\n");
  fclose(f);
}

extern memptr bufferseg;

memptr block1;
memptr block2;
memptr block3;

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

  printf("bufferseg at: %p\r\n", bufferseg);
  printf("block1 at %p\r\n", block1);
  printf("block2 at %p\r\n", block2);
  printf("block3 at %p\r\n", block3);

  MM_Shutdown();
}

bool argument_I(const char *arg) {
  if (strcmp(arg, "-I=SIGNON") != 0 && strcmp(arg, "/I=SIGNON") != 0)
    return false;

  cmd = CMD_I_SIGNON;

  return true;
}

bool argument_S(const char *arg) {
  if (strncmp(arg, "-S=", 3) != 0 && strncmp(arg, "/S=", 3) != 0)
    return false;

  cmd      = CMD_S;
  filename = arg + 3;

  return true;
}

bool argument_P(const char *arg) {
  if (strncmp(arg, "-P=", 3) != 0 && strncmp(arg, "/P=", 3) != 0)
    return false;

  cmd      = CMD_P;
  filename = arg + 3;

  return true;
}

bool argument_T_MM(const char *arg) {
  if (strcmp(arg, "-T=MM") != 0 && strcmp(arg, "/T=MM") != 0)
    return false;

  cmd = CMD_T_MM;

  return true;
}

int main(const int argc, const char *argv[]) {

  const int heap_size = 0x400000 - (int)_heap;
  printf("Heap size: %d\r\n", heap_size);
  malloc_init(heap_size);

  void *p = malloc(1024 * 512);
  printf("malloc: %p\r\n", p);

  for (int i = 1; i < argc; i++) {
    if (argument_I(argv[i]))
      continue;

    if (argument_S(argv[i]))
      continue;

    if (argument_P(argv[i]))
      continue;

    if (argument_T_MM(argv[i]))
      continue;

    if (argument_help(argv[i]))
      continue;

    printf("Unknown argument: %s\r\n", argv[i]);
    show_help();
    return 1;
  }

#if 0
  if (cmd == CMD_I_SIGNON) {
    create_signon_image();
  } else
#endif

  if (cmd == CMD_S) {
    display_img_file();
  } else if (cmd == CMD_P) {
    parse_test_swap_file();
  } else if (cmd == CMD_T_MM) {
    test_mm_functions();
  }

  return 0;
}
