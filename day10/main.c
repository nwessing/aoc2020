#include "aoc_types.h"
#include "assert.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "ctype.h"

#define BUFFER_SIZE 1024

int compare_adapters(const void *adapter1, const void *adapter2) {
  i32 value1 = *(i32*)adapter1;
  i32 value2 = *(i32*)adapter2;
  return value1 - value2;
}

u64 count_possibilities(i32 adapters[], u32 length) {
  // the number at each index, is how many possible paths
  // lead to the last adapter from that adapter
  u64 *path_counts = calloc(length, sizeof(u64));
  path_counts[length - 1] = 1;
  for (i32 i = length - 1; i >= 0; --i) {
    i32 j = i + 1;
    while (j <= length - 1 && adapters[j] - adapters[i] <= 3) {
      path_counts[i] += path_counts[j];
      ++j;
    }
  }

  i64 result = path_counts[0];
  free(path_counts);
  return result;
}


int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Need an input file!\n");
    return 1;
  }

  u32 capacity = 1024;
  u32 length = 0;
  i32 *adapters = calloc(capacity, sizeof(i32));

  char *filename = argv[1];
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    printf("could not load file %s\n", filename);
    return 1;
  }

  char buffer[BUFFER_SIZE];
  adapters[length++] = 0;
  while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
    resize_if_full((void **) &adapters, length, &capacity, sizeof(i64));
    adapters[length] = atoi(buffer);
    ++length;
  }

  qsort(adapters, length, sizeof(adapters[0]), &compare_adapters);
  resize_if_full((void **) &adapters, length, &capacity, sizeof(i64));
  adapters[length] = adapters[length - 1] + 3;
  ++length;

  u32 differences[4] = {0};
  for (u32 i = 1; i < length; ++i) {
    u32 difference = adapters[i] - adapters[i -1];
    assert(difference < 4);
    ++differences[difference];
  }

  printf("Exercise 1: %i\n", differences[1] * differences[3]);

  u64 exercise2_result = count_possibilities(adapters, length);
  printf("Exercise 2: %lli\n", exercise2_result);

  fclose(file);

  return 0;
}
