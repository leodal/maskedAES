#include "aesLike.h"
#include "gf256.h"
#include "sbox_tools.h"
#include <stdio.h>

#define testPP(test)  (test ? "OK" : "KO")

int main() {
  int i, j, passed;
  byte e0, e1;
  byte linear[LINEAR_SIZE][LINEAR_SIZE];
  byte Sbox[256], Sbox_poly[256];
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
  printf("Sbox mise à 0xff+0xfe*x\n");
  for(i=2; i<256; i++)
    Sbox_poly[i] = 0;
  Sbox_poly[0] = 0xff; Sbox_poly[1] = 0xfe;
  for(i=2; i<256; i++)
    Sbox[i] = 0;
  /* conversion */
  revertTab(Sbox_poly, 256);
  buildSbox(Sbox_poly, Sbox);

  printf("Définition de la clef : [ ");
  for(i = 0; i < LINEAR_SIZE*(NB_ROUNDS+1); i++) {
    key[i] = i;
    printf("%#2.2x ", key[i]);
  }
  printf("]\n");

  printf("définition du texte clair :");
  x[0] = 0x1; x[1] = 0x2;
  printf("[ ");
  for(i=0; i<LINEAR_SIZE; i++) {
    printf("%d ", x[i]);
  }
  printf("]\n");
  printf("Chargement de l'algo :\n");
  loadAESlike(linear, Sbox);
  printf("Test de la Sbox :\n");
  expected[0] = 0xff^mult(0xfe, 0x1);
  expected[1] = 0xff^mult(0xfe, 0x2);  
  for(i = 0; i < LINEAR_SIZE ; i++) {
    y[i] = evalSbox(x[i]);
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
  printf("Clef : [ ");
  for(i = 0; i < LINEAR_SIZE*(NB_ROUNDS+1); i++)
    printf("%#2.2x ", key[i]);
  printf("]\n");

  expected[0] = add(0x2, add(mult(0x3, 0xff^mult(0xfe, 0x1)), mult(0x4, 0xff^mult(0xfe, add(0x2, 0x1)))));
  expected[1] = add(0x3, add(mult(0x5, 0xff^mult(0xfe, 0x1)), mult(0x6, 0xff^mult(0xfe, add(0x2, 0x1)))));
  printf("Résultat attendu round 1 : [ ");
  for(i = 0; i < LINEAR_SIZE ; i++) {
    printf("%#2.2x ", expected[i]);
  }
  printf("]\n");
  e0 = expected[0]; e1 = expected[1];
  expected[0] = add(0x4, add(mult(0x3, 0xff^mult(0xfe, e0)), mult(0x4, 0xff^mult(0xfe, e1))));
  expected[1] = add(0x5, add(mult(0x5, 0xff^mult(0xfe, e0)), mult(0x6, 0xff^mult(0xfe, e1))));
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
