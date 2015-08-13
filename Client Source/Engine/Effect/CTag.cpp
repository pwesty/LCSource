//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
// CTag.cpp
//

#include "stdh.h"
#include "CTag.h"
#include <Engine/ska/ModelInstance.h>
#include <Engine/ska/Render.h>
#include <Engine/Math/Matrix.h>
#include <Engine/Math/Matrix12.h>
#include <Engine/Base/Memory.h>

CModelInstance *g_pmiAxis = NULL;

INDEX	CTag::m_iAxisRefCount = 0;

CTag::CTag()
: m_vOffsetPos(0,0,0)
, m_qOffsetRot(1,0,0,0)
, m_fMaxTimeLength(0.0f)
, m_eType(TT_NOTHING)
, m_fOldestTime( 0.0f )
, m_iActive( 0 )
, m_ulRefCount( 0 )
{
	m_tiTagInfo.m_fTime = 0;
	m_tiTagInfo.m_vPos = m_vOffsetPos;
	m_tiTagInfo.m_qRot = m_qOffsetRot;
}

// 순수가상함수지만 소멸자인 경우는 정의해 주어야함.
CTag::~CTag()
{
}

void CTag::SetContent(CTag *pTag)
{
	m_strName = pTag->m_strName;
	m_eType = pTag->m_eType;
	m_tiTagInfo = pTag->m_tiTagInfo;
	m_vOffsetPos = pTag->m_vOffsetPos;
	m_qOffsetRot = pTag->m_qOffsetRot;
	m_fOldestTime = pTag->m_fOldestTime;
	m_fMaxTimeLength = pTag->m_fMaxTimeLength;
}


void CTag::Render(COLOR col, FLOAT3D strech)
{
	if(g_pmiAxis == NULL) return;
	if(Active())
	{
		g_pmiAxis->SetModelColor(col);
	}
	else
	{
		g_pmiAxis->SetModelColor( MulColors(col, 0x7F7F7F7F) );
	}
	g_pmiAxis->mi_vStretch = strech;
	TagInfo &curInfo = CurrentTagInfo();
	FLOATmatrix3D matRot;
	curInfo.m_qRot.ToMatrix(matRot);
	RM_SetObjectPlacement( matRot, curInfo.m_vPos );
	RM_RenderSKA(*g_pmiAxis);
}

void CTag::AddNewTagInfo(FLOAT time, const FLOATquat3D &rot, const FLOAT3D &pos, const FLOAT3D &stretch)
{
	if(!Active()) return;

	//새로운 태그정보 추가
	TagInfo &info = m_tiTagInfo;
	info.m_fTime = time;
	FLOATquat3D	&infoQRot = info.m_qRot;
	FLOAT3D		&infoVPos = info.m_vPos;
	FLOAT3D		infoVStrech = stretch;;

	//회전처리
	FLOATmatrix3D matOriginalRot;
	infoQRot = rot;
	infoQRot *= m_qOffsetRot;
	rot.ToMatrix(matOriginalRot);

	//위치 처리
	FLOATmatrix3D m;
	m(1,1) = infoVStrech(1);	m(1,2) = 0;					m(1,3) = 0;
	m(2,1) = 0;					m(2,2) = infoVStrech(2);	m(2,3) = 0;
	m(3,1) = 0;					m(3,2) = 0;					m(3,3) = infoVStrech(3);
	infoVPos = pos;
	infoVPos += m_vOffsetPos * matOriginalRot * m;
}

void CTag::AddNewTagInfo(FLOAT time, const Matrix12 &mInfo, const Matrix12 &mStrInfo)
{
	if(!Active()) return;

	//새로운 태그정보 추가
	TagInfo &info = m_tiTagInfo;
	info.m_fTime = time;
	FLOATquat3D	&infoQRot = info.m_qRot;
	FLOAT3D		&infoVPos = info.m_vPos;
	FLOATmatrix3D	rot;
	FLOAT3D			pos;

	//회전처리
	rot(1,1) = mInfo[ 0]; rot(1,2) = mInfo[ 1]; rot(1,3) = mInfo[ 2];
	rot(2,1) = mInfo[ 4]; rot(2,2) = mInfo[ 5]; rot(2,3) = mInfo[ 6];
	rot(3,1) = mInfo[ 8]; rot(3,2) = mInfo[ 9]; rot(3,3) = mInfo[10];
	infoQRot.FromMatrix(rot);
	infoQRot *= m_qOffsetRot;

	//위치처리
	rot(1,1) = mStrInfo[ 0]; rot(1,2) = mStrInfo[ 1]; rot(1,3) = mStrInfo[ 2];
	rot(2,1) = mStrInfo[ 4]; rot(2,2) = mStrInfo[ 5]; rot(2,3) = mStrInfo[ 6];
	rot(3,1) = mStrInfo[ 8]; rot(3,2) = mStrInfo[ 9]; rot(3,3) = mStrInfo[10];
	infoVPos(1) = mStrInfo[ 3];
	infoVPos(2) = mStrInfo[ 7];
	infoVPos(3) = mStrInfo[11];
	infoVPos += m_vOffsetPos * rot;
}

void CTag::AddNewTagInfo(const TagInfo &info)
{
	if(!Active()) return;

	if(info.m_vPos(1) < -FLT_MAX * 0.5f || info.m_vPos(1) > FLT_MAX * 0.5f) return;
	if(info.m_vPos(2) < -FLT_MAX * 0.5f || info.m_vPos(2) > FLT_MAX * 0.5f) return;
	if(info.m_vPos(3) < -FLT_MAX * 0.5f || info.m_vPos(3) > FLT_MAX * 0.5f) return;
	m_tiTagInfo = info;
}

//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
