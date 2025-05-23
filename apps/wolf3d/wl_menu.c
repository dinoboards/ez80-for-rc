////////////////////////////////////////////////////////////////////
//
// WL_MENU.C
// by John Romero (C) 1992 Id Software, Inc.
//
////////////////////////////////////////////////////////////////////

#include "ez80.h"
#include "wl_def.h"
#include <hbios.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "id_mm.h"

extern int lastgamemusicoffset;
extern int numEpisodesMissing;

//
// PRIVATE PROTOTYPES
//
int CP_ReadThis(int);

#ifdef SPEAR
#define STARTITEM newgame

#else
#ifdef GOODTIMES
#define STARTITEM newgame

#else
#define STARTITEM readthis
#endif
#endif

// ENDSTRx constants are defined in foreign.h
char endStrings[9][80] = {ENDSTR1, ENDSTR2, ENDSTR3, ENDSTR4, ENDSTR5, ENDSTR6, ENDSTR7, ENDSTR8, ENDSTR9};

CP_itemtype MainMenu[] = {
#ifdef JAPAN
    {1, "", CP_NewGame},
    {1, "", CP_Sound},
    {1, "", CP_Control},
    {1, "", CP_LoadGame},
    {0, "", CP_SaveGame},
    {1, "", CP_ChangeView},
    {2, "", CP_ReadThis},
    {1, "", CP_ViewScores},
    {1, "", 0},
    {1, "", 0}
#else

    {1, STR_NG, CP_NewGame},
    {1, STR_SD, CP_Sound},
    {1, STR_CL, CP_Control},
    {1, STR_LG, CP_LoadGame},
    {0, STR_SG, CP_SaveGame},
    {1, STR_CV, CP_ChangeView},

#ifndef GOODTIMES
#ifndef SPEAR

#ifdef SPANISH
    {2, "Ve esto!", CP_ReadThis},
#else
    {2, "Read This!", CP_ReadThis},
#endif

#endif
#endif

    {1, STR_VS, CP_ViewScores},
    {1, STR_BD, 0},
    {1, STR_QT, 0}
#endif
};

CP_itemtype SndMenu[] = {
#ifdef JAPAN
    {1, "", 0}, {1, "", 0}, {1, "", 0}, {0, "", 0}, {0, "", 0}, {1, "", 0},
    {1, "", 0}, {1, "", 0}, {0, "", 0}, {0, "", 0}, {1, "", 0}, {1, "", 0},
#else
    {1, STR_NONE, 0}, {0, STR_PC, 0}, {1, STR_ALSB, 0}, {0, "", 0},       {0, "", 0},      {1, STR_NONE, 0}, {0, STR_DISNEY, 0},
    {1, STR_SB, 0},   {0, "", 0},     {0, "", 0},       {1, STR_NONE, 0}, {1, STR_ALSB, 0}
#endif
};

enum { CTL_MOUSEENABLE, CTL_MOUSESENS, CTL_JOYENABLE, CTL_CUSTOMIZE };

CP_itemtype CtlMenu[] = {
#ifdef JAPAN
    {0, "", 0}, {0, "", MouseSensitivity}, {0, "", 0}, {1, "", CustomControls}
#else
    {0, STR_MOUSEEN, 0}, {0, STR_SENS, MouseSensitivity}, {0, STR_JOYEN, 0}, {1, STR_CUSTOM, CustomControls}
#endif
};

#ifndef SPEAR
CP_itemtype NewEmenu[] = {
#ifdef JAPAN
#ifdef JAPDEMO
    {1, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0},
    {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0},
#else
    {1, "", 0}, {0, "", 0}, {1, "", 0}, {0, "", 0}, {1, "", 0}, {0, "", 0}, {1, "", 0},
    {0, "", 0}, {1, "", 0}, {0, "", 0}, {1, "", 0}, {0, "", 0}
#endif
#else
#ifdef SPANISH
    {1,
     "Episodio 1\n"
     "Fuga desde Wolfenstein",
     0},
    {0, "", 0},
    {3,
     "Episodio 2\n"
     "Operacion Eisenfaust",
     0},
    {0, "", 0},
    {3,
     "Episodio 3\n"
     "Muere, Fuhrer, Muere!",
     0},
    {0, "", 0},
    {3,
     "Episodio 4\n"
     "Un Negro Secreto",
     0},
    {0, "", 0},
    {3,
     "Episodio 5\n"
     "Huellas del Loco",
     0},
    {0, "", 0},
    {3,
     "Episodio 6\n"
     "Confrontacion",
     0}
#else
    {1,
     "Episode 1\n"
     "Wolfenstein",
     0},
    {0, "", 0},
    {3,
     "Episode 2\n"
     "Eisenfaust",
     0},
    {0, "", 0},
    {3,
     "Episode 3\n"
     "Die, Fuhrer, Die!",
     0},
    {0, "", 0},
    {3,
     "Episode 4\n"
     "A Dark Secret",
     0},
    {0, "", 0},
    {3,
     "Episode 5\n"
     "Trail of the Mad",
     0},
    {0, "", 0},
    {3,
     "Episode 6\n"
     "Confrontation",
     0}
#endif
#endif
};
#endif

CP_itemtype NewMenu[] = {
#ifdef JAPAN
    {1, "", 0}, {1, "", 0}, {1, "", 0}, {1, "", 0}
#else
    {1, STR_DADDY, 0}, {1, STR_HURTME, 0}, {1, STR_BRINGEM, 0}, {1, STR_DEATH, 0}
#endif
};

CP_itemtype LSMenu[] = {{1, "", 0}, {1, "", 0}, {1, "", 0}, {1, "", 0}, {1, "", 0}, {1, "", 0}, {1, "", 0}, {1, "", 0}, {1, "", 0}};

CP_itemtype CusMenu[] = {{1, "", 0}, {0, "", 0}, {0, "", 0}, {1, "", 0}, {0, "", 0},
                         {0, "", 0}, {1, "", 0}, {0, "", 0}, {1, "", 0}};

// CP_iteminfo struct format: short x, y, amount, curpos, indent;
CP_iteminfo MainItems = {MENU_X, MENU_Y, lengthof(MainMenu), STARTITEM, 38};
CP_iteminfo SndItems  = {SM_X, SM_Y1, lengthof(SndMenu), 0, 64};
CP_iteminfo LSItems   = {LSM_X, LSM_Y, lengthof(LSMenu), 0, 30};
CP_iteminfo CtlItems  = {CTL_X, CTL_Y, lengthof(CtlMenu), -1, 56};
CP_iteminfo CusItems  = {1, CST_Y + 13 * 2 - 4, lengthof(CusMenu), -1, 0};

#ifndef SPEAR
CP_iteminfo NewEitems = {NE_X, NE_Y, lengthof(NewEmenu), 0, 84};
#endif
CP_iteminfo NewItems = {NM_X, NM_Y, lengthof(NewMenu), 2, 30};

int color_hlite[] = {DEACTIVE, HIGHLIGHT, READHCOLOR, 0x67};

int color_norml[] = {DEACTIVE, TEXTCOLOR, READCOLOR, 0x6b};

int EpisodeSelect[6] = {1};

#define MAX_SAVE_GAMES 9

static int  SaveGamesAvail[10];
static int  StartGame;
static int  SoundStatus = 1;
static int  pickquick;
static char SaveGameNames[10][32];
static char SaveName[13] = "savegam?.";

////////////////////////////////////////////////////////////////////
//
// Wolfenstein Control Panel!  Ta Da!
//
////////////////////////////////////////////////////////////////////
void US_ControlPanel(ScanCode scancode) {
  printf("US_ControlPanel(%x)\r\n", scancode);
  int which;

  if (ingame) {
    if (CP_CheckQuick(scancode))
      return;
    lastgamemusicoffset = StartCPMusic(MENUSONG);
  } else
    StartCPMusic(MENUSONG);
  SetupControlPanel();

  //
  // F-KEYS FROM WITHIN GAME
  //
  switch (scancode) {
  case USB_KEY_F1:
#ifdef SPEAR
    BossKey();
#else
#ifdef GOODTIMES
    BossKey();
#else
    HelpScreens();
#endif
#endif
    goto finishup;

  case USB_KEY_F2:
    CP_SaveGame(0);
    goto finishup;

  case USB_KEY_F3:
    CP_LoadGame(0);
    goto finishup;

  case USB_KEY_F4:
    CP_Sound(0);
    goto finishup;

  case USB_KEY_F5:
    CP_ChangeView(0);
    goto finishup;

  case USB_KEY_F6:
    CP_Control(0);
    goto finishup;

  finishup:
    CleanupControlPanel();
#ifdef SPEAR
    UnCacheLump(OPTIONS_LUMP_START, OPTIONS_LUMP_END);
#endif
    return;
  }

#ifdef SPEAR
  CacheLump(OPTIONS_LUMP_START, OPTIONS_LUMP_END);
#endif

  DrawMainMenu();
  MenuFadeIn();
  StartGame = 0;

  //
  // MAIN MENU LOOP
  //
  do {
    which = HandleMenu(&MainItems, &MainMenu[0], NULL);
    // NewGame(1, 0);

#ifdef SPEAR
#ifndef SPEARDEMO
    IN_ProcessEvents();

    //
    // EASTER EGG FOR SPEAR OF DESTINY!
    //
    if (Keyboard[sc_I] && Keyboard[sc_D]) {
      VW_FadeOut();
      StartCPMusic(XJAZNAZI_MUS);
      UnCacheLump(OPTIONS_LUMP_START, OPTIONS_LUMP_END);
      UnCacheLump(BACKDROP_LUMP_START, BACKDROP_LUMP_END);
      ClearMemory();

      CA_CacheGrChunk(IDGUYS1PIC);
      VWB_DrawPic(0, 0, IDGUYS1PIC);
      UNCACHEGRCHUNK(IDGUYS1PIC);

      CA_CacheGrChunk(IDGUYS2PIC);
      VWB_DrawPic(0, 80, IDGUYS2PIC);
      UNCACHEGRCHUNK(IDGUYS2PIC);

      VW_UpdateScreen();

      GRB_t pal[256];
      CA_CacheGrChunk(IDGUYSPALETTE);
      VL_ConvertPalette(grsegs[IDGUYSPALETTE], pal, 256);
      VL_FadeIn(0, 255, pal, 30);
      UNCACHEGRCHUNK(IDGUYSPALETTE);

      while (Keyboard[sc_I] || Keyboard[sc_D])
        IN_WaitAndProcessEvents();
      IN_ClearKeysDown();
      IN_Ack();

      VW_FadeOut();

      CacheLump(BACKDROP_LUMP_START, BACKDROP_LUMP_END);
      CacheLump(OPTIONS_LUMP_START, OPTIONS_LUMP_END);
      DrawMainMenu();
      StartCPMusic(MENUSONG);
      MenuFadeIn();
    }
#endif
#endif

    switch (which) {
    case viewscores:
      if (MainMenu[viewscores].routine == NULL) {
        if (CP_EndGame(0))
          StartGame = 1;
      } else {
        DrawMainMenu();
        MenuFadeIn();
      }
      break;

    case backtodemo:
      StartGame = 1;
      if (!ingame)
        StartCPMusic(INTROSONG);
      VL_FadeOut(0, 255, 0, 0, 0, 10);
      break;

    case -1:
    case quit:
      CP_Quit(0);
      break;

    default:
      if (!StartGame) {
        DrawMainMenu();
        MenuFadeIn();
      }
    }

    //
    // "EXIT OPTIONS" OR "NEW GAME" EXITS
    //
  } while (!StartGame);

  //
  // DEALLOCATE EVERYTHING
  //
  CleanupControlPanel();

  //
  // CHANGE MAINMENU ITEM
  //
  if (startgame || loadedgame)
    EnableEndGameMenuItem();

    // RETURN/START GAME EXECUTION

#ifdef SPEAR
  UnCacheLump(OPTIONS_LUMP_START, OPTIONS_LUMP_END);
#endif
}

void EnableEndGameMenuItem() {
  MainMenu[viewscores].routine = NULL;
#ifndef JAPAN
  strcpy(MainMenu[viewscores].string, STR_EG);
#endif
}

////////////////////////
//
// DRAW MAIN MENU SCREEN
//
void DrawMainMenu(void) {
#ifdef JAPAN
  CA_CacheScreen(S_OPTIONSPIC);
#else
  ClearMScreen();

  VWB_DrawPic(76, SCREEN_HEIGHT - 16, C_MOUSELBACKPIC); // 104, 16
  DrawStripes(10);
  VWB_DrawPic(52, 0, C_OPTIONSPIC); // 152, 48

#ifdef SPANISH
  DrawWindow(MENU_X, MENU_Y, MENU_W, MENU_H, BKGDCOLOR);
#else
  DrawWindow(MENU_X, MENU_Y, MENU_W, MENU_H, BKGDCOLOR);
#endif
#endif

  //
  // CHANGE "GAME" AND "DEMO"
  //
  if (ingame) {
#ifndef JAPAN

#ifdef SPANISH
    strcpy(&MainMenu[backtodemo].string, STR_GAME);
#else
    strcpy(&MainMenu[backtodemo].string[8], STR_GAME);
#endif

#else
    CA_CacheGrChunk(C_MRETGAMEPIC);
    VWB_DrawPic(12 * 8, 20 * 8, C_MRETGAMEPIC);
    UNCACHEGRCHUNK(C_MRETGAMEPIC);
    CA_CacheGrChunk(C_MENDGAMEPIC);
    VWB_DrawPic(12 * 8, 18 * 8, C_MENDGAMEPIC);
    UNCACHEGRCHUNK(C_MENDGAMEPIC);
#endif
    MainMenu[backtodemo].active = 2;
  } else {
#ifndef JAPAN
#ifdef SPANISH
    strcpy(&MainMenu[backtodemo].string, STR_BD);
#else
    strcpy(&MainMenu[backtodemo].string[8], STR_DEMO);
#endif
#else
    CA_CacheGrChunk(C_MRETDEMOPIC);
    VWB_DrawPic(12 * 8, 20 * 8, C_MRETDEMOPIC);
    UNCACHEGRCHUNK(C_MRETDEMOPIC);
    CA_CacheGrChunk(C_MSCORESPIC);
    VWB_DrawPic(12 * 8, 18 * 8, C_MSCORESPIC);
    UNCACHEGRCHUNK(C_MSCORESPIC);
#endif
    MainMenu[backtodemo].active = 1;
  }

  DrawMenu(&MainItems, &MainMenu[0]);

  // VW_UpdateScreen();
}

#ifndef GOODTIMES
#ifndef SPEAR
////////////////////////////////////////////////////////////////////
//
// READ THIS!
//
////////////////////////////////////////////////////////////////////
int CP_ReadThis(int _ __attribute__((unused))) {
  StartCPMusic(CORNER_MUS);
  HelpScreens();
  StartCPMusic(MENUSONG);
  return true;
}
#endif
#endif

#ifdef GOODTIMES
////////////////////////////////////////////////////////////////////
//
// BOSS KEY
//
////////////////////////////////////////////////////////////////////
void BossKey(void) {
#ifdef NOTYET
  byte palette1[256][3];
  SD_MusicOff();
  /*       _AX = 3;
                  geninterrupt(0x10); */
  _asm
      {
		mov eax, 3 int 0x10}
  puts("C>");
  SetTextCursor(2, 0);
  //      while (!Keyboard[USB_KEY_ESCAPE])
  IN_Ack();
  IN_ClearKeysDown();

  SD_MusicOn();
  VL_InitVideoMode();
  for (int i = 0; i < 768; i++)
    palette1[0][i] = 0;

  VL_SetPalette(&palette1[0][0]);

  LoadLatchMem();
#endif
}
#else
#ifdef SPEAR
void BossKey(void) {
#ifdef NOTYET
  byte palette1[256][3];
  SD_MusicOff();
  /*       _AX = 3;
                  geninterrupt(0x10); */
  _asm
      {
		mov eax, 3 int 0x10}
  puts("C>");
  SetTextCursor(2, 0);
  //      while (!Keyboard[USB_KEY_ESCAPE])
  IN_Ack();
  IN_ClearKeysDown();

  SD_MusicOn();
  VL_InitVideoMode();
  for (int i = 0; i < 768; i++)
    palette1[0][i] = 0;

  VL_SetPalette(&palette1[0][0]);

  LoadLatchMem();
#endif
}
#endif
#endif

////////////////////////////////////////////////////////////////////
//
// CHECK QUICK-KEYS & QUIT (WHILE IN A GAME)
//
////////////////////////////////////////////////////////////////////
int CP_CheckQuick(ScanCode scancode) {
  switch (scancode) {
    //
    // END GAME
    //
  case USB_KEY_F7:
    CA_CacheGrChunk(STARTFONT + 1);

    WindowH = 160;
#ifdef JAPAN
    if (GetYorN(7, 8, C_JAPQUITPIC))
#else
    if (Confirm(ENDGAMESTR))
#endif
    {
      playstate = ex_died;
      killerobj = NULL;
      pickquick = gamestate.lives = 0;
    }

    WindowH                   = 200;
    fontnumber                = 0;
    MainMenu[savegame].active = 0;
    return 1;

    //
    // QUICKSAVE
    //
  case USB_KEY_F8:
    if (SaveGamesAvail[LSItems.curpos] && pickquick) {
      CA_CacheGrChunk(STARTFONT + 1);
      fontnumber = 1;
      Message(STR_SAVING "...");
      CP_SaveGame(1);
      fontnumber = 0;
    } else {
#ifndef SPEAR
      CA_CacheGrChunk(STARTFONT + 1);
      CA_CacheGrChunk(C_CURSOR1PIC);
      CA_CacheGrChunk(C_CURSOR2PIC);
      CA_CacheGrChunk(C_DISKLOADING1PIC);
      CA_CacheGrChunk(C_DISKLOADING2PIC);
      CA_CacheGrChunk(C_SAVEGAMEPIC);
      CA_CacheGrChunk(C_MOUSELBACKPIC);
#else
      CacheLump(BACKDROP_LUMP_START, BACKDROP_LUMP_END);
      CA_CacheGrChunk(C_CURSOR1PIC);
#endif

      VW_FadeOut();
      if (screenHeight % 200 != 0)
        VL_ClearScreen(0);

      lastgamemusicoffset = StartCPMusic(MENUSONG);
      pickquick           = CP_SaveGame(0);

      SETFONTCOLOR(0, 15);
      IN_ClearKeysDown();
      VW_FadeOut();
      DrawPlayScreen();

      if (!startgame && !loadedgame)
        ContinueMusic(lastgamemusicoffset);

      if (loadedgame)
        playstate = ex_abort;
      lasttimecount = GetTimeCount();

#ifndef SPEAR
      UNCACHEGRCHUNK(C_CURSOR1PIC);
      UNCACHEGRCHUNK(C_CURSOR2PIC);
      UNCACHEGRCHUNK(C_DISKLOADING1PIC);
      UNCACHEGRCHUNK(C_DISKLOADING2PIC);
      UNCACHEGRCHUNK(C_SAVEGAMEPIC);
      UNCACHEGRCHUNK(C_MOUSELBACKPIC);
#else
      UnCacheLump(BACKDROP_LUMP_START, BACKDROP_LUMP_END);
#endif
    }
    return 1;

    //
    // QUICKLOAD
    //
  case USB_KEY_F9:
    if (SaveGamesAvail[LSItems.curpos] && pickquick) {
      char string[100] = STR_LGC;

      CA_CacheGrChunk(STARTFONT + 1);
      fontnumber = 1;

      strcat(string, SaveGameNames[LSItems.curpos]);
      strcat(string, "\"?");

      if (Confirm(string))
        CP_LoadGame(1);

      fontnumber = 0;
    } else {
#ifndef SPEAR
      CA_CacheGrChunk(STARTFONT + 1);
      CA_CacheGrChunk(C_CURSOR1PIC);
      CA_CacheGrChunk(C_CURSOR2PIC);
      CA_CacheGrChunk(C_DISKLOADING1PIC);
      CA_CacheGrChunk(C_DISKLOADING2PIC);
      CA_CacheGrChunk(C_LOADGAMEPIC);
      CA_CacheGrChunk(C_MOUSELBACKPIC);
#else
      CA_CacheGrChunk(C_CURSOR1PIC);
      CacheLump(BACKDROP_LUMP_START, BACKDROP_LUMP_END);
#endif

      VW_FadeOut();
      if (screenHeight % 200 != 0)
        VL_ClearScreen(0);

      lastgamemusicoffset = StartCPMusic(MENUSONG);
      pickquick           = CP_LoadGame(0); // loads lastgamemusicoffs

      SETFONTCOLOR(0, 15);
      IN_ClearKeysDown();
      VW_FadeOut();
      DrawPlayScreen();

      if (!startgame && !loadedgame)
        ContinueMusic(lastgamemusicoffset);

      if (loadedgame)
        playstate = ex_abort;

      lasttimecount = GetTimeCount();

#ifndef SPEAR
      UNCACHEGRCHUNK(C_CURSOR1PIC);
      UNCACHEGRCHUNK(C_CURSOR2PIC);
      UNCACHEGRCHUNK(C_DISKLOADING1PIC);
      UNCACHEGRCHUNK(C_DISKLOADING2PIC);
      UNCACHEGRCHUNK(C_LOADGAMEPIC);
      UNCACHEGRCHUNK(C_MOUSELBACKPIC);
#else
      UnCacheLump(BACKDROP_LUMP_START, BACKDROP_LUMP_END);
#endif
    }
    return 1;

    //
    // QUIT
    //
  case USB_KEY_F10:
    CA_CacheGrChunk(STARTFONT + 1);

    WindowX = WindowY = 0;
    WindowW           = 256;
    WindowH           = 160;
#ifdef JAPAN
    if (GetYorN(7, 8, C_QUITMSGPIC))
#else
#ifdef SPANISH
    if (Confirm(ENDGAMESTR))
#else
    if (Confirm(endStrings[US_RndT() & 0x7 + (US_RndT() & 1)]))
#endif
#endif
    {
      VW_UpdateScreen();
      SD_MusicOff();
      SD_StopSound();
      MenuFadeOut();

      Quit(NULL);
    }

    DrawPlayBorder();
    WindowH    = 200;
    fontnumber = 0;
    return 1;
  }

  return 0;
}

////////////////////////////////////////////////////////////////////
//
// END THE CURRENT GAME
//
////////////////////////////////////////////////////////////////////
int CP_EndGame(int _ __attribute__((unused))) {
  int res;
#ifdef JAPAN
  res = GetYorN(7, 8, C_JAPQUITPIC);
#else
  res = Confirm(ENDGAMESTR);
#endif
  DrawMainMenu();
  if (!res)
    return 0;

  pickquick = gamestate.lives = 0;
  playstate                   = ex_died;
  killerobj                   = NULL;

  MainMenu[savegame].active    = 0;
  MainMenu[viewscores].routine = CP_ViewScores;
#ifndef JAPAN
  strcpy(MainMenu[viewscores].string, STR_VS);
#endif

  return 1;
}

////////////////////////////////////////////////////////////////////
//
// VIEW THE HIGH SCORES
//
////////////////////////////////////////////////////////////////////
int CP_ViewScores(int _ __attribute__((unused))) {
  fontnumber = 0;

#ifdef SPEAR
  UnCacheLump(OPTIONS_LUMP_START, OPTIONS_LUMP_END);
  StartCPMusic(XAWARD_MUS);
#else
  StartCPMusic(ROSTER_MUS);
#endif

  DrawHighScores();
  VW_UpdateScreen();
  MenuFadeIn();
  fontnumber = 1;

  IN_Ack();

  StartCPMusic(MENUSONG);
  MenuFadeOut();

#ifdef SPEAR
  CacheLump(BACKDROP_LUMP_START, BACKDROP_LUMP_END);
  CacheLump(OPTIONS_LUMP_START, OPTIONS_LUMP_END);
#endif
  return 0;
}

////////////////////////////////////////////////////////////////////
//
// START A NEW GAME
//
////////////////////////////////////////////////////////////////////
int CP_NewGame(int _ __attribute__((unused))) {
  int which, episode;

#ifdef SPEAR
  UnCacheLump(OPTIONS_LUMP_START, OPTIONS_LUMP_END);
#endif

#ifndef SPEAR
firstpart:

  DrawNewEpisode();
  do {
    which = HandleMenu(&NewEitems, &NewEmenu[0], NULL);
    switch (which) {
    case -1:
      MenuFadeOut();
      return 0;

    default:
      if (!EpisodeSelect[which / 2]) {
        SD_PlaySound(NOWAYSND);
        Message("Please select \"Read This!\"\n"
                "from the Options menu to\n"
                "find out how to order this\n"
                "episode from Apogee.");
        IN_ClearKeysDown();
        IN_Ack();
        DrawNewEpisode();
        which = 0;
      } else {
        episode = which / 2;
        which   = 1;
      }
      break;
    }
  } while (!which);

  ShootSnd();

  //
  // ALREADY IN A GAME?
  //
  if (ingame)
#ifdef JAPAN
    if (!GetYorN(7, 8, C_JAPNEWGAMEPIC))
#else
    if (!Confirm(CURGAME))
#endif
    {
      MenuFadeOut();
      return 0;
    }

  MenuFadeOut();

#else
  episode = 0;

  //
  // ALREADY IN A GAME?
  //
  CacheLump(NEWGAME_LUMP_START, NEWGAME_LUMP_END);
  DrawNewGame();
  if (ingame)
    if (!Confirm(CURGAME)) {
      MenuFadeOut();
      UnCacheLump(NEWGAME_LUMP_START, NEWGAME_LUMP_END);
      CacheLump(OPTIONS_LUMP_START, OPTIONS_LUMP_END);
      return 0;
    }

#endif

  DrawNewGame();
  which = HandleMenu(&NewItems, &NewMenu[0], DrawNewGameDiff);
  if (which < 0) {
    MenuFadeOut();
#ifndef SPEAR
    goto firstpart;
#else
    UnCacheLump(NEWGAME_LUMP_START, NEWGAME_LUMP_END);
    CacheLump(OPTIONS_LUMP_START, OPTIONS_LUMP_END);
    return 0;
#endif
  }

  ShootSnd();
  NewGame(which, episode);
  StartGame = 1;
  MenuFadeOut();

  //
  // CHANGE "READ THIS!" TO NORMAL COLOR
  //
#ifndef SPEAR
#ifndef GOODTIMES
  MainMenu[readthis].active = 1;
#endif
#endif

  pickquick = 0;

#ifdef SPEAR
  UnCacheLump(NEWGAME_LUMP_START, NEWGAME_LUMP_END);
  CacheLump(OPTIONS_LUMP_START, OPTIONS_LUMP_END);
#endif

  return 0;
}

#ifndef SPEAR
/////////////////////
//
// DRAW NEW EPISODE MENU
//
void DrawNewEpisode(void) {
  int i;

#ifdef JAPAN
  CA_CacheScreen(S_EPISODEPIC);
#else
  ClearMScreen();
  VWB_DrawPic(76, SCREEN_HEIGHT - 16, C_MOUSELBACKPIC); // 104, 16

  DrawWindow(NE_X, NE_Y, NE_W, NE_H, BKGDCOLOR);
  SETFONTCOLOR(READHCOLOR, BKGDCOLOR);
  PrintY  = 2;
  WindowX = 0;
#ifdef SPANISH
  US_CPrint("Cual episodio jugar?");
#else
  US_CPrint("Which episode to play?");
#endif
#endif

  SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);
  DrawMenu(&NewEitems, &NewEmenu[0]);

  for (i = 0; i < 6; i++)
    VWB_DrawPic(NE_X + 31, NE_Y + 1 + i * 26, C_EPISODE1PIC + i);

  VW_UpdateScreen();
  MenuFadeIn();
  WaitKeyUp();
}
#endif

/////////////////////
//
// DRAW NEW GAME MENU
//
void DrawNewGame(void) {
#ifdef JAPAN
  CA_CacheScreen(S_SKILLPIC);
#else
  ClearMScreen();
  VWB_DrawPic(76, SCREEN_HEIGHT - 16, C_MOUSELBACKPIC); // 104, 16

  SETFONTCOLOR(READHCOLOR, BKGDCOLOR);
  PrintX = NM_X + 16;
  PrintY = NM_Y - 30;

#ifndef SPEAR
#ifdef SPANISH
  US_Print("Eres macho?");
#else
  US_Print("How tough are you?");
#endif
#else
  VWB_DrawPic(PrintX, PrintY, C_HOWTOUGHPIC);
#endif
  DrawWindow(NM_X, NM_Y, NM_W, NM_H, BKGDCOLOR);
#endif

  DrawMenu(&NewItems, &NewMenu[0]);
  DrawNewGameDiff(NewItems.curpos);
  VW_UpdateScreen();
  MenuFadeIn();
  WaitKeyUp();
}

////////////////////////
//
// DRAW NEW GAME GRAPHIC
//
void DrawNewGameDiff(int w) { VWB_DrawPic(SCREEN_WIDTH - 24 - 8, NM_Y + 7, w + C_BABYMODEPIC); }

////////////////////////////////////////////////////////////////////
//
// HANDLE SOUND MENU
//
////////////////////////////////////////////////////////////////////
int CP_Sound(int _ __attribute__((unused))) {
  int which;

#ifdef SPEAR
  UnCacheLump(OPTIONS_LUMP_START, OPTIONS_LUMP_END);
  CacheLump(SOUND_LUMP_START, SOUND_LUMP_END);
#endif

  DrawSoundMenu();
  MenuFadeIn();
  WaitKeyUp();

  do {
    which = HandleMenu(&SndItems, &SndMenu[0], NULL);
    //
    // HANDLE MENU CHOICES
    //
    switch (which) {
      //
      // SOUND EFFECTS
      //
    case 0:
      if (SoundMode != sdm_Off) {
        SD_WaitSoundDone();
        SD_SetSoundMode(sdm_Off);
        DrawSoundMenu();
      }
      break;
    case 1:
      if (SoundMode != sdm_PC) {
        SD_WaitSoundDone();
        SD_SetSoundMode(sdm_PC);
        CA_LoadAllSounds();
        DrawSoundMenu();
        ShootSnd();
      }
      break;
    case 2:
      if (SoundMode != sdm_AdLib) {
        SD_WaitSoundDone();
        SD_SetSoundMode(sdm_AdLib);
        CA_LoadAllSounds();
        DrawSoundMenu();
        ShootSnd();
      }
      break;

      //
      // DIGITIZED SOUND
      //
    case 5:
      if (DigiMode != sds_Off) {
        SD_SetDigiDevice(sds_Off);
        DrawSoundMenu();
      }
      break;
    case 6:
      /*                if (DigiMode != sds_SoundSource)
                                      {
                                              SD_SetDigiDevice (sds_SoundSource);
                                              DrawSoundMenu ();
                                              ShootSnd ();
                                      }*/
      break;
    case 7:
      if (DigiMode != sds_SoundBlaster) {
        SD_SetDigiDevice(sds_SoundBlaster);
        DrawSoundMenu();
        ShootSnd();
      }
      break;

      //
      // MUSIC
      //
    case 10:
      if (MusicMode != smm_Off) {
        SD_SetMusicMode(smm_Off);
        DrawSoundMenu();
        ShootSnd();
      }
      break;
    case 11:
      if (MusicMode != smm_AdLib) {
        SD_SetMusicMode(smm_AdLib);
        DrawSoundMenu();
        ShootSnd();
        StartCPMusic(MENUSONG);
      }
      break;
    }
  } while (which >= 0);

  MenuFadeOut();

#ifdef SPEAR
  UnCacheLump(SOUND_LUMP_START, SOUND_LUMP_END);
  CacheLump(OPTIONS_LUMP_START, OPTIONS_LUMP_END);
#endif
  return 0;
}

//////////////////////
//
// DRAW THE SOUND MENU
//
void DrawSoundMenu(void) {
  int i, on;

#ifdef JAPAN
  CA_CacheScreen(S_SOUNDPIC);
#else
  //
  // DRAW SOUND MENU
  //
  ClearMScreen();
  VWB_DrawPic(76, SCREEN_HEIGHT - 16, C_MOUSELBACKPIC); // 104, 16

  DrawWindow(SM_X, SM_Y1, SM_W, SM_H1, BKGDCOLOR);
  DrawWindow(SM_X, SM_Y2, SM_W, SM_H2, BKGDCOLOR);
  DrawWindow(SM_X, SM_Y3, SM_W, SM_H3, BKGDCOLOR);
#endif

  //
  // IF NO ADLIB, NON-CHOOSENESS!
  //
  if (!AdLibPresent && !SoundBlasterPresent) {
    SndMenu[2].active = SndMenu[10].active = SndMenu[11].active = 0;
  }

  if (!SoundBlasterPresent)
    SndMenu[7].active = 0;

  if (!SoundBlasterPresent)
    SndMenu[5].active = 0;

  DrawMenu(&SndItems, &SndMenu[0]);
#ifndef JAPAN
  VWB_DrawPic(60, SM_Y1 - 15, C_FXTITLEPIC);
  VWB_DrawPic(60, SM_Y2 - 15, C_DIGITITLEPIC);
  VWB_DrawPic(60, SM_Y3 - 15, C_MUSICTITLEPIC);
#endif

  for (i = 0; i < SndItems.amount; i++)
#ifdef JAPAN
    if (i != 3 && i != 4 && i != 8 && i != 9)
#else
    if (SndMenu[i].string[0])
#endif
    {
      //
      // DRAW SELECTED/NOT SELECTED GRAPHIC BUTTONS
      //
      on = 0;
      switch (i) {
        //
        // SOUND EFFECTS
        //
      case 0:
        if (SoundMode == sdm_Off)
          on = 1;
        break;
      case 1:
        if (SoundMode == sdm_PC)
          on = 1;
        break;
      case 2:
        if (SoundMode == sdm_AdLib)
          on = 1;
        break;

        //
        // DIGITIZED SOUND
        //
      case 5:
        if (DigiMode == sds_Off)
          on = 1;
        break;
      case 6:
        //                    if (DigiMode == sds_SoundSource)
        //                        on = 1;
        break;
      case 7:
        if (DigiMode == sds_SoundBlaster)
          on = 1;
        break;

        //
        // MUSIC
        //
      case 10:
        if (MusicMode == smm_Off)
          on = 1;
        break;
      case 11:
        if (MusicMode == smm_AdLib)
          on = 1;
        break;
      }

      if (on)
        VWB_DrawPic(SM_X + 38, SM_Y1 + 2 + i * 13 + 2, C_SELECTEDPIC);
      else
        VWB_DrawPic(SM_X + 38, SM_Y1 + 2 + i * 13 + 2, C_NOTSELECTEDPIC);
    }

  DrawMenuGun(&SndItems);
  VW_UpdateScreen();
}

//
// DRAW LOAD/SAVE IN PROGRESS
//
void DrawLSAction(int which) {
#define LSA_X 96
#define LSA_Y 80
#define LSA_W 130
#define LSA_H 42

  DrawWindow(LSA_X, LSA_Y, LSA_W, LSA_H, TEXTCOLOR);
  DrawOutline(LSA_X, LSA_Y, LSA_W, LSA_H, 0, HIGHLIGHT);
  VWB_DrawPic(LSA_X + 8, LSA_Y + 5, C_DISKLOADING1PIC);

  fontnumber = 1;
  SETFONTCOLOR(0, TEXTCOLOR);
  PrintX = LSA_X + 46;
  PrintY = LSA_Y + 13;

  if (!which)
    US_Print(STR_LOADING "...");
  else
    US_Print(STR_SAVING "...");

  VW_UpdateScreen();
}

////////////////////////////////////////////////////////////////////
//
// LOAD SAVED GAMES
//
////////////////////////////////////////////////////////////////////
int CP_LoadGame(int quick) {
  FILE *file;
  int   which, exit = 0;
  char  name[13];
  char  loadpath[300];

  strcpy(name, SaveName);

  //
  // QUICKLOAD?
  //
  if (quick) {
    which = LSItems.curpos;

    if (SaveGamesAvail[which]) {
      name[7] = which + '0';

      if (configdir[0])
        snprintf(loadpath, sizeof(loadpath), "%s/%s", configdir, name);
      else
        strcpy(loadpath, name);

      file = fopen(loadpath, "rb");
      printf("Reading file %s\r\n", loadpath);

      fseek(file, 32, SEEK_SET);
      loadedgame = true;
      LoadTheGame(file, 0, 0);
      loadedgame = false;
      fclose(file);

      DrawFace();
      DrawHealth();
      DrawLives();
      DrawLevel();
      DrawAmmo();
      DrawKeys();
      DrawWeapon();
      DrawScore();
      ContinueMusic(lastgamemusicoffset);
      return 1;
    }
  }

#ifdef SPEAR
  UnCacheLump(OPTIONS_LUMP_START, OPTIONS_LUMP_END);
  CacheLump(LOADSAVE_LUMP_START, LOADSAVE_LUMP_END);
#endif

  DrawLoadSaveScreen(0);

  do {
    which = HandleMenu(&LSItems, &LSMenu[0], TrackWhichGame);
    if (which >= 0 && SaveGamesAvail[which]) {
      ShootSnd();
      name[7] = which + '0';

      if (configdir[0])
        snprintf(loadpath, sizeof(loadpath), "%s/%s", configdir, name);
      else
        strcpy(loadpath, name);

      file = fopen(loadpath, "rb");
      printf("Reading file %s\r\n", loadpath);

      fseek(file, 32, SEEK_SET);

      DrawLSAction(0);
      loadedgame = true;

      LoadTheGame(file, LSA_X + 8, LSA_Y + 5);
      fclose(file);

      StartGame = 1;
      ShootSnd();
      //
      // CHANGE "READ THIS!" TO NORMAL COLOR
      //

#ifndef SPEAR
#ifndef GOODTIMES
      MainMenu[readthis].active = 1;
#endif
#endif

      exit = 1;
      break;
    }
  } while (which >= 0);

  MenuFadeOut();

#ifdef SPEAR
  UnCacheLump(LOADSAVE_LUMP_START, LOADSAVE_LUMP_END);
  CacheLump(OPTIONS_LUMP_START, OPTIONS_LUMP_END);
#endif

  return exit;
}

///////////////////////////////////
//
// HIGHLIGHT CURRENT SELECTED ENTRY
//
void TrackWhichGame(int w) {
  static int lastgameon = 0;

  PrintLSEntry(lastgameon, TEXTCOLOR);
  PrintLSEntry(w, HIGHLIGHT);

  lastgameon = w;
}

////////////////////////////
//
// DRAW THE LOAD/SAVE SCREEN
//
void DrawLoadSaveScreen(int loadsave) {
#define DISKX 100
#define DISKY 0

  int i;

  ClearMScreen();
  fontnumber = 1;
  VWB_DrawPic(76, SCREEN_HEIGHT - 16, C_MOUSELBACKPIC); // 104, 16
  DrawWindow(LSM_X, LSM_Y, LSM_W, LSM_H, BKGDCOLOR);
  DrawStripes(10);

  if (!loadsave)
    VWB_DrawPic(20, 0, C_LOADGAMEPIC); // 216, 48
  else
    VWB_DrawPic(20, 0, C_SAVEGAMEPIC);

  for (i = 0; i < MAX_SAVE_GAMES; i++)
    PrintLSEntry(i, TEXTCOLOR);

  DrawMenu(&LSItems, &LSMenu[0]);
  VW_UpdateScreen();
  MenuFadeIn();
  WaitKeyUp();
}

///////////////////////////////////////////
//
// PRINT LOAD/SAVE GAME ENTRY W/BOX OUTLINE
//
void PrintLSEntry(int w, int color) {
  SETFONTCOLOR(color, BKGDCOLOR);
  DrawOutline(LSM_X + LSItems.indent, LSM_Y + w * 13 + 4, LSM_W - LSItems.indent - 15, 11, color, color);
  PrintX     = LSM_X + LSItems.indent + 2;
  PrintY     = LSM_Y + w * 13 + 5;
  fontnumber = 0;

  if (SaveGamesAvail[w])
    US_Print(SaveGameNames[w]);
  else
    US_Print("      - " STR_EMPTY " -");

  fontnumber = 1;
}

////////////////////////////////////////////////////////////////////
//
// SAVE CURRENT GAME
//
////////////////////////////////////////////////////////////////////
int CP_SaveGame(int quick) {
  int   which, exit = 0;
  FILE *file;
  char  name[13];
  char  savepath[300];
  char  input[32];

  strcpy(name, SaveName);

  //
  // QUICKSAVE?
  //
  if (quick) {
    which = LSItems.curpos;

    if (SaveGamesAvail[which]) {
      name[7] = which + '0';

      if (configdir[0])
        snprintf(savepath, sizeof(savepath), "%s/%s", configdir, name);
      else
        strcpy(savepath, name);

      unlink(savepath);
      file = fopen(savepath, "wb");
      printf("Saving file(1) %s\r\n", savepath);

      strcpy(input, &SaveGameNames[which][0]);

      fwrite(input, 1, 32, file);
      fseek(file, 32, SEEK_SET);
      SaveTheGame(file, 0, 0);
      fclose(file);

      return 1;
    }
  }

#ifdef SPEAR
  UnCacheLump(OPTIONS_LUMP_START, OPTIONS_LUMP_END);
  CacheLump(LOADSAVE_LUMP_START, LOADSAVE_LUMP_END);
#endif

  DrawLoadSaveScreen(1);

  do {
    which = HandleMenu(&LSItems, &LSMenu[0], TrackWhichGame);
    if (which >= 0) {
      //
      // OVERWRITE EXISTING SAVEGAME?
      //
      if (SaveGamesAvail[which]) {
#ifdef JAPAN
        if (!GetYorN(7, 8, C_JAPSAVEOVERPIC))
#else
        if (!Confirm(GAMESVD))
#endif
        {
          DrawLoadSaveScreen(1);
          continue;
        } else {
          DrawLoadSaveScreen(1);
          PrintLSEntry(which, HIGHLIGHT);
          VW_UpdateScreen();
        }
      }

      ShootSnd();

      strcpy(input, &SaveGameNames[which][0]);
      name[7] = which + '0';

      fontnumber = 0;
      if (!SaveGamesAvail[which]) {
        printf("SaveGame1\r\n");
        VWB_Bar(LSM_X + LSItems.indent + 1, LSM_Y + which * 13 + 1, LSM_W - LSItems.indent - 16, 10, BKGDCOLOR);
      }
      VW_UpdateScreen();

      if (US_LineInput(LSM_X + LSItems.indent + 2, LSM_Y + which * 13 + 5, input, input, true, 31, LSM_W - LSItems.indent - 30)) {
        SaveGamesAvail[which] = 1;
        strcpy(&SaveGameNames[which][0], input);

        if (configdir[0])
          snprintf(savepath, sizeof(savepath), "%s/%s", configdir, name);
        else
          strcpy(savepath, name);

        unlink(savepath);
        file = fopen(savepath, "wb");
        printf("Saving file(2) %s\r\n", savepath);

        fwrite(input, 32, 1, file);
        fseek(file, 32, SEEK_SET);

        DrawLSAction(1);
        SaveTheGame(file, LSA_X + 8, LSA_Y + 5);

        fclose(file);

        ShootSnd();
        exit = 1;
      } else {
        VWB_Bar(LSM_X + LSItems.indent + 1, LSM_Y + which * 13 + 1, LSM_W - LSItems.indent - 16, 10, BKGDCOLOR);
        PrintLSEntry(which, HIGHLIGHT);
        VW_UpdateScreen();
        SD_PlaySound(ESCPRESSEDSND);
        continue;
      }

      fontnumber = 1;
      break;
    }
  } while (which >= 0);

  MenuFadeOut();

#ifdef SPEAR
  UnCacheLump(LOADSAVE_LUMP_START, LOADSAVE_LUMP_END);
  CacheLump(OPTIONS_LUMP_START, OPTIONS_LUMP_END);
#endif

  return exit;
}

////////////////////////////////////////////////////////////////////
//
// DEFINE CONTROLS
//
////////////////////////////////////////////////////////////////////
int CP_Control(int _ __attribute__((unused))) {
  int which;

#ifdef SPEAR
  UnCacheLump(OPTIONS_LUMP_START, OPTIONS_LUMP_END);
  CacheLump(CONTROL_LUMP_START, CONTROL_LUMP_END);
#endif

  DrawCtlScreen();
  MenuFadeIn();
  WaitKeyUp();

  do {
    which = HandleMenu(&CtlItems, CtlMenu, NULL);
    switch (which) {
    case CTL_MOUSEENABLE:
      mouseenabled ^= 1;
      DrawCtlScreen();
      CusItems.curpos = -1;
      ShootSnd();
      break;

    case CTL_MOUSESENS:
    case CTL_CUSTOMIZE:
      DrawCtlScreen();
      MenuFadeIn();
      WaitKeyUp();
      break;
    }
  } while (which >= 0);

  MenuFadeOut();

#ifdef SPEAR
  UnCacheLump(CONTROL_LUMP_START, CONTROL_LUMP_END);
  CacheLump(OPTIONS_LUMP_START, OPTIONS_LUMP_END);
#endif
  return 0;
}

////////////////////////////////
//
// DRAW MOUSE SENSITIVITY SCREEN
//
void DrawMouseSens(void) {
#ifdef JAPAN
  CA_CacheScreen(S_MOUSESENSPIC);
#else
  ClearMScreen();
  VWB_DrawPic(76, SCREEN_HEIGHT - 16, C_MOUSELBACKPIC); // 104, 16
#ifdef SPANISH
  DrawWindow(10, 80, 256 - 16, 43, BKGDCOLOR);
#else
  DrawWindow(10, 80, 256 - 16, 30, BKGDCOLOR);
#endif

  WindowX = 0;
  WindowW = 256;
  PrintY = 82;
  SETFONTCOLOR(READCOLOR, BKGDCOLOR);
  US_CPrint(STR_MOUSEADJ);

  SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);
#ifdef SPANISH
  PrintX = 14;
  PrintY = 95 + 13;
  US_Print(STR_SLOW);
  PrintX = 252;
  US_Print(STR_FAST);
#else
  PrintX = 14;
  PrintY = 95;
  US_Print(STR_SLOW);
  PrintX = 269;
  US_Print(STR_FAST);
#endif
#endif

  VWB_Bar(SCREEN_WIDTH_FACTOR(60), 97, SCREEN_WIDTH_FACTOR(200), 10, TEXTCOLOR);
  DrawOutline(60, 97, 200, 10, 0, HIGHLIGHT);
  DrawOutline(60 + 20 * mouseadjustment, 97, 20, 10, 0, READCOLOR);
  VWB_Bar(SCREEN_WIDTH_FACTOR(61 + 20) * mouseadjustment, 98, SCREEN_WIDTH_FACTOR(19), 9, READHCOLOR);

  VW_UpdateScreen();
  MenuFadeIn();
}

///////////////////////////
//
// ADJUST MOUSE SENSITIVITY
//
int MouseSensitivity(int _ __attribute__((unused))) {
  ControlInfo ci;
  int         exit = 0, oldMA;

  oldMA = mouseadjustment;
  DrawMouseSens();
  do {
    ReadAnyControl(&ci);
    switch (ci.dir) {
    case dir_North:
    case dir_West:
      if (mouseadjustment) {
        mouseadjustment--;
        VWB_Bar(SCREEN_WIDTH_FACTOR(60), 97, SCREEN_WIDTH_FACTOR(200), 10, TEXTCOLOR);
        DrawOutline(60, 97, 200, 10, 0, HIGHLIGHT);
        DrawOutline(60 + 20 * mouseadjustment, 97, 20, 10, 0, READCOLOR);
        VWB_Bar(SCREEN_WIDTH_FACTOR(61 + 20 * mouseadjustment), 98, SCREEN_WIDTH_FACTOR(19), 9, READHCOLOR);
        VW_UpdateScreen();
        SD_PlaySound(MOVEGUN1SND);
        TicDelay(20);
      }
      break;

    case dir_South:
    case dir_East:
      if (mouseadjustment < 9) {
        mouseadjustment++;
        VWB_Bar(SCREEN_WIDTH_FACTOR(60), 97, SCREEN_WIDTH_FACTOR(200), 10, TEXTCOLOR);
        DrawOutline(60, 97, 200, 10, 0, HIGHLIGHT);
        DrawOutline(60 + 20 * mouseadjustment, 97, 20, 10, 0, READCOLOR);
        VWB_Bar(SCREEN_WIDTH_FACTOR(61 + 20 * mouseadjustment), 98, SCREEN_WIDTH_FACTOR(19), 9, READHCOLOR);
        VW_UpdateScreen();
        SD_PlaySound(MOVEGUN1SND);
        TicDelay(20);
      }
      break;

    default:
      break;
    }

    if (ci.button0 || Keyboard[USB_KEY_SPACE] || Keyboard[USB_KEY_ENTER])
      exit = 1;
    else if (ci.button1 || Keyboard[USB_KEY_ESCAPE])
      exit = 2;
  } while (!exit);

  if (exit == 2) {
    mouseadjustment = oldMA;
    SD_PlaySound(ESCPRESSEDSND);
  } else
    SD_PlaySound(SHOOTSND);

  WaitKeyUp();
  MenuFadeOut();

  return 0;
}

///////////////////////////
//
// DRAW CONTROL MENU SCREEN
//
void DrawCtlScreen(void) {
  int i, x, y;

#ifdef JAPAN
  CA_CacheScreen(S_CONTROLPIC);
#else
  ClearMScreen();
  DrawStripes(10);
  VWB_DrawPic(56, 0, C_CONTROLPIC);                     // 144
  VWB_DrawPic(76, SCREEN_HEIGHT - 16, C_MOUSELBACKPIC); // 104, 16
  DrawWindow(CTL_X, CTL_Y, CTL_W, CTL_H, BKGDCOLOR);
#endif
  WindowX = 0;
  WindowW = 256;
  SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);

  if (MousePresent) {
    CtlMenu[CTL_MOUSESENS].active = CtlMenu[CTL_MOUSEENABLE].active = 1;
  }

  CtlMenu[CTL_MOUSESENS].active = mouseenabled;

  DrawMenu(&CtlItems, CtlMenu);

  x = CTL_X + CtlItems.indent - 26;
  y = CTL_Y + 4;
  if (mouseenabled)
    VWB_DrawPic(x, y, C_SELECTEDPIC);
  else
    VWB_DrawPic(x, y, C_NOTSELECTEDPIC);

  y = CTL_Y + 29;
  VWB_DrawPic(x, y, C_NOTSELECTEDPIC);

  //
  // PICK FIRST AVAILABLE SPOT
  //
  if (CtlItems.curpos < 0 || !CtlMenu[CtlItems.curpos].active) {
    for (i = 0; i < CtlItems.amount; i++) {
      if (CtlMenu[i].active) {
        CtlItems.curpos = i;
        break;
      }
    }
  }

  DrawMenuGun(&CtlItems);
  VW_UpdateScreen();
}

////////////////////////////////////////////////////////////////////
//
// CUSTOMIZE CONTROLS
//
////////////////////////////////////////////////////////////////////
enum { FIRE, STRAFE, RUN, OPEN };
char   mbarray[4][3] = {"b0", "b1", "b2", "b3"};
int8_t order[4]      = {RUN, OPEN, FIRE, STRAFE};

int CustomControls(int _ __attribute__((unused))) {
  int which;

  DrawCustomScreen();
  do {
    which = HandleMenu(&CusItems, &CusMenu[0], FixupCustom);
    switch (which) {
    case 0:
      DefineMouseBtns();
      DrawCustMouse(1);
      break;
    case 3:
      DefineJoyBtns();
      DrawCustJoy(0);
      break;
    case 6:
      DefineKeyBtns();
      DrawCustKeybd(0);
      break;
    case 8:
      DefineKeyMove();
      DrawCustKeys(0);
    }
  } while (which >= 0);

  MenuFadeOut();

  return 0;
}

////////////////////////
//
// DEFINE THE MOUSE BUTTONS
//
void DefineMouseBtns(void) {
  CustomCtrls mouseallowed = {0, 1, 1, 1};
  EnterCtrlData(2, &mouseallowed, DrawCustMouse, PrintCustMouse, MOUSE);
}

////////////////////////
//
// DEFINE THE JOYSTICK BUTTONS
//
void DefineJoyBtns(void) {
  CustomCtrls joyallowed = {1, 1, 1, 1};
  EnterCtrlData(5, &joyallowed, DrawCustJoy, PrintCustJoy, JOYSTICK);
}

////////////////////////
//
// DEFINE THE KEYBOARD BUTTONS
//
void DefineKeyBtns(void) {
  CustomCtrls keyallowed = {1, 1, 1, 1};
  EnterCtrlData(8, &keyallowed, DrawCustKeybd, PrintCustKeybd, KEYBOARDBTNS);
}

////////////////////////
//
// DEFINE THE KEYBOARD BUTTONS
//
void DefineKeyMove(void) {
  CustomCtrls keyallowed = {1, 1, 1, 1};
  EnterCtrlData(10, &keyallowed, DrawCustKeys, PrintCustKeys, KEYBOARDMOVE);
}

////////////////////////
//
// ENTER CONTROL DATA FOR ANY TYPE OF CONTROL
//
enum { FWRD, RIGHT, BKWD, LEFT };
int moveorder[4] = {LEFT, RIGHT, FWRD, BKWD};

void EnterCtrlData(int index, CustomCtrls *cust, void (*DrawRtn)(int), void (*PrintRtn)(int), int type) {
  int         j, exit, tick, redraw, which, x, picked, lastFlashTime;
  ControlInfo ci;

  ShootSnd();
  PrintY = CST_Y + 13 * index;
  IN_ClearKeysDown();
  exit   = 0;
  redraw = 1;
  //
  // FIND FIRST SPOT IN ALLOWED ARRAY
  //
  for (j = 0; j < 4; j++)
    if (cust->allowed[j]) {
      which = j;
      break;
    }

  do {
    if (redraw) {
      x = CST_START + CST_SPC * which;
      DrawWindow(1, PrintY - 1, 253, 13, BKGDCOLOR);

      DrawRtn(1);
      DrawWindow(x - 2, PrintY, CST_SPC, 11, TEXTCOLOR);
      DrawOutline(x - 2, PrintY, CST_SPC, 11, 0, HIGHLIGHT);
      SETFONTCOLOR(0, TEXTCOLOR);
      PrintRtn(which);
      PrintX = x;
      SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);
      VW_UpdateScreen();
      WaitKeyUp();
      redraw = 0;
    }

    ReadAnyControl(&ci);

    if (type == MOUSE || type == JOYSTICK)
      if (IN_KeyDown(USB_KEY_ENTER) || IN_KeyDown(USB_KEY_LCTRL) || IN_KeyDown(USB_KEY_LALT)) {
        IN_ClearKeysDown();
        ci.button0 = ci.button1 = false;
      }

    //
    // CHANGE BUTTON VALUE?
    //
    if (((type != KEYBOARDBTNS && type != KEYBOARDMOVE) && (ci.button0 | ci.button1 | ci.button2 | ci.button3)) ||
        ((type == KEYBOARDBTNS || type == KEYBOARDMOVE) && LastScan == USB_KEY_ENTER)) {
      lastFlashTime = GetTimeCount();
      tick = picked = 0;
      SETFONTCOLOR(0, TEXTCOLOR);

      if (type == KEYBOARDBTNS || type == KEYBOARDMOVE)
        IN_ClearKeysDown();

      while (1) {
        int button, result = 0;

        //
        // FLASH CURSOR
        //
        if (GetTimeCount() - lastFlashTime > 10) {
          switch (tick) {
          case 0:
            VWB_Bar(SCREEN_WIDTH_FACTOR(x), PrintY + 1, SCREEN_WIDTH_FACTOR(CST_SPC - 2), 10, TEXTCOLOR);
            break;
          case 1:
            PrintX = x;
            US_Print("?");
            SD_PlaySound(HITWALLSND);
          }
          tick ^= 1;
          lastFlashTime = GetTimeCount();
          VW_UpdateScreen();
        }

        //
        // WHICH TYPE OF INPUT DO WE PROCESS?
        //
        switch (type) {
        case MOUSE:
          button = IN_MouseButtons();
          switch (button) {
          case 1:
            result = 1;
            break;
          case 2:
            result = 2;
            break;
          case 4:
            result = 3;
            break;
          }

          if (result) {
            for (int z = 0; z < 4; z++)
              if (order[which] == buttonmouse[z]) {
                buttonmouse[z] = bt_nobutton;
                break;
              }

            buttonmouse[result - 1] = order[which];
            picked                  = 1;
            SD_PlaySound(SHOOTDOORSND);
          }
          break;

        case JOYSTICK:
          if (ci.button0)
            result = 1;
          else if (ci.button1)
            result = 2;
          else if (ci.button2)
            result = 3;
          else if (ci.button3)
            result = 4;

          if (result) {
            for (int z = 0; z < 4; z++) {
              if (order[which] == buttonjoy[z]) {
                buttonjoy[z] = bt_nobutton;
                break;
              }
            }

            buttonjoy[result - 1] = order[which];
            picked                = 1;
            SD_PlaySound(SHOOTDOORSND);
          }
          break;

        case KEYBOARDBTNS:
          if (LastScan && LastScan != USB_KEY_ESCAPE) {
            buttonscan[order[which]] = LastScan;
            picked                   = 1;
            ShootSnd();
            IN_ClearKeysDown();
          }
          break;

        case KEYBOARDMOVE:
          if (LastScan && LastScan != USB_KEY_ESCAPE) {
            dirscan[moveorder[which]] = LastScan;
            picked                    = 1;
            ShootSnd();
            IN_ClearKeysDown();
          }
          break;
        }

        //
        // EXIT INPUT?
        //
        if (IN_KeyDown(USB_KEY_ESCAPE) || (type != JOYSTICK && ci.button1)) {
          picked = 1;
          SD_PlaySound(ESCPRESSEDSND);
        }

        if (picked)
          break;

        ReadAnyControl(&ci);
      }

      SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);
      redraw = 1;
      WaitKeyUp();
      continue;
    }

    if (ci.button1 || IN_KeyDown(USB_KEY_ESCAPE))
      exit = 1;

    //
    // MOVE TO ANOTHER SPOT?
    //
    switch (ci.dir) {
    case dir_West:
      do {
        which--;
        if (which < 0)
          which = 3;
      } while (!cust->allowed[which]);
      redraw = 1;
      SD_PlaySound(MOVEGUN1SND);
      while (ReadAnyControl(&ci), ci.dir != dir_None)
        ;
      IN_ClearKeysDown();
      break;

    case dir_East:
      do {
        which++;
        if (which > 3)
          which = 0;
      } while (!cust->allowed[which]);
      redraw = 1;
      SD_PlaySound(MOVEGUN1SND);
      while (ReadAnyControl(&ci), ci.dir != dir_None)
        ;
      IN_ClearKeysDown();
      break;
    case dir_North:
    case dir_South:
      exit = 1;
    default:
      break;
    }

  } while (!exit);

  SD_PlaySound(ESCPRESSEDSND);
  WaitKeyUp();
  DrawWindow(1, PrintY - 1, 253, 13, BKGDCOLOR);
}

////////////////////////
//
// FIXUP GUN CURSOR OVERDRAW SHIT
//
void FixupCustom(int w) {
  static int lastwhich = -1;
  int        y         = CST_Y + 26 + w * 13;

  VWB_Hlin(4, 32, y - 1, DEACTIVE);
  VWB_Hlin(4, 32, y + 12, BORD2COLOR);
#ifndef SPEAR
  VWB_Hlin(4, 32, y - 2, BORDCOLOR);
  VWB_Hlin(4, 32, y + 13, BORDCOLOR);
#else
  VWB_Hlin(4, 32, y - 2, BORD2COLOR);
  VWB_Hlin(4, 32, y + 13, BORD2COLOR);
#endif

  switch (w) {
  case 0:
    DrawCustMouse(1);
    break;
  case 3:
    DrawCustJoy(1);
    break;
  case 6:
    DrawCustKeybd(1);
    break;
  case 8:
    DrawCustKeys(1);
  }

  if (lastwhich >= 0) {
    y = CST_Y + 26 + lastwhich * 13;
    VWB_Hlin(4, 32, y - 1, DEACTIVE);
    VWB_Hlin(4, 32, y + 12, BORD2COLOR);
#ifndef SPEAR
    VWB_Hlin(4, 32, y - 2, BORDCOLOR);
    VWB_Hlin(4, 32, y + 13, BORDCOLOR);
#else
    VWB_Hlin(4, 32, y - 2, BORD2COLOR);
    VWB_Hlin(4, 32, y + 13, BORD2COLOR);
#endif

    if (lastwhich != w)
      switch (lastwhich) {
      case 0:
        DrawCustMouse(0);
        break;
      case 3:
        DrawCustJoy(0);
        break;
      case 6:
        DrawCustKeybd(0);
        break;
      case 8:
        DrawCustKeys(0);
      }
  }

  lastwhich = w;
}

////////////////////////
//
// DRAW CUSTOMIZE SCREEN
//
void DrawCustomScreen(void) {
  int i;

#ifdef JAPAN
  CA_CacheScreen(S_CUSTOMPIC);
  fontnumber = 1;

  PrintX = CST_START;
  PrintY = CST_Y + 26;
  DrawCustMouse(0);

  PrintX = CST_START;
  US_Print("\n\n\n");
  DrawCustJoy(0);

  PrintX = CST_START;
  US_Print("\n\n\n");
  DrawCustKeybd(0);

  PrintX = CST_START;
  US_Print("\n\n\n");
  DrawCustKeys(0);
#else
  ClearMScreen();
  WindowX = 0;
  WindowW = 256;
  VWB_DrawPic(76, SCREEN_HEIGHT - 16, C_MOUSELBACKPIC); // 104, 16
  DrawStripes(10);
  VWB_DrawPic(52, 0, C_CUSTOMIZEPIC); // 152 x 48

  //
  // MOUSE
  //
  SETFONTCOLOR(READCOLOR, BKGDCOLOR);
  WindowX = 0;
  WindowW = 256;

#ifndef SPEAR
  PrintY = CST_Y;
  US_CPrint("Mouse\n");
#else
  PrintY = CST_Y + 13;
  VWB_DrawPic(128, 48, C_MOUSEPIC);
#endif

  SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);
#ifdef SPANISH
  PrintX = CST_START - 16;
  US_Print(STR_CRUN);
  PrintX = CST_START - 16 + CST_SPC * 1;
  US_Print(STR_COPEN);
  PrintX = CST_START - 16 + CST_SPC * 2;
  US_Print(STR_CFIRE);
  PrintX = CST_START - 16 + CST_SPC * 3;
  US_Print(STR_CSTRAFE "\n");
#else
  PrintX = CST_START;
  US_Print(STR_CRUN);
  PrintX = CST_START + CST_SPC * 1;
  US_Print(STR_COPEN);
  PrintX = CST_START + CST_SPC * 2;
  US_Print(STR_CFIRE);
  PrintX = CST_START + CST_SPC * 3;
  US_Print(STR_CSTRAFE "\n");
#endif

  DrawWindow(1, PrintY - 1, 253, 13, BKGDCOLOR);
  DrawCustMouse(0);
  US_Print("\n");

  //
  // JOYSTICK/PAD
  //
#ifndef SPEAR
  SETFONTCOLOR(READCOLOR, BKGDCOLOR);
  US_CPrint("Joystick/Gravis GamePad\n");
#else
  PrintY += 13;
  VWB_DrawPic(40, 88, C_JOYSTICKPIC);
#endif

#ifdef SPEAR
  VWB_DrawPic(112, 120, C_KEYBOARDPIC);
#endif

  SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);
#ifdef SPANISH
  PrintX = CST_START - 16;
  US_Print(STR_CRUN);
  PrintX = CST_START - 16 + CST_SPC * 1;
  US_Print(STR_COPEN);
  PrintX = CST_START - 16 + CST_SPC * 2;
  US_Print(STR_CFIRE);
  PrintX = CST_START - 16 + CST_SPC * 3;
  US_Print(STR_CSTRAFE "\n");
#else
  PrintX = CST_START;
  US_Print(STR_CRUN);
  PrintX = CST_START + CST_SPC * 1;
  US_Print(STR_COPEN);
  PrintX = CST_START + CST_SPC * 2;
  US_Print(STR_CFIRE);
  PrintX = CST_START + CST_SPC * 3;
  US_Print(STR_CSTRAFE "\n");
#endif
  DrawWindow(1, PrintY - 1, 253, 13, BKGDCOLOR);
  DrawCustJoy(0);
  US_Print("\n");

  //
  // KEYBOARD
  //
#ifndef SPEAR
  SETFONTCOLOR(READCOLOR, BKGDCOLOR);
  US_CPrint("Keyboard\n");
#else
  PrintY += 13;
#endif
  SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);
#ifdef SPANISH
  PrintX = CST_START - 16;
  US_Print(STR_CRUN);
  PrintX = CST_START - 16 + CST_SPC * 1;
  US_Print(STR_COPEN);
  PrintX = CST_START - 16 + CST_SPC * 2;
  US_Print(STR_CFIRE);
  PrintX = CST_START - 16 + CST_SPC * 3;
  US_Print(STR_CSTRAFE "\n");
#else
  PrintX = CST_START;
  US_Print(STR_CRUN);
  PrintX = CST_START + CST_SPC * 1;
  US_Print(STR_COPEN);
  PrintX = CST_START + CST_SPC * 2;
  US_Print(STR_CFIRE);
  PrintX = CST_START + CST_SPC * 3;
  US_Print(STR_CSTRAFE "\n");
#endif
  DrawWindow(1, PrintY - 1, 253, 13, BKGDCOLOR);
  DrawCustKeybd(0);
  US_Print("\n");

  //
  // KEYBOARD MOVE KEYS
  //
  SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);
#ifdef SPANISH
  PrintX = 4;
  US_Print(STR_LEFT);
  US_Print("/");
  US_Print(STR_RIGHT);
  US_Print("/");
  US_Print(STR_FRWD);
  US_Print("/");
  US_Print(STR_BKWD "\n");
#else
  PrintX = CST_START;
  US_Print(STR_LEFT);
  PrintX = CST_START + CST_SPC * 1;
  US_Print(STR_RIGHT);
  PrintX = CST_START + CST_SPC * 2;
  US_Print(STR_FRWD);
  PrintX = CST_START + CST_SPC * 3;
  US_Print(STR_BKWD "\n");
#endif
  DrawWindow(1, PrintY - 1, 253, 13, BKGDCOLOR);
  DrawCustKeys(0);
#endif
  //
  // PICK STARTING POINT IN MENU
  //
  if (CusItems.curpos < 0)
    for (i = 0; i < CusItems.amount; i++)
      if (CusMenu[i].active) {
        CusItems.curpos = i;
        break;
      }

  MenuFadeIn();
}

void PrintCustMouse(int i) {
  int j;

  for (j = 0; j < 4; j++)
    if (order[i] == buttonmouse[j]) {
      PrintX = CST_START + CST_SPC * i;
      US_Print(mbarray[j]);
      break;
    }
}

void DrawCustMouse(int hilight) {
  int i, color;

  color = TEXTCOLOR;
  if (hilight)
    color = HIGHLIGHT;
  SETFONTCOLOR(color, BKGDCOLOR);

  if (!mouseenabled) {
    SETFONTCOLOR(DEACTIVE, BKGDCOLOR);
    CusMenu[0].active = 0;
  } else
    CusMenu[0].active = 1;

  PrintY = CST_Y + 13 * 2;
  for (i = 0; i < 4; i++)
    PrintCustMouse(i);
}

void PrintCustJoy(int i) {
  for (int j = 0; j < 4; j++) {
    if (order[i] == buttonjoy[j]) {
      PrintX = CST_START + CST_SPC * i;
      US_Print(mbarray[j]);
      break;
    }
  }
}

void DrawCustJoy(int hilight) {
  int i, color;

  color = TEXTCOLOR;
  if (hilight)
    color = HIGHLIGHT;
  SETFONTCOLOR(color, BKGDCOLOR);

  SETFONTCOLOR(DEACTIVE, BKGDCOLOR);
  CusMenu[3].active = 0;

  PrintY = CST_Y + 13 * 5;
  for (i = 0; i < 4; i++)
    PrintCustJoy(i);
}

void PrintCustKeybd(int i) {
  PrintX = CST_START + CST_SPC * i;
  US_Print((const char *)IN_GetScanName(buttonscan[order[i]]));
}

void DrawCustKeybd(int hilight) {
  int i, color;

  color = TEXTCOLOR;
  if (hilight)
    color = HIGHLIGHT;
  SETFONTCOLOR(color, BKGDCOLOR);

  PrintY = CST_Y + 13 * 8;
  for (i = 0; i < 4; i++)
    PrintCustKeybd(i);
}

void PrintCustKeys(int i) {
  PrintX = CST_START + CST_SPC * i;
  US_Print((const char *)IN_GetScanName(dirscan[moveorder[i]]));
}

void DrawCustKeys(int hilight) {
  int i, color;

  color = TEXTCOLOR;
  if (hilight)
    color = HIGHLIGHT;
  SETFONTCOLOR(color, BKGDCOLOR);

  PrintY = CST_Y + 13 * 10;
  for (i = 0; i < 4; i++)
    PrintCustKeys(i);
}

////////////////////////////////////////////////////////////////////
//
// CHANGE SCREEN VIEWING SIZE
//
////////////////////////////////////////////////////////////////////
int CP_ChangeView(int _ __attribute__((unused))) {
  int         exit = 0, oldview, newview;
  ControlInfo ci;

  WindowX = WindowY = 0;
  WindowW           = SCREEN_WIDTH;
  WindowH           = SCREEN_HEIGHT;
  newview = oldview = viewsize;
  DrawChangeView(oldview);
  MenuFadeIn();

  do {
    CheckPause();
    ReadAnyControl(&ci);
    switch (ci.dir) {
    case dir_South:
    case dir_West:
      newview--;
      if (newview < 4)
        newview = 4;
      ShowViewSize(newview);
      VW_UpdateScreen();
      SD_PlaySound(HITWALLSND);
      TicDelay(10);
      break;

    case dir_North:
    case dir_East:
      newview++;
      if (newview >= MAX_VIEW_SIZE) {
        newview = MAX_VIEW_SIZE;
        DrawChangeView(newview);
      } else
        ShowViewSize(newview);
      VW_UpdateScreen();
      SD_PlaySound(HITWALLSND);
      TicDelay(10);
      break;

    default:
      break;
    }

    if (ci.button0 || Keyboard[USB_KEY_ENTER])
      exit = 1;
    else if (ci.button1 || Keyboard[USB_KEY_ESCAPE]) {
      SD_PlaySound(ESCPRESSEDSND);
      MenuFadeOut();
      // if (screenHeight % SCREEN_HEIGHT != 0)
      //   VL_ClearScreen(0);
      return 0;
    }
  } while (!exit);

  if (oldview != newview) {
    SD_PlaySound(SHOOTSND);
    Message(STR_THINK "...");
    printf("Viewsize: %d\n", newview);
    NewViewSize(newview);
  }

  ShootSnd();
  MenuFadeOut();
  // if (screenHeight % SCREEN_HEIGHT != 0)
  //   VL_ClearScreen(0);

  return 0;
}

/////////////////////////////
//
// DRAW THE CHANGEVIEW SCREEN
//
void DrawChangeView(int view) {
  printf("DrawChangeView(%d)\r\n", view);
  int rescaledHeight = screenHeight / scaleFactor;
  VWB_Bar(0, rescaledHeight - 40, 256, 40, bordercol);

#ifdef JAPAN
  CA_CacheScreen(S_CHANGEPIC);

  ShowViewSize(view);
#else
  ShowViewSize(view);

  PrintY = (screenHeight / scaleFactor) - 39;
  WindowX = 0;
  WindowY = 256; // TODO: Check this!
  SETFONTCOLOR(HIGHLIGHT, BKGDCOLOR);

  US_CPrint(STR_SIZE1 "\n");
  US_CPrint(STR_SIZE2 "\n");
  US_CPrint(STR_SIZE3);
#endif
  VW_UpdateScreen();
}

////////////////////////////////////////////////////////////////////
//
// QUIT THIS INFERNAL GAME!
//
////////////////////////////////////////////////////////////////////
int CP_Quit(int _ __attribute((unused))) {
#ifdef JAPAN
  if (GetYorN(7, 11, C_QUITMSGPIC))
#else

#ifdef SPANISH
  if (Confirm(ENDGAMESTR))
#else
  if (Confirm(endStrings[US_RndT() & 0x7 + (US_RndT() & 1)]))
#endif

#endif
  {
    VW_UpdateScreen();
    SD_MusicOff();
    SD_StopSound();
    MenuFadeOut();
    Quit(NULL);
  }

  DrawMainMenu();
  return 0;
}

////////////////////////////////////////////////////////////////////
//
// HANDLE INTRO SCREEN (SYSTEM CONFIG)
//
////////////////////////////////////////////////////////////////////
void IntroScreen(void) {
#ifdef SPEAR

#define MAINCOLOR 0x4f
#define EMSCOLOR  0x4f
#define XMSCOLOR  0x4f

#else

#define MAINCOLOR 0x6c
#define EMSCOLOR  0x6c // 0x4f
#define XMSCOLOR  0x6c // 0x7f

#endif
#define FILLCOLOR 14

  int i;
  //
  // DRAW MAIN MEMORY
  //
#ifdef ABCAUS
  memory = (1023l + mminfo.nearheap + mminfo.farheap) / 1024l;
  for (i = 0; i < 10; i++)
    if (memory >= main[i])
      VWB_Bar(49, 163 - 8 * i, 6, 5, MAINCOLOR - i);

  //
  // DRAW EMS MEMORY
  //
  if (EMSPresent) {
    emshere = 4l * EMSPagesAvail;
    for (i = 0; i < 10; i++)
      if (emshere >= ems[i])
        VWB_Bar(89, 163 - 8 * i, 6, 5, EMSCOLOR - i);
  }

  //
  // DRAW XMS MEMORY
  //
  if (XMSPresent) {
    xmshere = 4l * XMSPagesAvail;
    for (i = 0; i < 10; i++)
      if (xmshere >= xms[i])
        VWB_Bar(129, 163 - 8 * i, 6, 5, XMSCOLOR - i);
  }
#else
  for (i = 0; i < 2; i++)
    VWB_Bar(42 - 7, (163 - 8) - 8 * i, 5, 5, MAINCOLOR - i);
  for (i = 0; i < 10; i++)
    VWB_Bar(71, (163 - 8) - 8 * i, 5, 5, EMSCOLOR - i);
  for (i = 0; i < 10; i++)
    VWB_Bar(100 + 6, (163 - 8) - 8 * i, 5, 5, XMSCOLOR - i);
#endif

  //
  // FILL BOXES
  //
  if (MousePresent)
    VWB_Bar(131 + 7, 82 - 8, 9, 2, FILLCOLOR);

  if (AdLibPresent && !SoundBlasterPresent)
    VWB_Bar(131 + 7, 128 - 8, 9, 2, FILLCOLOR);

  if (SoundBlasterPresent)
    VWB_Bar(131 + 7, 151 - 8, 9, 2, FILLCOLOR);

  if (SoundSourcePresent)
    VWB_Bar(131 + 7, 174 - 8, 9, 2, FILLCOLOR);
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//
// SUPPORT ROUTINES
//
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//
// Clear Menu screens to dark red
//
////////////////////////////////////////////////////////////////////
void ClearMScreen(void) {
#ifndef SPEAR
  VWB_Bar(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BORDCOLOR);
#else
  VWB_DrawPic(0, 0, C_BACKDROPPIC);
#endif
}

////////////////////////////////////////////////////////////////////
//
// Un/Cache a LUMP of graphics
//
////////////////////////////////////////////////////////////////////
void CacheLump(int lumpstart, int lumpend) {
  int i;

  for (i = lumpstart; i <= lumpend; i++)
    CA_CacheGrChunk(i);
}

void UnCacheLump(int lumpstart, int lumpend) {
  int i;

  for (i = lumpstart; i <= lumpend; i++)
    if (grsegs[i])
      UNCACHEGRCHUNK(i);
}

////////////////////////////////////////////////////////////////////
//
// Draw a window for a menu
//
////////////////////////////////////////////////////////////////////
void DrawWindow(int x, int y, int w, int h, int wcolor) {
  VWB_Bar(x, y, w, h, wcolor);
  DrawOutline(x, y, w, h, BORD2COLOR, DEACTIVE);
}

void DrawOutline(int x, int y, int w, int h, int color1, int color2) {
  VWB_Hlin(x, x + w, y, color2);
  VWB_Vlin(y, y + h, x, color2);
  VWB_Hlin(x, x + w, y + h, color1);
  VWB_Vlin(y, y + h, x + w, color1);
}

////////////////////////////////////////////////////////////////////
//
// Setup Control Panel stuff - graphics, etc.
//
////////////////////////////////////////////////////////////////////
void SetupControlPanel(void) {
  //
  // CACHE GRAPHICS & SOUNDS
  //
  CA_CacheGrChunk(STARTFONT + 1);
#ifndef SPEAR
  CacheLump(CONTROLS_LUMP_START, CONTROLS_LUMP_END);
#else
  CacheLump(BACKDROP_LUMP_START, BACKDROP_LUMP_END);
#endif

  SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);
  fontnumber = 1;
  WindowH    = 200;
  if (screenHeight % 200 != 0)
    VL_ClearScreen(0);

  if (!ingame)
    CA_LoadAllSounds();
  else
    MainMenu[savegame].active = 1;
}

////////////////////////////////////////////////////////////////////
//
// SEE WHICH SAVE GAME FILES ARE AVAILABLE & READ STRING IN
//
////////////////////////////////////////////////////////////////////
void SetupSaveGames() {
  char name[13];
  char savepath[300];

  strcpy(name, SaveName);
  for (int i = 0; i < MAX_SAVE_GAMES; i++) {
    name[7] = '0' + i;

    if (configdir[0])
      snprintf(savepath, sizeof(savepath), "%s/%s", configdir, name);
    else
      strcpy(savepath, name);

    const int handle = open(savepath, O_RDONLY | O_BINARY);
    printf("Load file %s\r\n", savepath);

    if (handle >= 0) {
      char temp[32];

      SaveGamesAvail[i] = 1;
      read(handle, temp, 32);
      close(handle);
      strcpy(&SaveGameNames[i][0], temp);
    }
  }
}

////////////////////////////////////////////////////////////////////
//
// Clean up all the Control Panel stuff
//
////////////////////////////////////////////////////////////////////
void CleanupControlPanel(void) {
#ifndef SPEAR
  UnCacheLump(CONTROLS_LUMP_START, CONTROLS_LUMP_END);
#else
  UnCacheLump(BACKDROP_LUMP_START, BACKDROP_LUMP_END);
#endif

  fontnumber = 0;
}

////////////////////////////////////////////////////////////////////
//
// Handle moving gun around a menu
//
////////////////////////////////////////////////////////////////////
int HandleMenu(CP_iteminfo *item_i, CP_itemtype *items, void (*routine)(int w)) {
  char        key;
  static int  redrawitem = 1, lastitem = -1;
  int         i, x, y, basey, exit, which, shape;
  uint24_t    timer;
  ControlInfo ci;

  which = item_i->curpos;
  x     = item_i->x + 4;
  basey = item_i->y + 2;
  y     = basey + which * 13;

  VWB_DrawPic(x, y, C_CURSOR1PIC);
  SetTextColor(items + which, 1);
  if (redrawitem) {
    PrintX = item_i->x + item_i->indent;
    PrintY = item_i->y + 2 + which * 13;
    US_Print((items + which)->string);
  }
  //
  // CALL CUSTOM ROUTINE IF IT IS NEEDED
  //
  if (routine)
    routine(which);
  VW_UpdateScreen();

  shape = C_CURSOR1PIC;
  timer = 8;
  exit  = 0;
  IN_ClearKeysDown();

  do {
    PM_Preload(NULL, true);
    //
    // CHANGE GUN SHAPE
    //
    //
    // CHANGE GUN SHAPE
    //
    if (GetTimeCount() > timer) {
      SetTimeCount(0);
      if (shape == C_CURSOR1PIC) {
        shape = C_CURSOR2PIC;
        timer = 8;
      } else {
        shape = C_CURSOR1PIC;
        timer = 70;
      }
      VWB_DrawPic(x, y, shape);
      if (routine)
        routine(which);
      VW_UpdateScreen();
    }

    CheckPause();

    //
    // SEE IF ANY KEYS ARE PRESSED FOR INITIAL CHAR FINDING
    //
    key = LastASCII;
    if (key) {
      int ok = 0;

      if (key >= 'a')
        key -= 'a' - 'A';

      for (i = which + 1; i < item_i->amount; i++)
        if ((items + i)->active && (items + i)->string[0] == key) {
          EraseGun(item_i, items, x, y, which);
          which = i;
          DrawGun(item_i, items, x, &y, which, basey, routine);
          ok = 1;
          IN_ClearKeysDown();
          break;
        }

      //
      // DIDN'T FIND A MATCH FIRST TIME THRU. CHECK AGAIN.
      //
      if (!ok) {
        for (i = 0; i < which; i++)
          if ((items + i)->active && (items + i)->string[0] == key) {
            EraseGun(item_i, items, x, y, which);
            which = i;
            DrawGun(item_i, items, x, &y, which, basey, routine);
            IN_ClearKeysDown();
            break;
          }
      }
    }

    //
    // GET INPUT
    //
    ReadAnyControl(&ci);
    switch (ci.dir) {
      ////////////////////////////////////////////////
      //
      // MOVE UP
      //
    case dir_North:

      EraseGun(item_i, items, x, y, which);

      //
      // ANIMATE HALF-STEP
      //
      if (which && (items + which - 1)->active) {
        y -= 6;
        DrawHalfStep(x, y);
      }

      //
      // MOVE TO NEXT AVAILABLE SPOT
      //
      do {
        if (!which)
          which = item_i->amount - 1;
        else
          which--;
      } while (!(items + which)->active);

      DrawGun(item_i, items, x, &y, which, basey, routine);
      //
      // WAIT FOR BUTTON-UP OR DELAY NEXT MOVE
      //
      TicDelay(20);
      break;

      ////////////////////////////////////////////////
      //
      // MOVE DOWN
      //
    case dir_South:

      EraseGun(item_i, items, x, y, which);
      //
      // ANIMATE HALF-STEP
      //
      if (which != item_i->amount - 1 && (items + which + 1)->active) {
        y += 6;
        DrawHalfStep(x, y);
      }

      do {
        if (which == item_i->amount - 1)
          which = 0;
        else
          which++;
      } while (!(items + which)->active);

      DrawGun(item_i, items, x, &y, which, basey, routine);

      //
      // WAIT FOR BUTTON-UP OR DELAY NEXT MOVE
      //
      TicDelay(20);
      break;

    default:
      break;
    }

    if (ci.button0 || Keyboard[USB_KEY_SPACE] || Keyboard[USB_KEY_ENTER])
      exit = 1;

    if (ci.button1 || Keyboard[USB_KEY_ESCAPE])
      exit = 2;
  } while (!exit);

  IN_ClearKeysDown();

  //
  // ERASE EVERYTHING
  //
  if (lastitem != which) {
    VWB_Bar(x - 1, y, 25, 16, BKGDCOLOR);
    PrintX = item_i->x + item_i->indent;
    PrintY = item_i->y + 2 + which * 13;
    US_Print((items + which)->string);
    redrawitem = 1;
  } else
    redrawitem = 0;

  if (routine)
    routine(which);
  VW_UpdateScreen();

  item_i->curpos = which;

  lastitem = which;
  switch (exit) {
  case 1:
    //
    // CALL THE ROUTINE
    //
    if ((items + which)->routine != NULL) {
      ShootSnd();
      MenuFadeOut();
      (items + which)->routine(0);
    }
    return which;

  case 2:
    SD_PlaySound(ESCPRESSEDSND);
    return -1;
  }

  return 0; // JUST TO SHUT UP THE ERROR MESSAGES!
}

//
// ERASE GUN & DE-HIGHLIGHT STRING
//
void EraseGun(CP_iteminfo *item_i, CP_itemtype *items, int x, int y, int which) {
  VWB_Bar(x - 1, y, 25, 16, BKGDCOLOR);
  SetTextColor(items + which, 0);

  PrintX = item_i->x + item_i->indent;
  PrintY = item_i->y + 2 + which * 13;
  US_Print((items + which)->string);
  // VW_UpdateScreen();
}

//
// DRAW HALF STEP OF GUN TO NEXT POSITION
//
void DrawHalfStep(int x, int y) {
  VWB_DrawPic(x, y, C_CURSOR1PIC);
  // VW_UpdateScreen();
  SD_PlaySound(MOVEGUN1SND);
}

//
// DRAW GUN AT NEW POSITION
//
void DrawGun(CP_iteminfo *item_i, CP_itemtype *items, int x, int *y, int which, int basey, void (*routine)(int w)) {
  VWB_Bar(x - 1, *y, 25, 16, BKGDCOLOR);
  *y = basey + which * 13;
  VWB_DrawPic(x, *y, C_CURSOR1PIC);
  SetTextColor(items + which, 1);

  PrintX = item_i->x + item_i->indent;
  PrintY = item_i->y + 2 + which * 13;
  US_Print((items + which)->string);

  //
  // CALL CUSTOM ROUTINE IF IT IS NEEDED
  //
  if (routine)
    routine(which);
  // VW_UpdateScreen();
  SD_PlaySound(MOVEGUN2SND);
}

////////////////////////////////////////////////////////////////////
//
// DELAY FOR AN AMOUNT OF TICS OR UNTIL CONTROLS ARE INACTIVE
//
////////////////////////////////////////////////////////////////////
void TicDelay(int count) {
  ControlInfo ci;

  int24_t startTime = GetTimeCount();
  do {
    ReadAnyControl(&ci);
  } while ((int24_t)GetTimeCount() - startTime < count && ci.dir != dir_None);
}

////////////////////////////////////////////////////////////////////
//
// Draw a menu
//
////////////////////////////////////////////////////////////////////
void DrawMenu(CP_iteminfo *item_i, CP_itemtype *items) {
  int i, which = item_i->curpos;

  WindowX = PrintX = item_i->x + item_i->indent;
  WindowY = PrintY = item_i->y;
  WindowW          = 256;
  WindowH          = 192;

  for (i = 0; i < item_i->amount; i++) {
    SetTextColor(items + i, which == i);

    PrintY = item_i->y + 2 + i * 13;
    if ((items + i)->active)
      US_Print((items + i)->string);
    else {
      SETFONTCOLOR(DEACTIVE, BKGDCOLOR);
      US_Print((items + i)->string);
      SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);
    }

    US_Print("\n");
  }
}

////////////////////////////////////////////////////////////////////
//
// SET TEXT COLOR (HIGHLIGHT OR NO)
//
////////////////////////////////////////////////////////////////////
void SetTextColor(CP_itemtype *items, int hlight) {
  if (hlight) {
    SETFONTCOLOR(color_hlite[items->active], BKGDCOLOR);
  } else {
    SETFONTCOLOR(color_norml[items->active], BKGDCOLOR);
  }
}

////////////////////////////////////////////////////////////////////
//
// WAIT FOR CTRLKEY-UP OR BUTTON-UP
//
////////////////////////////////////////////////////////////////////
void WaitKeyUp(void) {
  ControlInfo ci;
  while (ReadAnyControl(&ci), ci.button0 | ci.button1 | ci.button2 | ci.button3 | Keyboard[USB_KEY_SPACE] |
                                  Keyboard[USB_KEY_ENTER] | Keyboard[USB_KEY_ESCAPE]) {
    IN_WaitAndProcessEvents();
  }
}

////////////////////////////////////////////////////////////////////
//
// READ KEYBOARD, JOYSTICK AND MOUSE FOR INPUT
//
////////////////////////////////////////////////////////////////////
static int totalMousex = 0, totalMousey = 0;
void       ReadAnyControl(ControlInfo *ci) {
  int mouseactive = 0;

  IN_ReadControl(0, ci);

  if (mouseenabled) {
    // READ MOUSE MOTION COUNTERS
    // RETURN DIRECTION
    // HOME MOUSE
    // CHECK MOUSE BUTTONS

    io_mouse_move(&totalMousex, &totalMousey);

    if (totalMousey < -SENSITIVE) {
      ci->dir     = dir_North;
      mouseactive = 1;
    } else if (totalMousey > SENSITIVE) {
      ci->dir     = dir_South;
      mouseactive = 1;
    }

    if (totalMousex < -SENSITIVE) {
      ci->dir     = dir_West;
      mouseactive = 1;
    } else if (totalMousex > SENSITIVE) {
      ci->dir     = dir_East;
      mouseactive = 1;
    }

    if (mouseactive) {
      totalMousex = 0;
      totalMousey = 0;
    }

    if (usb_mouse_report.buttons) {
      ci->button0 = usb_mouse_report.buttons & 1;
      ci->button1 = usb_mouse_report.buttons & 2;
      ci->button2 = usb_mouse_report.buttons & 4;
      ci->button3 = false;
      mouseactive = 1;
    }
  }
}

////////////////////////////////////////////////////////////////////
//
// DRAW DIALOG AND CONFIRM YES OR NO TO QUESTION
//
////////////////////////////////////////////////////////////////////
int Confirm(const char *string) {
  int xit = 0, x, y, tick = 0, lastBlinkTime;
  // int         whichsnd[2] = {ESCPRESSEDSND, SHOOTSND};
  ControlInfo ci;

  Message(string);
  IN_ClearKeysDown();
  WaitKeyUp();

  //
  // BLINK CURSOR
  //
  x             = PrintX;
  y             = PrintY;
  lastBlinkTime = GetTimeCount();

  do {
    ReadAnyControl(&ci);

    if (GetTimeCount() - lastBlinkTime >= 10) {
      switch (tick) {
      case 0:
        VWB_Bar(x, y, 8, 13, TEXTCOLOR);
        break;
      case 1:
        PrintX = x;
        PrintY = y;
        US_Print("_");
      }
      VW_UpdateScreen();
      tick ^= 1;
      lastBlinkTime = GetTimeCount();
    }

#ifdef SPANISH
  } while (!Keyboard[sc_S] && !Keyboard[USB_KEY_N] && !Keyboard[USB_KEY_ESCAPE]);
#else
  } while (!Keyboard[USB_KEY_Y] && !Keyboard[USB_KEY_N] && !Keyboard[USB_KEY_ESCAPE] && !ci.button0 && !ci.button1);
#endif

#ifdef SPANISH
  if (Keyboard[sc_S] || ci.button0) {
    xit = 1;
    ShootSnd();
  }
#else
  if (Keyboard[USB_KEY_Y] || ci.button0) {
    xit = 1;
    ShootSnd();
  }
#endif

  IN_ClearKeysDown();
  WaitKeyUp();

  SD_PlaySound((soundnames)whichsnd[xit]);

  return xit;
}

#ifdef JAPAN
////////////////////////////////////////////////////////////////////
//
// DRAW MESSAGE & GET Y OR N
//
////////////////////////////////////////////////////////////////////
int GetYorN(int x, int y, int pic) {
  int xit = 0, whichsnd[2] = {ESCPRESSEDSND, SHOOTSND};

  CA_CacheGrChunk(pic);
  VWB_DrawPic(x * 8, y * 8, pic);
  UNCACHEGRCHUNK(pic);
  VW_UpdateScreen();
  IN_ClearKeysDown();

  do {
    IN_WaitAndProcessEvents();
#ifndef SPEAR
    if (Keyboard[sc_Tab] && Keyboard[sc_P] && param_debugmode)
      PicturePause();
#endif

#ifdef SPANISH
  } while (!Keyboard[sc_S] && !Keyboard[USB_KEY_N] && !Keyboard[USB_KEY_ESCAPE]);
#else
  } while (!Keyboard[USB_KEY_Y] && !Keyboard[USB_KEY_N] && !Keyboard[USB_KEY_ESCAPE]);
#endif

#ifdef SPANISH
  if (Keyboard[sc_S]) {
    xit = 1;
    ShootSnd();
  }

  while (Keyboard[sc_S] || Keyboard[USB_KEY_N] || Keyboard[USB_KEY_ESCAPE])
    IN_WaitAndProcessEvents();

#else

  if (Keyboard[USB_KEY_Y]) {
    xit = 1;
    ShootSnd();
  }

  while (Keyboard[USB_KEY_Y] || Keyboard[USB_KEY_N] || Keyboard[USB_KEY_ESCAPE])
    IN_WaitAndProcessEvents();
#endif

  IN_ClearKeysDown();
  SD_PlaySound(whichsnd[xit]);
  return xit;
}
#endif

////////////////////////////////////////////////////////////////////
//
// PRINT A MESSAGE IN A WINDOW
//
////////////////////////////////////////////////////////////////////
void Message(const char *string) {
  printf("Message: %s\r\n", string);
  int         h = 0, w = 0, mw = 0, i, len = (int)strlen(string);
  fontstruct *font;

  CA_CacheGrChunk(STARTFONT + 1);
  fontnumber = 1;
  font       = (fontstruct *)grsegs[STARTFONT + fontnumber];
  h          = font->height;
  for (i = 0; i < len; i++) {
    if (string[i] == '\n') {
      if (w > mw)
        mw = w;
      w = 0;
      h += font->height;
    } else
      w += font->width[(uint8_t)string[i]];
  }

  if (w + 10 > mw)
    mw = w + 10;

  printf("w: %d, mw: %d\r\n", w, mw);

  PrintY = (SCREEN_HEIGHT / 2) - h / 2;
  PrintX = WindowX = (SCREEN_WIDTH / 2) - mw / 2;
  printf("3: Window: %d, %d, %d, %d\r\n", WindowX, WindowY, WindowW, WindowH);

  DrawWindow(WindowX - 5, PrintY - 5, mw + 10, h + 10, TEXTCOLOR);
  DrawOutline(WindowX - 5, PrintY - 5, mw + 10, h + 10, 0, HIGHLIGHT);
  SETFONTCOLOR(0, TEXTCOLOR);
  US_Print(string);
  VW_UpdateScreen();
}

////////////////////////////////////////////////////////////////////
//
// THIS MAY BE FIXED A LITTLE LATER...
//
////////////////////////////////////////////////////////////////////
static int lastmusic;

int StartCPMusic(int song) {
  int lastoffs;

  lastmusic = song;
  lastoffs  = SD_MusicOff();
  UNCACHEAUDIOCHUNK(STARTMUSIC + lastmusic);

  SD_StartMusic(STARTMUSIC + song);
  return lastoffs;
}

void FreeMusic(void) { UNCACHEAUDIOCHUNK(STARTMUSIC + lastmusic); }

///////////////////////////////////////////////////////////////////////////
//
//      IN_GetScanName() - Returns a string containing the name of the
//              specified scan code
//
///////////////////////////////////////////////////////////////////////////
const char *IN_GetScanName(ScanCode scan) {
  switch (scan) {
  case (USB_KEY_BACKSPACE):
    return "BaSp";
  case (USB_KEY_TAB):
    return "Tab";
  case (USB_KEY_ENTER):
    return "Ent";
  case (USB_KEY_PAUSE):
    return "Pas";
  case (USB_KEY_ESCAPE):
    return "Esc";
  case (USB_KEY_SPACE):
    return "Spc";
  case (USB_KEY_QUOTE):
    return "'";
  case (USB_KEY_COMMA):
    return ",";
  case (USB_KEY_MINUS):
    return "-";
  case (USB_KEY_PERIOD):
    return ".";
  case (USB_KEY_SLASH):
    return "/";
  case (USB_KEY_0):
    return "0";
  case (USB_KEY_1):
    return "1";
  case (USB_KEY_2):
    return "2";
  case (USB_KEY_3):
    return "3";
  case (USB_KEY_4):
    return "4";
  case (USB_KEY_5):
    return "5";
  case (USB_KEY_6):
    return "6";
  case (USB_KEY_7):
    return "7";
  case (USB_KEY_8):
    return "8";
  case (USB_KEY_9):
    return "9";
  case (USB_KEY_SEMICOLON):
    return ";";
  case (USB_KEY_A):
    return "A";
  case (USB_KEY_B):
    return "B";
  case (USB_KEY_C):
    return "C";
  case (USB_KEY_D):
    return "D";
  case (USB_KEY_E):
    return "E";
  case (USB_KEY_F):
    return "F";
  case (USB_KEY_G):
    return "G";
  case (USB_KEY_H):
    return "H";
  case (USB_KEY_I):
    return "I";
  case (USB_KEY_J):
    return "J";
  case (USB_KEY_K):
    return "K";
  case (USB_KEY_L):
    return "L";
  case (USB_KEY_M):
    return "M";
  case (USB_KEY_N):
    return "N";
  case (USB_KEY_O):
    return "O";
  case (USB_KEY_P):
    return "P";
  case (USB_KEY_Q):
    return "Q";
  case (USB_KEY_R):
    return "R";
  case (USB_KEY_S):
    return "S";
  case (USB_KEY_T):
    return "T";
  case (USB_KEY_U):
    return "U";
  case (USB_KEY_V):
    return "V";
  case (USB_KEY_W):
    return "W";
  case (USB_KEY_X):
    return "X";
  case (USB_KEY_Y):
    return "Y";
  case (USB_KEY_Z):
    return "Z";
  case (USB_KEY_LBRACKET):
    return "[";
  case (USB_KEY_BACKSLASH):
    return "\\";
  case (USB_KEY_RBRACKET):
    return "]";
  case (USB_KEY_GRAVE):
    return "`";
  case (USB_KEY_UP):
    return "Up";
  case (USB_KEY_DOWN):
    return "Dwn";
  case (USB_KEY_RIGHT):
    return "Rght";
  case (USB_KEY_LEFT):
    return "Left";
  case (USB_KEY_INSERT):
    return "Ins";
  case (USB_KEY_HOME):
    return "Home";
  case (USB_KEY_END):
    return "End";
  case (USB_KEY_PAGEUP):
    return "PgUp";
  case (USB_KEY_PAGEDOWN):
    return "PgDn";
  case (USB_KEY_DELETE):
    return "Del";
  case (USB_KEY_F1):
    return "F1";
  case (USB_KEY_F2):
    return "F2";
  case (USB_KEY_F3):
    return "F3";
  case (USB_KEY_F4):
    return "F4";
  case (USB_KEY_F5):
    return "F5";
  case (USB_KEY_F6):
    return "F6";
  case (USB_KEY_F7):
    return "F7";
  case (USB_KEY_F8):
    return "F8";
  case (USB_KEY_F9):
    return "F9";
  case (USB_KEY_F10):
    return "F10";
  case (USB_KEY_F11):
    return "F11";
  case (USB_KEY_F12):
    return "F12";
  case (USB_KEY_NUMLOCK):
    return "NmLk";
  case (USB_KEY_CAPSLOCK):
    return "CpsL";
  case (USB_KEY_SCROLLLOCK):
    return "Scrl";
  case (USB_KEY_RSHIFT):
    return "RShft";
  case (USB_KEY_LSHIFT):
    return "Shft";
  case (USB_KEY_RCTRL):
    return "RCtl";
  case (USB_KEY_LCTRL):
    return "Ctrl";
  case (USB_KEY_RALT):
    return "RAlt";
  case (USB_KEY_LALT):
    return "Alt";
  case (USB_KEY_PRINTSCREEN):
    return "PrtSc";
  default:
    return "?";
  }
}

///////////////////////////////////////////////////////////////////////////
//
// CHECK FOR PAUSE KEY (FOR MUSIC ONLY)
//
///////////////////////////////////////////////////////////////////////////
void CheckPause(void) {
  if (Paused) {
    switch (SoundStatus) {
    case 0:
      SD_MusicOn();
      break;
    case 1:
      SD_MusicOff();
      break;
    }

    SoundStatus ^= 1;
    VW_WaitVBL(3);
    IN_ClearKeysDown();
    Paused = false;
  }
}

///////////////////////////////////////////////////////////////////////////
//
// DRAW GUN CURSOR AT CORRECT POSITION IN MENU
//
///////////////////////////////////////////////////////////////////////////
void DrawMenuGun(CP_iteminfo *iteminfo) {
  int x, y;

  x = iteminfo->x + 4;
  y = iteminfo->y + 2 + iteminfo->curpos * 13 - 2;
  VWB_DrawPic(x, y, C_CURSOR1PIC);
}

///////////////////////////////////////////////////////////////////////////
//
// DRAW SCREEN TITLE STRIPES
//
///////////////////////////////////////////////////////////////////////////
void DrawStripes(int y) {
#ifndef SPEAR
  VWB_Bar(0, y, 256, 24, 0);
  VWB_Hlin(0, 255, y + 22, STRIPE);
#else
  VWB_Bar(SCREEN_WIDTH_FACTOR(0), y, SCREEN_WIDTH_FACTOR(320), 22, 0);
  VWB_Hlin(0, 255, y + 23, 0);
#endif
}

void ShootSnd(void) { SD_PlaySound(SHOOTSND); }

///////////////////////////////////////////////////////////////////////////
//
// CHECK FOR EPISODES
//
///////////////////////////////////////////////////////////////////////////
void CheckForEpisodes(void) {
  struct stat statbuf;

  //
  // JAPANESE VERSION
  //
#ifdef JAPAN
#ifdef JAPDEMO
  if (!stat("vswap.wj1", &statbuf)) {
    strcpy(extension, "wj1");
    numEpisodesMissing = 5;
#else
  if (!stat("vswap.wj6", &statbuf)) {
    strcpy(extension, "wj6");
#endif
    strcat(configname, extension);
    strcat(SaveName, extension);
    strcat(demoname, extension);
    EpisodeSelect[1] = EpisodeSelect[2] = EpisodeSelect[3] = EpisodeSelect[4] = EpisodeSelect[5] = 1;
  } else
    Quit("NO JAPANESE WOLFENSTEIN 3-D DATA FILES to be found!");
#else

//
// ENGLISH
//
#ifdef UPLOAD
  if (!stat("vswap.wl1", &statbuf)) {
    strcpy(extension, "wl1");
    numEpisodesMissing = 5;
  } else
    Quit("NO WOLFENSTEIN 3-D DATA FILES to be found!");
#else
#ifndef SPEAR
  if (!stat("vswap.wl6", &statbuf)) {
    strcpy(extension, "wl6");
    NewEmenu[2].active = NewEmenu[4].active = NewEmenu[6].active = NewEmenu[8].active = NewEmenu[10].active = EpisodeSelect[1] =
        EpisodeSelect[2] = EpisodeSelect[3] = EpisodeSelect[4] = EpisodeSelect[5] = 1;
  } else {
    if (!stat("vswap.wl3", &statbuf)) {
      strcpy(extension, "wl3");
      numEpisodesMissing = 3;
      NewEmenu[2].active = NewEmenu[4].active = EpisodeSelect[1] = EpisodeSelect[2] = 1;
    } else {
      if (!stat("vswap.wl1", &statbuf)) {
        strcpy(extension, "wl1");
        numEpisodesMissing = 5;
      } else {
        printf("not found any\r\n");
        Quit("NO WOLFENSTEIN 3-D DATA FILES to be found!");
      }
    }
  }
#endif
#endif

#ifdef SPEAR
#ifndef SPEARDEMO
  if (param_mission == 0) {
    if (!stat("vswap.sod", &statbuf))
      strcpy(extension, "sod");
    else
      Quit("NO SPEAR OF DESTINY DATA FILES TO BE FOUND!");
  } else if (param_mission == 1) {
    if (!stat("vswap.sd1", &statbuf))
      strcpy(extension, "sd1");
    else
      Quit("NO SPEAR OF DESTINY DATA FILES TO BE FOUND!");
  } else if (param_mission == 2) {
    if (!stat("vswap.sd2", &statbuf))
      strcpy(extension, "sd2");
    else
      Quit("NO SPEAR OF DESTINY DATA FILES TO BE FOUND!");
  } else if (param_mission == 3) {
    if (!stat("vswap.sd3", &statbuf))
      strcpy(extension, "sd3");
    else
      Quit("NO SPEAR OF DESTINY DATA FILES TO BE FOUND!");
  } else
    Quit("UNSUPPORTED MISSION!");
  strcpy(graphext, "sod");
  strcpy(audioext, "sod");
#else
  if (!stat("vswap.sdm", &statbuf)) {
    strcpy(extension, "sdm");
  } else
    Quit("NO SPEAR OF DESTINY DEMO DATA FILES TO BE FOUND!");
  strcpy(graphext, "sdm");
  strcpy(audioext, "sdm");
#endif
#else
  strcpy(graphext, extension);
  strcpy(audioext, extension);
#endif

  strcat(configname, extension);
  strcat(SaveName, extension);
  strcat(demoname, extension);

#ifndef SPEAR
#ifndef GOODTIMES
  strcat(helpfilename, extension);
#endif
  strcat(endfilename, extension);
#endif
#endif
}
