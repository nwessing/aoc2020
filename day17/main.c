#include "aoc_types.h"
#include "assert.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "ctype.h"

#define BUFFER_SIZE 1024

typedef struct CubeState {
  bool active;
} CubeState;

typedef struct Space {
  u32 width;
  u32 height;
  u32 depth;
  u32 capacity;
  CubeState *nodes;
  CubeState *next;
} Space;

#define NUM_CYCLES 6

u32 cube_index_at(Space *space, u32 x, u32 y, u32 z) {
  assert(x < space->width);
  assert(y < space->height);
  assert(z < space->depth);
  u32 index = (z * space->width * space->height) + (y * space->width) + x;
  assert(index < space->capacity);
  return index;
}

u32 get_active_neighbor_count(Space *space, u32 x, u32 y, u32 z) {
  u32 result = 0;
  for (i32 z_diff = -1; z_diff <= 1; ++z_diff) {
    for (i32 y_diff = -1; y_diff <= 1; ++y_diff) {
      for (i32 x_diff = -1; x_diff <= 1; ++x_diff) {
        if (x_diff == 0 && y_diff == 0 && z_diff == 0) {
          continue;
        }

        i32 x_neighbor = x + x_diff;
        i32 y_neighbor = y + y_diff;
        i32 z_neighbor = z + z_diff;
        if (x_neighbor < 0 || x_neighbor >= space->width) { continue; }
        if (y_neighbor < 0 || y_neighbor >= space->height) { continue; }
        if (z_neighbor < 0 || z_neighbor >= space->depth) { continue; }

        i32 index = cube_index_at(space, x_neighbor, y_neighbor, z_neighbor);
        if (space->nodes[index].active) {
          ++result;
        }
      }
    }
  }
  return result;
}


u32 count_active_cubes(Space *space) {
  u32 result = 0;
  for (u32 i = 0; i < space->capacity; ++i) {
    if (space->nodes[i].active) {
      ++result;
    }
  }

  return result;
}

void print_state(Space *space, u32 turn) {
  u32 offset = NUM_CYCLES - turn;
  for (u32 z = offset; z < space->depth - offset; ++z) {
    for (u32 y = offset; y < space->height - offset; ++y) {
      for (u32 x = offset; x < space->width - offset; ++x) {
        u32 index = cube_index_at(space, x, y, z);
        if (space->nodes[index].active) {
          printf("#");
        } else {
          printf(".");
        }
      }

      printf("\n");
    }

    printf("\n");
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

  u32 initial_width = 0;
  u32 initial_height = 0;

  int c;
  u32 current_width = 0;
  while ((c = fgetc(file)) != EOF) {
    if (c == '\n') {
      if (initial_width == 0) {
        initial_width = current_width;
      } else {
        assert(initial_width == current_width);
      }

      current_width = 0;
      ++initial_height;
    } else {
      ++current_width;
    }
  }

  u32 initial_depth = 1;
  Space space = {
    .width = initial_width + (NUM_CYCLES * 2),
    .height = initial_height + (NUM_CYCLES * 2),
    .depth = initial_depth + (NUM_CYCLES * 2),
    .capacity = space.width * space.height * space.depth
  };
  space.nodes = calloc(space.capacity, sizeof(CubeState));
  space.next = calloc(space.capacity, sizeof(CubeState));

  u32 y = NUM_CYCLES;

  char buffer[BUFFER_SIZE];
  fseek(file, 0, SEEK_SET);
  while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
    u32 x = NUM_CYCLES;
    u32 i_buffer = 0;
    while (buffer[i_buffer] != '\0') {
      u32 cube_index = cube_index_at(&space, x, y, NUM_CYCLES);
      switch(buffer[i_buffer]) {
        case '.':
          space.nodes[cube_index].active = false;
          break;
        case '#':
          space.nodes[cube_index].active = true;
          break;
        default:
          break;
      }
      ++i_buffer;
      ++x;
    }
    ++y;
  }

  for(u32 turn = 1; turn <= NUM_CYCLES; ++turn) {
    for (u32 z = 0; z < space.depth; ++z) {
      for (u32 y = 0; y < space.height; ++y) {
        for (u32 x = 0; x < space.width; ++x) {
          u32 active_neighbors = get_active_neighbor_count(&space, x, y, z);
          u32 cube_index = cube_index_at(&space, x, y, z);

          if (space.nodes[cube_index].active) {
            space.next[cube_index].active = (active_neighbors == 2 || active_neighbors == 3);
          } else {
            space.next[cube_index].active = active_neighbors == 3;
          }
        }
      }
    }

    CubeState *temp = space.next;
    space.next = space.nodes;
    space.nodes = temp;
  }

  u32 active_count = count_active_cubes(&space);
  printf("Exercise 1: %i\n", active_count);

  free(space.nodes);
  free(space.next);
  fclose(file);

  return 0;
}
