#include "StdH.h"
#include "MyInfo.h"
#include <Engine/Interface/UIManager.h>
#include <Engine/Contents/function/TargetInfoNewUI.h>

MyInfo::MyInfo()
	: nBuy_addition(0)
	, nBuy_rate(0)
	, nSell_addition(0)
	, nSell_rate(0)
{

}

void MyInfo::clear()
{
	MyInfoSkill::clear();
	nBuy_addition = 0;
	nBuy_rate = 0;
	nSell_addition = 0;
	nSell_rate = 0;
}

__int64 MyInfo::CalcPriceRate( __int64 iShopPrice, BOOL bBuy )
{
	if (iShopPrice <= 0)
		return 0;

	float fRate = 0.f;

	if (bBuy == TRUE)
		fRate = nBuy_rate;
	else
		fRate = nSell_rate;

	if (fRate <= 0)
		return 0;

	__int64 iAddPrice = 0;
	double temp = iShopPrice / 100.f;
	iAddPrice = temp * (fRate - 100.f);

	return iAddPrice;
}

__int64 MyInfo::CalcPriceAddition( BOOL bBuy )
{
	int nAddition = 0;

	if (bBuy == TRUE)
		nAddition = nBuy_addition;
	else
		nAddition = nSell_addition;

	return nAddition;
}

ObjInfo::ObjInfo()
	: m_MyApetInfo(NULL)
{
}

void ObjInfo::clear()
{
	TargetClear();
	m_MyApetInfo = NULL;
	m_MyPetInfo.Init();
	m_MySlaveInfo[0].Init();
	m_MySlaveInfo[1].Init();
}

void ObjInfo::SetTargetInfo( CTargetInfom Info )
{
	m_TargetInfo.bIsActive		= Info.bIsActive;
	m_TargetInfo.TargetType		= Info.TargetType;
	m_TargetInfo.pen_pEntity	= Info.pen_pEntity;
	m_TargetInfo.fMaxHealth		= Info.fMaxHealth;
	m_TargetInfo.fHealth		= Info.fHealth;
	m_TargetInfo.iLevel			= Info.iLevel;
	m_TargetInfo.PkMode			= Info.PkMode;
	m_TargetInfo.PkState		= Info.PkState;
	m_TargetInfo.PkTitle		= Info.PkTitle;
	m_TargetInfo.Legit			= Info.Legit;
	m_TargetInfo.dbIdx			= Info.dbIdx;
	m_TargetInfo.bPkHideState	= Info.bPkHideState;
	m_TargetInfo.nSyndicateType	= Info.nSyndicateType;	
	m_TargetInfo.nSyndicateGrade	= Info.nSyndicateGrade;
	strcpy(m_TargetInfo.TargetName, Info.TargetName);

	CUIManager* pUIMgr = CUIManager::getSingleton();

	if (pUIMgr->GetTargetInfoUI() != NULL)
	{
		pUIMgr->GetTargetInfoUI()->OpenUI();
	}
}

void ObjInfo::SetTargetRealInfo( CTargetInfomReal Info )
{
	m_TargetInfoReal.bIsActive		= Info.bIsActive;
	m_TargetInfoReal.TargetType		= Info.TargetType;
	m_TargetInfoReal.pen_pEntity	= Info.pen_pEntity;
	m_TargetInfoReal.fMaxHealth		= Info.fMaxHealth;
	m_TargetInfoReal.fHealth		= Info.fHealth;
	m_TargetInfoReal.iLevel			= Info.iLevel;
	m_TargetInfoReal.PkMode			= Info.PkMode;
	m_TargetInfoReal.PkState		= Info.PkState;
	m_TargetInfoReal.PkTitle		= Info.PkTitle;
	m_TargetInfoReal.Legit			= Info.Legit;
	m_TargetInfoReal.dbIdx			= Info.dbIdx;
	strcpy(m_TargetInfoReal.TargetName, Info.TargetName);

	m_TargetInfoReal.llCount			= Info.llCount;
	m_TargetInfoReal.lGuildIndex		= Info.lGuildIndex;
	m_TargetInfoReal.sbGuildRank		= Info.sbGuildRank;
	m_TargetInfoReal.ubGuildNameColor	= Info.ubGuildNameColor;
	m_TargetInfoReal.iNickIndex			= Info.iNickIndex;
	m_TargetInfoReal.strGuildName		= Info.strGuildName;
}

void ObjInfo::SetTargetHealth( float fCur, float fMax )
{
	CTargetInfoUI* pTargetUI = UIMGR()->GetTargetInfoUI();

	m_TargetInfo.fHealth = fCur;
	m_TargetInfo.fMaxHealth = fMax;

	if (pTargetUI != NULL && pTargetUI->GetHide() == FALSE)
		pTargetUI->UpdateHPInfo();
}

void ObjInfo::SetTargetPKMode( int pkMode, int pkState, int Legit, int pkTitle)
{
	CTargetInfoUI* pTargetUI = UIMGR()->GetTargetInfoUI();

	m_TargetInfo.PkMode = pkMode;
	m_TargetInfo.PkTitle = pkTitle;
	m_TargetInfo.PkState = pkState;
	m_TargetInfo.Legit = Legit;

	if (pTargetUI != NULL && pTargetUI->GetHide() == FALSE)
		pTargetUI->UpdateTargetInfo();
}

void ObjInfo::SetTargetPKHide( BOOL bPkState )
{
	m_TargetInfo.bPkHideState = bPkState;
}

void ObjInfo::SetTargetSyndicate( int nType, int nGrade )
{
	CTargetInfoUI* pTargetUI = UIMGR()->GetTargetInfoUI();

	m_TargetInfo.nSyndicateType = nType;
	m_TargetInfo.nSyndicateGrade = nGrade;

	if (pTargetUI != NULL && pTargetUI->GetHide() == FALSE)
		pTargetUI->UpdateSyndicateIcon();
}

CEntity* ObjInfo::GetTargetEntity(eTARGET_TYPE eType)
{
	CEntity* pEntity = NULL;

	switch(eType)
	{
	case eTARGET:
		pEntity = m_TargetInfo.pen_pEntity;
		break;
	case eTARGET_REAL:
		pEntity = m_TargetInfoReal.pen_pEntity;
		break;
	}
	return pEntity;
}

int ObjInfo::GetTargetDBIdx( eTARGET_TYPE eType )
{
	int nDbIdx = 0;

	switch(eType)
	{
	case eTARGET:
		nDbIdx = m_TargetInfo.dbIdx;
		break;
	case eTARGET_REAL:
		nDbIdx = m_TargetInfoReal.dbIdx;
		break;
	}
	return nDbIdx;
}

SBYTE ObjInfo::GetTargetType( eTARGET_TYPE eType )
{
	SBYTE sbType = 0;

	switch(eType)
	{
	case eTARGET:
		sbType = m_TargetInfo.TargetType;
		break;
	case eTARGET_REAL:
		sbType = m_TargetInfoReal.TargetType;
		break;
	}
	return sbType;
}

char* ObjInfo::GetTargetName( eTARGET_TYPE eType )
{
	switch(eType)
	{
	case eTARGET:
		return m_TargetInfo.TargetName;
	case eTARGET_REAL:
		return m_TargetInfoReal.TargetName;
	}
	return NULL;
}

int ObjInfo::GetTargetLevel( eTARGET_TYPE eType )
{
	int nLevel = 0;

	switch(eType)
	{
	case eTARGET:
		nLevel = m_TargetInfo.iLevel;
		break;
	case eTARGET_REAL:
		nLevel = m_TargetInfoReal.iLevel;
		break;
	}
	return nLevel;
}

float ObjInfo::GetTargetCurHP( eTARGET_TYPE eType )
{
	float fcurHP = 0.f;

	switch(eType)
	{
	case eTARGET:
		fcurHP = m_TargetInfo.fHealth;
		break;
	case eTARGET_REAL:
		fcurHP = m_TargetInfoReal.fHealth;
		break;
	}
	return fcurHP;
}

float ObjInfo::GetTargetMaxHP( eTARGET_TYPE eType )
{
	float fmaxHP = 0.f;

	switch(eType)
	{
	case eTARGET:
		fmaxHP = m_TargetInfo.fMaxHealth;
		break;
	case eTARGET_REAL:
		fmaxHP = m_TargetInfoReal.fMaxHealth;
		break;
	}
	return fmaxHP;
}

void ObjInfo::TargetClear( eTARGET_TYPE eType )
{
	switch(eType)
	{
	case eTARGET:
		m_TargetInfo.Init();
		break;
	case eTARGET_REAL:
		m_TargetInfoReal.Init();
		break;
	case eTARGET_ALL:
		m_TargetInfo.Init();
		m_TargetInfoReal.Init();
		break;
	}	
}

int ObjInfo::GetTargetServerIdx( eTARGET_TYPE eType )
{
	CEntity* pEntity = NULL;

	switch(eType)
	{
	case eTARGET:
		pEntity = m_TargetInfo.pen_pEntity;
		break;
	case eTARGET_REAL:
		pEntity = m_TargetInfoReal.pen_pEntity;
		break;
	}

	if(pEntity == NULL)
		return -1;

	return pEntity->GetNetworkID();
}

BOOL ObjInfo::IsTargetActive( eTARGET_TYPE eType )
{
	BOOL bActive = FALSE;

	switch(eType)
	{
	case eTARGET:
		bActive = m_TargetInfo.bIsActive;
		break;
	case eTARGET_REAL:
		bActive = m_TargetInfoReal.bIsActive;
		break;
	}
	return bActive; 
}

int ObjInfo::GetTargetPKState( eTARGET_TYPE eType )
{
	int state = 0;

	switch(eType)
	{
	case eTARGET:
		state = m_TargetInfo.PkState;;
		break;
	case eTARGET_REAL:
		state = m_TargetInfoReal.PkState;
		break;
	}
	return state;
}

int ObjInfo::GetTargetPKMode( eTARGET_TYPE eType )
{
	int pkMode = 0;

	switch(eType)
	{
	case eTARGET:
		pkMode = m_TargetInfo.PkMode;
		break;
	case eTARGET_REAL:
		pkMode = m_TargetInfoReal.PkMode;
		break;
	}
	return pkMode;
}

void ObjInfo::SetTargetActive( eTARGET_TYPE eType, BOOL bActive)
{
	switch(eType)
	{
	case eTARGET:
		m_TargetInfo.bIsActive = bActive;
		break;
	case eTARGET_REAL:
		m_TargetInfoReal.bIsActive = bActive;
		break;
	}
}

int ObjInfo::GetShowAnotherRelicEffect()
{
	int nShowEffect = -1;
	int nTemp = -1;

	for (int i = WEAR_ACCESSORY1; i < WEAR_ACCESSORY4; ++i)
	{
		if (_pNetwork->MyWearItem[i].IsEmptyItem() == TRUE)
			continue;

		if (_pNetwork->MyWearItem[i].Item_Index == 10953)
			nTemp = 2;
		else if (_pNetwork->MyWearItem[i].Item_Index == 10952)
			nTemp = 1;
		else if (_pNetwork->MyWearItem[i].Item_Index == 10951)
			nTemp = 0;

		if (nTemp > nShowEffect)
			nShowEffect = nTemp;
	}

	return nShowEffect;
}