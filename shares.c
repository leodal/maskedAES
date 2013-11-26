#include "shares.h"
#include <assert.h>

#include <stdlib.h>

void expand(byte a, byte r[]) {
  int i;
  byte last = a;
  /* the n - 1 first values are random */
  for(i = 0; i < SHARES-1; i++){
    /* TODO Change rand function */
    r[i] = rand();
    /* last = last - r[i] = last + r[i] */
    last ^= r[i];
  }
  r[SHARES-1] = last;
}

void refresh(byte share[]) {
  int i;
  for(i = 1; i < SHARES; i++) {
    /* TODO Change rand function */
    byte tmp = rand();
    share[i] ^= tmp;
    share[0] ^= tmp;
  }
}

byte collapse(byte a[]) {
  int i;
  byte res = 0;
  for(i = 0; i < SHARES; i++) {
    res ^= a[i];
  }
  return res;
}

void secAdd(byte a[], byte b[], byte r[]) {
  int i;
  for(i = 0; i < SHARES; i++) {
    r[i] = a[i] ^ b[i];
  }
}

void secSquare(byte a[], byte r[]) {
  int i;
  for(i = 0; i < SHARES; i++) {
    r[i] = square(a[i]);
  }
}

void secMult(byte a[], byte b[], byte r[]) {
  int i, j;
  byte rvals[SHARES][SHARES];
  for(i = 0; i < SHARES; i++) {
      for(j = i+1; j < SHARES; j++) {
	/* TODO Change rand function */
	rvals[i][j] = rand();
	rvals[j][i] = (rvals[i][j] ^ mult_log(a[i], b[j]))
	                   ^ mult_log(a[j], b[i]);
      }
  }
  for(i = 0; i < SHARES; i++) {
    r[i] = mult_log(a[i], b[i]);
    for(j = 0; j < i; j++) {
      r[i] = add(r[i], rvals[i][j]);
    }
    for(j = i+1; j < SHARES; j++) {
      r[i] = add(r[i], rvals[i][j]);
    }
  }
}

/* 0 est son propre inverse */
void secExp254(byte a[], byte r[]) {
  int i;
  byte z[SHARES], w[SHARES];
  /* z = a^2 */
  for(i = 0; i < SHARES ; i++)
    z[i] = square(a[i]);
  refresh(z);
  /* r = z * x = x^3 */
  secMult(z, a, r);
  /* w = r^4 = (x^3)^4 = x^12 */
  for(i = 0; i < SHARES ; i++)
    w[i] = pow4(r[i]);
  refresh(w);
  /* r = r * w = x^3X^12 = x^15 */
  secMult(r, w, r);
  /* r = r^16 = (x^15)^16 = x^240 */
  for(i = 0; i < SHARES ; i++)
    r[i] = pow16(r[i]);
  /* r = r * w */
  secMult(r, w, r);
  /* r = r * z */
  secMult(r, z, r);
}
