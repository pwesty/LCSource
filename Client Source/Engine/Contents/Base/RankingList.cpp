#include "stdh.h"

#include <Engine/Interface/UIInternalClasses.h>
#include "RankingList.h"
#include <Engine/Interface/UIRankingClassify.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIListBoxMultiList.h>

CRankingList::CRankingList()
{
}

CRankingList::~CRankingList()
{
	ClearRankingPoint();
	ClearRankingGuild();
	ClearRankingPointSearch();
	ClearRankingGuildSearch();
}

VSRankingPointData& CRankingList::GetRankingPointByType(ERANKINGEX_SUB_TYPE eSubType)
{
	switch(eSubType)
	{
	case ERANKINGEX_SUB_TYPE_LEVELMASTER:
		return m_vRankLevelMaster;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_TITAN:
		return m_vRankLevelClass_Titan;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_KNIGHT:
		return m_vRankLevelClass_Knight;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_HEALER:
		return m_vRankLevelClass_Healer;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_MAGE:
		return m_vRankLevelClass_Mage;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_ROGUE:
		return m_vRankLevelClass_Rogue;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_SORCERER:
		return m_vRankLevelClass_Sorcerer;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_NIGHTSHADOW:
		return m_vRankLevelClass_NightShadow;

#ifdef CHAR_EX_ROGUE
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_EX_ROGUE:	// [2012/08/27 : Sora] EX로그 추가
		return m_vRankLevelClass_EXRogue;
#endif
#ifdef CHAR_EX_MAGE
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_EX_MAGE:	// 2013/01/08 jeil EX메이지 추가 
		return m_vRankLevelClass_EXMage;
#endif

	case ERANKINGEX_SUB_TYPE_PK:
		return m_vRankPK;

	case ERANKINGEX_SUB_TYPE_QUESTACUM:
		return m_vRankQuestAcum;

	case ERANKINGEX_SUB_TYPE_QUESTWEEK:
		return m_vRankQuestWeek;
	}

	// default
	return m_vRankLevelMaster;
}

VSRankingGuildData& CRankingList::GetRankingGuildByType(ERANKINGEX_SUB_TYPE eSubType)
{
	switch(eSubType)
	{
	case ERANKINGEX_SUB_TYPE_GUILD:
		return m_vRankGuild;
	}

	return m_vRankGuild;
}

std::map<std::string, VSRankingPointData>& CRankingList::GetRankingPointSearchType(ERANKINGEX_SUB_TYPE eSubType)
{
	switch(eSubType)
	{
	case ERANKINGEX_SUB_TYPE_LEVELMASTER:
		return m_mapRankSearchLevelMaster;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_TITAN:
		return m_mapRankSearchLevelClass_Titan;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_KNIGHT:
		return m_mapRankSearchLevelClass_Knight;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_HEALER:
		return m_mapRankSearchLevelClass_Healer;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_MAGE:
		return m_mapRankSearchLevelClass_Mage;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_ROGUE:
		return m_mapRankSearchLevelClass_Rogue;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_SORCERER:
		return m_mapRankSearchLevelClass_Sorcerer;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_NIGHTSHADOW:
		return m_mapRankSearchLevelClass_NightShadow;

#ifdef CHAR_EX_ROGUE
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_EX_ROGUE:	// [2012/08/27 : Sora] EX로그 추가
		return m_mapRankSearchLevelClass_EXRogue;
#endif
#ifdef CHAR_EX_MAGE
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_EX_MAGE:	// 2013/01/08 jeil EX메이지 추가
		return m_mapRankSearchLevelClass_EXMage;
#endif

	case ERANKINGEX_SUB_TYPE_PK:
		return m_mapRankSearchPK;

	case ERANKINGEX_SUB_TYPE_QUESTACUM:
		return m_mapRankSearchQuestAcum;

	case ERANKINGEX_SUB_TYPE_QUESTWEEK:
		return m_mapRankSearchQuestWeek;
	}

	// default
	return m_mapRankSearchLevelMaster;
}

std::map<std::string, VSRankingGuildData>& CRankingList::GetRankingGuildSearchType(ERANKINGEX_SUB_TYPE eSubType)
{
	switch(eSubType)
	{
	case ERANKINGEX_SUB_TYPE_GUILD:
		return m_mapRankSearchGuild;
	}

	return m_mapRankSearchGuild;
}

void CRankingList::AddRankingPoint(ERANKINGEX_SUB_TYPE eSubType, SRankingPointData oData)
{
	VSRankingPointData&		vData	= GetRankingPointByType(eSubType);

	vData.push_back(oData);
}

void CRankingList::AddRankingGuild(ERANKINGEX_SUB_TYPE eSubType, SRankingGuildData oData)
{
	VSRankingGuildData&		vData	= GetRankingGuildByType(eSubType);

	vData.push_back(oData);
}

void CRankingList::AddRankingPointSearch(ERANKINGEX_SUB_TYPE eSubType, SRankingPointData oData, CTString cstrName)
{
	std::map<std::string, VSRankingPointData>&				mapData		= GetRankingPointSearchType(eSubType);
	std::map<std::string, VSRankingPointData>::iterator		itmapData	= mapData.begin();

	BOOL		bExistData	= FALSE;
	for(; itmapData != mapData.end(); itmapData++)
	{
		if((itmapData->first).c_str() == cstrName)
		{
			bExistData		= TRUE;
			break;
		}
	}

	if(bExistData == TRUE)
	{	// 기존 검색한 기록이 있다
		VSRankingPointData&		vData	= itmapData->second;

		vData.push_back(oData);
	}
	else
	{
		std::string				strTemp	= cstrName.str_String;
		VSRankingPointData		vData;

		vData.push_back(oData);
		mapData.insert(std::map<std::string, VSRankingPointData>::value_type(strTemp, vData));
	}
}

void CRankingList::AddRankingPointGuild(ERANKINGEX_SUB_TYPE eSubType, SRankingGuildData oData, CTString cstrName)
{
}

void CRankingList::ClearRankingPoint()
{
	m_vRankLevelMaster.clear();
	m_vRankLevelClass_Titan.clear();
	m_vRankLevelClass_Knight.clear();
	m_vRankLevelClass_Healer.clear();
	m_vRankLevelClass_Mage.clear();
	m_vRankLevelClass_Rogue.clear();
	m_vRankLevelClass_Sorcerer.clear();
	m_vRankLevelClass_NightShadow.clear();
#ifdef CHAR_EX_ROGUE
	m_vRankLevelClass_EXRogue.clear();	// [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
	m_vRankLevelClass_EXMage.clear();	// 2013/01/08 jeil EX메이지 추가
#endif
	m_vRankPK.clear();
	m_vRankQuestAcum.clear();
	m_vRankQuestWeek.clear();
}

void CRankingList::ClearRankingGuild()
{
	m_vRankGuild.clear();
}

void CRankingList::ClearRankingPointSearch()
{
	m_mapRankSearchLevelMaster.clear();
	m_mapRankSearchLevelClass_Titan.clear();
	m_mapRankSearchLevelClass_Knight.clear();
	m_mapRankSearchLevelClass_Healer.clear();
	m_mapRankSearchLevelClass_Mage.clear();
	m_mapRankSearchLevelClass_Rogue.clear();
	m_mapRankSearchLevelClass_Sorcerer.clear();
	m_mapRankSearchLevelClass_NightShadow.clear();
#ifdef CHAR_EX_ROGUE
	m_mapRankSearchLevelClass_EXRogue.clear();	// [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
	m_mapRankSearchLevelClass_EXMage.clear();	// 2013/01/08 jeil EX메이지 추가 
#endif
	m_mapRankSearchPK.clear();	
	m_mapRankSearchQuestAcum.clear();
	m_mapRankSearchQuestWeek.clear();
}

void CRankingList::ClearRankingGuildSearch()
{
	m_mapRankSearchGuild.clear();
}

int CRankingList::GetCountRankPoint(ERANKINGEX_SUB_TYPE eSubType)
{
	VSRankingPointData&		vData	= GetRankingPointByType(eSubType);

	return vData.size();
}

int CRankingList::GetCountRankGuild(ERANKINGEX_SUB_TYPE eSubType)
{
	VSRankingGuildData&		vData	= GetRankingGuildByType(eSubType);

	return vData.size();
}

int CRankingList::GetCountRankPointSearch(CTString cstrName, ERANKINGEX_SUB_TYPE eSubType)
{
	std::map<std::string, VSRankingPointData>&			mapData		= GetRankingPointSearchType(eSubType);
	std::map<std::string, VSRankingPointData>::iterator	itmapData	= mapData.begin();

	BOOL		bExistData	= FALSE;
	for(; itmapData != mapData.end(); itmapData++)
	{
		if((itmapData->first).c_str() == cstrName)
		{
			bExistData		= TRUE;
			break;
		}
	}

	if(bExistData == FALSE)
		return 0;

	return (itmapData->second).size();
}

int CRankingList::GetCountRankGuildSearch(CTString cstrName, ERANKINGEX_SUB_TYPE eSubType)
{
	std::map<std::string, VSRankingGuildData>&			mapData		= GetRankingGuildSearchType(eSubType);
	std::map<std::string, VSRankingGuildData>::iterator	itmapData	= mapData.begin();

	BOOL		bExistData	= FALSE;
	for(; itmapData != mapData.end(); itmapData++)
	{
		if((itmapData->first).c_str() == cstrName)
		{
			bExistData		= TRUE;
			break;
		}
	}

	if(bExistData == FALSE)
		return 0;

	return (itmapData->second).size();
}

BOOL CRankingList::GetRankPoint(SRankingPointData* poData, INDEX idxRank, ERANKINGEX_SUB_TYPE eSubType)
{
	VSRankingPointData&		vData	= GetRankingPointByType(eSubType);

	if(idxRank < 0 || idxRank >= vData.size())
		return FALSE;

	*poData		= vData[idxRank];

	return TRUE;
}

BOOL CRankingList::GetRankPointSearch(SRankingPointData* poData, CTString cstrName, INDEX idxRank, ERANKINGEX_SUB_TYPE eSubType)
{
	std::map<std::string, VSRankingPointData>&			mapData		= GetRankingPointSearchType(eSubType);
	std::map<std::string, VSRankingPointData>::iterator	itmapData	= mapData.begin();

	BOOL		bExistData	= FALSE;
	for(; itmapData != mapData.end(); itmapData++)
	{
		if((itmapData->first).c_str() == cstrName)
		{
			bExistData		= TRUE;
			break;
		}
	}

	if(bExistData == FALSE)
		return FALSE;

	VSRankingPointData&		vRankData	= itmapData->second;

	if(idxRank < 0 || idxRank >= vRankData.size())
		return FALSE;

	*poData		= vRankData[idxRank];

	return TRUE;
}

BOOL CRankingList::GetRankGuild(SRankingGuildData* poData, INDEX idxRank, ERANKINGEX_SUB_TYPE eSubType)
{
	VSRankingGuildData&		vData	= GetRankingGuildByType(eSubType);

	if(idxRank < 0 || idxRank >= vData.size())
		return FALSE;

	*poData		= vData[idxRank];

	return TRUE;
}

BOOL CRankingList::GetRankGuildSearch(SRankingGuildData* poData, CTString cstrName, INDEX idxRank, ERANKINGEX_SUB_TYPE eSubType)
{
	std::map<std::string, VSRankingGuildData>&			mapData		= GetRankingGuildSearchType(eSubType);
	std::map<std::string, VSRankingGuildData>::iterator	itmapData	= mapData.begin();

	BOOL		bExistData	= FALSE;
	for(; itmapData != mapData.end(); itmapData++)
	{
		if((itmapData->first).c_str() == cstrName)
		{
			bExistData		= TRUE;
			break;
		}
	}

	if(bExistData == FALSE)
		return FALSE;

	VSRankingGuildData&		vRankData	= itmapData->second;

	if(idxRank < 0 || idxRank >= vRankData.size())
		return FALSE;

	*poData		= vRankData[idxRank];

	return TRUE;
}
