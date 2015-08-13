#ifndef UIRANKING_VIEWEX_H
#define UIRANKING_VIEWEX_H

#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIListBoxMultiList.h>

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
protected:
	// 랭킹 정보 : 정보를 쌓은 순서대로 적재
	VSRankingPointData									m_vRankLevelMaster;
	VSRankingPointData									m_vRankLevelClass_Titan;
	VSRankingPointData									m_vRankLevelClass_Knight;
	VSRankingPointData									m_vRankLevelClass_Healer;
	VSRankingPointData									m_vRankLevelClass_Mage;
	VSRankingPointData									m_vRankLevelClass_Rogue;
	VSRankingPointData									m_vRankLevelClass_Sorcerer;
	VSRankingPointData									m_vRankLevelClass_NightShadow;
#ifdef CHAR_EX_ROGUE
	VSRankingPointData									m_vRankLevelClass_EXRogue;	// [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
	VSRankingPointData									m_vRankLevelClass_EXMage;	// 2013/01/08 jeil EX메이지 추가 
#endif
	VSRankingGuildData									m_vRankGuild;
	VSRankingPointData									m_vRankPK;
	VSRankingPointData									m_vRankQuestAcum;
	VSRankingPointData									m_vRankQuestWeek;

	// 검색 랭킹 정보
	std::map<std::string, VSRankingPointData>			m_mapRankSearchLevelMaster;
	std::map<std::string, VSRankingPointData>			m_mapRankSearchLevelClass_Titan;
	std::map<std::string, VSRankingPointData>			m_mapRankSearchLevelClass_Knight;
	std::map<std::string, VSRankingPointData>			m_mapRankSearchLevelClass_Healer;
	std::map<std::string, VSRankingPointData>			m_mapRankSearchLevelClass_Mage;
	std::map<std::string, VSRankingPointData>			m_mapRankSearchLevelClass_Rogue;
	std::map<std::string, VSRankingPointData>			m_mapRankSearchLevelClass_Sorcerer;
	std::map<std::string, VSRankingPointData>			m_mapRankSearchLevelClass_NightShadow;
#ifdef CHAR_EX_ROGUE
	std::map<std::string, VSRankingPointData>			m_mapRankSearchLevelClass_EXRogue;// [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
	std::map<std::string, VSRankingPointData>			m_mapRankSearchLevelClass_EXMage;	//2013/01/08 jeil EX메이지 추가 
#endif
	std::map<std::string, VSRankingGuildData>			m_mapRankSearchGuild;
	std::map<std::string, VSRankingPointData>			m_mapRankSearchPK;
	std::map<std::string, VSRankingPointData>			m_mapRankSearchQuestAcum;
	std::map<std::string, VSRankingPointData>			m_mapRankSearchQuestWeek;

private:	// Helper
	VSRankingPointData&									GetRankingPointByType(ERANKINGEX_SUB_TYPE eSubType);
	VSRankingGuildData&									GetRankingGuildByType(ERANKINGEX_SUB_TYPE eSubType);
	std::map<std::string, VSRankingPointData>&			GetRankingPointSearchType(ERANKINGEX_SUB_TYPE eSubType);
	std::map<std::string, VSRankingGuildData>&			GetRankingGuildSearchType(ERANKINGEX_SUB_TYPE eSubType);

public:
	void		AddRankingPoint(ERANKINGEX_SUB_TYPE eSubType, SRankingPointData oData);
	void		AddRankingGuild(ERANKINGEX_SUB_TYPE eSubType, SRankingGuildData oData);
	void		AddRankingPointSearch(ERANKINGEX_SUB_TYPE eSubType, SRankingPointData oData, CTString cstrName);
	void		AddRankingPointGuild(ERANKINGEX_SUB_TYPE eSubType, SRankingGuildData oData, CTString cstrName);
	void		ClearRankingPoint();
	void		ClearRankingGuild();
	void		ClearRankingPointSearch();
	void		ClearRankingGuildSearch();

public:
	CRankingList();
	virtual ~CRankingList();

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

};


class CUIRankingViewEX: public CUIWindow
{
public:
	explicit CUIRankingViewEX( int nClassID );
	~CUIRankingViewEX ();

	int					m_iClassID; 

	CRankingList		m_oRankingList;
	ERANKINGEX_SUB_TYPE	m_eSubType;						// 보여지고있는 분류

	// Texture Data
	CTextureData*		m_ptexBaseInterface;
	CTextureData*		m_ptexBaseMessageBox;
	CTextureData*		m_ptexBaseCashshop;
	CTextureData*		m_ptexBaseRanking;

	// UI RECT
	// UI 창틀
	UIRect				m_rcTitleArea;
	UIRect				m_rcBackT[3];
	UIRectUV			m_rcuvBackTL;
	UIRectUV			m_rcuvBackTM;
	UIRectUV			m_rcuvBackTR;
	UIRect				m_rcBackM[3];
	UIRectUV			m_rcuvBackML;
	UIRectUV			m_rcuvBackMM;
	UIRectUV			m_rcuvBackMR;
	UIRect				m_rcBackB[3];
	UIRectUV			m_rcuvBackBL;
	UIRectUV			m_rcuvBackBM;
	UIRectUV			m_rcuvBackBR;
	UIRect				m_rcBackgroundInside[9];
	UIRectUV			m_rcuvBackgoundInside[9];
	UIRect				m_rcSearchBox[3];
	UIRectUV			m_rcuvSearchBox[3];
	// 랭킹 리스트 박스의 타이틀/내용 배경
	LONG				m_rcListTitleWidth[_ERANKINGEX_SUB_TYPE_END][8];	// 랭킹 리스트의 각 cols width
	UIRect				m_rcRankingColumn[2][8];		// [Top/Bottom][cols]
	UIRectUV			m_rcuvRankingColumn[3];			// [Left/Middle/Right]
	UIRect				m_rcRankTypeBox;
	UIRectUV			m_rcuvRankTypeBox;
	// 정보 가져오기 프로그래시브 바
	UIRect				m_rcProgressBox;				
	UIRectUV			m_rcuvProgressBox;
	CUIProgressBar		m_prbProgress;

	// UI Control
	CUIButton			m_btnClose;						// 우측 상단 종료 버튼
	CUIComboBox			m_cbRankType;					// 랭킹 타입 콤보박스
	CUIButton			m_btnReset;						// 초기화 버튼
	CUIButton			m_btnSearch;					// 검색 버튼
	CUIListBoxMultiCol	m_lbTopRanking;					// 상단 랭킹 리스트
	CUIListBoxMultiCol	m_lbBottomRanking;				// 하단 랭킹 리스트
	CUIEditBox			m_ebSearch;						// 검색 에디트 박스

	CTString			m_cstrTitle;					// 타이틀
	CTString			m_cstrSearch;					// 검색한 아이디
	CTString			m_cstrTopRankingMsg;			// 윗부분 랭킹리스트 박스 내 표시되는 메세지
	CTString			m_cstrBottomRankingMsg;			// 아랫부분 랭킹리스트 박스 내 표시되는 메세지
	
	INT					m_nRecvRankTotalCount;			// 전송받을 총 데이터의 갯수 (패킷 받을 총 랭킹 건 수)
	INT					m_nRecvRankCount;				// 전송받은 데이터의 갯수 (패킷 받은 랭킹 건 수)
	BOOL				m_bRecvRank;					// 전송받는 중

public:
	CRankingList*		GetRankingList()				{ return &m_oRankingList;			}

public:
	CTString			GetJobStringByType(UBYTE ubClass);					// 직업 타입을 스트링으로 얻기
	CTString			GetRankStringByType(ERANKINGEX_SUB_TYPE eSubType);	// 랭킹 타입을 스트링으로 얻기
	UCHAR				GetRankingNetworkType();							// 0 : 레벨마스터랭킹, 1: 직업랭킹
	UCHAR				GetRankingNetworkSubType();							// 0:(없음) , 1:(0 : 타이탄직업, 1 : 나이트직업 ...)
	void				SetTitle(CTString cstrTitle)	{ m_cstrTitle = cstrTitle;			}

public:	
	
	// Create
	virtual void		Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight);

	// 자신의 id가지기 
	virtual void		SetClassID(int nID)
	{
		m_iClassID = nID;
	}
	
	// Render
	void				Render();
	
	// Adjust position
	void				ResetPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ);
	void				AdjustPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ);
	
	// Messages
	WMSG_RESULT			KeyMessage(MSG *pMsg);
	WMSG_RESULT			CharMessage(MSG *pMsg);
	WMSG_RESULT			IMEMessage(MSG *pMsg);
	WMSG_RESULT			MouseMessage(MSG *pMsg);

	void				ToggleVisible();
	void				ToggleReset();
	// 분류 창 보이기 
	void				Open();
	void				Open(ERANKINGEX_SUB_TYPE eSubType);
	// 분류 창 닫기
	void				Close();

	// 보여지는 분류 변경
	void				ChangeViewType(ERANKINGEX_SUB_TYPE eSubType);

	// 검색
	void				SearchName(CTString cstrName);

	// 랭킹 정보 업데이트
	void				UpdateTopRanking();
	void				UpdateBottomRanking();

	// 랭킹 정보 패킷 수진아~~
	void				RecvRankingList(CNetworkMessage& nmMessage);
	BOOL				CheckRecvRankingListAndReq();
	
	// esc closing support [5/30/2011 rumist]
	BOOL				CloseWindowByEsc()			{ Close();	return TRUE;			}
	virtual BOOL		IsEditBoxFocused()			{ return m_ebSearch.IsFocused();	}

	void				ClearRankingList();

public:
	void				AddTopRankingString(int iCol, int iRow, CTString cstrString, COLOR cColor = 0xF2F2F2FF);
	void				AddBottomRankingString(int iCol, int iRow, CTString cstrString, COLOR cColor = 0xF2F2F2FF);
};

#endif
