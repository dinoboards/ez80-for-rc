#ifndef __WOLFUTIL_H__
#define __WOLFUTIL_H__

extern void test_rendering_tiles();

#define LEFT   0
#define RIGHT  256
#define TOP    0
#define BOTTOM (212)
#define WIDTH  (RIGHT - LEFT)
#define HEIGHT (BOTTOM - TOP)

extern uint8_t signon[];

// typedef struct SDL_Color {
//   uint8_t r;
//   uint8_t g;
//   uint8_t b;
//   // uint8_t a;
// } SDL_Color;

typedef uint8_t SDL_Color;

// #define RGB(r, g, b)                                                                                                               \
//   { (r), (g), (b), 0 }

extern SDL_Color gamepal[];

extern void erase_page_0();

#endif
