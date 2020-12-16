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

typedef struct AddressSpace {
  Address *addresses;
  u32 length;
  u32 capacity;
} AddressSpace;

typedef struct Masks {
  u64 and;
  u64 or;
  u64 floating;
} Masks;

Masks parse_mask(char *mask) {
  u64 or_mask = 0;
  u64 and_mask = UINT32_MAX;
  u64 float_mask = 0;
  u32 i_mask = 0;
  while (mask[i_mask] != '\0') {
    or_mask = or_mask << 1;
    and_mask = and_mask << 1;
    and_mask |= 1;
    float_mask = float_mask << 1;
    if (mask[i_mask] == '1') {
      or_mask |= 1;
    } else if (mask[i_mask] == '0') {
      and_mask &= UINT32_MAX - 1;
    } else if (mask[i_mask] == 'X') {
      float_mask |= 1;
    }
    ++i_mask;
  }

  return (Masks) {
    .or = or_mask,
    .and = and_mask,
    .floating = float_mask,
  };
}

Address *find_address(Address *addresses, u32 length, u64 address) {
  for (u32 i = 0; i < length; ++i) {
    if (addresses[i].address == address) {
      return &addresses[i];
    }
  }

  return NULL;
}

void write_value(AddressSpace *space, u64 address, u64 value) {
  Address *mem_address = find_address(space->addresses, space->length, address);
  if (mem_address == NULL) {
    resize_if_full((void**) &space->addresses, space->length, &space->capacity, sizeof(space->addresses[0]));
    space->addresses[space->length++] = (Address) {
      .address = address,
      .value = 0
    };
    mem_address = &space->addresses[space->length - 1];
  }
  mem_address->value = value;
}

void write_through_decoder(AddressSpace *space, u64 address, u64 value, Masks masks) {
  u64 least_set_bit = masks.floating & -((i64)masks.floating);
  if (least_set_bit > 0) {
    Masks next_masks = masks;
    next_masks.floating &= ~least_set_bit;
    write_through_decoder(space, address | least_set_bit, value, next_masks);

    write_through_decoder(space, address & ~least_set_bit, value, next_masks);
  } else {
    write_value(space, address | masks.or, value);
  }
}

void write_value_with_mask(AddressSpace *space, u64 address, u64 value, Masks masks) {
  write_value(space, address, (value & masks.and) | masks.or);
}

u64 find_sum(AddressSpace *space) {
  u64 result = 0;
  for (u32 i = 0; i < space->length; ++i) {
    result += space->addresses[i].value;
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

  AddressSpace space1 = {
    .capacity = 1024,
    .length = 0,
    .addresses = calloc(1024, sizeof(space1.addresses[0]))
  };
  AddressSpace space2 = {
    .capacity = 1024,
    .length = 0,
    .addresses = calloc(1024, sizeof(space2.addresses[0]))
  };

  Masks masks = {0};
  char buffer[BUFFER_SIZE];
  while(fgets(buffer, BUFFER_SIZE, file) != NULL) {
    if (strstr(buffer, "mask") == buffer) {
      u32 mask_size = 64;
      char mask[mask_size];
      u32 scan_result = sscanf(buffer, "mask = %64s\n", mask);
      assert(scan_result == 1);
      masks = parse_mask(mask);
    } else if (strstr(buffer, "mem") == buffer) {
      u64 address, value;
      u32 scan_result = sscanf(buffer, "mem[%llu] = %llu\n", &address, &value);
      assert(scan_result == 2);

      write_value_with_mask(&space1, address, value, masks);
      write_through_decoder(&space2, address, value, masks);

    } else {
      assert(false);
    }
  }

  printf("Exercise 1: %llu\n", find_sum(&space1));
  printf("Exercise 2: %llu\n", find_sum(&space2));

  fclose(file);

  return 0;
}
