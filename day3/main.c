#include "assert.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"

typedef enum Tile { Tile_Open, Tile_Tree, Tile_End } Tile;
typedef struct Map {
  uint32_t width;
  uint32_t height;
  Tile *tiles;
  uint32_t tiles_count;
  uint32_t tiles_capacity;
} Map;

#define INITIAL_TILES_CAPACITY 1024

Map read_map(char *filename) {
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    printf("could not load file %s\n", filename);
    assert(false);
  }

  Map map = {
    .width = 0,
    .height = 1,
    .tiles = calloc(INITIAL_TILES_CAPACITY, sizeof(Tile)),
    .tiles_count = 0,
    .tiles_capacity = INITIAL_TILES_CAPACITY
  };

  int32_t input;
  uint32_t i_tile = 0;
  uint32_t current_width = 0;
  while ((input = getc(file)) != EOF) {
    if (i_tile >= map.tiles_capacity) {
      map.tiles_capacity *= 2;
      map.tiles = realloc(map.tiles, map.tiles_capacity * sizeof(Tile));
    }

    switch (input) {
      case '.':
        map.tiles[i_tile] = Tile_Open;
        ++i_tile;
        ++current_width;
        ++map.tiles_count;
        break;
      case '#':
        map.tiles[i_tile] = Tile_Tree;
        ++i_tile;
        ++current_width;
        ++map.tiles_count;
        break;
      case '\n':
        if (map.height == 1) {
          map.width = current_width;
        }

        if (map.width != current_width) {
          printf("row: %i, map width: %i, current width: %i, i_tile: %i\n", map.height, map.width, current_width, i_tile);
          assert(false);
        }
        current_width = 0;
        ++map.height;
        break;

      default:
        printf("row: %i, map width: %i, current width: %i, i_tile: %i\n", map.height, map.width, current_width, i_tile);
        printf("unexpected char %i\n", input);
        assert(false);
    }
  }

  fclose(file);

  return map;
}

Tile get_tile_at(Map *map, uint32_t position_x, uint32_t position_y) {
  if (position_y >= map->height) {
    return Tile_End;
  }

  position_x = position_x % map->width;
  uint32_t index = (position_y * map->width) + position_x;
  assert(index < map->tiles_count);
  return map->tiles[index];
}

uint32_t get_tree_count_for_direction(Map *map, uint32_t velocity_x, uint32_t velocity_y) {
  uint32_t position_x = 0, position_y = 0;
  uint32_t trees_encountered = 0;

  Tile tile;
  while ((tile = get_tile_at(map, position_x, position_y)) != Tile_End) {
    if (tile == Tile_Tree) {
      ++trees_encountered;
    }

    position_x += velocity_x;
    position_y += velocity_y;
  }

  return trees_encountered;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Need an input file!\n");
    return 1;
  }

  char *filename = argv[1];
  Map map = read_map(filename);

  uint64_t slope1_count = get_tree_count_for_direction(&map, 1, 1);
  uint64_t slope2_count = get_tree_count_for_direction(&map, 3, 1);
  uint64_t slope3_count = get_tree_count_for_direction(&map, 5, 1);
  uint64_t slope4_count = get_tree_count_for_direction(&map, 7, 1);
  uint64_t slope5_count = get_tree_count_for_direction(&map, 1, 2);

  printf("Exercise 1: %lli\n", slope2_count);
  uint64_t exercise_two_answer = slope1_count * slope2_count * slope3_count * slope4_count * slope5_count;
  printf("Exercise 2: %lli\n", exercise_two_answer);

  free(map.tiles);

  return 0;
}
