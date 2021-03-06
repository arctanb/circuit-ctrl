#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "ckt/logger.h"
#include "ckt/pfc.h"
#include "ckt/pwm.h"
#include "ckt/fir.h"

#define PI 3.14159265
#define ABS(a) (((a) < 0) ? -(a) : (a))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

// circuit parameters

double l = 50E-6;
double c = 100E-6;
double rl = 1E3;

double vimax = 100;
double vifreq = 60;

// simulation parameters

double dt = 1E-7;

// initial conditions

double il = 0;
double vo = 0;

bool sw = false;

// logged variables

double vi;
double io;
double pi;

/**
 * Program Entry Point
 */
int main(int argc, char *argv[]) {
  struct pwm pwm;
  struct pfc pfc;
  struct logger logger;
  struct fir fir;
  double fir_impulse_resp[FIR_MAX_LEN];
  for (int i = 0; i < FIR_MAX_LEN; ++i) {
    fir_impulse_resp[i] = 1.0;
  }
  pwm_init(&pwm, dt, 0.7, 1.0/1E5, &sw);
  pfc_init(&pfc, dt, 1.0/1E4, 400, 0.95, &pwm, &vi, &vo);
  fir_init(&fir, FIR_MAX_LEN, fir_impulse_resp, FIR_INPUT_DBL, &pi);
  logger_init(&logger, dt);

  logger_add_var(&logger, "sw", LOGGER_TYPE_INT, 1, &sw);
  logger_add_var(&logger, "vout", LOGGER_TYPE_DBL, 1, &vo);
  logger_add_var(&logger, "il", LOGGER_TYPE_DBL, 10, &il);
  logger_add_var(&logger, "vin", LOGGER_TYPE_DBL, 1, &vi);
  logger_add_var(&logger, "io", LOGGER_TYPE_DBL, 10, &io);
  logger_add_var(&logger, "ccr", LOGGER_TYPE_INT, 1, &pwm.CCR);
  logger_add_var(&logger, "vavg", LOGGER_TYPE_DBL, 1, &pfc.v_avg);
  logger_add_var(&logger, "pin", LOGGER_TYPE_DBL, 1, &pi);
  logger_add_var(&logger, "s_pin", LOGGER_TYPE_DBL, 1, &fir.output);

  int cnt = 0;
  while (true) {
    vi = vimax * ABS(sin(cnt*vifreq*2*PI*dt));
    double vl = sw ? vi : vi - vo;
    il += vl / l * dt;
    il = MAX(il, 0);

    if (!sw) {
      vo += il / c * dt;
    }

    vo -= io * dt / c;

    io = vo / rl;

    pi = vi * il;

    logger_tick(&logger);

    pfc_tick(&pfc);
    pwm_tick(&pwm);
    fir_tick(&fir);

    ++cnt;
  }

  return 0;
}

