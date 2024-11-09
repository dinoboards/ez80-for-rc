#include <stdint.h>
#include <stdio.h>
#include <string.h>

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
  char str[] = "Hello, World!";
  char *ptr = strchr(str, 'W');
  if (ptr != NULL && strcmp(ptr, "World!") == 0) {
    printf("strchr: OK\r\n");
  } else {
    printf("strchr: FAIL\r\n");
  }
}

void test_echo_args(const int argc, const char *argv[]) {
  printf("argc: %d\r\n", argc);
  for (int i = 0; i < argc; i++) {
    printf("argv[%d]: %s\r\n", i, argv[i]);
  }
}

int main(const int argc, const char *argv[]) {
  test_echo_args(argc, argv);

  test_strcmp();
  test_strcpy();
  test_strchr();

  return 0;
}
