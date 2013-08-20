#ifndef _FIR_H
#define _FIR_H

#define FIR_MAX_LEN 100

enum fir_input_type {
  FIR_INPUT_INT,
  FIR_INPUT_DBL,
};

struct fir {
  int length;
  double buffer[FIR_MAX_LEN];
  double impulse_resp[FIR_MAX_LEN];
  enum fir_input_type input_type;
  void *input;
  double output;
};

void fir_init(struct fir *fir, int length, double *impulse_resp,
    enum fir_input_type input_type, void *input);

void fir_tick(struct fir *fir);

#endif
