#include "pfc.h"

void pfc_init(struct pfc *pfc, double dt, double T, double target_vout,
    double i_alpha, struct pwm *pwm) {
  pfc->dt = dt;
  pfc->i_alpha = i_alpha;
  pfc->pwm = pwm;
  pfc->counter = 0;
  pfc->i_avg = 0;
  pfc_set_t(pfc, T);
}

void pfc_set_t(struct pfc *pfc, double T) {
  pfc->T = (int)(T / pfc->dt);
}

void pfc_tick(struct pfc *pfc, double current, double voltage) {
  ++pfc->counter;
  if (pfc->counter > pfc->T) {
    pfc->counter = 0;
  } else {
    return;
  }
  pfc->i_avg = pfc->i_avg * pfc->i_alpha + current * (1 - pfc->i_alpha);
}

