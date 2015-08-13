//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)

#include "stdH.h"
#pragma warning(disable : 4786)
#include "CTagManager.h"
#include <Engine/Base/Memory.h>

CTagManager g_tmWorld;

CTagManager::CTagManager()
: m_penOwner( NULL )
, m_iTempValue( 0 )
{
	m_ptrRootTag.SetNull();
}

CTagManager::~CTagManager()
{
	m_mapRegistered.clear();
}

BOOL CTagManager::Register(CTag *pTag)
{
	//이름이 없거나 같은 이름으로 등록된 것이 있음
	if( pTag->GetName() == "" || Find(pTag->GetName()) != NULL ) return FALSE;

	my_ptr ptr( pTag->Copy() );
	if( pTag->GetName() == "__ROOT" ) m_ptrRootTag = ptr;
	my_map::value_type registerTri( pTag->GetName(), ptr );
	//등록 성공 or 실패 정보를 리턴
	return m_mapRegistered.insert( registerTri ).second;
}

BOOL CTagManager::RegisterNoCopy(CTag *pTag)
{
	//이름이 없거나 같은 이름으로 등록된 것이 있음
	if( pTag->GetName() == "" || Find(pTag->GetName()) != NULL ) return FALSE;

	if( pTag->GetName() == "__ROOT" ) m_ptrRootTag = pTag;
	my_map::value_type registerTri( pTag->GetName(), ptr_tag(pTag) );
	//등록 성공 or 실패 정보를 리턴
	return m_mapRegistered.insert( registerTri ).second;
}

//안태훈 수정 시작	//(For Performance)(0.2)
BOOL CTagManager::Unregister(const char *name)
{
	if( std::string(name) == "__ROOT" ) m_ptrRootTag.SetNull();
	return Unregister(std::string(name));
}

BOOL CTagManager::Unregister(std::string &name)
//안태훈 수정 끝	//(For Performance)(0.2)
{
	my_map::iterator iter = m_mapRegistered.find(name);
	if(iter == m_mapRegistered.end()) return FALSE;
	if( name == "__ROOT" ) m_ptrRootTag.SetNull();
	m_mapRegistered.erase(iter);
	return TRUE;
}

//안태훈 수정 시작	//(For Performance)(0.2)
BOOL CTagManager::IsRegister(const char *name)
{
	return IsRegister(std::string(name));
}

BOOL CTagManager::IsRegister(std::string &name)
//안태훈 수정 끝	//(For Performance)(0.2)
{
	my_map::iterator iter = m_mapRegistered.find(name);
	if(iter == m_mapRegistered.end()) return FALSE;
	return TRUE;
}

//안태훈 수정 시작	//(For Performance)(0.2)
CTag *CTagManager::Find(const char *name, BOOL bFindHierarchy)
{
	return Find(std::string(name), bFindHierarchy);
}

CTag *CTagManager::Find(std::string &name, BOOL bFindHierarchy)
{
	if (!m_mapRegistered.empty())
	{
		my_map::iterator iter = m_mapRegistered.find(name);
		if(iter != m_mapRegistered.end()) return (*iter).second.GetNative();
	}

	if (bFindHierarchy == FALSE)
	{
		return NULL;
	}
	else if(m_vectorChild.empty() == false)
	{
		CTag *pRet = NULL;
		for(INDEX i=0; i < m_vectorChild.size() && pRet == NULL; ++i)
		{
			pRet = m_vectorChild[i]->Find(name, TRUE);

			if (pRet != NULL)
				return pRet;
		}
	}

	return NULL;
}

void CTagManager::Clear()
{
	m_mapRegistered.clear();
	m_ptrRootTag.SetNull();
}

BOOL CTagManager::HasActiveTag()
{
	my_map::iterator iter;
	my_map::iterator iterBegin = m_mapRegistered.begin();
	my_map::iterator iterEnd = m_mapRegistered.end();
	for(iter = iterBegin; iter != iterEnd; ++iter)
	{
		if((*iter).second->Active()) return TRUE;
	}
	return FALSE;
}


//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)