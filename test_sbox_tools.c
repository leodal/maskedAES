#include "sbox_tools.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  int i, test, isZero, test_all;
  byte poly1[5];
  byte poly2[2];
  byte result[6], expected[256];

  byte polySbox[256], sbox[256];
  byte rpool[256];

  srand((unsigned int) time (NULL));

  for(i = 0; i < 256; i++)
    polySbox[i] = 0;
  polySbox[2] = 0x2;
  polySbox[0] = 0x3;

  poly1[0] = 0x1; poly1[1] = 0x2; poly1[2] = 0;
  poly1[3] = 0; poly1[4] = 0x3;

  poly2[0] = 0x1; poly2[0x1] = 0x1;

  expected[0] = 0x1; expected[1] = 0x3; 
  expected[2] = 0x2; expected[3] = 0;
  expected[4] = 0x3; expected[5] = 0x3;

  printf("P1 = ");
  for(i = 0; i < 5; i++) {
    if(poly1[i] != 0)
      printf("%#2.2x * X^%d + ", poly1[i], i);
  }
  printf("\b\b\b  \n");
  
  printf("P2 = ");
  for(i = 0; i < 2; i++)
    if(poly2[i] != 0)
      printf("%#2.2x * X^%d + ", poly2[i], i);
  printf("\b\b\b  \n");

  polyProduct(poly1, poly2, result, 5);

  printf("P1 * P2 = ");
  for(i = 0; i < 6; i++)
    /*    if(result[i] != 0)*/
      printf("%#2.2x * X^%d + ", result[i], i);
  printf("\b\b\b  \n");
  printf("Expected = ");
  test = 1;
  for(i = 0; i < 6; i++) {
    if(expected[i])
      printf("%#2.2x * X^%d + ", expected[i], i);
    test &= expected[i] == result[i];
  }
  printf("\b\b\b  \nTest product... %s\n", test?"OK":"KO");
  test_all = test;

  printf("P1 = ");
  for(i = 0; i < 5; i++) {
    if(poly1[i] != 0)
      printf("%#2.2x * X^%d + ", poly1[i], i);
  }
  printf("\b\b\b  \n");
  
  printf("P2 = ");
  for(i = 0; i < 2; i++)
    if(poly2[i] != 0)
      printf("%#2.2x * X^%d + ", poly2[i], i);
  printf("\b\b\b  \n");

  printf("0x5 * P1 * P2 = ");
  polyScalMult(result, 0x5, 5);
  expected[0] = 0x5; expected[1] = 0xf; expected[2] = 0xa;
  expected[3] = 0x0; expected[4] = 0xf; expected[5] = 0xf;
  for(i = 0; i < 6; i++)
    if(result[i])
      printf("%#2.2x * X^%d + ", result[i], i);
  printf("\b\b\b  \n");
  printf("Expected = ");
  test = 1;
  for(i = 0; i < 6; i++) {
    if(expected[i]) {
      printf("%#2.2x * X^%d + ", expected[i], i);
    }
    test &= expected[i] == result[i];
  }
  printf("\b\b\b  \nTest scalar product... %s\n", test?"OK":"KO");
  test_all &= test;
  
  printf("0x5 * P1 * P2 +  0x5 * P1 * P2  = ");
  polyAdd(result, result, result, 5);
  isZero = 1;
  for(i = 0; i < 6; i++) {
    if(result[i]) {
      printf("%#2.2x * X^%d + ", result[i], i);
      isZero = 0;
    }
  }
  if(isZero) printf("0\n");
  else printf("\b\b\b  \n");
  printf("Expected = 0\n");
  test = 1;
  for(i = 0; i < 6; i++) {
    test &= 0 == result[i];
  }
  printf("Test addition... %s\n", test?"OK":"KO");
  test_all &= test;

  isZero = 1;
  printf("PolySbox(X) = ");
  for(i = 0; i < 256; i++) {
    if(polySbox[i]) {
      printf("%#2.2x * X^%d + ", polySbox[i], i);
      isZero = 0;
    }
  }
  if(isZero) printf("0\n");
  else printf("\b\b\b  \n");

  revertTab(polySbox, 256);

  printf("PolySbox(0) = %#2.2x\n", evalPolySbox(polySbox, 0));
  printf("PolySbox(1) = %#2.2x\n", evalPolySbox(polySbox, 1));
  test = 1;
  for(i = 0; i < 256; i++) {
    test &= evalPolySbox(polySbox, i) == add(mult(0x2, square(i)), 0x3);
  }
  printf("Test evalPolySbox... %s\n", test?"OK":"KO");
  test_all &= test;
  
  buildSbox(polySbox, sbox);
  test = 1;
  for(i = 0; i < 256; i++) {
    test &= sbox[i] == add(mult(0x2, square(i)), 0x3);
  }
  printf("Test buildSbox... %s\n", test?"OK":"KO");
  test_all &= test;

  for(i = 0; i < 256; i++) {
    expected[i] = 0;
    sbox[i] = 0;
  }
  expected[0] = 0x4; expected[1] = 0x2; expected[2] = 0x3;
  isZero = 1;
  
  printf("expected PolySbox(X) = ");
  for(i = 0; i < 3; i++) {
    if(expected[i]) {
      printf("%#2.2x * X^%d + ", expected[i], i);
      isZero = 0;
    }
  }
  if(isZero) printf("0\n");
  else printf("\b\b\b  \n");
  revertTab(expected, 256);
  printf("PolySbox(0) = %#2.2x\n", sbox[0] = evalPolySbox(expected, 0));
  printf("PolySbox(1) = %#2.2x\n", sbox[1] = evalPolySbox(expected, 1));
  printf("PolySbox(2) = %#2.2x\n", sbox[2] = evalPolySbox(expected, 2));
  revertTab(expected, 256);
  printf("sbox = {");
  for(i = 0; i < 3; i++) {
    printf("%#2.2x, ", sbox[i]);
  }
  printf("\b\b} \n");
  lagrange(sbox, polySbox, 2);
  printf("PolySbox(X) = ");
  isZero = test = 1;
  for(i = 0; i < 3; i++) {
    if(polySbox[i]) {
      printf("%#2.2x * X^%d + ", polySbox[i], i);
      isZero = 0;
    }
    test &= polySbox[i] == expected[i];
  }
  if(isZero) printf("0\n");
  else printf("\b\b\b  \n");
  printf("Test Lagrange... %s\n", test?"OK":"KO");
  test_all &= test;

  buildPolySbox(sbox, polySbox);
  test = 1;
  printf("PolySbox(X) = ");
  for(i = 0; i < 256; i++) {
    if(polySbox[i]) {
      printf("%#2.2x * X^%d + ", polySbox[i], i);
      isZero = 0;
    }
  }
  if(isZero) printf("0\n");
  else printf("\b\b\b  \n");
  for(i = 0; i < 256; i++) {
    test &= sbox[i] == evalPolySbox(polySbox, i);
  }
  printPolySbox(sbox);
  printf("Test buildPolySbox... %s\n", test?"OK":"KO");
  test_all &= test;

  /* Second test de buildPolySbox(Sbox, polySbox) */
  /* Génération d'une SBox */
  printf("Génération de la Sbox... ");
  for(i=0; i < 256; i++) {
    rpool[i] = 1;
  }
  for(i=0; i < 256; i++) {
    /* TODO Change rand function */
    byte candidate = rand();
    if(rpool[candidate]) {
      rpool[candidate] = 0;
      sbox[i] = candidate;
    } else {
      i--;
    }
  }
  printf(" Fait\n");

  printf("Calcul de la Sbox sous forme polynomiale... ");
  buildPolySbox(sbox, polySbox);
  printf(" Fait\n");
  
  test = 1;
  for(i = 0; i < 256; i++) {
    test &= sbox[i] == evalPolySbox(polySbox, i);
  }
    printf("Test buildPolySbox... %s\n", test?"OK":"KO");

  printf("Test Sbox Tools... %s\n", test_all?"ALL OK":"KO");
  return !test_all;
}
