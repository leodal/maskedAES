#ifndef __SHARES__
#define __SHARES__

#include "gf256.h"

/** Méthode de masquage de Rivain-Proof **/

/* Expand a byte a into n "shares" in r */
void expand(byte a, byte r[], int n);
/* Refresh the n shares by adding random values in all cells
   but the first one which sum all randoms */
/** Attaque de ce refresh par Matthieu et al. **/
/** TODO chercher reference **/
void refresh(byte shares[], int n);
/* Collapse n shares into a sigle byte 
   i.e. add all shares in gf256 
   i.e. xor all bytes of a */
byte collapse(byte a[], int n);

/* Operations on shares of length n, results in r */
void secAdd(byte a[], byte b[], byte r[], int n);
void secSquare(byte a[], byte r[], int n);
void secMult(byte a[], byte b[], byte r[], int n);
void secExp254(byte a[], byte r[], int n);

#endif
