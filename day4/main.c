#include "assert.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "ctype.h"
#include "stdbool.h"

typedef struct Passport {
  bool birth_year;
  bool birth_year_valid;
  bool issue_year;
  bool issue_year_valid;
  bool expiration_year;
  bool expiration_year_valid;
  bool height;
  bool height_valid;
  bool hair_color;
  bool hair_color_valid;
  bool eye_color;
  bool eye_color_valid;
  bool passport_id;
  bool passport_id_valid;
  bool country_id;
  bool country_id_valid;
} Passport;

#define BUFFER_SIZE 1024

bool valid_int(char *input, int32_t min, int32_t max) {
  int32_t value = atoi(input);
  return value >= min && value <= max;
}

bool valid_hair_color(char *input) {
  if (*input != '#') {
    return false;
  }

  ++input;

  uint32_t len = 0;
  while (*input != '\0') {
    ++len;
    if (len > 6 || !isxdigit(*input)) {
      return false;
    }
    ++input;
  }

  return len == 6;
}

char *eye_colors[] = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
#define NUM_EYE_COLORS (sizeof(eye_colors) / sizeof(eye_colors[0]))

bool valid_eye_color(char *input) {
  for (uint32_t i = 0; i < NUM_EYE_COLORS; ++i) {
    if (strcmp(input, eye_colors[i]) == 0) {
      return true;
    }
  }

  return false;
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
  uint32_t valid_passport_count = 0, present_passport_count = 0;

  Passport passport = {0};
  while (1) {
    char *fgets_result = fgets(buffer, BUFFER_SIZE, file);
    if (fgets_result == NULL || strcmp(buffer, "\n") == 0) {
      if (passport.birth_year &&
          passport.expiration_year && passport.eye_color &&
          passport.hair_color && passport.height &&
          passport.issue_year && passport.passport_id) {
          ++present_passport_count;

        if (passport.birth_year_valid &&
          passport.expiration_year_valid && passport.eye_color_valid &&
          passport.hair_color_valid && passport.height_valid &&
          passport.issue_year_valid && passport.passport_id_valid) {
          ++valid_passport_count;
        }
      }

      passport = (Passport) {0};

      if (fgets_result == NULL) {
        break;
      }
    }
    else {
      char *tokenize = buffer, *token, *saveptr;
      while ((token = strtok_r(tokenize, " \n", &saveptr)) != NULL) {
        tokenize = NULL;

        if (strstr(token, "byr:") == token) {
          passport.birth_year = true;
          passport.birth_year_valid = valid_int(token + 4, 1920, 2002);
        }
        else if (strstr(token, "iyr:") == token) {
          passport.issue_year = true;
          passport.issue_year_valid = valid_int(token + 4, 2010, 2020);
        }
        else if (strstr(token, "eyr:") == token) {
          passport.expiration_year = true;
          passport.expiration_year_valid = valid_int(token + 4, 2020, 2030);
        }
        else if (strstr(token, "hgt:") == token) {
          passport.height = true;
          uint32_t token_length = strlen(token);
          if (strstr(token, "cm") == (token + token_length - 2)) {
            passport.height_valid = valid_int(token + 4, 150, 193) ;
          }
          if (strstr(token, "in") == (token + token_length - 2)) {
            passport.height_valid = valid_int(token + 4, 59, 76) ;
          }
        }
        else if (strstr(token, "hcl:") == token) {
          passport.hair_color = true;
          passport.hair_color_valid = valid_hair_color(token + 4);
        }
        else if (strstr(token, "ecl:") == token) {
          passport.eye_color = true;
          passport.eye_color_valid = valid_eye_color(token + 4);
        }
        else if (strstr(token, "pid:") == token) {
          passport.passport_id = true;
          passport.passport_id_valid = strlen(token + 4) == 9 && valid_int(token + 4, 0, 999999999);
        }
        else if (strstr(token, "cid:") == token) {
          passport.country_id = true;
          passport.country_id_valid = true;
        }
      }
    }
  }

  printf("Exercise 1: %i\n", present_passport_count);
  printf("Exercise 2: %i\n", valid_passport_count);

  fclose(file);

  return 0;
}
