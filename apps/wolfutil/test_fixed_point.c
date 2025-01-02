#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef int32_t fixed;

#define GLOBAL1 (1l << 16)

extern void spike(int32_t a);

extern fixed FixedMul(fixed a, fixed b);

fixed FixedMulORG(fixed a, fixed b) { return (a >> 8) * (b >> 8); }

extern int16_t fixed_rounded_down(fixed a);

// For player movement in demos exactly as in the original Wolf3D v1.4 source code
fixed FixedByFracOrig(fixed a, fixed b) {
  int sign = 0;
  if (b == 65536)
    b = 65535;
  else if (b == -65536)
    b = 65535, sign = 1;
  else if (b < 0)
    b = (-b), sign = 1;

  if (a < 0) {
    a    = -a;
    sign = !sign;
  }
  fixed res = (fixed)(((int64_t)a * b) >> 16);

  if (sign)
    res = -res;
  return res;
}

// For player movement in demos exactly as in the original Wolf3D v1.4 source code
void test_fixed_mul() {
  fixed a, b, c;
  short d;

  // Test 1: 1 * 1 = 1
  a = 1 * GLOBAL1;
  b = 1 * GLOBAL1;
  c = FixedMul(a, b);
  if (c != 1 * GLOBAL1) {
    printf("FixedMul(%ld, %ld) = %ld\n", a, b, c);
    abort();
  }

  d = fixed_rounded_down(a);
  if (d != 1) {
    printf("FixedRoundedDown(%ld) = %d\n", a, d);
    abort();
  }

  // Test 2: 2 * 2 = 4
  a = 2 * GLOBAL1;
  b = 2 * GLOBAL1;
  c = FixedMul(a, b);
  if (c != 4 * GLOBAL1) {
    printf("2x2: FixedMul(%ld, %ld) = %ld\n", a, b, c);
    abort();
  }

  d = fixed_rounded_down(a);
  if (d != 2) {
    printf("FixedRoundedDown(%ld) = %d\n", a, d);
    abort();
  }

  // Test 3: -1 * 1 = -1
  a = -1 * GLOBAL1;
  b = 1 * GLOBAL1;
  c = FixedMul(a, b);
  if (c != -1 * GLOBAL1) {
    printf("-1*1: FixedMul(%ld, %ld) = %ld\n", a, b, c);
    abort();
  }

  d = fixed_rounded_down(a);
  if (d != -1) {
    printf("FixedRoundedDown(%ld) = %d\n", a, d);
    abort();
  }

  // Test 4: 1.5 * 2 = 3
  a = (fixed)(1.5 * GLOBAL1);
  b = 2 * GLOBAL1;
  c = FixedMul(a, b);
  if (c != 3 * GLOBAL1) {
    printf("1.5*2: FixedMul(%ld, %ld) = %ld\n", a, b, c);
    abort();
  }

  d = fixed_rounded_down(a);
  if (d != 1) {
    printf("FixedRoundedDown(%ld) = %d\n", a, d);
    abort();
  }
  // Test 5: -2 * -2 = 4
  a = -2 * GLOBAL1;
  b = -2 * GLOBAL1;
  c = FixedMul(a, b);
  if (c != 4 * GLOBAL1) {
    printf("-2*-2: FixedMul(%ld, %ld) = %ld\n", a, b, c);
    abort();
  }

  d = fixed_rounded_down(a);
  if (d != -2) {
    printf("FixedRoundedDown(%ld) = %d\n", a, d);
    abort();
  }

  // Test 6: 0.5 * 0.5 = 0.25
  a = (fixed)(0.5 * GLOBAL1);
  b = (fixed)(0.5 * GLOBAL1);
  c = FixedMul(a, b);
  if (c != (fixed)(0.25 * GLOBAL1)) {
    printf("0.5*0.5: FixedMul(%ld, %ld) = %ld\n", a, b, c);
    abort();
  }

  d = fixed_rounded_down(a);
  if (d != 0) {
    printf("FixedRoundedDown(%ld) = %d\n", a, d);
    abort();
  }

  // Test 6: 60000.45 * 4
  a = (fixed)(3000.45 * GLOBAL1);
  b = (fixed)(4 * GLOBAL1);
  c = FixedMul(a, b);
  if (c != 786549760) { // ~ 12001.8 * GLOBAL1
    printf("3000.45*4: FixedMul(%ld, %ld) = %ld\n", a, b, c);
    abort();
  }

  d = fixed_rounded_down(a);
  if (d != 3000) {
    printf("FixedRoundedDown(%ld) = %d\n", a, d);
    abort();
  }

  // Test 6: -0.5 * 0.5 = -0.25
  a = (fixed)(-0.5 * GLOBAL1);
  b = (fixed)(0.5 * GLOBAL1);
  c = FixedMul(a, b);
  if (c != (fixed)(-0.25 * GLOBAL1)) {
    printf("-0.5*0.5: FixedMul(%ld, %ld) = %ld\n", a, b, c);
    abort();
  }

  d = fixed_rounded_down(a);
  if (d != -1) {
    printf("FixedRoundedDown(%ld) = %d\n", a, d);
    abort();
  }

  a = -4640; // FFFF FFFF FFFF EDE0 ?? -0.07080078125
  b = 54866; // 0.837188720703125
  // -0.0592736154794693 -3884.5556640625
  c = FixedMul(a, b);
  if (c != -4066) {
    printf("FixedMul(%ld, %ld) = %ld\n", a, b, c);
    abort();
  }

  //  FixedMul(-4640, 54866) = -4066 -- -704647138

  printf("All FixedMul tests passed.\n");
}
