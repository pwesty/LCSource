#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "LCCrypt.h"

#include "des3.h"
#include "des3_static.h"
#include "des3_randomkey.h"

// bit-shuffle table : srand(1)
static const unsigned char tableShuffle[16][28] =
{
	{ 5,  9, 12, 25, 21,  2, 18, 15, 24, 22,  6, 19,  3, 27, 23, 11, 17,  7,  8, 16,  4, 13, 20,  1,  0, 14, 10, 26},
	{24,  7, 10,  5, 27, 16,  2, 23,  3, 25,  4, 17, 13,  9, 20, 12,  0, 22, 18, 14,  8, 26, 15,  6, 19, 21,  1, 11},
	{ 4, 23,  5, 14, 19, 25, 11,  1,  8, 24, 10, 12, 18,  0, 13, 26,  2, 15, 27, 17,  7, 16,  6, 21,  9,  3, 22, 20},
	{17,  2,  5, 23, 15, 16, 22,  3, 13,  0, 12, 11,  1,  6, 26, 21, 19, 24, 27, 10, 25,  8, 20,  9,  7, 14, 18,  4},
	{12, 10,  3,  9, 17, 16, 24, 22,  0, 14,  6,  7, 23, 11,  4, 18, 15, 20,  2, 13,  8, 25, 26, 27, 21,  5,  1, 19},
	{17,  5,  6, 14, 10,  2,  3, 23, 15, 19, 12, 16,  7,  1,  8, 22, 13,  4, 20, 24, 25, 18, 21,  0, 26,  9, 27, 11},
	{17, 14,  0, 23, 27, 25, 15, 10, 12, 20,  7, 16,  4,  3,  8, 11,  5, 24, 21, 19,  6,  2, 26, 13, 18,  9, 22,  1},
	{15, 22, 11, 19, 23,  0,  7, 10, 21, 20, 25, 14,  9, 13, 18, 16, 12, 27,  1,  4,  5,  2, 26,  3, 17, 24,  6,  8},
	{23, 17,  8, 26,  6, 14, 22, 12,  4, 20,  7, 13, 25, 11,  9, 27,  2, 16, 18,  1,  0,  3, 10, 21, 24, 19,  5, 15},
	{11, 19, 10,  9,  3, 26,  4, 15, 24, 20, 25,  7, 14,  5, 23, 12,  6,  2,  1, 27,  0, 18,  8, 21, 16, 13, 22, 17},
	{11, 27,  4,  3,  6, 12, 10, 25, 19,  2, 20, 26,  9,  7, 21, 23, 24,  8, 22, 18, 16,  5, 15, 13, 14, 17,  0,  1},
	{ 6, 20, 18, 11, 22, 12, 21, 13,  1, 15,  3,  5,  0, 10, 14, 17, 26,  8, 27, 23, 25, 16, 19,  9,  4,  2,  7, 24},
	{27,  5, 26,  3, 22, 20,  1, 12, 23, 16,  6, 24,  2, 15, 14, 25, 17,  4, 21, 18,  9,  8, 11,  7, 10, 13,  0, 19},
	{ 0,  9,  7, 23, 15,  5, 14, 27, 21,  1, 25,  6, 19,  2, 17, 18, 16,  3, 26, 22, 20, 12,  4, 13, 10, 11, 24,  8},
	{ 1, 15, 18, 20,  3,  0, 27, 26,  6,  7, 19, 17, 10, 23, 16, 22,  5, 12,  4, 24, 21, 13,  2,  8, 11, 25,  9, 14},
	{ 5, 26, 22, 25, 10, 12, 16, 23,  8,  3, 11,  6, 13, 21, 15,  9,  1, 20, 17,  4,  7, 27, 18, 24, 14,  0,  2, 19},
};

inline static void LCDES3_GenKey(LCDES3CONTEXT* pLCDES3Context)
{
	unsigned int nHigh = LCDES3_GetRandom32Bit(pLCDES3Context->nKeyLow);
	unsigned int nLow  = LCDES3_GetRandom32Bit(nHigh);
	LCDES3_InitKey(pLCDES3Context, nHigh, nLow);
}

void CNM_InitKeyValue(CNM_KEY_TYPE* pKey)
{
	LCDES3_InitKey(pKey, LC_DES3_DEFAULT_KEY_HIGH, LC_DES3_DEFAULT_KEY_LOW);
}

void CNM_NextKey(CNM_KEY_TYPE* pKey)
{
	LCDES3_GenKey(pKey);
}

void CNM_CopyKey(CNM_KEY_TYPE* pDestKey, const CNM_KEY_TYPE* pSrcKey)
{
	memcpy(pDestKey, pSrcKey, sizeof(CNM_KEY_TYPE));
}

int CNM_Crypt(const unsigned char* pSrc, int nLenSrc, CNM_KEY_TYPE* pKey, unsigned char* pDest, unsigned char* pTmpBuf)
{
	unsigned char* pOut = pTmpBuf;

	*pOut++ = (unsigned char)1;

	// 임시 버퍼에 원본 길이를 저장
	*pOut++ = (unsigned char)((nLenSrc >> 24) & 0xff);
	*pOut++ = (unsigned char)((nLenSrc >> 16) & 0xff);
	*pOut++ = (unsigned char)((nLenSrc >>  8) & 0xff);
	*pOut++ = (unsigned char)((nLenSrc      ) & 0xff);

	// 임시 버퍼에 원본 데이터 복사
	if (nLenSrc > 0)
	{
		memcpy(pOut, pSrc, nLenSrc);
		pOut += nLenSrc;
	}

	// 8 byte로 align
	int nLenBuf = pOut - pTmpBuf - 1;
	int nPad = 0;
	if (nLenBuf % 8 != 0)
		nPad = (8 - (nLenBuf % 8));
	while (nPad > 0)
	{
		*pOut++ = (unsigned char)rand();
		nPad--;
	}
	nLenBuf = pOut - pTmpBuf;

	// 암호화
	if (!LCDES3_Crypt(pKey, pTmpBuf + 1, pDest + 1, nLenBuf - 1, true))
		return -1;
	*pDest = *pTmpBuf;

	return nLenBuf;
}

int CNM_Decrypt(const unsigned char* pSrc, int nLenSrc, CNM_KEY_TYPE* pKey, unsigned char* pDest, unsigned char* pTmpBuf)
{
	unsigned char* pOut = pTmpBuf;

	// 복호화 : 더미 제외
	if (!LCDES3_Crypt(pKey, pSrc + 1, pTmpBuf, nLenSrc - 1, false))
		return -1;

	// 더미
	if (*pSrc != 1)
		return -1;

	// 길이를 가져옴
	int nLenBuf = ((((int)(pOut[0])) << 24) & 0xff000000)
				  | ((((int)(pOut[1])) << 16) & 0x00ff0000)
				  | ((((int)(pOut[2])) <<  8) & 0x0000ff00)
				  | ((((int)(pOut[3]))      ) & 0x000000ff);
	pOut += 4;

	// 평문 데이터 복사
	if (nLenBuf < 0 || nLenBuf >= nLenSrc)
		return -1;
	if (nLenBuf > 0)
	{
		memcpy(pDest, pOut, nLenBuf);
	}
	else
	{
		*pDest = 0;
	}

	return nLenBuf;
}

int CNM_OldCrypt(const unsigned char* pSrc, int nLenSrc, CNM_KEY_TYPE* pKey, unsigned char* pDest, unsigned char* pTmpBuf)
{
	unsigned char* pOut = pTmpBuf;

	*pOut++ = (unsigned char)1;

	// 임시 버퍼에 원본 길이를 저장
	*pOut++ = (unsigned char)((nLenSrc >> 24) & 0xff);
	*pOut++ = (unsigned char)((nLenSrc >> 16) & 0xff);
	*pOut++ = (unsigned char)((nLenSrc >>  8) & 0xff);
	*pOut++ = (unsigned char)((nLenSrc      ) & 0xff);

	// 임시 버퍼에 원본 데이터 복사
	if (nLenSrc > 0)
	{
		memcpy(pOut, pSrc, nLenSrc);
		pOut += nLenSrc;
	}

	// 8 byte로 align
	int nLenBuf = pOut - pTmpBuf - 1;
	int nPad = 0;
	if (nLenBuf % 8 != 0)
		nPad = (8 - (nLenBuf % 8));
	while (nPad > 0)
	{
		*pOut++ = (unsigned char)rand();
		nPad--;
	}
	nLenBuf = pOut - pTmpBuf;

	// 암호화
	if (!OLDDES3_Crypt(pKey, pTmpBuf + 1, pDest + 1, nLenBuf - 1, true))
		return -1;
	*pDest = *pTmpBuf;

	return nLenBuf;
}

int CNM_OldDecrypt(const unsigned char* pSrc, int nLenSrc, CNM_KEY_TYPE* pKey, unsigned char* pDest, unsigned char* pTmpBuf)
{
	unsigned char* pOut = pTmpBuf;

	// 복호화 : 더미 제외
	if (!OLDDES3_Crypt(pKey, pSrc + 1, pTmpBuf, nLenSrc - 1, false))
		return -1;

	// 더미
	if (*pSrc != 1)
		return -1;

	// 길이를 가져옴
	int nLenBuf = ((((int)(pOut[0])) << 24) & 0xff000000)
				  | ((((int)(pOut[1])) << 16) & 0x00ff0000)
				  | ((((int)(pOut[2])) <<  8) & 0x0000ff00)
				  | ((((int)(pOut[3]))      ) & 0x000000ff);
	pOut += 4;

	// 평문 데이터 복사
	if (nLenBuf < 0 || nLenBuf >= nLenSrc)
		return -1;
	if (nLenBuf > 0)
	{
		memcpy(pDest, pOut, nLenBuf);
	}
	else
	{
		*pDest = 0;
	}

	return nLenBuf;
}

unsigned int CNM_MakeSeedForClient(const char* strID, const char* strPW, unsigned long nTickCount)
{
	time_t timeDummy;
	time(&timeDummy);

	unsigned int nBitShuffle	= LCDES3_GetRandom32Bit((unsigned int)timeDummy);
	unsigned int nMethod		= LCDES3_GetRandom32Bit(nBitShuffle);
	unsigned int nID			= (unsigned int)(strID[0]);
	unsigned int nPW			= (unsigned int)(strPW[0]);

	nBitShuffle		= nBitShuffle & 0x0000000f;
	nMethod			= (nMethod & 0x0000000f) << 24;
	nID				= nID << 16;
	nPW				= nPW << 8;
	nTickCount		= nTickCount & 0x000000ff;

	unsigned int nInitSeed = nMethod | nID | nPW | nTickCount;
	unsigned int nRet = nBitShuffle << 28;

	int i;
	for (i = 0; i < 28; i++)
	{
		if ((nInitSeed >> i) & 1)
		{
			nRet |= (1 << tableShuffle[nBitShuffle][i]);
		}
	}

	return nRet;
}

unsigned int CNM_MakeSeedForServer(int nRandomValue, int nServerPulse)
{
	time_t timeDummy;
	time(&timeDummy);

	unsigned int nBitShuffle	= LCDES3_GetRandom32Bit((unsigned int)timeDummy);
	unsigned int nMethod		= LCDES3_GetRandom32Bit(nBitShuffle);

	nRandomValue &= 0x00000fff;
	nServerPulse &= 0x00000fff;

	nBitShuffle		= nBitShuffle & 0x0000000f;
	nMethod			= (nMethod & 0x0000000f) << 24;
	nRandomValue	= nRandomValue << 12;

	unsigned int nInitSeed = nMethod | nRandomValue | nServerPulse;
	unsigned int nRet = nBitShuffle << 28;

	int i;
	for (i = 0; i < 28; i++)
	{
		if ((nInitSeed >> i) & 1)
		{
			nRet |= (1 << tableShuffle[nBitShuffle][i]);
		}
	}

	return nRet;
}

void CNM_MakeKeyFromSeed(CNM_KEY_TYPE* pKey, unsigned int nSeed)
{
	unsigned int nData = 0;
	unsigned int nTableIndex = (nSeed >> 28) & 0x0000000f;
	int i;
	for (i = 0; i < 28; i++)
	{
		if ((nSeed >> tableShuffle[nTableIndex][i]) & 1)
		{
			nData |= (1 << i);
		}
	}

	unsigned int nMethod = (nData >> 24) & 0x0000000f;
	nSeed = nData & 0x00ffffff;

	unsigned int nKeyHigh = LCDES3_GetRandom32Bit(nSeed);
	unsigned int nKeyLow  = LCDES3_GetRandom32Bit(nKeyHigh);

	if (nMethod & 8)
	{
		nKeyHigh ^= ~(nKeyHigh & 0xffff0000);
	}
	if (nMethod & 4)
	{
		nKeyHigh ^= ~(nKeyHigh & 0x0000ffff);
	}
	if (nMethod & 2)
	{
		nKeyLow ^= ~(nKeyLow & 0xffff0000);
	}
	if (nMethod & 1)
	{
		nKeyLow ^= ~(nKeyLow & 0x0000ffff);
	}

	LCDES3_InitKey(pKey, nKeyHigh, nKeyLow);
}
//