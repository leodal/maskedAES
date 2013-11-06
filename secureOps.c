#include "secureOps.h"

#include <stdlib.h>

#include <stdio.h>


void refresh(byte a_share[], int order) {
  int i;
  for(i = 1; i < order; i++) {
    byte tmp = rand();
    a_share[i] ^= tmp;
    a_share[0] ^= tmp;
  }
}

void secAdd(byte a_shares[], byte b_shares[], byte r_shares[], int order) {
  int i;
  for(i = 0; i < order; i++) {
    r_shares[i] = a_shares[i] ^ b_shares[i];
  }
}

void secSquare(byte a_shares[], byte r_shares[], int order) {
  int i;
  for(i = 0; i < order; i++) {
    r_shares[i] = square(a_shares[i]);
  }
}

void secMult(byte a_shares[], byte b_shares[], byte r_shares[], int order) {
  int i, j;
  byte r[order][order];
  for(i = 0; i < order; i++) {
      for(j = i+1; j < order; j++) {
	// TODO Change rand function
	r[i][j] = rand();
	r[j][i] = (r[i][j] ^ mult_log(a_shares[i], b_shares[j]))
	                   ^ mult_log(a_shares[j], b_shares[i]);
      }
  }
  for(i = 0; i < order; i++) {
    r_shares[i] = mult_log(a_shares[i], b_shares[i]);
    for(j = 0; j < i; j++) {
      r_shares[i] = add(r_shares[i], r[i][j]);
    }
    for(j = i+1; j < order; j++) {
      r_shares[i] = add(r_shares[i], r[i][j]);
    }
  }
}

// TODO Il y a une erreur là dedans
void secExp254(byte a_share[], byte r_share[], int order) {
  int i;
  byte z[order], w[order];
  // z = a^2
  for(i = 0; i < order ; i++)
    z[i] = square(a_share[i]);
  refresh(z, order);
  // r = z * x = x^3
  secMult(z, a_share, r_share, order);
  // w = r^4 = (x^3)^4 = x^12
  for(i = 0; i < order ; i++)
    w[i] = pow4(r_share[i]);
  refresh(w, order);
  // r = r * w = x^3X^12 = x^15
  secMult(r_share, w, r_share, order);
  // r = r^16 = (x^15)^16 = x^240
  for(i = 0; i < order ; i++)
    r_share[i] = pow16(r_share[i]);
  // r = r * w
  secMult(r_share, w, r_share, order);
  // r = r * z
  secMult(r_share, z, r_share, order);
}

void secAffine(byte a[], byte x[], byte b[], byte res[], int order) {
  byte temp[order];
  secMult(a, x, temp, order);
  for(i = 0; i < order; i++) {
    res[i] = temp[i] ^ b[i];
  }
}

void secSBox(byte x_share[], byte a_share[], byte b_share[], byte r_share[]) {
  byte temp[order];
  secExp254(x_share, r_share, order);
  
}
