#include "secureOps.h"
#include "shares.h"
#include "aes.h"

#include <stdlib.h>

void secAdd(byte a[], byte b[], byte r[], int n) {
  int i;
  for(i = 0; i < n; i++) {
    r[i] = a[i] ^ b[i];
  }
}

void secSquare(byte a[], byte r[], int n) {
  int i;
  for(i = 0; i < n; i++) {
    r[i] = square(a[i]);
  }
}

void secMult(byte a[], byte b[], byte r[], int n) {
  int i, j;
  byte rvals[n][n];
  for(i = 0; i < n; i++) {
      for(j = i+1; j < n; j++) {
	// TODO Change rand function
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

void secAesAffine(byte a[], byte r[], int n) {
  int i;
  for(i = 0; i<n; i++) {
    r[i] = aes_affine(a[i]);
  }
  if(n % 2 == 1) r[0] ^= 0x63;
}

// TODO Il y a une erreur là dedans
void secExp254(byte a[], byte r[], int n) {
  int i;
  byte z[n], w[n];
  // z = a^2
  for(i = 0; i < n ; i++)
    z[i] = square(a[i]);
  refresh(z, n);
  // r = z * x = x^3
  secMult(z, a, r, n);
  // w = r^4 = (x^3)^4 = x^12
  for(i = 0; i < n ; i++)
    w[i] = pow4(r[i]);
  refresh(w, n);
  // r = r * w = x^3X^12 = x^15
  secMult(r, w, r, n);
  // r = r^16 = (x^15)^16 = x^240
  for(i = 0; i < n ; i++)
    r[i] = pow16(r[i]);
  // r = r * w
  secMult(r, w, r, n);
  // r = r * z
  secMult(r, z, r, n);
}


