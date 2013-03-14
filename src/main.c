#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "ckt/logger.h"
#include "ckt/pfc.h"
#include "ckt/pwm.h"

#define PI 3.14159265
#define ABS(a) (((a) < 0) ? -(a) : (a))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

// circuit parameters

double l = 100E-6;
double c = 100E-6;
double rl = 1E2;

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

int main(int argc, char *argv[]) {
  struct pwm pwm;
  struct pfc pfc;
  struct logger logger;
  pwm_init(&pwm, dt, 0.7, 1.0/1E5, &sw);
  pfc_init(&pfc, dt, 1.0/1E3, 400, 0.9, &pwm);
  logger_init(&logger, dt);

  logger_add_var(&logger, "sw", LOGGER_TYPE_INT, &sw);
  logger_add_var(&logger, "vout", LOGGER_TYPE_DBL, &vo);
  logger_add_var(&logger, "il", LOGGER_TYPE_DBL, &il);
  logger_add_var(&logger, "vin", LOGGER_TYPE_DBL, &vi);

  int cnt = 0;
  while (true) {
    vi = vimax * ABS(sin(cnt*vifreq*2*PI*dt));
    double vl = sw ? vi : vi - vo;
    il += vl / l * dt;
    il = MAX(il, 0);

    if (!sw) {
      vo += il / c * dt;
    }

    logger_tick(&logger);

    pwm_tick(&pwm);

    ++cnt;
  }

  printf("done\n");
  return 0;
}

