#ifndef __DES3_RANDOM_KEY_H__
#define __DES3_RANDOM_KEY_H__

// #ifndef 등으로 중복 include를 방지할 필요 없다!!!!
// 또한 절대 des3.cpp 이외의 곳에 include 해서는 안된다

// Key Gen용 Random 함수 임
// 서버 클라이언트 공통 함수로
// 모두 inline이고 des3.cpp 이외에서 include 하면 안됨
// CircleMUD에서 가져옴

#define DES3_RANDOM_M			((unsigned int)0x7FFFFFFF)
#define DES3_RANDOM_Q			((unsigned int)0x0001F31D)
#define DES3_RANDOM_A			((unsigned int)0x000041A7)
#define DES3_RANDOM_R			((unsigned int)0x00000B14)

inline unsigned int LCDES3_GetRandom32Bit(register unsigned int nSeed)
{
	register unsigned int nLow, nHigh;
	nHigh = nSeed / DES3_RANDOM_Q;
	nLow  = nSeed % DES3_RANDOM_Q;
	nSeed = DES3_RANDOM_A * nLow + DES3_RANDOM_R * nHigh;
	return nSeed;
}
//

#endif
