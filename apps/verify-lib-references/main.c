#include "main.h"
#include <math.h>
#include <stdbool.h>
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

extern void mem_alloc_dump(void);

#ifdef __clang__
void test_calloc() {
  printf("HEAP IS FROM %p. SPL: %p, SPS: %p \r\n\r\n", _heap, _get_spl(), _get_sps());
  printf("Initializing int array of size 5 using calloc() : ");

#define FIRST_SIZE  5
#define SECOND_SIZE 7

  int *arr = (int *)calloc(sizeof(int), FIRST_SIZE);
  printf("%p ->  \r\n", arr);

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

  void *block2 = malloc(1024);
  printf("\r\nAllocating 1024 bytes using malloc(): %p\r\n", block2);

  void *block3 = malloc(16);
  printf("Allocating 16 bytes using malloc(): %p\r\n", block3);

  printf("Freeing the 1024 bytes block\r\n");
  free(block2);

  block2 = malloc(1024);
  printf("Allocating 1024 bytes using malloc(): %p\r\n", block2);

  block2 = malloc(4024);
  printf("Allocating 4024 bytes using malloc(): %p\r\n", block2);

  printf("\r\n");

#ifdef EXE
  printf("Allocating 512KB of memory using malloc(): ");
  const void *p = malloc(512 * 1024);
  printf("%p\r\n", p);
#endif

  mem_alloc_dump();
}

uint8_t dividend = 0x80;
uint8_t divisor  = 0x10;

void test_unsigned_8_bit_div() {
  uint8_t result = dividend / divisor;

  if (result == 8)
    printf("8_bit_div: OK\r\n");
  else
    printf("8_bit_div: FAIL\r\n");
}

void test_unsigned_8_bit_mul() {
  uint8_t result = dividend * divisor;

  if (result == 8)
    printf("8_bit_mul: OK\r\n");
  else
    printf("8_bit_mul: FAIL\r\n");
}

int8_t sdividend = -0x80;
int8_t sdivisor  = 0x10;

void test_signed_8_bit_div() {
  int8_t result = sdividend / sdivisor;

  if (result == -8)
    printf("signed_8_bit_div: OK\r\n");
  else
    printf("signed_8_bit_div: FAIL\r\n");
}

void test_unsigned_8_bit_rem() {
  uint8_t result = dividend % divisor;

  if (result == 0)
    printf("8_bit_rem: OK\r\n");
  else
    printf("8_bit_rem: FAIL\r\n");
}

float  a = 1.0;
double b = 2.0;
double c = 3.0;
void   test_add_float_and_doubles() {
  if (a + b + c == 6.0)
    printf("add_float_and_doubles: OK\r\n");
  else
    printf("add_float_and_doubles: FAIL\r\n");
}

float fa = 10.0;
float fb = 3.0;
void  test_float_rem() {
  float c = fmod(fa, fb);
  if (c >= 0.9 && c <= 1.1)
    printf("float_rem: OK %d\r\n", (int)(c * 1000.0));
  else
    printf("float_rem: FAIL %d\r\n", (int)(c * 1000.0));
}

void test_type_sizes() {
  printf("sizeof(uint8_t)  = %d\r\n", sizeof(uint8_t));
  printf("sizeof(uint16_t) = %d\r\n", sizeof(uint16_t));
  printf("sizeof(uint24_t) = %d\r\n", sizeof(uint24_t));
  printf("sizeof(uint32_t) = %d\r\n", sizeof(uint32_t));
  // printf("sizeof(uint48_t) = %d\r\n", sizeof(uint48_t));
  printf("sizeof(uint64_t) = %d\r\n", sizeof(uint64_t));
  printf("sizeof(float)    = %d\r\n", sizeof(float));
  printf("sizeof(double)   = %d\r\n", sizeof(double));
  printf("sizeof(long)     = %d\r\n", sizeof(long));
  printf("sizeof(long long)= %d\r\n", sizeof(long long));
  printf("sizeof(void*)    = %d\r\n", sizeof(void *));
}

unsigned long long lla  = 0x100;
uint8_t            llas = 1;

void test_llshru() {

  unsigned long long b = lla >> llas;

  if (b == 0x80)
    printf("llshru: OK\r\n");
  else
    printf("llshru: FAIL\r\n");
}

double test_long_long_to_float() {
  double b = (double)(long long)lla;

  if (b == 256.0)
    printf("long_long_to_float: OK\r\n");
  else
    printf("long_long_to_float: FAIL\r\n");

  return b;
}

int16_t a16 = -0x100;

void test_mul_16() {
  int16_t b = a16 * 3;

  if (b == -0x300)
    printf("mul_16: OK\r\n");
  else
    printf("mul_16: FAIL\r\n");
}

void test_neg_16(bool invert) {
  int16_t b = invert ? -a16 : a16;
  if (b == 0x100)
    printf("neg_16: OK\r\n");
  else
    printf("neg_16: FAIL\r\n");
}

int inta = -12;

void test_libc() {
  // test abs
  if (abs(inta) == 12)
    printf("abs: OK\r\n");
  else
    printf("abs: FAIL\r\n");

  // test acos
  float r  = acos(b / 4.0);
  int   rr = (int)(r * 1000);
  if (rr == 1047)
    printf("acos: OK\r\n");
  else
    printf("acos: FAIL: %d\r\n", rr);

  // test div
  div_t d = div(inta, 3);
  if (d.quot == -4 && d.rem == 0)
    printf("div: OK\r\n");
  else
    printf("div: FAIL\r\n");
}
#endif

#ifdef __clang__
double xx = 2.0;

void test_ldexpf(float a, int b) {
  float x = ldexpf(a, b);
  if (x >= 3.9 && x <= 4.1)
    printf("ldexpf: OK %d\r\n", (int)(x * 1000.0));
  else
    printf("ldexpf: FAIL %d\r\n", (int)(x * 1000.0));
}

void test_expf(void) {
  float x = expf(xx);
  if (x >= 7.3 && x <= 7.4) // 7.389
    printf("expf: OK %d\r\n", (int)(x * 1000.0));
  else
    printf("expf: FAIL %d\r\n", (int)(x * 1000.0));
}

void test_pow(float base, float expnt, float low, float high) {
  float x = pow(base, expnt);
  if (x >= low && x <= high)
    printf("pow: OK %d\r\n", (int)(x * 1000.0));
  else
    printf("pow: FAIL %d\r\n", (int)(x * 1000.0));
}

void test_exp2f(void) {
  float x = exp2f(xx);
  if (x >= 3.9 && x <= 4.1)
    printf("exp2f: OK %d\r\n", (int)(x * 1000.0));
  else
    printf("exp2f: FAIL %d\r\n", (int)(x * 1000.0));
}

void test_tan(float a, float low, float high) {
  float x = tan(a);
  if (x >= low && x <= high)
    printf("tan: OK %d\r\n", (int)(x * 1000.0));
  else
    printf("tan: FAIL %d\r\n", (int)(x * 1000.0));
}

const char *world = "World";

void formatString(char *buffer, const char *format, ...) {
  va_list args;
  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);
}

void test_vsprintf(void) {
  char buffer[100];
  memset(buffer, 0, 100);

  formatString(buffer, "Hello, %s!", world);

  buffer[99] = 0;
  if (strcmp(buffer, "Hello, World!") == 0)
    printf("vsprintf: OK\r\n");
  else
    printf("vsprintf: FAIL (%s)\r\n", buffer);
}

#endif

void report_exit(void) { printf("Exiting...\r\n"); }

int main(int argc, char *argv[]) {
  printf("argc: %d\r\n", argc);
  for (int i = 0; i < argc; i++) {
    printf("argv[%d]: %s\r\n", i, argv[i]);
  }
  atexit(report_exit);

#ifdef __clang__
#ifdef EXE
  malloc_init(0x400000 - (int)_heap); // allocate all
#else
  malloc_init(4096); // declare 4096 bytes of heap memory
#endif
#endif

  test_getopt(argc, argv);
  test_memcpy();
  test___inot(4112);
  test_strcmp();
  test_strcpy();
  test_strchr();

#ifdef __clang__
  test_vsprintf();

  test_calloc();

  test_unsigned_8_bit_div();
  test_signed_8_bit_div();
  test_unsigned_8_bit_mul();

  test_unsigned_8_bit_rem();

  test_add_float_and_doubles();

  test_float_rem();

  test_type_sizes();

  test_llshru();

  test_long_long_to_float();

  test_mul_16();

  test_neg_16(true);

  test_libc();

  test_pow(2.0, 2.0, 3.9, 4.1);
  test_pow(2.0, 3.0, 7.9, 8.1);
  test_pow(2.2, 3.3, 13.4, 13.6);
  test_pow(-2.2, 4.4, 0.0, 0.1);
  test_pow(-2.2, -3.3, 0.0, 0.0);

  test_ldexpf(1.0, 2);

  test_expf();

  test_exp2f();

  test_tan(0.50, 0.42, 0.43);

  abort();
#endif

  return 0;
}
