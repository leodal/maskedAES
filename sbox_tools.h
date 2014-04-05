/* Transformations de Sbox de 256 octets (type byte défini dans gf256) */

/* Permet de passer de leur forme standard (tables de lookup) à leur
   forme calculatoire, par l'interpollation de Lagrange tel que décrit
   dans la contre-mesure de Carlet et al. */
#ifndef __SBOX_TOOLS__
#define __SBOX_TOOLS__

#include "gf256.h"

/* Transformation d'une sbox de sa forme polynomiale vers sa forme
   standard (lookup), résultat dans le paramètre "sbox" */
void buildSbox(byte polynomial[256], byte sbox[256]);

/* Transformation d'une sbox de sa forme standard (lookup) vers sa
   forme polynomiale, résultat dans le paramètre "polySbox" */
void buildPolySbox(byte sbox[256], byte polySbox[256]);

/*
  Produit de polymômes
 
  poly1  : polynome de degré d-1
  poly2  : polynome de degré 1
  result : polynome de degré d, produit de poly1 et poly2
  Attention à l'odre : ici les petits degrés on les petits indices ;
  je considère l'inverse dans mes sbox
 */
void polyProduct(byte poly1[], byte poly2[], byte result[], int d);
/* Multiplication d'un polynôme par un scalaire */
void polyScalMult(byte poly[], byte a, int d);
/* Addition de deux polynômes de même degré d */
void polyAdd(byte poly1[], byte poly2[], byte result[], int d);
/* Evaluation d'une sbox sous forme polynomiale */
byte evalPolySbox(byte polynomial[256], byte x);
/* Calcule le polynome d'interpollation de Lagrange d'une table */
void lagrange(byte table[], byte result[], int d);
/* Inverse un table de length élements */
void revertTab(byte tab[], int length);

/* Transformation des sbox d'une forme à l'autre, puis affichage (sous
   une forme acceptable par mes programmes */
void printSbox(byte polynomial[256]);
void printPolySbox(byte sbox[256]);
#endif

