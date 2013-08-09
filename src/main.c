#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "ckt/logger.h"
#include "ckt/mppt.h"

#define PI 3.14159265
#define ABS(a) (((a) < 0) ? -(a) : (a))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

// circuit parameters

double voc = 40;
double vout = 120;
double vset = 30;

double eff = 0.98;

double delta_v = 0.5;

double rin = 1;

// simulation parameters

double dt = 1E-3;

// initial conditions

// logged variables

double iin;
double iout;
double pout;
double vmpp;
double pmpp;
double efficiency;

double solar_current() {
  return (voc - vset) / rin;
}

double solar_vmpp() {
  return voc / 2;
}

/**
 * Program Entry Point
 */
int main(int argc, char *argv[]) {
  struct logger logger;
  struct mppt mppt;

  logger_init(&logger, dt);
  mppt_init(&mppt, dt, 1E-2, delta_v, &voc, &vout, &iin, &iout, &vset);

  logger_add_var(&logger, "vset", LOGGER_TYPE_DBL, 1, &vset);
  logger_add_var(&logger, "vmpp", LOGGER_TYPE_DBL, 1, &vmpp);
  logger_add_var(&logger, "voc", LOGGER_TYPE_DBL, 1, &voc);
  logger_add_var(&logger, "vout", LOGGER_TYPE_DBL, 1, &vout);
  logger_add_var(&logger, "iin", LOGGER_TYPE_DBL, 1, &iin);
  logger_add_var(&logger, "iout", LOGGER_TYPE_DBL, 1, &iout);
  logger_add_var(&logger, "pout", LOGGER_TYPE_DBL, 1, &pout);
  logger_add_var(&logger, "pmpp", LOGGER_TYPE_DBL, 1, &pmpp);
  logger_add_var(&logger, "efficiency", LOGGER_TYPE_DBL, 100, &efficiency);

  int cnt = 0;
  while (cnt < 10/dt) {

    if (cnt == 5/dt) {
      voc = 90;
    }

    vmpp = solar_vmpp();
    iin = solar_current();
    pout = (vset * iin) * eff;
    iout = pout / vout;
    pmpp = voc * voc / (4 * rin) * eff;
    efficiency = pout / pmpp;

    logger_tick(&logger);

    mppt_tick(&mppt);

    ++cnt;
  }

  return 0;
}

