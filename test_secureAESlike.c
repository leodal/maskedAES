#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "aesLike.h"
#include "secureAESlike.h"
#include "sbox_tools.h"

int main() {
  int i,j;
  byte lin[2][2];
  byte sbox[256], rpool[256], polySbox[256];
  byte x[2], y[2], e[2], key[6];
  int test = 1, test_all;

  /* First test */
  lin[0][0] = 0x03; lin[0][1] = 0x04;
  lin[1][0] = 0x05; lin[1][1] = 0x06;
  key[0] = 0x00; key[1] = 0x01; key[2] = 0x02; key[3] = 0x03;
  key[4] = 0x04; key[5] = 0x05;
  /* key[0] = 0x0b; key[1] = 0x0b; key[2] = 0x0b; key[3] = 0x0b; */
  x[0] = 1; x[1] = 2;
  for(i = 0; i < 256; i++)
    polySbox[i] = 0;
  polySbox[254] = 0xfe; polySbox[255] = 0xff; 
  /* e[0] = 0xb6; e[1] = 0x67; */ 
  /* e[0] = 0xb4; e[1] = 0x64; */
  e[0] = 0x54; e[1] = 0x98;
  srand((unsigned int) time (NULL));
  /* srand(100); */

  printf("Test version secure...\n");
  printf("Lin :\n");
  for(i = 0; i<2; i++) {
    printf("[");
    for(j = 0; j < 2; j++)
      printf("%#2.2x ", lin[i][j]);
    printf("]\n");
  }
  printf("Key : [ ");
  for(i = 0; i<6; i++)
    printf("%#2.2x ", key[i]);
  printf("]\n");
  printf("Clear :   [ ");
  for(i = 0; i<2; i++)
    printf("%#2.2x ", x[i]);
  printf("]\n");
  loadSecureAES(lin, polySbox);
  setKey(key);
  secureAES(x, y);
  printf("Result :   [ ");
  for(i = 0; i<2; i++)
    printf("%#2.2x ", y[i]);
  printf("]\n");
  printf("Expected : [ ");
  for(i = 0; i<2; i++) {
    printf("%#2.2x ", e[i]);
    test &= e[i] == y[i];
  }
  printf("] ... %s\n", (test?"OK":"KO"));
  test_all = test;

  printf("Test version pas secure...\n");
  buildSbox(polySbox, sbox);
  printf("Validation de la transformation de Sbox... ");
  for(i = 0; i < 256; i++) {
    test &= sbox[i] == evalPolySbox(polySbox, i);
  }
  printf("%s\n", test?"OK":"KO");
  
    printf("Lin :\n");
  for(i = 0; i<2; i++) {
    printf("[");
    for(j = 0; j < 2; j++)
      printf("%#2.2x ", lin[i][j]);
    printf("]\n");
  }
  printf("Key : [ ");
  for(i = 0; i<6; i++)
    printf("%#2.2x ", key[i]);
  printf("]\n");
  printf("Clear :   [ ");
  for(i = 0; i<2; i++)
    printf("%#2.2x ", x[i]);
  printf("]\n");
  loadAESlike(lin, sbox);
  loadKey(key);
  aesLike(x, y);
  printf("Result :   [ ");
  for(i = 0; i<2; i++)
    printf("%#2.2x ", y[i]);
  printf("]\n");
  printf("Expected : [ ");
  for(i = 0; i<2; i++) {
    printf("%#2.2x ", e[i]);
    test &= e[i] == y[i];
  }
  printf("] ... %s\n", (test?"OK":"KO"));
  test_all = test;
  
  /* /\* 2nd test *\/ */
  /* /\* Sbox random generation *\/ */
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
  /* Sbox transformation */
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

  printf("Chargement des algorithmes\n");
  printf("\tAES-like\n");
  loadAESlike(lin, sbox);
  loadKey(key);
  printf("\tVersion secure\n");
  loadSecureAES(lin, polySbox);
  setKey(key);

  printf("Exécution des algorithmes\n");
  printf("\tAES-like...");
  aesLike(x, e);
  printf("Fait\n");
  printf("\tResult : [ ");
  for(i = 0; i<2; i++) {
    printf("%#2.2x ", e[i]);
    test &= e[i] == y[i];
  }
  printf("]\n");
  printf("Version secure...");
  secureAES(x, y);
  printf("Fait\n");
  printf("\tResult :   [ ");
  for(i = 0; i<2; i++)
    printf("%#2.2x ", y[i]);
  printf("]\n");
  printf("Test d'égalité : ");
  test = 1;
  for(i = 0; i<2; i++) {
    test &= e[i] == y[i];
  }
  printf("%s\n", test?"OK":"KO");
  test_all &= test;

  /* 3rd Test : all random */
  printf("\n\n====== All random ============\n");
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
  /* Sbox transformation */
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

  printf("Génération de la fonction linéaire :\n");
  for(i = 0; i<2; i++) {
    printf("[ ");
    for(j = 0; j<2; j++) {
      printf("%#2.2x ", lin[i][j] = rand());
    }
    printf("]\n");
  }
  printf("Génération de la clef : [ ");
  for(j = 0; j<6; j++) {
    printf("%#2.2x ", key[j] = rand());
  }
  printf("]\n");
  printf("Génération du clair : [ ");
  for(j = 0; j<2; j++) {
    printf("%#2.2x ", x[j] = rand());
  }
  printf("]\n");
   
  printf("Chargement des algorithmes\n");
  printf("\tAES-like\n");
  loadAESlike(lin, sbox);
  loadKey(key);
  printf("\tVersion secure\n");
  loadSecureAES(lin, polySbox);
  setKey(key);

    printf("Exécution des algorithmes\n");
  printf("\tAES-like...");
  aesLike(x, e);
  printf("Fait\n");
  printf("\tResult : [ ");
  for(i = 0; i<2; i++) {
    printf("%#2.2x ", e[i]);
    test &= e[i] == y[i];
  }
  printf("]\n");
  printf("Version secure...");
  secureAES(x, y);
  printf("Fait\n");
  printf("\tResult :   [ ");
  for(i = 0; i<2; i++)
    printf("%#2.2x ", y[i]);
  printf("]\n");
  printf("Test d'égalité : ");
  test = 1;
  for(i = 0; i<2; i++) {
    test &= e[i] == y[i];
  }
  printf("%s\n", test?"OK":"KO");
  test_all &= test;

  printf("Test Secure AES-like : %s\n", test_all?"ALL OK":"KO");
  return !test_all;
}
