#ifndef __SHARES__
#define __SHARES__

#ifndef _BYTE_
#define _BYTE_
typedef unsigned char byte;
#endif

// Expend byte a into n shares in r
void expand(byte a, byte r[], int n);
// Refresh the n shares
void refresh(byte shares[], int n);

#endif
