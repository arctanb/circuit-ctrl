#include <stdio.h>

#include "mppt.h"
#include "util.h"


void mppt_init(struct mppt *mppt, double dt, double T, double delta_v,
    double *vin, double *vout, double *iin, double *iout, double *vset) {
  mppt->dt = dt;
  mppt->delta_v = delta_v;
  mppt->vin = vin;
  mppt->vout = vout;
  mppt->iin = iin;
  mppt->iout = iout;
  mppt->vset = vset;
  mppt->counter = 0;
  mppt->prev_pwr = 0;
  mppt->direction = DIR_DOWN;
  mppt_set_t(mppt, T);

}

void mppt_set_t(struct mppt *mppt, double T) {
  mppt->T = (int)(T / mppt->dt);
}

void mppt_tick(struct mppt *mppt) {
  ++mppt->counter;
  if (mppt->counter > mppt->T) {
    mppt->counter = 0;
  } else {
    return;
  }

  double pout = *(mppt->iout) * *(mppt->vout);
  double old_pout = mppt->prev_pwr;

  if (pout < old_pout) {
    mppt->direction = mppt->direction == DIR_UP ? DIR_DOWN : DIR_UP;
  }

  *mppt->vset += mppt->direction == DIR_UP ? mppt->delta_v : -mppt->delta_v;

  mppt->prev_pwr = pout;

}


