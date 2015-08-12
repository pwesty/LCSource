//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)

#include "StdH.h"
#include "CRandomTable.h"
#include <Engine/Base/Memory.h>

CRandomTable CRandomTable::m_instance;

CRandomTable::CRandomTable()
: m_iPosIndex( 0 )
, m_iCylinderPosIndex( 0 )
, m_iDirIndex( 0 )
, m_iOffsetIndex( 0 )
, m_iFactorIndex( 0 )
{
	srand( GetTickCount() );

	//table 생성
	for(int i=0; i<RANDOM_COUNT; ++i)
	{
		static const FLOAT inv_rand_max = 1.0f / (FLOAT)RAND_MAX;
		static const FLOAT inv_rand_max_half = 1.0f / (FLOAT)RAND_MAX * 2.0f;

		m_vPosTable[i](1) = rand() * inv_rand_max_half - 1.0f;
		m_vPosTable[i](2) = rand() * inv_rand_max_half - 1.0f;
		m_vPosTable[i](3) = rand() * inv_rand_max_half - 1.0f;
		m_vPosTable[i].Normalize() *= rand() * inv_rand_max;
		
		m_vCylinderPosTable[i](1) = rand() * inv_rand_max_half - 1.0f;
		m_vCylinderPosTable[i](2) = 0;
		m_vCylinderPosTable[i](3) = rand() * inv_rand_max_half - 1.0f;
		m_vCylinderPosTable[i].Normalize() *= rand() * inv_rand_max;
		m_vCylinderPosTable[i](2) = rand() * inv_rand_max;

		m_vCylinderDirTable[i](1) = rand() * inv_rand_max_half - 1.0f;
		m_vCylinderDirTable[i](2) = 0;
		m_vCylinderDirTable[i](3) = rand() * inv_rand_max_half - 1.0f;
		m_vCylinderDirTable[i].Normalize();
		m_vCylinderDirTable[i](2) = rand() * inv_rand_max;

		m_vDirTable[i](1) = rand() * inv_rand_max_half - 1.0f;
		m_vDirTable[i](2) = rand() * inv_rand_max_half - 1.0f;
		m_vDirTable[i](3) = rand() * inv_rand_max_half - 1.0f;
		m_vDirTable[i].Normalize();

		m_fOffsetTable[i] = rand() * inv_rand_max_half - 1.0f;
		m_fFactorTable[i] = rand() * inv_rand_max;
	}
}

CRandomTable::~CRandomTable()
{
}

//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)