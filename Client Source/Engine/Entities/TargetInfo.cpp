
#include "stdh.h"

#include <Engine/Entities/TargetInfo.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Contents/function/TargetInfoNewUI.h>

CTargetInfom::CTargetInfom(void)
{	
	Init();
}

CTargetInfom::~CTargetInfom(void)
{

}

void CTargetInfom::Init(void)
{
	stTargetBase::Init();

	bPkHideState = FALSE;
	nSyndicateType = 0;
	nSyndicateGrade = 0;

	CUIManager* pUIMgr = CUIManager::getSingleton();
	if (pUIMgr != NULL && pUIMgr->GetTargetInfoUI()->GetHide() == FALSE)
	{
		pUIMgr->GetTargetInfoUI()->CloseUI();
	}
}
/////////////////////////////////////////////////
CTargetInfomReal::CTargetInfomReal(void)
{	
	Init();
}

CTargetInfomReal::~CTargetInfomReal(void)
{

}

void CTargetInfomReal::Init(void)
{
	stTargetBase::Init();

	llCount = 0;
	lGuildIndex		= -1;
	strGuildName	= "";
	sbGuildRank = 0;		// WSS_GUILDMASTER 070517
	ubGuildNameColor =0;	//[071123: Su-won] DRATAN_SIEGE_DUNGEON
	iNickIndex = 0;	
}
