#include "aoc_types.h"
#include "assert.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "ctype.h"

#define BUFFER_SIZE 1024
#define PREAMBLE_PREV_TO_CONSIDER 25

bool find_non_conforming(i64 preamble[], i64 search_value, u32 start_index) {
  for (u32 i = start_index; i < start_index + PREAMBLE_PREV_TO_CONSIDER; ++i) {
    for (u32 j = start_index; j < start_index + PREAMBLE_PREV_TO_CONSIDER; ++j) {
      if (i == j) { continue; }
      if (preamble[i] + preamble[j] == search_value) {
        return false;
      }
    }
  }

  return true;
}

i64 *build_sum_list(i64 preamble[], u32 length) {
  i64 *sums = calloc(length, sizeof(i64));

  sums[0] = preamble[0];
  for (u32 i = 1; i < length; ++i) {
    sums[i] = sums[i - 1] + preamble[i];
  }

  return sums;
}

void find_sum_range(i64 *sums, u32 length, i64 target, i64 *smallest, i64 *largest) {
  for (u32 i_min = 0; i_min < length; ++i_min) {
    for (u32 i_max = i_min + 2; i_max < length; ++i_max) {
      i64 sum = sums[i_max] - sums[i_min];
      if (sum == target) {

        // Find the smallest and largest values in range
        for (u32 i = i_min - 1; i < i_max; ++i) {
          i64 value = sums[i + 1] - sums[i];;
          if (value < *smallest) {
            *smallest = value;
          }

          if (value > *largest) {
            *largest = value;
          }
        }

        return;
      }
    }
  }
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

  u32 capacity = 1024, length = 0;
  i64 *preamble = calloc(capacity, sizeof(i64));

  char buffer[BUFFER_SIZE];
  while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
    resize_if_full((void **) &preamble, length, &capacity, sizeof(i64));
    preamble[length] = atoi(buffer);
    ++length;
  }

  assert(PREAMBLE_PREV_TO_CONSIDER < length);
  i64 invalid_number = 0;
  for (u32 i = PREAMBLE_PREV_TO_CONSIDER; i < length; ++i) {
    i64 value = preamble[i];
    bool found_answer = find_non_conforming(preamble, value, i - PREAMBLE_PREV_TO_CONSIDER);
    if (found_answer) {
      invalid_number = value;
      printf("Exercise 1: %lli\n", invalid_number);
      break;
    }
  }

  i64 *sums = build_sum_list(preamble, length);
  i64 smallest = INT64_MAX, largest = 0;
  find_sum_range(sums, length, invalid_number, &smallest, &largest);
  printf("Exercise 2: %lli, (%lli + %lli)\n", smallest + largest, smallest, largest);

  free(sums);
  free(preamble);
  fclose(file);

  return 0;
}
