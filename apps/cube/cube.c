#include "large_palette.h"
#include <cpm.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <v99x8-super.h>

#define LEFT   0
#define RIGHT  ((int24_t)vdp_get_screen_width())
#define TOP    0
#define BOTTOM ((int24_t)vdp_get_screen_height())
#define WIDTH  (RIGHT - LEFT)
#define HEIGHT (BOTTOM - TOP)

uint16_t page_height;
uint8_t  change_colour = true;
uint8_t  graphic_mode;
uint8_t  super_mode = false;

#define cx   (WIDTH / 2)
#define cy   (HEIGHT / 2)
#define cz   (0)
#define size (HEIGHT / 4)

// width: 256-0 = 256
// cx: width/2 = 128
// height: 192-0 = 192
// size = height / 4 = 192/4 = 48
// cx + size = 128-48

void wait_for_key() {
  uint8_t k = 0;
  while (k == 0) {
    k = cpm_c_rawio();
    if (k == 27)
      exit(0);
  }
}

uint8_t current_page = 0;

void erase_page_0() {
  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vdp_to_vram(0, 0, vdp_get_screen_width(), vdp_get_screen_height(), 0, 0, 0);
}

void erase_page_1() {
  vdp_cmd_wait_completion();
  vdp_cmd_logical_move_vdp_to_vram(0, page_height, vdp_get_screen_width(), vdp_get_screen_height(), 0, 0, 0);
}

void erase_current_page() {
  if (current_page == 0) {
    erase_page_0();
  } else {
    erase_page_1();
  }
}

void page_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color) {
  if (current_page == 0) {
    vdp_cmd_wait_completion();
    vdp_draw_line(x1, y1, (x2), y2, color, CMD_LOGIC_IMP);
  } else {
    vdp_cmd_wait_completion();
    vdp_draw_line(x1, y1 + page_height, x2, y2 + page_height, color, CMD_LOGIC_IMP);
  }
}

void swap_page() {
  if (current_page == 0) {
    current_page = 1;
    vdp_cmd_wait_completion();
    vdp_set_page(0);
  } else {
    current_page = 0;
    vdp_cmd_wait_completion();
    vdp_set_page(1);
  }
}

typedef struct {
  float x;
  float y;
  float z;
} POINT3D;

const float SPEED_X = 0.05; // rps
const float SPEED_Y = 0.15; // rps
const float SPEED_Z = 0.10; // rps

#define VERTICES 8
POINT3D vertices[VERTICES];

#define EDGES 12
uint8_t edges[EDGES][2] = {
    {0, 1}, {1, 2}, {2, 3}, {3, 0}, // back face
    {4, 5}, {5, 6}, {6, 7}, {7, 4}, // front face
    {0, 4}, {1, 5}, {2, 6}, {3, 7}  // connecting sides
};

int24_t timeDelta, timeLast = 0;

uint8_t colour = 1;

void draw_cube(int24_t timeNow) {

  // calculate the time difference
  timeDelta = timeNow - timeLast;
  timeLast  = timeNow;

  erase_current_page();

  // rotate the cube along the z axis
  float angle = timeDelta * 0.001 * SPEED_Z * M_PI * 2;
  for (int i = 0; i < VERTICES; i++) {
    POINT3D *v  = &vertices[i];
    float    dx = v->x - cx;
    float    dy = v->y - cy;
    float    x  = dx * cos(angle) - dy * sin(angle);
    float    y  = dx * sin(angle) + dy * cos(angle);
    v->x        = x + cx;
    v->y        = y + cy;
  }

  // rotate the cube along the x axis
  angle = timeDelta * 0.001 * SPEED_X * M_PI * 2;
  for (int i = 0; i < VERTICES; i++) {
    POINT3D *v  = &vertices[i];
    float    dy = v->y - cy;
    float    dz = v->z - cz;
    float    y  = dy * cos(angle) - dz * sin(angle);
    float    z  = dy * sin(angle) + dz * cos(angle);
    v->y        = y + cy;
    v->z        = z + cz;
  }

  // rotate the cube along the y axis
  angle = timeDelta * 0.001 * SPEED_Y * M_PI * 2;
  for (int i = 0; i < VERTICES; i++) {
    POINT3D *v  = &vertices[i];
    float    dx = v->x - cx;
    float    dz = v->z - cz;
    float    x  = dz * sin(angle) + dx * cos(angle);
    float    z  = dz * cos(angle) - dx * sin(angle);
    v->x        = x + cx;
    v->z        = z + cz;
  }

  // draw each edge
  for (int i = 0; i < EDGES; i++) {
    uint8_t *edge = edges[i];
    page_draw_line(vertices[edge[0]].x, vertices[edge[0]].y, vertices[edge[1]].x, vertices[edge[1]].y, colour);
  }
  if (change_colour)
    colour++;

  swap_page();
}

void init() {
  vertices[0] = (POINT3D){cx - size, cy - size, cz - size};
  vertices[1] = (POINT3D){cx + size, cy - size, cz - size};
  vertices[2] = (POINT3D){cx + size, cy + size, cz - size};
  vertices[3] = (POINT3D){cx - size, cy + size, cz - size};
  vertices[4] = (POINT3D){cx - size, cy - size, cz + size};
  vertices[5] = (POINT3D){cx + size, cy - size, cz + size};
  vertices[6] = (POINT3D){cx + size, cy + size, cz + size};
  vertices[7] = (POINT3D){cx - size, cy + size, cz + size};
}

char input[4];

void set_video_mode(void) {

retry:
  printf("Choose Graphic Mode\r\n");
  printf("a: V9958 Graphics Mode 6\n");
  printf("b: V9958 Graphics Mode 7\n");

#ifdef VDP_SUPER_HDMI
  printf("c: Super Graphics Mode 1\n");
  printf("d: Super Graphics Mode 2\n");
  printf("e: Super Graphics Mode 3\n");
  printf("f: Super Graphics Mode 4\n");
  printf("g: Super Graphics Mode 5\n");
  printf("h: Super Graphics Mode 6\n");
  printf("i: Super Graphics Mode 7\n");
  printf("j: Super Graphics Mode 8\n");
  printf("k: Super Graphics Mode 9\n");
#endif

  printf("\n?: ");
  fgets(input, 4, stdin);
  printf("\r\n");

  char sel = input[0];
  if (sel >= 'A' && sel <= 'Z')
    sel += 'a' - 'A'; // to lower

  switch (sel) {
  case 'a':
    vdp_set_graphic_6();
    break;
  case 'b':
    vdp_set_graphic_7();
    break;

#ifdef VDP_SUPER_HDMI
  case 'c':
    vdp_set_super_graphic_1();
    break;
  case 'd':
    vdp_set_super_graphic_2();
    break;
  case 'e':
    vdp_set_super_graphic_3();
    break;
  case 'f':
    vdp_set_super_graphic_4();
    break;
  case 'g':
    vdp_set_super_graphic_5();
    break;
  case 'h':
    vdp_set_super_graphic_6();
    break;
  case 'i':
    vdp_set_super_graphic_7();
    break;
  case 'j':
    vdp_set_super_graphic_8();
    break;
  case 'k':
    vdp_set_super_graphic_9();
    break;
#endif

  default:
    printf("Invalid selection. Try again.\n");
    goto retry;
  }

#ifdef VDP_SUPER_HDMI
  if (sel >= 'c') {
    graphic_mode = sel - 'c' + 1;
    vdp_set_extended_palette(large_palette);
    page_height   = ((int24_t)vdp_get_screen_height());
    change_colour = true;
    super_mode    = true;
  } else {
#endif
    graphic_mode = sel - 'a' + 1;
    vdp_set_lines(212);
    vdp_set_refresh(PAL);
    page_height   = 256;
    change_colour = false;
    colour        = 2;
    super_mode    = false;
#ifdef VDP_SUPER_HDMI
  }
#endif
}

int main(/*const int argc, const char *argv[]*/) {

  vdp_init();
  set_video_mode();

  printf("%sGraphics Mode %d (%d x %d)%s\r\n", (super_mode ? "Super " : ""), graphic_mode, vdp_get_screen_width(),
         vdp_get_screen_height(), (super_mode ? ", 256 Colours" : ""));

  init();

  erase_current_page();
  vdp_cmd_wait_completion();
  vdp_set_page(1);

  erase_current_page();
  vdp_cmd_wait_completion();
  vdp_set_page(0);

  uint24_t time = 0;
  while (true) {
    if (cpm_c_rawio() != 0)
      return 0;
    draw_cube(time);
    time += 10;
  }
}
