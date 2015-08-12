
#ifndef		_STAGE_MANAGER_H_
#define		_STAGE_MANAGER_H_

#include <Engine/DesignClasses/Singleton/SingletonBase.h>
#include <vector>

enum eSTAGE
{
	eSTAGE_NONE = -1,
	eSTAGE_INTRO = 0,
	eSTAGE_LOGIN,
	eSTAGE_SELSERVER,
	eSTAGE_SELCHAR,
	eSTAGE_CREATECHAR,
	eSTAGE_ZONELOADING,
	eSTAGE_GAMEPLAY,
	eSTAGE_GAMEEND,
	eSTAGE_MAX,
};

class CStage
{
public:
	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void Run() {}
};

class ENGINE_API StageMgr : public CSingletonBase< StageMgr >
{
public:
	StageMgr();
	~StageMgr();

	void			Create();
	void			DestroyAll();

	void			GoStage();
	void			Run();

	void			SetNextStage(eSTAGE e1stStage, eSTAGE e2ndStage = eSTAGE_NONE);
	eSTAGE			GetCurStage()	{ return m_eCurStage; }
	eSTAGE			GetNextStage()	{ return m_eNextStage; }

	//-----------------------------------------------------
	void			EndCharData(); // 캐릭터 정보를 모두 받은 후

private:
	BOOL	IsCorrectStage(eSTAGE eStage);

	CStage* m_pCurStage;
	eSTAGE	m_eCurStage;
	eSTAGE	m_eNextStage;
	eSTAGE	m_e2ndNextStage;
	std::vector<CStage*> m_vecStage;
};

#define STAGEMGR() StageMgr::getSingleton()

#endif		// _STAGE_MANAGER_H_