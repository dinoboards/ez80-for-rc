//
//  ID Engine
//  ID_IN.h - Header file for Input Manager
//  v1.0d1
//  By Jason Blochowiak
//

#ifndef __ID_IN__
#define __ID_IN__

#ifdef __DEBUG__
#define __DEBUG_InputMgr__
#endif

typedef uint8_t ScanCode;
#define sc_None  0
#define key_None 0

typedef enum { demo_Off, demo_Record, demo_Playback, demo_PlayDone } Demo;
typedef enum {
  ctrl_Keyboard,
  ctrl_Keyboard1 = ctrl_Keyboard,
  ctrl_Keyboard2,
  ctrl_Joystick,
  ctrl_Joystick1 = ctrl_Joystick,
  ctrl_Joystick2,
  ctrl_Mouse
} ControlType;
typedef enum { motion_Left = -1, motion_Up = -1, motion_None = 0, motion_Right = 1, motion_Down = 1 } Motion;
typedef enum {
  dir_North,
  dir_NorthEast,
  dir_East,
  dir_SouthEast,
  dir_South,
  dir_SouthWest,
  dir_West,
  dir_NorthWest,
  dir_None
} Direction;
typedef struct {
  boolean   button0, button1, button2, button3;
  short     x, y;
  Motion    xaxis, yaxis;
  Direction dir;
} CursorInfo;
typedef CursorInfo ControlInfo;

// Global variables
#define NumCodes 256
extern boolean Keyboard[NumCodes];

// I do not see multiple threads reading this
extern boolean  MousePresent;
extern boolean  Paused;
extern char     LastASCII;
extern ScanCode LastScan;
extern boolean  forcegrabmouse;

// Function prototypes
#define IN_KeyDown(code) (Keyboard[(uint8_t)(code)])
#define IN_ClearKey(code)                                                                                                          \
  {                                                                                                                                \
    Keyboard[(uint8_t)(code)] = false;                                                                                             \
    if (code == LastScan)                                                                                                          \
      LastScan = sc_None;                                                                                                          \
  }

// DEBUG - put names in prototypes
extern void        IN_Startup(void), IN_Shutdown(void);
extern void        IN_ClearKeysDown(void);
extern void        IN_ReadControl(int, ControlInfo *);
extern void        IN_StopDemo(void), IN_FreeDemoBuffer(void);
extern void        IN_Ack(void);
extern void        IN_Ack_AndPreload(void);
extern boolean     IN_UserInput(uint24_t delay);
extern boolean     IN_UserInput_AndPreload(uint24_t delay);
extern const char *IN_GetScanName(ScanCode);

void IN_WaitAndProcessEvents();
void IN_ProcessEvents();

int IN_MouseButtons(void);

void    IN_StartAck(void);
boolean IN_CheckAck(void);

#endif
