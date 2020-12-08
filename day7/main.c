#include "aoc_types.h"
#include "assert.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "ctype.h"

#define BUFFER_SIZE 1024

typedef struct BagContent {
  char *name;
  u32 amount;
} BagContent;

typedef struct Bag {
  char *name;
  u32 contains_length;
  BagContent *contains;
} Bag;


typedef struct BagSet {
  char **set;
  u32 capacity;
  u32 length;
} BagSet;

void resize_if_full(void **array, u32 count, u32 *capacity, size_t element_size) {
  if (count != *capacity) {
    return;
  }

  *capacity *= 2;
  *array = realloc(*array, (*capacity) * element_size);
}

bool set_contains(BagSet *set, char *bag_name) {
  for (u32 i = 0; i < set->length; ++i) {
    if (strcmp(set->set[i], bag_name) == 0) {
      return true;
    }
  }

  return false;
}

void set_add(BagSet *set, char *bag_name) {
  if (set_contains(set, bag_name)) {
    return;
  }

  resize_if_full((void**) &set->set, set->length, &set->capacity, sizeof(char*));
  set->set[set->length] = strdup(bag_name);
  ++set->length;
}

void find_bags_containing(BagSet *set, char *target, Bag *bags, u32 bags_count) {
  for (u32 i_bag = 0; i_bag < bags_count; ++i_bag) {
    Bag bag = bags[i_bag];
    for (u32 i_content = 0; i_content < bag.contains_length; ++i_content) {
      char *content = bag.contains[i_content].name;
      if (strcmp(target, content) == 0) {
        set_add(set, bag.name);
      }
    }
  }
}

u64 get_num_bags_inside(Bag *bags, u32 bag_count, char *target) {
  for (u32 i_bag = 0; i_bag < bag_count; ++i_bag) {
    Bag bag = bags[i_bag];
    if (strcmp(bag.name, target) != 0) {
      continue;
    }

    if (bag.contains_length == 0) {
      return 0;
    }

    u64 result = 0;
    for (u32 i_contains = 0; i_contains < bag.contains_length; ++i_contains) {
      BagContent content = bag.contains[i_contains];
      result += content.amount + (content.amount * get_num_bags_inside(bags, bag_count, content.name));
    }

    return result;
  }

  assert(false);
  return 0;
}

u32 get_element_count(char *contains) {
  u32 result = 1;
  while (*contains != '\0') {
    if (*contains == ',') {
      ++result;
    }
    ++contains;
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

  u32 bags_capacity = 1024;
  u32 bags_count = 0;
  Bag *bags = calloc(bags_capacity, sizeof(Bag));

  char buffer[BUFFER_SIZE];
  while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
    char *end_bag_name = strstr(buffer, " contain");
    assert(end_bag_name != NULL);
    resize_if_full((void**) &bags, bags_count, &bags_capacity, sizeof(Bag));

    Bag bag;
    bag.name = strndup(buffer, end_bag_name - buffer - 1); // sub 1 to remove s
    char *contains_start = end_bag_name + strlen(" contain ");
    if (strcmp(contains_start, "no other bags.\n") == 0) {
      bag.contains_length = 0;
      bag.contains = NULL;
    } else {
      bag.contains_length = get_element_count(contains_start);
      bag.contains = calloc(bag.contains_length, sizeof(BagContent));

      u32 i_element = 0;
      char *tok_ptr;
      char *element_start = strtok_r(contains_start, ",.", &tok_ptr);
      while (element_start != NULL && *element_start != '\n') {
        bag.contains[i_element].amount = atoi(element_start);
        while (*element_start != '\0' && !isalpha(*element_start)) {
          ++element_start;
        }

        u32 element_size = strlen(element_start);
        char last_char = element_start[element_size - 1];
        if (last_char == 's') {
          --element_size;
        }

        assert(i_element < bag.contains_length);
        bag.contains[i_element].name = strndup(element_start, element_size);
        ++i_element;
        element_start = strtok_r(NULL, ",.", &tok_ptr);
      }
    }

    bags[bags_count] = bag;
    ++bags_count;
  }

  BagSet set = {
    .capacity = 1024,
    .length = 0,
    .set = calloc(1024, sizeof(char *))
  };

  char *target = "shiny gold bag";
  find_bags_containing(&set, target, bags, bags_count);
  for (u32 i_set = 0; i_set < set.length; ++i_set) {
    find_bags_containing(&set, set.set[i_set], bags, bags_count);
  }

  u64 exercise2_answer = get_num_bags_inside(bags, bags_count, target);

  printf("Exercise 1: %i\n", set.length);
  printf("Exercise 2: %lli\n", exercise2_answer);

  for (u32 i = 0; i < set.length; ++i) {
    free(set.set[i]);
  }
  free(set.set);

  for (u32 i = 0; i < bags_count; ++i) {
    for (u32 i_contains = 0; i_contains < bags[i].contains_length; ++i_contains) {
      free(bags[i].contains[i_contains].name);
    }
    free(bags[i].name);
    free(bags[i].contains);
  }

  free(bags);
  fclose(file);

  return 0;
}
