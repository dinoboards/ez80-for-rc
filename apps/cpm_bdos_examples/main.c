#include <cpm.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

FCB  fcb;
char buffer[150];

void test_cpmfcb_address(void) {
  if ((uint24_t)CPM_FCB == 0x03005C)
    printf("fcb_address: OK\r\n");
  else {
    printf("fcb_address: FAIL\r\n");
  }

  CPM_FCB[0] = 'A';
  CPM_FCB[1] = 0;

  if (CPM_FCB[0] == 'A' && CPM_FCB[1] == 0)
    printf("fcb update: OK\r\n");
  else {
    printf("fcb update: FAIL\r\n");
  }
}

void test_cpm_c_write() {
  cpm_c_write('C');
  cpm_c_write('P');
  cpm_c_write('/');
  cpm_c_write('M');
  cpm_c_write('\r');
  cpm_c_write('\n');
}

void test_cpm_get_iobyte() {
  uint8_t       iobyte  = cpm_get_iobyte();
  const uint8_t console = CPM_IOBYTE_GET_CONSOLE(iobyte);
  const uint8_t reader  = CPM_IOBYTE_GET_READER(iobyte);
  const uint8_t punch   = CPM_IOBYTE_GET_PUNCH(iobyte);
  const uint8_t list    = CPM_IOBYTE_GET_LIST(iobyte);

  printf("cpm_get_iobyte: %X\r\n", iobyte);
  printf("Console: %X, Reader: %X, Punch: %X, List: %X\r\n", console, reader, punch, list);

  const char *list_device = cpm_get_list_device();
  printf("List device: %s\r\n", list_device);

  const char *punch_device = cpm_get_punch_device();
  printf("Punch device: %s\r\n", punch_device);

  const char *reader_device = cpm_get_reader_device();
  printf("Reader device: %s\r\n", reader_device);

  const char *console_device = cpm_get_console_device();
  printf("Console device: %s\r\n", console_device);
}

void test_cpm_c_writestr() { cpm_c_writestr(AS_NEAR_PTR("CP/M\r\n$")); }

void test_cpm_c_readstr() {
  memset(buffer, 0, sizeof(buffer));
  buffer[0] = 120;
  buffer[1] = 120;

  printf("Enter a string:\r\n");
  cpm_c_readstr(AS_CPM_PTR(buffer)); // can not be on the stack as that is not near memory
  printf("cpm_c_readstr: %d, %d, %s\r\n", buffer[0], buffer[1], &buffer[2]);
}

void test_cpm_c_stat() {
  printf("Press any key to continue\r\n");
  uint8_t stat;
  while ((stat = cpm_c_stat()) == 0) {
    // wait for a key press
  }
  printf("cpm_c_stat: %d, OK\r\n", stat);

  char c = cpm_c_read();
  printf("cpm_c_read: %d, OK\r\n", c);
}

void test_cpm_c_rawio() {
  printf("Press any key to continue\r\n");
  uint8_t stat;

  while ((stat = cpm_c_rawio()) == 0)
    ;

  printf("cpm_c_rawio: %d, OK\r\n", stat);

  while (cpm_c_rawio() != 0)
    ;
}

void test_cpm_s_bdosver() {
  uint16_t version = cpm_s_bdosver();
  printf("cpm_s_bdosver: %x, OK\r\n", version);
}

void test_cpm_drv_allreset() {
  const uint8_t r = cpm_drv_allreset();
  printf("cpm_drv_allreset: %d, OK\r\n", r);
}

void test_cpm_drv_set_get() {
  uint8_t r = cpm_drv_get();
  printf("cpm_drv_get: %d, OK\r\n", r);

  r = cpm_drv_set(1);
  printf("cpm_drv_set(1): %d, OK\r\n", r);

  r = cpm_drv_get();
  if (r == 1)
    printf("cpm_drv_get: %d, OK\r\n", r);
  else
    printf("cpm_drv_get: %d, FAIL\r\n", r);
}

void test_cpm_f_make() {
  memset(&fcb, 0, sizeof(fcb));
  fcb.drive = 1;
  strncpy(fcb.name, "BOB     ", 8);
  strncpy(fcb.ext, "TXT", 3);

  uint16_t r = cpm_f_make(AS_NEAR_PTR(&fcb));
  printf("cpm_f_make: %d, (errno: %d) OK\r\n", r, errno);

  cpm_f_dmaoff(AS_NEAR_PTR(buffer));
  memset(buffer, 26, 129);
  strcpy(buffer, "Hello World\r\n");

  uint16_t rr = cpm_f_write(AS_NEAR_PTR(&fcb));
  printf("cpm_f_write: %X OK\r\n", rr);

  rr = cpm_f_close(AS_NEAR_PTR(&fcb));
  printf("cpm_f_close: %X OK\r\n", rr);
}

void test_cpm_f_open() {
  memset(&fcb, 0, sizeof(fcb));
  fcb.drive = 3;
  strncpy(fcb.name, "BOB     ", 8);
  strncpy(fcb.ext, "TXT", 3);

  uint16_t r = cpm_f_open(AS_NEAR_PTR(&fcb));
  printf("cpm_f_open: %d, (errno: %d) OK\r\n", r, errno);

  cpm_f_dmaoff(AS_NEAR_PTR(buffer));
  memset(buffer, 0, sizeof(buffer));
  strcpy(buffer, "SHOULD NOT SEE THIS!\r\n");

  fcb.ranrec = 0;
  uint16_t rr;

  do {
    rr = cpm_f_readrand(AS_NEAR_PTR(&fcb));
    printf("cpm_f_readrand: %X OK\r\n", rr);

    // uint16_t rr = cpm_f_read(AS_NEAR_PTR(&fcb));
    // printf("cpm_f_read: %X OK\r\n", rr);
    printf("fcb.next_record: %d\r\n", fcb.next_record);
    printf("fcb.ranrec: %d\r\n", fcb.ranrec);
    fcb.ranrec++;
  } while (rr == 0);

  printf("buffer: %s\r\n", buffer);

  // cpm_f_dmaoff(AS_NEAR_PTR(buffer));
  // rr = cpm_f_read(AS_NEAR_PTR(&fcb));
  // printf("cpm_f_read: %X OK\r\n", rr);
  // printf("fcb.next_record: %d\r\n", fcb.next_record);
  // printf("fcb.ranrec: %d\r\n", fcb.ranrec);

  rr = cpm_f_close(AS_NEAR_PTR(&fcb));
  printf("cpm_f_close: %X OK\r\n", rr);
}

void test_cpm_f_usernum() {
  uint8_t r = cpm_f_usernum(0xFF);
  printf("cpm_f_usernum(0xFF): %d, OK\r\n", r);

  r = cpm_f_usernum(1);
  printf("cpm_f_usernum(1): %d, OK\r\n", r);

  r = cpm_f_usernum(0xFF);
  printf("cpm_f_usernum(0xFF): %d, OK\r\n", r);

  r = cpm_f_usernum(0);
  printf("cpm_f_usernum(0): %d, OK\r\n", r);
}

void test_cpm_f_first_next() {
  memset(&fcb, 0, sizeof(fcb));
  fcb.drive = 3;
  strncpy(fcb.name, "BO?     ", 8);
  strncpy(fcb.ext, "TXT", 3);

  uint16_t r = cpm_f_sfirst(AS_NEAR_PTR(&fcb));

  printf("cpm_f_first: %d, (errno: %d) OK\r\n", r, errno);

  r = cpm_f_snext(AS_NEAR_PTR(&fcb));
  printf("cpm_f_next: %d, (errno: %d) OK\r\n", r, errno);
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

void test_c_stat() {
  struct stat st;
  int         r = stat("C:BOX.TXT", &st);
  printf("stat: %d, (errno: %d) OK\r\n", r, errno);
}

int flags = 123;

int main(/*int argc, char *argv[]*/) {

  test_cpmfcb_address();

  test_cpm_c_write();

  test_cpm_c_writestr();

  test_cpm_get_iobyte();

  test_cpm_c_rawio();

  test_cpm_c_stat();

  test_cpm_c_readstr();

  test_cpm_s_bdosver();

  test_cpm_drv_allreset();

  test_cpm_drv_set_get();

  test_cpm_f_make();

  test_cpm_f_open();

  test_cpm_f_first_next();

  test_c_file_read();

  test_c_stat();

  test_cpm_f_usernum();

  cpm_term();

  printf("Should never get here\r\n");

  return 0;
}
