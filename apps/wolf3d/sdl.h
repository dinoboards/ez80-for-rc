#ifndef SDL_H
#define SDL_H

#include <cdefs.h>
#include <ez80-firmware.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct SDL_Texture SDL_Texture;

typedef struct SDL_PixelFormat {
  uint8_t BytesPerPixel;
} SDL_PixelFormat;

typedef struct {
  uint16_t pitch;
  uint16_t w;
  uint16_t h;
  uint8_t  pixels[];
} SDL_Surface;

typedef void SDL_Window;
typedef void SDL_Renderer;
typedef void SDL_Rect;

struct _SDL_GameController;
typedef struct _SDL_GameController SDL_GameController;

typedef enum SDL_GameControllerAxis {
  SDL_CONTROLLER_AXIS_INVALID = -1,
  SDL_CONTROLLER_AXIS_LEFTX,
  SDL_CONTROLLER_AXIS_LEFTY,
  SDL_CONTROLLER_AXIS_RIGHTX,
  SDL_CONTROLLER_AXIS_RIGHTY,
  SDL_CONTROLLER_AXIS_TRIGGERLEFT,
  SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
  SDL_CONTROLLER_AXIS_MAX
} SDL_GameControllerAxis;

typedef enum SDL_GameControllerButton {
  SDL_CONTROLLER_BUTTON_INVALID = -1,
  SDL_CONTROLLER_BUTTON_A,
  SDL_CONTROLLER_BUTTON_B,
  SDL_CONTROLLER_BUTTON_X,
  SDL_CONTROLLER_BUTTON_Y,
  SDL_CONTROLLER_BUTTON_BACK,
  SDL_CONTROLLER_BUTTON_GUIDE,
  SDL_CONTROLLER_BUTTON_START,
  SDL_CONTROLLER_BUTTON_LEFTSTICK,
  SDL_CONTROLLER_BUTTON_RIGHTSTICK,
  SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
  SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
  SDL_CONTROLLER_BUTTON_DPAD_UP,
  SDL_CONTROLLER_BUTTON_DPAD_DOWN,
  SDL_CONTROLLER_BUTTON_DPAD_LEFT,
  SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
  SDL_CONTROLLER_BUTTON_MISC1, /* Xbox Series X share button, PS5 microphone button, Nintendo Switch Pro capture button, Amazon Luna
                                  microphone button */
  SDL_CONTROLLER_BUTTON_PADDLE1,  /* Xbox Elite paddle P1 (upper left, facing the back) */
  SDL_CONTROLLER_BUTTON_PADDLE2,  /* Xbox Elite paddle P3 (upper right, facing the back) */
  SDL_CONTROLLER_BUTTON_PADDLE3,  /* Xbox Elite paddle P2 (lower left, facing the back) */
  SDL_CONTROLLER_BUTTON_PADDLE4,  /* Xbox Elite paddle P4 (lower right, facing the back) */
  SDL_CONTROLLER_BUTTON_TOUCHPAD, /* PS4/PS5 touchpad button */
  SDL_CONTROLLER_BUTTON_MAX
} SDL_GameControllerButton;

/**
 * The SDL keyboard scancode representation.
 *
 * Values of this type are used to represent keyboard keys, among other places
 * in the SDL_Keysym::scancode key.keysym.scancode field of the SDL_Event
 * structure.
 *
 * The values in this enumeration are based on the USB usage page standard:
 * https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf
 */
typedef enum SDL_Scancode {
  SDL_SCANCODE_UNKNOWN = 0,

  /**
   *  \name Usage page 0x07
   *
   *  These values are from usage page 0x07 (USB keyboard page).
   */
  /* @{ */

  SDL_SCANCODE_A = 4,
  SDL_SCANCODE_B = 5,
  SDL_SCANCODE_C = 6,
  SDL_SCANCODE_D = 7,
  SDL_SCANCODE_E = 8,
  SDL_SCANCODE_F = 9,
  SDL_SCANCODE_G = 10,
  SDL_SCANCODE_H = 11,
  SDL_SCANCODE_I = 12,
  SDL_SCANCODE_J = 13,
  SDL_SCANCODE_K = 14,
  SDL_SCANCODE_L = 15,
  SDL_SCANCODE_M = 16,
  SDL_SCANCODE_N = 17,
  SDL_SCANCODE_O = 18,
  SDL_SCANCODE_P = 19,
  SDL_SCANCODE_Q = 20,
  SDL_SCANCODE_R = 21,
  SDL_SCANCODE_S = 22,
  SDL_SCANCODE_T = 23,
  SDL_SCANCODE_U = 24,
  SDL_SCANCODE_V = 25,
  SDL_SCANCODE_W = 26,
  SDL_SCANCODE_X = 27,
  SDL_SCANCODE_Y = 28,
  SDL_SCANCODE_Z = 29,

  SDL_SCANCODE_1 = 30,
  SDL_SCANCODE_2 = 31,
  SDL_SCANCODE_3 = 32,
  SDL_SCANCODE_4 = 33,
  SDL_SCANCODE_5 = 34,
  SDL_SCANCODE_6 = 35,
  SDL_SCANCODE_7 = 36,
  SDL_SCANCODE_8 = 37,
  SDL_SCANCODE_9 = 38,
  SDL_SCANCODE_0 = 39,

  SDL_SCANCODE_RETURN    = 40,
  SDL_SCANCODE_ESCAPE    = 41,
  SDL_SCANCODE_BACKSPACE = 42,
  SDL_SCANCODE_TAB       = 43,
  SDL_SCANCODE_SPACE     = 44,

  SDL_SCANCODE_MINUS        = 45,
  SDL_SCANCODE_EQUALS       = 46,
  SDL_SCANCODE_LEFTBRACKET  = 47,
  SDL_SCANCODE_RIGHTBRACKET = 48,
  SDL_SCANCODE_BACKSLASH    = 49, /**< Located at the lower left of the return
                                   *   key on ISO keyboards and at the right end
                                   *   of the QWERTY row on ANSI keyboards.
                                   *   Produces REVERSE SOLIDUS (backslash) and
                                   *   VERTICAL LINE in a US layout, REVERSE
                                   *   SOLIDUS and VERTICAL LINE in a UK Mac
                                   *   layout, NUMBER SIGN and TILDE in a UK
                                   *   Windows layout, DOLLAR SIGN and POUND SIGN
                                   *   in a Swiss German layout, NUMBER SIGN and
                                   *   APOSTROPHE in a German layout, GRAVE
                                   *   ACCENT and POUND SIGN in a French Mac
                                   *   layout, and ASTERISK and MICRO SIGN in a
                                   *   French Windows layout.
                                   */
  SDL_SCANCODE_NONUSHASH = 50,    /**< ISO USB keyboards actually use this code
                                   *   instead of 49 for the same key, but all
                                   *   OSes I've seen treat the two codes
                                   *   identically. So, as an implementor, unless
                                   *   your keyboard generates both of those
                                   *   codes and your OS treats them differently,
                                   *   you should generate SDL_SCANCODE_BACKSLASH
                                   *   instead of this code. As a user, you
                                   *   should not rely on this code because SDL
                                   *   will never generate it with most (all?)
                                   *   keyboards.
                                   */
  SDL_SCANCODE_SEMICOLON  = 51,
  SDL_SCANCODE_APOSTROPHE = 52,
  SDL_SCANCODE_GRAVE      = 53, /**< Located in the top left corner (on both ANSI
                                 *   and ISO keyboards). Produces GRAVE ACCENT and
                                 *   TILDE in a US Windows layout and in US and UK
                                 *   Mac layouts on ANSI keyboards, GRAVE ACCENT
                                 *   and NOT SIGN in a UK Windows layout, SECTION
                                 *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                                 *   layouts on ISO keyboards, SECTION SIGN and
                                 *   DEGREE SIGN in a Swiss German layout (Mac:
                                 *   only on ISO keyboards), CIRCUMFLEX ACCENT and
                                 *   DEGREE SIGN in a German layout (Mac: only on
                                 *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
                                 *   French Windows layout, COMMERCIAL AT and
                                 *   NUMBER SIGN in a French Mac layout on ISO
                                 *   keyboards, and LESS-THAN SIGN and GREATER-THAN
                                 *   SIGN in a Swiss German, German, or French Mac
                                 *   layout on ANSI keyboards.
                                 */
  SDL_SCANCODE_COMMA  = 54,
  SDL_SCANCODE_PERIOD = 55,
  SDL_SCANCODE_SLASH  = 56,

  SDL_SCANCODE_CAPSLOCK = 57,

  SDL_SCANCODE_F1  = 58,
  SDL_SCANCODE_F2  = 59,
  SDL_SCANCODE_F3  = 60,
  SDL_SCANCODE_F4  = 61,
  SDL_SCANCODE_F5  = 62,
  SDL_SCANCODE_F6  = 63,
  SDL_SCANCODE_F7  = 64,
  SDL_SCANCODE_F8  = 65,
  SDL_SCANCODE_F9  = 66,
  SDL_SCANCODE_F10 = 67,
  SDL_SCANCODE_F11 = 68,
  SDL_SCANCODE_F12 = 69,

  SDL_SCANCODE_PRINTSCREEN = 70,
  SDL_SCANCODE_SCROLLLOCK  = 71,
  SDL_SCANCODE_PAUSE       = 72,
  SDL_SCANCODE_INSERT      = 73, /**< insert on PC, help on some Mac keyboards (but
                                      does send code 73, not 117) */
  SDL_SCANCODE_HOME     = 74,
  SDL_SCANCODE_PAGEUP   = 75,
  SDL_SCANCODE_DELETE   = 76,
  SDL_SCANCODE_END      = 77,
  SDL_SCANCODE_PAGEDOWN = 78,
  SDL_SCANCODE_RIGHT    = 79,
  SDL_SCANCODE_LEFT     = 80,
  SDL_SCANCODE_DOWN     = 81,
  SDL_SCANCODE_UP       = 82,

  SDL_SCANCODE_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                   */
  SDL_SCANCODE_KP_DIVIDE   = 84,
  SDL_SCANCODE_KP_MULTIPLY = 85,
  SDL_SCANCODE_KP_MINUS    = 86,
  SDL_SCANCODE_KP_PLUS     = 87,
  SDL_SCANCODE_KP_ENTER    = 88,
  SDL_SCANCODE_KP_1        = 89,
  SDL_SCANCODE_KP_2        = 90,
  SDL_SCANCODE_KP_3        = 91,
  SDL_SCANCODE_KP_4        = 92,
  SDL_SCANCODE_KP_5        = 93,
  SDL_SCANCODE_KP_6        = 94,
  SDL_SCANCODE_KP_7        = 95,
  SDL_SCANCODE_KP_8        = 96,
  SDL_SCANCODE_KP_9        = 97,
  SDL_SCANCODE_KP_0        = 98,
  SDL_SCANCODE_KP_PERIOD   = 99,

  SDL_SCANCODE_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                      *   keyboards have over ANSI ones,
                                      *   located between left shift and Y.
                                      *   Produces GRAVE ACCENT and TILDE in a
                                      *   US or UK Mac layout, REVERSE SOLIDUS
                                      *   (backslash) and VERTICAL LINE in a
                                      *   US or UK Windows layout, and
                                      *   LESS-THAN SIGN and GREATER-THAN SIGN
                                      *   in a Swiss German, German, or French
                                      *   layout. */
  SDL_SCANCODE_APPLICATION = 101,    /**< windows contextual menu, compose */
  SDL_SCANCODE_POWER       = 102,    /**< The USB document says this is a status flag,
                                      *   not a physical key - but some Mac keyboards
                                      *   do have a power key. */
  SDL_SCANCODE_KP_EQUALS  = 103,
  SDL_SCANCODE_F13        = 104,
  SDL_SCANCODE_F14        = 105,
  SDL_SCANCODE_F15        = 106,
  SDL_SCANCODE_F16        = 107,
  SDL_SCANCODE_F17        = 108,
  SDL_SCANCODE_F18        = 109,
  SDL_SCANCODE_F19        = 110,
  SDL_SCANCODE_F20        = 111,
  SDL_SCANCODE_F21        = 112,
  SDL_SCANCODE_F22        = 113,
  SDL_SCANCODE_F23        = 114,
  SDL_SCANCODE_F24        = 115,
  SDL_SCANCODE_EXECUTE    = 116,
  SDL_SCANCODE_HELP       = 117, /**< AL Integrated Help Center */
  SDL_SCANCODE_MENU       = 118, /**< Menu (show menu) */
  SDL_SCANCODE_SELECT     = 119,
  SDL_SCANCODE_STOP       = 120, /**< AC Stop */
  SDL_SCANCODE_AGAIN      = 121, /**< AC Redo/Repeat */
  SDL_SCANCODE_UNDO       = 122, /**< AC Undo */
  SDL_SCANCODE_CUT        = 123, /**< AC Cut */
  SDL_SCANCODE_COPY       = 124, /**< AC Copy */
  SDL_SCANCODE_PASTE      = 125, /**< AC Paste */
  SDL_SCANCODE_FIND       = 126, /**< AC Find */
  SDL_SCANCODE_MUTE       = 127,
  SDL_SCANCODE_VOLUMEUP   = 128,
  SDL_SCANCODE_VOLUMEDOWN = 129,
  /* not sure whether there's a reason to enable these */
  /*     SDL_SCANCODE_LOCKINGCAPSLOCK = 130,  */
  /*     SDL_SCANCODE_LOCKINGNUMLOCK = 131, */
  /*     SDL_SCANCODE_LOCKINGSCROLLLOCK = 132, */
  SDL_SCANCODE_KP_COMMA       = 133,
  SDL_SCANCODE_KP_EQUALSAS400 = 134,

  SDL_SCANCODE_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                          footnotes in USB doc */
  SDL_SCANCODE_INTERNATIONAL2 = 136,
  SDL_SCANCODE_INTERNATIONAL3 = 137, /**< Yen */
  SDL_SCANCODE_INTERNATIONAL4 = 138,
  SDL_SCANCODE_INTERNATIONAL5 = 139,
  SDL_SCANCODE_INTERNATIONAL6 = 140,
  SDL_SCANCODE_INTERNATIONAL7 = 141,
  SDL_SCANCODE_INTERNATIONAL8 = 142,
  SDL_SCANCODE_INTERNATIONAL9 = 143,
  SDL_SCANCODE_LANG1          = 144, /**< Hangul/English toggle */
  SDL_SCANCODE_LANG2          = 145, /**< Hanja conversion */
  SDL_SCANCODE_LANG3          = 146, /**< Katakana */
  SDL_SCANCODE_LANG4          = 147, /**< Hiragana */
  SDL_SCANCODE_LANG5          = 148, /**< Zenkaku/Hankaku */
  SDL_SCANCODE_LANG6          = 149, /**< reserved */
  SDL_SCANCODE_LANG7          = 150, /**< reserved */
  SDL_SCANCODE_LANG8          = 151, /**< reserved */
  SDL_SCANCODE_LANG9          = 152, /**< reserved */

  SDL_SCANCODE_ALTERASE   = 153, /**< Erase-Eaze */
  SDL_SCANCODE_SYSREQ     = 154,
  SDL_SCANCODE_CANCEL     = 155, /**< AC Cancel */
  SDL_SCANCODE_CLEAR      = 156,
  SDL_SCANCODE_PRIOR      = 157,
  SDL_SCANCODE_RETURN2    = 158,
  SDL_SCANCODE_SEPARATOR  = 159,
  SDL_SCANCODE_OUT        = 160,
  SDL_SCANCODE_OPER       = 161,
  SDL_SCANCODE_CLEARAGAIN = 162,
  SDL_SCANCODE_CRSEL      = 163,
  SDL_SCANCODE_EXSEL      = 164,

  SDL_SCANCODE_KP_00              = 176,
  SDL_SCANCODE_KP_000             = 177,
  SDL_SCANCODE_THOUSANDSSEPARATOR = 178,
  SDL_SCANCODE_DECIMALSEPARATOR   = 179,
  SDL_SCANCODE_CURRENCYUNIT       = 180,
  SDL_SCANCODE_CURRENCYSUBUNIT    = 181,
  SDL_SCANCODE_KP_LEFTPAREN       = 182,
  SDL_SCANCODE_KP_RIGHTPAREN      = 183,
  SDL_SCANCODE_KP_LEFTBRACE       = 184,
  SDL_SCANCODE_KP_RIGHTBRACE      = 185,
  SDL_SCANCODE_KP_TAB             = 186,
  SDL_SCANCODE_KP_BACKSPACE       = 187,
  SDL_SCANCODE_KP_A               = 188,
  SDL_SCANCODE_KP_B               = 189,
  SDL_SCANCODE_KP_C               = 190,
  SDL_SCANCODE_KP_D               = 191,
  SDL_SCANCODE_KP_E               = 192,
  SDL_SCANCODE_KP_F               = 193,
  SDL_SCANCODE_KP_XOR             = 194,
  SDL_SCANCODE_KP_POWER           = 195,
  SDL_SCANCODE_KP_PERCENT         = 196,
  SDL_SCANCODE_KP_LESS            = 197,
  SDL_SCANCODE_KP_GREATER         = 198,
  SDL_SCANCODE_KP_AMPERSAND       = 199,
  SDL_SCANCODE_KP_DBLAMPERSAND    = 200,
  SDL_SCANCODE_KP_VERTICALBAR     = 201,
  SDL_SCANCODE_KP_DBLVERTICALBAR  = 202,
  SDL_SCANCODE_KP_COLON           = 203,
  SDL_SCANCODE_KP_HASH            = 204,
  SDL_SCANCODE_KP_SPACE           = 205,
  SDL_SCANCODE_KP_AT              = 206,
  SDL_SCANCODE_KP_EXCLAM          = 207,
  SDL_SCANCODE_KP_MEMSTORE        = 208,
  SDL_SCANCODE_KP_MEMRECALL       = 209,
  SDL_SCANCODE_KP_MEMCLEAR        = 210,
  SDL_SCANCODE_KP_MEMADD          = 211,
  SDL_SCANCODE_KP_MEMSUBTRACT     = 212,
  SDL_SCANCODE_KP_MEMMULTIPLY     = 213,
  SDL_SCANCODE_KP_MEMDIVIDE       = 214,
  SDL_SCANCODE_KP_PLUSMINUS       = 215,
  SDL_SCANCODE_KP_CLEAR           = 216,
  SDL_SCANCODE_KP_CLEARENTRY      = 217,
  SDL_SCANCODE_KP_BINARY          = 218,
  SDL_SCANCODE_KP_OCTAL           = 219,
  SDL_SCANCODE_KP_DECIMAL         = 220,
  SDL_SCANCODE_KP_HEXADECIMAL     = 221,

  SDL_SCANCODE_LCTRL  = 224,
  SDL_SCANCODE_LSHIFT = 225,
  SDL_SCANCODE_LALT   = 226, /**< alt, option */
  SDL_SCANCODE_LGUI   = 227, /**< windows, command (apple), meta */
  SDL_SCANCODE_RCTRL  = 228,
  SDL_SCANCODE_RSHIFT = 229,
  SDL_SCANCODE_RALT   = 230, /**< alt gr, option */
  SDL_SCANCODE_RGUI   = 231, /**< windows, command (apple), meta */

  SDL_SCANCODE_MODE = 257, /**< I'm not sure if this is really not covered
                            *   by any of the above, but since there's a
                            *   special KMOD_MODE for it I'm adding it here
                            */

  /* @} */ /* Usage page 0x07 */

  /**
   *  \name Usage page 0x0C
   *
   *  These values are mapped from usage page 0x0C (USB consumer page).
   *  See https://usb.org/sites/default/files/hut1_2.pdf
   *
   *  There are way more keys in the spec than we can represent in the
   *  current scancode range, so pick the ones that commonly come up in
   *  real world usage.
   */
  /* @{ */

  SDL_SCANCODE_AUDIONEXT    = 258,
  SDL_SCANCODE_AUDIOPREV    = 259,
  SDL_SCANCODE_AUDIOSTOP    = 260,
  SDL_SCANCODE_AUDIOPLAY    = 261,
  SDL_SCANCODE_AUDIOMUTE    = 262,
  SDL_SCANCODE_MEDIASELECT  = 263,
  SDL_SCANCODE_WWW          = 264, /**< AL Internet Browser */
  SDL_SCANCODE_MAIL         = 265,
  SDL_SCANCODE_CALCULATOR   = 266, /**< AL Calculator */
  SDL_SCANCODE_COMPUTER     = 267,
  SDL_SCANCODE_AC_SEARCH    = 268, /**< AC Search */
  SDL_SCANCODE_AC_HOME      = 269, /**< AC Home */
  SDL_SCANCODE_AC_BACK      = 270, /**< AC Back */
  SDL_SCANCODE_AC_FORWARD   = 271, /**< AC Forward */
  SDL_SCANCODE_AC_STOP      = 272, /**< AC Stop */
  SDL_SCANCODE_AC_REFRESH   = 273, /**< AC Refresh */
  SDL_SCANCODE_AC_BOOKMARKS = 274, /**< AC Bookmarks */

  /* @} */ /* Usage page 0x0C */

  /**
   *  \name Walther keys
   *
   *  These are values that Christian Walther added (for mac keyboard?).
   */
  /* @{ */

  SDL_SCANCODE_BRIGHTNESSDOWN = 275,
  SDL_SCANCODE_BRIGHTNESSUP   = 276,
  SDL_SCANCODE_DISPLAYSWITCH  = 277, /**< display mirroring/dual display
                                          switch, video mode switch */
  SDL_SCANCODE_KBDILLUMTOGGLE = 278,
  SDL_SCANCODE_KBDILLUMDOWN   = 279,
  SDL_SCANCODE_KBDILLUMUP     = 280,
  SDL_SCANCODE_EJECT          = 281,
  SDL_SCANCODE_SLEEP          = 282, /**< SC System Sleep */

  SDL_SCANCODE_APP1 = 283,
  SDL_SCANCODE_APP2 = 284,

  /* @} */ /* Walther keys */

  /**
   *  \name Usage page 0x0C (additional media keys)
   *
   *  These values are mapped from usage page 0x0C (USB consumer page).
   */
  /* @{ */

  SDL_SCANCODE_AUDIOREWIND      = 285,
  SDL_SCANCODE_AUDIOFASTFORWARD = 286,

  /* @} */ /* Usage page 0x0C (additional media keys) */

  /**
   *  \name Mobile keys
   *
   *  These are values that are often used on mobile phones.
   */
  /* @{ */

  SDL_SCANCODE_SOFTLEFT = 287,  /**< Usually situated below the display on phones and
                                     used as a multi-function feature key for selecting
                                     a software defined function shown on the bottom left
                                     of the display. */
  SDL_SCANCODE_SOFTRIGHT = 288, /**< Usually situated below the display on phones and
                                     used as a multi-function feature key for selecting
                                     a software defined function shown on the bottom right
                                     of the display. */
  SDL_SCANCODE_CALL    = 289,   /**< Used for accepting phone calls. */
  SDL_SCANCODE_ENDCALL = 290,   /**< Used for rejecting phone calls. */

  /* @} */ /* Mobile keys */

  /* Add any other keys here. */

  SDL_NUM_SCANCODES = 512 /**< not a key, just marks the number of scancodes
                               for array bounds */
} SDL_Scancode;

/**
 * The SDL virtual key representation.
 *
 * Values of this type are used to represent keyboard keys using the current
 * layout of the keyboard. These values include Unicode values representing
 * the unmodified character that would be generated by pressing the key, or an
 * SDLK_* constant for those keys that do not generate characters.
 *
 * A special exception is the number keys at the top of the keyboard which map
 * to SDLK_0...SDLK_9 on AZERTY layouts.
 */
typedef int32_t SDL_Keycode;

typedef enum SDL_Keymod {
  KMOD_NONE   = 0x0000,
  KMOD_LSHIFT = 0x0001,
  KMOD_RSHIFT = 0x0002,
  KMOD_LCTRL  = 0x0040,
  KMOD_RCTRL  = 0x0080,
  KMOD_LALT   = 0x0100,
  KMOD_RALT   = 0x0200,
  KMOD_LGUI   = 0x0400,
  KMOD_RGUI   = 0x0800,
  KMOD_NUM    = 0x1000,
  KMOD_CAPS   = 0x2000,
  KMOD_MODE   = 0x4000,
  KMOD_SCROLL = 0x8000,

  KMOD_CTRL  = KMOD_LCTRL | KMOD_RCTRL,
  KMOD_SHIFT = KMOD_LSHIFT | KMOD_RSHIFT,
  KMOD_ALT   = KMOD_LALT | KMOD_RALT,
  KMOD_GUI   = KMOD_LGUI | KMOD_RGUI,

  KMOD_RESERVED = KMOD_SCROLL /* This is for source-level compatibility with SDL 2.0.0. */
} SDL_Keymod;

#define SDLK_SCANCODE_MASK         (1 << 30)
#define SDL_SCANCODE_TO_KEYCODE(X) (X | SDLK_SCANCODE_MASK)

typedef enum SDL_KeyCode {
  SDLK_UNKNOWN = 0,

  SDLK_RETURN     = '\r',
  SDLK_ESCAPE     = '\x1B',
  SDLK_BACKSPACE  = '\b',
  SDLK_TAB        = '\t',
  SDLK_SPACE      = ' ',
  SDLK_EXCLAIM    = '!',
  SDLK_QUOTEDBL   = '"',
  SDLK_HASH       = '#',
  SDLK_PERCENT    = '%',
  SDLK_DOLLAR     = '$',
  SDLK_AMPERSAND  = '&',
  SDLK_QUOTE      = '\'',
  SDLK_LEFTPAREN  = '(',
  SDLK_RIGHTPAREN = ')',
  SDLK_ASTERISK   = '*',
  SDLK_PLUS       = '+',
  SDLK_COMMA      = ',',
  SDLK_MINUS      = '-',
  SDLK_PERIOD     = '.',
  SDLK_SLASH      = '/',
  SDLK_0          = '0',
  SDLK_1          = '1',
  SDLK_2          = '2',
  SDLK_3          = '3',
  SDLK_4          = '4',
  SDLK_5          = '5',
  SDLK_6          = '6',
  SDLK_7          = '7',
  SDLK_8          = '8',
  SDLK_9          = '9',
  SDLK_COLON      = ':',
  SDLK_SEMICOLON  = ';',
  SDLK_LESS       = '<',
  SDLK_EQUALS     = '=',
  SDLK_GREATER    = '>',
  SDLK_QUESTION   = '?',
  SDLK_AT         = '@',

  /*
     Skip uppercase letters
   */

  SDLK_LEFTBRACKET  = '[',
  SDLK_BACKSLASH    = '\\',
  SDLK_RIGHTBRACKET = ']',
  SDLK_CARET        = '^',
  SDLK_UNDERSCORE   = '_',
  SDLK_BACKQUOTE    = '`',
  SDLK_a            = 'a',
  SDLK_b            = 'b',
  SDLK_c            = 'c',
  SDLK_d            = 'd',
  SDLK_e            = 'e',
  SDLK_f            = 'f',
  SDLK_g            = 'g',
  SDLK_h            = 'h',
  SDLK_i            = 'i',
  SDLK_j            = 'j',
  SDLK_k            = 'k',
  SDLK_l            = 'l',
  SDLK_m            = 'm',
  SDLK_n            = 'n',
  SDLK_o            = 'o',
  SDLK_p            = 'p',
  SDLK_q            = 'q',
  SDLK_r            = 'r',
  SDLK_s            = 's',
  SDLK_t            = 't',
  SDLK_u            = 'u',
  SDLK_v            = 'v',
  SDLK_w            = 'w',
  SDLK_x            = 'x',
  SDLK_y            = 'y',
  SDLK_z            = 'z',

  SDLK_CAPSLOCK = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CAPSLOCK),

  SDLK_F1  = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F1),
  SDLK_F2  = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F2),
  SDLK_F3  = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F3),
  SDLK_F4  = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F4),
  SDLK_F5  = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F5),
  SDLK_F6  = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F6),
  SDLK_F7  = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F7),
  SDLK_F8  = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F8),
  SDLK_F9  = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F9),
  SDLK_F10 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F10),
  SDLK_F11 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F11),
  SDLK_F12 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F12),

  SDLK_PRINTSCREEN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PRINTSCREEN),
  SDLK_SCROLLLOCK  = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SCROLLLOCK),
  SDLK_PAUSE       = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAUSE),
  SDLK_INSERT      = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_INSERT),
  SDLK_HOME        = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_HOME),
  SDLK_PAGEUP      = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAGEUP),
  SDLK_DELETE      = '\x7F',
  SDLK_END         = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_END),
  SDLK_PAGEDOWN    = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAGEDOWN),
  SDLK_RIGHT       = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RIGHT),
  SDLK_LEFT        = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LEFT),
  SDLK_DOWN        = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DOWN),
  SDLK_UP          = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_UP),

  SDLK_NUMLOCKCLEAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMLOCKCLEAR),
  SDLK_KP_DIVIDE    = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DIVIDE),
  SDLK_KP_MULTIPLY  = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MULTIPLY),
  SDLK_KP_MINUS     = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MINUS),
  SDLK_KP_PLUS      = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PLUS),
  SDLK_KP_ENTER     = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_ENTER),
  SDLK_KP_1         = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_1),
  SDLK_KP_2         = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_2),
  SDLK_KP_3         = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_3),
  SDLK_KP_4         = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_4),
  SDLK_KP_5         = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_5),
  SDLK_KP_6         = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_6),
  SDLK_KP_7         = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_7),
  SDLK_KP_8         = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_8),
  SDLK_KP_9         = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_9),
  SDLK_KP_0         = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_0),
  SDLK_KP_PERIOD    = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PERIOD),

  SDLK_APPLICATION    = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_APPLICATION),
  SDLK_POWER          = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_POWER),
  SDLK_KP_EQUALS      = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EQUALS),
  SDLK_F13            = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F13),
  SDLK_F14            = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F14),
  SDLK_F15            = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F15),
  SDLK_F16            = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F16),
  SDLK_F17            = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F17),
  SDLK_F18            = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F18),
  SDLK_F19            = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F19),
  SDLK_F20            = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F20),
  SDLK_F21            = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F21),
  SDLK_F22            = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F22),
  SDLK_F23            = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F23),
  SDLK_F24            = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F24),
  SDLK_EXECUTE        = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EXECUTE),
  SDLK_HELP           = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_HELP),
  SDLK_MENU           = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MENU),
  SDLK_SELECT         = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SELECT),
  SDLK_STOP           = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_STOP),
  SDLK_AGAIN          = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AGAIN),
  SDLK_UNDO           = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_UNDO),
  SDLK_CUT            = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CUT),
  SDLK_COPY           = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_COPY),
  SDLK_PASTE          = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PASTE),
  SDLK_FIND           = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_FIND),
  SDLK_MUTE           = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MUTE),
  SDLK_VOLUMEUP       = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_VOLUMEUP),
  SDLK_VOLUMEDOWN     = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_VOLUMEDOWN),
  SDLK_KP_COMMA       = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_COMMA),
  SDLK_KP_EQUALSAS400 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EQUALSAS400),

  SDLK_ALTERASE   = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_ALTERASE),
  SDLK_SYSREQ     = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SYSREQ),
  SDLK_CANCEL     = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CANCEL),
  SDLK_CLEAR      = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CLEAR),
  SDLK_PRIOR      = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PRIOR),
  SDLK_RETURN2    = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RETURN2),
  SDLK_SEPARATOR  = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SEPARATOR),
  SDLK_OUT        = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_OUT),
  SDLK_OPER       = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_OPER),
  SDLK_CLEARAGAIN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CLEARAGAIN),
  SDLK_CRSEL      = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CRSEL),
  SDLK_EXSEL      = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EXSEL),

  SDLK_KP_00              = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_00),
  SDLK_KP_000             = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_000),
  SDLK_THOUSANDSSEPARATOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_THOUSANDSSEPARATOR),
  SDLK_DECIMALSEPARATOR   = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DECIMALSEPARATOR),
  SDLK_CURRENCYUNIT       = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CURRENCYUNIT),
  SDLK_CURRENCYSUBUNIT    = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CURRENCYSUBUNIT),
  SDLK_KP_LEFTPAREN       = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LEFTPAREN),
  SDLK_KP_RIGHTPAREN      = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_RIGHTPAREN),
  SDLK_KP_LEFTBRACE       = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LEFTBRACE),
  SDLK_KP_RIGHTBRACE      = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_RIGHTBRACE),
  SDLK_KP_TAB             = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_TAB),
  SDLK_KP_BACKSPACE       = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_BACKSPACE),
  SDLK_KP_A               = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_A),
  SDLK_KP_B               = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_B),
  SDLK_KP_C               = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_C),
  SDLK_KP_D               = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_D),
  SDLK_KP_E               = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_E),
  SDLK_KP_F               = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_F),
  SDLK_KP_XOR             = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_XOR),
  SDLK_KP_POWER           = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_POWER),
  SDLK_KP_PERCENT         = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PERCENT),
  SDLK_KP_LESS            = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LESS),
  SDLK_KP_GREATER         = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_GREATER),
  SDLK_KP_AMPERSAND       = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_AMPERSAND),
  SDLK_KP_DBLAMPERSAND    = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DBLAMPERSAND),
  SDLK_KP_VERTICALBAR     = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_VERTICALBAR),
  SDLK_KP_DBLVERTICALBAR  = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DBLVERTICALBAR),
  SDLK_KP_COLON           = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_COLON),
  SDLK_KP_HASH            = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_HASH),
  SDLK_KP_SPACE           = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_SPACE),
  SDLK_KP_AT              = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_AT),
  SDLK_KP_EXCLAM          = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EXCLAM),
  SDLK_KP_MEMSTORE        = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMSTORE),
  SDLK_KP_MEMRECALL       = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMRECALL),
  SDLK_KP_MEMCLEAR        = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMCLEAR),
  SDLK_KP_MEMADD          = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMADD),
  SDLK_KP_MEMSUBTRACT     = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMSUBTRACT),
  SDLK_KP_MEMMULTIPLY     = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMMULTIPLY),
  SDLK_KP_MEMDIVIDE       = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMDIVIDE),
  SDLK_KP_PLUSMINUS       = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PLUSMINUS),
  SDLK_KP_CLEAR           = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_CLEAR),
  SDLK_KP_CLEARENTRY      = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_CLEARENTRY),
  SDLK_KP_BINARY          = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_BINARY),
  SDLK_KP_OCTAL           = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_OCTAL),
  SDLK_KP_DECIMAL         = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DECIMAL),
  SDLK_KP_HEXADECIMAL     = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_HEXADECIMAL),

  SDLK_LCTRL  = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LCTRL),
  SDLK_LSHIFT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LSHIFT),
  SDLK_LALT   = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LALT),
  SDLK_LGUI   = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LGUI),
  SDLK_RCTRL  = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RCTRL),
  SDLK_RSHIFT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RSHIFT),
  SDLK_RALT   = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RALT),
  SDLK_RGUI   = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RGUI),

  SDLK_MODE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MODE),

  SDLK_AUDIONEXT    = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIONEXT),
  SDLK_AUDIOPREV    = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOPREV),
  SDLK_AUDIOSTOP    = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOSTOP),
  SDLK_AUDIOPLAY    = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOPLAY),
  SDLK_AUDIOMUTE    = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOMUTE),
  SDLK_MEDIASELECT  = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIASELECT),
  SDLK_WWW          = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_WWW),
  SDLK_MAIL         = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MAIL),
  SDLK_CALCULATOR   = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CALCULATOR),
  SDLK_COMPUTER     = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_COMPUTER),
  SDLK_AC_SEARCH    = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_SEARCH),
  SDLK_AC_HOME      = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_HOME),
  SDLK_AC_BACK      = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_BACK),
  SDLK_AC_FORWARD   = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_FORWARD),
  SDLK_AC_STOP      = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_STOP),
  SDLK_AC_REFRESH   = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_REFRESH),
  SDLK_AC_BOOKMARKS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_BOOKMARKS),

  SDLK_BRIGHTNESSDOWN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_BRIGHTNESSDOWN),
  SDLK_BRIGHTNESSUP   = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_BRIGHTNESSUP),
  SDLK_DISPLAYSWITCH  = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DISPLAYSWITCH),
  SDLK_KBDILLUMTOGGLE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KBDILLUMTOGGLE),
  SDLK_KBDILLUMDOWN   = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KBDILLUMDOWN),
  SDLK_KBDILLUMUP     = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KBDILLUMUP),
  SDLK_EJECT          = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EJECT),
  SDLK_SLEEP          = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SLEEP),
  SDLK_APP1           = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_APP1),
  SDLK_APP2           = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_APP2),

  SDLK_AUDIOREWIND      = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOREWIND),
  SDLK_AUDIOFASTFORWARD = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOFASTFORWARD),

  SDLK_SOFTLEFT  = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SOFTLEFT),
  SDLK_SOFTRIGHT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SOFTRIGHT),
  SDLK_CALL      = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CALL),
  SDLK_ENDCALL   = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_ENDCALL)
} SDL_KeyCode;

typedef struct SDL_Keysym {
  SDL_Scancode scancode; /**< SDL physical key code - see SDL_Scancode for details */
  SDL_Keycode  sym;      /**< SDL virtual key code - see SDL_Keycode for details */
  uint16_t     mod;      /**< current key modifiers */
  uint32_t     unused;
} SDL_Keysym;

typedef struct SDL_KeyboardEvent {
  uint32_t   type;      /**< SDL_KEYDOWN or SDL_KEYUP */
  uint32_t   timestamp; /**< In milliseconds, populated using SDL_GetTicks() */
  uint32_t   windowID;  /**< The window with keyboard focus, if any */
  uint8_t    state;     /**< SDL_PRESSED or SDL_RELEASED */
  uint8_t    repeat;    /**< Non-zero if this is a key repeat */
  uint8_t    padding2;
  uint8_t    padding3;
  SDL_Keysym keysym; /**< The key that was pressed or released */
} SDL_KeyboardEvent;

typedef union SDL_Event {
  uint32_t
      type; /**< Event type, shared with all events, Uint32 to cover user events which are not in the SDL_EventType enumeration */
  // SDL_CommonEvent common;                 /**< Common event data */
  // SDL_DisplayEvent display;               /**< Display event data */
  // SDL_WindowEvent window;                 /**< Window event data */
  // SDL_KeyboardDeviceEvent kdevice;        /**< Keyboard device change event data */
  SDL_KeyboardEvent key; /**< Keyboard event data */
  // SDL_TextEditingEvent edit;              /**< Text editing event data */
  // SDL_TextEditingCandidatesEvent edit_candidates; /**< Text editing candidates event data */
  // SDL_TextInputEvent text;                /**< Text input event data */
  // SDL_MouseDeviceEvent mdevice;           /**< Mouse device change event data */
  // SDL_MouseMotionEvent motion;            /**< Mouse motion event data */
  // SDL_MouseButtonEvent button;            /**< Mouse button event data */
  // SDL_MouseWheelEvent wheel;              /**< Mouse wheel event data */
  // SDL_JoyDeviceEvent jdevice;             /**< Joystick device change event data */
  // SDL_JoyAxisEvent jaxis;                 /**< Joystick axis event data */
  // SDL_JoyBallEvent jball;                 /**< Joystick ball event data */
  // SDL_JoyHatEvent jhat;                   /**< Joystick hat event data */
  // SDL_JoyButtonEvent jbutton;             /**< Joystick button event data */
  // SDL_JoyBatteryEvent jbattery;           /**< Joystick battery event data */
  // SDL_GamepadDeviceEvent gdevice;         /**< Gamepad device event data */
  // SDL_GamepadAxisEvent gaxis;             /**< Gamepad axis event data */
  // SDL_GamepadButtonEvent gbutton;         /**< Gamepad button event data */
  // SDL_GamepadTouchpadEvent gtouchpad;     /**< Gamepad touchpad event data */
  // SDL_GamepadSensorEvent gsensor;         /**< Gamepad sensor event data */
  // SDL_AudioDeviceEvent adevice;           /**< Audio device event data */
  // SDL_CameraDeviceEvent cdevice;          /**< Camera device event data */
  // SDL_SensorEvent sensor;                 /**< Sensor event data */
  // SDL_QuitEvent quit;                     /**< Quit request event data */
  // SDL_UserEvent user;                     /**< Custom event data */
  // SDL_TouchFingerEvent tfinger;           /**< Touch finger event data */
  // SDL_PenProximityEvent pproximity;       /**< Pen proximity event data */
  // SDL_PenTouchEvent ptouch;               /**< Pen tip touching event data */
  // SDL_PenMotionEvent pmotion;             /**< Pen motion event data */
  // SDL_PenButtonEvent pbutton;             /**< Pen button event data */
  // SDL_PenAxisEvent paxis;                 /**< Pen axis event data */
  // SDL_RenderEvent render;                 /**< Render event data */
  // SDL_DropEvent drop;                     /**< Drag and drop event data */
  // SDL_ClipboardEvent clipboard;           /**< Clipboard event data */

  /* This is necessary for ABI compatibility between Visual C++ and GCC.
     Visual C++ will respect the push pack pragma and use 52 bytes (size of
     SDL_TextEditingEvent, the largest structure for 32-bit and 64-bit
     architectures) for this union, and GCC will use the alignment of the
     largest datatype within the union, which is 8 bytes on 64-bit
     architectures.

     So... we'll add padding to force the size to be the same for both.

     On architectures where pointers are 16 bytes, this needs rounding up to
     the next multiple of 16, 64, and on architectures where pointers are
     even larger the size of SDL_UserEvent will dominate as being 3 pointers.
  */
  uint8_t padding[128];
} SDL_Event;

/** @name Event State */
/*@{*/
#define SDL_QUERY   -1
#define SDL_IGNORE  0
#define SDL_DISABLE 0
#define SDL_ENABLE  1
/*@}*/

#define SDL_GetTicks ez80_timers_ticks_get

extern void SDL_Delay(uint32_t ms);
extern int  SDL_LockSurface(SDL_Surface *surface);
extern int  SDL_FillRect(SDL_Surface *dst, const SDL_Rect *rect, uint32_t color);
// extern uint32_t            SDL_GetTicks(void);
extern int                 SDL_Init(int32_t flags);
extern const char         *SDL_GetError(void);
extern void                SDL_Quit(void);
extern int                 SDL_NumJoysticks(void);
extern uint32_t            SDL_GetRelativeMouseState(int *x, int *y);
extern uint32_t            SDL_GetMouseState(int *x, int *y);
extern void                SDL_GameControllerUpdate(void);
extern int16_t             SDL_GameControllerGetAxis(SDL_GameController *gamecontroller, SDL_GameControllerAxis axis);
extern uint8_t             SDL_GameControllerGetButton(SDL_GameController *gamecontroller, SDL_GameControllerButton button);
extern SDL_Keymod          SDL_GetModState(void);
extern int                 SDL_WaitEvent(SDL_Event *event);
extern int                 SDL_PollEvent(SDL_Event *event);
extern int                 SDL_InitSubSystem(uint32_t flags);
extern bool                SDL_IsGameController(int joystick_index);
extern SDL_GameController *SDL_GameControllerOpen(int joystick_index);
extern const char         *SDL_GameControllerName(SDL_GameController *gamecontroller);
extern int                 SDL_GameControllerEventState(int state);
extern uint8_t             SDL_EventState(uint32_t type, int state);
extern int                 SDL_SetRelativeMouseMode(bool enabled);
extern void                SDL_GameControllerClose(SDL_GameController *gamecontroller);

#define SDL_BUTTON(X)     (1 << ((X)-1))
#define SDL_BUTTON_LEFT   1
#define SDL_BUTTON_MIDDLE 2
#define SDL_BUTTON_RIGHT  3
#define SDL_BUTTON_X1     4
#define SDL_BUTTON_X2     5
#define SDL_BUTTON_LMASK  SDL_BUTTON(SDL_BUTTON_LEFT)
#define SDL_BUTTON_MMASK  SDL_BUTTON(SDL_BUTTON_MIDDLE)
#define SDL_BUTTON_RMASK  SDL_BUTTON(SDL_BUTTON_RIGHT)
#define SDL_BUTTON_X1MASK SDL_BUTTON(SDL_BUTTON_X1)
#define SDL_BUTTON_X2MASK SDL_BUTTON(SDL_BUTTON_X2)

typedef uint8_t SDL_Color;

#define RED_FRM_GRB(grb)   ((grb & 0x1C) >> 2)
#define GREEN_FRM_GRB(grb) ((grb & 0xE0) >> 5)
#define BLUE_FRM_GRB(grb)  (grb & 0x03)
#define GRB(g, r, b)       (g >> 3) << 5 | (r >> 3) << 2 | (b >> 4)

#define MIX_CHANNELS 8

typedef struct Mix_Chunk {
  int      allocated;
  uint8_t *abuf;
  uint32_t alen;
  uint8_t  volume; /* Per-sample volume, 0-128 */
} Mix_Chunk;

#define SDLCALL
#define DECLSPEC

typedef struct SDL_RWops {
  /**
   *  Return the size of the file in this rwops, or -1 if unknown
   */
  int64_t(SDLCALL *size)(struct SDL_RWops *context);

  /**
   *  Seek to `offset` relative to `whence`, one of stdio's whence values:
   *  RW_SEEK_SET, RW_SEEK_CUR, RW_SEEK_END
   *
   *  \return the final offset in the data stream, or -1 on error.
   */
  int64_t(SDLCALL *seek)(struct SDL_RWops *context, int64_t offset, int whence);

  /**
   *  Read up to `maxnum` objects each of size `size` from the data
   *  stream to the area pointed at by `ptr`.
   *
   *  \return the number of objects read, or 0 at error or end of file.
   */
  size_t(SDLCALL *read)(struct SDL_RWops *context, void *ptr, size_t size, size_t maxnum);

  /**
   *  Write exactly `num` objects each of size `size` from the area
   *  pointed at by `ptr` to data stream.
   *
   *  \return the number of objects written, or 0 at error or end of file.
   */
  size_t(SDLCALL *write)(struct SDL_RWops *context, const void *ptr, size_t size, size_t num);

  /**
   *  Close and free an allocated SDL_RWops structure.
   *
   *  \return 0 if successful or -1 on write error when flushing data.
   */
  int(SDLCALL *close)(struct SDL_RWops *context);

  int32_t type;
  union {
    struct {
      int8_t *base;
      int8_t *here;
      int8_t *stop;
    } mem;
    struct {
      void *data1;
      void *data2;
    } unknown;
  } hidden;

} SDL_RWops;

#define SDLK_m 'm'

#define Mix_GetError SDL_GetError

typedef void(SDLCALL *Mix_MixCallback)(void *udata, uint8_t *stream, int len);
typedef void(SDLCALL *Mix_ChannelFinishedCallback)(int channel);
extern DECLSPEC void SDLCALL Mix_FreeChunk(Mix_Chunk *chunk);

extern int        Mix_HaltChannel(int channel);
extern int        Mix_GroupAvailable(int tag);
extern int        Mix_GroupOldest(int tag);
extern int        Mix_SetPanning(int channel, uint8_t left, uint8_t right);
extern Mix_Chunk *Mix_LoadWAV_RW(SDL_RWops *src, int freesrc);
extern int        Mix_PlayChannel(int channel, Mix_Chunk *chunk, int loops);
extern int        Mix_OpenAudio(int frequency, uint16_t format, int channels, int chunksize);
extern int        Mix_ReserveChannels(int num);
extern int        Mix_GroupChannels(int from, int to, int tag);
extern void       Mix_ChannelFinished(Mix_ChannelFinishedCallback channel_finished);
extern void       Mix_HookMusic(Mix_MixCallback mix_func, void *arg);

extern SDL_RWops *SDL_RWFromMem(void *mem, int size);

#define AUDIO_U8     0x0008 /**< Unsigned 8-bit samples */
#define AUDIO_S8     0x8008 /**< Signed 8-bit samples */
#define AUDIO_U16LSB 0x0010 /**< Unsigned 16-bit samples */
#define AUDIO_S16LSB 0x8010 /**< Signed 16-bit samples */
#define AUDIO_U16MSB 0x1010 /**< As above, but big-endian byte order */
#define AUDIO_S16MSB 0x9010 /**< As above, but big-endian byte order */
#define AUDIO_U16    AUDIO_U16LSB
#define AUDIO_S16    AUDIO_S16LSB

/**
 * The types of events that can be delivered.
 */
typedef enum SDL_EventType {
  SDL_FIRSTEVENT = 0, /**< Unused (do not remove) */

  /* Application events */
  SDL_QUIT = 0x100, /**< User-requested quit */

  /* These application events have special meaning on iOS, see README-ios.md for details */
  SDL_APP_TERMINATING,         /**< The application is being terminated by the OS
                                    Called on iOS in applicationWillTerminate()
                                    Called on Android in onDestroy()
                               */
  SDL_APP_LOWMEMORY,           /**< The application is low on memory, free memory if possible.
                                    Called on iOS in applicationDidReceiveMemoryWarning()
                                    Called on Android in onLowMemory()
                               */
  SDL_APP_WILLENTERBACKGROUND, /**< The application is about to enter the background
                                   Called on iOS in applicationWillResignActive()
                                   Called on Android in onPause()
                              */
  SDL_APP_DIDENTERBACKGROUND,  /**< The application did enter the background and may not get CPU for some time
                                    Called on iOS in applicationDidEnterBackground()
                                    Called on Android in onPause()
                               */
  SDL_APP_WILLENTERFOREGROUND, /**< The application is about to enter the foreground
                                   Called on iOS in applicationWillEnterForeground()
                                   Called on Android in onResume()
                              */
  SDL_APP_DIDENTERFOREGROUND,  /**< The application is now interactive
                                    Called on iOS in applicationDidBecomeActive()
                                    Called on Android in onResume()
                               */

  SDL_LOCALECHANGED, /**< The user's locale preferences have changed. */

  /* Display events */
  SDL_DISPLAYEVENT = 0x150, /**< Display state change */

  /* Window events */
  SDL_WINDOWEVENT = 0x200, /**< Window state change */
  SDL_SYSWMEVENT,          /**< System specific event */

  /* Keyboard events */
  SDL_KEYDOWN = 0x300, /**< Key pressed */
  SDL_KEYUP,           /**< Key released */
  SDL_TEXTEDITING,     /**< Keyboard text editing (composition) */
  SDL_TEXTINPUT,       /**< Keyboard text input */
  SDL_KEYMAPCHANGED,   /**< Keymap changed due to a system event such as an
                            input language or keyboard layout change.
                       */
  SDL_TEXTEDITING_EXT, /**< Extended keyboard text editing (composition) */

  /* Mouse events */
  SDL_MOUSEMOTION = 0x400, /**< Mouse moved */
  SDL_MOUSEBUTTONDOWN,     /**< Mouse button pressed */
  SDL_MOUSEBUTTONUP,       /**< Mouse button released */
  SDL_MOUSEWHEEL,          /**< Mouse wheel motion */

  /* Joystick events */
  SDL_JOYAXISMOTION = 0x600, /**< Joystick axis motion */
  SDL_JOYBALLMOTION,         /**< Joystick trackball motion */
  SDL_JOYHATMOTION,          /**< Joystick hat position change */
  SDL_JOYBUTTONDOWN,         /**< Joystick button pressed */
  SDL_JOYBUTTONUP,           /**< Joystick button released */
  SDL_JOYDEVICEADDED,        /**< A new joystick has been inserted into the system */
  SDL_JOYDEVICEREMOVED,      /**< An opened joystick has been removed */
  SDL_JOYBATTERYUPDATED,     /**< Joystick battery level change */

  /* Game controller events */
  SDL_CONTROLLERAXISMOTION = 0x650, /**< Game controller axis motion */
  SDL_CONTROLLERBUTTONDOWN,         /**< Game controller button pressed */
  SDL_CONTROLLERBUTTONUP,           /**< Game controller button released */
  SDL_CONTROLLERDEVICEADDED,        /**< A new Game controller has been inserted into the system */
  SDL_CONTROLLERDEVICEREMOVED,      /**< An opened Game controller has been removed */
  SDL_CONTROLLERDEVICEREMAPPED,     /**< The controller mapping was updated */
  SDL_CONTROLLERTOUCHPADDOWN,       /**< Game controller touchpad was touched */
  SDL_CONTROLLERTOUCHPADMOTION,     /**< Game controller touchpad finger was moved */
  SDL_CONTROLLERTOUCHPADUP,         /**< Game controller touchpad finger was lifted */
  SDL_CONTROLLERSENSORUPDATE,       /**< Game controller sensor was updated */
  SDL_CONTROLLERUPDATECOMPLETE_RESERVED_FOR_SDL3,
  SDL_CONTROLLERSTEAMHANDLEUPDATED, /**< Game controller Steam handle has changed */

  /* Touch events */
  SDL_FINGERDOWN = 0x700,
  SDL_FINGERUP,
  SDL_FINGERMOTION,

  /* Gesture events */
  SDL_DOLLARGESTURE = 0x800,
  SDL_DOLLARRECORD,
  SDL_MULTIGESTURE,

  /* Clipboard events */
  SDL_CLIPBOARDUPDATE = 0x900, /**< The clipboard or primary selection changed */

  /* Drag and drop events */
  SDL_DROPFILE = 0x1000, /**< The system requests a file open */
  SDL_DROPTEXT,          /**< text/plain drag-and-drop event */
  SDL_DROPBEGIN,         /**< A new set of drops is beginning (NULL filename) */
  SDL_DROPCOMPLETE,      /**< Current set of drops is now complete (NULL filename) */

  /* Audio hotplug events */
  SDL_AUDIODEVICEADDED = 0x1100, /**< A new audio device is available */
  SDL_AUDIODEVICEREMOVED,        /**< An audio device has been removed. */

  /* Sensor events */
  SDL_SENSORUPDATE = 0x1200, /**< A sensor was updated */

  /* Render events */
  SDL_RENDER_TARGETS_RESET = 0x2000, /**< The render targets have been reset and their contents need to be updated */
  SDL_RENDER_DEVICE_RESET,           /**< The device has been reset and all textures need to be recreated */

  /* Internal events */
  SDL_POLLSENTINEL = 0x7F00, /**< Signals the end of an event poll cycle */

  /** Events SDL_USEREVENT through SDL_LASTEVENT are for your use,
   *  and should be allocated with SDL_RegisterEvents()
   */
  SDL_USEREVENT = 0x8000,

  /**
   *  This last event is only for bounding internal arrays
   */
  SDL_LASTEVENT = 0xFFFF
} SDL_EventType;

/**
 *  \name SDL_INIT_*
 *
 *  These are the flags which may be passed to SDL_Init().  You should
 *  specify the subsystems which you will be using in your application.
 */
/* @{ */
#define SDL_INIT_TIMER          0x00000001
#define SDL_INIT_AUDIO          0x00000010
#define SDL_INIT_VIDEO          0x00000020 /**< SDL_INIT_VIDEO implies SDL_INIT_EVENTS */
#define SDL_INIT_JOYSTICK       0x00000200 /**< SDL_INIT_JOYSTICK implies SDL_INIT_EVENTS */
#define SDL_INIT_HAPTIC         0x00001000
#define SDL_INIT_GAMECONTROLLER 0x00002000 /**< SDL_INIT_GAMECONTROLLER implies SDL_INIT_JOYSTICK */
#define SDL_INIT_EVENTS         0x00004000
#define SDL_INIT_SENSOR         0x00008000
#define SDL_INIT_NOPARACHUTE    0x00100000 /**< compatibility; this flag is ignored. */
#define SDL_INIT_EVERYTHING                                                                                                        \
  (SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC |                      \
   SDL_INIT_GAMECONTROLLER | SDL_INIT_SENSOR)

#define SDL_BlitSurface SDL_UpperBlit

extern int          SDL_UpperBlit(SDL_Surface *src, const SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect);
extern SDL_Surface *SDL_CreateRGBSurface(
    /*uint32_t flags, */ int width, int height /*, int depth, uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask*/);

typedef struct SDL_Palette {
  int        ncolors;
  SDL_Color *colors;
  uint32_t   version;
  int        refcount;
} SDL_Palette;

#define SDL_SetPaletteColors(...)
#define SDL_ConvertSurface(...) NULL
#define SDL_FreeSurface(...)
extern uint32_t SDL_MapRGB(/*const SDL_PixelFormat *format,*/ uint8_t r, uint8_t g, uint8_t b);

extern SDL_Texture *SDL_CreateTextureFromSurface(SDL_Renderer *renderer, SDL_Surface *surface);

extern int SDL_RenderCopy(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_Rect *srcrect, const SDL_Rect *dstrect);

extern void           SDL_RenderPresent(SDL_Renderer *renderer);
extern void           SDL_DestroyTexture(SDL_Texture *texture);
extern const uint8_t *SDL_GetKeyboardState(int *numkeys);

#define SDL_BlitScaled(...)
#define SDL_SaveBMP(...)

extern const uint8_t scale_points[320];

#endif
