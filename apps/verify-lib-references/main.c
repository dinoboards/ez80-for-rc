#include "main.h"
#include <cpm.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// uint16_t NO_INLINE spike(uint16_t a, uint16_t b) { return ((a << 1) - b); }

void test_strcmp(void) {
  char str1[] = "Hello, World!";
  char str2[] = "Hello, World!";
  if (strcmp(str1, str2) == 0) {
    printf("strcmp: OK\r\n");
  } else {
    printf("strcmp: FAIL\r\n");
  }
}

void test_strcpy(void) {
  char src[] = "Hello, World!";
  char dst[14];
  strcpy(dst, src);
  if (strcmp(dst, src) == 0) {
    printf("strcpy: OK\r\n");
  } else {
    printf("strcpy: FAIL\r\n");
  }
}

void test_strchr(void) {
  char  str[] = "Hello, World!";
  char *ptr   = strchr(str, 'W');
  if (ptr != NULL && strcmp(ptr, "World!") == 0) {
    printf("strchr: OK\r\n");
  } else {
    printf("strchr: FAIL\r\n");
  }
}

extern int test_getopt(int argc, char *argv[]);

int NO_INLINE  conduct__inot(int a) { return ~a; }
void NO_INLINE test___inot(int a) {
  if (conduct__inot(a) == -4113)
    printf("__inot: OK\r\n");
  else
    printf("__inot: FAIL\r\n");
}

#ifdef __clang__
void test_cpmfcb_address(void) {
  if ((uint24_t)CPM_FCB == 0x03005C)
    printf("cpmfcb_address: OK\r\n");
  else {
    printf("cpmfcb_address: FAIL\r\n");
  }

  CPM_FCB[0] = 'A';
  CPM_FCB[1] = 0;

  if (CPM_FCB[0] == 'A' && CPM_FCB[1] == 0)
    printf("cpmfcb_address: OK\r\n");
  else {
    printf("cpmfcb_address: FAIL\r\n");
  }
}
#endif

int main(int argc, char *argv[]) {
  test_getopt(argc, argv);

  test___inot(4112);
  test_strcmp();
  test_strcpy();
  test_strchr();

#ifdef __clang__
  test_cpmfcb_address();
#endif
  return 0;
}
