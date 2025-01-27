//
//  ID Engine
//  ID_IN.c - Input Manager
//  v1.0d1
//  By Jason Blochowiak
//

//
//  This module handles dealing with the various input devices
//
//  Depends on: Memory Mgr (for demo recording), Sound Mgr (for timing stuff),
//              User Mgr (for command line parms)
//
//  Globals:
//      LastScan - The keyboard scan code of the last key pressed
//      LastASCII - The ASCII value of the last key pressed
//  DEBUG - there are more globals
//

#include "id_vl.h"
#include "wl_def.h"

/*
=============================================================================

                                        GLOBAL VARIABLES

=============================================================================
*/

//
// configuration variables
//
boolean MousePresent;
boolean forcegrabmouse;

//  Global variables
boolean Keyboard[NumCodes];

// extern boolean Keyboard[] = {
// 	{sc_None, false},
// 	{sc_Return, false},
// 	{sc_Escape, false},
// 	{sc_Space, false},
// 	{sc_BackSpace, false},
// 	{sc_Tab, false},
// 	{sc_Alt, false},
// 	{sc_Control, false},
// 	{sc_CapsLock, false},
// 	{sc_LShift, false},
// 	{sc_RShift, false},
// 	{sc_UpArrow, false},
// 	{sc_DownArrow, false},
// 	{sc_LeftArrow, false},
// 	{sc_RightArrow, false},
// 	{sc_Insert, false},
// 	{sc_Delete, false},
// 	{sc_Home, false},
// 	{sc_End, false},
// 	{sc_PgUp, false},
// 	{sc_PgDn, false},
// 	{sc_F1, false},
// 	{sc_F2, false},
// 	{sc_F3, false},
// 	{sc_F4, false},
// 	{sc_F5, false},
// 	{sc_F6, false},
// 	{sc_F7, false},
// 	{sc_F8, false},
// 	{sc_F9, false},
// 	{sc_F10, false},
// 	{sc_F11, false},
// 	{sc_F12, false},
// 	{sc_ScrollLock, false},
// 	{sc_PrintScreen, false},
// 	{sc_1, false},
// 	{sc_2, false},
// 	{sc_3, false},
// 	{sc_4, false},
// 	{sc_5, false},
// 	{sc_6, false},
// 	{sc_7, false},
// 	{sc_8, false},
// 	{sc_9, false},
// 	{sc_0, false},
// 	{sc_A, false},
// 	{sc_B, false},
// 	{sc_C, false},
// 	{sc_D, false},
// 	{sc_E, false},
// 	{sc_F, false},
// 	{sc_G, false},
// 	{sc_H, false},
// 	{sc_I, false},
// 	{sc_J, false},
// 	{sc_K, false},
// 	{sc_L, false},
// 	{sc_M, false},
// 	{sc_N, false},
// 	{sc_O, false},
// 	{sc_P, false},
// 	{sc_Q, false},
// 	{sc_R, false},
// 	{sc_S, false},
// 	{sc_T, false},
// 	{sc_U, false},
// 	{sc_V, false},
// 	{sc_W, false},
// 	{sc_X, false},
// 	{sc_Y, false},
// 	{sc_Z, false}
// };

volatile boolean  Paused;
volatile char     LastASCII;
volatile ScanCode LastScan;

// KeyboardDef   KbdDefs = {0x1d,0x38,0x47,0x48,0x49,0x4b,0x4d,0x4f,0x50,0x51};
static KeyboardDef KbdDefs = {
    sc_Control,    // button0
    sc_Alt,        // button1
    sc_Home,       // upleft
    sc_UpArrow,    // up
    sc_PgUp,       // upright
    sc_LeftArrow,  // left
    sc_RightArrow, // right
    sc_End,        // downleft
    sc_DownArrow,  // down
    sc_PgDn        // downright
};

// static SDL_Joystick* Joystick;
static SDL_GameController *GameController;
// Flip the right stick axes to match usual mapping of Joystick API.
static SDL_GameControllerAxis GameControllerAxisMap[SDL_CONTROLLER_AXIS_MAX] = {
    SDL_CONTROLLER_AXIS_LEFTX,       SDL_CONTROLLER_AXIS_LEFTY,  // X, Y
    SDL_CONTROLLER_AXIS_RIGHTY,      SDL_CONTROLLER_AXIS_RIGHTX, // Z, R
    SDL_CONTROLLER_AXIS_TRIGGERLEFT, SDL_CONTROLLER_AXIS_TRIGGERRIGHT};
struct JoystickSens *JoySensitivity;
int                  JoyNumButtons;
int                  JoyNumAxes;
static int           JoyNumHats;

static bool GrabInput = false;
/*
=============================================================================

                                        LOCAL VARIABLES

=============================================================================
*/
byte ASCIINames[] = // Unshifted ASCII for scan codes       // TODO: keypad
    {
        //   0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
        0,   0,   0,   0,   0,   0,   0,   0,   8,   9,   0,   0,   0,   13,  0,   0,   // 0
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   27,  0,   0,   0,   // 1
        ' ', 0,   0,   0,   0,   0,   0,   39,  0,   0,   '*', '+', ',', '-', '.', '/', // 2
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 0,   ';', 0,   '=', 0,   0,   // 3
        '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', // 4
        'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '[', 92,  ']', 0,   0,   // 5
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   // 6
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0    // 7
};
byte ShiftNames[] = // Shifted ASCII for scan codes
    {
        //   0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
        0,   0,   0,   0,   0,   0,   0,   0,   8,   9,   0,   0,   0,   13,  0,   0,   // 0
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   27,  0,   0,   0,   // 1
        ' ', 0,   0,   0,   0,   0,   0,   34,  0,   0,   '*', '+', '<', '_', '>', '?', // 2
        ')', '!', '@', '#', '$', '%', '^', '&', '*', '(', 0,   ':', 0,   '+', 0,   0,   // 3
        '~', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', // 4
        'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '{', '|', '}', 0,   0,   // 5
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   // 6
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0    // 7
};
byte SpecialNames[] = // ASCII for 0xe0 prefixed codes
    {
        //   0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
        0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0,  0, 0, 0, // 0
        0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 13, 0, 0, 0, // 1
        0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0,  0, 0, 0, // 2
        0, 0, 0, 0, 0, '/', 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, // 3
        0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0,  0, 0, 0, // 4
        0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0,  0, 0, 0, // 5
        0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0,  0, 0, 0, // 6
        0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0,  0, 0, 0  // 7
};

static boolean IN_Started;

static Direction DirTable[] = // Quick lookup for total direction
    {dir_NorthWest, dir_North, dir_NorthEast, dir_West, dir_None, dir_East, dir_SouthWest, dir_South, dir_SouthEast};

///////////////////////////////////////////////////////////////////////////
//
//  INL_GetMouseButtons() - Gets the status of the mouse buttons from the
//      mouse driver
//
///////////////////////////////////////////////////////////////////////////
static int INL_GetMouseButtons(void) {
  int buttons       = SDL_GetMouseState(NULL, NULL);
  int middlePressed = buttons & SDL_BUTTON(SDL_BUTTON_MIDDLE);
  int rightPressed  = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
  buttons &= ~(SDL_BUTTON(SDL_BUTTON_MIDDLE) | SDL_BUTTON(SDL_BUTTON_RIGHT));
  if (middlePressed)
    buttons |= 1 << 2;
  if (rightPressed)
    buttons |= 1 << 1;

  return buttons;
}

///////////////////////////////////////////////////////////////////////////
//
//  IN_GetJoyDelta() - Returns the relative movement of the specified
//      joystick (from +/-127)
//
///////////////////////////////////////////////////////////////////////////
void IN_GetJoyDelta(int *dx, int *dy) {
  int x, y;

  if (!GameController) {
    *dx = *dy = 0;
    return;
  }

  SDL_GameControllerUpdate();
  x = SDL_GameControllerGetAxis(GameController, SDL_CONTROLLER_AXIS_RIGHTX) >> 8;
  y = SDL_GameControllerGetAxis(GameController, SDL_CONTROLLER_AXIS_LEFTY) >> 8;

  if (SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
    x += 127;
  else if (SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_DPAD_LEFT))
    x -= 127;
  if (SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_DPAD_DOWN))
    y += 127;
  else if (SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_DPAD_UP))
    y -= 127;

  *dx = x;
  *dy = y;
}

///////////////////////////////////////////////////////////////////////////
//
//  IN_GetJoyFineDelta() - Returns the relative movement of the specified
//      joystick without dividing the results by 256 (from +/-127)
//
///////////////////////////////////////////////////////////////////////////
void IN_GetJoyFineDelta(int *dx, int *dy) {
  if (!GameController) {
    *dx = 0;
    *dy = 0;
    return;
  }

  SDL_GameControllerUpdate();
  int x = SDL_GameControllerGetAxis(GameController, SDL_CONTROLLER_AXIS_RIGHTX) >> 8;
  int y = SDL_GameControllerGetAxis(GameController, SDL_CONTROLLER_AXIS_LEFTY) >> 8;

  if (x < -128)
    x = -128;
  else if (x > 127)
    x = 127;

  if (y < -128)
    y = -128;
  else if (y > 127)
    y = 127;

  *dx = x;
  *dy = y;
}

int IN_GetJoyAxis(int axis) { return SDL_GameControllerGetAxis(GameController, GameControllerAxisMap[axis]); }

/*
===================
=
= IN_JoyButtons
=
===================
*/

int IN_JoyButtons() {
  SDL_GameControllerUpdate();

  int res = 0;
  for (int i = 0; i < JoyNumButtons; ++i) {
    if (SDL_GameControllerGetButton(GameController, (SDL_GameControllerButton)i)) {
      // Allow controllers or PS4/PS5 controllers to use the game controller API
      // to enter the menu.
      if (i == SDL_CONTROLLER_BUTTON_START)
        US_ControlPanel(buttonstate[bt_esc] = true);
      else if (i == SDL_CONTROLLER_BUTTON_TOUCHPAD)
        US_ControlPanel(buttonstate[bt_esc] = true);
      else
        res |= 1 << i;
    }
  }
  return res;
}

boolean IN_JoyPresent() { return GameController != NULL; }

int IN_JoyAxes(void) {
  SDL_GameControllerUpdate();
  int res = 0;
  for (int i = 0; i < JoyNumAxes; ++i) {
    int16_t pos = SDL_GameControllerGetAxis(GameController, (SDL_GameControllerAxis)GameControllerAxisMap[i]);
    if (pos <= -0x1000)
      res |= 1 << (i * 2);
    else if (pos >= 0x1000)
      res |= 1 << (i * 2 + 1);
  }
  return res;
}

static void processEvent(SDL_Event *event) {
  switch (event->type) {
    // exit if the window is closed
  case SDL_QUIT:
    Quit(NULL);

    // check for keypresses
  case SDL_KEYDOWN: {
    if (event->key.keysym.sym == SDLK_SCROLLLOCK || event->key.keysym.sym == SDLK_F12) {
      GrabInput = !GrabInput;
      // SDL_SetRelativeMouseMode(GrabInput ? SDL_TRUE : SDL_FALSE);
      return;
    }

    LastScan       = event->key.keysym.sym;
    SDL_Keymod mod = SDL_GetModState();
    if (Keyboard[(uint8_t)sc_Alt]) {
      if (LastScan == SDLK_F4)
        Quit(NULL);
    }

    if (LastScan == SDLK_KP_ENTER)
      LastScan = SDLK_RETURN;
    else if (LastScan == SDLK_RSHIFT)
      LastScan = SDLK_LSHIFT;
    else if (LastScan == SDLK_RALT)
      LastScan = SDLK_LALT;
    else if (LastScan == SDLK_RCTRL)
      LastScan = SDLK_LCTRL;
    else {
      if ((mod & KMOD_NUM) == 0) {
        switch (LastScan) {
        case SDLK_KP_2:
          LastScan = SDLK_DOWN;
          break;
        case SDLK_KP_4:
          LastScan = SDLK_LEFT;
          break;
        case SDLK_KP_6:
          LastScan = SDLK_RIGHT;
          break;
        case SDLK_KP_8:
          LastScan = SDLK_UP;
          break;
        }
      }
    }

    int sym = LastScan;
    if (sym >= 'a' && sym <= 'z')
      sym -= 32; // convert to uppercase

    if (mod & (KMOD_SHIFT | KMOD_CAPS)) {
      if (sym < (int)lengthof(ShiftNames) && ShiftNames[sym])
        LastASCII = ShiftNames[sym];
    } else {
      if (sym < (int)lengthof(ASCIINames) && ASCIINames[sym])
        LastASCII = ASCIINames[sym];
    }

    int intLasScan       = LastScan;
    Keyboard[intLasScan] = 1;

    if (LastScan == SDLK_PAUSE)
      Paused = true;
    break;
  }

  case SDL_KEYUP: {
    int key = event->key.keysym.sym;
    if (key == SDLK_KP_ENTER)
      key = SDLK_RETURN;
    else if (key == SDLK_RSHIFT)
      key = SDLK_LSHIFT;
    else if (key == SDLK_RALT)
      key = SDLK_LALT;
    else if (key == SDLK_RCTRL)
      key = SDLK_LCTRL;
    else {
      if ((SDL_GetModState() & KMOD_NUM) == 0) {
        switch (key) {
        case SDLK_KP_2:
          key = SDLK_DOWN;
          break;
        case SDLK_KP_4:
          key = SDLK_LEFT;
          break;
        case SDLK_KP_6:
          key = SDLK_RIGHT;
          break;
        case SDLK_KP_8:
          key = SDLK_UP;
          break;
        }
      }
    }

    Keyboard[key] = 0;

    break;
  }
  }
}

void IN_WaitAndProcessEvents() {
  SDL_Event event;
  if (!SDL_WaitEvent(&event))
    return;
  do {
    processEvent(&event);
  } while (SDL_PollEvent(&event));
}

void IN_ProcessEvents() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    processEvent(&event);
  }
}

///////////////////////////////////////////////////////////////////////////
//
//  IN_Startup() - Starts up the Input Mgr
//
///////////////////////////////////////////////////////////////////////////
void IN_Startup(void) {
  if (IN_Started)
    return;

  IN_ClearKeysDown();

  if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) == 0 && SDL_IsGameController(param_joystickindex)) {
    GameController = SDL_GameControllerOpen(param_joystickindex);
    printf("Using game controller: %s\n", SDL_GameControllerName(GameController));
    SDL_GameControllerEventState(SDL_IGNORE);
    JoyNumButtons = SDL_CONTROLLER_BUTTON_MAX;
    JoyNumAxes    = SDL_CONTROLLER_AXIS_MAX;
    JoyNumHats    = 0;

    // JoySensitivity = new  JoystickSens[JoyNumAxes];
  }

  SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);

  if (fullscreen || forcegrabmouse) {
    GrabInput = true;
    SDL_SetRelativeMouseMode(true);
  }

  // I didn't find a way to ask libSDL whether a mouse is present, yet...
  MousePresent = true;

  IN_Started = true;
}

///////////////////////////////////////////////////////////////////////////
//
//  IN_Shutdown() - Shuts down the Input Mgr
//
///////////////////////////////////////////////////////////////////////////
void IN_Shutdown(void) {
  if (!IN_Started)
    return;

  if (GameController)
    SDL_GameControllerClose(GameController);

  IN_Started = false;
}

///////////////////////////////////////////////////////////////////////////
//
//  IN_ClearKeysDown() - Clears the keyboard array
//
///////////////////////////////////////////////////////////////////////////
void IN_ClearKeysDown(void) {
  LastScan  = sc_None;
  LastASCII = key_None;

  memset(Keyboard, 0, sizeof(Keyboard));
  // for (auto &pair : Keyboard) {
  //   pair.second = false;
  // }
}

///////////////////////////////////////////////////////////////////////////
//
//  IN_ReadControl() - Reads the device associated with the specified
//      player and fills in the control info struct
//
///////////////////////////////////////////////////////////////////////////
void IN_ReadControl(int player __attribute__((unused)), ControlInfo *info) {
  word   buttons;
  int    dx, dy;
  Motion mx, my;

  dx = dy = 0;
  mx = my = motion_None;
  buttons = 0;

  IN_ProcessEvents();

  if (Keyboard[KbdDefs.upleft])
    mx = motion_Left, my = motion_Up;
  else if (Keyboard[KbdDefs.upright])
    mx = motion_Right, my = motion_Up;
  else if (Keyboard[KbdDefs.downleft])
    mx = motion_Left, my = motion_Down;
  else if (Keyboard[KbdDefs.downright])
    mx = motion_Right, my = motion_Down;

  if (Keyboard[KbdDefs.up])
    my = motion_Up;
  else if (Keyboard[KbdDefs.down])
    my = motion_Down;

  if (Keyboard[KbdDefs.left])
    mx = motion_Left;
  else if (Keyboard[KbdDefs.right])
    mx = motion_Right;

  if (Keyboard[KbdDefs.button0])
    buttons += 1 << 0;
  if (Keyboard[KbdDefs.button1])
    buttons += 1 << 1;

  dx = mx * 127;
  dy = my * 127;

  info->x       = dx;
  info->xaxis   = mx;
  info->y       = dy;
  info->yaxis   = my;
  info->button0 = (buttons & (1 << 0)) != 0;
  info->button1 = (buttons & (1 << 1)) != 0;
  info->button2 = (buttons & (1 << 2)) != 0;
  info->button3 = (buttons & (1 << 3)) != 0;
  info->dir     = DirTable[((my + 1) * 3) + (mx + 1)];
}

///////////////////////////////////////////////////////////////////////////
//
//  IN_Ack() - waits for a button or key press.  If a button is down, upon
// calling, it must be released for it to be recognized
//
///////////////////////////////////////////////////////////////////////////

boolean btnstate[NUMBUTTONS];

void IN_StartAck(void) {
  IN_ProcessEvents();
  //
  // get initial state of everything
  //
  IN_ClearKeysDown();
  memset(btnstate, 0, sizeof(btnstate));

  int buttons = IN_JoyButtons() << 4;

  if (MousePresent)
    buttons |= IN_MouseButtons();

  for (int i = 0; i < NUMBUTTONS; i++, buttons >>= 1)
    if (buttons & 1)
      btnstate[i] = true;
}

boolean IN_CheckAck(void) {
  IN_ProcessEvents();
  //
  // see if something has been pressed
  //
  if (LastScan)
    return true;

  int buttons = IN_JoyButtons() << 4;

  if (MousePresent)
    buttons |= IN_MouseButtons();

  for (int i = 0; i < NUMBUTTONS; i++, buttons >>= 1) {
    if (buttons & 1) {
      if (!btnstate[i]) {
        // Wait until button has been released
        do {
          IN_WaitAndProcessEvents();
          buttons = IN_JoyButtons() << 4;

          if (MousePresent)
            buttons |= IN_MouseButtons();
        } while (buttons & (1 << i));

        return true;
      }
    } else
      btnstate[i] = false;
  }

  return false;
}

void IN_Ack(void) {
  IN_StartAck();

  do {
    IN_WaitAndProcessEvents();
  } while (!IN_CheckAck());
}

///////////////////////////////////////////////////////////////////////////
//
//  IN_UserInput() - Waits for the specified delay time (in ticks) or the
//      user pressing a key or a mouse button. If the clear flag is set, it
//      then either clears the key or waits for the user to let the mouse
//      button up.
//
///////////////////////////////////////////////////////////////////////////
boolean IN_UserInput(longword delay) {
  longword lasttime;

  lasttime = GetTimeCount();
  IN_StartAck();
  do {
    IN_ProcessEvents();
    if (IN_CheckAck())
      return true;
    SDL_Delay(5);
  } while (GetTimeCount() - lasttime < delay);
  return (false);
}

//===========================================================================

/*
===================
=
= IN_MouseButtons
=
===================
*/
int IN_MouseButtons(void) {
  if (MousePresent)
    return INL_GetMouseButtons();
  else
    return 0;
}

bool IN_IsInputGrabbed() { return GrabInput; }
