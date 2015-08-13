// ----------------------------------------------------------------------------
//  File : UIPlayerInfo.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIPLAYERINFO_H_
#define	UIPLAYERINFO_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <Engine/Interface/UIButton.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/Interface/UITrackPopup.h>

// Define position
/****************************************
#define	PLAYERINFO_LEVEL_CX			22
#define	PLAYERINFO_LEVEL_SY			5
#define	PLAYERINFO_NAME_SX			33
#define	PLAYERINFO_NAME_SY			5

#define	PLAYERINFO_HP_SX			28
#define	PLAYERINFO_HP_CX			91
#define	PLAYERINFO_HP_SY			53
#define	PLAYERINFO_MP_SY			67
#define	PLAYERINFO_EXP_SY			81


// Define size of player information
#define	PLAYERINFO_WIDTH			140
#define	PLAYERINFO_HEIGHT			98
#define	PLAYERINFO_BAR_WIDTH		120
******************************************/

///////////////////////////////////////////////////////
// UI_REFORM :Su-won
#define	PLAYERINFO_LEVEL_CX			83
#define	PLAYERINFO_LEVEL_SY			71
#define	PLAYERINFO_NAME_SX			120
#define	PLAYERINFO_NAME_SY			5

#define	PLAYERINFO_HP_SX			114
#define	PLAYERINFO_HP_CX			180
#define	PLAYERINFO_HP_SY			29
#define	PLAYERINFO_MP_SY			42
#define	PLAYERINFO_EXP_SY			55


// Define size of player information
#define	PLAYERINFO_WIDTH			275
#define	PLAYERINFO_HEIGHT			90
#define	PLAYERINFO_BAR_WIDTH		120
// UI_REFORM :Su-won

// [7/15/2010 kiny8216] Monster Energy Level Point
#define ENERGY_POINT_LEVEL1			50000		// 1레벨 EP량
#define ENERGY_POINT_LEVEL2			60000		// 2레벨 EP량
#define ENERGY_POINT_LEVEL3			70000		// 3레벨 EP량
#define ENERGY_POINT_LEVEL4			70000		// 4레벨 EP량
#define ENERGY_POINT_LEVEL5			100000		// 5레벨 EP량

#define ENERGY_POINT_MAX1			50000		// 1레벨 누적 EP량
#define ENERGY_POINT_MAX2			110000		// 2레벨 누적 EP량
#define ENERGY_POINT_MAX3			180000		// 3레벨 누적 EP량
#define ENERGY_POINT_MAX4			250000		// 4레벨 누적 EP량
#define ENERGY_POINT_MAX5			350000		// 5레벨 누적 EP량

#define ENERGY_GAGEBAR_WIDTH		166			// EP 게이지바 너비
///////////////////////////////////////////////////////



// ----------------------------------------------------------------------------
// Name : CUIPlayerInfo
// Desc :
// ----------------------------------------------------------------------------
class CUIPlayerInfo : public CUIWindow
{
protected:
	enum eQuickBtnCount
	{
		QUICKMENU_BTN_CHAR_INFO = 0,
		QUICKMENU_BTN_INVEN,
		QUICKMENU_BTN_SKILL,
		QUICKMENU_BTN_QUEST,
		QUICKMENU_BTN_PET_INFO,
		QUICKMENU_BTN_MESSENGER,
		QUICKMENU_BTN_WEB_BOARD,
		QUICKMENU_BTN_SYSTEM_MENU,
		QUICKMENU_BTN_RANKING,
		QUICKMENU_BTN_CASH_SHOP,
		QUICKMENU_BTN_LAKA_BALL,

		QUICKMENU_BTN_MAX
	};
	// Controls
	CUIButton			m_btnBoard;					// Button of web board
	CUIButton			m_btnMessanger;				// Button of messanger
	CUIButton			m_btnSysMenu;				// Button of system menu
	CUIButton			m_btnInven;					// Button of inventory
	CUIButton			m_btnCharInfo;				// Button of character info

	// UI_REFORM :Su-won
	CUIButton			m_btnQuickMenu[QUICKMENU_BTN_MAX];			//퀵메뉴 버튼


	BOOL				m_bShowToolTip;				// If tool tip is shown or not
	CTString			m_strToolTip;				// String of tool tip

	// Region of each part
	UIRect				m_rcToolTip;				// Region of tool tip

	// UI_REFORM :Su-won
	UIRect				m_rcQuickMenu;				//퀵메뉴 영역

 	UIRectUV			m_rtToolTipL;				// UV of left region of tool tip
 	UIRectUV			m_rtToolTipM;				// UV of middle region of tool tip
 	UIRectUV			m_rtToolTipR;				// UV of right region of tool tip

	// UI_REFORM :Su-won
	UIRectUV			m_rtQuickMenu;				//퀵메뉴 영역 텍스쳐 좌표

	CTextureData		*m_ptdCommonBtnTexture;
	CTextureData		*m_ptdGuildBattleTexture;
	CTextureData		*m_ptdMessageBoxTexture;
	CTextureData		*m_ptdExpeditionTexture;
	CTextureData		*m_ptdQuestBookTexture;

	// [090727: selo] 선택되어 있는 퀘스트를 표시하기 위한 리스트 박스
	CUIListBox			m_lbSelectedQuest;
	int					m_iSelectedQuestLineCnt;

	// [090817: selo] 퀘스트 공지를 위한 리스트 박스
	CUIListBox			m_lbQuestNotice;
	int					m_iQuestNoticeLineCnt;
	int					m_iQuestNoticeMaxStringCnt;
	CUIButton			m_btnQuestNoticeClose;			// [090821: selo] 퀘스트 공지 닫기 버튼

	// [090907: selo] 시간 표시용 숫자
	UIRectUV			m_rtSmallNumber[10];			// 시간 표시 용 숫자
	UIRectUV			m_rtLargeNumber[10];			// 숫자 큰거
	UIRectUV			m_rtColon;						// 시간 표시 용 :(콜론)

	SQUAD				m_llShortCutTime;
	
protected:
	// Internal functinos
	void	ToggleUIWebBoard();
	void	ToggleUIMessanger();
	void	ToggleUISysMenu();
	void	ToggleUIInventory();
	void	ToggleUICharacterInfo();
	void	ToggleUIHelp();
	void	ShowToolTip( BOOL bShow, int nToolTipID = -1 );

public:
	CUIPlayerInfo();
	~CUIPlayerInfo();

	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	Render();
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );

	BOOL	ProcessShortCut( MSG *pMsg );

	WMSG_RESULT	MouseMessage( MSG *pMsg );

	void ResetQuickMenuPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void ShowQuickMenuToolTip(BOOL bShow, int nToolTipID = -1);
	void OpenQuickMenu(int nMenu);
	
	// 전제화면에 표시할 타임어택 남은시간 랜더
	void RenderTimeAttackRemainTime();

	// [090803 : sora] 선택된 퀘스트 리스트를 지워준다
	void ClearSelectedQuest();
	
	// [090817: selo] 퀘스트 공지 관련
	void UpdateQuestNotice(INDEX iQuestIndex);
	void AddQuestNoticeString(CTString& strMessage, const COLOR col);
	void RenderQuestNotice();
	
	// [090907: selo] 시간 그리기
	void RenderElapsedTime(int nPosX, int nPosY, DOUBLE dStartTime, COLOR col = 0xFFFFFFFF);	// 시작시간을 기점으로 흐른 시간 그리기(단위 초)
	void RenderRemainTime(int nPosX, int nPosY, DOUBLE dTargetTime, COLOR col = 0xFFFFFFFF);	// 시작시간을 기점으로 남은 시간 그리기(단위 초)

	void RenderTime(int nPosX, int nPosY, DOUBLE dTime, COLOR col);
	void RenderTime(int nPosX, int nPosY, int nHour, int nMinute, int nSecond, COLOR col);
	void DrawNumber(int nPosX, int nPosY, int nNumber, COLOR col, bool bLarge = false);
	void DrawColon(int nPosX, int nPosY, COLOR col);
};


#endif	// UIPLAYERINFO_H_

