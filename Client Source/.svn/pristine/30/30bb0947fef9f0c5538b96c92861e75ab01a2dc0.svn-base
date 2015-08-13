//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
// CSkaTag.cpp
//

#include "stdh.h"
#include "CSkaTag.h"
#include <Engine/Ska/ModelInstance.h>
#include <Engine/Ska/Mesh.h>
#include <Engine/Math/Matrix12.h>
#include <Engine/Math/Functions.h>
#include <engine/Base/Memory.h>

#define BONE_BLEND_ZERO		0
#define BONE_BLEND_ONE		1
#define BONE_BLEND_TWO		2
#define BONE_BLEND_THREE	3
#define BONE_BLEND_FOUR		4

#define FLOAT_SAME(f1, f2)	( fabs((f1)-(f2)) < (0.0001f) ? true : false )

CSkaTag::CSkaTag()
: m_pMeshLOD( NULL )
, m_iParentBoneCount( 0 )
{
	m_eType = TT_SKA;
	m_iActive = 1;
}

CSkaTag::CSkaTag(CTag &tag)
: m_pMeshLOD( NULL )
, m_iParentBoneCount( 0 )
{
	//tag
	m_eType = TT_SKA;
	m_strName = tag.GetName();
	m_fMaxTimeLength = tag.GetMaxTimeLength();
	m_vOffsetPos = tag.GetOffsetPos();
	m_qOffsetRot = tag.GetOffsetRot();
	CTag::AddNewTagInfo(tag.CurrentTagInfo());
}

CSkaTag::~CSkaTag()
{
}

CTag *CSkaTag::Copy()
{
	CSkaTag *pTag = new CSkaTag;
	//tag
	pTag->SetContent(this);
	//ska tag
	pTag->m_pMeshLOD = m_pMeshLOD;
	pTag->m_iParentBoneCount = m_iParentBoneCount;
	for(INDEX i=0; i<4; ++i)
	{
		pTag->m_iParentBoneID[i] = m_iParentBoneID[i];
		pTag->m_fParentBoneWeights[i] = m_fParentBoneWeights[i];
	}
	return pTag;
}

void CSkaTag::SetBoneBlendValue( MeshLOD *parent, INDEX boneCount
								 , INDEX boneID1, FLOAT weight1
								 , INDEX boneID2, FLOAT weight2
								 , INDEX boneID3, FLOAT weight3
								 , INDEX boneID4, FLOAT weight4 )
{
	ASSERT(boneCount > 0);
	m_pMeshLOD = parent;
	m_iParentBoneCount = boneCount;
	m_iParentBoneID[0] = boneID1;
	m_fParentBoneWeights[0] = weight1;
	m_iParentBoneID[1] = boneID2;
	m_fParentBoneWeights[1] = weight2;
	m_iParentBoneID[2] = boneID3;
	m_fParentBoneWeights[2] = weight3;
	m_iParentBoneID[3] = boneID4;
	m_fParentBoneWeights[3] = weight4;
	if(boneCount > 1)
	{
		FLOAT exceptOne = m_fParentBoneWeights[0];
		for(INDEX i=1; i<m_iParentBoneCount-1; ++i) exceptOne += m_fParentBoneWeights[i];
		m_fParentBoneWeights[ m_iParentBoneCount-1 ] = 1.0f - exceptOne;
	}
}

void CSkaTag::AddNewTagInfo(FLOAT time, const Matrix12 &matBlend, const Matrix12 &matStrBlend)
{
	ASSERT( BONE_BLEND_ZERO == m_iParentBoneCount || BONE_BLEND_ONE == m_iParentBoneCount );
	ASSERT( BONE_BLEND_ZERO == m_iParentBoneCount || FLOAT_SAME(m_fParentBoneWeights[0], 1.0f) );

	CTag::AddNewTagInfo(time, matBlend, matStrBlend);
}

void CSkaTag::AddNewTagInfo(FLOAT time, const Matrix12 &matBlend1, const Matrix12 &matStrBlend1
									  , const Matrix12 &matBlend2, const Matrix12 &matStrBlend2)
{
	ASSERT(BONE_BLEND_TWO == m_iParentBoneCount);
	ASSERT( FLOAT_SAME(m_fParentBoneWeights[0] + m_fParentBoneWeights[1], 1.0f) );

	//CTag의 코드에 기반하여 각 본별 계산
	CTag::AddNewTagInfo(time, matBlend1, matStrBlend1);
	TagInfo tiBlend1 = CTag::CurrentTagInfo();
	CTag::AddNewTagInfo(time, matBlend2, matStrBlend2);
	TagInfo tiBlend2 = CTag::CurrentTagInfo();

	//Lerp pos
	tiBlend1.m_vPos -= tiBlend2.m_vPos;
	tiBlend1.m_vPos *= m_fParentBoneWeights[0];
	tiBlend1.m_vPos += tiBlend2.m_vPos;
	//Lerp rot
	tiBlend1.m_qRot = Slerp(m_fParentBoneWeights[0], tiBlend2.m_qRot, tiBlend1.m_qRot);

	CTag::AddNewTagInfo(tiBlend1);
}

void CSkaTag::AddNewTagInfo(FLOAT time, const Matrix12 &matBlend1, const Matrix12 &matStrBlend1
									  , const Matrix12 &matBlend2, const Matrix12 &matStrBlend2
									  , const Matrix12 &matBlend3, const Matrix12 &matStrBlend3)
{
	ASSERT(BONE_BLEND_THREE == m_iParentBoneCount);
	ASSERT( FLOAT_SAME(m_fParentBoneWeights[0] + m_fParentBoneWeights[1] + m_fParentBoneWeights[2], 1.0f) );

	//CTag의 코드에 기반하여 각 본별 계산
	CTag::AddNewTagInfo(time, matBlend1, matStrBlend1);
	TagInfo tiBlend1 = CTag::CurrentTagInfo();
	CTag::AddNewTagInfo(time, matBlend2, matStrBlend2);
	TagInfo tiBlend2 = CTag::CurrentTagInfo();
	CTag::AddNewTagInfo(time, matBlend3, matStrBlend3);
	TagInfo tiBlend3 = CTag::CurrentTagInfo();

	//value for Lerp
	FLOAT fFactor1 = m_fParentBoneWeights[0] / (m_fParentBoneWeights[0] + m_fParentBoneWeights[1]);
	FLOAT fFactor12 = m_fParentBoneWeights[0] + m_fParentBoneWeights[1];
	//Lerp pos
	// 1 and 2
	tiBlend1.m_vPos -= tiBlend2.m_vPos;
	tiBlend1.m_vPos *= fFactor1;
	tiBlend1.m_vPos += tiBlend2.m_vPos;
	// 12 and 3
	tiBlend1.m_vPos -= tiBlend3.m_vPos;
	tiBlend1.m_vPos *= fFactor12;
	tiBlend1.m_vPos += tiBlend3.m_vPos;

	//Lerp rot
	tiBlend1.m_qRot = Slerp(fFactor1, tiBlend2.m_qRot, tiBlend1.m_qRot);
	tiBlend1.m_qRot = Slerp(fFactor12, tiBlend3.m_qRot, tiBlend1.m_qRot);

	CTag::AddNewTagInfo(tiBlend1);
}

void CSkaTag::AddNewTagInfo(FLOAT time, const Matrix12 &matBlend1, const Matrix12 &matStrBlend1
									  , const Matrix12 &matBlend2, const Matrix12 &matStrBlend2
									  , const Matrix12 &matBlend3, const Matrix12 &matStrBlend3
									  , const Matrix12 &matBlend4, const Matrix12 &matStrBlend4)
{
	ASSERT(BONE_BLEND_FOUR == m_iParentBoneCount);
	ASSERT( FLOAT_SAME(m_fParentBoneWeights[0] + m_fParentBoneWeights[1] + m_fParentBoneWeights[2] + m_fParentBoneWeights[3], 1.0f) );

	//CTag의 코드에 기반하여 각 본별 계산
	CTag::AddNewTagInfo(time, matBlend1, matStrBlend1);
	TagInfo tiBlend1 = CTag::CurrentTagInfo();
	CTag::AddNewTagInfo(time, matBlend2, matStrBlend2);
	TagInfo tiBlend2 = CTag::CurrentTagInfo();
	CTag::AddNewTagInfo(time, matBlend3, matStrBlend3);
	TagInfo tiBlend3 = CTag::CurrentTagInfo();
	CTag::AddNewTagInfo(time, matBlend4, matStrBlend4);
	TagInfo tiBlend4 = CTag::CurrentTagInfo();

	//Lerp pos
	FLOAT fFactor1 = m_fParentBoneWeights[0] / (m_fParentBoneWeights[0] + m_fParentBoneWeights[1]);
	FLOAT fFactor3 = m_fParentBoneWeights[2] / (m_fParentBoneWeights[2] + m_fParentBoneWeights[3]);
	FLOAT fFactor12 = m_fParentBoneWeights[0] + m_fParentBoneWeights[1];

	// 1 and 2
	tiBlend1.m_vPos -= tiBlend2.m_vPos;
	tiBlend1.m_vPos *= fFactor1;
	tiBlend1.m_vPos += tiBlend2.m_vPos;

	// 3 and 4
	tiBlend3.m_vPos -= tiBlend4.m_vPos;
	tiBlend3.m_vPos *= fFactor3;
	tiBlend3.m_vPos += tiBlend4.m_vPos;

	// 12 and 34
	tiBlend1.m_vPos -= tiBlend3.m_vPos;
	tiBlend1.m_vPos *= fFactor12;
	tiBlend1.m_vPos += tiBlend3.m_vPos;

	//Lerp rot
	tiBlend1.m_qRot = Slerp(fFactor1, tiBlend3.m_qRot, tiBlend1.m_qRot);
	tiBlend3.m_qRot = Slerp(fFactor3, tiBlend4.m_qRot, tiBlend3.m_qRot);
	tiBlend1.m_qRot = Slerp(fFactor12, tiBlend3.m_qRot, tiBlend1.m_qRot);

	CTag::AddNewTagInfo(tiBlend1);
}
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
