#include "aoc_types.h"
#include "assert.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "ctype.h"

#define BUFFER_SIZE 1024

typedef struct Bus {
  i32 bus_id;
  i32 after_t;
} Bus;


i64 find_base_multiple(Bus *buses, u32 length) {
  Bus longest_gap = {0};
  for (u32 i = 0; i < length; ++i) {
    Bus bus = buses[i];
    if (bus.bus_id > longest_gap.bus_id) {
      longest_gap = bus;
    }
  }


  /* for (i64 i = longest_gap.bus_id - longest_gap.after_t; 1; i += longest_gap.bus_id) { */

  /* } */


  for (i64 i = longest_gap.bus_id - longest_gap.after_t; 1; i += longest_gap.bus_id) {
    bool found = true;
    for (i32 i_bus = 0; i_bus < length; ++i_bus) {
      Bus bus = buses[i_bus];
      if ((bus.after_t + i) % bus.bus_id != 0) {
        found = false;
        break;
      }
    }


    /* if (found || i > 1068781) { */
    /*   for (i32 i_bus = 0; i_bus < length; ++i_bus) { */
    /*     Bus bus = buses[i_bus]; */
    /*     [> if (bus.bus_id + bus.after_t % i != 0) { <] */
    /*       printf("%lli + %i mod %i = %lli\n", i, bus.after_t, bus.bus_id, i % (bus.bus_id + bus.after_t)); */
    /*       [> break; <] */
    /*     [> } <] */
    /*   } */
    /*   if (!found) { */
    /*     assert(false); */
    /*   } */
    /* } */

    if (found) {
      return i;
    }

    /* if (i % 1000 == 0) { */
    /*   printf("%lli\n", i); */
    /* } */
  }

}

i64 find_timestamp(Bus *buses, u32 length) {
  i64 base_multiple = find_base_multiple(buses, length);
  return base_multiple;
  /* for (i64 i = 0; 1; i += base_multiple) { */
  /*   bool found = true; */
  /*   for (i32 i_bus = 0; i_bus < length; ++i_bus) { */
  /*     Bus bus = buses[i_bus]; */
  /*     if (bus.bus_id % (i - bus.bus_id) != bus.after_t) { */
  /*       found = false; */
  /*       break; */
  /*     } */
  /*   } */

  /*   if (found) { */
  /*     return i; */
  /*   } */
  /*   if (i % 1000 == 0) { */
  /*     printf("%lli\n", i); */
  /*   } */
  /* } */
}


void find_next_bus(Bus *buses, u32 length, i32 now, i32 *bus_id, i32 *minutes_from_now) {
  i32 smallest_gap = INT32_MAX;
  i32 best_bus_id = 0;
  for (u32 i = 0; i < length; ++i) {
    Bus bus = buses[i];
    i32 difference = now % bus.bus_id;
    difference = bus.bus_id - difference;
    if (difference < smallest_gap) {
      smallest_gap = difference;
      best_bus_id = bus.bus_id;
    }
  }
  *minutes_from_now = smallest_gap;
  *bus_id = best_bus_id;
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
  fgets(buffer, BUFFER_SIZE, file);
  i32 timestamp = atoi(buffer);

  fgets(buffer, BUFFER_SIZE, file);
  char *lasts;
  char *token = strtok_r(buffer, ",", &lasts);

  u32 capacity = 1024;
  u32 length = 0;
  Bus *buses = calloc(capacity, sizeof(buses[0]));

  i32 after_t = 0;
  while (token != NULL) {
    if (strcmp(token, "x") != 0) {
      i32 bus_id = atoi(token);
      resize_if_full((void**) &buses, length, &capacity, sizeof(buses[0]));
      buses[length++] = (Bus) {
        .bus_id = bus_id,
        .after_t = after_t
      };
    }

    token = strtok_r(NULL, ",", &lasts);
    ++after_t;
  }

  i32 smallest_gap = 0, best_bus_id = 0;
  find_next_bus(buses, length, timestamp, &best_bus_id, &smallest_gap);
  printf("Exercise 1: %i\n", best_bus_id * smallest_gap);

  i32 ex2_timestamp = find_timestamp(buses, length);
  printf("Exercise 2: %i\n", ex2_timestamp);

  fclose(file);

  return 0;
}
