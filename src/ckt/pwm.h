#ifndef _PWM_H
#define _PWM_H

#include <stdbool.h>

/*
 * Switches *out at a given period and duty cycle
 */

struct pwm {

  // configuration
  double dt;
  int CCR;
  int T;

  // output
  bool *out;

  // state
  int counter;

};

void pwm_init(struct pwm *pwm, double dt, double D, double T, bool *out);
void pwm_set_d(struct pwm *pwm, double D);
void pwm_set_t(struct pwm *pwm, double T);
void pwm_tick(struct pwm *pwm);

#endif
