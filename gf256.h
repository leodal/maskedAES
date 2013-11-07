#ifndef __FIELD__
#define __FIELD__

#ifndef _BYTE_
#define _BYTE_
typedef unsigned char byte;
#endif

byte add(byte a, byte b);
byte mult(byte a, byte b);
byte mult_log(byte a, byte b);
byte divide(byte a, byte b);
byte inverse(byte a);
byte square(byte a);
byte pow4(byte a);
byte pow16(byte a);
byte pow254(byte a);

void genMaskTable();
void genSquareTable();
#endif
