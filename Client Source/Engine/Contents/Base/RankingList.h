#ifndef UIRANKING_LIST_H
#define UIRANKING_LIST_H

#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// 랭킹 시스템 개편 : 랭킹 종류
typedef enum _eRankingExSubType
{
	ERANKINGEX_SUB_TYPE_LEVELMASTER					= 0,
	ERANKINGEX_SUB_TYPE_LEVELCLASS_TITAN,
	ERANKINGEX_SUB_TYPE_LEVELCLASS_KNIGHT,
	ERANKINGEX_SUB_TYPE_LEVELCLASS_HEALER,
	ERANKINGEX_SUB_TYPE_LEVELCLASS_MAGE,
	ERANKINGEX_SUB_TYPE_LEVELCLASS_ROGUE,
	ERANKINGEX_SUB_TYPE_LEVELCLASS_SORCERER,
	ERANKINGEX_SUB_TYPE_LEVELCLASS_NIGHTSHADOW,
#ifdef CHAR_EX_ROGUE
	ERANKINGEX_SUB_TYPE_LEVELCLASS_EX_ROGUE,	// [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
	ERANKINGEX_SUB_TYPE_LEVELCLASS_EX_MAGE,	// 2013/01/08 jeil EX메이지 추가 
#endif
	_ERANKINGEX_SUB_TYPE_END,		// ** 리스트의 끝

	ERANKINGEX_SUB_TYPE_GUILD,
	ERANKINGEX_SUB_TYPE_PK,
	ERANKINGEX_SUB_TYPE_QUESTACUM,
	ERANKINGEX_SUB_TYPE_QUESTWEEK,
}ERANKINGEX_SUB_TYPE;


typedef struct _SRankingPointData
{
	CTString		cstrCharacterName;
	INT				nLastRank;
	INT				nThisRank;
	INT				nLastPoint;
	INT				nThisPoint;
	INT				nLevel;
	UBYTE			ubTribe;
	UBYTE			ubClass;
	CTString		cstrFixedDate;

	_SRankingPointData()
	{
		nLastRank			= 0;
		nThisRank			= 0;
		nLastPoint			= 0;
		nThisPoint			= 0;
		nLevel				= 0;
		ubTribe				= 0;
		ubClass				= 0;
	}

	void operator = (const _SRankingPointData& oData)
	{
		cstrCharacterName	= oData.cstrCharacterName;
		nLastRank			= oData.nLastRank;
		nThisRank			= oData.nThisRank;
		nLastPoint			= oData.nLastPoint;
		nThisPoint			= oData.nThisPoint;
		nLevel				= oData.nLevel;
		ubTribe				= oData.ubTribe;
		ubClass				= oData.ubClass;
		cstrFixedDate		= oData.cstrFixedDate;
	}
}SRankingPointData;

typedef struct _SRankingGuildData
{
	CTString		cstrGuildName;
	INT				nLastRank;
	INT				nThisRank;
	INT				nLastEXP;
	INT				nThisEXP;
	INT				nGuildLevel;
	CTString		cstrGuildLeaderName;
	INT				nGuildMemberCount;
	CTString		cstrFixedDate;

	_SRankingGuildData()
	{
		nLastRank			= 0;
		nThisRank			= 0;
		nLastEXP			= 0;
		nThisEXP			= 0;
		nGuildLevel			= 0;
		nGuildMemberCount	= 0;
	}

	void operator = (const _SRankingGuildData& oData)
	{
		cstrGuildName		= oData.cstrGuildName;
		nLastRank			= oData.nLastRank;
		nThisRank			= oData.nThisRank;
		nLastEXP			= oData.nLastEXP;
		nThisEXP			= oData.nThisEXP;
		nGuildLevel			= oData.nGuildLevel;
		cstrGuildLeaderName	= oData.cstrGuildLeaderName;
		nGuildMemberCount	= oData.nGuildMemberCount;
		cstrFixedDate		= oData.cstrFixedDate;
	}

}SRankingGuildData;

typedef		std::vector<SRankingPointData>		VSRankingPointData;
typedef		std::vector<SRankingGuildData>		VSRankingGuildData;

class CRankingList
{
public:
	CRankingList();
	virtual ~CRankingList();

	typedef std::map<std::string, VSRankingPointData>	mapRankingPtData;
	typedef std::map<std::string, VSRankingGuildData>	mapRankingGuildData;

	void		AddRankingPoint(ERANKINGEX_SUB_TYPE eSubType, SRankingPointData oData);
	void		AddRankingGuild(ERANKINGEX_SUB_TYPE eSubType, SRankingGuildData oData);
	void		AddRankingPointSearch(ERANKINGEX_SUB_TYPE eSubType, SRankingPointData oData, CTString cstrName);
	void		AddRankingPointGuild(ERANKINGEX_SUB_TYPE eSubType, SRankingGuildData oData, CTString cstrName);
	void		ClearRankingPoint();
	void		ClearRankingGuild();
	void		ClearRankingPointSearch();
	void		ClearRankingGuildSearch();	

	int			GetCountRankPoint(ERANKINGEX_SUB_TYPE eSubType = ERANKINGEX_SUB_TYPE_LEVELMASTER);
	int			GetCountRankGuild(ERANKINGEX_SUB_TYPE eSubType = ERANKINGEX_SUB_TYPE_GUILD);
	int			GetCountRankPointSearch(CTString cstrName, ERANKINGEX_SUB_TYPE eSubType = ERANKINGEX_SUB_TYPE_LEVELMASTER);
	int			GetCountRankGuildSearch(CTString cstrName, ERANKINGEX_SUB_TYPE eSubType = ERANKINGEX_SUB_TYPE_GUILD);

	// 랭킹 point정보 얻기
	BOOL		GetRankPoint(SRankingPointData* poData, INDEX idxRank, ERANKINGEX_SUB_TYPE eSubType = ERANKINGEX_SUB_TYPE_LEVELMASTER);
	BOOL		GetRankPointSearch(SRankingPointData* poData, CTString cstrName, INDEX idxRank, ERANKINGEX_SUB_TYPE eSubType = ERANKINGEX_SUB_TYPE_LEVELMASTER);

	// 랭킹 guild정보 얻기
	BOOL		GetRankGuild(SRankingGuildData* poData, INDEX idxRank, ERANKINGEX_SUB_TYPE eSubType = ERANKINGEX_SUB_TYPE_GUILD);
	BOOL		GetRankGuildSearch(SRankingGuildData* poData, CTString cstrName, INDEX idxRank, ERANKINGEX_SUB_TYPE eSubType = ERANKINGEX_SUB_TYPE_GUILD);

private:	// Helper
	VSRankingPointData&			GetRankingPointByType(ERANKINGEX_SUB_TYPE eSubType);
	VSRankingGuildData&			GetRankingGuildByType(ERANKINGEX_SUB_TYPE eSubType);
	mapRankingPtData&			GetRankingPointSearchType(ERANKINGEX_SUB_TYPE eSubType);
	mapRankingGuildData&		GetRankingGuildSearchType(ERANKINGEX_SUB_TYPE eSubType);

protected:	
	// 랭킹 정보 : 정보를 쌓은 순서대로 적재
	VSRankingPointData		m_vRankLevelMaster;
	VSRankingPointData		m_vRankLevelClass_Titan;
	VSRankingPointData		m_vRankLevelClass_Knight;
	VSRankingPointData		m_vRankLevelClass_Healer;
	VSRankingPointData		m_vRankLevelClass_Mage;
	VSRankingPointData		m_vRankLevelClass_Rogue;
	VSRankingPointData		m_vRankLevelClass_Sorcerer;
	VSRankingPointData		m_vRankLevelClass_NightShadow;
#ifdef CHAR_EX_ROGUE
	VSRankingPointData		m_vRankLevelClass_EXRogue;	// [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
	VSRankingPointData		m_vRankLevelClass_EXMage;	// 2013/01/08 jeil EX메이지 추가 
#endif
	VSRankingGuildData		m_vRankGuild;
	VSRankingPointData		m_vRankPK;
	VSRankingPointData		m_vRankQuestAcum;
	VSRankingPointData		m_vRankQuestWeek;

	// 검색 랭킹 정보
	mapRankingPtData		m_mapRankSearchLevelMaster;
	mapRankingPtData		m_mapRankSearchLevelClass_Titan;
	mapRankingPtData		m_mapRankSearchLevelClass_Knight;
	mapRankingPtData		m_mapRankSearchLevelClass_Healer;
	mapRankingPtData		m_mapRankSearchLevelClass_Mage;
	mapRankingPtData		m_mapRankSearchLevelClass_Rogue;
	mapRankingPtData		m_mapRankSearchLevelClass_Sorcerer;
	mapRankingPtData		m_mapRankSearchLevelClass_NightShadow;
#ifdef CHAR_EX_ROGUE
	mapRankingPtData		m_mapRankSearchLevelClass_EXRogue;// [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
	mapRankingPtData		m_mapRankSearchLevelClass_EXMage;	//2013/01/08 jeil EX메이지 추가 
#endif
	mapRankingGuildData		m_mapRankSearchGuild;
	mapRankingPtData		m_mapRankSearchPK;
	mapRankingPtData		m_mapRankSearchQuestAcum;
	mapRankingPtData		m_mapRankSearchQuestWeek;
};

#endif	//	UIRANKING_LIST_H