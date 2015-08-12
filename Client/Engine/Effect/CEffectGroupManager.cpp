//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)

#include "stdH.h"
#include "CEffectGroupManager.h"

#pragma warning(disable : 4786)
#include <Engine/Base/Timer.h>
#include <algorithm>
#pragma warning(disable : 4786)
#include <Engine/Base/Memory.h>

extern CTimer *_pTimer;

CEffectGroupManager CEffectGroupManager::m_instance;

CEffectGroupManager::CEffectGroupManager()
{
}

CEffectGroupManager::~CEffectGroupManager()
{
	Clear();
}

void CEffectGroupManager::Clear()
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

void CEffectGroupManager::ClearCreated()
{
	if(!m_listCreated.empty())
	{
		my_list::iterator iterBegin = m_listCreated.begin();
		my_list::iterator iterEnd = m_listCreated.end();
		my_list::iterator iter;
		for(iter = iterBegin; iter != iterEnd; ++iter)
		{
			delete (*iter);
		}
		m_listCreated.clear();
	}
}

BOOL CEffectGroupManager::Register(CEffectGroup *protoCEffectGroup)
{
	if(protoCEffectGroup == NULL) return FALSE;
	if( protoCEffectGroup->GetName() == "" || IsRegistered(protoCEffectGroup->GetName()) ) return FALSE;

	my_map::value_type registerTri( protoCEffectGroup->GetName(), protoCEffectGroup );
	//등록 성공 or 실패 정보를 리턴
	return m_mapRegistered.insert( registerTri ).second;
}

BOOL CEffectGroupManager::Unregister(const std::string name)
{
	my_map::iterator iter = m_mapRegistered.find(name);
	if(iter == m_mapRegistered.end()) return FALSE;
	delete (*iter).second;
	m_mapRegistered.erase(iter);
	return TRUE;
}

BOOL CEffectGroupManager::IsRegistered(const std::string name)
{
	my_map::iterator iter = m_mapRegistered.find(name);
	return iter != m_mapRegistered.end();
}

CEffectGroup *CEffectGroupManager::Create(const std::string name)
{
	my_map::iterator iter = m_mapRegistered.find(name);
	if(iter == m_mapRegistered.end()) return NULL;
	CEffectGroup *temp = (*iter).second->Copy();
	if(temp == NULL) return NULL;
	m_listCreated.push_back(temp);
	return temp;
}

void CEffectGroupManager::Destroy(CEffectGroup *&obj)
{
	if(obj == NULL) return;
	
	my_list::iterator tmpItr;
	
	tmpItr = std::find(m_listCreated.begin(), m_listCreated.end(), obj);

	if (tmpItr != m_listCreated.end())
	{
		m_listCreated.remove(obj);
		delete obj;
		obj = NULL;
	}
}

bool CEffectGroupManager::IsValidCreated(CEffectGroup *obj)
{
	if(obj == NULL) return false;
	my_list::iterator iter = std::find(m_listCreated.begin(), m_listCreated.end(), obj);
	if(iter != m_listCreated.end()) return true;

	return false;
}

void CEffectGroupManager::StopAll()
{
	//ClearCreated();
	for(my_list::iterator iter = m_listCreated.begin(); iter != m_listCreated.end(); ++iter)
	{
		(*iter)->Stop(0);
	}
}

//안태훈 수정 시작	//(Remake Effect)(0.1)
#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 1

void CEffectGroupManager::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;

	is.ExpectID_t("EGMG");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		DWORD dwSize;
		is >> dwSize;
		for(DWORD i=0; i<dwSize; ++i)
		{
			CEffectGroup* eg = new CEffectGroup;
			eg->Read(&is);
			if (!Register(eg))
			{
				if (eg) delete eg;
			}
		}
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CEffectGroupManager::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;

	os.WriteID_t("EGMG");
	os << (UBYTE)CURRENT_VERSION;

	os << (DWORD)m_mapRegistered.size();
	for(my_map::iterator iter=m_mapRegistered.begin(); iter!=m_mapRegistered.end(); ++iter)
	{
		(*iter).second->Write(&os);
	}
}

// WSS_DRATAN_SIEGEWARFARE 2007/09/17
CEffectGroup*	CEffectGroupManager::GetEffectGroup( const std::string name , CEntity* pEntity)
{
	CEntity* pRegEntity;
	my_list::iterator	EGstart,EGend;
	EGstart = m_listCreated.begin();
	EGend	= m_listCreated.end();
	
	for( ;EGstart != EGend; ++EGstart )
	{
		if ((*EGstart)->GetName() == name )
		{			
			CTagManager * tTagMgr = (*EGstart)->GetTagManager();
			if ( tTagMgr )
			{
				pRegEntity = tTagMgr->GetOwner();
				if( pRegEntity )
				{
					if( pRegEntity == pEntity)
						return (*EGstart);
				}				
			}
		}
	}		
	return NULL;
	
}
