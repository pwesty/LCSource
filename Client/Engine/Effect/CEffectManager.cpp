//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#include "stdH.h"

#pragma warning(disable : 4786)
#include <Engine/Base/Timer.h>
#include <algorithm>
#pragma warning(disable : 4786)
#include <Engine/Base/Memory.h>

#include "CEffectManager.h"

extern CTimer *_pTimer;

CEffectManager CEffectManager::m_instance;

CEffectManager::CEffectManager()
{
}

CEffectManager::~CEffectManager()
{
	Clear();
}

void CEffectManager::Clear()
{
	ClearCreated();

	if(!m_mapRegistered.empty())
	{
		my_map::iterator iterBegin = m_mapRegistered.begin();
		my_map::iterator iterEnd = m_mapRegistered.end();
		my_map::iterator iter;
		for(iter = iterBegin; iter != iterEnd; ++iter)
		{
			delete (*iter).second;
		}
		m_mapRegistered.clear();
	}
}

void CEffectManager::ClearCreated()
{
	for(INDEX i=0; i<ET_COUNT; ++i)
	{
		if(!m_listCreated[i].empty())
		{
			my_list::iterator iterBegin = m_listCreated[i].begin();
			my_list::iterator iterEnd = m_listCreated[i].end();
			my_list::iterator iter;
			for(iter = iterBegin; iter != iterEnd; ++iter)
			{
				delete (*iter);
			}
			m_listCreated[i].clear();
		}
	}
}

BOOL CEffectManager::Register(CEffect *prototype)
{
	if(prototype == NULL) return FALSE;
	if( prototype->GetName() == "" || IsRegistered(prototype->GetName()) ) return FALSE;

	my_map::value_type registerTri( prototype->GetName(), prototype );
	//등록 성공 or 실패 정보를 리턴
	return m_mapRegistered.insert( registerTri ).second;
}

BOOL CEffectManager::Unregister(const std::string name)
{
	my_map::iterator iter = m_mapRegistered.find(name);
	if(iter == m_mapRegistered.end()) return FALSE;
	delete (*iter).second;
	m_mapRegistered.erase(iter);
	return TRUE;
}

BOOL CEffectManager::IsRegistered(const std::string name)
{
	my_map::iterator iter = m_mapRegistered.find(name);
	return iter != m_mapRegistered.end();
}

CEffect *CEffectManager::Create(const std::string name)
{
	my_map::iterator iter = m_mapRegistered.find(name);
	if(iter == m_mapRegistered.end()) return NULL;
	CEffect *temp = (*iter).second->Copy();
	if(temp == NULL) return NULL;
	ASSERT(temp->GetType() >= 0 && temp->GetType() < ET_COUNT);
	m_listCreated[temp->GetType()].push_back(temp);
	return temp;
}

bool CEffectManager::IsValidCreated(CEffect *obj)
{
	if(obj == NULL) return false;
	for(INDEX i=0; i<ET_COUNT; ++i)
	{
		my_list::iterator iter = std::find(m_listCreated[i].begin(), m_listCreated[i].end(), obj);
		if(iter != m_listCreated[i].end()) return true;
	}

	return false;
}


void CEffectManager::Destroy(CEffect *&obj)
{
	if(obj == NULL) return;
	ASSERT(obj->GetType() >= 0 && obj->GetType() < ET_COUNT);
	if(obj->GetType() < 0 || obj->GetType() >= ET_COUNT) return;
	
	my_list::iterator tmpItr;

	tmpItr = std::find(m_listCreated[obj->GetType()].begin(), m_listCreated[obj->GetType()].end(), obj);

	if (tmpItr != m_listCreated[obj->GetType()].end())
	{
		m_listCreated[obj->GetType()].remove(obj);
		delete obj;
	}

	obj = NULL;
}
#include <Engine/Effect/EffectCommon.h>
#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 1

void CEffectManager::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;

	is.ExpectID_t("EFMG");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		DWORD dwSize;
		is >> dwSize;
		for(DWORD i=0; i<dwSize; ++i)
		{
			DWORD dwType;
			is >> dwType;
			CEffect *pEffect = CreateFromType((EFFECT_TYPE)dwType);
			ASSERT(pEffect);
			pEffect->Read(&is);
			if (!Register(pEffect))
			{
				if (pEffect) delete pEffect;
			}
		}
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CEffectManager::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;

	os.WriteID_t("EFMG");
	os << (UBYTE)CURRENT_VERSION;

	os << (DWORD)m_mapRegistered.size();
	for(my_map::iterator iter=m_mapRegistered.begin(); iter!=m_mapRegistered.end(); ++iter)
	{
		os << (DWORD)(*iter).second->GetType();
		(*iter).second->Write(&os);
	}
}
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
