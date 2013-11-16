#ifndef __AES__
#define __AES__

#ifndef _BYTE_
#define _BYTE_
typedef unsigned char byte;
#endif

#ifndef KEY_SIZE
#define KEY_SIZE 128
#endif

#if KEY_SIZE == 128
#define NB_ROUNDS 10
#elif KEY_SIZE == 192
#define NB_ROUNDS 12
#elif KEY_SIZE == 256
#define NB_ROUNDS 14
#endif

void aes(byte in[16], byte out[16], byte key[16]);

/* void aes_keyExpansion(key, rk); */

/* void aes_addRoundKey(byte state, byte rk, int round); */
byte aes_affine(byte x);
byte coron_affine(byte x);

void format_affine();
#endif
