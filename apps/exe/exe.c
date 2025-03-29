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

#define CMD_DEFAULT 0
#define CMD_T       1
#define CMD_M       2
#define CMD_F       4

typedef int (*main_func_ptr)(int argc, const char **argv);

// extern int main_func(int argc, const char **argv);

extern const uint8_t start_marshalling[];
extern const uint8_t end_marshalling[];
extern uint8_t      *largest_continuous;
extern uint8_t      *fastest_continuous;

static CPM_FCB fcb __attribute__((section(".bss_z80")));
uint8_t        buffer[SECSIZE] __attribute__((section(".bss_z80")));

uint24_t        base_address      = 0x200000;
uint8_t         relocation_offset = 0x00;
const char     *filename          = NULL;
static uint24_t cmd               = 0;

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

void find_install_ram();

void show_help() {
  printf("Usage: EXE [options] FILENAME [arguments]\r\n");
  printf("\r\n");
  printf("Load and execute an eZ80 executable program. The file must have\r\n");
  printf("the .EXE extension. Any additional arguments after FILENAME\r\n");
  printf("will be passed to the executed program.\r\n");
  printf("\r\n");
  printf("Options:\r\n");
  printf("  -F          Use fastest available RAM bank (default)\r\n");
  printf("  -M          Use maximum continuous RAM space\r\n");
  printf("  -T=HH       Load into specific target address where HH\r\n");
  printf("              is a hex value between 20-58 (e.g. -T=40)\r\n");
  printf("  -? /?       Show this help message\r\n");
  printf("\r\n");
  printf("Examples:\r\n");
  printf("  EXE PROGRAM         Run in fastest RAM bank\r\n");
  printf("  EXE -M PROGRAM      Run in largest available RAM\r\n");
  printf("  EXE -T=40 PROGRAM   Run at address 0x400000\r\n");
  printf("  EXE PROGRAM ARG1    Run with argument ARG1\r\n");
}

void argument_T(const char *arg) {
  if (strncmp(arg, "-T=", 3) != 0 && strncmp(arg, "/T=", 3) != 0)
    return;

  cmd = CMD_T;

  base_address = strtoul(&arg[3], NULL, 16);
  if (base_address < 0x20 || base_address > 0x58) {
    printf("Error: target base address must be between 0x20 and 0x58\r\n");
    exit(1);
  }
  relocation_offset = base_address - 0x20;
  base_address <<= 16;
}

void argument_M(const char *arg) {
  if (strcmp(arg, "-M") != 0 && strcmp(arg, "/M") != 0)
    return;

  base_address      = (uint24_t)largest_continuous;
  relocation_offset = (base_address >> 16) - 0x20;
  cmd               = CMD_M;
}

void argument_F(const char *arg) {
  if (strcmp(arg, "-F") != 0 && strcmp(arg, "/F") != 0)
    return;

  base_address      = (uint24_t)fastest_continuous;
  relocation_offset = (base_address >> 16) - 0x20;
  cmd               = CMD_F;
}

void argument_help(const char *arg) {
  if (strcmp(arg, "-?") == 0 || strcmp(arg, "/?") == 0) {
    show_help();
    exit(0);
  }
}

int main(const int argc, const char *argv[]) {

  find_install_ram();

  if (largest_continuous == NULL) {
    printf("No extended memory detected.  Unable to load executable\r\n");
    exit(1);
  }

  cmd = CMD_DEFAULT;
  argument_T(argv[1]);
  argument_M(argv[1]);
  argument_F(argv[1]);
  argument_help(argv[1]);

  if (cmd == CMD_DEFAULT && argc <= 1) {
    show_help();
    exit(1);
  }
  if (cmd == CMD_DEFAULT && (argv[1][0] == '-' || argv[1][0] == '/')) {
    show_help();
    exit(1);
  }
  if (cmd != CMD_DEFAULT && argc <= 2) {
    show_help();
    exit(1);
  }
  if (cmd != CMD_DEFAULT && (argv[2][0] == '-' || argv[2][0] == '/')) {
    show_help();
    exit(1);
  }

  if (cmd == CMD_DEFAULT) {
    base_address      = (uint24_t)fastest_continuous;
    relocation_offset = (base_address >> 16) - 0x20;
  }

  uint8_t arg_start = cmd == CMD_DEFAULT ? 1 : 2;
  filename          = argv[arg_start];

  return load_and_execute(argc - arg_start, &argv[arg_start]);
}
