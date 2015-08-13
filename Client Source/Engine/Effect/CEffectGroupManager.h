//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#ifndef __CEFFECTGROUPMANAGER_H__
#define __CEFFECTGROUPMANAGER_H__

#include "CEffectGroup.h"
#include "CWorldTag.h"

#include <Engine/Base/Memory.h>
#pragma warning(disable : 4786)
#include <list>
#pragma warning(disable : 4786)
#include <map>
#pragma warning(disable : 4786)
#include <string>
#pragma warning(disable : 4786)

class ENGINE_API CEffectGroupManager
{
public:
	typedef std::list<CEffectGroup*> my_list;
	typedef std::map<std::string, CEffectGroup*> my_map;

public:
	~CEffectGroupManager();
	
//안태훈 수정 시작	//(Remake Effect)(0.1)
	void Read(CTStream *istrFile);
	void Write(CTStream *ostrFile);
//안태훈 수정 끝	//(Remake Effect)(0.1)

	static inline CEffectGroupManager &Instance() { return m_instance; }
	BOOL Register(CEffectGroup *protoCEffectGroup);
	BOOL Unregister(const std::string name);
	BOOL IsRegistered(const std::string name);
	void Clear();
	void ClearCreated();

	bool IsValidCreated(CEffectGroup *obj);
	CEffectGroup *Create(const std::string name);
	void Destroy(CEffectGroup *&obj);
	void StopAll();

	my_list &GetCreatedList()	{ return m_listCreated; }
	my_map &GetRegisteredMap()	{ return m_mapRegistered;	}

	CEffectGroup	*GetEffectGroup( const std::string name )
	{
		my_map::iterator	it = m_mapRegistered.find( name );
		if( it == m_mapRegistered.end() )
			return NULL;
		return it->second;
	}
	
	// WSS_DRATAN_SIEGEWARFARE 2007/09/17
	CEffectGroup	*GetEffectGroup( const std::string name , CEntity* pEntity);

private:
	CEffectGroupManager();

protected:
	static CEffectGroupManager m_instance;
	my_list		m_listCreated;
	my_map		m_mapRegistered;
};

inline CEffectGroupManager* GetEffectGroupManager(void)
{
	return &CEffectGroupManager::Instance();
}

inline CEffectGroup *CreateEffectGroup(const char *szName)
{
	return CEffectGroupManager::Instance().Create(szName);
}

inline CEffectGroup *StartEffectGroup(const char *szName, CTagManager *pTM, float time, EFF_RENDER_TYPE erType = ER_NORMAL, EFF_RENDER_SUB_TYPE erSubType = ERS_NORMAL)
{
	CEffectGroup *pEG = CreateEffectGroup(szName);
	if(pEG == NULL) return pEG;
	pEG->SetgERType(erType);
	pEG->SetgERSubType(erSubType);
	pEG->SetTagManager(pTM);
	pEG->Start(time);
	return pEG;
}

inline CEffectGroup *StartEffectGroup(const char *szName, float time, const ptr_tag &ptrTag)
{
	CEffectGroup *pEG = CreateEffectGroup(szName);
	if(pEG == NULL) return pEG;
	ptrTag->Activate();
	pEG->Start(time, ptrTag);
	return pEG;
}

inline CEffectGroup *StartEffectGroup(const char *szName, float time, const FLOAT3D &pos, const FLOATquat3D &rot)
{
	ptr_tag ptrTag(new CWorldTag);
	ptrTag->Activate();
	ptrTag->SetOffsetPos(pos);
	ptrTag->SetOffsetRot(rot);
	((CWorldTag*)ptrTag.GetNative())->AddNewTagInfo(0, FLOATquat3D(1,0,0,0), FLOAT3D(0,0,0), FLOAT3D(1,1,1));
	return StartEffectGroup(szName, time, ptrTag);
}

inline CEffectGroup *StartEffectGroup(const char *szName, float time, const FLOAT3D &pos, const ANGLE3D &rot)
{
	ptr_tag ptrTag(new CWorldTag);
	ptrTag->Activate();
	ptrTag->SetOffsetPos(pos);
	ptrTag->SetOffsetRot(rot);
	((CWorldTag*)ptrTag.GetNative())->AddNewTagInfo(0, FLOATquat3D(1,0,0,0), FLOAT3D(0,0,0), FLOAT3D(1,1,1));
	return StartEffectGroup(szName, time, ptrTag);
}

inline void DestroyEffectGroup( CEffectGroup *&pEG)
{
	if(pEG == NULL) return;
	CEffectGroupManager::Instance().Destroy(pEG);
	pEG = NULL;
}

inline void DestroyEffectGroupIfValid( CEffectGroup *&pEG)
{
	if(pEG == NULL) return;
	if(CEffectGroupManager::Instance().IsValidCreated(pEG))
		CEffectGroupManager::Instance().Destroy(pEG);
	pEG = NULL;
}

inline void StopEffectGroupIfValid(CEffectGroup *&pEG, FLOAT time)
{
	if(pEG == NULL) return;
	if(CEffectGroupManager::Instance().IsValidCreated(pEG))
		pEG->Stop(time);
	pEG = NULL;
}
#endif	//__CEFFECTGROUPMANAGER_H__
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
