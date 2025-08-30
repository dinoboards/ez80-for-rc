#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define ALT       0x00000001 /* alternate form */
#define HEXPREFIX 0x00000002 /* add 0x or 0X prefix */
#define LADJUST   0x00000004 /* left adjustment */
#define LONGDBL   0x00000008 /* long double */
#define LONGINT   0x00000010 /* long integer */
#define QUADINT   0x00000020 /* quad integer */
#define SHORTINT  0x00000040 /* short integer */
#define ZEROPAD   0x00000080 /* zero (as opposed to blank) pad */
#define FPT       0x00000100 /* floating point number */

#define to_digit(c) ((c) - '0')
#define is_digit(c) ((unsigned)to_digit(c) <= 9)
#define to_char(n)  ((n) + '0')

void AddInt(char **buf_p, int val, int width, int flags) {
  char  text[32];
  int   digits;
  int   signedVal;
  char *buf;

  digits    = 0;
  signedVal = val;
  if (val < 0) {
    val = -val;
  }
  do {
    text[digits++] = '0' + val % 10;
    val /= 10;
  } while (val);

  if (signedVal < 0) {
    text[digits++] = '-';
  }

  buf = *buf_p;

  if (!(flags & LADJUST)) {
    while (digits < width) {
      *buf++ = (flags & ZEROPAD) ? '0' : ' ';
      width--;
    }
  }

  while (digits--) {
    *buf++ = text[digits];
    width--;
  }

  if (flags & LADJUST) {
    while (width--) {
      *buf++ = (flags & ZEROPAD) ? '0' : ' ';
    }
  }

  *buf_p = buf;
}

void AddFloat(char **buf_p, float fval, int width, int prec) {
  char  text[32];
  int   digits;
  float signedVal;
  char *buf;
  int   val;

  // get the sign
  signedVal = fval;
  if (fval < 0) {
    fval = -fval;
  }

  // write the float number
  digits = 0;
  val    = (int)fval;
  do {
    text[digits++] = '0' + val % 10;
    val /= 10;
  } while (val);

  if (signedVal < 0) {
    text[digits++] = '-';
  }

  buf = *buf_p;

  while (digits < width) {
    *buf++ = ' ';
    width--;
  }

  while (digits--) {
    *buf++ = text[digits];
  }

  *buf_p = buf;

  if (prec < 0)
    prec = 6;
  // write the fraction
  digits = 0;
  while (digits < prec) {
    fval -= (int)fval;
    fval *= 10.0;
    val            = (int)fval;
    text[digits++] = '0' + val % 10;
  }

  if (digits > 0) {
    buf    = *buf_p;
    *buf++ = '.';
    for (prec = 0; prec < digits; prec++) {
      *buf++ = text[prec];
    }
    *buf_p = buf;
  }
}

void AddString(char **buf_p, char *string, int width, int prec) {
  int   size;
  char *buf;

  buf = *buf_p;

  if (string == NULL) {
    string = "(null)";
    prec   = -1;
  }

  if (prec >= 0) {
    for (size = 0; size < prec; size++) {
      if (string[size] == '\0') {
        break;
      }
    }
  } else {
    size = strlen(string);
  }

  width -= size;

  while (size--) {
    *buf++ = *string++;
  }

  while (width-- > 0) {
    *buf++ = ' ';
  }

  *buf_p = buf;
}

/*
vsprintf

I'm not going to support a bunch of the more arcane stuff in here
just to keep it simpler.  For example, the '*' and '$' are not
currently supported.  I've tried to make it so that it will just
parse and ignore formats we don't support.
*/
int vsprintf(char *buffer, const char *fmt, va_list argptr) {
  int  *arg;
  char *buf_p;
  char  ch;
  int   flags;
  int   width;
  int   prec;
  int   n;
  char  sign;

  buf_p = buffer;
  arg   = (int *)argptr;

  while (1) {
    // run through the format string until we hit a '%' or '\0'
    for (ch = *fmt; (ch = *fmt) != '\0' && ch != '%'; fmt++) {
      *buf_p++ = ch;
    }
    if (ch == '\0') {
      goto done;
    }

    // skip over the '%'
    fmt++;

    // reset formatting state
    flags = 0;
    width = 0;
    prec  = -1;
    sign  = '\0';

  rflag:
    ch = *fmt++;
  reswitch:

    switch (ch) {
    case '-':
      flags |= LADJUST;
      goto rflag;
    case '.':
      n = 0;
      while (is_digit((ch = *fmt++))) {
        n = 10 * n + (ch - '0');
      }
      prec = n < 0 ? -1 : n;
      goto reswitch;
    case '0':
      flags |= ZEROPAD;
      goto rflag;
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      n = 0;
      do {
        n  = 10 * n + (ch - '0');
        ch = *fmt++;
      } while (is_digit(ch));
      width = n;
      goto reswitch;
    case 'c':
      *buf_p++ = (char)*arg;
      arg++;
      break;
    case 'd':
    case 'i':
      AddInt(&buf_p, *arg, width, flags);
      arg++;
      break;
    case 'f':
      AddFloat(&buf_p, *(double *)arg, width, prec);
#ifdef __LCC__
      arg += 1; // everything is 32 bit in my compiler
#else
      arg += 2;
#endif
      break;
    case 's':
      AddString(&buf_p, (char *)*arg, width, prec);
      arg++;
      break;
    case '%':
      *buf_p++ = ch;
      break;
    default:
      *buf_p++ = (char)*arg;
      arg++;
      break;
    }
  }

done:
  *buf_p = 0;
  return buf_p - buffer;
}
