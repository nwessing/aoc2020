#include "assert.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

int32_t *read_file(const char *filename, uint32_t *count) {
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    printf("could not load file %s\n", filename);
    return NULL;
  }

  char buffer[256];
  uint32_t result_capacity = 1024;
  uint32_t result_count = 0;
  int32_t *result = calloc(result_capacity, sizeof(int32_t));
  assert(result != NULL);

  while (fgets(buffer, 256, file) != NULL) {
    result[result_count] = atoi(buffer);
    ++result_count;

    if (result_count == result_capacity) {
      result_capacity *= 2;
      result = realloc(result, result_capacity * sizeof(int32_t));
      assert(result != NULL);
    }
  }

  fclose(file);

  *count = result_count;
  return result;
}

int32_t compare(const void *a, const void *b) {
    int32_t a_value =  *(int32_t*)a;
    int32_t b_value =  *(int32_t*)b;
   return a_value - b_value;

}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Please supply input file\n");
    return 1;
  }

  char *input_path = argv[1];
  uint32_t count;
  int32_t *inputs = read_file(input_path, &count);

  qsort(inputs, count, sizeof(inputs[0]), compare);

  uint32_t i_min = 0;
  uint32_t i_max = count - 1;
  while (i_min != i_max) {
    int32_t sum = inputs[i_min] + inputs[i_max];
    if (sum == 2020) {
      printf("SOLUTION FOUND!\n");
      printf("%i + %i == 2020\n", inputs[i_min], inputs[i_max]);
      printf("%i * %i == %i\n", inputs[i_min], inputs[i_max], inputs[i_min] * inputs[i_max]);
      return 0;
    }
    else if (sum > 2020) {
      --i_max;
    }
    else if (sum < 2020) {
      ++i_min;
    }
  }

  return 0;
}


