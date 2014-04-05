#include "gf256.h"
#include <stdio.h>

int main() {
  genExpLogTables(0xE5);
  genMaskTable();
  printf("\n");
  genSquareTable();
  printf("\n");
  genPow4Table();
  return 0;
}
