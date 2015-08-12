#ifndef __CRYPT_NET_MSG_H__
#define __CRYPT_NET_MSG_H__

// ************************************************************************************
#ifdef	CRYPT_NET_MSG_LEVEL2


#define CRYPT_INIT_CHAR				128

static unsigned char	g_ubKey[]	= { 0x77, 0x6C, 0x71, 0x64, 0x70, 0x72, 0x6B, 0x72, 0x68, 0x74, 0x6C, 0x76, 0x65, 0x6B };

/*
#if defined (LC_KOR) || defined (LC_CHN)
#define CRYPT_XOR_STRING			"dkwk"
#else
#define CRYPT_XOR_STRING			"nako"
#endif*/

#define CRYPT_XOR_STRING_LENGTH		4

inline unsigned char* CryptMem(unsigned char* buf, int size, unsigned char arrKeyToken[], int nKeyToken)
{
	char ch = (char)CRYPT_INIT_CHAR;
//	const char* strxor = CRYPT_XOR_STRING;
	const unsigned char* strxor = arrKeyToken;
	int i;
	for (i = 0; i < size; i++)
	{
		buf[i] += ch;
		ch = buf[i];
		buf[i] ^= strxor[i % nKeyToken];
	}
	return buf;
}

inline unsigned char* DecryptMem(unsigned char* buf, int size, unsigned char arrKeyToken[], int nKeyToken)
{
	char ch = (char)CRYPT_INIT_CHAR;
	const unsigned char* strxor = arrKeyToken;
	int i;
	for (i = 0; i < size; i++)
	{
		buf[i] ^= strxor[i % nKeyToken];
		buf[i] -= ch;
		ch = buf[i] + ch;
	}
	return buf;
}

#else	// CRYPT_NET_MSG_LEVEL2
// ************************************************************************************

// 4바이트 변수에서 각 바이트 값을 unsigned char로 얻음, 하위 바이트 0 ~ 최상위 바이트 3
#define CNM_BYTE_0(n)		((unsigned char)((n      ) & 0x000000ff))
#define CNM_BYTE_1(n)		((unsigned char)((n >>  8) & 0x000000ff))
#define CNM_BYTE_2(n)		((unsigned char)((n >> 16) & 0x000000ff))
#define CNM_BYTE_3(n)		((unsigned char)((n >> 24) & 0x000000ff))

#define CNM_MAKEWORD(low, high)			((unsigned short)(((high << 8) & 0xff00) | (low & 0x00ff)))
#define CNM_MAKELONG(b0, b1, b2, b3)	((unsigned int)((b0 & 0x000000ff) | ((b1 << 8) & 0x0000ff00) | ((b2 << 16) & 0x00ff0000) | ((b3 << 24) & 0xff000000)))

#define CNM_ROTATE_RIGHT(n)	((unsigned char)((((n << 7) & 0x80) | ((n >> 1) & 0x7f)) & 0xff))
#define CNM_ROTATE_LEFT(n)	((unsigned char)(((n >> 7) & 0x01) | ((n << 1) & 0xfe) & 0xff))

#define CNM_INIT_KEY		((unsigned int)(1852535664))	// 초기 키 값 : nksp

#define CNM_DUMMY_SIZE		(1)								// 더미 사이즈 : 더미는 메시지 타입 대신에 CNetMsg에서 사용
#define CNM_BEGIN_SIZE		(1)								// 시작 마크 크기
#define CNM_END_SIZE		(1)								// 끝 마크 크기
#define CNM_CHECKSUM_SIZE	(2)								// 체크섬 크기
#define CNM_DUMMY_TYPE		((unsigned char)1)				// 더미
#define CNM_BEGIN_MARK		((unsigned char)('l'))			// 시작 마크 : l
#define CNM_END_MARK		((unsigned char)('c'))			// 시작 마크 : c

#define CNM_CRC				(0x8003)						// CRC-16

// 키 랜덤 생성
inline unsigned int CNM_KeyGen()
{
	return CNM_MAKELONG((unsigned char)rand(), (unsigned char)rand(), (unsigned char)rand(), (unsigned char)rand());
}

// 16비트 CRC 생성
inline unsigned short CNM_CalcCRC(unsigned char* pData, int nLen)
{
	unsigned short crc = 0;
	int i;

	while (nLen-- > 0)
	{
		crc = crc ^ ((int)*pData++ << 8);
		i = 8;
		while (i-- > 0)
		{
			if (crc & 0x8000)
				crc = (crc << 1) ^ CNM_CRC;
			else
				crc <<= 1;
		}
	}

	return crc;
}


////////////////////
// Function name	: CNM_Crypt
// Description	    : 메시지 암호화
// Return type		: int 
//                  : 암호화된 길이, 오류시 -1
// Argument         : const unsigned char* pSrc
//                  : 원본 데이터 버퍼
// Argument         : int nLenSrc
//                  : 원본 길이
// Argument         : unsigned int nKey
//                  : 사용되는 키값
// Argument         : unsigned char** pDest
//                  : 복호 데이터 버퍼, new로 할당함, 호출한 곳에서 delete [] 필요
inline int CNM_Crypt(const unsigned char* pSrc, int nLenSrc, unsigned int nKey, unsigned char** pDest)
{
	// 변환 테이블
	#include "CryptNetMsg_TransTable"

	int nLenDest = nLenSrc + (CNM_BEGIN_SIZE + CNM_END_SIZE + CNM_CHECKSUM_SIZE + CNM_DUMMY_SIZE);
	unsigned short nCheckSum = 0;	// 체크섬
	int nIndexSrc = 0;				// pSrc 버퍼 인덱스
	int nIndexDest = 0;				// pDest 버퍼 인덱스
	unsigned char btTrans;			// 변환 테이블용
	unsigned char btKey[4];			// 키를 바이트로 분리

	(*pDest) = NULL;

	if (nLenSrc < 0)
		return -1;

	btKey[0] = CNM_BYTE_3(nKey);
	btKey[1] = CNM_BYTE_2(nKey);
	btKey[2] = CNM_BYTE_1(nKey);
	btKey[3] = CNM_BYTE_0(nKey);

	(*pDest) = new unsigned char[nLenDest];
	memset((*pDest), 0, sizeof(unsigned char) * nLenDest);

	// 더미 삽입
	(*pDest)[nIndexDest] = CNM_DUMMY_TYPE;
	nIndexDest++;

	// 시작마크 삽입
	(*pDest)[nIndexDest] = CNM_BEGIN_MARK;
	nIndexDest++;

	// 데이터 복사
	while (nIndexSrc < nLenSrc)
	{
		(*pDest)[nIndexDest] = pSrc[nIndexSrc];
		nIndexDest++;
		nIndexSrc++;
	}

	// 끝 마크 삽입
	(*pDest)[nIndexDest] = CNM_END_MARK;
	nIndexDest++;

	// 체크섬 생성 : 더미 제외
	nCheckSum = CNM_CalcCRC((*pDest) + CNM_DUMMY_SIZE, nIndexDest - CNM_DUMMY_SIZE);

	// 체크섬 삽입
	(*pDest)[nIndexDest] = CNM_BYTE_1(nCheckSum);
	nIndexDest++;
	(*pDest)[nIndexDest] = CNM_BYTE_0(nCheckSum);;
	nIndexDest++;

	// 데이터 암호화 : 더미 제외
	nIndexDest = CNM_DUMMY_SIZE;
	while (nIndexDest < nLenDest)
	{
		// 치환
		btTrans = CNM_TransTable[(*pDest)[nIndexDest]];

		// 키와 XOR
		btTrans ^= btKey[nIndexDest % 4];

		// 오른쪽으로 Rotate
		btTrans = CNM_ROTATE_RIGHT(btTrans);

		// 키 변경
		btKey[nIndexDest % 4] = btTrans;

		// 값 저장
		(*pDest)[nIndexDest] = btTrans;

		nIndexDest++;
	}

	return nLenDest;
}


////////////////////
// Function name	: CNM_Decrypt
// Description	    : 메시지 복호화
// Return type		: int 
//                  : 복호화된 길이, 오류시 -1
// Argument         : const unsigned char* pSrc
//                  : 암호 데이터 버퍼
// Argument         : int nLenSrc
//                  : 암호 데이터 길이
// Argument         : unsigned int nKey
//                  : 사용되는 키값
// Argument         : unsigned char** pDest
//                  : 복호 데이터 버퍼, new로 할당함, 호출한 곳에서 delete [] 필요
inline int CNM_Decrypt(const unsigned char* pSrc, int nLenSrc, unsigned int nKey, unsigned char** pDest)
{
	// 변환 테이블
	#include "CryptNetMsg_TransTable"

	int nLenDest = nLenSrc - (CNM_BEGIN_SIZE + CNM_END_SIZE + CNM_CHECKSUM_SIZE + CNM_DUMMY_SIZE);
	unsigned short nCheckSum = 0;	// 체크섬
	unsigned char btKey[4];			// 키를 바이트로 분리
	int nIndexSrc = 0;				// pSrc 버퍼 인덱스
	int nIndexDest = 0;				// pDest 버퍼 인덱스
	unsigned char btTrans;			// 변환 테이블용
	unsigned char* pTmpBuf;			// 임시용

	(*pDest) = NULL;

	if (nLenDest < 0)
		return -1;

	btKey[0] = CNM_BYTE_3(nKey);
	btKey[1] = CNM_BYTE_2(nKey);
	btKey[2] = CNM_BYTE_1(nKey);
	btKey[3] = CNM_BYTE_0(nKey);

	pTmpBuf = new unsigned char[nLenSrc];

	// 데이터 복호화 : 더미 제외
	nIndexSrc = CNM_DUMMY_SIZE;
	while (nIndexSrc < nLenSrc)
	{
		// 현재 복호값을 저장
		btTrans = pSrc[nIndexSrc];

		// 왼쪽으로 Rotate
		btTrans = CNM_ROTATE_LEFT(btTrans);

		// 키와 XOR
		btTrans ^= btKey[nIndexSrc % 4];

		// 치환
		btTrans = CNM_RTransTable[btTrans];

		// 키변경
		btKey[nIndexSrc % 4] = pSrc[nIndexSrc];

		// 값 저장
		pTmpBuf[nIndexSrc] = btTrans;

		nIndexSrc++;
	}

	// 체크섬 검사 : 더미 제외
	nCheckSum = CNM_MAKEWORD(pTmpBuf[nLenSrc - 1], pTmpBuf[nLenSrc - 2]);
	if (nCheckSum != CNM_CalcCRC(pTmpBuf + CNM_DUMMY_SIZE, nLenSrc - CNM_CHECKSUM_SIZE - CNM_DUMMY_SIZE))
	{
		delete [] pTmpBuf;
		return -1;
	}

	// 시작마크 검사
	if (pTmpBuf[CNM_DUMMY_SIZE] != CNM_BEGIN_MARK)
	{
		delete [] pTmpBuf;
		return -1;
	}

	// 끝마크 검사
	if (pTmpBuf[nLenSrc - CNM_CHECKSUM_SIZE - 1] != CNM_END_MARK)
	{
		delete [] pTmpBuf;
		return -1;
	}

	(*pDest) = new unsigned char[nLenDest];
	memcpy((*pDest), pTmpBuf + CNM_BEGIN_SIZE + CNM_DUMMY_SIZE, sizeof(unsigned char) * nLenDest);

	delete [] pTmpBuf;

	return nLenDest;
}

#endif	// CRYPT_NET_MSG_LEVEL2

#endif // __CRYPT_NET_MSG_H__