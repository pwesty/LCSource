//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)

#pragma warning(disable : 4786)
#include <Engine/Base/Timer.h>
#include <algorithm>
#pragma warning(disable : 4786)
#include <Engine/Base/Memory.h>

extern CTimer *_pTimer;

CTYPEManager CTYPEManager::m_instance;

struct Delete
{
	inline void operator()(TYPE *p) { if (p) delete p; p = NULL; }
};
struct DeleteSecond
{
	inline void operator()(CTYPEManager::my_map::value_type &p) { if (p.second) delete p.second; p.second = NULL; }
};

CTYPEManager::CTYPEManager()
{
}

CTYPEManager::~CTYPEManager()
{
	Clear();
}

void CTYPEManager::Clear()
{
	Delete delObj;
	DeleteSecond delObj2;

	std::for_each(m_mapRegistered.begin(), m_mapRegistered.end(), delObj2);
	m_mapRegistered.clear();

	std::for_each(m_listCreated.begin(), m_listCreated.end(), delObj);
	m_listCreated.clear();
}

void CTYPEManager::ClearCreated()
{
	Delete delObj;
	std::for_each(m_listCreated.begin(), m_listCreated.end(), delObj);
	m_listCreated.clear();
}

BOOL CTYPEManager::Register(TYPE *prototype)
{
	if(prototype == NULL) return FALSE;
	if( prototype->GetName() == "" || IsRegistered(prototype->GetName()) ) return FALSE;

	my_map::value_type registerTri( prototype->GetName(), prototype->Copy() );
	//등록 성공 or 실패 정보를 리턴
	return m_mapRegistered.insert( registerTri ).second;
}

BOOL CTYPEManager::Unregister(const std::string name)
{
	my_map::iterator iter = m_mapRegistered.find(name);
	if(iter == m_mapRegistered.end()) return FALSE;
	delete (*iter).second;
	m_mapRegistered.erase(iter);
	return TRUE;
}

BOOL CTYPEManager::IsRegistered(const std::string name)
{
	my_map::iterator iter = m_mapRegistered.find(name);
	return iter != m_mapRegistered.end();
}

TYPE *CTYPEManager::Create(const std::string name)
{
	my_map::iterator iter = m_mapRegistered.find(name);
	if(iter == m_mapRegistered.end()) return NULL;
	TYPE *temp = (*iter).second->Copy();
	m_listCreated.push_back(temp);
	return temp;
}

void CTYPEManager::Destroy(TYPE *obj)
{
	if(obj == NULL) return;
	m_listCreated.remove(obj);
	delete obj;
	obj = NULL;
}

//안태훈 수정 시작	//(Remake Effect)(0.1)
#ifndef FINALVERSION
void CTYPEManager::Read(CTStream *istrFile)
{
	ASSERT(istrFile);

	istrFile->ExpectID_t("EGMG");
}

void CTYPEManager::Write(CTStream *ostrFile)
{
	ASSERT(ostrFile);

	ostrFile->WriteID_t("EGMG");
}
#endif //FINALVERSION
//안태훈 수정 끝	//(Remake Effect)(0.1)

//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
