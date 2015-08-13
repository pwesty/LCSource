#include "StdH.h"
#include "StageMgr.h"
#include <Engine/GameState.h>
#include <Engine/GameStageManager/StageIntro.h>
#include <Engine/GameStageManager/StageLogin.h>
#include <Engine/GameStageManager/StageCreateChar.h>
#include <Engine/GameStageManager/StageSelServer.h>
#include <Engine/GameStageManager/StageSelChar.h>
#include <Engine/GameStageManager/StageLoading.h>
#include <Engine/GameStageManager/StageGameEnd.h>
#include <Engine/GameStageManager/StageGamePlay.h>

StageMgr::StageMgr()
	:	m_pCurStage(NULL)
	,	m_e2ndNextStage(eSTAGE_NONE)
	,	m_eNextStage(eSTAGE_NONE)
	,	m_eCurStage(eSTAGE_NONE)
{
	m_vecStage.clear();
}

StageMgr::~StageMgr()
{
	DestroyAll();
}

void StageMgr::DestroyAll()
{
	std::vector<CStage*>::iterator iter = m_vecStage.begin();
	std::vector<CStage*>::iterator iterEnd = m_vecStage.end();

	for( iter; iter != iterEnd; ++iter )
	{
		SAFE_DELETE(*iter);
	}

	m_vecStage.clear();
}

void StageMgr::Create()
{	

#define DEF_CREATE_STAGE(class_type) { \
		class_type* pStage = new class_type; \
		m_vecStage.push_back(pStage); }

	//eSTAGE_INTRO
	DEF_CREATE_STAGE(CStageIntro);
	DEF_CREATE_STAGE(CStageLogin);
	DEF_CREATE_STAGE(CStageSelServer);
	DEF_CREATE_STAGE(CStageSelChar);
	DEF_CREATE_STAGE(CStageCreateChar);
	DEF_CREATE_STAGE(CStageLoading);
	DEF_CREATE_STAGE(CStageGamePlay);
	DEF_CREATE_STAGE(CStageGameEnd);
}

void StageMgr::GoStage()
{
	if (m_eNextStage <= eSTAGE_NONE)
		return;

	if (m_pCurStage != NULL)
		m_pCurStage->Release();
	
	m_eCurStage	= m_eNextStage;
	m_pCurStage = m_vecStage[m_eCurStage];
	m_pCurStage->Init();	
	
	m_eNextStage = m_e2ndNextStage;
	m_e2ndNextStage = eSTAGE_NONE;
}

void StageMgr::SetNextStage( eSTAGE e1stStage, eSTAGE e2ndStage )
{
	if (m_eCurStage == e1stStage )
		return;

	if (IsCorrectStage(e1stStage) == TRUE)
		m_eNextStage = e1stStage;

	if (IsCorrectStage(e2ndStage) == TRUE)
		m_e2ndNextStage = e2ndStage;	
}

BOOL StageMgr::IsCorrectStage( eSTAGE eStage )
{
	if (eStage <= eSTAGE_NONE || eStage >= eSTAGE_MAX)
	{
		LOG_DEBUG("eStage has no value");
		return FALSE;
	}

	if (eStage >= m_vecStage.size())
	{
		LOG_DEBUG("Stage value is too large.");
		return FALSE;
	}

	if (m_vecStage[eStage] == NULL)
	{
		LOG_DEBUG("%d Stage has no value", eStage);
		return FALSE;
	}

	return TRUE;
}

void StageMgr::Run()
{	
	if (m_eNextStage != eSTAGE_NONE)
		GoStage();

	if (m_pCurStage != NULL)
		m_pCurStage->Run();
}

void StageMgr::EndCharData()
{
	if (m_eCurStage == eSTAGE_SELSERVER)
	{
		if (_pGameState->GetExistCharNum() == 0)
			SetNextStage(eSTAGE_CREATECHAR);// 캐릭터가 없다면 생성 창으로
		else
			SetNextStage(eSTAGE_SELCHAR);
	}
	else if (m_eCurStage == eSTAGE_CREATECHAR)
	{
		SetNextStage(eSTAGE_SELCHAR);
	}
}
