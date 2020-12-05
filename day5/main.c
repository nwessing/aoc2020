#include "assert.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "ctype.h"
#include "stdbool.h"


#define BUFFER_SIZE 1024
#define NUM_ROWS 128
#define NUM_COLS 8

typedef struct Seat {
  uint32_t col;
  uint32_t row;
} Seat;

uint32_t seat_to_id(uint32_t row, uint32_t col) {
  return (row * NUM_COLS) + col;
}

uint32_t calc_seat_id(char *input) {
  uint32_t row_min = 0, row_max = NUM_ROWS;
  uint32_t col_min = 0, col_max = NUM_COLS;
  while (*input != '\0') {
    uint32_t row_diff = row_max - row_min;
    uint32_t col_diff = col_max - col_min;
    switch (*input) {
      case 'F':
        row_max -= row_diff / 2;
        break;
      case 'B':
        row_min += row_diff / 2;
        break;
      case 'L':
        col_max -= col_diff / 2;
        break;
      case 'R':
        col_min += col_diff / 2;
        break;
    }
    ++input;
  }

  return seat_to_id(row_min, col_min);
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
  bool presence[NUM_ROWS * NUM_COLS] = {0};
  while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
    uint32_t seat_id = calc_seat_id(buffer);
    presence[seat_id] = true;
    if (seat_id > exercise1_answer) {
      exercise1_answer = seat_id;
    }
  }

  printf("Exercise 1: %i\n", exercise1_answer);

  for (uint32_t id = 1; id < (NUM_ROWS * NUM_COLS) - 1; ++id) {
    if (!presence[id] && presence[id - 1] && presence[id + 1]) {
      printf("Exercise 2: %i\n", id);
      break;
    }
  }

  fclose(file);

  return 0;
}
