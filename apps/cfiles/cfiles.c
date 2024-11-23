#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char buffer[150];

void test_c_create_write_file() {
  FILE *f = fopen("C:BOB.TXT", "w");
  printf("fopen: %p\r\n", f);
  if (f == NULL) {
    printf("fopen: %p, (errno: %d) FAIL\r\n", f, errno);
    return;
  }

  const char *text = "Hello, this is a sample text.\n";
  size_t      n    = fwrite(text, 1, strlen(text), f);
  printf("fwrite: %d, (errno: %d) OK\r\n", n, errno);

  int r = fclose(f);
  printf("fclose: %d, (errno: %d) OK\r\n", r, errno);
}

void test_c_file_read() {
  FILE *f = fopen("C:BOB.TXT", "r");
  printf("fopen: %p\r\n", f);
  if (f == NULL) {
    printf("fopen: %p, (errno: %d) FAIL\r\n", f, errno);
    return;
  }
  memset(buffer, 0, sizeof(buffer));

  size_t n;

  do {
    n = fread(buffer, 1, sizeof(buffer), f);
    printf("fread: %d, (errno: %d) OK\r\n", n, errno);
  } while (n > 0);

  int r = fclose(f);
  printf("fclose: %d, (errno: %d) OK\r\n", r, errno);
}

void test_c_file_modify() {
  FILE *f = fopen("C:BOB.TXT", "r+");
  printf("fopen: %p\r\n", f);
  if (f == NULL) {
    printf("fopen: %p, (errno: %d) FAIL\r\n", f, errno);
    return;
  }

  fseek(f, 10, SEEK_SET);
  const char *text = "XXXX";
  size_t      n    = fwrite(text, 1, strlen(text), f);
  printf("fwrite: %d, (errno: %d) OK\r\n", n, errno);

  int r = fclose(f);
  printf("fclose: %d, (errno: %d) OK\r\n", r, errno);
}

int main(/*int argc, char *argv[]*/) {

  test_c_create_write_file();

  test_c_file_read();

  test_c_file_modify();
}
