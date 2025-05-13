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
#include "ez80.h"
#include <ez80-firmware-usb.h>
#include <hbios.h>
#include <stdint.h>

#include "id_vl.h"

#include "wl_def.h"

#include "id_sd.h"

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

boolean  Paused;
char     LastASCII;
ScanCode LastScan;

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

static inline uint8_t INL_GetMouseButtons(void) { return io_mouse_buttons(); }

/**
 * @brief return true if a key event was received and loaded
 *
 * updates the Keyboard array for the key event
 *
 * @return uint8_t true if a key event was received
 */
static inline uint8_t processEvent() { return io_keyboard_event(Keyboard, &LastScan, &LastASCII); }

// block waiting for a key event
// key events are key down, followed eventually by keyup

void IN_WaitAndProcessEvents() {
  while (!processEvent())
    ;
}

void IN_WaitAndProcessEvents_AndPreload() {
  while (!processEvent())
    PM_Preload(NULL, true);
}

void IN_ProcessEvents() { processEvent(); }

///////////////////////////////////////////////////////////////////////////
//
//  IN_Startup() - Starts up the Input Mgr
//
///////////////////////////////////////////////////////////////////////////
void IN_Startup(void) {
  if (IN_Started)
    return;

  IN_ClearKeysDown();

  MousePresent = io_mouse_init();

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

  IN_Started = false;
}

///////////////////////////////////////////////////////////////////////////
//
//  IN_ClearKeysDown() - Clears the keyboard array
//
///////////////////////////////////////////////////////////////////////////
void IN_ClearKeysDown(void) {
  LastScan  = 0;
  LastASCII = 0;

  memset(Keyboard, 0, sizeof(Keyboard));
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

  // if (Keyboard[KbdDefs.xupleft])
  //   mx = motion_Left, my = motion_Up;
  // else if (Keyboard[KbdDefs.upright])
  //   mx = motion_Right, my = motion_Up;
  // else if (Keyboard[KbdDefs.downleft])
  //   mx = motion_Left, my = motion_Down;
  // else if (Keyboard[KbdDefs.downright])
  //   mx = motion_Right, my = motion_Down;

  if (Keyboard[USB_KEY_UP])
    my = motion_Up;
  else if (Keyboard[USB_KEY_DOWN])
    my = motion_Down;

  if (Keyboard[USB_KEY_LEFT])
    mx = motion_Left;
  else if (Keyboard[USB_KEY_RIGHT])
    mx = motion_Right;

  // if (Keyboard[KbdDefs.button0])
  //   buttons += 1 << 0;
  // if (Keyboard[KbdDefs.button1])
  //   buttons += 1 << 1;

  dx = mx * 31;
  dy = my * 31;

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

  int buttons = 0;

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

  int buttons = 0;

  if (MousePresent)
    buttons |= IN_MouseButtons();

  for (int i = 0; i < NUMBUTTONS; i++, buttons >>= 1) {
    if (buttons & 1) {
      if (!btnstate[i]) {
        // Wait until button has been released
        do {
          IN_WaitAndProcessEvents();
          buttons = 0;

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

void IN_Ack_AndPreload(void) {
  IN_StartAck();

  do {
    IN_WaitAndProcessEvents_AndPreload();

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
boolean IN_UserInput(uint24_t delay) {
  longword lasttime;

  lasttime = GetTimeCount();
  IN_StartAck();
  do {
    IN_ProcessEvents();
    if (IN_CheckAck())
      return true;
  } while (GetTimeCount() - lasttime < delay);
  return (false);
}

boolean IN_UserInput_AndPreload(uint24_t delay) {
  longword lasttime;

  lasttime = GetTimeCount();
  IN_StartAck();
  do {
    PM_Preload(NULL, true);
    IN_ProcessEvents();
    if (IN_CheckAck())
      return true;
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
