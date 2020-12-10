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
  while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
    resize_if_full((void **) &adapters, length, &capacity, sizeof(i64));
    adapters[length] = atoi(buffer);
    ++length;
  }

  qsort(adapters, length, sizeof(adapters[0]), &compare_adapters);
  /* for (u32 i = 0; i < length; ++i) { */
  /*   printf("%i\n", adapters[i]); */
  /* } */

  u32 differences[4] = {0};
  assert(adapters[0] < 4 && adapters[0] >= 0);
  ++differences[adapters[0]]; // Account for wall to first adapter difference
  for (u32 i = 1; i < length; ++i) {
    u32 difference = adapters[i] - adapters[i -1];
    assert(difference < 4);
    ++differences[difference];
  }
  ++differences[3]; // Account for final adapter to device difference

  /* for (u32 i = 0; i < 4; ++i) { */
  /*   printf("%i - %i jolt differences\n", differences[i], i); */
  /* } */

  printf("Exercise 1: %i\n", differences[1] * differences[3]);

  fclose(file);

  return 0;
}
