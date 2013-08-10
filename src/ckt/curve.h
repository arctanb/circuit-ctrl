#ifndef _CURVE_H
#define _CURVE_H

#define CURVE_MAX_SIZE 100

struct curve {
  // y = points[(int)(x / x_scale)]
  double points[CURVE_MAX_SIZE];
  double x_scale;
  int len;
};

void curve_init(struct curve *curve, double *points, int len, double x_scale);

double curve_interpolate(struct curve *curve, double x);

double find_best_xy(struct curve *curve);

void curve_print(struct curve *curve);

#endif

