#ifndef _PFC_H
#define _PFC_H

#include "../model/pwm.h"

struct pfc {

  // configuration
  double dt;
  int T;
  double v_alpha;
  double target_vout;

  // input
  double *vin;
  double *vout;

  // output
  struct pwm *pwm;

  // state
  int counter;
  double v_avg;
  double d;

};

void pfc_init(struct pfc *pfc, double dt, double T, double target_vout,
    double v_alpha, struct pwm *pwm, double *vin, double *vout);
void pfc_set_t(struct pfc *pfc, double T);
void pfc_tick(struct pfc *pfc);

#endif

