#ifndef __AES_LIKE__
#define __AES_LIKE__

/* Paramètres de l'implémentation, peuvent être surchargés lors de la compilation 
   LINEAR_SIZE est la taille de la matrice carrée décrivant l'application linéaire
   NB_ROUNDS est le nombre de tours de chiffrements
*/
#ifndef LINEAR_SIZE
#define LINEAR_SIZE 16
#endif
#ifndef NB_ROUNDS
#define NB_ROUNDS 10
#endif

#include "gf256.h"

/* Chargement des données formant le programme */
/* Partie linéaire : une matrice carrée d'octets (type byte), défini dans gf256.h */
void loadLinearPart(byte linear[LINEAR_SIZE][LINEAR_SIZE]);
/* Sbox : une lookup-table bijective de 256 octets (type byte) */
void loadSbox(byte Sbox[256]);
/* Chargement des deux d'un coup */
void loadAESlike(byte linear[LINEAR_SIZE][LINEAR_SIZE], byte Sbox[256]);
/* Chargement de la clef (déjà diversifiée) */
void loadKey(byte key[LINEAR_SIZE*(NB_ROUNDS+1)]);

/* Le produit de matrice, exposé notamment pour les tests */
void matrixProduct(byte vector[LINEAR_SIZE], byte result[LINEAR_SIZE]);
/* Evaluation de la SBox, même motif */
byte evalSbox(byte value);
/* Application de la Sbox sur tous les éléments d'un vecteur */
void applySbox(byte vector[LINEAR_SIZE]);
/* Ajout de la clef de tour sur un vecteur */
void addRoundKey(byte vector[LINEAR_SIZE], int round);
/* Execution de l'algo sur le clair "clear", résultat dans "cypher" */ 
void aesLike(byte clear[LINEAR_SIZE], byte cypher[LINEAR_SIZE]);
#endif
