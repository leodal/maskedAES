#ifndef __SBOX_TOOLS__
#define __SBOX_TOOLS__

#include "gf256.h"

void printSbox(byte polynomial[256]);
void printPolySbox(byte sbox[256]);

void buildSbox(byte polynomial[256], byte sbox[256]);
void buildPolySbox(byte sbox[256], byte polySbox[256]);
/**
 * poly1  : polynome de degré d-1
 * poly2  : polynome de degré 1
 * result : polynome de degré d, produit de poly1 et poly2
 * Attention à l'odre : ici les petits degrés on les petits indices ; je considère l'inverse dans mes sbox
 */
void polyProduct(byte poly1[], byte poly2[], byte result[], int d);
void polyScalMult(byte poly[], byte a, int d);
void polyAdd(byte poly1[], byte poly2[], byte result[], int d);
byte evalPolySbox(byte polynomial[256], byte x);
void lagrange(byte table[], byte result[], int d);
void revertTab(byte tab[], int length);
#endif

