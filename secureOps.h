#ifndef __SEC_OPS__
#define __SEC_OPS__

#include "gf256.h"

void expand(byte a, byte r[], int order);

void secAdd(byte a_shares[], byte b_shares[], byte r_shares[], int order);
void secSquare(byte a_shares[], byte r_shares[], int order);
void secMult(byte a_shares[], byte b_shares[], byte r_shares[], int order);
void secExp254(byte a_share[], byte r_share[], int order);
#endif
