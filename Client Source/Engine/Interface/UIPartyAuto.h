// ----------------------------------------------------------------------------
//  File : CUIPartyAuto.h
//  Desc : Created by eons
// ----------------------------------------------------------------------------

#ifndef UIPARTY_AUTO_H_ 
#define UIPARTY_AUTO_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// #include <Engine/Interface/UIButton.h>
// #include <Engine/Interface/UIEditBox.h>
// #include <Engine/Interface/UICheckButton.h>
// #include <Engine/Interface/UIListBox.h>
// #include <Engine/Interface/UIComboBox.h>
// #include <vector>

#define P_AUTO_WIDTH	598
#define P_AUTO_HEIGHT	423

//	Job Flags
#define JF_TITAN		0x00000001
#define JF_KNIGHT		0x00000002
#define JF_HEALER		0x00000004
#define JF_MAGE			0x00000008
#define JF_ROGUE		0x00000010
#define JF_SORCERER		0x00000020
#define JF_NIGHTSHADOW	0x00000040

#ifdef CHAR_EX_ROGUE
	#define JF_EX_ROGUE		0x00000080	// [2012/08/27 : Sora] EX로그 추가
		#ifdef CHAR_EX_MAGE
			#define JF_EX_MAGE 0x00000100	//	2013/01/08 jeil EX메이지 추가 플레그 관련해서 물어보고 추가로 수정필요 임시값 
			#define JF_ALLCLASS		0x000001FF	
		#else
			#define JF_ALLCLASS		0x000000FF
	#endif
#else
	#define JF_ALLCLASS		0x0000007F
#endif
/*
<<<<<<< .mine
#ifdef CHAR_EX_MAGE
	#define JF_EX_MAGE 0x00000100	//	2013/01/08 jeil EX메이지 추가 플레그 관련해서 물어보고 추가로 수정필요 임시값 
#endif
#define JF_ALLCLASS		0x0000007F	
=======
>>>>>>> .r2954
*/

enum eAutoPartySelection
{
	PARTY_REGIST,
	PARTY_REGIST_LEADER,
	INVITE_LIST,
	PARTY_LIST,
	PARTY_DEL,
};

// ----------------------------------------------------------------------------
// Name : CUIPartyAuto
// Desc : 파티오토 매칭 시스템
// ----------------------------------------------------------------------------

class CUIPartyAuto : public CUIWindow
{
protected:
	typedef struct
	{
		CTString	strSubject;		// 파티 문구 및 캐릭명
		DWORD		dwJobflag;		// 직업
		int			nIndex;			// 캐릭 식별자 및 보스 식별자
		int			nLeveldiff;		// 레벨 제한 on/off
		int			nZone;			// 파티 위치
		int			nPartyType;		// 파티 종류
		int			nLimitLv;		// 제한 레벨( 사용 안함 )
		int			nPartyNum;		// 파티 현재 모집 인원
	}PartyListData;

	// storage current page list
	std::vector<PartyListData>	m_vectorPartyListData;

	CTextureData	*m_ptdAddTexture;		// Add Texture resource

	// Button
	// Join set
	CUIButton	m_btnEqualDivide;			// 균등 분배 파티
	CUIButton	m_btnFirstObtain;			// 입수 우선 파티
	CUIButton	m_btnFight;					// 전투형 파티
	CUIButton	m_btnClose;					// Close button

	CUIButton	m_btnRegistOK;				// 파티 등록 확인( 및  파티 조인 신청, 파티 초대 )
	CUIButton	m_btnRegistCancel;			// 파티 등록 취소
	
	CUIButton	m_btnNextbtn;				// 다음 페이지
	CUIButton	m_btnPrevbtn;				// 이전 페이지
	CUIButton	m_btnRefresh;				// 새로 고침
	CUIEditBox	m_ebNoticeName;				// Input box

	CUICheckButton	m_cbtnTitan;			// Titan
	CUICheckButton	m_cbtnKnight;			// Knight
	CUICheckButton	m_cbtnHealer;			// Healer
	CUICheckButton	m_cbtnMage;				// Mage
	CUICheckButton	m_cbtnRogue;			// Rogue
	CUICheckButton	m_cbtnSorcerer;			// Socceror
	CUICheckButton	m_cbtnNightShadow;		// NightShadow
#ifdef CHAR_EX_ROGUE
	CUICheckButton	m_cbtnEXRogue;			// [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
	CUICheckButton	m_cbtnEXMage;			// 2013/01/08 jeil EX메이지 추가 
#endif
	CUICheckButton	m_cbtnLevelLimit;		// 레벨 제한
	CUIComboBox		m_cmbNeedClass;			// 필요 클래스
	CUIComboBox		m_cmbPartyType;			// 파티 타입

	// Region of each part
	// Join set
	UIRect		m_rcJoinTitle;				// Region of title
	UIRect		m_rcSelLine;				// line select rectangle

	// UV of each part
	// Join set
	UIRectUV	m_rtJoinTopL;				// UV of Top background
	UIRectUV	m_rtJoinTopM;				// UV of Top background
	UIRectUV	m_rtJoinTopR;				// UV of Top background
	UIRectUV	m_rtJoinGapL;				// UV of Gap
	UIRectUV	m_rtJoinGapM;				// UV of Gap
	UIRectUV	m_rtJoinGapR;				// UV of Gap
	UIRectUV	m_rtJoinMiddleL;			// UV of middle background
	UIRectUV	m_rtJoinMiddleM;			// UV of middle background
	UIRectUV	m_rtJoinMiddleR;			// UV of middle background
	UIRectUV	m_rtJoinBottomL;			// UV of bottom background
	UIRectUV	m_rtJoinBottomM;			// UV of bottom background
	UIRectUV	m_rtJoinBottomR;			// UV of bottom	background

	UIRectUV	m_rtSelListBackTopL;		// UV of List Background
	UIRectUV	m_rtSelListBackTopM;
	UIRectUV	m_rtSelListBackTopR;
	UIRectUV	m_rtSelListBackMiddleL;
	UIRectUV	m_rtSelListBackMiddleM;
	UIRectUV	m_rtSelListBackMiddleR;
	UIRectUV	m_rtSelListBackBottomL;
	UIRectUV	m_rtSelListBackBottomM;
	UIRectUV	m_rtSelListBackBottomR;
	UIRectUV	m_rtSelListMiddleL;			// UV of List middle background
	UIRectUV	m_rtSelListMiddleM;
	UIRectUV	m_rtSelListMiddleR;

	UIRectUV	m_rtStrTooltipTopL;				// tool tip
	UIRectUV	m_rtStrTooltipTopM;
	UIRectUV	m_rtStrTooltipTopR;
	UIRectUV	m_rtStrTooltipMiddleL;	
	UIRectUV	m_rtStrTooltipMiddleM;	
	UIRectUV	m_rtStrTooltipMiddleR;
	UIRectUV	m_rtStrTooltipBottomL;
	UIRectUV	m_rtStrTooltipBottomM;
	UIRectUV	m_rtStrTooltipBottomR;

	UIRectUV	m_rtNoticeNameL;			// UV of NoticeName background
	UIRectUV	m_rtNoticeNameM;			
	UIRectUV	m_rtNoticeNameR;

	UIRectUV	m_rtListSubjectRect;			// UV of Subject background
	UIRectUV	m_rtline;					// UV of line background
	UIRectUV	m_rtSelectBar;				// UV of select bar background

	UIRectUV	m_rtTitanImage;			// UV of Check box Job Image
	UIRectUV	m_rtKnightImage;
	UIRectUV	m_rtHealerImage;
	UIRectUV	m_rtMageImage;
	UIRectUV	m_rtRogueImage;
	UIRectUV	m_rtSorcererImage;
	UIRectUV	m_rtNightShadowImage;
#ifdef CHAR_EX_ROGUE
	UIRectUV	m_rtEXRogueImage;		// [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
	UIRectUV	m_rtEXMageImage;		// 2013/01/08 jeil EX메이지 추가 
#endif
	UIRectUV	m_rtPartyLvImage;		// UV of limitlevel image
	UIRectUV	m_rtPeaceeverImage;		// UV of PT_PEACEEVER image
	UIRectUV	m_rtSurvival;			// UV of PT_SURVIVAL image
	UIRectUV	m_rtAttack;				// UV of PT_ATTACK image

	CTString	m_strNoticeName;			// Party Notice Name
	
	DWORD		m_dwJobFlag;				// Job Flag
	
	BOOL		m_bIsPartyRequested;			// 
	BOOL		m_bShowTextScroll;
	BOOL		m_bTextDir;

	TIME		mtmTextScrollTime;

	int			m_nSelectState;				// selection state
	int			m_nLevelLimit;				// level limit num
	int			m_nCurrentPageNum;			// Current Page number
	int			m_nSelectLine;				// List select line
	int			m_nCharIndex;				// Allow and Reject CharIndex

	int			m_FrontChPos;				// string start pos;

protected:
	void		AdjustUIPos();
	void		PressOKBtn();
	void		PartyMatchJoin( SBYTE sbType, SLONG slBoss, CTString strBossName, SLONG slChar,
							CTString strCharName, SBYTE sbCharJob );
	void		SendPartyReq( int nNum );	// 1-> Next, 0->Current, -1-> Prev
	void		RenderTooltip();		// 삭제 예정
	BOOL		Is2ByteChar( int nCharIndex, int nFirstCheck, CTString strText );
public:
	CUIPartyAuto();
	~CUIPartyAuto();

	// Create
	void		Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void		Clear();
	// Render
	void		Render();
	void		JoinRender();
	void		LeaderJoinRender();
	void		SelectListRender();
	void		ListRecordRender();

	// Adjust position
	void		ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void		AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void		OpenPartyMatching();										// system select mode
	void		OpenAutoParty( int nType );			// Oepn AutoParty Menu
	void		CloseAutoParty();						

	// Edit box focus
	BOOL		IsEditBoxFocused() { return m_ebNoticeName.IsFocused();	}
	void		KillFocusEditBox() { m_ebNoticeName.SetFocus( FALSE ); }

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	WMSG_RESULT KeyMessage( MSG *pMsg );
	WMSG_RESULT IMEMessage( MSG *pMsg );
	WMSG_RESULT CharMessage( MSG *pMsg );

	WMSG_RESULT JoinMouseMsg( MSG *pMsg ); // 가입 희망 파티 등록
	WMSG_RESULT LeaderRegMouseMsg( MSG *pMsg ); // 파티장 파티 등록
	WMSG_RESULT	PartyListMouseMsg( MSG *pMsg ); // 파티 리스트 및 파티원 초대 리스트 

	// Command functions
	void		MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void		MsgBoxLCommand( int nCommandCode, int nResult );

	// Error Msg
	void		MatchRegMemberRep( int nErrorcode );
	void		MatchRegPartyRep( int nErrorcode );

	// Network receive
	void		ReceiveMemberData( CNetworkMessage *istr );
	void		ReceivePartyData( CNetworkMessage *istr );
	void		ReceivePartyInviteMessage( int nErrorcode, CNetworkMessage *istr );
	void		ReceivePartyJoinMessage( int nErrorcode, CNetworkMessage *istr );

	// Add list
	void		AddPartyData( int nIndex, int nLvdiff, int nZone,
										CTString strComment, DWORD dwFlag, int limitLv, int nPtType );
	void		AddCharData( int nIndex, int nLvdiff, int nZone, 
										CTString strCharName, int nJob, int limitLv, int nPtType );

	// text scroll
	void		TextScroll( CTString strText );

	// select line
	int			ListSelectLine( int nY );

};

#endif