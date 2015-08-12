//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
// CEntityTag.cpp
//

#include "stdh.h"
#include "CEntityTag.h"
#include <engine/Base/Memory.h>
#include <Engine/Base/Memory.h>

CEntityTag::CEntityTag()
{
	m_eType = TT_ENTITY;
}

CEntityTag::CEntityTag(CTag &tag)
{
	//tag
	m_eType = TT_ENTITY;
	m_strName = tag.GetName();
	m_fMaxTimeLength = tag.GetMaxTimeLength();
	m_vOffsetPos = tag.GetOffsetPos();
	m_qOffsetRot = tag.GetOffsetRot();
	this->AddNewTagInfo(tag.CurrentTagInfo());
}

CEntityTag::~CEntityTag()
{
}

CTag *CEntityTag::Copy()
{
	CEntityTag *pTag = new CEntityTag;
	//tag
	pTag->SetContent(this);
	//ska tag
	pTag->m_epEntity = m_epEntity;
	return pTag;
}

void CEntityTag::AddNewTagInfo(FLOAT time, FLOATquat3D &rot, FLOAT3D &pos, FLOAT3D &stretch)
{
	if(!Active()) return;

	//엔티티의 정보
	FLOAT3D vEntityPos(0,0,0);
	FLOATquat3D qEntityRot(1,0,0,0);
	if( ((CEntity*)m_epEntity) != NULL )
	{
		vEntityPos = m_epEntity->en_plPlacement.pl_PositionVector;
		qEntityRot.FromEuler(m_epEntity->en_plPlacement.pl_OrientationAngle);
	}
	//새로운 태그정보 추가
	CTag::AddNewTagInfo(time, rot*qEntityRot, pos+vEntityPos, stretch);
}

void CEntityTag::AddNewTagInfo(FLOAT time, Matrix12 &mInfo, Matrix12 &mStrInfo)
{
	if(!Active()) return;

	//엔티티의 정보
	FLOAT3D vEntityPos(0,0,0);
	FLOATquat3D qEntityRot(1,0,0,0);
	if( ((CEntity*)m_epEntity) != NULL )
	{
		vEntityPos = m_epEntity->en_plPlacement.pl_PositionVector;
		qEntityRot.FromEuler(m_epEntity->en_plPlacement.pl_OrientationAngle);
	}

	//새로운 태그정보
	TagInfo info;
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
	infoQRot *= m_qOffsetRot * qEntityRot;

	//위치처리
	rot(1,1) = mStrInfo[ 0]; rot(1,2) = mStrInfo[ 1]; rot(1,3) = mStrInfo[ 2];
	rot(2,1) = mStrInfo[ 4]; rot(2,2) = mStrInfo[ 5]; rot(2,3) = mStrInfo[ 6];
	rot(3,1) = mStrInfo[ 8]; rot(3,2) = mStrInfo[ 9]; rot(3,3) = mStrInfo[10];
	infoVPos(1) = mStrInfo[ 3];
	infoVPos(2) = mStrInfo[ 7];
	infoVPos(3) = mStrInfo[11];
	infoVPos += m_vOffsetPos * rot + vEntityPos;

	//새로운 태그정보 추가
	CTag::AddNewTagInfo(info);
}

void CEntityTag::AddNewTagInfo(TagInfo &info)
{
	if(!Active()) return;

	//엔티티의 정보
	FLOAT3D vEntityPos(0,0,0);
	FLOATquat3D qEntityRot(1,0,0,0);
	if( ((CEntity*)m_epEntity) != NULL )
	{
		vEntityPos = m_epEntity->en_plPlacement.pl_PositionVector;
		qEntityRot.FromEuler(m_epEntity->en_plPlacement.pl_OrientationAngle);
	}

	TagInfo infoNew = info;
	infoNew.m_fTime = info.m_fTime;
	infoNew.m_qRot *= qEntityRot;
	infoNew.m_vPos += vEntityPos;

	//새로운 태그정보 추가
	CTag::AddNewTagInfo(info);
}

//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
