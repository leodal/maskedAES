#ifndef __AES_LIKE__
#define __AES_LIKE__

/* Paramètres de l'implémentation peuvent être surchargés lors de la compilation */
#ifndef LINEAR_SIZE
#define LINEAR_SIZE 16
#endif
#ifndef NB_ROUNDS
#define NB_ROUNDS 10
#endif

#include "gf256.h"

/* Charge les données formant le programme */
void loadLinearPart(byte linear[LINEAR_SIZE][LINEAR_SIZE]);
void loadSbox(byte Sbox[256]);
void loadAESlike(byte linear[LINEAR_SIZE][LINEAR_SIZE], byte Sbox[256]);

void matrixProduct(byte vector[LINEAR_SIZE], byte result[LINEAR_SIZE]);
byte evalSbox(byte value);
void applySbox(byte vector[LINEAR_SIZE]);
/* Execution de l'algo sur le clair "clear", résultat dans "cypher" */ 
void aesLike(byte clear[LINEAR_SIZE], byte cypher[LINEAR_SIZE]);
#endif
