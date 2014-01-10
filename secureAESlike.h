#ifndef __SECURE_AES__
#define __SECURE_AES__

#include "shares.h"

/* Paramètres de l'implémentation peuvent être surchargés lors de la compilation */
#ifndef SHARES
#define SHARES 3
#endif
#ifndef LINEAR_SIZE
#define LINEAR_SIZE 16
#endif
#ifndef NB_ROUNDS
#define NB_ROUNDS 10
#endif

#ifndef _BYTE_
#define _BYTE_
typedef unsigned char byte;
#endif

void loadSecureAES(byte linear[LINEAR_SIZE][LINEAR_SIZE], byte sbox[256]);
void setKey(byte key[LINEAR_SIZE*(NB_ROUNDS+1)]);
void secureAES(byte x[LINEAR_SIZE], byte r[LINEAR_SIZE]);

void runSecureAES();
void matrixProduct();
#endif
