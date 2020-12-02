#include "assert.h"
#include "stdio.h"
#include "string.h"
#include "file.h"
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

int32_t parse_number_up_to(char *input, char token, char **found_token_ptr) {
  char *found_token = strchr(input, token);
  assert(found_token != NULL);

  uint32_t num_chars = found_token - input;

  uint32_t dest_size = num_chars + 1;
  char dest[dest_size];
  memset(dest, '\0', dest_size);
  strncpy(dest, input, num_chars);

  *found_token_ptr = found_token;
  return atoi(dest);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Need an input file!\n");
    return 1;
  }

  uint32_t valid_password_count_policy_a = 0;
  uint32_t valid_password_count_policy_b = 0;
  char *input = read_file_as_string(argv[1]);
  char *start = input;
  while (*start != '\0') {
    char *new_start;
    int32_t min_requirement = parse_number_up_to(start, '-', &new_start);
    start = new_start + 1;

    int32_t max_requirement = parse_number_up_to(start, ' ', &new_start);
    start = new_start + 1;

    char letter = *start;
    start = strchr(start, ' ');
    assert(start != NULL);
    start += 1;

    char *end_of_line = strchr(start, '\n');
    assert(end_of_line != NULL);

    uint32_t num_chars = end_of_line - start;
    uint32_t buffer_size = num_chars + 1;
    char buffer[buffer_size];
    memset(buffer, '\0', buffer_size);
    strncpy(buffer, start, num_chars);

    if (is_password_valid_for_policy_a(buffer, min_requirement, max_requirement, letter)) {
      ++valid_password_count_policy_a;
    }

    if (is_password_valid_for_policy_b(buffer, min_requirement, max_requirement, letter)) {
      ++valid_password_count_policy_b;
    }
    /* printf("%i-%i %c: %s\n", min_requirement, max_requirement, letter, buffer); */
    start = end_of_line + 1;
  }

  printf("Exercise 1 answer: %i\n", valid_password_count_policy_a);
  printf("Exercise 2 answer: %i\n", valid_password_count_policy_b);
  free(input);

  return 0;
}
