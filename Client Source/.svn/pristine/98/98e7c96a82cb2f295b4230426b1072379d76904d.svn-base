//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)

#ifndef __CPARTICLEGROUPMANAGER_H__
#define __CPARTICLEGROUPMANAGER_H__

#include <Engine/Base/Memory.h>
#pragma warning(disable : 4786)
#include <list>
#pragma warning(disable : 4786)
#include <map>
#pragma warning(disable : 4786)
#include <string>
#pragma warning(disable : 4786)

#include "CParticleGroup.h"

class ENGINE_API CParticleGroupManager
{
public:
	typedef std::vector<CParticleGroup*> my_vector;
	typedef std::map<std::string, CParticleGroup*> my_map;

public:
	~CParticleGroupManager();
	
	static inline CParticleGroupManager &Instance() { return m_instance; }
	BOOL Register(CParticleGroup *prototype);
	BOOL Unregister(const std::string name);
	BOOL IsRegistered(const std::string name);
	void Clear();
	void ClearCreated();

	CParticleGroup *Create(const std::string name);
	void Destroy(CParticleGroup *&obj);

//안태훈 수정 시작	//(Remake Effect)(0.1)
	void Read(CTStream *istrFile);
	void Write(CTStream *ostrFile);
//안태훈 수정 끝	//(Remake Effect)(0.1)

	my_vector &GetCreatedList()	{ return m_vectorCreated; }
	my_map &GetRegisteredMap()	{ return m_mapRegistered;	}

private:
	CParticleGroupManager();

protected:
	static CParticleGroupManager m_instance;
	my_vector	m_vectorCreated;
	my_map		m_mapRegistered;
};

#endif //__CPARTICLEGROUPMANAGER_H__

//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
