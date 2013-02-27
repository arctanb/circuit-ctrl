#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "ckt/pwm.h"

#define PI 3.14159265
#define ABS(a) (((a) < 0) ? -(a) : (a))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

// circuit parameters

double l = 100E-6;
double c = 100E-6;
double rl = 1E2;

double vimax = 100;
double vifreq = 60;

// simulation parameters

double dt = 1E-7;

// initial conditions

double il = 0;
double vo = 0;

bool sw = false;

void print_hdr();
void print_val(char symbol, int val);

int main(int argc, char *argv[]) {
  struct pwm pwm;
  pwm_init(&pwm, dt, 0.7, 1.0/1E5, &sw);

  print_hdr();

  int cnt = 0;
  while (true) {
    double vi = vimax * ABS(sin(cnt*vifreq*2*PI*dt));
    double vl = sw ? vi : vi - vo;
    il += vl / l * dt;
    il = MAX(il, 0);

    if (!sw) {
      vo += il / c * dt;
    }

    printf("#%d\n", cnt);

    print_val('!', (int)vo);
    print_val('@', (int)il);
    print_val('#', (int)vi);
    print_val('*', (int)sw);

    pwm_tick(&pwm);

    ++cnt;
  }

  printf("done\n");
  return 0;
}

void print_hdr() {
  printf("$timescale 100 ns $end\n");
  printf("$var wire 32 ! Vout $end\n");
  printf("$var wire 32 @ IL $end\n");
  printf("$var wire 32 # Vin $end\n");
  printf("$var wire 32 * GD $end\n");
}

void print_binary(int number) {
  int remainder;

  if (number <= 1) {
    printf("%d", number);
    return;
  }

  remainder = number % 2;
  print_binary(number >> 1);
  printf("%d", remainder);
}

void print_val(char symbol, int val) {
  printf("b");
  print_binary(val);
  printf(" %c\n", symbol);
}

