#include "shares.h"

#include <stdlib.h>

void expand(byte a, byte r[], int order) {
  int i;
  byte last = a;
  // the order - 1 first values are random
  for(i = 0; i < order-1; i++){
    // TODO Change rand function
    r[i] = rand();
    // last = last - r[i] = last + r[i]
    last = r[i] ^ last;
  }
  r[order-1] = last;
}

void refresh(byte share[], int order) {
  int i;
  for(i = 1; i < order; i++) {
    byte tmp = rand();
    share[i] ^= tmp;
    share[0] ^= tmp;
  }
}
