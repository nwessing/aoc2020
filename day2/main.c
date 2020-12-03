#include "assert.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"

bool is_password_valid_for_policy_a(char *password, int32_t min, int32_t max, char letter) {
  uint32_t count = 0;
  while (*password != '\0') {
    if (*password == letter) {
      ++count;
      if (count > max) {
        return false;
      }
    }
    ++password;
  }

  return count >= min;
}

bool is_password_valid_for_policy_b(char *password, int32_t position_1, int32_t position_2, char letter) {
  uint32_t length = strlen(password);
  int32_t i_a = position_1 - 1;
  int32_t i_b = position_2 - 1;
  if (i_a < 0 || i_b < 0 || i_a >= length || i_b >= length) {
    return false;
  }

  uint32_t matches = 0;
  if (password[i_a] == letter) { ++matches; }
  if (password[i_b] == letter) { ++matches; }
  return matches == 1;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Need an input file!\n");
    return 1;
  }

  uint32_t valid_password_count_policy_a = 0;
  uint32_t valid_password_count_policy_b = 0;

  char *filename = argv[1];
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    printf("could not load file %s\n", filename);
    return 1;
  }

  uint32_t line_buffer_size = 256;
  char line_buffer[line_buffer_size];
  while (fgets(line_buffer, line_buffer_size, file)) {
    int32_t min_requirement, max_requirement;
    char letter;
    uint32_t password_size = 256;
    char password[password_size];
    int32_t scan_result = sscanf(line_buffer, "%i-%i %c: %256s\n", &min_requirement, &max_requirement, &letter, password);
    assert(scan_result == 4);
    /* printf("%i-%i %c: %s\n", min_requirement, max_requirement, letter, password); */

    if (is_password_valid_for_policy_a(password, min_requirement, max_requirement, letter)) {
      ++valid_password_count_policy_a;
    }

    if (is_password_valid_for_policy_b(password, min_requirement, max_requirement, letter)) {
      ++valid_password_count_policy_b;
    }
  }

  fclose(file);

  printf("Exercise 1 answer: %i\n", valid_password_count_policy_a);
  printf("Exercise 2 answer: %i\n", valid_password_count_policy_b);

  return 0;
}
