//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#ifndef __CRANDOMTABLE_H__
#define __CRANDOMTABLE_H__

#include <Engine/Base/Memory.h>
#include <Engine/Base/Types.h>
#include <Engine/Math/Vector.h>

#define RANDOM_COUNT	4096

class ENGINE_API CRandomTable
{
public:
	static inline CRandomTable &Instance() { return m_instance; }
	inline const FLOAT3D & GetRndPos()
	{
		m_iPosIndex = m_iPosIndex+1 < RANDOM_COUNT ? m_iPosIndex+1 : 0 ;
		return m_vPosTable[m_iPosIndex];
	}
	inline const FLOAT3D & GetRndDir()
	{
		m_iDirIndex = m_iDirIndex+1 < RANDOM_COUNT ? m_iDirIndex+1 : 0 ;
		return m_vDirTable[m_iDirIndex];
	}
	inline const FLOAT3D & GetRndCylinderPos()
	{
		m_iCylinderPosIndex = m_iCylinderPosIndex+1 < RANDOM_COUNT ? m_iCylinderPosIndex+1 : 0 ;
		return m_vCylinderPosTable[m_iCylinderPosIndex];
	}
	inline const FLOAT3D & GetRndCylinderDir()
	{
		m_iCylinderDirIndex = m_iCylinderDirIndex+1 < RANDOM_COUNT ? m_iCylinderDirIndex+1 : 0 ;
		return m_vCylinderDirTable[m_iCylinderDirIndex];
	}
	inline FLOAT GetRndOffset()
	{
		m_iOffsetIndex = m_iOffsetIndex+1 < RANDOM_COUNT ? m_iOffsetIndex+1 : 0;
		return m_fOffsetTable[m_iOffsetIndex];
	}
	inline FLOAT GetRndFactor()
	{
		m_iFactorIndex = m_iFactorIndex+1 < RANDOM_COUNT ? m_iFactorIndex+1 : 0;
		return m_fFactorTable[m_iFactorIndex];
	}

	~CRandomTable();
	
protected:
	CRandomTable();

public:
protected:
	FLOAT3D		m_vPosTable[RANDOM_COUNT]; // [-1,1], 변의 길이가 1인 구안의 점
	FLOAT3D		m_vDirTable[RANDOM_COUNT]; // [-1,1], 반지름이 항상 1인 구 위의 점.
	FLOAT3D		m_vCylinderPosTable[RANDOM_COUNT];	//반지름1, 높이가 0~1인 cylinder안의 random한 점.
	FLOAT3D		m_vCylinderDirTable[RANDOM_COUNT];	//반지름1, 높이가 0~1인 cylinder위의 random한 점.
	FLOAT		m_fOffsetTable[RANDOM_COUNT]; // [-1,1]
	FLOAT		m_fFactorTable[RANDOM_COUNT]; // [0,1]
	INDEX		m_iPosIndex;
	INDEX		m_iDirIndex;
	INDEX		m_iCylinderPosIndex;
	INDEX		m_iCylinderDirIndex;
	INDEX		m_iOffsetIndex;
	INDEX		m_iFactorIndex;
	static CRandomTable m_instance;
};

inline const FLOAT3D & RndPos()
{
	return CRandomTable::Instance().GetRndPos();
}
inline const FLOAT3D & RndDir()
{
	return CRandomTable::Instance().GetRndDir();
}
inline const FLOAT3D & RndCylinderPos()
{
	return CRandomTable::Instance().GetRndCylinderPos();
}
inline const FLOAT3D & RndCylinderDir()
{
	return CRandomTable::Instance().GetRndCylinderDir();
}
// -1 ~ 1
inline FLOAT RndOffset()
{
	return CRandomTable::Instance().GetRndOffset();
}
// 0 ~ 1
inline FLOAT RndFactor()
{
	return CRandomTable::Instance().GetRndFactor();
}

#endif //__CRANDOMTABLE_H__
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
