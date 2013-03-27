#include "logger.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

static char char_options[] = {'!', '@', '#', '*', '.', ',', '+'};

/*************************** Static Prototypes ***************************/

static void print_val_hdr(struct logged_var *logged_var);
static void print_val(struct logged_var *logged_var);

static void print_binary(int number);
static void print_int(char symbol, int val);

/*************************** Public Functions ***************************/

void logger_init(struct logger *logger, double dt) {
  assert(sizeof(char_options) == LOGGER_MAX_VARS);
  logger->num_vars = 0;
  logger->dt = dt;
  logger->tick = 0;
  printf("$timescale %d ns $end\n", (int)round(dt / 1E-9));
}

void logger_tick(struct logger *logger) {
  printf("#%d\n", logger->tick++);
  for (int i = 0; i < logger->num_vars; ++i) {
    print_val(&logger->logged_vars[i]);
  }
}

void logger_add_var(struct logger *logger, char name[LOGGER_NAME_MAX + 1],
    enum logger_type type, void *var) {

  assert(logger->num_vars < LOGGER_MAX_VARS);
  assert(strlen(name) <= LOGGER_NAME_MAX);

  char symbol = char_options[logger->num_vars];
  struct logged_var *logged_var = &logger->logged_vars[logger->num_vars++];
  logged_var->symbol = symbol;
  logged_var->type = type;
  logged_var->var = var;
  strcpy(logged_var->name, name);

  print_val_hdr(logged_var);
}

/*************************** Static Functions ***************************/

static void print_val_hdr(struct logged_var *logged_var) {
  printf("$var wire 32 %c %s $end\n", logged_var->symbol, logged_var->name);
}

static void print_val(struct logged_var *logged_var) {
  int val = -1;
  if (logged_var->type == LOGGER_TYPE_INT) {
    val = *(int *)logged_var->var;
  } else if (logged_var->type == LOGGER_TYPE_DBL) {
    val = (int)(*(double *)logged_var->var);
  }
  print_int(logged_var->symbol, val);
}

/*************************** Static Util Functions ***************************/

static void print_binary(int number) {
  int remainder;

  if (number <= 1) {
    printf("%d", number);
    return;
  }

  remainder = number % 2;
  print_binary(number >> 1);
  printf("%d", remainder);
}

static void print_int(char symbol, int val) {
  printf("b");
  print_binary(val);
  printf(" %c\n", symbol);
}

