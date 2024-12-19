#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef int32_t fixed;

#define GLOBAL1       (1l << 16)

extern void spike(int32_t a);

extern fixed FixedMul (fixed a, fixed b);

// fixed FixedMul (fixed a, fixed b)
// 	return (a>>8)*(b>>8);
// }

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

  // Test 1: 1 * 1 = 1
  a = 1 * GLOBAL1;
  b = 1 * GLOBAL1;
  c = FixedMul(a, b);
  if (c != 1 * GLOBAL1) {
    printf("FixedMul(%ld, %ld) = %ld\n", a, b, c);
    abort();
  }

  // Test 2: 2 * 2 = 4
  a = 2 * GLOBAL1;
  b = 2 * GLOBAL1;
  c = FixedMul(a, b);
  if (c != 4 * GLOBAL1) {
    printf("FixedMul(%ld, %ld) = %ld\n", a, b, c);
    abort();
  }

  // Test 3: -1 * 1 = -1
  a = -1 * GLOBAL1;
  b = 1 * GLOBAL1;
  c = FixedMul(a, b);
  if (c != -1 * GLOBAL1) {
    printf("FixedMul(%ld, %ld) = %ld\n", a, b, c);
    abort();
  }

  // Test 4: 1.5 * 2 = 3
  a = (fixed)(1.5 * GLOBAL1);
  b = 2 * GLOBAL1;
  c = FixedMul(a, b);
  if (c != 3 * GLOBAL1) {
    printf("FixedMul(%ld, %ld) = %ld\n", a, b, c);
    abort();
  }

  // Test 5: -2 * -2 = 4
  a = -2 * GLOBAL1;
  b = -2 * GLOBAL1;
  c = FixedMul(a, b);
  if (c != 4 * GLOBAL1) {
    printf("FixedMul(%ld, %ld) = %ld\n", a, b, c);
    abort();
  }

  // Test 6: 0.5 * 0.5 = 0.25
  a = (fixed)(0.5 * GLOBAL1);
  b = (fixed)(0.5 * GLOBAL1);
  c = FixedMul(a, b);
  if (c != (fixed)(0.25 * GLOBAL1)) {
    printf("FixedMul(%ld, %ld) = %ld\n", a, b, c);
    abort();
  }

    // Test 6: 60000.45 * 4
  a = (fixed)(60000.45 * GLOBAL1);
  b = (fixed)(4 * GLOBAL1);
  c = FixedMul(a, b);
  if (c != (fixed)(240001.8 * GLOBAL1)) {
    printf("FixedMul(%ld, %ld) = %ld\n", a, b, c);
    abort();
  }


  printf("All FixedMul tests passed.\n");
}
