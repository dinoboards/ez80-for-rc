#include "v9958.h"
#include <stdbool.h>
#include <stdlib.h>

#include <stdio.h>

#define DI __asm__("DI")
#define EI __asm__("EI")

void setBaseRegisters(uint8_t *pReg) {
  DI;

  for (uint8_t i = 0; i < REGISTER_COUNT; i++) {
    writeRegister(i, *pReg); // if we inline the increment, the compiler (with -Oz seems to pre-increment the pointer)
    pReg++;
  }

  EI;
}

void setVideoSignal(uint8_t *pReg, uint8_t lines, uint8_t mode) {
  if (lines == 212)
    pReg[9] |= 0x80;

  if (mode == PAL)
    pReg[9] |= 0x02;
}

uint8_t mode6Reg[REGISTER_COUNT] = {
    0x0A, // R0 - M5 = 1, M4 = 0, M3 = 1
    0x40, // R1 - ENABLE SCREEN, DISABLE INTERRUPTS, M1 = 0, M2 = 0
    0x1F, // R2 - PATTERN NAME TABLE := 0, A16 = 0
    0x00, // R3 - NO COLOR TABLE
    0x00, // R4 - N/A???
    0xF7, // R5 - SPRITE ATTRIBUTE TABLE -> FA00
    0x1E, // R6 - SPRITE PATTERN => F000
    0x00, // R7 - a background colour?
    0x8A, // R8 - COLOUR BUS INPUT, DRAM 64K, DISABLE SPRITE
    0x00, // R9 - LN = 1(212 lines), S1, S0 = 0, IL = 0, EO = 0, NT = 1 (PAL),
          // DC = 0
    0x00, // R10 - color table - n/a
    0x01  // R11 - SPRITE ATTRIBUTE TABLE -> FA00
};

void setMode6(uint8_t lines, uint8_t mode) {
  setVideoSignal(mode6Reg, lines, mode);
  setBaseRegisters(mode6Reg);
}

uint8_t mode7Reg[REGISTER_COUNT] = {
    0x0E, // R0 - M5 = 1, M4 = 1, M3 = 1
    0x40, // R1 - ENABLE SCREEN, DISABLE INTERRUPTS, M1 = 0, M2 = 0
    0x1F, // R2 - PATTERN NAME TABLE := 0, A16 = 0
    0x00, // R3 - NO COLOR TABLE
    0x00, // R4 - N/A???
    0xF7, // R5 - SPRITE ATTRIBUTE TABLE -> FA00
    0x1E, // R6 - SPRITE PATTERN => F000
    0x00, // R7 - a background colour?
    0x8A, // R8 - COLOUR BUS INPUT, DRAM 64K, DISABLE SPRITE
    0x00, // R9 LN = 1(212 lines), S1, S0 = 0, IL = 0, EO = 0, NT = 1 (PAL), DC
          // = 0
    0x00, // R10 - color table - n/a
    0x01  // R11 - SPRITE ATTRIBUTE TABLE -> FA00
};

void setMode7(uint8_t lines, uint8_t mode) {
  setVideoSignal(mode7Reg, lines, mode);
  setBaseRegisters(mode7Reg);
}

void setPalette(RGB *pPalette) {
  DI;
  writeRegister(16, 0);
  for (uint8_t c = 0; c < 16; c++) {
    outPal(pPalette->red * 16 + pPalette->blue);
    outPal(pPalette->green);
    pPalette++;
  }
  EI;
}

void clearAllMemory(void) {
  DI;
  writeRegister(14, 0);
  outCmd(0);
  outCmd(0x40);
  for (int i = 0; i < 0x10000; i++)
    outDat(0x41);
  for (int i = 0x10000; i < 0x20000; i++)
    outDat(0x00);
  EI;
}

extern void delay(void);

void clearScreenBank0(uint8_t color) {
  DI;
  // Clear bitmap data from 0x0000 to 0x3FFF

  writeRegister(17, 36);             // Set Indirect register Access
  outRegIndInt(0);                   // DX
  outRegIndInt(0);                   // DY
  outRegIndInt(512);                 // NX
  outRegIndInt(212);                 // NY
  outRegIndByte(color * 16 + color); // COLOUR for both pixels (assuming G7 mode)
  outRegIndByte(0);                  // Direction: VRAM, Right, Down
  outRegIndByte(CMD_VDP_TO_VRAM);
  EI;

  waitForCommandCompletion();
}

void clearScreenBank1(uint8_t color) {
  DI;
  // Clear bitmap data from 0x0000 to 0x3FFF

  writeRegister(17, 36);             // Set Indirect register Access
  outRegIndInt(512);                 // DX
  outRegIndInt(212);                 // DY
  outRegIndInt(512);                 // NX
  outRegIndInt(212);                 // NY
  outRegIndByte(color * 16 + color); // COLOUR for both pixels (assuming G7 mode)
  outRegIndByte(0x0);                // Direction: ExpVRAM, Right, Down
  outRegIndByte(CMD_VDP_TO_VRAM);
  EI;

  waitForCommandCompletion();
}

static uint16_t t;
static uint16_t y;
static uint16_t x;
uint16_t        _toX;
uint16_t        _toY;

void _drawLine(void) {
  if (_fromY > _toY) {
    t      = _fromY;
    _fromY = _toY;
    _toY   = t;
  };
  if (_fromX > _toX) {
    t      = _fromX;
    _fromX = _toX;
    _toX   = t;
  };
  y   = _toY - _fromY;
  x   = _toX - _fromX;
  dir = (y > x);

  if (y > x) {
    longSide  = y;
    shortSide = x;
  } else {
    longSide  = x;
    shortSide = y;
  }

  commandDrawLine();
}
