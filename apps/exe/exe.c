#include <cpm.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern int main_func(int argc, const char **argv);

extern const uint8_t start_marshalling[];
extern const uint8_t end_marshalling[];

uint8_t *const marshalling_vectors = (uint8_t *)(0x200000);

static CPM_FCB fcb __attribute__((section(".bss_z80")));
uint8_t        buffer[SECSIZE] __attribute__((section(".bss_z80")));

int extract_filename_parts(const char *input, CPM_FCB *fcb) {
  // Initialize output buffers
  fcb->drive = 0;
  memset(fcb->name, ' ', 8 + 3); // includes ext

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

int main(const int argc, const char *argv[]) {
  // size_t n;

  if (argc < 2) {
    printf("Usage: %s <filename> <...arguments>\r\n", argv[0]);
    return 1;
  }

  const char *filename = argv[1];
  uint8_t    *ptr      = (uint8_t *)0x200400;

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

    printf(".");

    memcpy(ptr, buffer, SECSIZE);
    ptr += SECSIZE;

  } while (true);

  cpm_f_close(AS_NEAR_PTR(&fcb));

  memcpy(marshalling_vectors, start_marshalling, end_marshalling - start_marshalling);

  return main_func(argc - 1, &argv[1]);
}
