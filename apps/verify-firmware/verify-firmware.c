#include <cpm.h>
#include <ez80-firmware.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(/*const int argc, const char *argv[]*/) {
  int8_t result;

  // write to default char
  result = ez80_uart_out('X');

  printf("\r\nez80_firm_uart_out result: %d\r\n", result);

  printf("\r\nWaiting for char on uart input\r\n");

  result = ez80_uart_in();

  printf("Received %c (%X)\r\n", result, result);
}
