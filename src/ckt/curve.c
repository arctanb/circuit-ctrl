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

/*
 * Returns the equivalent index in between the two input indexes s.t.
 * interpolating at that index would yield the highest value of (x*y).
 *
 * If y = ax = b, (xy) max is at x = -b/2a
 */
double best_loc(struct curve *curve, int x1, int x2) {
  double y1 = curve->points[x1];
  double y2 = curve->points[x2];
  double a = (y2 - y1) / (x2 - x1);
  double b = y1 - a * x1;
  double best_x = - b / (2 * a);
  if (best_x < (double)x1) {
    return (double)x1;
  } else if (best_x > (double)x2) {
    return (double)x2;
  }
  return best_x;
}

/*
 * Returns x s.t. curve(x) is the first local maximum
 * Returns the global maximum if the curve has a single local max
 */
double find_best_xy(struct curve *curve) {
  for (int i = 1; i < curve->len; ++i) {
    if (curve->points[i] * i < curve->points[i-1] * (i-1)) {

      double best_idx = (double)(i - 1);
      int prev_idx = i - 2;
      int next_idx = i;

      double best_xy = curve_interpolate(curve,
          best_idx * curve->x_scale) * best_idx;

      if (prev_idx >= 0) {
        double best_prev = best_loc(curve, prev_idx, best_idx);
        double best_prev_xy = curve_interpolate(curve,
            best_prev * curve->x_scale) * best_prev;
        if (best_prev_xy > best_xy) {
          best_xy = best_prev_xy;
          best_idx = best_prev;
        }
      }

      if (next_idx < curve->len) {
        double best_next = best_loc(curve, best_idx, next_idx);
        double best_next_xy = curve_interpolate(curve,
            best_next * curve->x_scale) * best_next;
        if (best_next_xy > best_xy) {
          best_xy = best_next_xy;
          best_idx = best_next;
        }
      }

      return best_idx * curve->x_scale;

    }
  }
  return 0;
}

