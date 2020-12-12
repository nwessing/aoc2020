#include "aoc_types.h"
#include "assert.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "ctype.h"

#define BUFFER_SIZE 1024

typedef struct Vector {
  i32 x;
  i32 y;
} Vector;

typedef struct Movement {
  Vector direction;
  i32 rotation;
  bool move_forward;
  i32 value;
} Movement;

i32 manhattan_distance(i32 x, i32 y) {
  return abs(x) + abs(y);
}

void ship_movement(Vector *ship, i32 *heading, Movement movement) {
  Vector forward = {0};
  if (movement.move_forward) {
    if (*heading == 0) {
      forward.y = 1;
    } else if (*heading == 90) {
      forward.x = 1;
    } else if (*heading == 180) {
      forward.y = -1;
    } else if (*heading == 270) {
      forward.x = -1;
    } else {
      assert(false);
    }
  }
  ship->x += (movement.direction.x + forward.x) * movement.value;
  ship->y += (movement.direction.y + forward.y) * movement.value;

  (*heading) += movement.rotation;
  while (*heading < 0) {
    (*heading) += 360;
  }
  *heading = *heading % 360;
}

void waypoint_movement(Vector *ship, Vector *waypoint, Movement movement) {
  if (movement.move_forward) {
    ship->x += waypoint->x * movement.value;
    ship->y += waypoint->y * movement.value;
  }

  waypoint->x += movement.direction.x * movement.value;
  waypoint->y += movement.direction.y * movement.value;

  i32 x_negate = movement.rotation > 0 ? 1 : -1;
  i32 y_negate = movement.rotation > 0 ? -1 : 1;
  while (movement.rotation != 0) {
    i32 temp = waypoint->x;
    waypoint->x = x_negate * waypoint->y;
    waypoint->y = y_negate * temp;
    if (movement.rotation < 0) {
      movement.rotation += 90;
    } else {
      movement.rotation -= 90;
    }
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

  Vector ex1 = {0};
  Vector ex2 = {0};
  Vector waypoint = { .x = 10, .y = -1 };
  i32 heading = 90 ;
  char buffer[BUFFER_SIZE];
  while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
    Movement movement = {0};
    i32 rotate_dir = 0;
    switch (buffer[0]) {
      case 'F':
        movement.move_forward = true;
        break;
      case 'R':
        rotate_dir = -1;
        break;
      case 'L':
        rotate_dir = 1;
        break;
      case 'N':
        movement.direction.y = -1;
        break;
      case 'S':
        movement.direction.y = 1;
        break;
      case 'E':
        movement.direction.x = 1;
        break;
      case 'W':
        movement.direction.x = -1;
        break;
      default:
        assert(false);
    }

    movement.value = atoi(buffer + 1);
    movement.rotation = rotate_dir * movement.value;

    ship_movement(&ex1, &heading, movement);
    waypoint_movement(&ex2, &waypoint, movement);
  }

  printf("Exercise 1: %i\n", manhattan_distance(ex1.x, ex1.y));
  printf("Exercise 2: %i\n", manhattan_distance(ex2.x, ex2.y));

  fclose(file);

  return 0;
}
