// WL_PLAY.C

#include "ez80.h"
#include "wl_def.h"
#include <hbios.h>

#include "id_mm.h"

/*
=============================================================================

                                                                                                 LOCAL CONSTANTS

=============================================================================
*/

#define sc_Question 0x35

/*
=============================================================================

                                                                                                 GLOBAL VARIABLES

=============================================================================
*/

boolean madenoise; // true when shooting or screaming

exit_t playstate;

static musicnames lastmusicchunk = (musicnames)0;

objtype  objlist[MAXACTORS];
objtype *newobj, *obj, *player, *lastobj, *objfreelist, *killerobj;

boolean noclip, ammocheat;
int     godmode, singlestep, extravbls = 0;

byte     tilemap[MAPSIZE][MAPSIZE]; // wall values only
byte     spotvis[MAPSIZE][MAPSIZE];
objtype *actorat[MAPSIZE][MAPSIZE];

//
// replacing refresh manager
//
unsigned tics;

//
// control info
//
boolean mouseenabled, joystickenabled;
int     dirscan[4]             = {USB_KEY_UP, USB_KEY_RIGHT, USB_KEY_DOWN, USB_KEY_LEFT};
int     buttonscan[NUMBUTTONS] = {USB_KEY_ENTER, USB_KEY_RCTRL, USB_KEY_LSHIFT, USB_KEY_SPACE,
                                  USB_KEY_1,     USB_KEY_2,     USB_KEY_3,      USB_KEY_4};
int     buttonmouse[4]         = {bt_attack, bt_strafe, bt_use, bt_nobutton};
int     buttonjoy[32]          = {
    bt_attack,     bt_strafe,     bt_use,      bt_run,      bt_strafeleft, bt_straferight, bt_esc,      bt_pause,
    bt_prevweapon, bt_nextweapon, bt_nobutton, bt_nobutton, bt_nobutton,   bt_nobutton,    bt_nobutton, bt_nobutton,

    bt_nobutton,   bt_nobutton,   bt_nobutton, bt_nobutton, bt_nobutton,   bt_nobutton,    bt_nobutton, bt_nobutton,
    bt_nobutton,   bt_nobutton,   bt_nobutton, bt_nobutton, bt_nobutton,   bt_nobutton,    bt_nobutton, bt_nobutton};

int viewsize;

boolean buttonheld[NUMBUTTONS];

boolean demorecord, demoplayback;
int8_t *demoptr, *lastdemoptr;
memptr  demobuffer;

//
// current user input
//
int     controlx, controly; // range from -100 to 100 per tic
boolean buttonstate[NUMBUTTONS];

int lastgamemusicoffset = 0;

//===========================================================================

void CenterWindow(word w, word h);
void InitObjList(void);
void RemoveObj(objtype *gone);
void PollControls(void);
int  StopMusic(void);
void StartMusic(void);
void ContinueMusic(int offs);
void PlayLoop(void);

/*
=============================================================================

                                                                                                 LOCAL VARIABLES

=============================================================================
*/

objtype dummyobj;

//
// LIST OF SONGS FOR EACH VERSION
//
int songs[] = {
#ifndef SPEAR
    //
    // Episode One
    //
    GETTHEM_MUS, SEARCHN_MUS, POW_MUS, SUSPENSE_MUS, GETTHEM_MUS, SEARCHN_MUS, POW_MUS, SUSPENSE_MUS,

    WARMARCH_MUS, // Boss level
    CORNER_MUS,   // Secret level

    //
    // Episode Two
    //
    NAZI_OMI_MUS, PREGNANT_MUS, GOINGAFT_MUS, HEADACHE_MUS, NAZI_OMI_MUS, PREGNANT_MUS, HEADACHE_MUS, GOINGAFT_MUS,

    WARMARCH_MUS, // Boss level
    DUNGEON_MUS,  // Secret level

    //
    // Episode Three
    //
    INTROCW3_MUS, NAZI_RAP_MUS, TWELFTH_MUS, ZEROHOUR_MUS, INTROCW3_MUS, NAZI_RAP_MUS, TWELFTH_MUS, ZEROHOUR_MUS,

    ULTIMATE_MUS, // Boss level
    PACMAN_MUS,   // Secret level

    //
    // Episode Four
    //
    GETTHEM_MUS, SEARCHN_MUS, POW_MUS, SUSPENSE_MUS, GETTHEM_MUS, SEARCHN_MUS, POW_MUS, SUSPENSE_MUS,

    WARMARCH_MUS, // Boss level
    CORNER_MUS,   // Secret level

    //
    // Episode Five
    //
    NAZI_OMI_MUS, PREGNANT_MUS, GOINGAFT_MUS, HEADACHE_MUS, NAZI_OMI_MUS, PREGNANT_MUS, HEADACHE_MUS, GOINGAFT_MUS,

    WARMARCH_MUS, // Boss level
    DUNGEON_MUS,  // Secret level

    //
    // Episode Six
    //
    INTROCW3_MUS, NAZI_RAP_MUS, TWELFTH_MUS, ZEROHOUR_MUS, INTROCW3_MUS, NAZI_RAP_MUS, TWELFTH_MUS, ZEROHOUR_MUS,

    ULTIMATE_MUS, // Boss level
    FUNKYOU_MUS   // Secret level
#else

    //////////////////////////////////////////////////////////////
    //
    // SPEAR OF DESTINY TRACKS
    //
    //////////////////////////////////////////////////////////////
    XTIPTOE_MUS,  XFUNKIE_MUS,  XDEATH_MUS,
    XGETYOU_MUS,  // DON'T KNOW
    ULTIMATE_MUS, // Trans Grösse

    DUNGEON_MUS,  GOINGAFT_MUS, POW_MUS,      TWELFTH_MUS,
    ULTIMATE_MUS, // Barnacle Wilhelm BOSS

    NAZI_OMI_MUS, GETTHEM_MUS,  SUSPENSE_MUS, SEARCHN_MUS, ZEROHOUR_MUS,
    ULTIMATE_MUS, // Super Mutant BOSS

    XPUTIT_MUS,
    ULTIMATE_MUS, // Death Knight BOSS

    XJAZNAZI_MUS, // Secret level
    XFUNKIE_MUS,  // Secret level (DON'T KNOW)

    XEVIL_MUS // Angel of Death BOSS
#endif
};

/*
=============================================================================

                                                           USER CONTROL

=============================================================================
*/

/*
===================
=
= PollKeyboardButtons
=
===================
*/

void PollKeyboardButtons(void) {
  int i;

  for (i = 0; i < NUMBUTTONS; i++)
    if (Keyboard[buttonscan[i]])
      buttonstate[i] = true;
}

/*
===================
=
= PollMouseButtons
=
===================
*/

void PollMouseButtons(void) {
  int buttons = IN_MouseButtons();

  if (buttons & 1)
    buttonstate[buttonmouse[0]] = true;
  if (buttons & 2)
    buttonstate[buttonmouse[1]] = true;
  if (buttons & 4)
    buttonstate[buttonmouse[2]] = true;
}

/*
===================
=
= PollKeyboardMove
=
===================
*/

void PollKeyboardMove(void) {
  int delta = buttonstate[bt_run] ? RUNMOVE * tics : BASEMOVE * tics;

  if (Keyboard[dirscan[di_north]])
    controly -= delta;
  if (Keyboard[dirscan[di_south]])
    controly += delta;
  if (Keyboard[dirscan[di_west]])
    controlx -= delta;
  if (Keyboard[dirscan[di_east]])
    controlx += delta;
}

/*
===================
=
= PollMouseMove
=
===================
*/

void PollMouseMove(void) {
  io_mouse_poll();

  controlx += usb_mouse_report.x * 10 / (13 - mouseadjustment);
  controly += usb_mouse_report.y * 20 / (13 - mouseadjustment);
}

/*
===================
=
= PollControls
=
= Gets user or demo input, call once each frame
=
= controlx              set between -100 and 100 per tic
= controly
= buttonheld[]  the state of the buttons LAST frame
= buttonstate[] the state of the buttons THIS frame
=
===================
*/

void PollControls(void) {
  int  max, min, i;
  byte buttonbits;

  IN_ProcessEvents();

  //
  // get timing info for last frame
  //
  if (demoplayback) {
    while (GetTimeCount() < lasttimecount + DEMOTICS)
      ;
    SetTimeCount(lasttimecount + DEMOTICS);
    lasttimecount += DEMOTICS;
    tics = DEMOTICS;
  } else if (demorecord) // demo recording and playback needs
  {                      // to be constant
                         //
                         // take DEMOTICS or more tics, and modify Timecount to reflect time taken
                         //
    while (GetTimeCount() < lasttimecount + DEMOTICS)
      ;
    SetTimeCount(lasttimecount + DEMOTICS);
    lasttimecount += DEMOTICS;
    tics = DEMOTICS;
  } else
    CalcTics();

  controlx = 0;
  controly = 0;
  memcpy(buttonheld, buttonstate, sizeof(buttonstate));
  memset(buttonstate, 0, sizeof(buttonstate));

  if (demoplayback) {
    //
    // read commands from demo buffer
    //
    buttonbits = *demoptr++;
    for (i = 0; i < NUMBUTTONS; i++) {
      buttonstate[i] = buttonbits & 1;
      buttonbits >>= 1;
    }

    controlx = *demoptr++;
    controly = *demoptr++;

    if (demoptr == lastdemoptr)
      playstate = ex_completed; // demo is done

    controlx *= (int)tics;
    controly *= (int)tics;

    return;
  }

  //
  // get button states
  //
  PollKeyboardButtons();

  if (mouseenabled)
    PollMouseButtons();

  //
  // get movements
  //
  PollKeyboardMove();

  if (mouseenabled)
    PollMouseMove();

  //
  // bound movement to a maximum
  //
  max = 100 * tics;
  min = -max;
  if (controlx > max)
    controlx = max;
  else if (controlx < min)
    controlx = min;

  if (controly > max)
    controly = max;
  else if (controly < min)
    controly = min;

  if (demorecord) {
    //
    // save info out to demo buffer
    //
    controlx /= (int)tics;
    controly /= (int)tics;

    buttonbits = 0;

    // TODO: Support 32-bit buttonbits
    for (i = NUMBUTTONS - 1; i >= 0; i--) {
      buttonbits <<= 1;
      if (buttonstate[i])
        buttonbits |= 1;
    }

    *demoptr++ = buttonbits;
    *demoptr++ = controlx;
    *demoptr++ = controly;

    if (demoptr >= lastdemoptr - 8)
      playstate = ex_completed;
    else {
      controlx *= (int)tics;
      controly *= (int)tics;
    }
  }
}

//==========================================================================

///////////////////////////////////////////////////////////////////////////
//
//      CenterWindow() - Generates a window of a given width & height in the
//              middle of the screen
//
///////////////////////////////////////////////////////////////////////////
#define MAXX SCREEN_WIDTH_FACTOR(320)
#define MAXY 160

#ifdef APP_DEBUG
void CenterWindow(word w, word h) { US_DrawWindow(((MAXX / 8) - w) / 2, ((MAXY / 8) - h) / 2, w, h); }
#endif

//===========================================================================

/*
=====================
=
= CheckKeys
=
=====================
*/

void CheckKeys(void) {
  ScanCode scan;

  if (screenfaded || demoplayback) // don't do anything with a faded screen
    return;

  scan = LastScan;

#ifdef SPEAR
  //
  // SECRET CHEAT CODE: TAB-G-F10
  //
  if (Keyboard[sc_Tab] && Keyboard[sc_G] && Keyboard[sc_F10]) {
    WindowH = 160;
    if (godmode) {
      Message("God mode OFF");
      SD_PlaySound(NOBONUSSND);
    } else {
      Message("God mode ON");
      SD_PlaySound(ENDBONUS2SND);
    }

    IN_Ack();
    godmode ^= 1;
    DrawPlayBorderSides();
    IN_ClearKeysDown();
    return;
  }
#endif

  //
  // SECRET CHEAT CODE: 'MLI'
  //
  if (Keyboard[USB_KEY_M] && Keyboard[USB_KEY_L] && Keyboard[USB_KEY_I]) {
    gamestate.health = 100;
    gamestate.ammo   = 99;
    gamestate.keys   = 3;
    gamestate.score  = 0;
    gamestate.TimeCount += 42000L;
    GiveWeapon(wp_chaingun);
    DrawWeapon();
    DrawHealth();
    DrawKeys();
    DrawAmmo();
    DrawScore();

    ClearMemory();
    CA_CacheGrChunk(STARTFONT + 1);
    ClearSplitVWB();

    Message(STR_CHEATER1 "\n" STR_CHEATER2 "\n\n" STR_CHEATER3 "\n" STR_CHEATER4 "\n" STR_CHEATER5);

    UNCACHEGRCHUNK(STARTFONT + 1);
    IN_ClearKeysDown();
    IN_Ack();

    if (viewsize < 17)
      DrawPlayBorder();
  }

  //
  // TRYING THE KEEN CHEAT CODE!
  //
  if (Keyboard[USB_KEY_B] && Keyboard[USB_KEY_A] && Keyboard[USB_KEY_T]) {
    ClearMemory();
    CA_CacheGrChunk(STARTFONT + 1);
    ClearSplitVWB();

    Message("Commander Keen is also\n"
            "available from Apogee, but\n"
            "then, you already know\n"
            "that - right, Cheatmeister?!");

    UNCACHEGRCHUNK(STARTFONT + 1);
    IN_ClearKeysDown();
    IN_Ack();

    if (viewsize < 18)
      DrawPlayBorder();
  }

  //
  // pause key weirdness can't be checked as a scan code
  //
  if (buttonstate[bt_pause])
    Paused = true;
  if (Paused) {
    int lastoffs = StopMusic();
    LatchDrawPic(SCREEN_WIDTH_FACTOR((20 - 4) * 8), 80 - 2 * 8, PAUSEDPIC);
    VH_UpdateScreen();
    IN_Ack();
    Paused = false;
    ContinueMusic(lastoffs);
    lasttimecount = GetTimeCount();
    return;
  }

  //
  // F1-F7/ESC to enter control panel
  //
  if (
#ifndef DEBCHECK
      scan == USB_KEY_F10 ||
#endif
      scan == USB_KEY_F9 || scan == USB_KEY_F7 || scan == USB_KEY_F8) // pop up quit dialog
  {
    ClearMemory();
    ClearSplitVWB();
    US_ControlPanel(scan);

    DrawPlayBorderSides();

    SETFONTCOLOR(0, 15);
    IN_ClearKeysDown();
    return;
  }

  if ((scan >= USB_KEY_F1 && scan <= USB_KEY_F9) || scan == USB_KEY_ESCAPE || buttonstate[bt_esc]) {
    int lastoffs = StopMusic();
    ClearMemory();
    VW_FadeOut();

    US_ControlPanel(buttonstate[bt_esc] ? USB_KEY_ESCAPE : scan);

    SETFONTCOLOR(0, 15);
    IN_ClearKeysDown();
    VW_FadeOut();
    DrawPlayScreen();
    if (!startgame && !loadedgame)
      ContinueMusic(lastoffs);
    if (loadedgame)
      playstate = ex_abort;
    lasttimecount = GetTimeCount();
    return;
  }
}

//===========================================================================

/*
#############################################################################

                                                                  The objlist data structure

#############################################################################

objlist containt structures for every actor currently playing.  The structure
is accessed as a linked list starting at *player, ending when ob->next ==
NULL.  GetNewObj inserts a new object at the end of the list, meaning that
if an actor spawn another actor, the new one WILL get to think and react the
same frame.  RemoveObj unlinks the given object and returns it to the free
list, but does not damage the objects ->next pointer, so if the current object
removes itself, a linked list following loop can still safely get to the
next element.

<backwardly linked free list>

#############################################################################
*/

/*
=========================
=
= InitActorList
=
= Call to clear out the actor object lists returning them all to the free
= list.  Allocates a special spot for the player.
=
=========================
*/

int objcount;

void InitActorList(void) {
  int i;

  //
  // init the actor lists
  //
  for (i = 0; i < MAXACTORS; i++) {
    objlist[i].prev = &objlist[i + 1];
    objlist[i].next = NULL;
  }

  objlist[MAXACTORS - 1].prev = NULL;

  objfreelist = &objlist[0];
  lastobj     = NULL;

  objcount = 0;

  //
  // give the player the first free spots
  //
  GetNewActor();
  player = newobj;
}

//===========================================================================

/*
=========================
=
= GetNewActor
=
= Sets the global variable new to point to a free spot in objlist.
= The free spot is inserted at the end of the liked list
=
= When the object list is full, the caller can either have it bomb out ot
= return a dummy object pointer that will never get used
=
=========================
*/

void GetNewActor(void) {
  if (!objfreelist)
    Quit("GetNewActor: No free spots in objlist!");

  newobj      = objfreelist;
  objfreelist = newobj->prev;
  memset(newobj, 0, sizeof(*newobj));

  if (lastobj)
    lastobj->next = newobj;
  newobj->prev = lastobj; // new->next is allready NULL from memset

  newobj->active = ac_no;
  lastobj        = newobj;

  objcount++;
}

//===========================================================================

/*
=========================
=
= RemoveObj
=
= Add the given object back into the free list, and unlink it from it's
= neighbors
=
=========================
*/

void RemoveObj(objtype *gone) {
  if (gone == player)
    Quit("RemoveObj: Tried to remove the player!");

  gone->state = NULL;

  //
  // fix the next object's back link
  //
  if (gone == lastobj)
    lastobj = (objtype *)gone->prev;
  else
    gone->next->prev = gone->prev;

  //
  // fix the previous object's forward link
  //
  gone->prev->next = gone->next;

  //
  // add it back in to the free list
  //
  gone->prev  = objfreelist;
  objfreelist = gone;

  objcount--;
}

/*
=============================================================================

                                                                                                MUSIC STUFF

=============================================================================
*/

/*
=================
=
= StopMusic
=
=================
*/
int StopMusic(void) {
  int lastoffs = SD_MusicOff();

  UNCACHEAUDIOCHUNK(STARTMUSIC + lastmusicchunk);

  return lastoffs;
}

//==========================================================================

/*
=================
=
= StartMusic
=
=================
*/

void StartMusic() {
  SD_MusicOff();
  lastmusicchunk = (musicnames)songs[gamestate.mapon + gamestate.episode * 10];
  SD_StartMusic(STARTMUSIC + lastmusicchunk);
}

void ContinueMusic(int offs __attribute__((unused))) {
  SD_MusicOff();
  lastmusicchunk = (musicnames)songs[gamestate.mapon + gamestate.episode * 10];
  SD_ContinueMusic(STARTMUSIC + lastmusicchunk, offs);
}

/*
=============================================================================

                                                                                PALETTE SHIFTING STUFF

=============================================================================
*/

#define NUMREDSHIFTS 6
#define REDSTEPS     8

#define NUMWHITESHIFTS 3
#define WHITESTEPS     20
#define WHITETICS      6

GRB_t redshifts[NUMREDSHIFTS][256];
GRB_t whiteshifts[NUMWHITESHIFTS][256];

int     damagecount, bonuscount;
boolean palshifted;

/*
=====================
=
= InitRedShifts
=
=====================
*/

void InitRedShifts(void) {
  GRB_t *workptr, *baseptr;
  int    i, j, delta;

  //
  // fade through intermediate frames
  //
  for (i = 1; i <= NUMREDSHIFTS; i++) {
    workptr = redshifts[i - 1];
    baseptr = gamepal;

    for (j = 0; j <= 255; j++) {
      delta                   = 256 - RED_FRM_GRB(*baseptr);
      const uint8_t new_red   = RED_FRM_GRB(*baseptr) + delta * i / REDSTEPS;
      delta                   = -GREEN_FRM_GRB(*baseptr);
      const uint8_t new_green = GREEN_FRM_GRB(*baseptr) + delta * i / REDSTEPS;
      delta                   = -BLUE_FRM_GRB(*baseptr);
      const uint8_t new_blue  = BLUE_FRM_GRB(*baseptr) + delta * i / REDSTEPS;

      *workptr = GRB(new_green, new_red, new_blue);
      baseptr++;
      workptr++;
    }
  }

  for (i = 1; i <= NUMWHITESHIFTS; i++) {
    workptr = whiteshifts[i - 1];
    baseptr = gamepal;

    for (j = 0; j <= 255; j++) {
      delta             = 256 - RED_FRM_GRB(*baseptr);
      uint8_t new_red   = RED_FRM_GRB(*baseptr) + delta * i / WHITESTEPS;
      delta             = 248 - GREEN_FRM_GRB(*baseptr);
      uint8_t new_green = GREEN_FRM_GRB(*baseptr) + delta * i / WHITESTEPS;
      delta             = 0 - BLUE_FRM_GRB(*baseptr);
      uint8_t new_blue  = BLUE_FRM_GRB(*baseptr) + delta * i / WHITESTEPS;

      *workptr = GRB(new_green, new_red, new_blue);

      baseptr++;
      workptr++;
    }
  }
}

/*
=====================
=
= ClearPaletteShifts
=
=====================
*/

void ClearPaletteShifts(void) {
  bonuscount = damagecount = 0;
  palshifted               = false;
}

/*
=====================
=
= StartBonusFlash
=
=====================
*/

void StartBonusFlash(void) {
  bonuscount = NUMWHITESHIFTS * WHITETICS; // white shift palette
}

/*
=====================
=
= StartDamageFlash
=
=====================
*/

void StartDamageFlash(int damage) { damagecount += damage; }

/*
=====================
=
= UpdatePaletteShifts
=
=====================
*/

void UpdatePaletteShifts(void) {
  int red, white;

  if (bonuscount) {
    white = bonuscount / WHITETICS + 1;
    if (white > NUMWHITESHIFTS)
      white = NUMWHITESHIFTS;
    bonuscount -= tics;
    if (bonuscount < 0)
      bonuscount = 0;
  } else
    white = 0;

  if (damagecount) {
    red = damagecount / 10 + 1;
    if (red > NUMREDSHIFTS)
      red = NUMREDSHIFTS;

    damagecount -= tics;
    if (damagecount < 0)
      damagecount = 0;
  } else
    red = 0;

  if (red) {
    VL_SetPalette(redshifts[red - 1], false);
    palshifted = true;
  } else if (white) {
    VL_SetPalette(whiteshifts[white - 1], false);
    palshifted = true;
  } else if (palshifted) {
    VL_SetPalette(gamepal, false); // back to normal
    palshifted = false;
  }
}

/*
=====================
=
= FinishPaletteShifts
=
= Resets palette to normal if needed
=
=====================
*/

void FinishPaletteShifts(void) {
  if (palshifted) {
    palshifted = 0;
    VL_SetPalette(gamepal, true);
  }
}

/*
=============================================================================

                                                                                                CORE PLAYLOOP

=============================================================================
*/

/*
=====================
=
= DoActor
=
=====================
*/

void DoActor(objtype *ob) {
  void (*think)(objtype *);

  if (!ob->active && !areabyplayer[ob->areanumber])
    return;

  if (!(ob->flags & (FL_NONMARK | FL_NEVERMARK)))
    actorat[ob->tilex][ob->tiley] = NULL;

  //
  // non transitional object
  //

  if (!ob->ticcount) {
    think = (void (*)(objtype *))ob->state->think;
    if (think) {
      think(ob);
      if (!ob->state) {
        RemoveObj(ob);
        return;
      }
    }

    if (ob->flags & FL_NEVERMARK)
      return;

    if ((ob->flags & FL_NONMARK) && actorat[ob->tilex][ob->tiley])
      return;

    actorat[ob->tilex][ob->tiley] = ob;
    return;
  }

  //
  // transitional object
  //
  ob->ticcount -= (short)tics;
  while (ob->ticcount <= 0) {
    think = (void (*)(objtype *))ob->state->action; // end of state action
    if (think) {
      think(ob);
      if (!ob->state) {
        RemoveObj(ob);
        return;
      }
    }

    ob->state = ob->state->next;

    if (!ob->state) {
      RemoveObj(ob);
      return;
    }

    if (!ob->state->tictime) {
      ob->ticcount = 0;
      goto think;
    }

    ob->ticcount += ob->state->tictime;
  }

think:
  //
  // think
  //
  think = (void (*)(objtype *))ob->state->think;
  if (think) {
    think(ob);
    if (!ob->state) {
      RemoveObj(ob);
      return;
    }
  }

  if (ob->flags & FL_NEVERMARK)
    return;

  if ((ob->flags & FL_NONMARK) && actorat[ob->tilex][ob->tiley])
    return;

  actorat[ob->tilex][ob->tiley] = ob;
}

//==========================================================================

/*
===================
=
= PlayLoop
=
===================
*/
#ifdef SPEAR
int32_t funnyticount;
#endif

void PlayLoop(void) {
  playstate     = ex_stillplaying;
  lasttimecount = GetTimeCount();
  frameon       = 0;
  anglefrac     = 0;
  facecount     = 0;
#ifdef SPEAR
  funnyticount = 0;
#endif
  memset(buttonstate, 0, sizeof(buttonstate));
  ClearPaletteShifts();

  if (demoplayback)
    IN_StartAck();

  do {
    PollControls();

    //
    // actor thinking
    //
    madenoise = false;

    MoveDoors();

    MovePWalls();

    for (obj = player; obj; obj = obj->next)
      DoActor(obj);

    UpdatePaletteShifts();

    ThreeDRefresh();

    //
    // MAKE FUNNY FACE IF BJ DOESN'T MOVE FOR AWHILE
    //
#ifdef SPEAR
    funnyticount += tics;
    if (funnyticount > 30l * 70) {
      funnyticount = 0;
      StatusDrawFace(BJWAITING1PIC + (US_RndT() & 1));
      facecount = 0;
    }
#endif

    gamestate.TimeCount += tics;

    UpdateSoundLoc(); // JAB
    if (screenfaded)
      VW_FadeIn();

    CheckKeys();

    //
    // debug aids
    //
    if (singlestep) {
      VW_WaitVBL(singlestep);
      lasttimecount = GetTimeCount();
    }
    if (extravbls)
      VW_WaitVBL(extravbls);

    if (demoplayback) {
      if (IN_CheckAck()) {
        IN_ClearKeysDown();
        playstate = ex_abort;
      }
    }

  } while (!playstate && !startgame);

  if (playstate != ex_died)
    FinishPaletteShifts();
}
