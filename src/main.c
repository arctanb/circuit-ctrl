#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "ckt/logger.h"
#include "ckt/mppt.h"
#include "ckt/curve.h"
#include "ckt/curveset.h"

#define PI 3.14159265
#define ABS(a) (((a) < 0) ? -(a) : (a))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

// circuit parameters

double vout = 120;
double vset = 30;

double mppt_eff = 0.98;

double delta_v = 0.1;

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

/**
 * Program Entry Point
 */
int main(int argc, char *argv[]) {
  struct logger logger;
  struct mppt mppt;
  struct curve *iv_curve;
  struct curveset iv_curveset;

  logger_init(&logger, dt);
  mppt_init(&mppt, dt, 1E-2, delta_v, &vout, &iin, &iout, &vset);
  curveset_init(&iv_curveset, &iv_curve, dt, "iv_profile.curves");

  logger_add_var(&logger, "vset", LOGGER_TYPE_DBL, 1, &vset);
  logger_add_var(&logger, "vmpp", LOGGER_TYPE_DBL, 1, &vmpp);
  logger_add_var(&logger, "vout", LOGGER_TYPE_DBL, 1, &vout);
  logger_add_var(&logger, "iin", LOGGER_TYPE_DBL, 1, &iin);
  logger_add_var(&logger, "iout", LOGGER_TYPE_DBL, 1, &iout);
  logger_add_var(&logger, "pout", LOGGER_TYPE_DBL, 1, &pout);
  logger_add_var(&logger, "pmpp", LOGGER_TYPE_DBL, 1, &pmpp);
  logger_add_var(&logger, "efficiency", LOGGER_TYPE_DBL, 100, &efficiency);

  int cnt = 0;
  while (cnt < 10/dt) {

    vmpp = find_best_xy(iv_curve);
    iin = curve_interpolate(iv_curve, vset);
    pout = (vset * iin) * mppt_eff;
    iout = pout / vout;
    pmpp = vmpp * curve_interpolate(iv_curve, vmpp) * mppt_eff;
    efficiency = pout / pmpp;

    logger_tick(&logger);

    curveset_tick(&iv_curveset);
    mppt_tick(&mppt);

    ++cnt;
  }

  return 0;
}

