#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __EZ80__
#include <io.h>
#endif

char buffer[150];

void test_c_create_write_file(void) {
  FILE *f = fopen("C:BOB.DAT", "w");
  printf("fopen: %p\n", f);
  if (f == NULL) {
    printf("fopen: %p, (errno: %d) FAIL\n", f, errno);
    return;
  }

  const char *text = "Hello, this is a sample text.\r\n";
  size_t      n    = fwrite(text, 1, strlen(text), f);
  printf("fwrite: %d, (errno: %d) OK\n", n, errno);

  int r = fclose(f);
  printf("fclose: %d, (errno: %d) OK\n", r, errno);
}

void test_c_create_text_file(void) {
  FILE *f = fopen("C:BOB.TXT", "wt");
  printf("fopen(wt): %p\n", f);
  if (f == NULL) {
    printf("fopen: %p, (errno: %d) FAIL\n", f, errno);
    return;
  }

  const char *text = "Hello, this is a sample text.\nAnd this is the next line.\r\n";
  size_t      n    = fwrite(text, 1, strlen(text), f);
  printf("fwrite: %d, (errno: %d) OK\n", n, errno);

  int r = fclose(f);
  printf("fclose: %d, (errno: %d) OK\n", r, errno);
}

void test_c_file_read(void) {
  FILE *f = fopen("C:BOB.DAT", "r");
  printf("fopen: %p\n", f);
  if (f == NULL) {
    printf("fopen: %p, (errno: %d) FAIL\n", f, errno);
    return;
  }
  memset(buffer, 0, sizeof(buffer));

  size_t n;

  do {
    n = fread(buffer, 1, sizeof(buffer), f);
    printf("fread: %d, (errno: %d) OK\n", n, errno);
    for(size_t i = 0; i < n; i++) {
      printf("%x ", buffer[i]);
    }
  } while (n > 0);

  int r = fclose(f);
  printf("fclose: %d, (errno: %d) OK\n", r, errno);
}

void test_c_file_text_read(void) {
  FILE *f = fopen("C:BOB.TXT", "rt");
  printf("fopen(rt): %p\n", f);
  if (f == NULL) {
    printf("fopen: %p, (errno: %d) FAIL\n", f, errno);
    return;
  }
  memset(buffer, 0, sizeof(buffer));

  size_t n;

  do {
    n = fread(buffer, 1, sizeof(buffer), f);
    printf("fread: %d, (errno: %d) OK\n", n, errno);
    for(size_t i = 0; i < n; i++) {
      printf("%x ", buffer[i]);
    }
  } while (n > 0);

  int r = fclose(f);
  printf("fclose: %d, (errno: %d) OK\n", r, errno);
}

void test_c_file_modify(void) {
  FILE *f = fopen("C:BOB.DAT", "r+");
  printf("fopen: %p\n", f);
  if (f == NULL) {
    printf("fopen: %p, (errno: %d) FAIL\n", f, errno);
    return;
  }

  fseek(f, 10, SEEK_SET);
  const char *text = "XXXX";
  size_t      n    = fwrite(text, 1, strlen(text), f);
  printf("fwrite: %d, (errno: %d) OK\n", n, errno);

  int r = fclose(f);
  printf("fclose: %d, (errno: %d) OK\n", r, errno);
}

void test_c_file_size_compliance(void) {
  remove("C:BOB.DAT");
  FILE *f = fopen("C:BOB.DAT", "w");
  printf("fopen: %p\n", f);
  if (f == NULL) {
    printf("fopen: %p, (errno: %d) FAIL\n", f, errno);
    return;
  }

  const char *text = "Hello, this is a sample dat.\nThis is the next line\r\n";
  size_t      n    = fwrite(text, 1, strlen(text), f);
  printf("fwrite: %d, (errno: %d) OK\n", n, errno);

  int r = fclose(f);

  f = fopen("C:BOB.DAT", "r");
  printf("fopen: %p\n", f);
  if (f == NULL) {
    printf("fopen: %p, (errno: %d) FAIL\n", f, errno);
    return;
  }

  r = fseek(f, 0, SEEK_END);
  printf("fseek: %d, (errno: %d) OK\n", r, errno);

  long size = ftell(f);
  printf("ftell: %ld, (errno: %d) OK\n", size, errno);

  r = fclose(f);
  printf("fclose: %d, (errno: %d) OK\n", r, errno);
}

int main(void /*int argc, char *argv[]*/) {

  #ifdef __EZ80__
    // _setmode(fileno( stdin ), _IOBINARY ); //default is _IOTEXT_TRANSLATION
  #endif

  test_c_create_write_file();

  test_c_file_read();

  test_c_file_modify();

  test_c_file_size_compliance();

  test_c_create_text_file();

  test_c_file_text_read();

  return 0;
}
