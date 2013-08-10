#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "ckt/logger.h"
#include "ckt/mppt.h"
#include "ckt/curve.h"

#define PI 3.14159265
#define ABS(a) (((a) < 0) ? -(a) : (a))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

// circuit parameters

double voc = 40;
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

double solar_current() {
  return (voc - vset) / rin;
}

double solar_vmpp() {
  return voc / 2;
}

/*
 * Returns the equivalent index in between the two input indexes s.t.
 * interpolating at that index would yield the highest value of (x*y).
 *
 * If y = ax = b, (xy) max is at x = -b/2a
 */
double best_loc(struct curve *curve, int x1, int x2) {
  double y1 = curve->points[x1];
  double y2 = curve->points[x2];
  double a = (y2 - y1) / (x2 - x1);
  double b = y1 - a * x1;
  double best_x = - b / (2 * a);
  if (best_x < (double)x1) {
    return (double)x1;
  } else if (best_x > (double)x2) {
    return (double)x2;
  }
  return best_x;
}

double find_vmpp(struct curve *curve) {
  for (int i = 1; i < curve->len; ++i) {
    if (curve->points[i] * i * curve->x_scale <
        curve->points[i-1] * (i-1) * curve->x_scale) {
      double best_idx = (double)(i - 1);
      int prev_idx = i - 2;
      int next_idx = i;
      double best_power = curve_interpolate(curve,
          best_idx * curve->x_scale) * best_idx * curve->x_scale;
      if (prev_idx >= 0) {
        double best_prev = best_loc(curve, prev_idx, best_idx);
        double best_prev_pwr = curve_interpolate(curve,
            best_prev * curve->x_scale) * best_prev * curve->x_scale;
        if (best_prev_pwr > best_power) {
          best_power = best_prev_pwr;
          best_idx = best_prev;
        }
      }
      if (next_idx < curve->len) {
        double best_next = best_loc(curve, best_idx, next_idx);
        double best_next_pwr = curve_interpolate(curve,
            best_next * curve->x_scale) * best_next * curve->x_scale;
        if (best_next_pwr > best_power) {
          best_power = best_next_pwr;
          best_idx = best_next;
        }
      }
      return best_idx * curve->x_scale;
    }
  }
  return 0;
}

/**
 * Program Entry Point
 */
int main(int argc, char *argv[]) {
  struct logger logger;
  struct mppt mppt;
  struct curve iv_curve;

  double currents1[] = { 50.0, 45.0, 25.0, 5.0, 0.0 };
  double currents2[] = { 50.0, 45.0, 40.0, 15.0, 0.0 };

  logger_init(&logger, dt);
  mppt_init(&mppt, dt, 1E-2, delta_v, &voc, &vout, &iin, &iout, &vset);
  curve_init(&iv_curve, currents1, 5, 20);

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
      curve_init(&iv_curve, currents2, 5, 20);
    }

    vmpp = find_vmpp(&iv_curve);
    iin = curve_interpolate(&iv_curve, vset);
    pout = (vset * iin) * mppt_eff;
    iout = pout / vout;
    pmpp = vmpp * curve_interpolate(&iv_curve, vmpp) * mppt_eff;
    efficiency = pout / pmpp;

    logger_tick(&logger);

    mppt_tick(&mppt);

    ++cnt;
  }

  return 0;
}

