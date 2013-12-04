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

int test_var(byte a, byte* a_shares) {
  int i;
  byte sum;
#ifdef TEST_VERBOSE
  printMem();
  printf("\n");
#endif
  sum = 0;
  for(i = 0; i<SHARES; i++)
    sum ^= a_shares[i];
#ifdef TEST_VERBOSE
  printf("Somme = %#02.2x\n", sum);
#endif
  return sum == a && collapse(a_shares) == a; 
}

int test(byte a, byte* a_shares, byte b, byte* b_shares, byte c, byte* c_shares) {
  int local_test, pass_all = 1;
  printf("******* %d shares *************\n", SHARES);
  expand(a, a_shares);
  expand(b, b_shares);
  expand(c, c_shares);
  local_test = test_var(a, a_shares);
  local_test &= test_var(b, b_shares);
  pass_all &= local_test;
  printf("Expand and collapse... %s\n", testPP(local_test));
  refresh(a_shares);
  local_test = test_var(a, a_shares);
  pass_all &= local_test;
  printf("Refresh... %s\n", testPP(local_test));
  secAdd(a_shares, b_shares, c_shares);
  local_test = test_var(add(a, b), c_shares);
  pass_all &= local_test;
  printf("Sum of a and b in gf256 : %#02.2x + %#02.2x = %#02.2x (=?= %#02.2x)... %s\n", a, b, add(a, b), collapse(c_shares), testPP(local_test));
  secAdd(a_shares, b_shares, a_shares);
  local_test = test_var(a=add(a, b), a_shares);
  pass_all &= local_test;
  printf("Sum of a and b in gf256, result in a : %#02.2x + %#02.2x = %#02.2x (=?= ^)... %s\n", add(a, b), b, a, testPP(local_test));
  secSquare(a_shares, c_shares+10);
  local_test = test_var(square(a), c_shares+10);
  pass_all &= local_test;
  secSquare(b_shares, c_shares+10);
  local_test &= test_var(square(b), c_shares+10);
  pass_all &= local_test;
  printf("Square of a and b in gf256 : %#02.2x^2 = %#02.2x (=?= ^^), %#02.2x^2 = %#02.2x (=?= ^)... %s\n", a, square(a), b, square(b), testPP(local_test));
  expand(0x0, c_shares);
  secSquare(c_shares, c_shares+10);
  local_test = test_var(square(c), c_shares+10);
  pass_all &= local_test;
  printf("Square of 0 in gf256 : %#02.2x^2 = %#02.2x (=?= ^)... %s\n", c, square(c), testPP(local_test));
  secSquare(b_shares, b_shares);
  local_test = test_var(square(b), b_shares);
  pass_all &= local_test;
  printf("Square of b in gf256, result in b : %#02.2x^2 = %#02.2x (=?= ^)... %s\n", b, square(b), testPP(local_test));
  b = square(b);
  secMult(a_shares, b_shares, c_shares+10);
  local_test = test_var(mult(a, b), c_shares+10);
  pass_all &= local_test;
  printf("Product of a and b in gf256 : %#02.2x * %#02.2x = %#02.2x (=?= ^)... %s\n", a, b, mult(a, b), testPP(local_test));
  expand(0x0, c_shares);
  secMult(c_shares, b_shares, c_shares+10);
  local_test = test_var(mult(c, b), c_shares+10);
  pass_all &= local_test;
  printf("Product of 0 and b in gf256 : %#02.2x * %#02.2x = %#02.2x (=?= %#02.2x)... %s\n", collapse(c_shares), b, mult(c, b), collapse(c_shares+10), testPP(local_test));
  secMult(a_shares, c_shares, c_shares+10);
  local_test = test_var(mult(a, c), c_shares+10);
  pass_all &= local_test;
  printf("Product of a and 0 in gf256 : %#02.2x * %#02.2x = %#02.2x (=?= ^)... %s\n", a, c, mult(a, c), testPP(local_test));
  secMult(a_shares, b_shares, a_shares);
  local_test = test_var(a=mult(a, b), a_shares);
  pass_all &= local_test;
  printf("Product of a and b in gf256, result in a : %#02.2x * %#02.2x = %#02.2x (=?= ^)... %s\n", mult(a, b), b, a, testPP(local_test));
  
  secExp254(a_shares, c_shares+10);
  local_test = test_var(inverse(a), c_shares+10);
  pass_all &= local_test;
  secExp254(b_shares, c_shares+10);
  local_test &= test_var(inverse(b), c_shares+10);
  pass_all &= local_test;
  printf("Inverse of a and b in gf256 : %#02.2x^-1 = %#02.2x (=?= ^^), %#02.2x^-1 = %#02.2x (=?= ^)... %s\n", a, inverse(a), b, inverse(b), testPP(local_test));
  secExp254(c_shares, c_shares+10);
  local_test = test_var(inverse(c), c_shares+10);
  pass_all &= local_test;
  printf("Inverse of 0 in gf256 : %#02.2x^-1 = %#02.2x (=?= ^)... %s\n", c, inverse(c), testPP(local_test));
  secExp254(b_shares, b_shares);
  local_test = test_var(inverse(b), b_shares);
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
  
  pass_all = test(a, shares, b, shares+10, c, shares+20);

  printf("Test %s\n", (pass_all?"OK":"KO"));
  return !pass_all;
}
