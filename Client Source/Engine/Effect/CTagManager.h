//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)

#ifndef __CTAGMANAGER_H__
#define __CTAGMANAGER_H__

#include <Engine/Base/Memory.h>
#include "CTag.h"
#include "CRefCountPtr.h"

#pragma warning(disable : 4786)
#include <map>
#pragma warning(disable : 4786)
#include <string>
#pragma warning(disable : 4786)
#include <vector>
#include <algorithm>

class ENGINE_API CTagManager
{
public:
	typedef CRefCountPtr<CTag>				my_ptr;
	typedef std::map< std::string, my_ptr >	my_map;
public:
	CTagManager();
	~CTagManager();

	//등록시 Copy를 사용한다.
	BOOL Register(CTag *pTag);	//copy하면서 등록
	BOOL RegisterNoCopy(CTag *pTag);	//copy하지 않으며서 등록
//안태훈 수정 시작	//(For Performance)(0.2)
	BOOL Unregister(std::string &name);
	BOOL Unregister(const char *name);
	BOOL IsRegister(std::string &name);
	BOOL IsRegister(const char *name);
	CTag *Find(std::string &name, BOOL bFindHierarchy = FALSE);
	CTag *Find(const char *name, BOOL bFindHierarchy = FALSE);
//안태훈 수정 끝	//(For Performance)(0.2)
	void Clear();
	BOOL Empty() { return m_mapRegistered.empty() && m_vectorChild.empty(); }	//비어있나?

	inline void AddChild(CTagManager *ptmChild)		
	{ 
		m_vectorChild.push_back(ptmChild);
	}
	inline void RemoveChild(CTagManager *ptmChild)
	{
		std::vector<CTagManager*>::iterator iter = std::find(m_vectorChild.begin(), m_vectorChild.end(), ptmChild);
		if(iter != m_vectorChild.end()) 
			m_vectorChild.erase(iter);
	}

	inline void ClearChild()
	{
		m_vectorChild.clear();
	}

	inline void SetOwner(CEntity *pen)	{ m_penOwner = pen;		}
	inline CEntity *GetOwner()			{ return m_penOwner;	}

	inline void SetTempValue(INDEX temp)	{ m_iTempValue = temp;	}
	inline INDEX GetTempValue()				{ return m_iTempValue;	}

	BOOL HasActiveTag();

	CTag *GetRootTag()					{ return m_ptrRootTag.GetNative();	}

protected:
	my_map	m_mapRegistered;
	std::vector<CTagManager*>	m_vectorChild;
	CEntity	*m_penOwner;
	INDEX	m_iTempValue;
	my_ptr	m_ptrRootTag;
};

ENGINE_API extern CTagManager g_tmWorld;


#endif //__CTAGMANAGER_H__

//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)