#include "main.h"
#include <cpm.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __clang__
#include <ez80.h>
#endif

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

volatile int size = 14;

void NO_INLINE test_memcpy(void) {
  char src[] = "Hello, World!";
  char dst[14];
  memcpy(dst, src, size);
  if (strcmp(dst, src) == 0) {
    printf("memcpy: OK\r\n");
  } else {
    printf("memcpy: FAIL\r\n");
  }
}

#ifdef __clang__
void test_calloc() {
  printf("HEAP IS FROM %p. SPL: %p, SPS: %p \r\n\r\n", _heap, _get_spl(), _get_sps());
  printf("Initializing int array of size 5 using calloc() : ");

#define FIRST_SIZE  5
#define SECOND_SIZE 7

  int *arr = (int *)calloc(sizeof(int), FIRST_SIZE);
  printf("%p ->  ", arr);

  for (int i = 0; i < FIRST_SIZE; i++)
    arr[i] = i;

  for (int i = 0; i < FIRST_SIZE; i++)
    printf("%d, ", arr[i]);

  printf("\r\nResizing it to size 10 using realloc(): ");

  arr = (int *)realloc(arr, SECOND_SIZE * sizeof(int));
  printf("%p ->  ", arr);

  for (int i = 0; i < SECOND_SIZE; i++) {
    printf("%d, ", arr[i]);
  }

  printf("\r\n");
}
#endif

int main(int argc, char *argv[]) {
#ifdef __clang__
  malloc_init(1024); // declare heap from end of bss upto stack pointer minus buffer
#endif

  test_getopt(argc, argv);
  test_memcpy();
  test___inot(4112);
  test_strcmp();
  test_strcpy();
  test_strchr();

#ifdef __clang__
  test_cpmfcb_address();
  test_calloc();
  cpm_term();
  printf("Should never get here\r\n");
#endif

  return 0;
}
