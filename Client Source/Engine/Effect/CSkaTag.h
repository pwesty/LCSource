//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
// CSkaTag.h
//

#if !defined(__CSKATAG_H__)
#define __CSKATAG_H__
#ifdef PRAGMA_ONCE
#	pragma once
#endif

#include <Engine/base/CTString.h>
#include <Engine/Effect/CTag.h>

struct MeshLOD;

class ENGINE_API CSkaTag : public CTag
{
public:
	CSkaTag();
	CSkaTag(CTag &tag);
	virtual ~CSkaTag();
	virtual CTag *Copy();
	
	void AddNewTagInfo(FLOAT time, const Matrix12 &matBlend, const Matrix12 &matStrBlend);
	void AddNewTagInfo(FLOAT time, const Matrix12 &matBlend1, const Matrix12 &matStrBlend1
								 , const Matrix12 &matBlend2, const Matrix12 &matStrBlend2);
	void AddNewTagInfo(FLOAT time, const Matrix12 &matBlend1, const Matrix12 &matStrBlend1
								 , const Matrix12 &matBlend2, const Matrix12 &matStrBlend2
								 , const Matrix12 &matBlend3, const Matrix12 &matStrBlend3);
	void AddNewTagInfo(FLOAT time, const Matrix12 &matBlend1, const Matrix12 &matStrBlend1
								 , const Matrix12 &matBlend2, const Matrix12 &matStrBlend2
								 , const Matrix12 &matBlend3, const Matrix12 &matStrBlend3
								 , const Matrix12 &matBlend4, const Matrix12 &matStrBlend4);

	inline void SetBoneBlendValue( MeshLOD *parent ) { m_pMeshLOD = parent; m_iParentBoneCount = 0; }
	void SetBoneBlendValue( MeshLOD *parent, INDEX boneCount
							, INDEX boneID1, FLOAT weight1
							, INDEX boneID2, FLOAT weight2
							, INDEX boneID3, FLOAT weight3
							, INDEX boneID4, FLOAT weight4 );
	inline MeshLOD *GetMeshLOD() { return m_pMeshLOD; }
	inline INDEX GetBoneCount() { return m_iParentBoneCount; }
	inline INDEX GetBoneID(INDEX bi) { return m_iParentBoneID[bi]; }
	inline FLOAT GetBoneWeight(INDEX bi) { return m_fParentBoneWeights[bi]; }
	
protected:
	
public:
protected:
	MeshLOD			*m_pMeshLOD;
	INDEX			m_iParentBoneCount;
	INDEX			m_iParentBoneID[4];
	FLOAT			m_fParentBoneWeights[4];
	INDEX			M_iMeshLOD;
};

#endif /* __CSKATAG_H__ */
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
