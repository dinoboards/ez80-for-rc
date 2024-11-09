/*****************************************************************************
 * getopt test program.
 *
 * Copyright (c)2002-2003 Mark K. Kim
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *
 *   * Neither the original author of this software nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*****************************************************************************
 * DEFINES
 */

/**
 * flags for different command-line options
 *
 * these options don't do anything - there's just here
 * as examples
 */
#define FLAG_INTERACT  0x0001 /* interactive mode */
#define FLAG_FORCE     0x0002 /* force mode */
#define FLAG_RECURSIVE 0x0004 /* recursive mode */

/*****************************************************************************
 * GLOBALS
 */

int flags = 0; /* store flags here */

int         verbose   = 5;    /* verbosity level */
const char *in_fname  = NULL; /* input filename */
const char *out_fname = NULL; /* output filename */

/*****************************************************************************
 * arg_to_int - Convert argument string to integer.
 *
 * min - Minimum allowed value, inclusive.
 * max - Maximum allowed value, inclusive.
 * defalt - The default value, in case of an error.
 * opt - Option string of this argument.  (ex., "-h");
 */

int arg_to_int(const char *arg, int min, int max, int defalt, const char *opt) {
  int i = defalt;
  int rv;

  /* no argument means we use the default value */
  if (!arg)
    goto done;

  /* make sure we got an integer argument */
  rv = sscanf((char *)arg, (char *)"%d", &i);
  if (rv != 1) {
    fprintf(stderr, "%s: integer argument required.\r\n", opt);
    i = defalt;
    goto done;
  }

  /* make sure the integer argument is within the desired range */
  if (i < min || max < i) {
    fprintf(stderr, "%s: argument out of integer range.\r\n", opt);
    i = defalt;
    goto done;
  }

done:
  return i;
}

/*****************************************************************************
 * help
 */

void help(void) {
  printf("getopt test program\r\n"
         "Usage: test [OPTION] [INPUT]\r\n"
         "   INPUT           set input filename (doesn't do anything)\r\n"
         "   -H              help menu (this screen)\r\n"
         "   -I              interactive mode (doesn't do anything)\r\n"
         "   -F              force mode (doesn't do anything)\r\n"
         "   -R              recursive mode (doesn't do anything)\r\n"
         "   -V[level]       set verbosity level (5 is default; doesn't do anything)\r\n"
         "   -O filename     set output filename (doesn't do anything)\r\n");
}

/*****************************************************************************
 * MAIN
 */

int test_getopt(int argc, char *argv[]) {
  /* check arguments */
  while (1) {
    int c = getopt(argc, argv, "-IFRHV::O:");
    if (c == -1)
      break;

    switch (c) {
    case 'I':
      flags |= FLAG_INTERACT;
      break;
    case 'F':
      flags |= FLAG_FORCE;
      break;
    case 'R':
      flags |= FLAG_RECURSIVE;
      break;

    case 'H':
      help();
      exit(0);

    case 'V':
      verbose = arg_to_int(optarg, 0, 10, 5, "v");
      break;
    case 'O':
      out_fname = optarg;
      break;
    case 1:
      in_fname = optarg;
      break;

#ifdef DEBUG
    default:
      printf("Option '%c' (%d) with '%s'\r\n", c, c, optarg);
#endif
    }
  }

#ifdef DEBUG
  printf("optind at %d; argv[optind] = '%s'\r\n", optind, argv[optind]);
#endif

  /* print out what we got */
  if (flags & FLAG_INTERACT)
    printf("in interactive mode\r\n");
  else
    printf("not in interactive mode\r\n");
  if (flags & FLAG_FORCE)
    printf("in force mode\r\n");
  else
    printf("not in force mode\r\n");
  if (flags & FLAG_RECURSIVE)
    printf("in recursive mode\r\n");
  else
    printf("not in recursive mode\r\n");
  printf("verbosity level: %d\r\n", verbose);
  if (in_fname)
    printf("input filename: %s\r\n", in_fname);
  else
    printf("no input filename\r\n");
  if (out_fname)
    printf("output filename: %s\r\n", out_fname);
  else
    printf("no output filename\r\n");

  return 0;
}

