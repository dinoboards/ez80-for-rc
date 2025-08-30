#ifndef __STDIO_H
#define __STDIO_H

#include <stdarg.h>

extern int vsprintf(char *buffer, const char *fmt, va_list argptr);
extern int printf(const char *fmt, ...);

#endif
