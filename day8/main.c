#include "aoc_types.h"
#include "assert.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "ctype.h"

#define BUFFER_SIZE 1024

typedef enum Operation {
  OP_ACC,
  OP_JMP,
  OP_NOP
} Operation;

typedef struct Instruction {
  Operation operation;
  i32 argument;
  i32 times_executed;
} Instruction;

typedef struct Computer {
  i32 accumulator;
  u32 pc;
  bool terminated_normally;
} Computer;

Computer run_program(Instruction *instructions, u32 length, i32 swap_index) {
  Computer computer = {0};
  while (1) {
    if (computer.pc >= length) {
      computer.terminated_normally = true;
      break;
    }

    Instruction *ins = &instructions[computer.pc];
    if (ins->times_executed == 1) {
      // infinite loop found
      break;
    }

    Operation operation = ins->operation;
    if (swap_index == computer.pc) {
      if (operation == OP_JMP) {
        operation = OP_NOP;
      } else if (operation == OP_NOP) {
        operation = OP_JMP;
      }
    }

    bool increment_pc = true;
    switch (operation) {
      case OP_ACC:
        computer.accumulator += ins->argument;
        break;
      case OP_JMP:
        increment_pc = false;
        computer.pc += ins->argument;
        break;
      case OP_NOP:
        break;
      default:
        assert(false);
        break;
    }

    ++ins->times_executed;
    if (increment_pc) {
      ++computer.pc;
    }
  }

  return computer;
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

  u32 capacity = 1024;
  u32 length = 0;
  Instruction *instructions = calloc(capacity, sizeof(Instruction));;

  char buffer[BUFFER_SIZE];
  while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
    char *lasts;
    char *operation_str = strtok_r(buffer, " ", &lasts);
    Operation operation;
    if (strcmp(operation_str, "acc") == 0) {
      operation = OP_ACC;
    }
    else if (strcmp(operation_str, "jmp") == 0) {
      operation = OP_JMP;
    }
    else if (strcmp(operation_str, "nop") == 0) {
      operation = OP_NOP;
    }
    else {
      assert(false);
    }

    char *argument_str = strtok_r(NULL, "\n", &lasts);
    i32 argument = atoi(argument_str);

    Instruction instruction = {
      .operation = operation,
      .argument = argument,
      .times_executed = 0
    };

    resize_if_full((void**) &instructions, length, &capacity, sizeof(Instruction));
    instructions[length] = instruction;
    ++length;
  }

  Computer exercise_1 = run_program(instructions, length, -1);
  printf("Exercise 1: %i\n", exercise_1.accumulator);

  Computer exercise_2 = {0};
  for (u32 swap_index = 0; swap_index < length; ++swap_index) {
    for (u32 i = 0; i < length; ++i) {
      instructions[i].times_executed = 0;
    }

    if (instructions[swap_index].operation == OP_ACC) {
      continue;
    }

    exercise_2 = run_program(instructions, length, swap_index);
    if (exercise_2.terminated_normally) {
      break;
    }
  }

  if (exercise_2.terminated_normally) {
    printf("Exercise 2: %i\n", exercise_2.accumulator);
  }

  free(instructions);
  fclose(file);

  return 0;
}
