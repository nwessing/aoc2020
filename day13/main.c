#include "aoc_types.h"
#include "assert.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "ctype.h"

#define BUFFER_SIZE 1024

typedef struct Bus {
  i64 bus_id;
  i64 after_t;
} Bus;


Bus find_lcm(i64 a, i64 a_offset, i64 b, i64 b_offset) {
  i64 a_counter = 0;
  i64 b_counter = 0;
  /* if (a < b) { */
  /*   a = a * (b / a); */
  /* } */
  /* if (b < a) { */
  /*   b = b * (a / b); */
  /* } */
  /* i64 a_step = step > 0 ? step : a; */
  /* i64 b_step = step > 0 ? step : b; */

  /* printf("s %lli, a %lli, b %lli\n", step, a_step, b_step); */

  /* i64 step = a > b ? a : b; */
  i64 first_multiple = 0;
  while (1) {
    printf("a %lli, b %lli\n", a_counter, b_counter);
    /* if ((a_counter - a_offset) % a == 0 && (b_counter - b_offset) % b == 0 && a_counter == b_counter) { */
    if (a_counter - a_offset == b_counter - b_offset) {
      /* if (operation) { */
      /*   return a_counter; */
      /* } */

      if (first_multiple == 0) {
        first_multiple = a_counter;
      } else {
        return (Bus) {
          .bus_id = a_counter - first_multiple,
          .after_t =  first_multiple
        };
        /* return a_counter - first_multiple; */
      }
    }

    if (a_counter < b_counter) {
      a_counter += a;
    } else {
      b_counter += b;
    }
  }

}


i64 find_least_common_multiple(Bus *buses, u32 length) {
  /* u64 *multiples = calloc(length, sizeof(multiples[0])); */
  /* u64 cur_mul = 0; */
  /* for (u32 i = 0; i < length; ++i) { */
  /*   multiples[i] = buses[i].bus_id - buses[i].after_t; */
  /*   if (multiples[i] > cur_mul) { */
  /*     cur_mul = multiples[i]; */
  /*   } */
  /* } */

  /* i32 step = buses[0].bus_id > buses[1].bus_id ? buses[0].bus_id : buses[1].bus_id; */
  /* i64 step = 0; */

  i64 result = 1, multiple = 1;
  for (u32 i = 0; i < length; ++i) {
    Bus bus = buses[i];
    while ((result + bus.after_t) % bus.bus_id != 0) {
      result += multiple;
    }
    multiple *= bus.bus_id;
  }
  return result;

    /*    let answer2 = { */
    /*     let mut result = 1; */
    /*     let mut mode = 1; */
    /*     for (offset, bus_id) in buses { */
    /*         while (result + offset as u64) % bus_id != 0 { */
    /*             result += mode; */
    /*         } */
    /*         mode *= bus_id; */
    /*     } */
    /*     result */
    /* }; */


  Bus cumulative_bus = buses[0];
  for (u32 i = 1; i < length; ++i) {
    printf("mul %lli, t %lli\n", cumulative_bus.bus_id, cumulative_bus.after_t);
    cumulative_bus = find_lcm(cumulative_bus.bus_id, 0, buses[i].bus_id, buses[i].after_t);
    assert(i < 2);
    /* step = find_lcm(buses[i].bus_id, buses[i].after_t, buses[i + 1].bus_id, buses[i + 1].after_t, step, false); */



    /* while (multiples[i] != multiples[i + 1]) { */
    /*   if (multiples[i] < multiples[i + 1]) { */
    /*     multiples[i] += step; */
    /*   } else { */
    /*     multiples[i + 1] += step; */
    /*   } */
    /* } */
  }
  printf("mul %lli, t %lli\n", cumulative_bus.bus_id, cumulative_bus.after_t);



  /* bool all_equal = false; */
  /* while (!all_equal) { */
  /*   all_equal = true; */
  /*   for (u32 i = 0; i < length; ++i) { */
  /*     while (multiples[i] < cur_mul) { */
  /*       multiples[i] += buses[i].bus_id; */
  /*     } */

  /*     if (multiples[i] != cur_mul) { */
  /*       all_equal = false; */
  /*     } */

  /*     if (multiples[i] > cur_mul) { */
  /*       cur_mul = multiples[i]; */
  /*     } */
  /*   } */
  /* } */

  /* free(multiples); */
  return cumulative_bus.after_t;
}

i64 find_base_multiple(Bus *buses, u32 length) {
  Bus longest_gap = {0};
  for (u32 i = 0; i < length; ++i) {
    Bus bus = buses[i];
    if (bus.bus_id > longest_gap.bus_id) {
      longest_gap = bus;
    }
  }

  /* u64 lcm = find_least_common_multiple(buses, length); */

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

  /* printf("LCM %lli\n", find_lcm(7, 0, 13, 1)); */
  /* i64 gcf = greatest_common_factor(182664, 154875); */
  /* printf("GCF: %lli\n", gcf); */

  /* i32 ex2_timestamp = find_timestamp(buses, length); */
  printf("LCM: %lli\n", find_least_common_multiple(buses, length));
  /* printf("Exercise 2: %i\n", ex2_timestamp); */

  fclose(file);

  return 0;
}
