#include "shares.h"
#include <assert.h>

#include <stdlib.h>

/* Asserts :
 *      n is strictly positive
 */
void expand(byte a, byte r[], int n) {
  int i;
  byte last = a;
  assert(n > 0);
  /* the n - 1 first values are random */
  for(i = 0; i < n-1; i++){
    /* TODO Change rand function */
    r[i] = rand();
    /* last = last - r[i] = last + r[i] */
    last ^= r[i];
  }
  r[n-1] = last;
}

/* Asserts :
 *      n is strictly positive
 */
void refresh(byte share[], int n) {
  int i;
  assert(n > 0);
  for(i = 1; i < n; i++) {
    /* TODO Change rand function */
    byte tmp = rand();
    share[i] ^= tmp;
    share[0] ^= tmp;
  }
}

/* Asserts :
 *      n is strictly positive
 */
byte collapse(byte a[], int n) {
  int i;
  byte res = 0;
  assert(n > 0);
  for(i = 0; i < n; i++) {
    res ^= a[i];
  }
  return res;
}

/* Asserts :
 *      n is strictly positive
 */
void secAdd(byte a[], byte b[], byte r[], int n) {
  int i;
  assert(n > 0);
  for(i = 0; i < n; i++) {
    r[i] = a[i] ^ b[i];
  }
}

/*
 * Asserts :
 *      n is strictly positive
 */
void secSquare(byte a[], byte r[], int n) {
  int i;
  assert(n > 0);
  for(i = 0; i < n; i++) {
    r[i] = square(a[i]);
  }
}

/*
 * Asserts :
 *      n is strictly positive
 */
void secMult(byte a[], byte b[], byte r[], int n) {
  int i, j;
  byte rvals[n][n];
  assert(n > 0);
  for(i = 0; i < n; i++) {
      for(j = i+1; j < n; j++) {
	/* TODO Change rand function */
	rvals[i][j] = rand();
	rvals[j][i] = (rvals[i][j] ^ mult_log(a[i], b[j]))
	                   ^ mult_log(a[j], b[i]);
      }
  }
  for(i = 0; i < n; i++) {
    r[i] = mult_log(a[i], b[i]);
    for(j = 0; j < i; j++) {
      r[i] = add(r[i], rvals[i][j]);
    }
    for(j = i+1; j < n; j++) {
      r[i] = add(r[i], rvals[i][j]);
    }
  }
}

/* 0 est son propre inverse */
void secExp254(byte a[], byte r[], int n) {
  int i;
  byte z[n], w[n];
  /* z = a^2 */
  for(i = 0; i < n ; i++)
    z[i] = square(a[i]);
  refresh(z, n);
  /* r = z * x = x^3 */
  secMult(z, a, r, n);
  /* w = r^4 = (x^3)^4 = x^12 */
  for(i = 0; i < n ; i++)
    w[i] = pow4(r[i]);
  refresh(w, n);
  /* r = r * w = x^3X^12 = x^15 */
  secMult(r, w, r, n);
  /* r = r^16 = (x^15)^16 = x^240 */
  for(i = 0; i < n ; i++)
    r[i] = pow16(r[i]);
  /* r = r * w */
  secMult(r, w, r, n);
  /* r = r * z */
  secMult(r, z, r, n);
}
