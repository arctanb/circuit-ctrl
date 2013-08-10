#include <string.h>

#include "curve.h"

void curve_init(struct curve *curve, double *points, int len, double x_scale) {
  memcpy(curve->points, points, sizeof(double) * len);
  curve->x_scale = x_scale;
  curve->len = len;
}

double curve_interpolate(struct curve *curve, double x) {
  double x_loc = x / curve->x_scale;
  int x_prev = (int)x_loc;
  int x_next = x_prev + 1;
  double x_prev_wgt = (double)x_next - x_loc;
  if (x_next >= curve->len) {
    return 0;
  }
  return curve->points[x_prev] * x_prev_wgt +
      curve->points[x_next] * (1 - x_prev_wgt);
}

