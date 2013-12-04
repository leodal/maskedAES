#ifndef LINEAR_SIZE
#define LINEAR_SIZE 2
#endif
#ifndef NB_ROUNDS
#define NB_ROUNDS 1
#endif

#include "aesLike.h"
#include "gf256.h"
#include <stdio.h>

#define testPP(test)  (test ? "OK" : "KO")

int main() {
  int i, j, passed;
  byte linear[LINEAR_SIZE][LINEAR_SIZE];
  byte Sbox[256];
  byte x[LINEAR_SIZE], y[LINEAR_SIZE], expected[LINEAR_SIZE];
  byte key[LINEAR_SIZE*(NB_ROUNDS+1)];
  printf("##############################\n");
  printf("NB_ROUNDS = %d\n", NB_ROUNDS);
  printf("LINEAR_SIZE = %d\n", LINEAR_SIZE);
  printf("##############################\n");

  printf("Définition de la matrice :\n");
  linear[0][0] = 0x03; linear[0][1] = 0x04;
  linear[1][0] = 0x05; linear[1][1] = 0x06;
  for(i=0; i<LINEAR_SIZE; i++) {
    printf("[ ");
    for(j=0; j<LINEAR_SIZE; j++) {
      printf("%d ", linear[i][j]);
    }
    printf("]\n");
  }
  printf("Sbox mise à 0xff+0xfe*x pour l'instant\n");
  Sbox[0] = 0xff; Sbox[1] = 0xfe;
  for(i=2; i<256; i++)
    Sbox[i] = 0;
  printf("définition du texte clair :");
  x[0] = 0x1; x[1] = 0x2;
  printf("[ ");
  for(i=0; i<LINEAR_SIZE; i++) {
    printf("%d ", x[i]);
  }
  printf("]\n");
/*   expected[0] = add(mult(0x3, 0xff^mult(0xfe, 0xb)), mult(0x4, 0xff^mult(0xfe, 0x9))); */
/*   expected[1] = add(mult(0x5, 0xff^mult(0xfe, 0xb)), mult(0x6, 0xff^mult(0xfe, 0x9))); */
/*   printf("Résultat attendu : [ "); */
/*   for(i = 0; i < LINEAR_SIZE ; i++) { */
/*     printf("%#2.2x ", expected[i]); */
/*   } */
/*   printf("]\n"); */
/*   expected[0] = add(mult(0x3, 0xff^mult(0xfe, 0x39)), mult(0x4, 0xff^mult(0xfe, 0x11))); */
/*   expected[1] = add(mult(0x5, 0xff^mult(0xfe, 0x39)), mult(0x6, 0xff^mult(0xfe, 0x11))); */
/*   printf("Résultat attendu : [ "); */
/*   for(i = 0; i < LINEAR_SIZE ; i++) { */
/*     printf("%#2.2x ", expected[i]); */
/*   } */
/*   printf("]\n"); */
/*   expected[0] = add(mult(0x3, 0xf), mult(0x4, 0xbb)); */
/*   expected[1] = add(mult(0x5, 0xf), mult(0x6, 0xbb)); */
/*   printf("Résultat attendu : [ "); */
/*   for(i = 0; i < LINEAR_SIZE ; i++) { */
/*     printf("%#2.2x ", expected[i]); */
/*   } */
/*   printf("]\n"); */
  printf("Chargement de l'algo :\n");
  loadAESlike(linear, Sbox);
  printf("Test de la Sbox :\n");
  expected[0] = 0xff^mult(0xfe, 0x1);
  expected[1] = 0xff^mult(0xfe, 0x2);  
  for(i = 0; i < LINEAR_SIZE ; i++) {
    y[i] = evalSbox(x[i]);
    printf("Sbox(%#2.2x) = %#2.2x\n", x[i], evalSbox(x[i]));
  }
  printf("Résultat attendu : [ ");
  for(i = 0; i < LINEAR_SIZE ; i++) {
    printf("%#2.2x ", expected[i]);
  }
  printf("]\n");
  passed = 1;
  printf("Résultat obtenu : [ ");
  for(i = 0; i < LINEAR_SIZE ; i++) {
    passed &= y[i] == expected[i];
    printf("%#2.2x ", y[i]);
  }
  printf("]\n");
  printf("Test de sBox... %s\n", testPP(passed));

  printf("Chargement de la clef en mémoire :\n");
  loadKey(key);

  expected[0] = add(mult(0x3, 0xff^mult(0xfe, 0x1)), mult(0x4, 0xff^mult(0xfe, 0x2)));
  expected[1] = add(mult(0x5, 0xff^mult(0xfe, 0x1)), mult(0x6, 0xff^mult(0xfe, 0x2)));
  printf("Résultat attendu : [ ");
  for(i = 0; i < LINEAR_SIZE ; i++) {
    printf("%#2.2x ", expected[i]);
  }
  printf("]\n");
  expected[0] = add(mult(0x3, 0xff^mult(0xfe, 0x63)), mult(0x4, 0xff^mult(0xfe, 0x55)));
  expected[1] = add(mult(0x5, 0xff^mult(0xfe, 0x63)), mult(0x6, 0xff^mult(0xfe, 0x55)));
  printf("Résultat attendu : [ ");
  for(i = 0; i < LINEAR_SIZE ; i++) {
    printf("%#2.2x ", expected[i]);
  }
  printf("]\n");

  printf("Exécution de l'algo :\n");
  aesLike(x, y);
  passed = 1;
  printf("Résultat : [ ");
  for(i=0; i<LINEAR_SIZE; i++) {
    printf("%#2.2x ", y[i]);
    passed &= y[i] == expected[i];
  }
  printf("]\n");
  printf("Résultat attendu ? %s\n", (passed?"Yes":"No"));
  return !passed;
}
