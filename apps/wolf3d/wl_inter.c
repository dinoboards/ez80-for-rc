// WL_INTER.C

#include "ez80.h"
#include "wl_def.h"

#include "id_mm.h"

LRstruct LevelRatios[LRpack];
int32_t  lastBreathTime = 0;

void Write(int x, int y, const char *string);

//==========================================================================

/*
==================
=
= CLearSplitVWB
=
==================
*/

void ClearSplitVWB(void) {
  WindowX = 0;
  WindowY = 0;
  WindowW = 256;
  WindowH = 160;
}

//==========================================================================

#ifdef SPEAR
#ifndef SPEARDEMO
////////////////////////////////////////////////////////
//
// End of Spear of Destiny
//
////////////////////////////////////////////////////////

void EndScreen(int palette, int screen) {
  GRB_t pal[256];
  CA_CacheScreen(screen);
  VW_UpdateScreen();
  CA_CacheGrChunk(palette);
  VL_ConvertPalette(grsegs[palette], pal, 256);
  VL_FadeIn(0, 255, pal, 30);
  UNCACHEGRCHUNK(palette);
  IN_ClearKeysDown();
  IN_Ack();
  VW_FadeOut();
}

void EndSpear(void) {
  GRB_t pal[256];

  EndScreen(END1PALETTE, ENDSCREEN11PIC);

  CA_CacheScreen(ENDSCREEN3PIC);
  VW_UpdateScreen();
  CA_CacheGrChunk(END3PALETTE);
  VL_ConvertPalette(grsegs[END3PALETTE], pal, 256);
  VL_FadeIn(0, 255, pal, 30);
  UNCACHEGRCHUNK(END3PALETTE);
  fontnumber = 0;
  fontcolor  = 0xd0;
  WindowX    = 0;
  WindowW    = SCREEN_WIDTH_FACTOR(320);
  PrintX     = 0;
  PrintY     = 180;
  US_CPrint(STR_ENDGAME1 "\n");
  US_CPrint(STR_ENDGAME2);
  VW_UpdateScreen();
  IN_UserInput(700);

  PrintX = 0;
  PrintY = 180;
  VWB_Bar(SCREEN_WIDTH_FACTOR(0), 180, SCREEN_WIDTH_FACTOR(320), 20, 0);
  US_CPrint(STR_ENDGAME3 "\n");
  US_CPrint(STR_ENDGAME4);
  VW_UpdateScreen();
  IN_UserInput(700);

  VW_FadeOut();

  EndScreen(END4PALETTE, ENDSCREEN4PIC);
  EndScreen(END5PALETTE, ENDSCREEN5PIC);
  EndScreen(END6PALETTE, ENDSCREEN6PIC);
  EndScreen(END7PALETTE, ENDSCREEN7PIC);
  EndScreen(END8PALETTE, ENDSCREEN8PIC);
  EndScreen(END9PALETTE, ENDSCREEN9PIC);

  EndScreen(END2PALETTE, ENDSCREEN12PIC);

  MainMenu[savegame].active = 0;
}
#endif
#endif

//==========================================================================

/*
==================
=
= Victory
=
==================
*/

void Victory(void) {
#ifndef SPEARDEMO
  int32_t sec;
  int     i, min, kr, sr, tr, x;
  char    tempstr[8];

#define RATIOX 6
#define RATIOY 14
#define TIMEX  14
#define TIMEY  8

#ifdef SPEAR
  StartCPMusic(XTHEEND_MUS);

  CA_CacheGrChunk(BJCOLLAPSE1PIC);
  CA_CacheGrChunk(BJCOLLAPSE2PIC);
  CA_CacheGrChunk(BJCOLLAPSE3PIC);
  CA_CacheGrChunk(BJCOLLAPSE4PIC);

  VWB_Bar(SCREEN_WIDTH_FACTOR(0), 0, SCREEN_WIDTH_FACTOR(320), SCREEN_HEIGHT, VIEWCOLOR);
  VWB_DrawPic(124, 44, BJCOLLAPSE1PIC);
  VW_UpdateScreen();
  VW_FadeIn();
  VW_WaitVBL(2 * 70);
  VWB_DrawPic(124, 44, BJCOLLAPSE2PIC);
  VW_UpdateScreen();
  VW_WaitVBL(105);
  VWB_DrawPic(124, 44, BJCOLLAPSE3PIC);
  VW_UpdateScreen();
  VW_WaitVBL(105);
  VWB_DrawPic(124, 44, BJCOLLAPSE4PIC);
  VW_UpdateScreen();
  VW_WaitVBL(3 * 70);

  UNCACHEGRCHUNK(BJCOLLAPSE1PIC);
  UNCACHEGRCHUNK(BJCOLLAPSE2PIC);
  UNCACHEGRCHUNK(BJCOLLAPSE3PIC);
  UNCACHEGRCHUNK(BJCOLLAPSE4PIC);
  VL_FadeOut(0, 255, 0, 17, 17, 5);
#endif

  StartCPMusic(URAHERO_MUS);
  ClearSplitVWB();
  CacheLump(LEVELEND_LUMP_START, LEVELEND_LUMP_END);
  CA_CacheGrChunk(STARTFONT);

#ifndef SPEAR
  CA_CacheGrChunk(C_TIMECODEPIC);
#endif

  VWB_Bar(SCREEN_WIDTH_FACTOR(0), 0, SCREEN_WIDTH_FACTOR(320), screenHeight / scaleFactor - STATUSLINES + 1, VIEWCOLOR);
  if (bordercol != VIEWCOLOR)
    DrawStatusBorder(VIEWCOLOR);

#ifdef JAPAN
#ifndef JAPDEMO
  CA_CacheGrChunk(C_ENDRATIOSPIC);
  VWB_DrawPic(0, 0, C_ENDRATIOSPIC);
  UNCACHEGRCHUNK(C_ENDRATIOSPIC);
#endif
#else
  Write(18, 2, STR_YOUWIN);

  Write(TIMEX, TIMEY - 2, STR_TOTALTIME);

  Write(12, RATIOY - 2, "averages");

#ifdef SPANISH
  Write(RATIOX + 2, RATIOY, STR_RATKILL);
  Write(RATIOX + 2, RATIOY + 2, STR_RATSECRET);
  Write(RATIOX + 2, RATIOY + 4, STR_RATTREASURE);
#else
  Write(RATIOX + 8, RATIOY, STR_RATKILL);
  Write(RATIOX + 4, RATIOY + 2, STR_RATSECRET);
  Write(RATIOX, RATIOY + 4, STR_RATTREASURE);
#endif

#endif

#ifndef JAPDEMO
  VWB_DrawPic(8, 4, L_BJWINSPIC);
#endif

  for (kr = sr = tr = sec = i = 0; i < LRpack; i++) {
    sec += LevelRatios[i].time;
    kr += LevelRatios[i].kill;
    sr += LevelRatios[i].secret;
    tr += LevelRatios[i].treasure;
  }

#ifndef SPEAR
  kr /= LRpack;
  sr /= LRpack;
  tr /= LRpack;
#else
  kr /= 14;
  sr /= 14;
  tr /= 14;
#endif

  min = sec / 60;
  sec %= 60;

  if (min > 99)
    min = sec = 99;

  i = TIMEX * 8 + 1;
  VWB_DrawPic(i, TIMEY * 8, L_NUM0PIC + (min / 10));
  i += 2 * 8;
  VWB_DrawPic(i, TIMEY * 8, L_NUM0PIC + (min % 10));
  i += 2 * 8;
  Write(i / 8, TIMEY, ":");
  i += 1 * 8;
  VWB_DrawPic(i, TIMEY * 8, L_NUM0PIC + (sec / 10));
  i += 2 * 8;
  VWB_DrawPic(i, TIMEY * 8, L_NUM0PIC + (sec % 10));
  VW_UpdateScreen();

  sprintf(tempstr, "%d", kr);
  x = RATIOX + 24 - (int)strlen(tempstr) * 2;
  Write(x, RATIOY, tempstr);

  sprintf(tempstr, "%d", sr);
  x = RATIOX + 24 - (int)strlen(tempstr) * 2;
  Write(x, RATIOY + 2, tempstr);

  sprintf(tempstr, "%d", tr);
  x = RATIOX + 24 - (int)strlen(tempstr) * 2;
  Write(x, RATIOY + 4, tempstr);

#ifndef SPANISH
#ifndef UPLOAD
#ifndef SPEAR
  //
  // TOTAL TIME VERIFICATION CODE
  //
  if (gamestate.difficulty >= gd_medium) {
    VWB_DrawPic(30 * 8, TIMEY * 8, C_TIMECODEPIC);
    fontnumber = 0;
    fontcolor  = READHCOLOR;
    PrintX     = 30 * 8 - 3;
    PrintY     = TIMEY * 8 + 8;
    PrintX += 4;
    tempstr[0] = (((min / 10) ^ (min % 10)) ^ 0xa) + 'A';
    tempstr[1] = (int)((((sec / 10) ^ (sec % 10)) ^ 0xa) + 'A');
    tempstr[2] = (tempstr[0] ^ tempstr[1]) + 'A';
    tempstr[3] = 0;
    US_Print(tempstr);
  }
#endif
#endif
#endif

  fontnumber = 1;

  VW_UpdateScreen();
  VW_FadeIn();

  IN_Ack();

  VW_FadeOut();
  if (screenHeight % 200 != 0)
    VL_ClearScreen(0);

#ifndef SPEAR
  UNCACHEGRCHUNK(C_TIMECODEPIC);
#endif
  UnCacheLump(LEVELEND_LUMP_START, LEVELEND_LUMP_END);

#ifndef SPEAR
  EndText();
#else
  EndSpear();
#endif

#endif // SPEARDEMO
}

//==========================================================================

#ifndef JAPAN
/*
==================
=
= PG13
=
==================
*/

void PG13(void) {
  CA_CacheGrChunk(PG13PIC);

  VW_FadeOut();
  VWB_Bar(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x82); // background
  VWB_DrawPic(256 - 90, 192 - 70, PG13PIC);

  UNCACHEGRCHUNK(PG13PIC);
}
#endif

//==========================================================================

void Write(int x, int y, const char *string) {
  static const int alpha[] = {L_NUM0PIC, L_NUM1PIC,  L_NUM2PIC, L_NUM3PIC, L_NUM4PIC, L_NUM5PIC, L_NUM6PIC, L_NUM7PIC, L_NUM8PIC,
                              L_NUM9PIC, L_COLONPIC, 0,         0,         0,         0,         0,         0,         L_APIC,
                              L_BPIC,    L_CPIC,     L_DPIC,    L_EPIC,    L_FPIC,    L_GPIC,    L_HPIC,    L_IPIC,    L_JPIC,
                              L_KPIC,    L_LPIC,     L_MPIC,    L_NPIC,    L_OPIC,    L_PPIC,    L_QPIC,    L_RPIC,    L_SPIC,
                              L_TPIC,    L_UPIC,     L_VPIC,    L_WPIC,    L_XPIC,    L_YPIC,    L_ZPIC};

  int  i, ox, nx, ny, len = (int)strlen(string);
  char ch;

  ox = nx = x * 8;
  ny      = y * 8;
  for (i = 0; i < len; i++) {
    if (string[i] == '\n') {
      nx = ox;
      ny += 16;
    } else {
      ch = string[i];
      if (ch >= 'a')
        ch -= ('a' - 'A');
      ch -= '0';

      switch (string[i]) {
      case '!':
        VWB_DrawPic(nx, ny, L_EXPOINTPIC);
        nx += 8;
        continue;

#ifndef APOGEE_1_0
      case '\'':
        VWB_DrawPic(nx, ny, L_APOSTROPHEPIC);
        nx += 8;
        continue;
#endif

      case ' ':
        break;

      case 0x3a: // ':'
        VWB_DrawPic(nx, ny, L_COLONPIC);
        nx += 8;
        continue;

      case '%':
        VWB_DrawPic(nx, ny, L_PERCENTPIC);
        break;

      default:
        VWB_DrawPic(nx, ny, alpha[(int)ch]);
      }
      nx += 16;
    }
  }
}

//
// Breathe Mr. BJ!!!
//
void BJ_Breathe(void) {
  static int which = 0, max = 10;
  int        pics[2] = {L_GUYPIC, L_GUY2PIC};

  if ((int24_t)GetTimeCount() > max) {
    which ^= 1;
    VWB_DrawPic(0, 16, pics[which]);
    VW_UpdateScreen();
    SetTimeCount(0);
    max = 35;
  }
}

/*
==================
=
= LevelCompleted
=
= Entered with the screen faded out
= Still in split screen mode with the status bar
=
= Exit with the screen faded out
=
==================
*/

void LevelCompleted(void) {
#define VBLWAIT       30
#define PAR_AMOUNT    500
#define PERCENT100AMT 10000
  typedef struct {
    float time;
    char  timestr[6];
  } times;

  int     x, i, min, sec, ratio, kr, sr, tr;
  char    tempstr[10];
  int24_t bonus, timeleft = 0;
  times   parTimes[] = {
#ifndef SPEAR
      //
      // Episode One Par Times
      //
      {1.5, "01:30"},
      {2, "02:00"},
      {2, "02:00"},
      {3.5, "03:30"},
      {3, "03:00"},
      {3, "03:00"},
      {2.5, "02:30"},
      {2.5, "02:30"},
      {0, "??:??"}, // Boss level
      {0, "??:??"}, // Secret level

      //
      // Episode Two Par Times
      //
      {1.5, "01:30"},
      {3.5, "03:30"},
      {3, "03:00"},
      {2, "02:00"},
      {4, "04:00"},
      {6, "06:00"},
      {1, "01:00"},
      {3, "03:00"},
      {0, "??:??"},
      {0, "??:??"},

      //
      // Episode Three Par Times
      //
      {1.5, "01:30"},
      {1.5, "01:30"},
      {2.5, "02:30"},
      {2.5, "02:30"},
      {3.5, "03:30"},
      {2.5, "02:30"},
      {2, "02:00"},
      {6, "06:00"},
      {0, "??:??"},
      {0, "??:??"},

      //
      // Episode Four Par Times
      //
      {2, "02:00"},
      {2, "02:00"},
      {1.5, "01:30"},
      {1, "01:00"},
      {4.5, "04:30"},
      {3.5, "03:30"},
      {2, "02:00"},
      {4.5, "04:30"},
      {0, "??:??"},
      {0, "??:??"},

      //
      // Episode Five Par Times
      //
      {2.5, "02:30"},
      {1.5, "01:30"},
      {2.5, "02:30"},
      {2.5, "02:30"},
      {4, "04:00"},
      {3, "03:00"},
      {4.5, "04:30"},
      {3.5, "03:30"},
      {0, "??:??"},
      {0, "??:??"},

      //
      // Episode Six Par Times
      //
      {6.5, "06:30"},
      {4, "04:00"},
      {4.5, "04:30"},
      {6, "06:00"},
      {5, "05:00"},
      {5.5, "05:30"},
      {5.5, "05:30"},
      {8.5, "08:30"},
      {0, "??:??"},
      {0, "??:??"}
#else
      //
      // SPEAR OF DESTINY TIMES
      //
      {1.5, "01:30"}, {3.5, "03:30"},  {2.75, "02:45"}, {3.5, "03:30"},  {0, "??:??"},               // Boss 1
      {4.5, "04:30"}, {3.25, "03:15"}, {2.75, "02:45"}, {4.75, "04:45"}, {0, "??:??"},               // Boss 2
      {6.5, "06:30"}, {4.5, "04:30"},  {2.75, "02:45"}, {4.5, "04:30"},  {6, "06:00"}, {0, "??:??"}, // Boss 3
      {6, "06:00"},   {0, "??:??"},                                                                  // Boss 4
      {0, "??:??"},                                                                                  // Secret level 1
      {0, "??:??"},                                                                                  // Secret level 2
#endif
  };

  CacheLump(LEVELEND_LUMP_START, LEVELEND_LUMP_END);
  ClearSplitVWB(); // set up for double buffering in split screen
  VWB_Bar(SCREEN_WIDTH_FACTOR(0), 0, SCREEN_WIDTH_FACTOR(320), screenHeight / scaleFactor - STATUSLINES + 1, VIEWCOLOR);

  if (bordercol != VIEWCOLOR)
    DrawStatusBorder(VIEWCOLOR);

  StartCPMusic(ENDLEVEL_MUS);

  //
  // do the intermission
  //
  IN_ClearKeysDown();
  IN_StartAck();

#ifdef JAPAN
  CA_CacheGrChunk(C_INTERMISSIONPIC);
  VWB_DrawPic(0, 0, C_INTERMISSIONPIC);
  UNCACHEGRCHUNK(C_INTERMISSIONPIC);
#endif
  VWB_DrawPic(0, 16, L_GUYPIC);

#ifndef SPEAR
  if (mapon < 8)
#else
  if (mapon != 4 && mapon != 9 && mapon != 15 && mapon < 17)
#endif
  {
#ifndef JAPAN
#ifdef SPANISH
    Write(14, 2, "piso\ncompletado");
#else
    Write(14, 2, "floor\ncompleted");
#endif

    Write(14, 7, STR_BONUS "     0");
    Write(16, 10, STR_TIME);
    Write(16, 12, STR_PAR);

#ifdef SPANISH
    Write(11, 14, STR_RAT2KILL);
    Write(11, 16, STR_RAT2SECRET);
    Write(11, 18, STR_RAT2TREASURE);
#else
    Write(9, 14, STR_RAT2KILL);
    Write(5, 16, STR_RAT2SECRET);
    Write(1, 18, STR_RAT2TREASURE);
#endif
    sprintf(tempstr, "%d", gamestate.mapon + 1);
    Write(26, 2, tempstr);
#endif

#ifdef SPANISH
    Write(30, 12, parTimes[gamestate.episode * 10 + mapon].timestr);
#else
    Write(26, 12, parTimes[gamestate.episode * 10 + mapon].timestr);
#endif

    //
    // PRINT TIME
    //
    sec = gamestate.TimeCount / 70;

    if (sec > 99 * 60) // 99 minutes max
      sec = 99 * 60;

    if (gamestate.TimeCount < parTimes[gamestate.episode * 10 + mapon].time * 4200)
      timeleft = (int32_t)((parTimes[gamestate.episode * 10 + mapon].time * 4200) / 70 - sec);

    min = sec / 60;
    sec %= 60;

#ifdef SPANISH
    i = 30 * 8;
#else
    i = 26 * 8;
#endif
    VWB_DrawPic(i, 10 * 8, L_NUM0PIC + (min / 10));
    i += 2 * 8;
    VWB_DrawPic(i, 10 * 8, L_NUM0PIC + (min % 10));
    i += 2 * 8;
    Write(i / 8, 10, ":");
    i += 1 * 8;
    VWB_DrawPic(i, 10 * 8, L_NUM0PIC + (sec / 10));
    i += 2 * 8;
    VWB_DrawPic(i, 10 * 8, L_NUM0PIC + (sec % 10));

    VW_UpdateScreen();
    VW_FadeIn();

    //
    // FIGURE RATIOS OUT BEFOREHAND
    //
    kr = sr = tr = 0;
    if (gamestate.killtotal)
      kr = (gamestate.killcount * 100) / gamestate.killtotal;
    if (gamestate.secrettotal)
      sr = (gamestate.secretcount * 100) / gamestate.secrettotal;
    if (gamestate.treasuretotal)
      tr = (gamestate.treasurecount * 100) / gamestate.treasuretotal;

    //
    // PRINT TIME BONUS
    //
    bonus = timeleft * PAR_AMOUNT;
    if (bonus) {
      for (i = 0; i <= timeleft; i++) {
        sprintf(tempstr, "%ld", (int32_t)i * PAR_AMOUNT);
        x = 36 - (int)strlen(tempstr) * 2;
        Write(x, 7, tempstr);
        if (!(i % (PAR_AMOUNT / 10)))
          SD_PlaySound(ENDBONUS1SND);
        if (!usedoublebuffering || !(i % (PAR_AMOUNT / 50)))
          VW_UpdateScreen();
#ifdef SOUND_ENABLED
        while (SD_SoundPlaying())
          BJ_Breathe();
#endif
        if (IN_CheckAck())
          goto done;
      }

      VW_UpdateScreen();

#ifdef SOUND_ENABLED
      SD_PlaySound(ENDBONUS2SND);
      while (SD_SoundPlaying())
        BJ_Breathe();
#endif
    }

#ifdef SPANISH
#define RATIOXX 33
#else
#define RATIOXX 37
#endif
    //
    // KILL RATIO
    //
    ratio = kr;
    for (i = 0; i <= ratio; i++) {
      sprintf(tempstr, "%d", i);
      x = RATIOXX - (int)strlen(tempstr) * 2;
      Write(x, 14, tempstr);
      if (!(i % 10))
        SD_PlaySound(ENDBONUS1SND);
      if (!usedoublebuffering || !(i & 1))
        VW_UpdateScreen();
#ifdef SOUND_ENABLED
      while (SD_SoundPlaying())
        BJ_Breathe();
#endif
      if (IN_CheckAck())
        goto done;
    }
    if (ratio >= 100) {
      VW_WaitVBL(VBLWAIT);
      SD_StopSound();
      bonus += PERCENT100AMT;
      sprintf(tempstr, "%d", bonus);
      x = (RATIOXX - 1) - (int)strlen(tempstr) * 2;
      Write(x, 7, tempstr);
      VW_UpdateScreen();
      SD_PlaySound(PERCENT100SND);
    } else if (!ratio) {
      VW_WaitVBL(VBLWAIT);
      SD_StopSound();
      SD_PlaySound(NOBONUSSND);
    } else
      SD_PlaySound(ENDBONUS2SND);

    VW_UpdateScreen();
#ifdef SOUND_ENABLED
    while (SD_SoundPlaying())
      BJ_Breathe();
#endif
    //
    // SECRET RATIO
    //
    ratio = sr;
    for (i = 0; i <= ratio; i++) {
      sprintf(tempstr, "%d", i);
      x = RATIOXX - (int)strlen(tempstr) * 2;
      Write(x, 16, tempstr);
      if (!(i % 10))
        SD_PlaySound(ENDBONUS1SND);
      if (!usedoublebuffering || !(i & 1))
        VW_UpdateScreen();

#ifdef SOUND_ENABLED
      while (SD_SoundPlaying())
        BJ_Breathe();
#endif
      if (IN_CheckAck())
        goto done;
    }
    if (ratio >= 100) {
      VW_WaitVBL(VBLWAIT);
      SD_StopSound();
      bonus += PERCENT100AMT;
      sprintf(tempstr, "%d", bonus);
      x = (RATIOXX - 1) - (int)strlen(tempstr) * 2;
      Write(x, 7, tempstr);
      VW_UpdateScreen();
      SD_PlaySound(PERCENT100SND);
    } else if (!ratio) {
      VW_WaitVBL(VBLWAIT);
      SD_StopSound();
      SD_PlaySound(NOBONUSSND);
    } else
      SD_PlaySound(ENDBONUS2SND);
    VW_UpdateScreen();
#ifdef SOUND_ENABLED
    while (SD_SoundPlaying())
      BJ_Breathe();
#endif
    //
    // TREASURE RATIO
    //
    ratio = tr;
    for (i = 0; i <= ratio; i++) {
      sprintf(tempstr, "%d", i);
      x = RATIOXX - (int)strlen(tempstr) * 2;
      Write(x, 18, tempstr);
      if (!(i % 10))
        SD_PlaySound(ENDBONUS1SND);
      if (!usedoublebuffering || !(i & 1))
        VW_UpdateScreen();
#ifdef SOUND_ENABLED
      while (SD_SoundPlaying())
        BJ_Breathe();
#endif
      if (IN_CheckAck())
        goto done;
    }
    if (ratio >= 100) {
      VW_WaitVBL(VBLWAIT);
      SD_StopSound();
      bonus += PERCENT100AMT;
      sprintf(tempstr, "%d", bonus);
      x = (RATIOXX - 1) - (int)strlen(tempstr) * 2;
      Write(x, 7, tempstr);
      VW_UpdateScreen();
      SD_PlaySound(PERCENT100SND);
    } else if (!ratio) {
      VW_WaitVBL(VBLWAIT);
      SD_StopSound();
      SD_PlaySound(NOBONUSSND);
    } else
      SD_PlaySound(ENDBONUS2SND);
    VW_UpdateScreen();
#ifdef SOUND_ENABLED
    while (SD_SoundPlaying())
      BJ_Breathe();
#endif
    //
    // JUMP STRAIGHT HERE IF KEY PRESSED
    //
  done:
    sprintf(tempstr, "%d", kr);
    x = RATIOXX - (int)strlen(tempstr) * 2;
    Write(x, 14, tempstr);

    sprintf(tempstr, "%d", sr);
    x = RATIOXX - (int)strlen(tempstr) * 2;
    Write(x, 16, tempstr);

    sprintf(tempstr, "%d", tr);
    x = RATIOXX - (int)strlen(tempstr) * 2;
    Write(x, 18, tempstr);

    bonus = (int24_t)timeleft * PAR_AMOUNT + (PERCENT100AMT * (kr >= 100)) + (PERCENT100AMT * (sr >= 100)) +
            (PERCENT100AMT * (tr >= 100));

    GivePoints(bonus);
    sprintf(tempstr, "%d", bonus);
    x = 36 - (int)strlen(tempstr) * 2;
    Write(x, 7, tempstr);

    //
    // SAVE RATIO INFORMATION FOR ENDGAME
    //
    LevelRatios[mapon].kill     = kr;
    LevelRatios[mapon].secret   = sr;
    LevelRatios[mapon].treasure = tr;
    LevelRatios[mapon].time     = min * 60 + sec;
  } else {
#ifdef SPEAR
#ifndef SPEARDEMO
    switch (mapon) {
    case 4:
      Write(14, 4,
            " trans\n"
            " grosse\n" STR_DEFEATED);
      break;
    case 9:
      Write(14, 4,
            "barnacle\n"
            "wilhelm\n" STR_DEFEATED);
      break;
    case 15:
      Write(14, 4, "ubermutant\n" STR_DEFEATED);
      break;
    case 17:
      Write(14, 4,
            " death\n"
            " knight\n" STR_DEFEATED);
      break;
    case 18:
      Write(13, 4,
            "secret tunnel\n"
            "    area\n"
            "  completed!");
      break;
    case 19:
      Write(13, 4,
            "secret castle\n"
            "    area\n"
            "  completed!");
      break;
    }
#endif
#else
    Write(14, 4, "secret floor\n completed!");
#endif

    Write(10, 16, "15000 bonus!");

    VW_UpdateScreen();
    VW_FadeIn();

    GivePoints(15000);
  }

  DrawScore();
  VW_UpdateScreen();

  lastBreathTime = GetTimeCount();
  IN_StartAck();
  while (!IN_CheckAck())
    BJ_Breathe();

    //
    // done
    //
#ifdef SPEARDEMO
  if (gamestate.mapon == 1) {
    SD_PlaySound(BONUS1UPSND);

    CA_CacheGrChunk(STARTFONT + 1);
    Message("This concludes your demo\n"
            "of Spear of Destiny! Now,\n"
            "go to your local software\n"
            "store and buy it!");
    UNCACHEGRCHUNK(STARTFONT + 1);

    IN_ClearKeysDown();
    IN_Ack();
  }
#endif

#ifdef JAPDEMO
  if (gamestate.mapon == 3) {
    SD_PlaySound(BONUS1UPSND);

    CA_CacheGrChunk(STARTFONT + 1);
    Message("This concludes your demo\n"
            "of Wolfenstein 3-D! Now,\n"
            "go to your local software\n"
            "store and buy it!");
    UNCACHEGRCHUNK(STARTFONT + 1);

    IN_ClearKeysDown();
    IN_Ack();
  }
#endif

  VW_FadeOut();
  DrawPlayBorder();

  UnCacheLump(LEVELEND_LUMP_START, LEVELEND_LUMP_END);
}

//==========================================================================

/*
=================
=
= PreloadGraphics
=
= Fill the cache up
=
=================
*/

void PreloadUpdate(uint8_t current) {
  unsigned w = WindowW - 10;

  VWB_Bar(WindowX + 5, WindowY + WindowH - 3, w, 2, BLACK);
  w = ((int32_t)w * current) / 100;
  if (w) {
    VWB_Bar(WindowX + 5, WindowY + WindowH - 3, w, 2, 0x37); // SECONDCOLOR);
    VWB_Bar(WindowX + 5, WindowY + WindowH - 3, w - 1, 1, 0x32);
  }
  VW_UpdateScreen();
}

void PreloadGraphics(void) {
  DrawLevel();
  ClearSplitVWB(); // set up for double buffering in split screen

  VWB_Bar(0, 0, screenWidth, screenHeight - (STATUSLINES - 1), bordercol);
  LatchDrawPic(((screenWidth - 224) / 16) * 8, (screenHeight - (STATUSLINES + 48)) / 2, GETPSYCHEDPIC);

  WindowX = (screenWidth - 224) / 2;
  WindowY = (screenHeight - (STATUSLINES + 48)) / 2;
  WindowW = 28 * 8;
  WindowH = 48;

  VW_UpdateScreen();
  VW_FadeIn();

  PM_Preload(PreloadUpdate, false);
  IN_UserInput(70);
  VW_FadeOut();

  DrawPlayBorder();
  VW_UpdateScreen();
}

//==========================================================================

/*
==================
=
= DrawHighScores
=
==================
*/

void DrawHighScores(void) {
  char buffer[16];
#ifndef SPEAR
  char *str;
#ifndef UPLOAD
  char buffer1[5];
#endif
#endif
  word       i, w, h;
  HighScore *s;

#ifndef SPEAR
  CA_CacheGrChunk(HIGHSCORESPIC);
  CA_CacheGrChunk(STARTFONT);
#ifndef APOGEE_1_0
  CA_CacheGrChunk(C_LEVELPIC);
  CA_CacheGrChunk(C_SCOREPIC);
  CA_CacheGrChunk(C_NAMEPIC);
#else
  CA_CacheGrChunk(C_CODEPIC);
#endif

  ClearMScreen();

  DrawStripes(10);

  VWB_DrawPic(16, 0, HIGHSCORESPIC); // 224x56
  UNCACHEGRCHUNK(HIGHSCORESPIC);

#ifndef APOGEE_1_0
  VWB_DrawPic(4 * 8, 68, C_NAMEPIC);
  VWB_DrawPic(20 * 8, 68, C_LEVELPIC);
  VWB_DrawPic(SCREEN_WIDTH - 32, 68, C_SCOREPIC); // 32x8
#else
  VWB_DrawPic(35 * 8, 68, C_CODEPIC);
#endif
  fontnumber = 0;

#else
  CacheLump(BACKDROP_LUMP_START, BACKDROP_LUMP_END);
  ClearMScreen();
  DrawStripes(10);
  UnCacheLump(BACKDROP_LUMP_START, BACKDROP_LUMP_END);

  CacheLump(HIGHSCORES_LUMP_START, HIGHSCORES_LUMP_END);
  CA_CacheGrChunk(STARTFONT + 1);
  VWB_DrawPic(0, 0, HIGHSCORESPIC);

  fontnumber = 1;
#endif

#ifndef SPEAR
  SETFONTCOLOR(15, 0x29);
#else
  SETFONTCOLOR(HIGHLIGHT, 0x29);
#endif

  for (i = 0, s = Scores; i < MaxScores; i++, s++) {
    PrintY = 76 + (16 * i);

    //
    // name
    //
#ifndef SPEAR
    PrintX = 3 * 8;
#else
    PrintX = 16;
#endif

    US_Print(s->name);

    //
    // level
    //
    sprintf(buffer, "%d", s->completed);
#ifndef SPEAR
    for (str = buffer; *str; str++)
      *str = *str + (129 - '0'); // Used fixed-width numbers (129...)
    USL_MeasureString(buffer, &w, &h);
    PrintX = (21 * 8) - w;
#else
    USL_MeasureString(buffer, &w, &h);
    PrintX = 194 - w;
#endif

#ifndef UPLOAD
#ifndef SPEAR
    PrintX -= 6;
    sprintf(buffer1, "%d", s->episode + 1);
    US_Print("E");
    US_Print(buffer1);
    US_Print("/L");
#endif
#endif

#ifdef SPEAR
    if (s->completed == 21)
      VWB_DrawPic(PrintX + 8, PrintY - 1, C_WONSPEARPIC);
    else
#endif
      US_Print(buffer);

    //
    // score
    //
    sprintf(buffer, "%d", (int)s->score); //%ld not supported - so int32 score not supported!
#ifndef SPEAR
    for (str = buffer; *str; str++)
      *str = *str + (129 - '0'); // Used fixed-width numbers (129...)
    USL_MeasureString(buffer, &w, &h);
    PrintX = (32 * 8) - 8 - w;
#else
    USL_MeasureString(buffer, &w, &h);
    PrintX = 292 - w;
#endif
    US_Print(buffer);

#ifdef APOGEE_1_0
    // #ifndef UPLOAD
#ifndef SPEAR
    //
    // verification #
    //
    if (!i) {
      char temp  = (((s->score >> 28) & 0xf) ^ ((s->score >> 24) & 0xf)) + 'A';
      char temp1 = (((s->score >> 20) & 0xf) ^ ((s->score >> 16) & 0xf)) + 'A';
      char temp2 = (((s->score >> 12) & 0xf) ^ ((s->score >> 8) & 0xf)) + 'A';
      char temp3 = (((s->score >> 4) & 0xf) ^ ((s->score >> 0) & 0xf)) + 'A';

      SETFONTCOLOR(0x49, 0x29);
      PrintX    = 35 * 8;
      buffer[0] = temp;
      buffer[1] = temp1;
      buffer[2] = temp2;
      buffer[3] = temp3;
      buffer[4] = 0;
      US_Print(buffer);
      SETFONTCOLOR(15, 0x29);
    }
#endif
    // #endif
#endif
  }

#ifdef SPEAR
  UnCacheLump(HIGHSCORES_LUMP_START, HIGHSCORES_LUMP_END);
  fontnumber = 0;
#endif
}

//===========================================================================

/*
=======================
=
= CheckHighScore
=
=======================
*/

void CheckHighScore(int32_t score, word other) {
  word      i, j;
  int       n;
  HighScore myscore;

  strcpy(myscore.name, "");
  myscore.score     = score;
  myscore.episode   = gamestate.episode;
  myscore.completed = other;

  for (i = 0, n = -1; i < MaxScores; i++) {
    if ((myscore.score > Scores[i].score) || ((myscore.score == Scores[i].score) && (myscore.completed > Scores[i].completed))) {
      for (j = MaxScores; --j > i;)
        Scores[j] = Scores[j - 1];
      Scores[i] = myscore;
      n         = i;
      break;
    }
  }

#ifdef SPEAR
  StartCPMusic(XAWARD_MUS);
#else
  StartCPMusic(ROSTER_MUS);
#endif
  DrawHighScores();

  VW_FadeIn();

  if (n != -1) {
    //
    // got a high score
    //
    PrintY = 76 + (16 * n);
#ifndef SPEAR
    PrintX    = 4 * 8;
    backcolor = BORDCOLOR;
    fontcolor = 15;
    US_LineInput(PrintX, PrintY, Scores[n].name, 0, true, MaxHighName, 100);
#else
    PrintX = 16;
    fontnumber = 1;
    VWB_Bar(PrintX - 2, PrintY - 2, SCREEN_WIDTH_FACTOR(145), 15, 0x9c);
    VW_UpdateScreen();
    backcolor = 0x9c;
    fontcolor = 15;
    US_LineInput(PrintX, PrintY, Scores[n].name, 0, true, MaxHighName, 130);
#endif
  } else {
    IN_ClearKeysDown();
    IN_UserInput(500);
  }
}

#ifndef UPLOAD
#ifndef SPEAR
#ifndef JAPAN
////////////////////////////////////////////////////////
//
// NON-SHAREWARE NOTICE
//
////////////////////////////////////////////////////////
void NonShareware(void) {
  VW_FadeOut();

  ClearMScreen();
  DrawStripes(10);

  CA_CacheGrChunk(STARTFONT + 1);
  fontnumber = 1;

  SETFONTCOLOR(READHCOLOR, BKGDCOLOR);
  PrintX = 110;
  PrintY = 15;

#ifdef SPANISH
  US_Print("Atencion");
#else
  US_Print("Attention");
#endif

  SETFONTCOLOR(HIGHLIGHT, BKGDCOLOR);
  WindowX = PrintX = 40;
  printf("WindowX = 40\r\n");
  PrintY = 60;
#ifdef SPANISH
  US_Print("Este juego NO es gratis y\n");
  US_Print("NO es Shareware; favor de\n");
  US_Print("no distribuirlo.\n\n");
#else
  US_Print("This game is NOT shareware.\n");
  US_Print("Please do not distribute it.\n");
  US_Print("Thanks.\n\n");
#endif
  US_Print("        Id Software\n");

  VW_UpdateScreen();
  VW_FadeIn();
  IN_Ack();
}
#endif
#endif
#endif

#ifdef SPEAR
#ifndef GOODTIMES
#ifndef SPEARDEMO
////////////////////////////////////////////////////////
//
// COPY PROTECTION FOR FormGen
//
////////////////////////////////////////////////////////
char CopyProFailedStrs[][100] = {STR_COPY1,  STR_COPY2,

                                 STR_COPY3,  STR_COPY4,

                                 STR_COPY5,  STR_COPY6,

                                 STR_COPY7,  STR_COPY8,

                                 STR_COPY9,  "",

                                 STR_COPY10, STR_COPY11,

                                 STR_COPY12, "",

                                 STR_COPY13, "",

                                 STR_COPY14, ""};

char BackDoorStrs[5][16] = {"a spoon?", "bite me!", "joshua", "pelt", "snoops"};

char GoodBoyStrs[10][40] = {"...is the CORRECT ANSWER!",
                            "",

                            "Consider yourself bitten, sir.",
                            "",

                            "Greetings Professor Falken, would you",
                            "like to play Spear of Destiny?",

                            "Do you have any gold spray paint?",
                            "",

                            "I wish I had a 21\" monitor...",
                            ""};

char bossstrs[4][24] = {"DEATH KNIGHT", "BARNACLE WILHELM", "UBERMUTANTUBER MUTANT", "TRANS GROSSE"};

char WordStr[5][20] = {"New Game", "Sound...F4", "Control...F6", "Change View...F5", "Quit...F10"};

char WordCorrect[5][2] = {"3", "4", "4", "5", "5"};

char MemberStr[10][40] = {STR_COPY15, "",

                          STR_COPY16, "",

                          STR_COPY17, STR_COPY18,

                          STR_COPY19, STR_COPY20,

                          STR_COPY21, STR_COPY22};

char MemberCorrect[5][24] = {"adrian carmack", "john carmackjohn romero", "tom hall", "jay wilbur", "kevin cloud"};

char DosMessages[9][80] = {STR_NOPE1, STR_NOPE2, STR_NOPE3, STR_NOPE4, STR_NOPE5, STR_NOPE6, STR_NOPE7, STR_NOPE8, STR_NOPE9};

char MiscTitle[4][20] = {"BLOOD TEST", "STRAIGHT-LACED", "QUITE SHAPELY", "I AM WHAT I AMMO"};

char MiscStr[12][40] = {STR_MISC1, STR_MISC2, "",

                        STR_MISC3, STR_MISC4, "",

                        STR_MISC5, STR_MISC6, "",

                        STR_MISC7, STR_MISC8, STR_MISC9};

char MiscCorrect[4][5] = {"ss", "8", STR_STAR, "45"};

int BackDoor(char *s) {
  for (int i = 0; i < 5; i++) {
    if (!strcasecmp(s, BackDoorStrs[i])) {
      SETFONTCOLOR(14, 15);
      fontnumber = 0;
      PrintY     = 175;
      VWB_DrawPic(0, 20 * 8, COPYPROTBOXPIC);
      US_CPrint(GoodBoyStrs[i * 2]);
      US_CPrint(GoodBoyStrs[i * 2 + 1]);
      VW_UpdateScreen();
      return 1;
    }
  }

  return 0;
}

void CopyProtection(void) {
#define TYPEBOX_Y    177
#define TYPEBOX_BKGD 0x9c
#define PRINTCOLOR   HIGHLIGHT

  unsigned i;
  int      match, whichboss, bossnum, attempt, whichline;
  int      enemypicked[4] = {0, 0, 0, 0};
  int      bosses[4]      = {BOSSPIC1PIC, BOSSPIC2PIC, BOSSPIC3PIC, BOSSPIC4PIC};
  int      whichpicked[4] = {0, 0, 0, 0};
  int      whichone, quiztype, whichmem, whichword;
  int      memberpicked[5] = {0, 0, 0, 0, 0};
  int      wordpicked[5]   = {0, 0, 0, 0, 0};

  char inputbuffer[20];
  char message[80];

  enum {
    debriefing,
    checkmanual,
    staffquiz,
    miscquiz,

    totaltypes
  };

  attempt = 0;
  VW_FadeOut();
  CA_CacheGrChunk(C_BACKDROPPIC);
  CacheLump(COPYPROT_LUMP_START, COPYPROT_LUMP_END);
  CA_CacheGrChunk(STARTFONT + 1);
  CA_LoadAllSounds();
  StartCPMusic(COPYPRO_MUS);
  US_InitRndT(true);

  while (attempt < 3) {
    fontnumber = 1;
    SETFONTCOLOR(PRINTCOLOR - 2, 15);
    VWB_DrawPic(0, 0, C_BACKDROPPIC);
    VWB_DrawPic(0, 0, COPYPROTTOPPIC);
    VWB_DrawPic(0, 20 * 8, COPYPROTBOXPIC);
    WindowX = WindowY = 0;
    WindowW           = SCREEN_WIDTH_FACTOR(320);
    WindowH           = 200;
    PrintY            = 65;

    quiztype = US_RndT() % totaltypes;
    switch (quiztype) {
      //
      // BOSSES QUIZ
      //
    case debriefing: {
      PrintX = 0;
      US_Print(STR_DEBRIEF);
      SETFONTCOLOR(PRINTCOLOR, 15);

      while (enemypicked[whichboss = US_RndT() & 3])
        ;
      enemypicked[whichboss] = 1;
      bossnum                = bosses[whichboss];
      VWB_DrawPic(128, 60, bossnum);
      fontnumber = 0;
      PrintY     = 130;
      US_CPrint(STR_ENEMY1 "\n");
      US_CPrint(STR_ENEMY2 "\n\n");

      VW_UpdateScreen();
      VW_FadeIn();

      PrintX         = 100;
      fontcolor      = 15;
      backcolor      = TYPEBOX_BKGD;
      inputbuffer[0] = 0;
      PrintY         = TYPEBOX_Y;
      fontnumber     = 1;
      US_LineInput(PrintX, PrintY, inputbuffer, 0, true, 20, 100);

      match           = 0;
      size_t inputlen = strlen(inputbuffer);
      if (inputlen > 3) {
        size_t bosslen = strlen(bossstrs[whichboss]);
        for (i = 0; i < bosslen; i++) {
          if (!strncasecmp(inputbuffer, bossstrs[whichboss] + i, inputlen)) {
            match = 1;
            break;
          }
        }
      }

      match += BackDoor(inputbuffer);
      break;
    }

    //
    // MANUAL CHECK
    //
    case checkmanual: {
      while (wordpicked[whichword = US_RndT() % 5])
        ;
      wordpicked[whichword] = 1;
      US_CPrint(STR_CHECKMAN);
      SETFONTCOLOR(PRINTCOLOR, 15);
      PrintY += 25;
      US_CPrint(STR_MAN1);
      US_CPrint(STR_MAN2);
      sprintf(message, STR_MAN3 " \"%s\" " STR_MAN4, WordStr[whichword]);
      US_CPrint(message);
      VW_UpdateScreen();
      VW_FadeIn();

      PrintX         = 146;
      fontcolor      = 15;
      backcolor      = TYPEBOX_BKGD;
      inputbuffer[0] = 0;
      PrintY         = TYPEBOX_Y;
      US_LineInput(PrintX, PrintY, inputbuffer, 0, true, 6, 100);

      match = 1 - (strcasecmp(inputbuffer, WordCorrect[whichword]) != 0);
      match += BackDoor(inputbuffer);
      break;
    }

    //
    // STAFF QUIZ
    //
    case staffquiz: {
      while (memberpicked[whichmem = US_RndT() % 5])
        ;
      memberpicked[whichmem] = 1;
      US_CPrint(STR_ID1);
      SETFONTCOLOR(PRINTCOLOR, 15);
      PrintY += 25;
      US_CPrint(MemberStr[whichmem * 2]);
      US_CPrint(MemberStr[whichmem * 2 + 1]);
      VW_UpdateScreen();
      VW_FadeIn();

      PrintX         = 100;
      fontcolor      = 15;
      backcolor      = TYPEBOX_BKGD;
      inputbuffer[0] = 0;
      PrintY         = TYPEBOX_Y;
      US_LineInput(PrintX, PrintY, inputbuffer, 0, true, 20, 120);

      match           = 0;
      size_t inputlen = strlen(inputbuffer);
      if (inputlen > 2) {
        size_t memberlen = strlen(MemberCorrect[whichmem]);
        for (i = 0; i < memberlen; i++) {
          if (!strncasecmp(inputbuffer, MemberCorrect[whichmem] + i, inputlen)) {
            match = 1;
            break;
          }
        }
      }

      match += BackDoor(inputbuffer);
      break;
    }

    //
    // MISCELLANEOUS QUESTIONS
    //
    case miscquiz: {
      while (whichpicked[whichone = US_RndT() & 3])
        ;
      whichpicked[whichone] = 1;
      US_CPrint(MiscTitle[whichone]);
      SETFONTCOLOR(PRINTCOLOR, 15);
      PrintY += 25;
      US_CPrint(MiscStr[whichone * 3]);
      US_CPrint(MiscStr[whichone * 3 + 1]);
      US_CPrint(MiscStr[whichone * 3 + 2]);
      VW_UpdateScreen();
      VW_FadeIn();

      PrintX         = 146;
      fontcolor      = 15;
      backcolor      = TYPEBOX_BKGD;
      inputbuffer[0] = 0;
      PrintY         = TYPEBOX_Y;
      US_LineInput(PrintX, PrintY, inputbuffer, 0, true, 6, 100);

      match = 1 - (strcasecmp(inputbuffer, MiscCorrect[whichone]) != 0);
      match += BackDoor(inputbuffer);
      break;
    }
    }

    //
    // IF NO MATCH, WE'VE GOT A (MINOR) PROBLEM!
    //

    if (!match) {
      whichline = 2 * (US_RndT() % 9);
      SETFONTCOLOR(14, 15);
      fontnumber = 0;
      PrintY     = 175;
      VWB_DrawPic(0, 20 * 8, COPYPROTBOXPIC);
      US_CPrint(CopyProFailedStrs[whichline]);
      US_CPrint(CopyProFailedStrs[whichline + 1]);

      VW_UpdateScreen();
      SD_PlaySound(NOWAYSND);
      IN_UserInput(TickBase * 2);
      VW_FadeOut();
      attempt++;
    } else {
      int start;

      SD_PlaySound(BONUS1UPSND);
      SD_WaitSoundDone();
      UNCACHEGRCHUNK(STARTFONT + 1);
      UNCACHEGRCHUNK(C_BACKDROPPIC);
      UnCacheLump(COPYPROT_LUMP_START, COPYPROT_LUMP_END);

      switch (SoundMode) {
      case sdm_Off:
        return;
      case sdm_PC:
        start = STARTPCSOUNDS;
        break;
      case sdm_AdLib:
        start = STARTADLIBSOUNDS;
      }

      /*                        for (i=0;i<NUMSOUNDS;i++,start++)
                                                                      MM_FreePtr ((memptr *)&audiosegs[start]); */
      return;
    }
  }

  ClearMemory();
  ShutdownId();

  printf("%s\n", DosMessages[US_RndT() % 9]);
  exit(1);
}

#endif // SPEARDEMO
#endif // GOODTIMES
#endif // SPEAR
//===========================================================================
