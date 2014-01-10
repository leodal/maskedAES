#include "aesLike.h"
#include "sbox_tools.h"
#include "secureAESlike.h"
#include "gf256.h"
#include <stdio.h>

#include <stdlib.h>
#include <time.h>

#define testPP(test)  (test ? "OK" : "KO")

int main() {
  int i, j, eg;
  byte linear[LINEAR_SIZE][LINEAR_SIZE];
  byte Sbox[256], polySbox[256];
  byte key[LINEAR_SIZE*(NB_ROUNDS+1)];
  byte x[LINEAR_SIZE], y[LINEAR_SIZE], r[LINEAR_SIZE];

  byte rpool[256];
  double duration;
  clock_t start;

  srand((unsigned int) time (NULL));
  printf("##############################\n");
  printf("NB_ROUNDS = %d\n", NB_ROUNDS);
  printf("LINEAR_SIZE = %d\n", LINEAR_SIZE);
  printf("##############################\n");

  printf("Génération de la Sbox :\n");
  for(i=0; i < 256; i++) {
    rpool[i] = 1;
  }
  for(i=0; i < 256; i++) {
    /* TODO Change rand function */
    byte candidate = rand();
#ifdef DEBUG
    printf("Pour i = %d, candidat %d... %s\n", i, candidate, (rpool[candidate]?"LIBRE":"PRIS"));
#endif
    if(rpool[candidate]) {
      rpool[candidate] = 0;
      Sbox[i] = candidate;
    } else {
      i--;
    }
  }
  printf("Générée : ");
  printf("[ ");
  for(j=0; j<256; j++) {
    printf("%d ", Sbox[j]);
  }
  printf("]\n");
  printf("Génération de la matrice :\n");
  for(i=0; i<LINEAR_SIZE; i++) {
    for(j=0; j<LINEAR_SIZE; j++) {
      linear[i][j] = rand();
    }
  }
  printf("Matrice générée !!\n");
  for(i=0; i<LINEAR_SIZE; i++) {
    printf("[ ");
    for(j=0; j<LINEAR_SIZE; j++) {
      printf("%d ", linear[i][j]);
    }
    printf("]\n");
  }
  printf("Génération de la clef :");
  for(i=0; i < LINEAR_SIZE*(NB_ROUNDS+1); i++) {
    key[i] = rand();
    if(i%10 == 0)
      printf("\n");
    printf("%#2.2x ", key[i]);
  }
  printf("\n");
  printf("Chargement des algorithmes\n");
  printf("\tAES-like\n");
  loadAESlike(linear, Sbox);
  loadKey(key);
  printf("\tVersion secure\n");
  printf("\t\tTraduction de la sbox\n");
  buildPolySbox(Sbox, polySbox);
  loadSecureAES(linear, polySbox);
  setKey(key);

  printf("Génération d'un clair :\n");
  for(i=0; i < LINEAR_SIZE; i++) {
    x[i] = rand();
    printf("%#2.2x ", x[i]);
  }
  printf("\n");
  
  printf("Exécution de 1000 chiffrements \"normaux\"");
  start = clock();
  for(i = 0; i < 1000; i++)
    aesLike(x, y);
  duration = (double)(clock()-start)/(double)CLOCKS_PER_SEC;
  printf("\nTemps total : %.2f seconde(s)\n", duration);

  /* Un tour pour rien, pour charger les données */
  printf("Exécution de 1000 chiffrements \"secure\"");
  eg = 1;
  for(i =0; i < LINEAR_SIZE; i++)
    eg &= y[i] == r[i];
  printf("\nMême résultat ? %s", eg?"Oui":"Non");
  secureAES(x, r);
  start = clock();
  for(i = 0; i < 1000; i++)
    runSecureAES();
  /* aesLike(x, y); */
  
  duration = (double)(clock()-start)/(double)CLOCKS_PER_SEC;
  printf("\nTemps total : %.2f seconde(s)\n", duration);
}
