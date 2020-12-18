#include "aoc_types.h"
#include "assert.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "ctype.h"

#define BUFFER_SIZE 1024

typedef struct Spoken {
  i32 second_last;
  i32 last;
} Spoken;

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

  u32 capacity = 30000000;
  Spoken *numbers = calloc(capacity, sizeof(numbers[0]));

  char buffer[BUFFER_SIZE];
  fgets(buffer, BUFFER_SIZE, file);

  char *lasts;
  char *token = strtok_r(buffer, ",", &lasts);
  i32 turn = 1;
  i32 last_key = 0;
  while(token != NULL) {
    i32 key = atoi(token);
    Spoken spoken = {
        .last = turn,
        .second_last = turn
    };

    numbers[key] = spoken;
    last_key = key;

    ++turn;
    token = strtok_r(NULL, ",", &lasts);
  }

  i32 exercise1_answer = 0;
  while (turn <= 30000000) {
    Spoken last = numbers[last_key];
    i32 diff = last.last - last.second_last;

    Spoken *current = &numbers[diff];
    if (current->last > 0) {
      current->second_last = current->last;
      current->last = turn;
    } else {
      current->last = turn;
      current->second_last = turn;
    }

    last_key = diff;
    if (turn == 2020) {
      exercise1_answer = diff;
    }
    ++turn;
  }

  printf("Exercise 1: %i\n", exercise1_answer);
  printf("Exercise 2: %i\n", last_key);

  free(numbers);

  fclose(file);

  return 0;
}
