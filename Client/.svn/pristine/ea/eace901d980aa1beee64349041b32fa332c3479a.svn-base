//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)

#include "stdH.h"
#include "CParticleGroupManager.h"

#pragma warning(disable : 4786)
#include <Engine/Base/Timer.h>
#include <algorithm>
#pragma warning(disable : 4786)
#include <Engine/Base/Memory.h>

extern CTimer *_pTimer;

CParticleGroupManager CParticleGroupManager::m_instance;

CParticleGroupManager::CParticleGroupManager()
{
	m_vectorCreated.reserve(1024);
	//memset(&m_vectorCreated[0], 0, 1024*sizeof(CParticleGroup*));
}

CParticleGroupManager::~CParticleGroupManager()
{
	Clear();
}

void CParticleGroupManager::Clear()
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

void CParticleGroupManager::ClearCreated()
{
	if(!m_vectorCreated.empty())
	{
		for(INDEX i=0; i<m_vectorCreated.size(); ++i)
		{
			delete m_vectorCreated[i];
		}
		m_vectorCreated.clear();
	}
}

BOOL CParticleGroupManager::Register(CParticleGroup *prototype)
{
	if(prototype == NULL) return FALSE;
	if( prototype->GetName() == "" || IsRegistered(prototype->GetName()) ) return FALSE;

	my_map::value_type registerTri( prototype->GetName(), prototype );
	//등록 성공 or 실패 정보를 리턴
	return m_mapRegistered.insert( registerTri ).second;
}

BOOL CParticleGroupManager::Unregister(const std::string name)
{
	my_map::iterator iter = m_mapRegistered.find(name);
	if(iter == m_mapRegistered.end()) return FALSE;
	delete (*iter).second;
	m_mapRegistered.erase(iter);
	return TRUE;
}

BOOL CParticleGroupManager::IsRegistered(const std::string name)
{
	my_map::iterator iter = m_mapRegistered.find(name);
	return iter != m_mapRegistered.end();
}

CParticleGroup *CParticleGroupManager::Create(const std::string name)
{
	my_map::iterator iter = m_mapRegistered.find(name);
	if(iter == m_mapRegistered.end()) return NULL;
	CParticleGroup *temp = (*iter).second->Copy();
	m_vectorCreated.push_back(temp);
	return temp;
}

void CParticleGroupManager::Destroy(CParticleGroup *&obj)
{
	if(obj == NULL) return;

	my_vector::iterator tmpItr;

	tmpItr = std::find(m_vectorCreated.begin(), m_vectorCreated.end(), obj);

	if (tmpItr != m_vectorCreated.end())
	{
		m_vectorCreated.erase(std::remove(m_vectorCreated.begin(), m_vectorCreated.end(), obj));
		delete obj;
	}

	obj = NULL;
}

//안태훈 수정 시작	//(Remake Effect)(0.1)
#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 1

void CParticleGroupManager::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;

	is.ExpectID_t("PGMG");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		DWORD dwSize;
		is >> dwSize;
		for(DWORD i=0; i<dwSize; ++i)
		{
			CParticleGroup* pg = new CParticleGroup;
			pg->Read(&is);
			if (!Register(pg))
			{
				if (pg) delete pg;
			}
		}
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CParticleGroupManager::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;

	os.WriteID_t("PGMG");
	os << (UBYTE)CURRENT_VERSION;

	os << (DWORD)m_mapRegistered.size();
	for(my_map::iterator iter=m_mapRegistered.begin(); iter!=m_mapRegistered.end(); ++iter)
	{
		(*iter).second->Write(&os);
	}
}
//안태훈 수정 끝	//(Remake Effect)(0.1)

//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
