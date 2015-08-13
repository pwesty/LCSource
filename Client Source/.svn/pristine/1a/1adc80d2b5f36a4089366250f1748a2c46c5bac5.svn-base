// ----------------------------------------------------------------------------
//  File : UISELCHAR.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	UISELCHAR_H_
#define	UISELCHAR_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/GameState.h>

// Define position
#define	SELCHAR_LEVEL_CX			36
#define	SELCHAR_LEVEL_SY			4
#define	SELCHAR_NAME_SX				62
#define	SELCHAR_NAME_SY				4

#define	SELCHAR_HP_SX				40
#define	SELCHAR_HP_CX				110
#define	SELCHAR_HP_SY				22
#define	SELCHAR_MP_SY				36
#define	SELCHAR_EXP_SY				50

//wooss 050819
#define SELCHAR_EX_SLOT_X1			90
#define SELCHAR_EX_SLOT_X2			60
#define SELCHAR_EX_SLOT_Y1			70
#define SELCHAR_EX_SLOT_Y2			90
#define SELCHAR_EX_SLOT_Y3			105
						

// Define size of select character
#define	SELCHAR_WIDTH				504
#define	SELCHAR_HEIGHT				22
#define	SELCHAR_BAR_WIDTH			162


// ----------------------------------------------------------------------------
// Name : CUISelChar
// Desc :
// ----------------------------------------------------------------------------
class CUISelChar : public CUIWindow
{
public:
	CUISelChar();
	~CUISelChar();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	void	SetBackgroundWorld(CWorld *pWorld) { m_pWorld = pWorld; }
	void	StartGame();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Reset
	void	Reset();

	// Render
	void	Render();	

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );	

	void	Lock(BOOL bLock);

protected:
	// Internal functions
	void	UpdateStatus();
	// 캐릭터 생성, 삭제, 선택.	
	BOOL	DeleteCharacter();
	BOOL	DeleteCharacter(CTString secuNum);
	void	SelectCharacter(int nX, int nY);
	BOOL	PopUpMsgBoxExist();
	
	
public:
	BOOL	ShowCharacter();
	void	SetExSlotTime(int slotNum,int rTime);
	int		GetExSlotTime(int slotNum=0,int* tSlot1=NULL,int* tSlot2=NULL);	// 0 : All  1 : slot1  2 : slot2
	void	ShowCharMoveRollbackMsgBox(bool bShow); // Server move fail messageBox [7/26/2012 Ranma]
	
	// [2012/07/05 : Sora]  캐릭터 슬롯 확장
	void	SetCharSlotTime( ULONG ulTime );
	CTString GetRemainTime();
	void	UpdateCharSlotTime();

private:
	// texture container.
	CTextureData*		m_ptdMsgTexture;
	// Controls
	CUIButton			m_btnCreateChar;		// Button for Create Character
	CUIButton			m_btnDeleteChar;		// Button for Delete Character
	CUIButton			m_btnOptions;			// Button for Options
	CUIButton			m_btnExit;				// Button for Exit Game
	CUIButton			m_btnOK;				// OK button(Game Start)
	CUIButton			m_btnBack;				// [2012/10/18 : Sora] 재시작시 자동 로그인
	CUIButton			m_btnTurnR;				// Button for Turn Right
	CUIButton			m_btnTurnL;				// Button for Turn Left
	CEffectGroup*		m_aEGslotPosID[MAX_SLOT];		// Position Entity ID
	int					m_iDestPosID;			//
	int					m_exSlot1,m_exSlot2;	// wooss 050820 extend slot remain time 	
	// Information
	CTString			m_strName;				// Name
	CTString			m_strLevel;				// Level
	CTString			m_strHP;				// HP
	CTString			m_strMP;				// MP
	CTString			m_strEXP;				// Exp
	CTString			m_strCharServerMove;	// 캐릭터 서버 이전 [7/25/2012 Ranma]

	// Region of each part
	UIRect				m_rcCharInfo;			// Region of character information
	UIRect				m_rcHP;					// Region of HP
	UIRect				m_rcMP;					// Region of MP
	UIRect				m_rcEXP;				// Region of EXP
	UIRect				m_rcExSlot;				// Region of Extend slot info //wooss 050819
	UIRect				m_rcExSlotLineTop;		// line  //wooss 050819
	UIRect				m_rcExSlotLineMidH;		// line middle //wooss 050819
	UIRect				m_rcExSlotLineMidV;		// line middle //wooss 050819
	UIRect				m_rcExSlotLineBtm;		// line bottom //wooss 050819
	UIRect				m_rcDelTime;
	// night shadow cards. [11/6/2009 rumist]
	UIRect				m_rcMessageInfo;
	CUIMultiLineText	m_msgNCText;
	UIRect				m_rcCharMoveMsgInfo;
	CUIMultiLineText	m_msgCMText;


	// UV of each part
	UIRectUV			m_rtCharInfo;			// UV of character information
	UIRectUV			m_rtHP;					// UV of HP
	UIRectUV			m_rtMP;					// UV of MP
	UIRectUV			m_rtEXP;				// UV of EXP
	UIRectUV			m_rtExSlot;				// UV of ExSlot //wooss 050819
	UIRectUV			m_rtExSlotLine;			// line top //wooss 050819
	UIRectUV			m_rtExSlotLineMidV;		// line vertical //wooss 050819
	UIRectUV			m_rtDelTime;		// UV of remain time to del char

	// UV of message part. [11/6/2009 rumist]
	CUIDrawBox			m_bxNoticeMsg;
	BOOL				m_bIsLeftView;
	
	// 캐릭터 서버 이전 [7/26/2012 Ranma]
	CUIDrawBox			m_bxCharMoveNoticeMsg;
	BOOL				m_bIsShowCharMoveMsgInfo;
	BOOL				m_bIsShowMessageInfo;

	CTString			m_strServerMoveFailCharName;
	
	// [2012/07/05 : Sora]  캐릭터 슬롯 확장
	ULONG				m_ulCharSlotTime;
	__int64				m_CharSlotTimeOld;
	UIRect				m_rcCharSlotHelp;
	CUIDrawBox			m_bxCharSlotHelp;
	CUIMultiLineText	m_msgCharSlotHelp;

	UIRect				m_rcCharSlotRemain;
	CUIDrawBox			m_bxCharSlotRemain;

	// Slot Ani
	typedef struct ANIINFO {
		INDEX	m_idAni;
		INDEX	m_startTime;	 
	} SLOT_ANI_INFO;
	SLOT_ANI_INFO m_aSlotAni[MAX_SLOT];


private:
	CWorld*				m_pWorld;	
};

#endif // UISELCHAR_H_
