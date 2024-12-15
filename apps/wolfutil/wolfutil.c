#include <cpm.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <v99x8.h>

#include "id_defs.h"

#include "id_mm.h"

#include "wolfutil.h"

void erase_page_0() { vdp_cmd_vdp_to_vram(LEFT, TOP, WIDTH, HEIGHT, 0x0, 0); }

uint8_t temp[320 * 200];

#define RED_FRM_GRB(grb)   ((grb & 0x1C) >> 2)
#define GREEN_FRM_GRB(grb) ((grb & 0xE0) >> 5)
#define BLUE_FRM_GRB(grb)  (grb & 0x03)

void show_help() {
  printf("Usage: wolfutil [options]\r\n");
  printf("Common utility for converting/porting wolf3d assets\r\n");
  printf("Options:\r\n");
  printf("  -I=SIGNON        Convert and create the SIGNON.IMG\r\n");
  printf("  -S=<filename>    Display a converted IMG file\r\n");
  printf("  -P=<swapfile>    Parse and test loading swap file\r\n");
  printf("  -T=MM            Test MM functions\r\n");
  printf("  -X               Test rendering tiles\r\n");
  printf("  -? /?            Show this help message\r\n");
}

bool argument_help(const char *arg) {
  if (strcmp(arg, "-?") == 0 || strcmp(arg, "/?") == 0) {
    show_help();
    exit(0);
  }

  return false;
}
typedef enum { CMD_NONE, CMD_HELP, CMD_I_SIGNON, CMD_S, CMD_P, CMD_T_MM, CMD_X } command_type_t;
static command_type_t cmd = CMD_NONE;
const char           *filename;

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

bool argument_X(const char *arg) {
  if (strcmp(arg, "-X") != 0 && strcmp(arg, "/X") != 0)
    return false;

  cmd = CMD_X;

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

    if (argument_X(argv[i]))
      continue;

    if (argument_help(argv[i]))
      continue;

    printf("Unknown argument: %s\r\n", argv[i]);
    show_help();
    return 1;
  }

#if 1
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
  } else if (cmd == CMD_X) {
    test_rendering_tiles();
  }

  return 0;
}
