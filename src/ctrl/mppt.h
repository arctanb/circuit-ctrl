#ifndef _MPPT_H
#define _MPPT_H

enum direction { DIR_UP, DIR_DOWN };

struct mppt {

  // configuration
  double dt;
  int T;
  double delta_v;

  // input
  double *vout;
  double *iin;
  double *iout;

  // output
  double *vset;

  // state
  int counter;
  double prev_pwr;
  enum direction direction;

};

void mppt_init(struct mppt *mppt, double dt, double T, double delta_v,
    double *vout, double *iin, double *iout, double *vset);

void mppt_set_t(struct mppt *mppt, double T);

void mppt_tick(struct mppt *mppt);

#endif

