//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)

#ifndef __CWORLDTAG_H__
#define __CWORLDTAG_H__
#ifdef PRAGMA_ONCE
#	pragma once
#endif

#include <Engine/Effect/CTag.h>

class CWorld;

class ENGINE_API CWorldTag : public CTag
{
public:	
	CWorldTag();
	CWorldTag(CTag &tag);
	virtual ~CWorldTag();

	virtual CTag *Copy();
	
	//tag info control functions, CTag의 함수로 전달
	void AddNewTagInfo(TagInfo &info) { CTag::AddNewTagInfo(info); }
	void AddNewTagInfo(FLOAT time, Matrix12 &mInfo, Matrix12 &mStrInfo) { CTag::AddNewTagInfo(time, mInfo, mStrInfo); }
	void AddNewTagInfo(FLOAT time, FLOATquat3D &rot, FLOAT3D &pos, FLOAT3D &stretch) { CTag::AddNewTagInfo(time, rot, pos, stretch); }
};

#endif /* __CWORLDTAG_H__ */
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
