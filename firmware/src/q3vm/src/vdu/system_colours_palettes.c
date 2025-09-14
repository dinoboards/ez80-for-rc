
#include <v99x8.h>
#include <vdu-standards-colors.h>

static const RGB default_2_colour_palette[16]  = {RGB_BLACK, RGB_WHITE};
static const RGB default_4_colour_palette[16]  = {RGB_BLACK, RGB_RED, RGB_YELLOW, RGB_WHITE};
static const RGB default_16_colour_palette[16] = {RGB_BLACK,
                                                  RGB_RED,
                                                  RGB_GREEN,
                                                  RGB_YELLOW,
                                                  RGB_BLUE,
                                                  RGB_MAGENTA,
                                                  RGB_CYAN,
                                                  RGB_WHITE,
                                                  RGB_FLASHING_BLACK_WHITE,
                                                  RGB_FLASHING_RED_CYAN,
                                                  RGB_FLASHING_GREEN_MAGENTA,
                                                  RGB_FLASHING_YELLOW_BLUE,
                                                  RGB_FLASHING_BLUE_YELLOW,
                                                  RGB_FLASHING_MAGENTA_GREEN,
                                                  RGB_FLASHING_CYAN_RED,
                                                  RGB_FLASHING_WHITE_BLACK};
