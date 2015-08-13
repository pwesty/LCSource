
#ifndef	UIGUILDSTASH_H_
#define	UIGUILDSTASH_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// #include <Engine/Interface/UIWindow.h>
// #include <Engine/Interface/UIButton.h>
// #include <vector>

using namespace std;

struct STakeInfo
{
	int			m_nMonth;
	int			m_nDay;
	LONGLONG	m_llMoney;

	void SetData( int nMonth, int nDay, LONGLONG llMoney )
	{
		m_nMonth	= nMonth;
		m_nDay		= nDay;
		m_llMoney	= llMoney;

	}

};

typedef vector<STakeInfo>	VTakeInfo;


enum eGuilStashState
{
	GSS_NPC,
	GSS_VIEW,
	GSS_TAKE,
};

class CUIGuildStash : public CUIWindow
{
public:

protected:
	
	CTString m_strTitle;

	// Region of each part
	UIRect					m_rcTitle;								// Region of title bar

	// UV of each part
	UIRectUV				m_rtTopL;								// UV of top background
	UIRectUV				m_rtTopM;								// UV of top background
	UIRectUV				m_rtTopR;								// UV of top background
	UIRectUV				m_rtMiddleL;							// UV of middle background
	UIRectUV				m_rtMiddleM;							// UV of middle background
	UIRectUV				m_rtMiddleR;							// UV of middle background
	UIRectUV				m_rtMiddleGapL;							// UV of middle gap background
	UIRectUV				m_rtMiddleGapM;							// UV of middle gap background
	UIRectUV				m_rtMiddleGapR;							// UV of middle gap background
	UIRectUV				m_rtBottomL;							// UV of bottom background
	UIRectUV				m_rtBottomM;							// UV of bottom background
	UIRectUV				m_rtBottomR;							// UV of bottom background

	// Table
	UIRectUV3				m_rtTableTitle;							// UV of top background
	UIRectUV3				m_rtTableMiddle;						// UV of top background
	UIRectUV3				m_rtTableBottom;						// UV of top background
	
	UIRectUV				m_rtTableGrid;

		
	// Properties
	CTString			strTitle;
	eGuilStashState		GameState;
	CUIButton			m_btnOk;
	
	LONGLONG			m_llBalance;		// Balance;

	VTakeInfo			m_vTakeInfo;


public:
	CUIGuildStash();
	~CUIGuildStash();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();
	void	RenderTake();	// 출금 창 렌더 
	void	RenderTable();


	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );
	
	ENGINE_API void Message( int nCommandCode, CTString strMessage, DWORD dwStyle );

	// Open
	ENGINE_API void	OpenGuildStash();
	void			OpenGuildStash_N();

	void OpenTake( LONGLONG nBalance );
	void OpenView();

	void	CloseStash();

	void AddTakeInfo( int nMonth, int nDay, LONGLONG llMoney );
	void ResetTakeInfo();
	
	void ErrorProc( LONG lErrorCode, UBYTE ubType );
	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );	
	
	
};

class CUINewGuildStash : public CUIWindow
{
// 전처리 [6/16/2011 rumist]
private:
	//2013/04/02 jeil 나스 아이템 제거 
	BOOL					m_bIsTake;								//보관인지 찾는것인지 결정 T 가 찾는것 
	typedef	void (CUINewGuildStash::*pReq)();
	typedef void (CUINewGuildStash::*pRender)();
	typedef WMSG_RESULT	(CUINewGuildStash::*pMsgProc)( MSG* pMsg );
	enum __tagGuildStashMode
	{
		GSMODE_DUTY,
		GSMODE_KEEP,
		GSMODE_TAKE,
		GSMODE_LOG,	
	};

	enum __tagGuildStashItem
	{
		GSITEM_COL = 5,
		GSITEM_ROW = 4,
	};

	enum __tagGuildStashUI
	{
		GSUI_TOP_H				= 22,
		GSUI_NOTICE_H			= 32,
		GSUI_STASH_H			= 151,
		GSUI_TAKE_H				= 40,
		GSUI_SEPARATOR_H		= 3,
		GSUI_BOTTOM_H			= 33,
		GSUI_STASH_WIDTH		= 216,
		GSUI_STASH_HEIGHT		= GSUI_TOP_H+GSUI_NOTICE_H+GSUI_SEPARATOR_H+GSUI_STASH_H+GSUI_SEPARATOR_H+GSUI_TAKE_H+GSUI_SEPARATOR_H+GSUI_BOTTOM_H,
		GSUI_LOG_WIDTH			= 600,
		GSUI_LOG_HEIGHT			= 200,
		GSUI_LOG_COLUMN1_SIZE	= 140,
		GSUI_LOG_COLUMN2_SIZE	= 100,
		GSUI_LOG_COLUMN3_SIZE	= 90,
		GSUI_LOG_COLUMN4_SIZE	= 160,
		GSUI_LOG_COLUMN5_SIZE	= 74,
		GSUI_LOG_COLUMN1_W		= GSUI_LOG_COLUMN1_SIZE,
		GSUI_LOG_COLUMN2_W		= GSUI_LOG_COLUMN1_W+GSUI_LOG_COLUMN2_SIZE-1,
		GSUI_LOG_COLUMN3_W		= GSUI_LOG_COLUMN2_W+GSUI_LOG_COLUMN3_SIZE-1,
		GSUI_LOG_COLUMN4_W		= GSUI_LOG_COLUMN3_W+GSUI_LOG_COLUMN4_SIZE-1,
		GSUI_LOG_COLUMN5_W		= GSUI_LOG_COLUMN4_W+GSUI_LOG_COLUMN5_SIZE-1,
		GSUI_LOG_COLUMN_GAP		= 36,
		GSUI_LOG_BOTTOM_GAP		= 61,
	};

	enum __tagGuildStashLogCulumnType
	{
		GSLOG_COLUMN_TIME,
		GSLOG_COLUMN_USERNAME,
		GSLOG_COLUMN_ACTION,
		GSLOG_COLUMN_ITEMNAME,
		GSLOG_COLUMN_ITEMCOUNT,
	};
	
	enum __tagGuildStashNetError
	{
		GSERR_SUCCESS,
		GSERR_NO_GUILD,
		GSERR_INVEN_FULL,
		GSERR_WEIGHT_FULL,
		GSERR_USED,
		GSERR_DONT_KEEP_ITEM,
		GSERR_NO_PERMISSION,
		GSERR_NO_GUILD_SKILL,
		GSERR_CREATE_FAIL,
		GSERR_STASH_FULL,	// [2012/06/01 : Sora] ITS 9436 길드창고 공간 부족 메시지 추가
	};

private:
	void							_openStashView( __tagGuildStashMode _mode = GSMODE_TAKE );
	void							_closeStashView();

	void							_initStashUI();
	void							_initLogUI();
	void							_initStashItemBtn();
	void							_initTakeItemBtn();

	void							_updateItemInfoInStash();

	void							_createStashUI();
	void							_createLogUI();

	void							_renderStashView();
	void							_renderStashLog();
	void							_renderDummy()														{						}

	const bool						_isVaildateData();

	void							_enableProcessButton(BOOL bEnable)									
									{ m_btnProcess.SetEnable(bEnable);	}

	WMSG_RESULT						_mouseStashViewMsg( MSG *pMsg );
	WMSG_RESULT						_mouseStashLogMsg( MSG *pMsg );
	WMSG_RESULT						_mouseStashDummyMsg( MSG *pMsg )									{ return WMSG_FAIL;		}

	void							_showNetErrorMsgBox( int _error );
	void							_showErrorMsgBox( int _error );

	void							_setLogData( INDEX& iLogIdx, CTString& strLogTime, CTString& strUserName, SBYTE& sbAction, INDEX& iItemIdx, INDEX& iItemPlus, SQUAD& sqItemCnt );
	void							_addItemToBasket( int nRow, int nCol, int nUniIndex, SQUAD llCount );
	void							_copyItemToBasket();
	void							_delItemInBasket(INDEX pos);
	void							clearItemInfo();
protected:
public:
	CUINewGuildStash();
	~CUINewGuildStash();

	// Create
	void							Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void							Render();
	
	// Adjust position
	void							ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void							AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void							MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void							MsgBoxLCommand( int nCommandCode, int nResult );
	
	// Open
	ENGINE_API void					OpenGuildStash();
	void							CloseStash();
		// Messages
	WMSG_RESULT						MouseMessage( MSG *pMsg );	

	void							ReceiveGuildStashMessage( UBYTE ubType, CNetworkMessage* istr );
	void							SendGuildStashListReq();
	void							SendGuildStashKeepReq();
	void							SendGuildStashTakeReq();
	void							SendGuildStashLogReq();

	//2013/04/02 jeil 나스 아이템 제거
	void							InNas();
	void							OutNas();
	void							SendGuildStashListReqNas();

	//
	void							GuildStashDoesMessageBoxExist();
	void							AddItemCallback();
private:
	pReq							m_fnReq;								// function pointer.
	pRender							m_fnRender;
	pMsgProc						m_fnMsgProc;
protected:
	CTextureData*					m_ptdStashTexture;
	CTString						m_strTitle;

	//2013/04/02 jeil 나스 아이템 제거 
	CTString						m_strTotalNas;	//보관 금액
	CTString						m_strInNas;	//입금 금액
	CTString						m_strOutNas;	//출금 금액

	// Region of each part
 	UIRect							m_rcTitle;								// Region of title bar
	UIRect							m_rcStash;
	UIRect							m_rcTake;
 
	// UV of each part
	// stash part
	UIRectUV						m_rtTop;
	UIRectUV						m_rtNotice;
	UIRectUV						m_rtStash;
	UIRectUV						m_rtTake;
	UIRectUV						m_rtSeparator;
	UIRectUV						m_rtBottom;
	UIRectUV						m_rtSelected;
	UIRectUV						m_rtInTake;

	//2013/04/02 jeil 나스 아이템 제거
	UIRectUV						m_rtNas;

	// log part
	UIRectUV						m_rtLogTop;
	CUIRectSurface					m_rtLogSurface;


	CUIButton						m_btnClose;
	CUIButton						m_btnProcess;

	//2013/04/02 jeil 나스 아이템 제거
	CUIButton						m_btnNas;		//입금 
	


	CUIButton						m_btnCloseX;
	CUIListBox						m_lbLog;

	CUIScrollBar					m_sbStash;
	CUIScrollBar					m_sbTake;

	CUIIcon*						m_pIconsStashItem[GSITEM_ROW][GSITEM_COL];
	CUIIcon*						m_pIconsTakeItem[GSITEM_COL];
	CItems*							m_pItemsDummy;

	__tagGuildStashMode				m_enMode;

	// data of stash part.
	std::vector< CItems* >			m_vecGuildItemInfo;
	int								m_nStartLineInStash;
	SBYTE							m_sbSelRowInStash;
	SBYTE							m_sbSelColInStash;
	SBYTE							m_sbSelColInTake;
	INDEX							m_iStashTotalPage;
	INDEX							m_iStashCurrentPage;
	CTString						m_strLeftUsedTime;
	INDEX							m_iStashCapacity;
	INDEX							m_iLastLogIdx;


	//2013/04/05 jeil 나스 아이템 제거
	__int64				m_nInNas;
	__int64				m_nOutNas;
	__int64				m_nTotalNas;
};

#endif	// UIGUILDSTASH_H_


