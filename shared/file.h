#pragma once
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

/*
 *  Parse file into a null-terminated string. Caller must free!
 */
char *read_file_as_string(const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    printf("could not load file %s\n", filename);
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  uint32_t length = ftell(file);
  fseek(file, 0, SEEK_SET);
  char *file_buffer = malloc(sizeof(char) * (length + 1));
  fread(file_buffer, 1, length, file);
  file_buffer[length] = '\0';

  fclose(file);

  return file_buffer;
}
