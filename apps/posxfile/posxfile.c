#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void test_stat() {
  struct stat st;
  int         r = stat("C:SAMPLE.TXT", &st);
  printf("stat: %d, (errno: %d) OK\r\n", r, errno);
}

void test_file_create_write() {
  const char *filename = "C:SAMPLE.TXT";
  const char *text     = "Hello, this is a sample text.\r\n";
  int         fd;

  // Open the file for writing (create if it doesn't exist, truncate if it does)
  fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    perror("Failed to open file");
    abort();
  }

  // Write the text to the file
  ssize_t bytes_written = write(fd, text, strlen(text));
  if (bytes_written == -1) {
    perror("Failed to write to file");
    close(fd);
    abort();
  }

  // Close the file
  if (close(fd) == -1) {
    perror("Failed to close file");
    abort();
  }

  printf("Successfully wrote to the file: %s\r\n", filename);
}

int main(/*int argc, char *argv[]*/) {

  test_file_create_write();

  test_stat();

  return 0;
}
