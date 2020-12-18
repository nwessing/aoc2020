#include "aoc_types.h"
#include "assert.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "ctype.h"

#define BUFFER_SIZE 1024

typedef enum ParseMode {
  Rules,
  MyTicket,
  NearbyTickets
} ParseMode;

typedef struct Range {
  i32 min;
  i32 max;
} Range;

typedef struct Rule {
  char *name;
  Range ranges[2];
} Rule;

#define CAPACITY_VALUES_ON_TICKET 20

typedef struct Ticket {
  i32 values[CAPACITY_VALUES_ON_TICKET];
  i32 length;
  bool valid;
} Ticket;

void print_ticket(Ticket ticket) {
  for (i32 i = 0; i < ticket.length; ++i) {
    printf("%i,", ticket.values[i]);
  }
  printf("\n");
}

void print_rule(Rule rule) {
  printf("%s: %i-%i or %i-%i\n",
    rule.name,
    rule.ranges[0].min,
    rule.ranges[0].max,
    rule.ranges[1].min,
    rule.ranges[1].max
  );
}

u32 mark_invalid_tickets(Rule *rules, i32 rules_length, Ticket *nearby_tickets, i32 nearby_tickets_length) {
  u32 error_rate = 0;
  for (u32 i_ticket = 0; i_ticket < nearby_tickets_length; ++i_ticket) {
    Ticket *ticket = &nearby_tickets[i_ticket];
    ticket->valid = true;
    for (u32 i_value = 0; i_value < ticket->length; ++i_value) {
      bool passed_rule = false;
      i32 value = ticket->values[i_value];
      for (u32 i_rule = 0; i_rule < rules_length; ++i_rule) {
        Rule rule = rules[i_rule];
        for (u32 i_range = 0; i_range < 2; ++i_range) {
          Range range = rule.ranges[i_range];
          if (value >= range.min && value <= range.max) {
            passed_rule = true;
            break;
          }
        }
      }

      if (!passed_rule) {
        error_rate += value;
        ticket->valid = false;
        break;
      }
    }
  }

  return error_rate;
}

i64 match_rules(Rule *rules, i32 rules_length, Ticket *tickets, i32 tickets_length, Ticket *my_ticket) {
  // each row is a rule, column is value
  bool *value_rule_matrix = calloc(CAPACITY_VALUES_ON_TICKET * rules_length, sizeof(bool));
  for (u32 i_rule = 0; i_rule < rules_length; ++i_rule) {

    Rule rule = rules[i_rule];
    for (u32 i_value = 0; i_value < CAPACITY_VALUES_ON_TICKET; ++i_value) {
      bool value_matches_data = true;
      for (u32 i_ticket = 0; i_ticket < tickets_length; ++i_ticket) {
        Ticket ticket = tickets[i_ticket];
        if (!ticket.valid) {
          continue;
        }

        i32 value = ticket.values[i_value];
        bool matches_a_range = false;
        for (u32 i_range = 0; i_range < 2; ++i_range) {
          Range range = rule.ranges[i_range];
          if (value <= range.max && value >= range.min) {
            matches_a_range = true;
            break;
          }
        }

        if (!matches_a_range) {
          value_matches_data = false;
          break;
        }
      }

      value_rule_matrix[(i_rule * CAPACITY_VALUES_ON_TICKET) + i_value] = value_matches_data;
    }
  }

  i32 num_solved_rules = 0;
  i32 *rule_to_value = calloc(rules_length, sizeof(i32));
  for (u32 i = 0; i < rules_length; ++i) {
    rule_to_value[i] = -1;
  }

  while(num_solved_rules < rules_length) {
    for (u32 i_rule = 0; i_rule < rules_length; ++i_rule) {

      u32 match_count = 0, i_matching_value = 0;
      for (u32 i_value = 0; i_value < CAPACITY_VALUES_ON_TICKET; ++i_value) {
        if (value_rule_matrix[(i_rule * CAPACITY_VALUES_ON_TICKET) + i_value]) {
          ++match_count;
          i_matching_value = i_value;
        }
      }

      if (rule_to_value[i_rule] < 0 && match_count == 1) {
        rule_to_value[i_rule] = i_matching_value;
        ++num_solved_rules;
        for (u32 i_other_rule = 0; i_other_rule < rules_length; ++i_other_rule) {
          if (i_other_rule == i_rule) { continue; }
          value_rule_matrix[(i_other_rule * CAPACITY_VALUES_ON_TICKET) + i_matching_value] = false;
        }
      }

    }
  }

  i64 result = 1;
  for (i32 i = 0; i < rules_length; ++i) {
    if (strstr(rules[i].name, "departure") == rules[i].name) {
      i32 i_value = rule_to_value[i];
      result *= my_ticket->values[i_value];
    }
  }

  free(rule_to_value);
  free(value_rule_matrix);

  return result;
}

void parse_ticket(Ticket *ticket, char *buffer) {
  char *lasts;
  char *token = strtok_r(buffer, ",", &lasts);
  while (token != NULL) {
    i32 value = atoi(token);
    assert(ticket->length < CAPACITY_VALUES_ON_TICKET);
    ticket->values[ticket->length] = value;
    ++ticket->length;

    token = strtok_r(NULL, ",", &lasts);
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

  u32 rules_capacity = 64, rules_length = 0;
  Rule *rules = calloc(rules_capacity, sizeof(rules[0]));

  Ticket my_ticket = {0};
  u32 nearby_tickets_capacity = 255, nearby_tickets_length = 0;
  Ticket *nearby_tickets = calloc(nearby_tickets_capacity, sizeof(nearby_tickets[0]));

  ParseMode mode = Rules;

  char buffer[BUFFER_SIZE];
  while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
    if (strcmp(buffer, "\n") == 0) {
      continue;
    }

    if (strcmp(buffer, "your ticket:\n") == 0) {
      mode = MyTicket;
      continue;
    }

    if (strcmp(buffer, "nearby tickets:\n") == 0) {
      mode = NearbyTickets;
      continue;
    }

    if (mode == Rules) {
      char rule_name[256];
      Rule rule = {0};
      i32 scan_result = sscanf(buffer, "%256[a-z ]: %d-%d or %d-%d",
          rule_name,
          &rule.ranges[0].min,
          &rule.ranges[0].max,
          &rule.ranges[1].min,
          &rule.ranges[1].max);

      rule.name = strdup(rule_name);
      resize_if_full((void**)&rules, rules_length, &rules_capacity, sizeof(rules[0]));
      rules[rules_length] = rule;
      /* print_rule(rules[rules_length]); */
      ++rules_length;
      assert(scan_result == 5);
    } else if (mode == MyTicket) {
      parse_ticket(&my_ticket, buffer);
      /* print_ticket(my_ticket); */
    } else if (mode == NearbyTickets) {
      resize_if_full((void**) &nearby_tickets, nearby_tickets_length, &nearby_tickets_capacity, sizeof(nearby_tickets[0]));
      parse_ticket(&nearby_tickets[nearby_tickets_length], buffer);
      ++nearby_tickets_length;
      /* print_ticket(nearby_tickets[nearby_tickets_length - 1]); */
    }
  }

  u32 error_rate = mark_invalid_tickets(rules, rules_length, nearby_tickets, nearby_tickets_length);
  printf("Exercise 1: %i\n", error_rate);

  i64 exercise2 = match_rules(rules, rules_length, nearby_tickets, nearby_tickets_length, &my_ticket);
  printf("Exercise 2: %lli\n", exercise2);

  free(rules);
  free(nearby_tickets);
  fclose(file);

  return 0;
}
