#include "gf256.h"
#include "aes.h"
#include <stdio.h>

int main() {
  genMaskTable();
  printf("\n\n");
  genSquareTable();
  printf("\n\n");
  format_affine();
}
