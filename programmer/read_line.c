#include "read_line.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int  line_index = 0;
int  cursor_pos = 0;
int  ch;
char input_buffer[BUFFER_SIZE];

void handle_backspace() {
  if (cursor_pos > 0) {
    cursor_pos--;
    line_index--;
    memmove(&input_buffer[cursor_pos], &input_buffer[cursor_pos + 1], line_index - cursor_pos + 1);
    printf("\b");                             // Move cursor back, print space, move cursor back again
    printf("%s ", &input_buffer[cursor_pos]); // Reprint the rest of the buffer
    for (int i = cursor_pos; i < line_index + 1; i++)
      printf("\b");
  }
}

void handle_delete() {
  if (cursor_pos < line_index) {
    memmove(&input_buffer[cursor_pos], &input_buffer[cursor_pos + 1], line_index - cursor_pos);
    line_index--;
    printf("%s ", &input_buffer[cursor_pos]); // Reprint the rest of the buffer
    for (int i = cursor_pos; i < line_index + 1; i++) {
      printf("\b");
    }
  }
}

void handle_escape_code() {
  getchar(); // Skip the '[' character
  switch (getchar()) {
  case 'D': // Left arrow
    if (cursor_pos > 0) {
      cursor_pos--;
      printf("\b");
    }
    break;
  case 'C': // Right arrow
    if (cursor_pos < line_index) {
      printf("%c", input_buffer[cursor_pos]);
      cursor_pos++;
    }
    break;

  case '3': // Delete key (Escape sequence is ESC [ 3 ~)
    if (getchar() == '~')
      handle_delete();

    break;
  }
}

void handle_char_insertion() {
  if (cursor_pos < line_index)
    memmove(&input_buffer[cursor_pos + 1], &input_buffer[cursor_pos], line_index - cursor_pos);

  input_buffer[cursor_pos++] = ch;
  line_index++;
  printf("%s", &input_buffer[cursor_pos - 1]); // Print the rest of the input_buffer
  for (int i = cursor_pos; i < line_index; i++)
    printf("\b");
}

void read_line() {
  memset(input_buffer, 0, BUFFER_SIZE);
  line_index = 0;
  cursor_pos = 0;

  while (true) {
    ch = getchar();

    if (ch == '\r' || ch == '\n')
      break;
    else if (ch == '\b') // Handle backspace
      handle_backspace();
    else if (ch == 27) // Handle arrow keys (Escape sequence)
      handle_escape_code();
    else if (ch == 127) // Handle delete key
      handle_delete();
    else if (line_index < BUFFER_SIZE - 1)
      handle_char_insertion();
  }
  input_buffer[line_index] = '\0'; // Null-terminate the string
}
