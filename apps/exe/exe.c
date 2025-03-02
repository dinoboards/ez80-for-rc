#include <cpm.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Loaded binaries are by default targetted to start at: 0x200400
And their CP/M vectors are at 0x200000 to 0x2003FF

so for a relocated binary we need to 0x400000:

  copy marshal code to 0x400000

  Any address in the range of 0x200000...0x2003FF to 0x400000...0x4003FF
  And address in the main exe also relocated up

  relocation table at end of binary file
  length: 24 bit count of address to be modified
  uint24_t[] address - each item points to main body.  read the address there and add 0x200000


*/

typedef int (*main_func_ptr)(int argc, const char **argv);

// extern int main_func(int argc, const char **argv);

extern const uint8_t start_marshalling[];
extern const uint8_t end_marshalling[];

static CPM_FCB fcb __attribute__((section(".bss_z80")));
uint8_t        buffer[SECSIZE] __attribute__((section(".bss_z80")));

int extract_filename_parts(const char *input, CPM_FCB *fcb) {
  // Initialize output buffers
  fcb->drive = 0;
  memset(fcb->name, ' ', 8); // includes ext
  fcb->ext[0] = 'E';
  fcb->ext[1] = 'X';
  fcb->ext[2] = 'E';

  const char *ptr      = input;
  int         name_len = 0;
  int         ext_len  = 0;

  // Check for drive designation
  if (isalpha(ptr[0]) && ptr[1] == ':') {
    fcb->drive = toupper(ptr[0]) - 'A' + 1;
    ptr += 2;
  }

  // Extract filename
  while (*ptr && *ptr != '.' && name_len < 8) {
    if (!isalnum(*ptr))
      return -1; // Invalid character
    fcb->name[name_len++] = toupper(*ptr);
    ptr++;
  }

  // Skip the dot if present
  if (*ptr == '.')
    ptr++;

  // Extract extension
  while (*ptr && ext_len < 3) {
    if (!isalnum(*ptr))
      return -1; // Invalid character
    fcb->ext[ext_len++] = toupper(*ptr);
    ptr++;
  }

  return 0; // Success
}

uint24_t base_address      = 0x200000;
uint8_t  relocation_offset = 0x00;

const char *filename = NULL;

uint8_t process_args(const int argc, const char *argv[]) {
  if (argc >= 4 && strcmp(argv[1], "-T") == 0) {
    // assume: exe -t <base> <filename> <args>

    base_address = strtoul(argv[2], NULL, 16);
    if (base_address < 0x20 || base_address > 0x58) {
      printf("Error: target base address must be between 0x20 and 0x58\r\n");
      exit(1);
    }
    relocation_offset = base_address - 0x20;
    base_address <<= 16;
    filename = argv[3];
    return 3;
  }

  if (argc >= 2) {
    // assume exe <filename> <args>

    filename = argv[1];
    if (filename[0] == '-') {
      printf("Usage: %s [-t <base-address>] <filename> <...arguments>\r\n", argv[0]);
      exit(1);
    }

    return 1;
  }

  printf("Usage: %s [-t <base-address>] <filename> <...arguments>\r\n", argv[0]);
  exit(1);
}

int load_and_execute(const int argc, const char *argv[]) {
  uint24_t       start_address       = base_address + 0x400;
  uint8_t *const marshalling_vectors = (uint8_t *)base_address;
  main_func_ptr  main_func           = (main_func_ptr)start_address;
  uint8_t       *ptr                 = (uint8_t *)start_address;

  extract_filename_parts(filename, &fcb);

  cpm_f_error_t result = cpm_f_open(AS_NEAR_PTR(&fcb));
  if (result != 0) {
    printf("Failed to open file %s\r\n", filename);
    return 1;
  }

  cpm_f_dmaoff(AS_NEAR_PTR(buffer));

  do {
    result = cpm_f_read(AS_NEAR_PTR(&fcb));
    if (result == 1) { // eof
      break;
    }

    if (result != 0) {
      printf("Error reading file %s\r\n", filename);
      return 1;
    }

    memcpy(ptr, buffer, SECSIZE);
    ptr += SECSIZE;

  } while (true);

  cpm_f_close(AS_NEAR_PTR(&fcb));

  if (relocation_offset != 0x00) {
    ptr -= 3;
    uint24_t relocation_table_offset = *((uint24_t *)ptr);
    ptr -= 3;
    uint24_t  relocation_table_count = *((uint24_t *)ptr);
    uint24_t *relocation_table       = (uint24_t *)(start_address + relocation_table_offset);
    for (uint24_t i = 0; i < relocation_table_count; i++) {
      uint8_t *r = ((uint8_t *)relocation_table[i] + start_address);
      *r += relocation_offset;
    }
  }

  memcpy(marshalling_vectors, start_marshalling, end_marshalling - start_marshalling);

  return main_func(argc, argv);
}

int main(const int argc, const char *argv[]) {
  uint8_t arg_start = process_args(argc, argv);

  return load_and_execute(argc - arg_start, &argv[arg_start]);
}
