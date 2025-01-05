#include <ez80.h>
// WL_DRAW.C

#include "wl_def.h"

#include "id_vh.h"

#include "keyboard.h"

/*
=============================================================================

                                                           LOCAL CONSTANTS

=============================================================================
*/

// the door is the last picture before the sprites
#define DOORWALL (PMSpriteStart - 8)

#define ACTORSIZE 0x4000

/*
=============================================================================

                                                          GLOBAL VARIABLES

=============================================================================
*/

byte *vbuf = NULL;

uint24_t lasttimecount;
int32_t  frameon;
// boolean fpscounter;

int fps_frames = 0, fps_time = 0, fps = 0;

uint16_t wallheight[MAXVIEWWIDTH];

int16_t min_wallheight;

//
// math tables
//
short  pixelangle[MAXVIEWWIDTH];
fixed  finetangent[FINEANGLES / 4];
fixed  sintable[ANGLES + ANGLES / 4];
fixed *costable = sintable + (ANGLES / 4);

//
// refresh variables
//
fixed viewx, viewy; // the focal point
short viewangle;
fixed viewsin, viewcos;

void TransformActor(objtype *ob);
void BuildTables(void);
void ClearScreen(void);
int  CalcRotate(objtype *ob);
void DrawScaleds(void);
void CalcTics(void);
void ThreeDRefresh(void);

//
// wall optimization variables
//
int   lastside; // true for vertical
short lastintercept;
int   lasttilehit;
int   lasttexture;

//
// ray tracing variables
//
short    focaltx, focalty, viewtx, viewty;
uint24_t xpartialup, xpartialdown, ypartialup, ypartialdown;

short midangle, angle;

word          tilehit;
uint16_t pixx __data_on_chip;

short xtile, ytile;
short xtilestep, ytilestep;
fixed xintercept, yintercept;
word  xspot, yspot;
int   texdelta;

word horizwall[MAXWALLTILES], vertwall[MAXWALLTILES];

/*
============================================================================

                                                   3 - D  DEFINITIONS

============================================================================
*/

/*
========================
=
= TransformActor
=
= Takes paramaters:
=   gx,gy               : globalx/globaly of point
=
= globals:
=   viewx,viewy         : point of view
=   viewcos,viewsin     : sin/cos of viewangle
=   scale               : conversion from global value to screen value
=
= sets:
=   screenx,transx,transy,screenheight: projected edge location and size
=
========================
*/

//
// transform actor
//
void TransformActor(objtype *ob) {
  fixed gx, gy, gxt, gyt, nx, ny;

  //
  // translate point to view centered coordinates
  //
  gx = ob->x - viewx;
  gy = ob->y - viewy;

  //
  // calculate newx
  //
  gxt = FixedMul(gx, viewcos);
  gyt = FixedMul(gy, viewsin);
  nx  = gxt - gyt - ACTORSIZE; // fudge the shape forward a bit, because
  // the midpoint could put parts of the shape
  // into an adjacent wall

  //
  // calculate newy
  //
  gxt = FixedMul(gx, viewsin);
  gyt = FixedMul(gy, viewcos);
  ny  = gyt + gxt;

  //
  // calculate perspective ratio
  //
  ob->transx = nx;
  ob->transy = ny;

  if (nx < MINDIST) // too close, don't overflow the divide
  {
    ob->viewheight = 0;
    return;
  }

  ob->viewx = (word)(centerx + ny * scale / nx);

  //
  // calculate height (heightnumerator/(nx>>8))
  //
  ob->viewheight = (word)(heightnumerator / sr_s32_s32_8(nx));
}

//==========================================================================

/*
========================
=
= TransformTile
=
= Takes paramaters:
=   tx,ty               : tile the object is centered in
=
= globals:
=   viewx,viewy         : point of view
=   viewcos,viewsin     : sin/cos of viewangle
=   scale               : conversion from global value to screen value
=
= sets:
=   screenx,transx,transy,screenheight: projected edge location and size
=
= Returns true if the tile is withing getting distance
=
========================
*/

boolean TransformTile(int tx, int ty, short *dispx, short *dispheight) {
  fixed gx, gy, gxt, gyt, nx, ny;

  //
  // translate point to view centered coordinates
  //
  gx = (short_to_fixed(tx)) + 0x8000 - viewx;
  gy = (short_to_fixed(ty)) + 0x8000 - viewy;

  //
  // calculate newx
  //
  gxt = FixedMul(gx, viewcos);
  gyt = FixedMul(gy, viewsin);
  nx  = gxt - gyt - 0x2000; // 0x2000 is size of object

  //
  // calculate newy
  //
  gxt = FixedMul(gx, viewsin);
  gyt = FixedMul(gy, viewcos);
  ny  = gyt + gxt;

  //
  // calculate height / perspective ratio
  //
  if (nx < MINDIST) // too close, don't overflow the divide
    *dispheight = 0;
  else {
    *dispx      = (short)(centerx + ny * scale / nx);
    *dispheight = (short)(heightnumerator / sr_s32_s32_8(nx));
  }

  //
  // see if it should be grabbed
  //
  if (nx < TILEGLOBAL && ny > -TILEGLOBAL / 2 && ny < TILEGLOBAL / 2)
    return true;
  else
    return false;
}

//==========================================================================

/*
====================
=
= CalcHeight
=
= Calculates the height of xintercept,yintercept from viewx,viewy
=
====================
*/

extern uint16_t __func_on_chip CalcHeight();

//==========================================================================

/*
===================
=
= ScalePost
=
===================
*/

byte   *postsource;
int16_t postx;

extern void __func_on_chip ScalePost();

/*
====================
=
= HitVertWall
=
= tilehit bit 7 is 0, because it's not a door tile
= if bit 6 is 1 and the adjacent tile is a door tile, use door side pic
=
====================
*/

void __func_on_chip HitVertWall(void) {
  int wallpic;
  int texture;

  texture = (TextureFromFixedShift(yintercept + texdelta)) & TEXTUREMASK;
  if (xtilestep == -1) {
    texture = TEXTUREMASK - texture;
    xintercept += TILEGLOBAL;
  }

  if (lastside == 1 && lastintercept == xtile && lasttilehit == tilehit && !(lasttilehit & 0x40)) {
    if ((pixx & 3) && texture == lasttexture) {
      ScalePost();
      postx            = pixx;
      wallheight[pixx] = wallheight[pixx - 1];
      return;
    }
    ScalePost();
    wallheight[pixx] = CalcHeight();
    postsource += texture - lasttexture;
    postx       = pixx;
    lasttexture = texture;
    return;
  }

  if (lastside != -1)
    ScalePost();

  lastside         = 1;
  lastintercept    = xtile;
  lasttilehit      = tilehit;
  lasttexture      = texture;
  wallheight[pixx] = CalcHeight();
  postx            = pixx;

  if (tilehit & 0x40) { // check for adjacent doors
    ytile = (short)(fixed_rounded_down(yintercept));
    if (tilemap[xtile - xtilestep][ytile] & 0x80)
      wallpic = DOORWALL + 3;
    else
      wallpic = vertwall[tilehit & ~0x40];
  } else
    wallpic = vertwall[tilehit];

  postsource = PM_GetTexture(wallpic) + texture;
}

/*
====================
=
= HitHorizWall
=
= tilehit bit 7 is 0, because it's not a door tile
= if bit 6 is 1 and the adjacent tile is a door tile, use door side pic
=
====================
*/

void __func_on_chip HitHorizWall(void) {
  int wallpic;
  int texture;

  texture = (TextureFromFixedShift(xintercept + texdelta)) & TEXTUREMASK;
  if (ytilestep == -1)
    yintercept += TILEGLOBAL;
  else
    texture = TEXTUREMASK - texture;

  if (lastside == 0 && lastintercept == ytile && lasttilehit == tilehit && !(lasttilehit & 0x40)) {
    if ((pixx & 3) && texture == lasttexture) {
      ScalePost();
      postx            = pixx;
      wallheight[pixx] = wallheight[pixx - 1];
      return;
    }
    ScalePost();
    wallheight[pixx] = CalcHeight();
    postsource += texture - lasttexture;
    postx       = pixx;
    lasttexture = texture;
    return;
  }

  if (lastside != -1)
    ScalePost();

  lastside         = 0;
  lastintercept    = ytile;
  lasttilehit      = tilehit;
  lasttexture      = texture;
  wallheight[pixx] = CalcHeight();
  postx            = pixx;

  if (tilehit & 0x40) { // check for adjacent doors
    xtile = (short)(fixed_rounded_down(xintercept));
    if (tilemap[xtile][ytile - ytilestep] & 0x80)
      wallpic = DOORWALL + 2;
    else
      wallpic = horizwall[tilehit & ~0x40];
  } else
    wallpic = horizwall[tilehit];

  postsource = PM_GetTexture(wallpic) + texture;
}

//==========================================================================

/*
====================
=
= HitHorizDoor
=
====================
*/

void __func_on_chip HitHorizDoor(void) {
  int doorpage;
  int doornum;
  int texture;

  doornum = tilehit & 0x7f;
  texture = (TextureFromFixedShift(xintercept - doorposition[doornum])) & TEXTUREMASK;

  if (lasttilehit == tilehit) {
    if ((pixx & 3) && texture == lasttexture) {
      ScalePost();
      postx            = pixx;
      wallheight[pixx] = wallheight[pixx - 1];
      return;
    }
    ScalePost();
    wallheight[pixx] = CalcHeight();
    postsource += texture - lasttexture;
    postx       = pixx;
    lasttexture = texture;
    return;
  }

  if (lastside != -1)
    ScalePost();

  lastside         = 2;
  lasttilehit      = tilehit;
  lasttexture      = texture;
  wallheight[pixx] = CalcHeight();
  postx            = pixx;

  switch (doorobjlist[doornum].lock) {
  case dr_normal:
    doorpage = DOORWALL;
    break;
  case dr_lock1:
  case dr_lock2:
  case dr_lock3:
  case dr_lock4:
    doorpage = DOORWALL + 6;
    break;
  case dr_elevator:
    doorpage = DOORWALL + 4;
    break;
  }

  postsource = PM_GetTexture(doorpage) + texture;
}

//==========================================================================

/*
====================
=
= HitVertDoor
=
====================
*/

void __func_on_chip HitVertDoor(void) {
  int doorpage;
  int doornum;
  int texture;

  doornum = tilehit & 0x7f;
  texture = (TextureFromFixedShift(yintercept - doorposition[doornum])) & TEXTUREMASK;

  if (lasttilehit == tilehit) {
    if ((pixx & 3) && texture == lasttexture) {
      ScalePost();
      postx            = pixx;
      wallheight[pixx] = wallheight[pixx - 1];
      return;
    }
    ScalePost();
    wallheight[pixx] = CalcHeight();
    postsource += texture - lasttexture;
    postx       = pixx;
    lasttexture = texture;
    return;
  }

  if (lastside != -1)
    ScalePost();

  lastside         = 2;
  lasttilehit      = tilehit;
  lasttexture      = texture;
  wallheight[pixx] = CalcHeight();
  postx            = pixx;

  switch (doorobjlist[doornum].lock) {
  case dr_normal:
    doorpage = DOORWALL + 1;
    break;
  case dr_lock1:
  case dr_lock2:
  case dr_lock3:
  case dr_lock4:
    doorpage = DOORWALL + 7;
    break;
  case dr_elevator:
    doorpage = DOORWALL + 5;
    break;
  }

  postsource = PM_GetTexture(doorpage) + texture;
}

//==========================================================================

#define HitHorizBorder HitHorizWall
#define HitVertBorder  HitVertWall

//==========================================================================

byte vgaCeiling[] = {
#ifndef SPEAR
    0x1d, 0x1d, 0x1d, 0x1d, 0x1d, 0x1d, 0x1d, 0x1d, 0x1d, 0xbf, 0x4e, 0x4e, 0x4e, 0x1d, 0x8d, 0x4e,
    0x1d, 0x2d, 0x1d, 0x8d, 0x1d, 0x1d, 0x1d, 0x1d, 0x1d, 0x2d, 0xdd, 0x1d, 0x1d, 0x98,

    0x1d, 0x9d, 0x2d, 0xdd, 0xdd, 0x9d, 0x2d, 0x4d, 0x1d, 0xdd, 0x7d, 0x1d, 0x2d, 0x2d, 0xdd, 0xd7,
    0x1d, 0x1d, 0x1d, 0x2d, 0x1d, 0x1d, 0x1d, 0x1d, 0xdd, 0xdd, 0x7d, 0xdd, 0xdd, 0xdd
#else
    0x6f, 0x4f, 0x1d, 0xde, 0xdf, 0x2e, 0x7f, 0x9e, 0xae, 0x7f, 0x1d,
    0xde, 0xdf, 0xde, 0xdf, 0xde, 0xe1, 0xdc, 0x2e, 0x1d, 0xdc
#endif
};

// /*
// =====================
// =
// = view_port_clear
// =
// =====================
// */

/**
 * @brief draw the ceiling and floor into the view port
 *
 * Globals:
 * @param vgaCeliling - ceiling color
 * @param gamestate.episode - episode number
 * @param mapon - map number
 * @param view_half_length - half of the view port length
 * @param vbuf - view port buffer
 */
void view_port_clear() {
  uint8_t ceiling = vgaCeiling[gamestate.episode * 10 + mapon];
  memset(vbuf, ceiling, view_half_length);

  // TODO: pre-calculate vbuf + view_half_length
  memset(vbuf + view_half_length, 0x19, view_half_length);
}

//==========================================================================

/*
=====================
=
= CalcRotate
=
=====================
*/

int CalcRotate(objtype *ob) {
  int angle, viewangle;

  // this isn't exactly correct, as it should vary by a trig value,
  // but it is close enough with only eight rotations

  viewangle = player->angle + (centerx - ob->viewx) / 8;

  if (ob->obclass == rocketobj || ob->obclass == hrocketobj)
    angle = (viewangle - 180) - ob->angle;
  else
    angle = (viewangle - 180) - dirangle[ob->dir];

  angle += ANGLES / 16;
  while (angle >= ANGLES)
    angle -= ANGLES;
  while (angle < 0)
    angle += ANGLES;

  if (ob->state->rotate == 2) // 2 rotation pain frame
    return 0;                 // pain with shooting frame bugfix

  return angle / (ANGLES / 8);
}

void ScaleShape(int xcenter, int shapenum, uint24_t height) {
  t_compshape *shape;
  uint24_t     scale;
  uint24_t     pixheight;
  uint16_t     starty;
  uint16_t     endy;
  uint16_t    *cmdptr;
  byte        *cline;
  byte        *line;
  byte        *vmem;
  int24_t      actx;
  uint16_t     i;
  int24_t      upperedge;
  int16_t      newstart;
  int24_t      scrstarty;
  int24_t      screndy;
  int24_t      lpix;
  int24_t      rpix;
  int24_t      pixcnt;
  int24_t      ycnt;
  uint16_t     j;
  uint8_t      color;

  shape = (t_compshape *)PM_GetSprite(shapenum);

  scale = sr_u24_u24_3(height); // >> 3; // low three bits are fractional
  if (!scale)
    return; // too close or far away

  pixheight = scale * SPRITESCALEFACTOR;
  actx      = xcenter - scale;
  upperedge = view_height / 2 - scale;

  cmdptr = (word *)shape->dataofs;

  for (i = shape->leftpix, pixcnt = i * pixheight, rpix = (pixcnt >> 6) + actx; i <= shape->rightpix; i++, cmdptr++) {
    lpix = rpix;

    if (lpix >= view_width)
      break;

    pixcnt += pixheight;
    rpix = (pixcnt >> 6) + actx;

    if (lpix != rpix && rpix > 0) {
      if (lpix < 0)
        lpix = 0;

      if (rpix > view_width)
        rpix = view_width, i = shape->rightpix + 1;

      cline = (byte *)shape + *cmdptr;

      while (lpix < rpix) {
        if (wallheight[lpix] <= (int16_t)height) {
          line = cline;
          while ((endy = READWORD(&line)) != 0) {
            endy >>= 1;
            newstart = READWORD(&line);
            starty   = READWORD(&line) >> 1;
            j        = starty;
            ycnt     = j * pixheight;
            screndy  = (ycnt >> 6) + upperedge;

            if (screndy < 0)
              vmem = vbuf + lpix;
            else
              vmem = vbuf + screndy * view_width + lpix;

            for (; j < endy; j++) {
              scrstarty = screndy;
              ycnt += pixheight;
              screndy = (ycnt >> 6) + upperedge;
              if (scrstarty != screndy && screndy > 0) {
                color = ((byte *)shape)[newstart + j];

                if (scrstarty < 0)
                  scrstarty = 0;

                if (screndy > view_height)
                  screndy = view_height, j = endy;

                while (scrstarty < screndy) {
                  *vmem = color;
                  vmem += view_width;
                  scrstarty++;
                }
              }
            }
          }
        }
        lpix++;
      }
    }
  }
}

void SimpleScaleShape(int xcenter, int shapenum, unsigned height) {
  t_compshape *shape;
  unsigned     scale, pixheight;
  unsigned     starty, endy;
  word        *cmdptr;
  byte        *cline;
  byte        *line;
  int          actx, i, upperedge;
  short        newstart;
  int          scrstarty, screndy, lpix, rpix, pixcnt, ycnt;
  unsigned     j;
  byte         col;
  byte        *vmem;

  shape = (t_compshape *)PM_GetSprite(shapenum);

  scale     = height >> 1;
  pixheight = scale * SPRITESCALEFACTOR;
  actx      = xcenter - scale;
  upperedge = view_height / 2 - scale;

  cmdptr = shape->dataofs;

  for (i = shape->leftpix, pixcnt = i * pixheight, rpix = (pixcnt >> 6) + actx; i <= shape->rightpix; i++, cmdptr++) {
    lpix = rpix;
    if (lpix >= view_width)
      break;
    pixcnt += pixheight;
    rpix = (pixcnt >> 6) + actx;
    if (lpix != rpix && rpix > 0) {
      if (lpix < 0)
        lpix = 0;
      if (rpix > view_width)
        rpix = view_width, i = shape->rightpix + 1;
      cline = (byte *)shape + *cmdptr;
      while (lpix < rpix) {
        line = cline;
        while ((endy = READWORD(&line)) != 0) {
          endy >>= 1;
          newstart = READWORD(&line);
          starty   = READWORD(&line) >> 1;
          j        = starty;
          ycnt     = j * pixheight;
          screndy  = (ycnt >> 6) + upperedge;
          if (screndy < 0)
            vmem = vbuf + lpix;
          else
            vmem = vbuf + screndy * view_width + lpix;
          for (; j < endy; j++) {
            scrstarty = screndy;
            ycnt += pixheight;
            screndy = (ycnt >> 6) + upperedge;
            if (scrstarty != screndy && screndy > 0) {
              col = ((byte *)shape)[newstart + j];
              if (scrstarty < 0)
                scrstarty = 0;
              if (screndy > view_height)
                screndy = view_height, j = endy;

              while (scrstarty < screndy) {
                *vmem = col;
                vmem += view_width;
                scrstarty++;
              }
            }
          }
        }
        lpix++;
      }
    }
  }
}

/*
=====================
=
= DrawScaleds
=
= Draws all objects that are visable
=
=====================
*/

#define MAXVISABLE 250

typedef struct {
  short viewx, viewheight, shapenum;
} visobj_t;

visobj_t  vislist[MAXVISABLE];
visobj_t *visptr, *visstep, *farthest;

void DrawScaleds(void) {
  int      i, least, numvisable, height;
  byte    *tilespot, *visspot;
  unsigned spotloc;

  statobj_t *statptr;
  objtype   *obj;

  visptr = &vislist[0];

  //
  // place static objects
  //
  for (statptr = &statobjlist[0]; statptr != laststatobj; statptr++) {
    if ((visptr->shapenum = statptr->shapenum) == -1)
      continue; // object has been deleted

    if (!*statptr->visspot)
      continue; // not visable

    if (TransformTile(statptr->tilex, statptr->tiley, &visptr->viewx, &visptr->viewheight) && statptr->flags & FL_BONUS) {
      GetBonus(statptr);
      if (statptr->shapenum == -1)
        continue; // object has been taken
    }

    if (!visptr->viewheight)
      continue; // to close to the object

    if (visptr < &vislist[MAXVISABLE - 1]) // don't let it overflow
      visptr++;
  }

  //
  // place active objects
  //
  for (obj = player->next; obj; obj = obj->next) {
    if ((visptr->shapenum = obj->state->shapenum) == 0)
      continue; // no shape

    spotloc  = (obj->tilex << mapshift) + obj->tiley; // optimize: keep in struct?
    visspot  = &spotvis[0][0] + spotloc;
    tilespot = &tilemap[0][0] + spotloc;

    //
    // could be in any of the nine surrounding tiles
    //
    if (*visspot || (*(visspot - 1) && !*(tilespot - 1)) || (*(visspot + 1) && !*(tilespot + 1)) ||
        (*(visspot - 65) && !*(tilespot - 65)) || (*(visspot - 64) && !*(tilespot - 64)) ||
        (*(visspot - 63) && !*(tilespot - 63)) || (*(visspot + 65) && !*(tilespot + 65)) ||
        (*(visspot + 64) && !*(tilespot + 64)) || (*(visspot + 63) && !*(tilespot + 63))) {
      obj->active = ac_yes;
      TransformActor(obj);
      if (!obj->viewheight)
        continue; // too close or far away

      visptr->viewx      = obj->viewx;
      visptr->viewheight = obj->viewheight;
      if (visptr->shapenum == -1)
        visptr->shapenum = obj->temp1; // special shape

      if (obj->state->rotate)
        visptr->shapenum += CalcRotate(obj);

      if (visptr < &vislist[MAXVISABLE - 1]) // don't let it overflow
        visptr++;

      obj->flags |= FL_VISABLE;
    } else
      obj->flags &= ~FL_VISABLE;
  }

  //
  // draw from back to front
  //

  numvisable = (int)(visptr - &vislist[0]);

  if (!numvisable)
    return; // no visable objects

  for (i = 0; i < numvisable; i++) {
    least = 32000;
    for (visstep = &vislist[0]; visstep < visptr; visstep++) {
      height = visstep->viewheight;
      if (height < least) {
        least    = height;
        farthest = visstep;
      }
    }
    //
    // draw farthest
    //
    ScaleShape(farthest->viewx, farthest->shapenum, farthest->viewheight);

    farthest->viewheight = 32000;
  }
}

//==========================================================================

/*
==============
=
= DrawPlayerWeapon
=
= Draw the player's hands
=
==============
*/

int weaponscale[NUMWEAPONS] = {SPR_KNIFEREADY, SPR_PISTOLREADY, SPR_MACHINEGUNREADY, SPR_CHAINREADY};

void DrawPlayerWeapon(void) {
  int shapenum;

#ifndef SPEAR
  if (gamestate.victoryflag) {
#ifndef APOGEE_1_0
    if (player->state == &s_deathcam && (GetTimeCount() & 32))
      SimpleScaleShape(view_width / 2, SPR_DEATHCAM, view_height + 1);
#endif
    return;
  }
#endif

  if ((int)gamestate.weapon != -1) {
    shapenum = weaponscale[gamestate.weapon] + gamestate.weaponframe;
    SimpleScaleShape(view_width / 2, shapenum, view_height + 1);
  }

  if (demorecord || demoplayback)
    SimpleScaleShape(view_width / 2, SPR_DEMO, view_height + 1);
}

//==========================================================================

/*
=====================
=
= CalcTics
=
=====================
*/

uint24_t offset = 0;
uint24_t calibrated_timer_get() {
  uint24_t t = ez80_timers_ticks_get();

  // convert to ms?
  t = t * 1000 / 50;

  return offset - t;
}

void CalcTics(void) {
  //
  // calculate tics since last refresh for adaptive timing
  //
  if (lasttimecount > calibrated_timer_get())
    lasttimecount = calibrated_timer_get(); // if the game was paused a LONG time

  uint24_t newtime = calibrated_timer_get();
  tics             = newtime - lasttimecount;

  if (tics > MAXTICS) {
    offset = (tics - MAXTICS);

    tics = MAXTICS;
  } else
    offset = 0;
}

//==========================================================================

extern short   asm_refresh_get_angl();
extern uint8_t asm_refresh_find_quarter();
short angl     __data_on_chip;
fixed xstep __data_on_chip, ystep __data_on_chip;
uint24_t xpartial __data_on_chip, ypartial __data_on_chip;

void AsmRefresh() {
  boolean playerInPushwallBackTile = tilemap[focaltx][focalty] == 64;

  for (pixx = 0; pixx < view_width; pixx++) {
    angl = asm_refresh_get_angl();

    switch (asm_refresh_find_quarter()) {
    case 0: // 0-90
      xtilestep = 1;
      ytilestep = -1;
      xstep     = finetangent[900 - 1 - angl];
      ystep     = -finetangent[angl];
      xpartial  = xpartialup;
      ypartial  = ypartialdown;
      break;
    case 1: // 90-180
      xtilestep = -1;
      ytilestep = -1;
      xstep     = -finetangent[angl - 900];
      ystep     = -finetangent[1800 - 1 - angl];
      xpartial  = xpartialdown;
      ypartial  = ypartialdown;
      break;
    case 2: // 180-270
      xtilestep = -1;
      ytilestep = 1;
      xstep     = -finetangent[2700 - 1 - angl];
      ystep     = finetangent[angl - 1800];
      xpartial  = xpartialdown;
      ypartial  = ypartialup;
      break;
    default: // 270-360
      xtilestep = 1;
      ytilestep = 1;
      xstep     = finetangent[angl - 2700];
      ystep     = finetangent[3600 - 1 - angl];
      xpartial  = xpartialup;
      ypartial  = ypartialup;
    }

    yintercept = FixedMul(ystep, xpartial) + viewy;
    xtile      = focaltx + xtilestep;
    xspot      = (word)((xtile << mapshift) + fixed_rounded_down(yintercept));
    xintercept = FixedMul(xstep, ypartial) + viewx;
    ytile      = focalty + ytilestep;
    yspot      = (word)(((fixed_rounded_down(xintercept)) << mapshift) + ytile);
    texdelta   = 0;

    // Special treatment when player is in back tile of pushwall
    if (playerInPushwallBackTile) {
      if ((pwalldir == di_east && xtilestep == 1) || (pwalldir == di_west && xtilestep == -1)) {
        fixed yintbuf = yintercept - ((ystep * (64 - pwallpos)) >> 6);
        if ((fixed_rounded_down(yintbuf)) == focalty) // ray hits pushwall back?
        {
          if (pwalldir == di_east)
            xintercept = (short_to_fixed(focaltx)) + (pwallpos << 10);
          else
            xintercept = (short_to_fixed(focaltx)) - TILEGLOBAL + ((64 - pwallpos) << 10);
          yintercept = yintbuf;
          ytile      = (short)(fixed_rounded_down(yintercept));
          tilehit    = pwalltile;
          HitVertWall();

          continue;
        }
      } else if ((pwalldir == di_south && ytilestep == 1) || (pwalldir == di_north && ytilestep == -1)) {
        fixed xintbuf = xintercept - ((xstep * (64 - pwallpos)) >> 6);
        if (fixed_rounded_down(xintbuf) == focaltx) // ray hits pushwall back?
        {
          xintercept = xintbuf;
          if (pwalldir == di_south)
            yintercept = (short_to_fixed(focalty)) + (pwallpos << 10);
          else
            yintercept = (short_to_fixed(focalty)) - TILEGLOBAL + ((64 - pwallpos) << 10);
          xtile   = (short)(fixed_rounded_down(xintercept));
          tilehit = pwalltile;
          HitHorizWall();
          continue;
        }
      }
    }

    do {
      if (ytilestep == -1 && fixed_rounded_down(yintercept) <= ytile)
        goto horizentry;
      if (ytilestep == 1 && fixed_rounded_down(yintercept) >= ytile)
        goto horizentry;
    vertentry:
      if ((uint32_t)yintercept > mapheight * 65536 - 1 || (word)xtile >= mapwidth) {
        if (xtile < 0)
          xintercept = 0, xtile = 0;
        else if (xtile >= mapwidth)
          xintercept = short_to_fixed(mapwidth), xtile = mapwidth - 1;
        else
          xtile = (short)(fixed_rounded_down(xintercept));
        if (yintercept < 0)
          yintercept = 0, ytile = 0;
        else if (yintercept >= short_to_fixed(mapheight))
          yintercept = short_to_fixed(mapheight), ytile = mapheight - 1;
        yspot   = 0xffff;
        tilehit = 0;
        HitHorizBorder();
        break;
      }
      if (xspot >= maparea)
        break;
      tilehit = ((byte *)tilemap)[xspot];
      if (tilehit) {
        if (tilehit & 0x80) {
          fixed yintbuf = yintercept + (ystep >> 1);
          if (fixed_rounded_down(yintbuf) != fixed_rounded_down(yintercept))
            goto passvert;
          if ((word)yintbuf < doorposition[tilehit & 0x7f])
            goto passvert;
          yintercept = yintbuf;
          xintercept = short_to_fixed(xtile) | 0x8000;
          ytile      = (short)(fixed_rounded_down(yintercept));
          HitVertDoor();
        } else {
          if (tilehit == 64) {
            if (pwalldir == di_west || pwalldir == di_east) {
              fixed yintbuf;
              int   pwallposnorm;
              int   pwallposinv;
              if (pwalldir == di_west) {
                pwallposnorm = 64 - pwallpos;
                pwallposinv  = pwallpos;
              } else {
                pwallposnorm = pwallpos;
                pwallposinv  = 64 - pwallpos;
              }
              if ((pwalldir == di_east && xtile == pwallx && (fixed_rounded_down(yintercept)) == pwally) ||
                  (pwalldir == di_west && !(xtile == pwallx && (fixed_rounded_down(yintercept)) == pwally))

              ) {
                yintbuf = yintercept + ((ystep * pwallposnorm) >> 6);
                if ((fixed_rounded_down(yintbuf)) != (fixed_rounded_down(yintercept)))
                  goto passvert;

                xintercept = short_to_fixed(xtile) + TILEGLOBAL - (pwallposinv << 10);
                yintercept = yintbuf;
                ytile      = (short)(fixed_rounded_down(yintercept));
                tilehit    = pwalltile;
                HitVertWall();
              } else {
                yintbuf = yintercept + ((ystep * pwallposinv) >> 6);
                if ((fixed_rounded_down(yintbuf)) != (fixed_rounded_down(yintercept)))
                  goto passvert;

                xintercept = short_to_fixed(xtile) - (pwallposinv << 10);
                yintercept = yintbuf;
                ytile      = (short)(fixed_rounded_down(yintercept));
                tilehit    = pwalltile;
                HitVertWall();
              }
            } else {
              int pwallposi = pwallpos;
              if (pwalldir == di_north)
                pwallposi = 64 - pwallpos;
              if ((pwalldir == di_south && (word)yintercept < (pwallposi << 10)) ||
                  (pwalldir == di_north && (word)yintercept > (pwallposi << 10))) {
                if ((fixed_rounded_down(yintercept)) == pwally && xtile == pwallx) {
                  if ((pwalldir == di_south && (int32_t)((word)yintercept) + ystep < (pwallposi << 10)) ||
                      (pwalldir == di_north && (int32_t)((word)yintercept) + ystep > (pwallposi << 10)))
                    goto passvert;

                  if (pwalldir == di_south)
                    yintercept = (yintercept & 0xffff0000) + (pwallposi << 10);
                  else
                    yintercept = (yintercept & 0xffff0000) - TILEGLOBAL + (pwallposi << 10);
                  xintercept = xintercept - (sr_s32_s32_6(xstep * (64 - pwallpos)));
                  xtile      = (short)(fixed_rounded_down(xintercept));
                  tilehit    = pwalltile;
                  HitHorizWall();
                } else {
                  texdelta   = -(pwallposi << 10);
                  xintercept = short_to_fixed(xtile);
                  ytile      = (short)(fixed_rounded_down(yintercept));
                  tilehit    = pwalltile;
                  HitVertWall();
                }
              } else {
                if ((fixed_rounded_down(yintercept)) == pwally && xtile == pwallx) {
                  texdelta   = -(pwallposi << 10);
                  xintercept = short_to_fixed(xtile);
                  ytile      = (short)(fixed_rounded_down(yintercept));
                  tilehit    = pwalltile;
                  HitVertWall();
                } else {
                  if ((pwalldir == di_south && (int32_t)((word)yintercept) + ystep > (pwallposi << 10)) ||
                      (pwalldir == di_north && (int32_t)((word)yintercept) + ystep < (pwallposi << 10)))
                    goto passvert;

                  if (pwalldir == di_south)
                    yintercept = (yintercept & 0xffff0000) - ((64 - pwallpos) << 10);
                  else
                    yintercept = (yintercept & 0xffff0000) + ((64 - pwallpos) << 10);
                  xintercept = xintercept - (sr_s32_s32_6(xstep * pwallpos));
                  xtile      = (short)(fixed_rounded_down(xintercept));
                  tilehit    = pwalltile;
                  HitHorizWall();
                }
              }
            }
          } else {
            xintercept = short_to_fixed(xtile);
            ytile      = (short)(fixed_rounded_down(yintercept));
            HitVertWall();
          }
        }
        break;
      }
    passvert:
      *((byte *)spotvis + xspot) = 1;
      xtile += xtilestep;
      yintercept += ystep;
      xspot = (word)(((uint32_t)xtile << mapshift) + (fixed_rounded_down(yintercept)));
    } while (1);
    continue;

    do {
      if (xtilestep == -1 && (fixed_rounded_down(xintercept)) <= xtile)
        goto vertentry;
      if (xtilestep == 1 && (fixed_rounded_down(xintercept)) >= xtile)
        goto vertentry;
    horizentry:
      if (xintercept > mapwidth * 65536 - 1 || (word)ytile >= mapheight) {
        if (ytile < 0)
          yintercept = 0, ytile = 0;
        else if (ytile >= mapheight)
          yintercept = short_to_fixed(mapheight), ytile = mapheight - 1;
        else
          ytile = (short)(fixed_rounded_down(yintercept));
        if (xintercept < 0)
          xintercept = 0, xtile = 0;
        else if (xintercept >= short_to_fixed(mapwidth))
          xintercept = short_to_fixed(mapwidth), xtile = mapwidth - 1;
        xspot   = 0xffff;
        tilehit = 0;
        HitVertBorder();
        break;
      }
      if (yspot >= maparea)
        break;
      tilehit = ((byte *)tilemap)[yspot];
      if (tilehit) {
        if (tilehit & 0x80) {
          int32_t xintbuf = xintercept + (xstep >> 1);
          if ((fixed_rounded_down(xintbuf)) != (fixed_rounded_down(xintercept)))
            goto passhoriz;
          if ((word)xintbuf < doorposition[tilehit & 0x7f])
            goto passhoriz;
          xintercept = xintbuf;
          yintercept = short_to_fixed(ytile) + 0x8000;
          xtile      = (short)(fixed_rounded_down(xintercept));
          HitHorizDoor();
        } else {
          if (tilehit == 64) {
            if (pwalldir == di_north || pwalldir == di_south) {
              fixed xintbuf;
              int   pwallposnorm;
              int   pwallposinv;
              if (pwalldir == di_north) {
                pwallposnorm = 64 - pwallpos;
                pwallposinv  = pwallpos;
              } else {
                pwallposnorm = pwallpos;
                pwallposinv  = 64 - pwallpos;
              }
              if ((pwalldir == di_south && ytile == pwally && (fixed_rounded_down(xintercept)) == pwallx) ||
                  (pwalldir == di_north && !(ytile == pwally && (fixed_rounded_down(xintercept)) == pwallx))) {
                xintbuf = xintercept + (sr_s32_s32_6(xstep * pwallposnorm));
                if ((fixed_rounded_down(xintbuf)) != (fixed_rounded_down(xintercept)))
                  goto passhoriz;

                yintercept = (short_to_fixed(ytile)) + TILEGLOBAL - (pwallposinv << 10);
                xintercept = xintbuf;
                xtile      = (short)(fixed_rounded_down(xintercept));
                tilehit    = pwalltile;
                HitHorizWall();
              } else {
                xintbuf = xintercept + (sr_s32_s32_6(xstep * pwallposinv));
                if ((fixed_rounded_down(xintbuf)) != (fixed_rounded_down(xintercept)))
                  goto passhoriz;

                yintercept = short_to_fixed(ytile) - (pwallposinv << 10);
                xintercept = xintbuf;
                xtile      = (short)(fixed_rounded_down(xintercept));
                tilehit    = pwalltile;
                HitHorizWall();
              }
            } else {
              int pwallposi = pwallpos;
              if (pwalldir == di_west)
                pwallposi = 64 - pwallpos;
              if ((pwalldir == di_east && (word)xintercept < (pwallposi << 10)) ||
                  (pwalldir == di_west && (word)xintercept > (pwallposi << 10))) {
                if ((fixed_rounded_down(xintercept)) == pwallx && ytile == pwally) {
                  if ((pwalldir == di_east && (int32_t)((word)xintercept) + xstep < (pwallposi << 10)) ||
                      (pwalldir == di_west && (int32_t)((word)xintercept) + xstep > (pwallposi << 10)))
                    goto passhoriz;

                  if (pwalldir == di_east)
                    xintercept = (xintercept & 0xffff0000) + (pwallposi << 10);
                  else
                    xintercept = (xintercept & 0xffff0000) - TILEGLOBAL + (pwallposi << 10);
                  yintercept = yintercept - (sr_s32_s32_6(ystep * (64 - pwallpos)));
                  ytile      = (short)((int32_t)fixed_rounded_down(yintercept));
                  tilehit    = pwalltile;
                  HitVertWall();
                } else {
                  texdelta   = -(pwallposi << 10);
                  yintercept = short_to_fixed(ytile);
                  xtile      = (short)(fixed_rounded_down(xintercept));
                  tilehit    = pwalltile;
                  HitHorizWall();
                }
              } else {
                if ((fixed_rounded_down(xintercept)) == pwallx && ytile == pwally) {
                  texdelta   = -(pwallposi << 10);
                  yintercept = short_to_fixed(ytile);
                  xtile      = (short)(fixed_rounded_down(xintercept));
                  tilehit    = pwalltile;
                  HitHorizWall();
                } else {
                  if ((pwalldir == di_east && (int32_t)((word)xintercept) + xstep > (pwallposi << 10)) ||
                      (pwalldir == di_west && (int32_t)((word)xintercept) + xstep < (pwallposi << 10)))
                    goto passhoriz;

                  if (pwalldir == di_east)
                    xintercept = (xintercept & 0xffff0000) - ((64 - pwallpos) << 10);
                  else
                    xintercept = (xintercept & 0xffff0000) + ((64 - pwallpos) << 10);
                  yintercept = yintercept - (sr_s32_s32_6(ystep * pwallpos));
                  ytile      = (short)(fixed_rounded_down(yintercept));
                  tilehit    = pwalltile;
                  HitVertWall();
                }
              }
            }
          } else {
            yintercept = short_to_fixed(ytile);
            xtile      = (short)(fixed_rounded_down(xintercept));
            HitHorizWall();
          }
        }
        break;
      }
    passhoriz:
      *((byte *)spotvis + yspot) = 1;
      ytile += ytilestep;
      xintercept += xstep;
      yspot = (word)(((fixed_rounded_down(xintercept)) << mapshift) + ytile);
    } while (1);
  }
}

/*
====================
=
= WallRefresh
=
====================
*/

void WallRefresh(void) {
  xpartialdown = viewx & (TILEGLOBAL - 1);
  xpartialup   = TILEGLOBAL - xpartialdown;
  ypartialdown = viewy & (TILEGLOBAL - 1);
  ypartialup   = TILEGLOBAL - ypartialdown;

  min_wallheight = view_height;
  lastside       = -1; // the first pixel is on a new wall
  AsmRefresh();

  ScalePost(); // no more optimization on last post
}

void CalcViewVariables() {
  viewangle = player->angle;
  midangle  = viewangle * (FINEANGLES / ANGLES);
  viewsin   = sintable[viewangle];
  viewcos   = costable[viewangle];
  viewx     = player->x - FixedMul(focallength, viewcos);
  viewy     = player->y + FixedMul(focallength, viewsin);

  focaltx = (short)(fixed_rounded_down(viewx));
  focalty = (short)(fixed_rounded_down(viewy));

  viewtx = (short)(fixed_rounded_down(player->x));
  viewty = (short)(fixed_rounded_down(player->y));
}

//==========================================================================

/*
========================
=
= ThreeDRefresh
=
========================
*/

int24_t last_system_tick = 0;

uint8_t view_port_buffer[SCREEN_WIDTH * SCREEN_HEIGHT];

void ThreeDRefresh(void) {
  int24_t start = ez80_timers_ticks_get();

  //
  // clear out the traced array
  //
  memset(spotvis, 0, maparea);
  spotvis[player->tilex][player->tiley] = 1; // Detect all sprites over player fix

  // vbuf = screenBuffer->xpixels + screenofs;
  vbuf = view_port_buffer;
  // chnage to a surface of the viewport

  CalcViewVariables();

  uint24_t stage1 = ez80_timers_ticks_get() - start;

  //
  // follow the walls from there to the right, drawing as we go
  //
  view_port_clear();

  uint24_t stage2 = ez80_timers_ticks_get() - start;

  WallRefresh();

  uint24_t stage3 = ez80_timers_ticks_get() - start;

  //
  // draw all the scaled images
  //

  DrawScaleds(); // draw scaled stuff

  uint24_t stage4 = ez80_timers_ticks_get() - start;

  DrawPlayerWeapon(); // draw player's hands

  if (Keyboard[KEY_TAB] && viewsize == 21 && (int)gamestate.weapon != -1)
    ShowActStatus();

  vbuf = NULL;

  uint24_t stage5 = ez80_timers_ticks_get() - start;

  //
  // show screen and time last cycle
  //

  if (fizzlein) {
    FizzleFade(screenBuffer, 0, 0, screenWidth, screenHeight, 20, false);
    fizzlein = false;

    lasttimecount = GetTimeCount(); // don't make a big tic count
  } else {
    // change to transmit viewport to screen
    //   VH_UpdateScreen();
    update_view_port();

    uint24_t stage6 = ez80_timers_ticks_get() - start;

    int24_t  x    = ez80_timers_ticks_get() * 1000 / 50;
    uint24_t diff = x - last_system_tick;

    printf(" diff: %d, t: %d, s1: %d, s2: %d, s3: %d, s4: %d, s5: %d, s6: %d    \r", diff, x, stage1 * 1000 / 50,
           stage2 * 1000 / 50, stage3 * 1000 / 50, stage4 * 1000 / 50, stage5 * 1000 / 50, stage6 * 1000 / 50);

    last_system_tick = x;

    // if (fpscounter) {
    //   fontnumber = 0;
    //   SETFONTCOLOR(7, 127);
    //   PrintX = 4;
    //   PrintY = 1;
    //   VWB_Bar(SCREEN_WIDTH_FACTOR(0), 0, SCREEN_WIDTH_FACTOR(50), 10, bordercol);
    //   US_PrintSigned(fps);
    //   US_Print(" fps");
    // }

    // SDL_BlitSurface(screenBuffer, NULL, screen, NULL);
    // SDL_Flip(screen);
  }

#ifdef APP_DEBUG
  // if (fpscounter) {
  fps_frames++;
  fps_time += tics;

  if (fps_time > 35) {
    fps_time -= 35;
    fps        = fps_frames << 1;
    fps_frames = 0;
  }
  // }
#endif
}
