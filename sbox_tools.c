#include "sbox_tools.h"

#include <stdio.h>

/**
 * Given a polynomial P in GF(256)[x], evaluate P(x)
 */
byte evalPolySbox(byte polynomial[256], byte x) {
  int i;
  byte accu = polynomial[0];
  for(i = 1; i < 256; i++) {
    accu = mult_log(accu, x);
    accu ^= polynomial[i];
  }
  return accu;
}

/**
 * Given a Sbox in polynomial form, print Sbox table
 */
void printSbox(byte polynomial[256]) {
  int i;
  printf("byte sbox[256] = {");
  for(i = 0; i < 256; i++) {
    if(i%10 == 0) printf("\n  ");
    printf("%#02.2x, ", evalPolySbox(polynomial, i));
  }
  printf("\b\b \n}\n");
}

/**
 * poly1  : polynome de degré d-1
 * poly2  : polynome de degré 1
 * result : polynome de degré d, produit de poly1 et poly2
 * Attention à l'odre : ici les petits degrés on les petits indices ; je considère l'inverse dans mes sbox
 */
void polyProduct(byte polyDegD[], byte polyDeg1[], byte result[], int d) {
  int i;
  result[0] = 0;
  for(i = 0; i < d; i++) {
    result[i+1] = mult_log(polyDeg1[1], polyDegD[i]);
    result[i]  ^= mult_log(polyDeg1[0], polyDegD[i]);
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
    poly[i] = mult_log(poly[i], a);
  }
}

/**
 * poly1  : polynome de degré d
 * poly2  : polynome de degré d
 * result : polynome de degré d, produit de poly1 et poly2
 */
void polyAdd(byte poly1[], byte poly2[], byte result[], int d) {
  int i;
  for(i = 0; i <= d; i++) {
    result[i] = add(poly1[i], poly2[i]);
  }
}

void buildPolySbox(byte sbox[256], byte polySbox[256]) {
  int i;
  int j;
  byte polyDeg1[2];
  polyDeg1[1] = 0x1;
  /* x_i = i et y_i = sbox[i] */
  for(i = 0; i < 256; i++) {
    byte a = 1;
    byte polyLagrange[256];
    polyLagrange[0] = 1;
    for(j = 0; j < i; j++) {
      polyDeg1[0] = j;
      polyProduct(polyLagrange, polyDeg1, polyLagrange, i);
      a = mult_log(a, add(i, j));
    }
    for(j = i+1; j < 256; j++) {
      polyDeg1[0] = j;
      polyProduct(polyLagrange, polyDeg1, polyLagrange, i);
      a = mult_log(a, add(i, j));
    }
    /* maintenant, a = \Pi^255_{j = 0, i \not= j} (x_i - x_j) 
                     = \Pi^255_{j = 0, i \not= j} (i - j)
       et polyProduct = \Pi^255_{j = 0, i \not= j} (X - x_i)
    */
    a = inverse(a);
    /* maintenant, a = 1 / \Pi^255_{j = 0, i \not= j} (x_i - x_j) */
    a = mult_log(sbox[i], a);
    /* maintenant, a = y_i / \Pi^255_{j = 0, i \not= j} (x_i - x_j) */
    polyScalMult(polyLagrange, a, 255);
    /* maintenant polyProduct = y_i * \Pi^255_{j = 0, i \not= j} (X - x_i) / \Pi^255_{j = 0, i \not= j} (x_i - x_j) */
    polyAdd(polyLagrange, polySbox, polySbox, 255);
  }
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
