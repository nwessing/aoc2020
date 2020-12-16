#include "aoc_types.h"
#include "assert.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "ctype.h"

#define BUFFER_SIZE 1024

typedef struct Address {
  u64 address;
  u64 value;
} Address;

void parse_mask(char *mask, u64 *out_or_mask, u64 *out_and_mask) {
  u64 or_mask = 0;
  u64 and_mask = UINT32_MAX;
  u32 i_mask = 0;
  while (mask[i_mask] != '\0') {
    or_mask = or_mask << 1;
    and_mask = and_mask << 1;
    and_mask |= 1;
    if (mask[i_mask] == '1') {
      or_mask |= 1;
    } else if (mask[i_mask] == '0') {
      and_mask &= UINT32_MAX - 1;
    }
    ++i_mask;
  }

  *out_or_mask = or_mask;
  *out_and_mask = and_mask;
}

Address *find_address(Address *addresses, u32 length, u64 address) {
  for (u32 i = 0; i < length; ++i) {
    if (addresses[i].address == address) {
      return &addresses[i];
    }
  }

  return NULL;
}

u64 find_sum(Address *addresses, u32 length) {
  u64 result = 0;
  for (u32 i = 0; i < length; ++i) {
    result += addresses[i].value;
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


  u32 capacity = 1024;
  u32 length = 0;
  Address *addresses = calloc(capacity, sizeof(addresses[0]));

  u64 or_mask = 0, and_mask = 0;
  char buffer[BUFFER_SIZE];
  while(fgets(buffer, BUFFER_SIZE, file) != NULL) {
    if (strstr(buffer, "mask") == buffer) {
      u32 mask_size = 64;
      char mask[mask_size];
      u32 scan_result = sscanf(buffer, "mask = %64s\n", mask);
      assert(scan_result == 1);
      parse_mask(mask, &or_mask, &and_mask);

      printf("OR %llx\n", or_mask);
      printf("AND %llx\n", and_mask);
    } else if (strstr(buffer, "mem") == buffer) {
      u64 address, value;
      u32 scan_result = sscanf(buffer, "mem[%llu] = %llu\n", &address, &value);
      assert(scan_result == 2);
      printf("mem[%llu] = %llu\n", address, value);

      Address *mem_address = find_address(addresses, length, address);
      if (mem_address == NULL) {
        resize_if_full((void**) &addresses, length, &capacity, sizeof(addresses[0]));
        addresses[length++] = (Address) {
          .address = address,
          .value = 0
        };
        mem_address = &addresses[length - 1];
      }
      mem_address->value = (value | or_mask) & and_mask;

    } else {
      assert(false);
    }
  }


  /* printf("OR %x\n", or_mask); */
  /* printf("AND %x\n", and_mask); */

  printf("Exercise 1: %llu\n", find_sum(addresses, length));

  fclose(file);

  return 0;
}
