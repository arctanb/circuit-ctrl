#ifndef _LOGGER_H
#define _LOGGER_H

#define LOGGER_MAX_VARS 8
#define LOGGER_NAME_MAX 16

enum logger_type {
  LOGGER_TYPE_INT,
  LOGGER_TYPE_DBL,
};

struct logger {

  // configuration

  double dt;

  // stored vars

  struct logged_var {
    char name[LOGGER_NAME_MAX + 1];
    char symbol;
    enum logger_type type;
    void *var;
    int mult;
  } logged_vars[LOGGER_MAX_VARS];

  int num_vars;

  // state

  int tick;

};

void logger_init(struct logger *logger, double dt);
void logger_tick(struct logger *logger);
void logger_add_var(struct logger *logger, char name[LOGGER_NAME_MAX + 1],
    enum logger_type type, int mult, void *var);
void logger_log_vars(struct logger *logger);

#endif

