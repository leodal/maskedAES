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

#ifndef _BYTE_
#define _BYTE_
typedef unsigned char byte;
#endif

void loadSecureAES(byte linear[LINEAR_SIZE][LINEAR_SIZE]);
void secureAES(byte x[LINEAR_SIZE], byte r[LINEAR_SIZE]);
#endif
