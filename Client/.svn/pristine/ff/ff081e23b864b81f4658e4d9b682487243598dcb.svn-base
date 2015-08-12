// ----------------------------------------------------------------------------
//  File : UINotice.h
//  Desc : Created by Dongmin
// ----------------------------------------------------------------------------

#ifndef	UINOTICE_H_
#define	UINOTICE_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// #include <vector>
// #include <Engine/Interface/UIButton.h>
// #include <Engine/Interface/UIButtonEx.h>
// #include <Engine/GlobalDefinition.h>

// Define size of notice slot
#if COUNTRY == TAIWAN
	#define	MAX_NOTICE						(3)
	#define CHARGE_BTN_POS					(BTN_SIZE - 2)
#else
	#define	MAX_NOTICE						(4)
	#define CHARGE_BTN_POS					(0)
	#define BILLING_DISABLE
#endif

#define	NOTICE_WIDTH					35
#define	NOTICE_HEIGHT					(BTN_SIZE*MAX_NOTICE)

#define NOTICE_HEIGHT_GAP				(20 + CHILD_QUICKSLOT_HEIGHT * 2)

enum eNoticeType
{
	NOTICE_QUEST	= 0,		// 퀘스트 일경우...
	NOTICE_POLYMOPH,			// 변신 상태에서 되돌아옴.
	NOTICE_EVENT,				// 이벤트.
	NOTICE_TRANSFORM,			// 변신체(강신)
//	NOTICE_AUCTION,				// 경매
	NOTICE_AFFINITY,			// [100322: selo] 친화도 보상
};

// ----------------------------------------------------------------------------
// Name : CUINotice
// Desc :
// ----------------------------------------------------------------------------
class CUINotice : public CUIWindow
{
private:
	struct sNoticeInfo
	{
		int				iIndex;
		eNoticeType		iType;

		bool operator<(const sNoticeInfo &other) const
		{
			if( iType < other.iType )
				return true;
			return false;
		}
		bool operator>(const sNoticeInfo &other) const
		{			
			return other.operator < (*this);
		}
	};

	// Function Object
	struct FindNotice
	{
		int				iIndex;
		eNoticeType		iType;
		FindNotice(sNoticeInfo& info) : iIndex(info.iIndex), iType(info.iType) {};
		bool operator() (const sNoticeInfo& info)
		{
			if( info.iIndex == iIndex && info.iType == iType )
			{
				return true;
			}
			else
				return false;
		}
	};

	std::vector<sNoticeInfo>	m_vectorNoticeList;

	// Slot information
	int					m_nSelNoticeID;					// Selected button ID of quick slot
	BOOL				m_bShowSlotInfo;				// If slot tool tip is shown or not	
	CTString			m_strSlotInfo;					// Slot information string
	UIRect				m_rcSlotInfo;					// Slot information region

	// ToolTip
	UIRectUV			m_rtInfoUL;						// UV of upper left region of information
	UIRectUV			m_rtInfoUM;						// UV of upper middle region of information
	UIRectUV			m_rtInfoUR;						// UV of upper right region of information
	UIRectUV			m_rtInfoML;						// UV of middle left region of information
	UIRectUV			m_rtInfoMM;						// UV of middle middle region of information
	UIRectUV			m_rtInfoMR;						// UV of middle right region of information
	UIRectUV			m_rtInfoLL;						// UV of lower left region of information
	UIRectUV			m_rtInfoLM;						// UV of lower middle region of information
	UIRectUV			m_rtInfoLR;						// UV of lower right region of information

	UIRect				m_rcIcons;						// Action information region

	// UV of each part
	UIRectUV			m_rtBackground;					// UV of background

	CUIButtonEx			m_abtnItems[MAX_NOTICE];

	int					m_nLeftTime;					// 남은 시간	
	TIME				m_tmLeftTime;					// 타이머 용 임시 변수

	// [2011/01/18 : Sora] 출석 이벤트
	BOOL			m_bAttendanceEvent;
private:
	// Internal functions
	void	RenderBtns();
	void	DelNoticeBySlotIndex( int iSlotIndex );
	void	AddBtnNotice( int iNoticeIndex, eNoticeType iNoticeType );		
	void	PressOK( int nSlotIndex );

public:
	CUINotice();
	~CUINotice();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// ToolTip Info
	void	ShowSlotInfo( BOOL bShowInfo, int nSlotIndex = -1 );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	void	MsgBoxLCommand( int nCommandCode, int nResult );

	// Command functions
	void	ENGINE_API AddToNoticeList( int iNoticeIndex, eNoticeType iNoticeType );
	void	ENGINE_API DelFromNoticeList( int iNoticeIndex, eNoticeType iNoticeType );
	void	ENGINE_API RefreshNoticeList();
	void	Clear();

	void	SetLeftTime( int nLeftTime );
	
	// [2011/01/18 : Sora] 출석 이벤트
	void	ShowAttendanceMessage( SLONG attendanceRate );
	void	ShowAttendanceNotice( SLONG attendanceRate, SLONG enchantRate, SLONG expSpRate );
	BOOL	IsAttendanceEventOn() { return m_bAttendanceEvent; }

	// used party item. [4/25/2011 rumist]
	void	ShowUsedPartyItemNotice( const INDEX& nItemIdx, const CTString& strNickName );

	// royal rumble notice [4/26/2011 rumist]
	void	ShowRoyalRumbleNotice( const UBYTE uType );
	void	ShowRoyalRumbleWinner( const INDEX& iLevel, const CTString& strNickName );
	void	ShowRoyalRumbleNextPlayerTime( const INDEX iLeftTime );
};


#endif	// UINOTICE_H_

