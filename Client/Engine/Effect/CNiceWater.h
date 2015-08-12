//강동민 수정 시작 Water 구현		04.22
#ifndef _CNICEWATER_H_
#define _CNICEWATER_H_
#ifdef PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Ska/Skeleton.h>
#include <Engine/Ska/AnimSet.h>
#include <Engine/Math/AABBox.h>
#include <Engine/Base/FileName.h>

// Water의 정보를 저장하는 클래스.
class ENGINE_API CNiceWater
{
public:
	CNiceWater();
	CNiceWater(CEntity* pEntity);
	virtual			~CNiceWater();

	void			Render();			// 냉무(SKA 렌더링시 대부분이 처리됨.)
	void			Clear(void);

public:
	FLOATplane3D	m_plPlane;			// plane in absolute space
	CEntity*		m_pWaterEntity;		// Water의 엔티티.

//protected:
	BOOL			m_bWave;
	BOOL			m_bReflection;		// 반사 유무.
	BOOL			m_bWireFrame;
	BOOL			m_bRenderSky;		// 반사시 하늘을 렌더링할 것인가?
	BOOL			m_bRenderModels;	// 반사시 하늘을 렌더링할 것인가?

	FLOAT			m_fBumpMat11;		// 범프 매핑 행렬값.
	FLOAT			m_fBumpMat12;
	FLOAT			m_fBumpMat21;
	FLOAT			m_fBumpMat22;

	FLOAT			m_fTexFlowX;		// 텍스쳐가 흘러가는 속도 및 방향.
	FLOAT			m_fTexFlowY;

//강동민 수정 시작 테스트 클라이언트 작업	06.17
	CTextureObject	m_toBump;			// 범프맵 텍스쳐1
	CTextureObject	m_toWater;			// 물 텍스쳐.
//강동민 수정 끝 테스트 클라이언트 작업		06.17

	CTFileName		m_fnBumpName;
	CTFileName		m_fnWaterName;
};

#endif  /* include-once check. */
//강동민 수정 끝 Water 구현			04.22