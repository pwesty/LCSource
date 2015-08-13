//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)

#ifndef __CGROUPTAG_H__
#define __CGROUPTAG_H__
#ifdef PRAGMA_ONCE
#	pragma once
#endif

#include <Engine/Effect/CTag.h>
#include <Engine/Effect/CRefCountPtr.h>
#include <vector>

class CWorld;

class ENGINE_API CGroupTag : public CTag
{
public:
	typedef std::vector< ptr_tag > my_vector;
public:	
	CGroupTag();
	CGroupTag(CTag &tag);
	virtual ~CGroupTag();

	virtual CTag *Copy();

	void AddTag(const ptr_tag &tag);
	void DeleteTag(const ptr_tag &tag);
	void Clear()					{ m_vectorTag.clear(); }
	ptr_tag GetTag(INDEX idx)		{ return m_vectorTag[idx]; }
	const my_vector GetTags()		{ return m_vectorTag; }

	inline bool Empty()				{ return m_vectorTag.empty(); }
	inline INDEX Count()			{ return m_vectorTag.size(); }
protected:
	my_vector		m_vectorTag;	//delete의 경우 많이 안쓰일테니 vector로
};

#endif /* __CGROUPTAG_H__ */
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
