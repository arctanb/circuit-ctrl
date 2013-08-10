#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "curveset.h"

bool curveset_load_due(struct curveset *curveset) {
  return (curveset->dt * (double)curveset->counter >=
      curveset->times[curveset->cur_time_idx]);
}

void curveset_load_curve(struct curveset *curveset) {
  if (curveset_load_due(curveset) &&
      curveset->cur_time_idx < curveset->len) {
    *(curveset->curve_ptr) = &(curveset->curves[curveset->cur_time_idx++]);
  }
}

/*
 * File format:
 * # curves in file (n)
 * # points per curve (p)
 * curve 0: time in secs that curve 0 should be loaded
 * curve 0: p space-separated floats
 * curve 1, etc.
 */
void curveset_init(struct curveset *curveset, struct curve **curve_ptr,
    double dt, char *fname) {
  curveset->counter = 0;
  curveset->dt = dt;
  curveset->cur_time_idx = 0;
  curveset->curve_ptr = curve_ptr;
  curveset->curves = NULL;
  curveset->times = NULL;

  FILE *curve_file = NULL;
  if (!(curve_file = fopen(fname, "r"))) {
    fprintf(stderr, "Curveset: Could not open file. Exiting.\n");
    exit(-1);
  }

  if (!fscanf(curve_file, "%d", &curveset->len)) {
    fprintf(stderr,
        "Curveset: First line does not contain an integer. Exiting.\n");
    exit(-1);
  }

  int len_per_curve = 0;
  if (!fscanf(curve_file, "%d", &len_per_curve)) {
    fprintf(stderr,
        "Curveset: Second line does not contain an integer. Exiting.\n");
    exit(-1);
  }

  double x_scale = 0.0;
  if (!fscanf(curve_file, "%lf", &x_scale)) {
    fprintf(stderr,
        "Curveset: Third line does not contain a float. Exiting.\n");
    exit(-1);
  }

  curveset->curves = malloc(sizeof(struct curve) * curveset->len);
  curveset->times = malloc(sizeof(double) * curveset->len);

  for (int i = 0; i < curveset->len; ++i) {
    if (!fscanf(curve_file, "%lf", &(curveset->times[i]))) {
      fprintf(stderr,
          "Curveset: Problem with curve #%d Exiting.\n", (i+1));
      exit(-1);
    }
    double points[len_per_curve];
    for (int u = 0; u < len_per_curve; ++u) {
      if (!fscanf(curve_file, "%lf", &(points[u]))) {
        fprintf(stderr,
            "Curveset: Problem with curve #%d Exiting.\n", (i+1));
        exit(-1);
      }
    }
    curve_init(&(curveset->curves[i]), points, len_per_curve, x_scale);
  }

  curveset_load_curve(curveset);
}

void curveset_tick(struct curveset *curveset) {
  ++curveset->counter;
  curveset_load_curve(curveset);
}

void curveset_print(struct curveset *curveset) {
  fprintf(stderr, "curveset:\n");
  fprintf(stderr, "len: %d\n", curveset->len);
  for (int i = 0; i < curveset->len; ++i) {
    fprintf(stderr, "time: %lf\n", curveset->times[i]);
    curve_print(&(curveset->curves[i]));
  }
}
