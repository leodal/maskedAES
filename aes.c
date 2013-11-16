#include "aes.h"

#define STATE_SIZE 16

/* Each round requires a 128 bits = 16 bytes key */
#define EXTENDED_KEY_SIZE  (NB_ROUNDS+1)*16
#define WSIZE (STATE_SIZE + EXTENDED_KEY_SIZE)

int i;
byte workZone[WSIZE];

#ifdef DEBUG
#include <stdio.h>
void zeroWorkZone() {
  int j;
  for(j = 0; j < WSIZE; j++) {
    workZone[j] = 0;
  }
}

void displayWorkZone() {
  int j;
  printf("##################################\n");
  printf("WSIZE = %d\n", WSIZE);
  for(j = 1; j < WSIZE+1; j++) {
    printf("%#02.2x ", workZone[j-1]);
    if(j>0 && j % 12 == 0) printf("\n");
  }
  if(j-1 % 12) printf("\n");
  printf("##################################\n");
}
#endif

void aes_setState(byte in[STATE_SIZE], byte state[STATE_SIZE]) {
  for(i=0; i < STATE_SIZE; i++)
    state[i] = in[i];
}

/** TODO Implémenter la vraie fonction**/ 
void aes_keyExpansion(byte key[], byte * rk) {
  for(i = 0; i < EXTENDED_KEY_SIZE; i++) 
    rk[i] = 0xFF^(i/STATE_SIZE);
}

void aes_addRoundKey(byte* state, byte* rk, int round) {
  for(i = 0; i < STATE_SIZE; i++)
    state[i] ^= (rk+round*STATE_SIZE)[i];
}

void aes_shiftRow(byte* state) {
  for(i = 1; i < 4; i++) {
    
  }
}

void aes(byte in[16], byte out[16], byte key[16]) {
  byte* state = workZone;
  byte* rk = workZone+16;
  int round;

  aes_setState(in, state);
  aes_keyExpansion(key, rk);
#ifdef DEBUG
  displayWorkZone();
#endif
  aes_addRoundKey(state, rk, 0);
#ifdef DEBUG
  printf("APRES LE PREMIER ADD_ROUND_KEY\n");
  displayWorkZone();
#endif
  for(round = 1; round <= NB_ROUNDS; round++) {
    printf("Ronde %d : \n", round);
/*     subBytes(state); */
/*     shiftRows(state); */
/*     mixColumn(state); */
    aes_addRoundKey(state, rk, round);
#ifdef DEBUG
    if(round < 3) {
      printf("APRES LE %deme ADD_ROUND_KEY\n", round+1);
      displayWorkZone();
    }
#endif    
  } 
}

const byte aes_affineTable[256] = {
  99, 124, 93, 66, 31, 0, 33, 62, 155, 132,
  165, 186, 231, 248, 217, 198, 146, 141, 172, 179,
  238, 241, 208, 207, 106, 117, 84, 75, 22, 9,
  40, 55, 128, 159, 190, 161, 252, 227, 194, 221,
  120, 103, 70, 89, 4, 27, 58, 37, 113, 110,
  79, 80, 13, 18, 51, 44, 137, 150, 183, 168,
  245, 234, 203, 212, 164, 187, 154, 133, 216, 199,
  230, 249, 92, 67, 98, 125, 32, 63, 30, 1,
  85, 74, 107, 116, 41, 54, 23, 8, 173, 178,
  147, 140, 209, 206, 239, 240, 71, 88, 121, 102,
  59, 36, 5, 26, 191, 160, 129, 158, 195, 220,
  253, 226, 182, 169, 136, 151, 202, 213, 244, 235,
  78, 81, 112, 111, 50, 45, 12, 19, 236, 243,
  210, 205, 144, 143, 174, 177, 20, 11, 42, 53,
  104, 119, 86, 73, 29, 2, 35, 60, 97, 126,
  95, 64, 229, 250, 219, 196, 153, 134, 167, 184,
  15, 16, 49, 46, 115, 108, 77, 82, 247, 232,
  201, 214, 139, 148, 181, 170, 254, 225, 192, 223,
  130, 157, 188, 163, 6, 25, 56, 39, 122, 101,
  68, 91, 43, 52, 21, 10, 87, 72, 105, 118,
  211, 204, 237, 242, 175, 176, 145, 142, 218, 197,
  228, 251, 166, 185, 152, 135, 34, 61, 28, 3,
  94, 65, 96, 127, 200, 215, 246, 233, 180, 171,
  138, 149, 48, 47, 14, 17, 76, 83, 114, 109,
  57, 38, 7, 24, 69, 90, 123, 100, 193, 222,
  255, 224, 189, 162, 131, 156
};

const byte aes_InvAffineTable[256] = {
  5, 79, 145, 219, 44, 102, 184, 242, 87, 29,
  195, 137, 126, 52, 234, 160, 161, 235, 53, 127,
  136, 194, 28, 86, 243, 185, 103, 45, 218, 144,
  78, 4, 76, 6, 216, 146, 101, 47, 241, 187,
  30, 84, 138, 192, 55, 125, 163, 233, 232, 162,
  124, 54, 193, 139, 85, 31, 186, 240, 46, 100,
  147, 217, 7, 77, 151, 221, 3, 73, 190, 244,
  42, 96, 197, 143, 81, 27, 236, 166, 120, 50,
  51, 121, 167, 237, 26, 80, 142, 196, 97, 43,
  245, 191, 72, 2, 220, 150, 222, 148, 74, 0,
  247, 189, 99, 41, 140, 198, 24, 82, 165, 239,
  49, 123, 122, 48, 238, 164, 83, 25, 199, 141,
  40, 98, 188, 246, 1, 75, 149, 223, 32, 106,
  180, 254, 9, 67, 157, 215, 114, 56, 230, 172,
  91, 17, 207, 133, 132, 206, 16, 90, 173, 231,
  57, 115, 214, 156, 66, 8, 255, 181, 107, 33,
  105, 35, 253, 183, 64, 10, 212, 158, 59, 113,
  175, 229, 18, 88, 134, 204, 205, 135, 89, 19,
  228, 174, 112, 58, 159, 213, 11, 65, 182, 252,
  34, 104, 178, 248, 38, 108, 155, 209, 15, 69,
  224, 170, 116, 62, 201, 131, 93, 23, 22, 92,
  130, 200, 63, 117, 171, 225, 68, 14, 208, 154,
  109, 39, 249, 179, 251, 177, 111, 37, 210, 152,
  70, 12, 169, 227, 61, 119, 128, 202, 20, 94,
  95, 21, 203, 129, 118, 60, 226, 168, 13, 71,
  153, 211, 36, 110, 176, 250
};

void format_affine() {
  printf("const byte aes_affineTable[256] = {\n  ");
  for(i = 1; i<257; i++) {
    printf("%d, ", aes_affineTable[i-1]);
    if(i>0 && i%10 == 0)
      printf("\b\n  ");
  }
  printf("\b\b  \n};\n\n");

  printf("const byte aes_InvAffineTable[256] = {\n  ");
  for(i = 1; i<257; i++) {
    printf("%d, ", aes_InvAffineTable[i-1]);
    if(i>0 && i%10 == 0)
      printf("\b\n  ");
  }
  printf("\b\b  \n};\n\n");
}

byte aes_affine(byte x) {
  return aes_affineTable[x];
}

byte bit(byte x,int i)
{
  return (x >> i) & 1;
}

byte coron_affine(byte x)
{
  byte y=0;
  int i;
  byte z;
  for(i=7;i>=0;i--)
  {
    z=bit(x,i) ^ bit(x,(i+4) %8) ^ bit(x,(i+5) %8) ^ bit(x,(i+6) % 8) ^ bit(x,(i+7) % 8);
    y=(y << 1) ^ z;
  }
  y=y ^ 99;
  return y;
}
