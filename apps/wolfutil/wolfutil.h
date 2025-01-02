#ifndef __WOLFUTIL_H__
#define __WOLFUTIL_H__

#include <stdint.h>

extern void test_rendering_tiles();
extern void display_img_file();
extern void create_signon_image();
extern void test_mm_functions();
extern void test_fixed_mul();
extern void test_key_serial_to_event(void);

#define LEFT   0
#define RIGHT  256
#define TOP    0
#define BOTTOM (212)
#define WIDTH  (RIGHT - LEFT)
#define HEIGHT (BOTTOM - TOP)

extern uint8_t     signon[];
extern const char *filename;

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
