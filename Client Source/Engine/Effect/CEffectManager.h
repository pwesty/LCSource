//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#ifndef __CEFFECTMANAGER_H__
#define __CEFFECTMANAGER_H__

#include <Engine/Base/Memory.h>
#pragma warning(disable : 4786)
#include <list>
#pragma warning(disable : 4786)
#include <map>
#pragma warning(disable : 4786)
#include <string>
#pragma warning(disable : 4786)
#include "CEffect.h"

class ENGINE_API CEffectManager
{
public:
	typedef std::list<CEffect*>				my_list;
	typedef std::map<std::string, CEffect*>	my_map;

public:
	~CEffectManager();
	
//안태훈 수정 시작	//(Remake Effect)(0.1)
	void Read(CTStream *istrFile);
	void Write(CTStream *ostrFile);
//안태훈 수정 끝	//(Remake Effect)(0.1)
	
	static inline CEffectManager &Instance() { return m_instance; }
	BOOL Register(CEffect *prototype);
	BOOL Unregister(const std::string name);
	BOOL IsRegistered(const std::string name);
	void Clear();
	void ClearCreated();
	bool IsValidCreated(CEffect *obj);

	CEffect *Create(const std::string name);
	void Destroy(CEffect *&obj);

	my_list &GetCreatedList(INDEX i)	{ ASSERT(i >= 0 && i < ET_COUNT); return m_listCreated[i]; }
	my_map &GetRegisteredMap()			{ return m_mapRegistered;	}

private:
	CEffectManager();

protected:
	static CEffectManager m_instance;
	my_list		m_listCreated[ET_COUNT];
	my_map		m_mapRegistered;
};

#endif //__CEFFECTMANAGER_H__
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)