#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "ckt/pwm.h"

#define PI 3.14159265

// circuit parameters

double l = 100E-6;
double c = 100E-6;
double rl = 1E3;

double vimax = 100;
double vifreq = 60;

// simulation parameters

double dt = 1E-7;

// initial conditions

double il = 0;
double vo = 0;

bool sw = false;

void print_hdr();
void print_val(char symbol, double val);

int main(int argc, char *argv[]) {
  struct pwm pwm;
  bool pwm_out = false;
  pwm_init(&pwm, 0.1, 0.4, 1, &pwm_out);

  print_hdr();

  int cnt = 0;
  while (true) {
    double vi = vimax * sin(cnt*vifreq*2*PI*dt);
    double vl = sw ? vi : vi - vo;
    il += vl / l * dt;

    if (!sw) {
      vo += il / c * dt;
    }

    printf("#%d\n", cnt);

    print_val('!', vo);
    print_val('@', il);
    print_val('#', vi);

    pwm_tick(&pwm);

    ++cnt;
  }

  printf("done\n");
  return 0;
}

void print_hdr() {
  printf("$timescale 0.1 ns $end\n");
  printf("$timescale 0.1 ns $end\n");
  printf("$var wire 32 ! Vout $end\n");
  printf("$var wire 32 @ IL $end\n");
  printf("$var wire 32 # Vin $end\n");
}

void print_binary(double val) {
  int number = (int)val;
  int remainder;

  if(number <= 1) {
    printf("%d", number);
    return;
  }

  remainder = number % 2;
  print_binary(number >> 1);
  printf("%d", remainder);
}

void print_val(char symbol, double val) {
  printf("b");
  print_binary(val);
  printf(" %c\n", symbol);
}

