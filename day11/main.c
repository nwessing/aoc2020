#include "aoc_types.h"
#include "assert.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "ctype.h"

typedef enum Tile {
  TILE_FLOOR,
  TILE_EMPTY,
  TILE_FULL
} Tile;

u32 count_occupied_seats(Tile *tiles, u32 length) {
  u32 result = 0;
  for (u32 i = 0; i < length; ++i) {
    if (tiles[i] == TILE_FULL) {
      ++result;
    }
  }

  return result;
}

i32 adjacent_seat_counter(Tile *tiles, u32 width, u32 height, i32 x, i32 y) {
  i32 adjacent_seat_count = 0;
  for (i32 dx = -1; dx < 2; ++dx) {
    for (i32 dy = -1; dy < 2; ++dy) {
      i32 x2 = x + dx;
      i32 y2 = y + dy;

      if (x2 < 0 || x2 >= width || y2 < 0 || y2 >= height || (dx == 0 && dy == 0)) {
        continue;
      }

      if (tiles[(y2 * width) + x2] == TILE_FULL) {
        ++adjacent_seat_count;
      }
    }
  }

  return adjacent_seat_count;
}

i32 line_of_sight_seat_counter(Tile *tiles, u32 width, u32 height, i32 x, i32 y) {
  i32 result = 0;

  for (i32 dx = -1; dx < 2; ++dx) {
    for (i32 dy = -1; dy < 2; ++dy) {


      i32 x2 = x + dx;
      i32 y2 = y + dy;

      while (1) {

        if (x2 < 0 || x2 >= width || y2 < 0 || y2 >= height || (dx == 0 && dy == 0)) {
          break;
        }


        Tile encountered = tiles[(y2 * width) + x2];
        if (encountered != TILE_FLOOR) {
          if (encountered == TILE_FULL) {
            ++result;
          }
          break;
        }

        x2 += dx;
        y2 += dy;
      }
    }
  }

  return result;
}

u32 tick(Tile *current, Tile *next, u32 width, u32 height, i32 (counter)(Tile*, u32, u32, i32, i32), u32 occupation_threshold) {
  u32 length = width * height;
  u32 change_count = 0;
  for (u32 i = 0; i < length; ++i) {
    i32 x = i % width;
    i32 y = i / width;
    Tile tile = current[i];
    if (tile == TILE_FLOOR) {
      next[i] = TILE_FLOOR;
      continue;
    }

    i32 adjacent_seat_count = counter(current, width, height, x, y);

    if (tile == TILE_EMPTY && adjacent_seat_count == 0) {
      next[i] = TILE_FULL;
      ++change_count;
    } else if (tile == TILE_FULL && adjacent_seat_count >= occupation_threshold) {
      next[i] = TILE_EMPTY;
      ++change_count;
    } else {
      next[i] = tile;
    }
  }

  return change_count;
}


int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Need an input file!\n");
    return 1;
  }

  u32 capacity = 1024;
  u32 length = 0;
  u32 width = 0, height = 0;
  Tile *tiles = calloc(capacity, sizeof(Tile));

  char *filename = argv[1];
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    printf("could not load file %s\n", filename);
    return 1;
  }

  i32 character;
  while ((character = fgetc(file)) != EOF) {
    resize_if_full((void **) &tiles, length, &capacity, sizeof(tiles[0]));
    switch (character) {
      case '.':
        tiles[length++] = TILE_FLOOR;
        break;
      case 'L':
        tiles[length++] = TILE_EMPTY;
        break;
      case '#':
        tiles[length++] = TILE_FULL;
        break;
      case '\n':
        if (width == 0) {
          width = length;
        } else {
          assert(length % width == 0);
        }
        break;
      default:
        assert(false);
    }
  }
  height = length / width;

  Tile *original_state = calloc(length, sizeof(original_state[0]));
  memcpy(original_state, tiles, length * sizeof(original_state[0]));

  Tile *next_state = calloc(length, sizeof(next_state[0]));

  while (tick(tiles, next_state, width, height, &adjacent_seat_counter, 4) != 0) {
    Tile *temp = tiles;
    tiles = next_state;
    next_state = temp;
  }

  printf("Exercise 1: %i\n", count_occupied_seats(next_state, length));

  memcpy(tiles, original_state, length * sizeof(original_state[0]));
  while (tick(tiles, next_state, width, height, &line_of_sight_seat_counter, 5) != 0) {
    Tile *temp = tiles;
    tiles = next_state;
    next_state = temp;
  }

  printf("Exercise 2: %i\n", count_occupied_seats(next_state, length));

  free(tiles);
  free(original_state);
  free(next_state);
  fclose(file);

  return 0;
}
