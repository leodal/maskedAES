#include "aes.h"

#include <stdio.h>

int main() {
  int passed = 1;
  byte in[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  byte out[16];
  byte key[16];
  printf("Test de AES\n");
  printf("============\n");
  printf("KEY_SIZE = %d\nNB_ROUNDS = %d\n", KEY_SIZE, NB_ROUNDS);
  /*  printf("STATE_SIZE = %D\nEXTENDED_KEY_SIZE = %d\nWSIZE = %d\n", STATE_SIZE, EXTENDED_KEY_SIZE, WSIZE);*/
  printf("============\n");
  printf("Fonction affine de la S box\t");
  passed &= aes_affine(0x07) == coron_affine(0x07);
  if(passed) printf("OK\n"); else printf("KO\n");
  
  printf("=============\n");
  printf(" Tableau de mémoire\n");
  aes(in, out, key);
  return 0;
}
