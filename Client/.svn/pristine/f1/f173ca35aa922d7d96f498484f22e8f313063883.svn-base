
#ifndef	UIGUILD_BATTLE_H_
#define	UIGUILD_BATTLE_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// Include Files
// #include <Engine/Interface/UIWindow.h>
// #include <Engine/Interface/UIButton.h>
// #include <Engine/Interface/UIListBox.h>
// #include <Engine/Interface/UIEditBox.h>
// #include <algorithm>
// #include <map>
// #include <Engine/Effect/CEffectGroupManager.h>
// #include <Engine/Effect/CEffectGroup.h>

// Define size of Guild Battle
#define	GB_WIDTH						311		// size of Guild Battle 신청창의 너비
#define	GB_HEIGHT						220		// size of Guild Battle 신청창의 높이

#define GB_MIN_MEMBER					10		// 길드 전투 최소 인원

//------------------------------------------------------------------------------
// CUIGuildBattle
// Explain:  길드 전투 
// Date : 2005-03-17,Author: Lee Ki-hwan
// History: 2005. 4. 4	: Test Server Open
//------------------------------------------------------------------------------
class ENGINE_API CUIGuildBattle : public CUIWindow
{
protected:	

	// Guild Battle State : 길드 전투의 상태 
	enum eGBState
	{
		GBS_NONE,			// 초기 상태
		GBS_REQ,			// 길드 전투 요청
		GBS_IN_REQ,			// 길드 전투 요청 중( 요청후 응답을 기다리는 상태 )
		GBS_IN_REQ_ACCEPT,	// 길드 전투 수락 중( 길드전 성립 후 대기 상태 : 5분 ) 		
		GBS_IN_BATTLE,		// 길드 전투 중
		GBS_BATTILE_NOT_ZONE, // 길드 전투 존이 아니다.
		GBS_END				// 길드 전투 종료 
	};
	eGBState				m_eGBState;

// 길드 신청
	// Use m_ptdBaseTexture
	// Controls	
	CUIButton				m_btnOK;						// OK button
	CUIButton				m_btnCancel;					// Cancel button
	CUIListBox				m_lbGBReqdDesc;					// 길드 신청에 관한 설명
	CUIEditBox				m_ebGuildReqNas;				// 길드 신청 금액 입력 ( 만 나스 단위 )
	CUIEditBox				m_ebGuildReqTime;				// 길드 신청 시간 ( 분단위 )
		
	// Region of each part
	UIRect					m_rcTitle;						// Region of title bar
	
	// UV of each part
	UIRectUV				m_rtBackground;					// UV of background		
	UIRectUV				m_rtBackTop;					// UV of top background
	UIRectUV				m_rtBackMiddle1;				// UV of middle background
	UIRectUV				m_rtBackMiddle2;				// UV of middle background
	UIRectUV				m_rtBackBottom;					// UV of bottom background
		
	UIRectUV				m_rtInputBoxL;					// UV of left region of input box
	UIRectUV				m_rtInputBoxM;					// UV of middle region of input box
	UIRectUV				m_rtInputBoxR;					// UV of right region of input box

// 길드 상태 표시 
	CTextureData*			m_ptdStateTexture;
	
	UIRect					m_rcBox;						// 시간 표시 창 (배경)
	UIRect					m_rcRedBox;						// RedBox : KillCount
	UIRect					m_rcBlueBox;					// BlueBox : KillCount
		
	UIRectUV				m_rtBox;						
	UIRectUV				m_rtBlueBox;					// BlueBox : KillCount
	UIRectUV				m_rtRedBox;						// RedBox : KillCount
	
	UIRectUV				m_rtSmallNumber[10];			// 시간 표시 용 숫자
	UIRectUV				m_rtLargeNumber[10];			// KillCount 표시 용 숫자 
	UIRectUV				m_rtColon;						// 시간 표시 용 :(콜론)
	
// Notice 
	UIRect					m_rcNotice1;					// Region of notice1 (위 쪽)
	UIRect					m_rcNotice2;					// Region of notice2 (아랫 쪽)
	
	UIRectUV				m_rtNoticeL;					// UV of notice left region
	UIRectUV				m_rtNoticeC;					// UV of notice center region
	UIRectUV				m_rtNoticeR;					// UV of notice right region
	
// Info 
	//Notice
	TIME					m_tmNoticeTime;					// 공지 사항 표시 시간
	BOOL					m_bShowNotice;					// 공지 사항을 보여주고 있는 가
	CTString				m_strNoticeMessage;				// 공자 사항 1 스트링 (위)
	CTString				m_strNoticeMessage2;			// 공지 사항 2 스트링 (아래)
	
	// 내 길드 정보 ( Guild에도 저장되어 있지만.. 그냥 처음에 저장해서 쓰자 )
	CTString				m_strGuildName;					// 길드 이름
	int						m_nCharIndex;					// 길드장의 캐릭터 Index (길드장만이 가지고 있음)
	int						m_nGuildIndex;					// 길드 Index 
	int						m_nKillPoint;					// Kill Point
	
	//상대 편 길드의 정보
	CTString				m_strTargetGuildName;			// 상대편 길드 이름
	int						m_nTargetCharIndex;				// 상대편 길드장의 Index (길드장만이 가지고 있음)
	int						m_nTargetGuildIndex;			// 상대편 길드 Index 
	int						m_nTargetKillPoint;				// 상대편의 KillPoint
	
	// Battle Status
	LONG					m_lLeftTime;					// 현재 남은 시간 (1분마다, 혹은 KillPoint변경시 갱신)
	int						m_nBattleZone;					// 길드 전투 지역 
	int						m_nPrize;						// 배팅 나스

	// etc
	CTString				m_strTitle;						// 타이틀 이름 (길드전투 관련 메세지 뿌릴때 사용)
	TIME					m_tmLeftTime;					// 타이머 용 임시 변수
	
	std::map<SLONG, CEffectGroup *> m_mapEG;
public:

	CUIGuildBattle();
	virtual ~CUIGuildBattle();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	Clear();	// All Data Clear ( EditBox는 제외 : 쩝.. 에러난당.. )
	void	Close();	// 종료

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	
	// Set focus
	void	SetFocus( BOOL bVisible );
	
	// Edit box focus
	BOOL	IsEditBoxFocused() { return m_ebGuildReqNas.IsFocused();	}
	void	KillFocusEditBox() { m_ebGuildReqNas.SetFocus( FALSE );		}

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	WMSG_RESULT	KeyMessage( MSG *pMsg );
	WMSG_RESULT	IMEMessage( MSG *pMsg );
	WMSG_RESULT	CharMessage( MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );

	// Render
	void	Render();					// Render
	void	RenderGBReq();				// 길드 전투 신청창 
	void	RenderGBStatus();			// 길드 전투 상태창
	void	DrawNumber( int x, int y, int nNumber, bool bLarge = false );	// 숫자
	void	RenderLeftTime();			// 남은 시간
	void	DrawColon( int x, int y );	// Colon Render

	void	RenderKillPoint();			// Render Kill Point
	void	RenderKillPointRed();		// 상대 편 
	void	RenderKillPointBlue();		// 우리 편
		
	// 길드 전투 신청
	void	OpenGBReq( int nTargetCharIndex, CTString strGuildName ); // 길드 신청 창
	void	AddGBReqdDescString( CTString &strDesc, COLOR colDesc = 0xF2F2F2FF );
			
	// NetWork Message
	void	GBReq();
	void	GBReq( int nGuildIndex, CTString strGuildName, int nPrize, int nTime );
	void	GBReqReject( int nRejectCharIndex );
	void	GBReqAccept( int nGuildIndex1, CTString strGuildName1, int nGuildIndex2, CTString strGuildName2, int nPrize, int nZone, int Time );
	void	GBStopReq( int nTargetCharIndex, CTString strGuildName );
	void	GBStopReqRev( int nGuildIndex, CTString strGuildName );
	void	GBStopReqReject( int nRejectCharIndex );
	void	GBStopReqAccept();	
	void	GBStart( int nGuildIndex1, CTString strGuildName1, int nGuildIndex2, CTString strGuildName2, int nPrize, int nZone, int nTime );	
	void	GBStatus( int nGuildIndex1, CTString strGuildName1, int nCount1, int nGuildIndex2, CTString strGuildName2, int nCount2, int Time, int nZone );
	void	GBEnd( int nWinnerGuildIndex, int nGuildIndex1, CTString strGuildName1, int nGuildIndex2, CTString strGuildName2, int nPrize );
	
	// 에러 처리
	void	GBErrorProcess ( int nErrorCode ); 

	// MessageBox
	void	CloseAllMsgBox();	// 모든 메세지 창 제거
	void	GBErrorMessage( int nCommandCode, CTString strErrorMessage );	// 에러 에디트 박스
	void	GBMessage( int nCommandCode, CTString strMessage, DWORD dwStyle ); // 메세지 박스
		
	BOOL	IsInBattle();
	BOOL	IsGBReq()	{ if( m_eGBState == GBS_REQ )	return TRUE; return FALSE; }
		
	// 공지 창 뿌리기
	void	SetNotice ( CTString strNoticeMessage, CTString m_strNoticeMessage2 );

	// Guild Battle Effect
	void	StartGuildEffect ( SLONG slCharIndex, CEntity* penEntity, BOOL bEnemy = TRUE );
	void	StopGuildEffect ( SLONG slCharIndex );
	void	StartEffect();
	void	StopEffect();
	
	// 적인지 확인 ( 전투 존 인지도 확인 합니다. )
	BOOL	IsEnemy( unsigned long ulID );
	int		IsEnemyGuild ( unsigned long ulGuildIndex );
	
};

#endif	// UIGB_H_