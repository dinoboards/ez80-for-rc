#include <cpm.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <v99x8.h>

#define LEFT   0
#define RIGHT  255
#define TOP    0
#define BOTTOM (212)
#define WIDTH  (RIGHT - LEFT)
#define HEIGHT (BOTTOM - TOP)

void erase_page_0() { vdp_cmd_vdp_to_vram(LEFT, TOP, WIDTH, HEIGHT, 0, 0); }

void erase_page_1() { vdp_cmd_vdp_to_vram(LEFT, TOP + 256, WIDTH, HEIGHT, 0, 0); }

uint8_t current_page = 0;

void erase_current_page() {
  if (current_page == 0) {
    erase_page_0();
  } else {
    erase_page_1();
  }
}

void page_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color) {
  if (current_page == 0) {
    vdp_draw_line(x1 + LEFT, y1 + TOP, (x2), y2, color, CMD_LOGIC_IMP);
  } else {
    vdp_draw_line(x1 + LEFT, y1 + TOP + 256, x2, y2 + 256, color, CMD_LOGIC_IMP);
  }
}

void swap_page() {
  if (current_page == 0) {
    current_page = 1;
    vdp_set_page(0);
  } else {
    current_page = 0;
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

#define cx   (WIDTH / 2)
#define cy   (HEIGHT / 2)
#define cz   (0)
#define size (HEIGHT / 4)

#define VERTICES 8
POINT3D vertices[VERTICES] = {{cx - size, cy - size, cz - size}, {cx + size, cy - size, cz - size},
                              {cx + size, cy + size, cz - size}, {cx - size, cy + size, cz - size},
                              {cx - size, cy - size, cz + size}, {cx + size, cy - size, cz + size},
                              {cx + size, cy + size, cz + size}, {cx - size, cy + size, cz + size}};

#define EDGES 12
uint8_t edges[EDGES][2] = {
    {0, 1}, {1, 2}, {2, 3}, {3, 0}, // back face
    {4, 5}, {5, 6}, {6, 7}, {7, 4}, // front face
    {0, 4}, {1, 5}, {2, 6}, {3, 7}  // connecting sides
};

int24_t timeDelta, timeLast = 0;

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
    page_draw_line(vertices[edge[0]].x, vertices[edge[0]].y, vertices[edge[1]].x, vertices[edge[1]].y, 255);
  }

  swap_page();
}

int main(/*const int argc, const char *argv[]*/) {

  vdp_set_mode(7, 192, PAL);
  vdp_erase_bank0(0);
  vdp_erase_bank1(0);
  erase_page_0();
  erase_page_1();

  vdp_set_page(0);

  uint24_t time = 0;
  while (true) {
    if (cpm_rawio() != 0)
      return 0;
    draw_cube(time);
    time += 10;
  }
}
