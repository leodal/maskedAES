#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "secureAESlike.h"

int main() {
  int i;
  byte lin[2][2];
  byte sbox[256];
  byte x[2], y[2], e[2], key[4];
  int test = 1;
  lin[0][0] = 0x03; lin[0][1] = 0x04;
  lin[1][0] = 0x05; lin[1][1] = 0x06;
    key[0] = 0x00; key[1] = 0x00; key[2] = 0x00; key[3] = 0x00;
  /*  key[0] = 0xbb; key[1] = 0xbb; key[2] = 0xbb; key[3] = 0xbb; */
  x[0] = 1; x[1] = 2;
  sbox[0] = 0; sbox[1] = 2;
  for(i = 2; i < 256; i++)
    sbox[i] = 0;
  e[0] = 0xe4; e[1] = 0x44;
  /* srand((unsigned int) time (NULL)); */
  srand(100);

  loadSecureAES(lin, sbox);
  setKey(key);
  secureAES(x, y);
  printf("Result :   [ ");
  for(i = 0; i<2; i++)
    printf("%#02.2x ", y[i]);
  printf("]\n");
  printf("Expected : [ ");
  for(i = 0; i<2; i++) {
    printf("%#02.2x ", e[i]);
    test &= e[i] == y[i];
  }
  printf("] ... %s\n", (test?"OK":"KO"));
  return !test;
}
