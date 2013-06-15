#include <stdio.h>

#include "pfc.h"
#include "util.h"

void pfc_init(struct pfc *pfc, double dt, double T, double target_vout,
    double v_alpha, struct pwm *pwm, double *vin, double *vout) {
  pfc->dt = dt;
  pfc->v_alpha = v_alpha;
  pfc->pwm = pwm;
  pfc->counter = 0;
  pfc->v_avg = 0;
  pfc->d = 0;
  pfc->target_vout = target_vout;
  pfc->vin = vin;
  pfc->vout = vout;
  pfc_set_t(pfc, T);
}

void pfc_set_t(struct pfc *pfc, double T) {
  pfc->T = (int)(T / pfc->dt);
}

void pfc_tick(struct pfc *pfc) {
  ++pfc->counter;
  if (pfc->counter > pfc->T) {
    pfc->counter = 0;
  } else {
    return;
  }
  pfc->v_avg = pfc->v_avg * pfc->v_alpha + *pfc->vout * (1 - pfc->v_alpha);

  double k = 5000;
  double duty = k / pfc->T * (1 - *(pfc->vin) / *(pfc->vout));

  fprintf(stderr, "duty is at %f\n", duty);

  pfc->d = duty;

  pfc->d = MAX(0, pfc->d);
  pfc->d = MIN(0.8, pfc->d);

  pwm_set_d(pfc->pwm, pfc->d);

}

