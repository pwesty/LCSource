#ifndef __DES3_H__
#define __DES3_H__

// LCDES3에서는 오로지 single-length DES3만 지원하도록 함

inline void LCDES3_InitKey(LCDES3CONTEXT* pLCDES3Context, unsigned int nKeyHigh, unsigned int nKeyLow)
{
	pLCDES3Context->nKeyHigh = nKeyHigh;
	pLCDES3Context->nKeyLow = nKeyLow;
}

#endif // __DES3_H__
//
