/* Opération dans le corps à 256 éléments décrit dans la norme AES */
/* Les opération multiplicatives sont tabulées (tables de log et de carrés + puisance 4) */
#ifndef __FIELD__
#define __FIELD__

#ifndef _BYTE_
#define _BYTE_
typedef unsigned char byte;
#endif

byte add(byte a, byte b);
byte mult_calc(byte a, byte b);
byte mult(byte a, byte b);
byte divide(byte a, byte b);
byte inverse(byte a);
byte square(byte a);
byte pow4(byte a);
byte pow16(byte a);
byte pow254(byte a);

/* Génération de tables */
void genExpLogTables(byte g);
void genMaskTable();
void genSquareTable();
void genPow4Table();
#endif
