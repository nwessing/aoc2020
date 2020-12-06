#include "assert.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "ctype.h"
#include "stdbool.h"

#define BUFFER_SIZE 1024

uint32_t count_set_bits(uint32_t value) {
  uint32_t result = 0;
  for (uint32_t i = 0; i < 32; ++i) {
    result += ((value >> i) & 1);
  }
  return result;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Need an input file!\n");
    return 1;
  }

  char *filename = argv[1];
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    printf("could not load file %s\n", filename);
    return 1;
  }

  char buffer[BUFFER_SIZE];

  uint32_t exercise1_answer = 0;
  uint32_t exercise2_answer = 0;

  uint32_t answers_and = 0xFFFFFFFF;
  uint32_t answers_or = 0;
  while (1) {
    char *fgets_result = fgets(buffer, BUFFER_SIZE, file);
    if (fgets_result == NULL || strcmp(buffer, "\n") == 0) {
      exercise1_answer += count_set_bits(answers_or);
      exercise2_answer += count_set_bits(answers_and);
      answers_and = 0xFFFFFFFF;
      answers_or = 0;
      if (fgets_result == NULL) {
        break;
      }
      continue;

    }

    uint32_t answer = 0;
    char *cursor = buffer;
    while(*cursor != '\0') {
      if (*cursor == '\n') {
        ++cursor;
        continue;
      }

      size_t i = *cursor - 'a';
      assert(i <= 26);
      answer |= (1 << i);
      ++cursor;
    }

    answers_or |= answer;
    answers_and &= answer;
  }

  printf("Exercise 1: %i\n", exercise1_answer);
  printf("Exercise 1: %i\n", exercise2_answer);

  fclose(file);

  return 0;
}
