#include "aesLike.h"
#include "sbox_tools.h"
#include "secureAESlike.h"
#include "gf256.h"
#include <stdio.h>

#include <stdlib.h>
#include <time.h>

#define testPP(test)  (test ? "OK" : "KO")

#define NB_EXECS 500

int main() {
  int i,j;

  byte sbox[256], polySbox[256];
  byte rpool[256];
  byte linear[LINEAR_SIZE][LINEAR_SIZE];
  byte key[LINEAR_SIZE*(NB_ROUNDS+1)];
  byte x[LINEAR_SIZE], e[LINEAR_SIZE], y[LINEAR_SIZE];

  int test, test_all = 1;
  clock_t start;
  double duration;

  srand((unsigned int) time (NULL));
  printf("##############################\n");
  printf("NB_ROUNDS = %d\n", NB_ROUNDS);
  printf("LINEAR_SIZE = %d\n", LINEAR_SIZE);
  printf("SHARES = %d\n", SHARES);
  printf("##############################\n");
  
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
  printf("Fait\n");
  printf("Transformation de la Sbox vers une version polynomiale... ");
  buildPolySbox(sbox, polySbox);
  printf("Fait\n");
  test = 1;
  printf("Validation... ");
  for(i = 0; i < 256; i++) {
    test &= sbox[i] == evalPolySbox(polySbox, i);
  }
  printf("%s\n", test?"OK":"KO");
  test_all &= test;

  printf("Génération de la fonction linéaire... ");
  for(i = 0; i<LINEAR_SIZE; i++) {
    for(j = 0; j<LINEAR_SIZE; j++) {
      linear[i][j] = rand();
    }
  }
  printf("Fait\n");
  printf("Génération de la clef : [ ");
  for(j = 0; j<LINEAR_SIZE*(NB_ROUNDS+1); j++) {
    printf("%#2.2x ", key[j] = rand());
  }
  printf("]\n");
  printf("Génération du clair : [ ");
  for(j = 0; j<LINEAR_SIZE; j++) {
    printf("%#2.2x ", x[j] = rand());
  }
  printf("]\n");
   
  printf("Chargement des algorithmes\n");
  printf("\tAES-like\n");
  loadAESlike(linear, sbox);
  loadKey(key);
  printf("\tVersion secure\n");
  loadSecureAES(linear, polySbox);
  setKey(key);

  printf("Exécution des algorithmes\n");
  printf("\tAES-like...");
  aesLike(x, e);
  printf("Fait\n");
  printf("\tResult : [ ");
  for(i = 0; i<LINEAR_SIZE; i++) {
    printf("%#2.2x ", e[i]);
  }
  printf("]\n");
  printf("Version secure...");
  secureAES(x, y);
  printf("Fait\n");
  printf("\tResult :   [ ");
  for(i = 0; i<LINEAR_SIZE; i++)
    printf("%#2.2x ", y[i]);
  printf("]\n");
  printf("Test d'égalité : ");
  test = 1;
  for(i = 0; i<LINEAR_SIZE; i++) {
    test &= e[i] == y[i];
  }
  printf("%s\n", test?"OK":"KO");
  test_all &= test;

  printf("Mesures de performances (%d executions) :\n");
  printf("\tAES-like...");
  start = clock();
  for(i = 0; i<NB_EXECS; i++)
    aesLike(x, e);
  duration = (double)(clock()-start)/(double)CLOCKS_PER_SEC;
  printf("\nTemps total : %.2f seconde(s)\n", duration);
  printf("\tVersion secure...");
  start = clock();
  for(i = 0; i<NB_EXECS; i++)
    secureAES(x, e);
  duration = (double)(clock()-start)/(double)CLOCKS_PER_SEC;
  printf("\nTemps total : %.2f seconde(s)\n", duration);
  return !test_all;
}
