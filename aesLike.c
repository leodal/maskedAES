#include "aesLike.h"
#include "gf256.h"

#define TMP_SIZE 1
#define EXTENDED_KEY_SIZE LINEAR_SIZE*(NB_ROUNDS+1)
#define DATA_SIZE LINEAR_SIZE*LINEAR_SIZE+256+EXTENDED_KEY_SIZE+2*LINEAR_SIZE+TMP_SIZE

byte workZone[DATA_SIZE];

#define LINEAR workZone /* LINEAR_SIZE^2 bytes */
#define SBOX (LINEAR + LINEAR_SIZE*LINEAR_SIZE) /* 256 bytes */
#define KEY (SBOX + 256) /* EXTENDED_KEY_SIZE bytes */
#define CT_ADDR  (KEY + EXTENDED_KEY_SIZE) /* LINEAR_SIZE bytes */
#define ET_ADDR (CT_ADDR + LINEAR_SIZE) /* LINEAR_SIZE bytes */
#define TMP_ZONE (ET_ADDR + LINEAR_SIZE) /* TMP_SIZE bytes */

#ifdef DEBUG
#include <stdio.h>

void zeroWorkZone() {
  int j;
  for(j = 0; j < DATA_SIZE; j++) {
    workZone[j] = 0;
  }
}

void displayWorkZone() {
  int j;
  printf("##################################\n");
  printf("DATA_SIZE %d\n", DATA_SIZE);
  for(j = 1; j < DATA_SIZE+1; j++) {
    printf("%#02.2x ", workZone[j-1]);
    if(j>0 && j % 12 == 0) printf("\n");
  }
  if(j-1 % 12) printf("\n");
  printf("##################################\n");
}
#endif

void loadLinearPart(byte linear[LINEAR_SIZE][LINEAR_SIZE]) {
  int i, j;
#ifdef DEBUG
  printf("Chargement de l'application linéaire :\n");
#endif
  for(i = 0; i < LINEAR_SIZE; i++) {
    for(j = 0; j < LINEAR_SIZE; j++) {
      workZone[i*LINEAR_SIZE+j] = linear[i][j];
    }
  }
#ifdef DEBUG
  displayWorkZone();
#endif
}

void loadSbox(byte Sbox[256]) {
  int i;
#ifdef DEBUG
  printf("Chargement de la Sbox :\n");
#endif
  for(i = 0; i < 256; i++) {
    SBOX[i] = Sbox[i];
  }
#ifdef DEBUG
  displayWorkZone();
#endif
}

void loadAESlike(byte linear[LINEAR_SIZE][LINEAR_SIZE], byte Sbox[256]) {
#ifdef DEBUG
  zeroWorkZone();
  printf("Etat initial : \n");
  displayWorkZone();
#endif
  loadLinearPart(linear);
  loadSbox(Sbox);
}

void loadKey(byte key[LINEAR_SIZE*(NB_ROUNDS+1)]) {
  int i;
#ifdef DEBUG
  printf("Chargement de la clef : \n");
#endif
  for(i = 0; i < LINEAR_SIZE*(NB_ROUNDS+1); i++)
    KEY[i] = key[i];
#ifdef DEBUG
  displayWorkZone();
#endif  
}

void loadClearText(byte clear[LINEAR_SIZE]) {
  int i;
  for(i = 0; i < LINEAR_SIZE; i++)
    CT_ADDR[i] = clear[i];
#ifdef DEBUG
  displayWorkZone();
#endif
}

void matrixProduct(byte vector[LINEAR_SIZE], byte result[LINEAR_SIZE]) {
  int i, j;
  for(i = 0; i < LINEAR_SIZE; i++) {
    TMP_ZONE[0] = 0;
    for(j = 0; j < LINEAR_SIZE; j++) {
      TMP_ZONE[0] ^= mult(LINEAR[i*LINEAR_SIZE+j], vector[j]);
    }
    result[i] = TMP_ZONE[0];
  }
}

byte evalSbox(byte value) {
  return SBOX[value];
}

void applySbox(byte vector[LINEAR_SIZE]) {
  int i;
  for(i = 0; i < LINEAR_SIZE; i++)
    vector[i] = SBOX[vector[i]];
}

void addRoundKey(byte vector[LINEAR_SIZE], int round) {
  int i;
  for(i = 0; i < LINEAR_SIZE; i++)
    vector[i] ^= (KEY + round*LINEAR_SIZE)[i];
}

void aesLike(byte clear[LINEAR_SIZE], byte cypher[LINEAR_SIZE]) {
  int i;
#ifdef DEBUG
  printf("Chargement du clair en mémoire :\n");
#endif
  loadClearText(clear);
  addRoundKey(CT_ADDR, 0);
  for(i = 0; i < NB_ROUNDS; i++) {
    if(i%2 == 0) {
      applySbox(CT_ADDR);
      matrixProduct(CT_ADDR, ET_ADDR);
      addRoundKey(ET_ADDR, i);
    } else {
      applySbox(ET_ADDR);
      matrixProduct(ET_ADDR, CT_ADDR);
      addRoundKey(CT_ADDR, i);
    }
  }
#ifdef DEBUG
  printf("i = %d (NB_ROUNDS = %d)\n", i, NB_ROUNDS);
  displayWorkZone();
#endif
  if(i%2 == 0) {
    for(i = 0; i < LINEAR_SIZE; i++)
      cypher[i] = CT_ADDR[i];
  } else {
    for(i = 0; i < LINEAR_SIZE; i++)
      cypher[i] = ET_ADDR[i];
  }
}
