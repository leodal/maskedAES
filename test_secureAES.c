#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "secureAES.h"

int main() {
  int i;
  byte lin[2][2];
  byte x[2];
  byte y[2];
  lin[0][0] = 0x03; lin[0][1] = 0x04;
  lin[1][0] = 0x05; lin[1][1] = 0x06;
  x[0] = 1; x[1] = 2;
  
  /* srand((unsigned int) time (NULL)); */
  srand(100);

  loadSecureAES(lin);
  secureAES(x, y);
  printf("Result : ");
  for(i = 0; i<2; i++)
    printf("%#02.2x ", y[i]);
  printf("\n");
  return 0;
}
