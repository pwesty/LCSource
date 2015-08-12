#ifndef SE_INCL_PROTECTION_H
#define SE_INCL_PROTECTION_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif
#define COPY_PROTECTION 0

typedef struct {
  unsigned long P[16 + 2];
  unsigned long S[4][256];
} BLOWFISH_CTX;

extern void Blowfish_Init(BLOWFISH_CTX *ctx, unsigned char *key, int keyLen);
extern void Blowfish_Decrypt(BLOWFISH_CTX *ctx, unsigned long *xl, unsigned long *xr);


#endif  /* include-once check. */

