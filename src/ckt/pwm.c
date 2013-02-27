#include "pwm.h"
#include <stdbool.h>


void pwm_init(struct pwm *pwm, double dt, double D, double T, bool *out) {
  pwm->dt = dt;
  pwm->out = out;
  pwm->counter = 0;
  pwm_set_t(pwm, T);
  pwm_set_d(pwm, D);
}

void pwm_set_d(struct pwm *pwm, double D) {
  pwm->CCR = (int)(D * pwm->T);
}

void pwm_set_t(struct pwm *pwm, double T) {
  pwm->T = (int)(T / pwm->dt);
}

void pwm_tick(struct pwm *pwm) {
  ++pwm->counter;
  if (pwm->counter > pwm->T) {
    pwm->counter = 0;
  }
  if (pwm->counter >= pwm->CCR) {
    *pwm->out = false;
  } else {
    *pwm->out = true;
  }
}
