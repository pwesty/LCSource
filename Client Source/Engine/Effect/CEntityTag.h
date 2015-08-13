//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
// CEntityTag.h
//

#if !defined(CEntityTag_h)
#define CEntityTag_h
#ifdef PRAGMA_ONCE
#	pragma once
#endif

#include <Engine/Effect/CTag.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Entities/EntityPointer.h>

class ENGINE_API CEntityTag : public CTag
{
public:	
	CEntityTag();
	CEntityTag(CTag &tag);
	virtual ~CEntityTag();
	virtual CTag *Copy();

	void AddNewTagInfo(TagInfo &info);
	void AddNewTagInfo(FLOAT time, Matrix12 &mInfo, Matrix12 &mStrInfo);
	void AddNewTagInfo(FLOAT time, FLOATquat3D &rot, FLOAT3D &pos, FLOAT3D &stretch);
	
	inline void SetEntity(CEntityPointer pen) { m_epEntity = pen; }
	
protected:
	CEntityPointer	m_epEntity;
};

#endif /* CEntityTag_h */
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
