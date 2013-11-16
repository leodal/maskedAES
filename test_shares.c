#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "shares.h"

#define testPP(test)  (test ? "OK" : "KO")

#define MEM_SIZE 50

byte shares[MEM_SIZE];

void printMem() {
  int i;
  for(i = 0; i<30; i++)
    printf("%#02.2x ", shares[i]);
}

int test_var(int n, byte a, byte* a_shares) {
  int i;
  byte sum;
#ifdef TEST_VERBOSE
  printMem();
  printf("\n");
#endif
  sum = 0;
  for(i = 0; i<n; i++)
    sum ^= a_shares[i];
#ifdef TEST_VERBOSE
  printf("Somme = %#02.2x\n", sum);
#endif
  return sum == a && collapse(a_shares, n) == a; 
}

int test_n(int n, byte a, byte* a_shares, byte b, byte* b_shares, byte c, byte* c_shares) {
  int local_test, pass_all = 1;
  printf("******* %d shares *************\n", n);
  expand(a, a_shares, n);
  expand(b, b_shares, n);
  expand(c, c_shares, n);
  local_test = test_var(n, a, a_shares);
  local_test &= test_var(n, b, b_shares);
  pass_all &= local_test;
  printf("Expand and collapse... %s\n", testPP(local_test));
  refresh(a_shares, n);
  local_test = test_var(n, a, a_shares);
  pass_all &= local_test;
  printf("Refresh... %s\n", testPP(local_test));
  secAdd(a_shares, b_shares, c_shares+10, n);
  local_test = test_var(n, add(a, b), c_shares+10);
  pass_all &= local_test;
  printf("Sum of a and b in gf256 : %#02.2x + %#02.2x = %#02.2x (=?= ^)... %s\n", a, b, add(a, b), testPP(local_test));
  secAdd(a_shares, b_shares, a_shares, n);
  local_test = test_var(n, a=add(a, b), a_shares);
  pass_all &= local_test;
  printf("Sum of a and b in gf256, result in a : %#02.2x + %#02.2x = %#02.2x (=?= ^)... %s\n", add(a, b), b, a, testPP(local_test));
  secSquare(a_shares, c_shares+10, n);
  local_test = test_var(n, square(a), c_shares+10);
  pass_all &= local_test;
  secSquare(b_shares, c_shares+10, n);
  local_test &= test_var(n, square(b), c_shares+10);
  pass_all &= local_test;
  printf("Square of a and b in gf256 : %#02.2x^2 = %#02.2x (=?= ^^), %#02.2x^2 = %#02.2x (=?= ^)... %s\n", a, square(a), b, square(b), testPP(local_test));
  secSquare(c_shares, c_shares+10, n);
  local_test = test_var(n, square(c), c_shares+10);
  pass_all &= local_test;
  printf("Square of 0 in gf256 : %#02.2x^2 = %#02.2x (=?= ^)... %s\n", c, square(c), testPP(local_test));
  secSquare(b_shares, b_shares, n);
  local_test = test_var(n, square(b), b_shares);
  pass_all &= local_test;
  printf("Square of b in gf256, result in b : %#02.2x^2 = %#02.2x (=?= ^)... %s\n", b, square(b), testPP(local_test));
  b = square(b);
  secMult(a_shares, b_shares, c_shares+10, n);
  local_test = test_var(n, mult_log(a, b), c_shares+10);
  pass_all &= local_test;
  printf("Product of a and b in gf256 : %#02.2x * %#02.2x = %#02.2x (=?= ^)... %s\n", a, b, mult_log(a, b), testPP(local_test));
  secMult(c_shares, b_shares, c_shares+10, n);
  local_test = test_var(n, mult_log(c, b), c_shares+10);
  pass_all &= local_test;
  printf("Product of 0 and b in gf256 : %#02.2x * %#02.2x = %#02.2x (=?= ^)... %s\n", c, b, mult_log(c, b), testPP(local_test));
  secMult(a_shares, c_shares, c_shares+10, n);
  local_test = test_var(n, mult_log(a, c), c_shares+10);
  pass_all &= local_test;
  printf("Product of a and 0 in gf256 : %#02.2x * %#02.2x = %#02.2x (=?= ^)... %s\n", a, c, mult_log(a, c), testPP(local_test));
  secMult(a_shares, b_shares, a_shares, n);
  local_test = test_var(n, a=mult_log(a, b), a_shares);
  pass_all &= local_test;
  printf("Product of a and b in gf256, result in a : %#02.2x * %#02.2x = %#02.2x (=?= ^)... %s\n", mult_log(a, b), b, a, testPP(local_test));
  
  secExp254(a_shares, c_shares+10, n);
  local_test = test_var(n, inverse(a), c_shares+10);
  pass_all &= local_test;
  secExp254(b_shares, c_shares+10, n);
  local_test &= test_var(n, inverse(b), c_shares+10);
  pass_all &= local_test;
  printf("Inverse of a and b in gf256 : %#02.2x^-1 = %#02.2x (=?= ^^), %#02.2x^-1 = %#02.2x (=?= ^)... %s\n", a, inverse(a), b, inverse(b), testPP(local_test));
  secExp254(c_shares, c_shares+10, n);
  local_test = test_var(n, inverse(c), c_shares+10);
  pass_all &= local_test;
  printf("Inverse of 0 in gf256 : %#02.2x^-1 = %#02.2x (=?= ^)... %s\n", c, inverse(c), testPP(local_test));
  secExp254(b_shares, b_shares, n);
  local_test = test_var(n, inverse(b), b_shares);
  pass_all &= local_test;
  printf("Inverse of b in gf256, result in b : %#02.2x^-1 = %#02.2x (=?= ^)... %s\n", b, inverse(b), testPP(local_test));
  b = inverse(b);
  printf("******************************\n");
  return pass_all;
}

int main() {
  byte a, b, c;
  int pass_all = 1;

  srand((unsigned int) time (NULL));
  a = rand(); b = rand(); c = 0;
  printf("#####################\n");
  printf("a = %d = %#02.2x\n", a, a);
  printf("b = %d = %#02.2x\n", b, b);
  printf("c = %d = %#02.2x\n", c, c);
  printf("#####################\n");
  
  pass_all &= test_n(1, a, shares, b, shares+10, c, shares+20);
  pass_all &= test_n(3, a, shares, b, shares+10, c, shares+20);
  pass_all &= test_n(5, a, shares+4, b, shares+14, c, shares+24);

  printf("Test %s\n", (pass_all?"OK":"KO"));
  return !pass_all;
}
