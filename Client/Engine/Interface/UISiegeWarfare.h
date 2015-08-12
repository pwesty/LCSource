
#ifndef _UISIEGE_WARFARE_H
#define _UISIEGE_WARFARE_H

#include <vector>
#include <Engine/Interface/UIWindow.h>
#include <Engine/Interface/UISpinButton.h>

class CUISiegeWarfare : public CUIWindow
{
// Control
	// 타이틀 
	CTString				m_strTitle;
	UIRect					m_rcTitle;

	// 공성 시간 설정시 사용
	CUIButton				m_btnOK;				
	CUIButton				m_btnClose;
	CUIButton				m_btnCancel;
	CUICheckButton			m_cbtnSaturday;				// 공성 일 설정
	CUICheckButton			m_cbtnSunday;
	CUISpinButton			m_sbtnSWTime;				// 공성 시간 설정
	CUIListBox				m_lbDesc;					// 공성 가능 시간
		
	// Back UV
	UIRectUV3				m_rt3BackT;								
	UIRectUV3				m_rt3BackSM;							
	UIRectUV3				m_rt3BackNM;
	UIRectUV3				m_rt3BackM;	
	UIRectUV3				m_rt3BackL;	

	// BattleBack UV
	UIRectUV				m_rtBattleBack;
	UIRectUV				m_rtSelectBattleBack;

	// Number UV
	UIRectUV				m_rtNumber[11];
	UIRectUV				m_rtColon;
	
	// Notice 
	UIRect					m_rcNotice1;				// Region of notice1 (위 쪽)
	UIRect					m_rcNotice2;				// Region of notice2 (아랫 쪽)
	
	UIRectUV				m_rtNoticeL;				// UV of notice left region
	UIRectUV				m_rtNoticeC;				// UV of notice center region
	UIRectUV				m_rtNoticeR;				// UV of notice right region
	
	//Notice Info
	TIME					m_tmNoticeTime;				// 공지 사항 표시 시간
	BOOL					m_bShowNotice;				// 공지 사항을 보여주고 있는 가
	CTString				m_strNoticeMessage;			// 공자 사항 1 스트링 (위)
	CTString				m_strNoticeMessage2;		// 공지 사항 2 스트링 (아래)

	CUISpinButton			m_sbtnSWZone;				// [2010/11/01 : Sora] 공성 개편
public:	
	CUISiegeWarfare();
	virtual ~CUISiegeWarfare();
	
	void	Clear();
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();
	void	RenderRectUV3( int nUnitSize, int nX, int nY, int nWidth, int nHeight, UIRectUV3 rtRectUV3 );
	
	// Adjust position
	void	ResetPos( BOOL bCenter = TRUE );
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Message
	WMSG_RESULT MouseMessage( MSG *pMsg );

	// Open SiegeWarfare
	void	OpenSiegeWarfare();
	void	OpenSWReq();
	void	OpenMercearReq();
	void	OpenSetSWTime();
	void	OpenSWInfo();
	void	OpenSelectBattle();
	void	OpenBattle();
	
	// Close
	void	CloseSiegeWarfare();	
	
	// Render
	void	RenderNotice();
	void	RenderSetSWTime();
	void	RenderBattle();
	void	RenderPoint( int nX, int nY, int nPoint );
	void	DrawNumber( int nX, int nY, int nNumber, int nWidth = 16 , int nHeight = 19 );
	void	RenderLeftTime();
	
	// MessageBox
	void	CloseAllMsgBox();
	void	MsgBoxLCommand( int nCommandCode, int nResult );
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	ErrorMessage( CTString strErrorMessage );					// 에러 에디트 박스
	void	Message( int nCommandCode, CTString strMessage, DWORD dwStyle );	// 메세지 박스
	
	// Etc
	void	SetNotice( CTString strNoticeMessage, CTString strNoticeMessage2 = " " );
	BOOL	m_bBottomNotice;

	void	SetDayOfWeek( int eDayOfWeek );

	void	OpenCostumeReq();

	// [2010/11/01 : Sora] 공성 개편
	struct stSiegeWarfareTimeInfo
	{
		UBYTE zone;
		UBYTE wday;
		UBYTE hour;

		stSiegeWarfareTimeInfo()
		{
			Init();
		}

		void Init()
		{
			zone = 0;
			wday = 0;
			hour = 0;
		}
	};

	std::vector<stSiegeWarfareTimeInfo> m_vecSWInfo;
	
	void	InitSiegeWarfareTimeInfo() { m_vecSWInfo.clear(); }
	void	SetSiegeWarfareTimeInfo( stSiegeWarfareTimeInfo swInfo );
	void	OpenSWInfoRenew();
};

// [2010/11/01 : Sora] 공성 개편
class CUISiegeWarfareInfo : public CUIWindow
{
protected:
	UIRect			m_rcTitle;
	CUIDrawBox		m_bxBackground[4];
	CUIButton		m_btnOK;
	
	CUIRectString	m_rsMyGuildName;
	CUIRectString	m_rsMyGuildBossName;
public:
	struct stSiegeWarfareDefGuildInfo // 수성길드 정보
	{
		char zone;
		char month;
		char day;
		char hour;
		CTString guildName;

		stSiegeWarfareDefGuildInfo()
		{
			Init();
		}

		void Init()
		{
			zone = 0;
			month = 0;
			day = 0;
			hour = 0;
			guildName = "";
		}
	};

	struct stMySiegeWarfareInfo	// 내 공성 정보
	{
		std::vector<char> vecZone;
		std::vector<char> vecJoinFlag;
		CTString guildName;
		CTString guildBossName;

		stMySiegeWarfareInfo()
		{
			Init();
		}

		void Init()
		{
			vecZone.clear();
			vecJoinFlag.clear();
			guildName = "";
			guildBossName = "";
		}
	};

	stMySiegeWarfareInfo m_MySiegeWarfareInfo;
	std::vector<stSiegeWarfareDefGuildInfo> m_vecDefGuildInfo;

	CUISiegeWarfareInfo();
	~CUISiegeWarfareInfo();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	
	// Render
	void	Render();
	
	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	WMSG_RESULT MouseMessage( MSG *pMsg );

	void	InitSiegeWarfareInfo();
	void	SetSiegeWarfareDefGuildInfo( stSiegeWarfareDefGuildInfo defGuildInfo );
	void	SetMySiegeWarfareGuildInfo( CTString guildName, CTString guildBossName );
	void	SetMySiegeWarfareJoinInfo(  char zone, char joinFlag );
	void	ToggleSiegeWarfareInfo( BOOL bShown );
	CTString	GetJoinStatusString( char joinFlag );
};


#endif // _UISIEGE_WARFARE_H
