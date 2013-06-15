#include <string.h>

#include "fir.h"

void fir_init(struct fir *fir, int length, double *impulse_resp,
    enum fir_input_type input_type, void *input) {
  fir->length = length;
  fir->input_type = input_type;
  fir->input = input;
  fir->output = 0.0;
  for (int i = 0; i < FIR_MAX_LEN; ++i) {
    fir->buffer[i] = 0.0;
    fir->impulse_resp[i] = 0.0;
  }
  memcpy(fir->impulse_resp, impulse_resp, length * sizeof(double));
}

void fir_tick(struct fir *fir) {

  // deal with input

  double input = fir->input_type == FIR_INPUT_INT ?
      (double)(*((int *)(fir->input))) :
      *(double *)(fir->input);
  memmove(&(fir->buffer[1]), &(fir->buffer[0]),
      sizeof(fir->buffer) - sizeof(double));
  fir->buffer[0] = input;

  // compute output

  fir->output = 0.0;
  for (int i = 0; i < fir->length; ++i) {
    fir->output += fir->impulse_resp[i] * fir->buffer[i];
  }

}

