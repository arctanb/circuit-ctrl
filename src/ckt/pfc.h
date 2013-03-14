#ifndef _PFC_H
#define _PFC_H

#include "pwm.h"

struct pfc {

  // configuration
  double dt;
  int T;
  double i_alpha;
  double target_vout;

  // output
  struct pwm *pwm;

  // state
  int counter;
  double i_avg;

};

void pfc_init(struct pfc *pfc, double dt, double T, double target_vout,
    double i_alpha, struct pwm *pwm);
void pfc_set_t(struct pfc *pfc, double T);
void pfc_tick(struct pfc *pfc, double current, double voltage);

#endif

