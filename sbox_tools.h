#ifndef __SBOX_TOOLS__
#define __SBOX_TOOLS__

#include "gf256.h"

void printSbox(byte polynomial[256]);
/**
 * poly1  : polynome de degré d-1
 * poly2  : polynome de degré 1
 * result : polynome de degré d, produit de poly1 et poly2
 * Attention à l'odre : ici les petits degrés on les petits indices ; je considère l'inverse dans mes sbox
 */
void polyProduct(byte poly1[], byte poly2[], byte result[], int d);
void polyScalMult(byte poly[], byte a, int d);
#endif

