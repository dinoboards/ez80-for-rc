#include <cpm.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CPM_FCB fcb __attribute__((section(".bss_z80")));
char    buffer[150] __attribute__((section(".bss_z80")));

void test_cpmfcb_address(void) {
  if (CPM_SYS_FCB == (CPM_FCB *)0x03005C)
    printf("fcb_address: OK\r\n");
  else {
    printf("fcb_address: FAIL %X, %X\r\n", (uintptr_t)&CPM_SYS_FCB, (uintptr_t)CPM_SYS_FCB);
    exit(1);
  }

  CPM_SYS_FCB->drive   = 'A';
  CPM_SYS_FCB->name[0] = 0;

  if (CPM_SYS_FCB->drive == 'A' && CPM_SYS_FCB->name[0] == 0)
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

void test_cpm_c_writestr() {
  strcpy(buffer, "CP/M\r\n$");

  cpm_c_writestr(AS_NEAR_PTR(buffer));
}

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
  fcb.drive = 3;
  strncpy(fcb.name, "BOB     ", 8);
  strncpy(fcb.ext, "TXT", 3);

  cpm_f_error_t r = cpm_f_make(AS_NEAR_PTR(&fcb));
  printf("cpm_f_make: %d OK\r\n", r);

  cpm_f_dmaoff(AS_NEAR_PTR(buffer));
  memset(buffer, 26, 129);
  strcpy(buffer, "Hello World\r\n");

  cpm_f_error_t rr = cpm_f_write(AS_NEAR_PTR(&fcb));
  printf("cpm_f_write: %X OK\r\n", rr);

  rr = cpm_f_close(AS_NEAR_PTR(&fcb));
  printf("cpm_f_close: %X OK\r\n", rr);
}

void test_cpm_f_make_write_rand() {
  memset(&fcb, 0, sizeof(fcb));
  fcb.drive = 3;
  strncpy(fcb.name, "BOB     ", 8);
  strncpy(fcb.ext, "TXT", 3);

  cpm_f_error_t r = cpm_f_delete(AS_NEAR_PTR(&fcb));
  if (r == 0)
    printf("cpm_f_delete: OK\r\n");
  else
    printf("cpm_f_delete: %d ERROR\r\n", r);

  r = cpm_f_delete(AS_NEAR_PTR(&fcb));
  if (r == 0)
    printf("cpm_f_delete: ERROR\r\n");
  else
    printf("cpm_f_delete: %d OK\r\n", r);

  r = cpm_f_make(AS_NEAR_PTR(&fcb));
  printf("cpm_f_make: %d OK\r\n", r);

  cpm_f_dmaoff(AS_NEAR_PTR(buffer));
  memset(buffer, 26, 129);
  strcpy(buffer, "Hello World\r\n");

  fcb.ranrec = 0;
  r          = cpm_f_writerand(AS_NEAR_PTR(&fcb));
  printf("cpm_f_write: %X OK\r\n", r);

  r = cpm_f_close(AS_NEAR_PTR(&fcb));
  printf("cpm_f_close: %X OK\r\n", r);
}

void test_cpm_f_open() {
  memset(&fcb, 0, sizeof(fcb));
  fcb.drive = 3;
  strncpy(fcb.name, "BOB     ", 8);
  strncpy(fcb.ext, "TXT", 3);

  cpm_f_error_t r = cpm_f_open(AS_NEAR_PTR(&fcb));
  printf("cpm_f_open: %d OK\r\n", r);

  cpm_f_dmaoff(AS_NEAR_PTR(buffer));
  memset(buffer, 0, sizeof(buffer));
  strcpy(buffer, "SHOULD NOT SEE THIS!\r\n");

  fcb.ranrec = 0;

  do {
    r = cpm_f_readrand(AS_NEAR_PTR(&fcb));
    printf("cpm_f_readrand: %d OK\r\n", r);

    printf("fcb.next_record: %d\r\n", fcb.next_record);
    printf("fcb.ranrec: %d\r\n", fcb.ranrec);
    fcb.ranrec++;
  } while (r == 0);

  printf("buffer: %s\r\n", buffer);

  r = cpm_f_close(AS_NEAR_PTR(&fcb));
  printf("cpm_f_close: %X OK\r\n", r);
}

void test_cpm_f_size() {
  memset(&fcb, 0, sizeof(fcb));
  fcb.drive = 3;
  strncpy(fcb.name, "BOB     ", 8);
  strncpy(fcb.ext, "TXT", 3);

  fcb.ranrec      = 0;
  fcb.next_record = 0;

  cpm_f_error_t r = cpm_f_size(AS_NEAR_PTR(&fcb));
  printf("cpm_f_size: %d OK\r\n", r);

  printf("fcb.next_record: %d\r\n", fcb.next_record);
  printf("fcb.ranrec: %d\r\n", fcb.ranrec);
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

  int8_t r = cpm_f_sfirst(AS_NEAR_PTR(&fcb));

  printf("cpm_f_first: %d OK\r\n", r);

  r = cpm_f_snext(AS_NEAR_PTR(&fcb));
  printf("cpm_f_snext: %d OK\r\n", r);
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

  test_cpm_f_make_write_rand();

  test_cpm_f_open();

  test_cpm_f_size();

  test_cpm_f_first_next();

  test_cpm_f_usernum();

  // cpm_term();

  // printf("Should never get here\r\n");

  return 0;
}
