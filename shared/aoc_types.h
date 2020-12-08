#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"

typedef uint32_t u32;
typedef uint64_t u64;

typedef int32_t i32;
typedef int64_t i64;

void resize_if_full(void **array, u32 count, u32 *capacity, size_t element_size) {
  if (count != *capacity) {
    return;
  }

  *capacity *= 2;
  *array = realloc(*array, (*capacity) * element_size);
}
