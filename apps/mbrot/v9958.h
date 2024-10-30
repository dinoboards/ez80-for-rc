#ifndef __V9958
#define __V9958

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define PAL  1
#define NTSC 2

typedef struct {
  uint8_t red;
  uint8_t blue;
  uint8_t green;
} RGB;

extern void outCmd(uint8_t b);
extern void outDat(uint8_t b);

extern void clearAllMemory(void);
extern void setPalette(RGB *);
extern void setMode6(uint8_t lines, uint8_t mode);
extern void setMode7(uint8_t lines, uint8_t mode);
extern void clearScreenBank0(uint8_t color);
extern void clearScreenBank1(uint8_t color);
extern void _writeRegister(uint16_t rd);
extern void outPal(uint8_t b);
extern void outRegIndInt(uint16_t b);
extern void outRegIndByte(uint8_t b);

#define writeRegister(a, b) _writeRegister(a * 256 + b)

#define REGISTER_COUNT 12

#define CMD_VDP_TO_VRAM 0xC0
#define CMD_LINE(op)    (0x70 | op)
#define CMD_PSET(op)    (0x50 | op)
#define CMD_LOGIC_IMP   0x00
#define CMD_LOGIC_AND   0x01

void commandDrawLine(void);
void waitForCommandCompletion(void);

extern uint8_t  dir;
extern uint16_t longSide;
extern uint16_t shortSide;
extern uint16_t _fromX;
extern uint16_t _fromY;
extern uint8_t  _color;
extern uint8_t  _operation;
extern uint16_t _toX;
extern uint16_t _toY;

#define drawLine(fromX, fromY, toX, toY, color, operation)                                                                         \
  _fromX     = (fromX);                                                                                                            \
  _fromY     = (fromY);                                                                                                            \
  _color     = (color);                                                                                                            \
  _operation = CMD_LINE((operation));                                                                                              \
  _toX       = (toX);                                                                                                              \
  _toY       = (toY);                                                                                                              \
  _drawLine()

extern void _drawLine(void);

#define pointSet(x, y, color, operation)                                                                                           \
  _fromX     = (x);                                                                                                                \
  _fromY     = (y);                                                                                                                \
  _color     = (color);                                                                                                            \
  _operation = CMD_PSET((operation));                                                                                              \
  commandDrawLine()

#endif