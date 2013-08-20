#ifndef _CURVESET_H
#define _CURVESET_H

#include "curve.h"

struct curveset {

  // configuration
  double dt;
  struct curve *curves;
  double *times;
  int len;

  // output
  struct curve **curve_ptr;

  // state
  int counter;
  int cur_time_idx;
};

void curveset_init(struct curveset *curveset, struct curve **curve_ptr,
    double dt, char *fname);

void curveset_tick(struct curveset *curveset);

void curveset_print(struct curveset *curveset);

#endif

