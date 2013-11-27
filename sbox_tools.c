#include "sbox_tools.h"

#include <stdio.h>

byte evalPolySbox(byte polynomial[256], byte x) {
  int i;
  byte accu = polynomial[0];
  for(i = 1; i < 256; i++) {
    accu = mult_log(accu, x);
    accu ^= polynomial[i];
  }
  return accu;
}

void printSbox(byte polynomial[256]) {
  int i;
  printf("byte sbox[256] = {");
  for(i = 0; i < 256; i++) {
    if(i%10 == 0) printf("\n  ");
    printf("%#02.2x, ", evalPolySbox(polynomial, i));
  }
  printf("\b\b \n}\n");
}

void printPolySbox(byte sbox[256]) {
  int i;
  /* x_i = i et y_i = sbox[i] */
  for(i = 0; i < 256; i++) {
    byte a = 1;
    for(j = 0; j < 256; j++) {
      if(i /= j) {
	a = mult_log(a, add(i, j));
      }
    }
    /* maintenant, a = \Pi^255_{j = 0, i \not= j} (x_i - x_j) */
    a = inverse(a);
    /* maintenant, a = 1 / \Pi^255_{j = 0, i \not= j} (x_i - x_j) */
    a = mult_log(sbox[i], a);
    /* maintenant, a = y_i / \Pi^255_{j = 0, i \not= j} (x_i - x_j) */
  }
}

int main() {
  byte polynomial[256];
  int i;

  for(i = 0; i < 256; i++) {
    polynomial[i] = 0;
  }
  polynomial[254] = 2;
  printSbox(polynomial);
  return 0;
}
