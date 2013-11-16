#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "gf256.h"
#include "shares.h"
#include "secureOps.h"

int main() {
  int i;
  byte sum;
  int order = 5;

  byte a = 255;
  byte b = 3;
  byte c = 2;
  byte d = 6;
  byte e = 229;
  byte sec_a[order];
  byte sec_b[order];
  byte sec_r[order];
  printf("============================\n");
  printf("= Test GF256 ===============\n");
  printf("a = %d\n", a);
  printf("b = %d\n", b);
  printf("c = %d\n", c);
  printf("d = %d\n", d);
  printf("e = %d\n", e);
  printf("= Test Opérations GF256 ===============\n");
  printf("a + b = %d\n", add(a, b));
  printf("a * b = %d\n", mult(b, a));
  printf("b * c = %d\n", mult(b, c));
  printf("a * c = %d\n", mult(a, c));
  printf("b * c = %d (Log version)\n", mult_log(b, c));
  printf("a * c = %d (Log version)\n", mult_log(a, c));
  printf("d / c = %d\n", divide(d, c));
  printf("e / c = %d\n", divide(e, c));
  printf("e / a = %d\n", divide(e, a));
  printf("e^-1 = %d\n", inverse(e));
  printf("d^2 = %d (Log version)\n", square_log(d));
  printf("a^2 = %d (Log version)\n", square_log(a));
  printf("d^2 = %d\n", square(d));
  printf("a^2 = %d\n", square(a));
  printf("a^254 = %d\n", pow254(a));
  printf("a^-1 = %d\n", inverse(a));
  printf("============================\n");
  printf("= Test SecureOps ===============\n");
  srand((unsigned int) time (NULL));
  expand(a, sec_a, order);
  expand(b, sec_b, order);
  sum = 0;
  printf("sec_a = ");
  for(i = 0; i < order; i++) {
    sum = add(sec_a[i], sum);
    printf("%d ", sec_a[i]);
  }
  printf(" (= %d)\n", sum);

  sum = 0;
  printf("sec_b = ");
  for(i = 0; i < order; i++) {
    sum = add(sec_b[i], sum);
    printf("%d ", sec_b[i]);
  }
  printf(" (= %d)\n", sum);
  
  secAdd(sec_a, sec_b, sec_r, order);
  sum = 0;
  printf("sec_a + sec_b = ");
  for(i = 0; i < order; i++) {
    sum = add(sec_r[i], sum);
    printf("%d ", sec_r[i]);
  }
  printf(" (= %d)\n", sum);

  secSquare(sec_a, sec_r, order);
  sum = 0;
  printf("sec_a^2 = ");
  for(i = 0; i < order; i++) {
    sum = add(sec_r[i], sum);
    printf("%d ", sec_r[i]);
  }
  printf(" (= %d)\n", sum);

  secMult(sec_a, sec_b, sec_r, order);
  sum = 0;
  printf("sec_a * sec_b = ");
  for(i = 0; i < order; i++) {
    sum = add(sec_r[i], sum);
    printf("%d ", sec_r[i]);
  }
  printf(" (= %d)\n", sum);

  secExp254(sec_a, sec_r, order);
  sum = 0;
  printf("sec_a ^ 254 = ");
  for(i = 0; i < order; i++) {
    sum = add(sec_r[i], sum);
    printf("%d ", sec_r[i]);
  }
  printf(" (= %d)\n", sum);
  return 0;
}
