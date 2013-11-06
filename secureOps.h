#ifndef __SEC_OPS__
#define __SEC_OPS__

#include "gf256.h"

// Operations on shares of length n, results in r
void secAdd(byte a[], byte b[], byte r[], int n);
void secSquare(byte a[], byte r[], int n);
void secMult(byte a[], byte b[], byte r[], int n);
void secExp254(byte a[], byte r[], int n);
#endif
