#include "secureAES.h"

#ifdef DEBUG
#include "gf256.h"
#endif

/**
void secureAES(byte in[], byte out[], byte key[], int n) {
  byte rk[176];

  masked_keyExpansion(key, rk);
  for(i = 0; i < 9; i++) {
    
  }
}
**/
/* Expanded matrix size = LINEAR_SIZE*LINEAR_SIZE*SHARES
   Clear data shares size = LINEAR_SIZE*SHARES
   Cypher data shares size = LINEAR_SIZE*SHARES
   Temporary datas = 2*SHARES
 */
#define WSIZE LINEAR_SIZE*LINEAR_SIZE*SHARES+2*LINEAR_SIZE*SHARES+2*SHARES 
byte workZone[WSIZE];
#define XTENDED_MATRIX_ADDR 0
#define CT_ADDR LINEAR_SIZE*LINEAR_SIZE*SHARES
#define ET_ADDR LINEAR_SIZE*LINEAR_SIZE*SHARES+LINEAR_SIZE*SHARES
#define TWZ_ADDR LINEAR_SIZE*LINEAR_SIZE*SHARES+2*LINEAR_SIZE*SHARES

#ifdef DEBUG
#include <stdio.h>
void zeroWorkZone() {
  int j;
  for(j = 0; j < WSIZE; j++) {
    workZone[j] = 0;
  }
}

void displayWorkZone() {
  int j;
  printf("##################################\n");
  for(j = 1; j < WSIZE+1; j++) {
    printf("%#02.2x ", workZone[j-1]);
    if(j>0 && j % 12 == 0) printf("\n");
  }
  if(j-1 % 12) printf("\n");
  printf("##################################\n");
}
#endif

void loadSecureAES(byte linear[LINEAR_SIZE][LINEAR_SIZE]) {
  int i, j;
#ifdef DEBUG
  zeroWorkZone();
  displayWorkZone();
  printf("Chargement de l'algo...\n");
#endif
  for(i = 0; i < LINEAR_SIZE; i++) {
    for(j = 0; j < LINEAR_SIZE; j++) {
      expand(linear[i][j], &workZone[SHARES*(i*LINEAR_SIZE+j)], SHARES);
    }
  }
#ifdef DEBUG
  displayWorkZone();
#endif
}

void matrix_product(byte * matrix_shares, byte * vector_shares, byte * result_shares) {
  int i, j;
#ifdef DEBUG
  byte m, x, re, r;
  printf("Etat initial :\n");
  displayWorkZone();
#endif
  for(i = 0; i < LINEAR_SIZE; i++) {
    /* Remise à zero de l'accumulateur */
    for(j=0; j<SHARES; j++)
      (workZone+TWZ_ADDR+LINEAR_SIZE)[j] = 0;
#ifdef DEBUG
    printf("Remise à zero de l'accumulateur :\n");
    displayWorkZone();
#endif
    for(j=0; j < LINEAR_SIZE; j++) {
      secMult(matrix_shares+(i*LINEAR_SIZE+j)*SHARES, vector_shares+j*SHARES, workZone+TWZ_ADDR, SHARES);
#ifdef DEBUG
      m = collapse(matrix_shares+(i*LINEAR_SIZE+j)*SHARES, SHARES);
      x = collapse(matrix_shares+(i*LINEAR_SIZE+j)*SHARES, SHARES);
      re = mult_log(m, x);
      r = collapse(workZone+TWZ_ADDR, SHARES);
      printf("Produit %d %d : %#02.2x * %#02.2x = %#02.2x =?= %#02.2x ... %s\n",
	     i, j,
	     m, x, re,
	     r, (r == re ?"OK":"KO")
	     );
      displayWorkZone();
#endif      
      secAdd(workZone+TWZ_ADDR, workZone+TWZ_ADDR+LINEAR_SIZE+1, workZone+TWZ_ADDR+LINEAR_SIZE+1, SHARES);
#ifdef DEBUG
      printf("Ajout dans l'accumulateur :\n");
      displayWorkZone();
#endif        
    }
    for(j=0; j<SHARES; j++)
      (result_shares+i*SHARES)[j] = (workZone+TWZ_ADDR+LINEAR_SIZE)[j];
  }
}

void runSecureAES() {
#ifdef DEBUG
  printf("Produit de matrices...\n");
#endif
  matrix_product(workZone, workZone+CT_ADDR, workZone+ET_ADDR);
#ifdef DEBUG
  displayWorkZone();
#endif
}

void secureAES(byte x[LINEAR_SIZE], byte r[LINEAR_SIZE]) {
  int i;
#ifdef DEBUG
  printf("Chargement des données...\n");
#endif
  for(i = 0; i < LINEAR_SIZE; i++) {
    expand(x[i] , (workZone+CT_ADDR+i*SHARES), SHARES);
  }
#ifdef DEBUG
  displayWorkZone();
#endif
  runSecureAES();
  for(i = 0; i < LINEAR_SIZE; i++) {  
    r[i] = collapse(workZone+ET_ADDR+i*SHARES, SHARES);
  }
}
