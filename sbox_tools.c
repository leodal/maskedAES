#include "sbox_tools.h"

#include <stdio.h>

/**
 * Given a polynomial P in GF(256)[x] (high degrees at first), evaluate P(x)
 */
byte evalPolySbox(byte polynomial[256], byte x) {
  int i;
  byte accu = polynomial[0];
  for(i = 1; i < 256; i++) {
    accu = mult(accu, x);
    accu ^= polynomial[i];
  }
  return accu;
}

void revertTab(byte tab[], int length) {
  byte tmp;
  int i, j;
  for(i=0, j=length-1; i < j; i++, j--) {
    tmp = tab[i];
    tab[i] = tab[j];
    tab[j] = tmp;
  }
}

/**
 * Given a Sbox in polynomial build the Sbox
 */
void buildSbox(byte polynomial[256], byte sbox[256]) {
  int i;
  for(i = 0; i < 256; i++) {
    sbox[i] = evalPolySbox(polynomial, i);
  }
}

/**
 * Given a Sbox in polynomial form, print Sbox table
 */
void printSbox(byte polynomial[256]) {
  int i;
  printf("byte sbox[256] = {");
  for(i = 0; i < 256; i++) {
    if(i%10 == 0) printf("\n  ");
    printf("%#2.2x, ", evalPolySbox(polynomial, i));
  }
  printf("\b\b \n}\n");
}

/**
 * poly1  : polynome de degré d-1
 * poly2  : polynome de degré 1
 * result : polynome de degré d, produit de poly1 et poly2
 * Attention à l'odre : ici les petits degrés on les petits indices ; je considère l'inverse dans mes sbox
 * Attention, il faut que polyDegD et result soient différents
 */
void polyProduct(byte polyDegD[], byte polyDeg1[], byte result[], int d) {
  int i;
  result[0] = 0;
  for(i = 0; i < d; i++) {
    result[i+1] = mult(polyDeg1[1], polyDegD[i]);
    result[i]  ^= mult(polyDeg1[0], polyDegD[i]);
  }
}

/**
 * poly  : polynome de degré d
 * a : le scalaire
 * result : polynome de degré d, produit de poly1 et poly2
 * Attention à l'odre : ici les petits degrés on les petits indices ; je considère l'inverse dans mes sbox
 */
void polyScalMult(byte poly[], byte a, int d) {
  int i;
  for(i = 0; i <= d; i++) {
    poly[i] = mult(poly[i], a);
  }
}

/**
 * poly1  : polynome de degré d
 * poly2  : polynome de degré d
 * result : polynome de degré d, somme de poly1 et poly2
 */
void polyAdd(byte poly1[], byte poly2[], byte result[], int d) {
  int i;
  for(i = 0; i <= d; i++) {
    result[i] = add(poly1[i], poly2[i]);
  }
}

void lagrange(byte table[], byte result[], int d) {
  int i, j, k;
  byte polyDeg1[2];
  polyDeg1[1] = 0x1;
  for(i = 0; i <= d; i++)
    result[i] = 0;
  for(i = 0; i <= d; i++) {
    byte a = 1;
    byte lagrangePoly[d+1], tmpPoly[d+1];
#ifdef DEBUG_LAGRANGE
    printf("i = %d (d = %d)\n", i, d);
#endif
    lagrangePoly[0] = 0x1;
    for(j = 1; j < d+1; j++) {
      lagrangePoly[j] = 0;
    }
#ifdef DEBUG_LAGRANGE
    printf("\tLagrange poly = ");
    for(j = 0; j < d+1; j++) {
      if(lagrangePoly[j]) {
	printf("%#2.2x * X^%d + ", lagrangePoly[j], j);
      }
    }
    printf("\b\b  \n");
#endif
    for(j = 0; j <= d; j++) { 
      if(i != j) {
	polyDeg1[0] = j;
#ifdef DEBUG_LAGRANGE
	printf("\tj = %d\n", j);
	printf("\t\tpolyDeg1 = ");
	for(k = 0; k < 2; k++) {
	  if(polyDeg1[k]) {
	    printf("%#2.2x * X^%d + ", polyDeg1[k], k);
	  }
	}
	printf("\b\b  \n");
#endif
 	a = mult(i ^ j, a);
 	polyProduct(lagrangePoly, polyDeg1, tmpPoly, d+1);
	for(k = 0; k < d+1; k++) {
	  lagrangePoly[k] = tmpPoly[k];
	}
#ifdef DEBUG_LAGRANGE
	printf("\t\ta = %#2.2x\n", a);
	printf("\t\tlagrangePoly = ");
	for(k = 0; k < 2; k++) {
	  if(lagrangePoly[k]) {
	    printf("%#2.2x * X^%d + ", lagrangePoly[k], k);
	    isZero = 0;
	  }
	}
	if(isZero) printf("0\n");
	else printf("\b\b\b  \n");
#endif
      }
    }
    a = inverse(a);
    a = mult(table[i], a);
    polyScalMult(lagrangePoly, a, d);
    polyAdd(lagrangePoly, result, result, d);
  }
}

void buildPolySbox(byte sbox[256], byte polySbox[256]) {
  lagrange(sbox, polySbox, 255);
  revertTab(polySbox, 256);
}

void printPolySbox(byte sbox[256]) {
  int i;
  byte polySbox[256];
  buildPolySbox(sbox, polySbox);
  printf("byte polySbox[256] = {");
  for(i = 0; i < 256; i++) {
    if(i %10 == 0) printf("\n  ");
    printf("%#2.2x, ", polySbox[i]);
  }
  printf("\b\b \n}\n");
}

/*
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
*/
