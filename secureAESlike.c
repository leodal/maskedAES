#include "secureAESlike.h"

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
   Temporary datas = SHARES
 */
#define WSIZE LINEAR_SIZE*LINEAR_SIZE*SHARES + 256 + 2*LINEAR_SIZE*SHARES+3*SHARES + LINEAR_SIZE*NB_ROUNDS*SHARES
byte workZone[WSIZE];

#define MATRIX_ADDR workZone
#define SBOX_ADDR   (workZone + LINEAR_SIZE*LINEAR_SIZE*SHARES)
#define KEY_ADDR    (workZone + LINEAR_SIZE*LINEAR_SIZE*SHARES + 256*SHARES)
#define CT_ADDR     (workZone + LINEAR_SIZE*LINEAR_SIZE*SHARES + 256*SHARES + LINEAR_SIZE*NB_ROUNDS*SHARES)
#define ET_ADDR     (workZone + LINEAR_SIZE*LINEAR_SIZE*SHARES + 256*SHARES + LINEAR_SIZE*NB_ROUNDS*SHARES + LINEAR_SIZE*SHARES)
#define TWZ_ADDR    (workZone + LINEAR_SIZE*LINEAR_SIZE*SHARES + 256*SHARES + LINEAR_SIZE*NB_ROUNDS*SHARES + 2*LINEAR_SIZE*SHARES)

int i, j, k;

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

/* void secSbox(byte x[SHARES]) { */
/*   /\* xj initialized to 1 *\/ */
/*   expand(1, TWZ_ADDR, SHARES); */
/*   /\* sum is initialized to 0 *\/ */
/*   expand(0, TWZ_ADDR+SHARES, SHARES); */
/*   for(j = 0; j < 256; j++) { */
/*     /\* tmp = s * xj *\/ */
/*     secMult(TWZ_ADDR, SBOX_ADDR+j*SHARES, TWZ_ADDR+2*SHARES, SHARES); */
/*     /\* accu += tmp *\/ */
/*     secAdd(TWZ_ADDR + 2*SHARES, TWZ_ADDR + SHARES, TWZ_ADDR + SHARES, SHARES); */
/*     /\* xj = xj * x ( = x ^j) *\/ */
/*     secMult(TWZ_ADDR, x, TWZ_ADDR, SHARES); */
/*   } */
/*   for(j = 0; j < SHARES; j++) */
/*     x[j] = (TWZ_ADDR + SHARES)[j]; */
/* } */

void secSbox(byte x[SHARES]) {
  int i;
  /* accu = *TWZ = sbox[0] = s_255 */
  for(i = 0; i < SHARES; i++) {
    TWZ_ADDR[i] = SBOX_ADDR[i];
  }
  for(i = 1; i < 256; i++) {
    secMult(TWZ_ADDR, x, TWZ_ADDR, SHARES);
    secAdd(TWZ_ADDR, SBOX_ADDR+i*SHARES, TWZ_ADDR, SHARES);
  }
  /* Le résultat est dans TWZ */
  for(i = 0; i < SHARES; i++) {
    x[i] = TWZ_ADDR[i];
  }
}

void apply_sbox() {
#ifdef DEBUG
  printf("Application de la Sbox...\n");
#endif
  for(i = 0; i < LINEAR_SIZE; i++)
    secSbox(CT_ADDR+i*SHARES);
#ifdef DEBUG
  displayWorkZone();
#endif
}

void loadSecureAES(byte linear[LINEAR_SIZE][LINEAR_SIZE], byte sbox[256]) {
  int i, j;
#ifdef DEBUG
  zeroWorkZone();
  displayWorkZone();
  printf("Chargement de l'algo...\n");
  printf("NB SHARES = %d; NB_ROUNDS = %d\n", SHARES, NB_ROUNDS);
  printf("Taille mémoire = %d\n", WSIZE);
#endif
  for(i = 0; i < LINEAR_SIZE; i++) {
    for(j = 0; j < LINEAR_SIZE; j++) {
      expand(linear[i][j], MATRIX_ADDR + SHARES*(i*LINEAR_SIZE+j), SHARES);
    }
  }
  for(i = 0; i < 256; i++)
    expand(sbox[i], SBOX_ADDR + i*SHARES, SHARES);
#ifdef DEBUG
  displayWorkZone();
#endif
}

void setKey(byte key[LINEAR_SIZE*NB_ROUNDS]) {
  for(i = 0; i < LINEAR_SIZE*NB_ROUNDS; i++) {
    expand(key[i], KEY_ADDR + i*SHARES, SHARES);
  }
}

void matrix_product() {
#ifdef DEBUG
  printf("Application linéaire...\n");
#endif
  /*#ifdef DEBUG
  byte m, x, re, r;
  #endif*/
  for(i = 0; i < LINEAR_SIZE; i++) {
    /* On remet à zero la zone de stockage du résultat */
    for(j = 0; j < SHARES; j++)
      (ET_ADDR + i*SHARES)[j] = 0;
    for(j = 0; j < LINEAR_SIZE; j++) {
      /* On effecture les produits du jème elt de la ligne i de la matrice
	 avec le jème element du vecteur */
      secMult(MATRIX_ADDR+(i*LINEAR_SIZE+j)*SHARES,
	      CT_ADDR+j*SHARES,
	      TWZ_ADDR, SHARES);
      /*#ifdef DEBUG
      m = collapse(MATRIX_ADDR+(i*LINEAR_SIZE+j)*SHARES, SHARES);
      x = collapse(CT_ADDR+j*SHARES, SHARES);
      re = mult_log(m, x);
      r = collapse(TWZ_ADDR, SHARES);
      printf("Produit %d %d : %#02.2x * %#02.2x = %#02.2x =?= %#02.2x ... %s\n",
	     i, j,
	     m, x, re,
	     r, (r == re ?"OK":"KO")
	     );
      displayWorkZone();
      x = collapse(ET_ADDR+j*SHARES, SHARES);
      #endif*/
      /* On ajoute ce produit au total dans ET_ADDR[i] */
       secAdd(TWZ_ADDR, ET_ADDR+i*SHARES, ET_ADDR+i*SHARES, SHARES);
       /*#ifdef DEBUG
       m = collapse(TWZ_ADDR, SHARES);
       re = add(r, x);
       r = collapse(ET_ADDR+j*SHARES, SHARES);
       printf("Ajout du résultat du produit %d dans ET[i] = %d : %d =?= %d... %s\n",
	      m, x, r, re,
	      (r == re ?"OK":"KO"));
      displayWorkZone();
      #endif*/
    }
  }
#ifdef DEBUG
  displayWorkZone();
#endif
}

void add_round_key() {
  for(i = 0; i < LINEAR_SIZE*SHARES; i++)
    CT_ADDR[i] ^= (KEY_ADDR + k*LINEAR_SIZE*SHARES)[i];
}

void runSecureAES() {
  for(k = 0; k < NB_ROUNDS; k++) {
    add_round_key();
    apply_sbox();
    matrix_product();
    for(i = 0; i < SHARES*LINEAR_SIZE; i++)
      CT_ADDR[i] = ET_ADDR[i];
  }
}

void secureAES(byte x[LINEAR_SIZE], byte r[LINEAR_SIZE]) {
#ifdef DEBUG
  printf("Chargement des données...\n");
#endif
  for(i = 0; i < LINEAR_SIZE; i++) {
    expand(x[i] , (CT_ADDR+i*SHARES), SHARES);
  }
#ifdef DEBUG
  displayWorkZone();
#endif
  runSecureAES();
#ifdef DEBUG
  printf("Récupération du résultat :\n");
  displayWorkZone();
#endif
  for(i = 0; i < LINEAR_SIZE; i++) {  
    r[i] = collapse(ET_ADDR+i*SHARES, SHARES);
  }
#ifdef DEBUG
  displayWorkZone();
#endif
}
