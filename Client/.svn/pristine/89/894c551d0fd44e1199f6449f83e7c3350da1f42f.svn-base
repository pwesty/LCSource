// ----------------------------------------------------------------------------
//  File : UICharServerMove.h
//  Desc : Create by 
// ----------------------------------------------------------------------------

#ifndef	UI_CHAR_SERVERMOVE_H_
#define	UI_CHAR_SERVERMOVE_H_
#ifdef	PRAGMA_ONCE
#pragma once
#endif
 
#define SERVER_TRANS_ERROR_SUCCESS			(0 << 0)
#define	SERVER_TRANS_ERROR_EXIST_GUILD		(1 << 0)		//	길드가 있는 경우
#define	SERVER_TRANS_ERROR_EXIST_GUARDIAN	(1 << 1)		//	후견인이 있는 경우
#define	SERVER_TRANS_ERROR_30DAYS			(1 << 2)		//	사용한지 30일이 안된경우
#define	SERVER_TRANS_ERROR_BLOCK_SERVER		(1 << 3)		//	서버가 블록 상태일 경우
#define	SERVER_TRANS_ERROR_INVALID_SERVER	(1 << 4)
#define SERVER_TRANS_ERROR_DB_INSERT		(1 << 5)
#define SERVER_TRANS_ERROR_NOT_EXIST_ITEM	(1 << 6)		//  사용중 아이템 이동
#define SERVER_TRANS_ERROR_ROLLBACK_SUCCESS (1 << 7)		//  롤백 성공



class CUICharServerMove : public CUIWindow
{
private:
	enum __tag_CharServerMove_UI
	{

		CSM_UI_GAP_POS_LEFT		= 4,
		CSM_UI_GAP_POS_TOP		= 2,
		CSM_UI_GAP_POS_RIGHT	= 2,
		CSM_UI_GAP_POS_BOTTOM	= 4,

		CSM_UI_WIDTH			= 216,
		CSM_UI_TITLE_POSX		= 0,
		CSM_UI_TITLE_POSY		= 0,
		CSM_UI_TITLE_WIDTH		= CSM_UI_WIDTH,
		CSM_UI_TITLE_HEIGHT		= 22,
		CSM_UI_MIDDLE_BOX_POSX	= CSM_UI_TITLE_POSX,
		CSM_UI_MIDDLE_BOX_POSY	= CSM_UI_TITLE_HEIGHT,
		CSM_UI_MIDDLE_BOX_WIDTH = CSM_UI_WIDTH,
		CSM_UI_MIDDLE_BOX_HEIGHT = 230,
		CSM_UI_BOTTOM_BOX_POSX	= CSM_UI_MIDDLE_BOX_POSX,
		CSM_UI_BOTTOM_BOX_POSY	= CSM_UI_MIDDLE_BOX_POSY+CSM_UI_MIDDLE_BOX_HEIGHT,
		CSM_UI_BOTTOM_BOX_WIDTH = CSM_UI_WIDTH,
		CSM_UI_BOTTOM_BOX_HEIGHT = 15,

		CSM_UI_LINEBOX_TOP_POSX = CSM_UI_GAP_POS_LEFT,
		CSM_UI_LINEBOX_TOP_POSY = CSM_UI_TITLE_HEIGHT + CSM_UI_GAP_POS_TOP,
		CSM_UI_LINEBOX_TOP_WIDTH = CSM_UI_WIDTH - CSM_UI_GAP_POS_LEFT - CSM_UI_GAP_POS_RIGHT,
		CSM_UI_LINEBOX_TOP_HEIGHT = 2,
		CSM_UI_LINEBOX_BOTTOM_HEIGHT = 2,

		CSM_UI_LINEBOX_MIDDLE_POSX = CSM_UI_LINEBOX_TOP_POSX,
		CSM_UI_LINEBOX_MIDDLE_POSY = CSM_UI_LINEBOX_TOP_POSY + CSM_UI_LINEBOX_TOP_HEIGHT,
		CSM_UI_LINEBOX_MIDDLE_WIDTH = CSM_UI_LINEBOX_TOP_WIDTH,
		CSM_UI_LINEBOX_MIDDLE_HEIGHT = CSM_UI_MIDDLE_BOX_HEIGHT - CSM_UI_LINEBOX_TOP_HEIGHT - CSM_UI_LINEBOX_BOTTOM_HEIGHT - CSM_UI_GAP_POS_BOTTOM - CSM_UI_GAP_POS_TOP - 30,
	
		CSM_UI_LINEBOX_BOTTOM_POSX = CSM_UI_LINEBOX_TOP_POSX,
		CSM_UI_LINEBOX_BOTTOM_POSY = CSM_UI_LINEBOX_MIDDLE_POSY + CSM_UI_LINEBOX_MIDDLE_HEIGHT,
		CSM_UI_LINEBOX_BOTTOM_WIDTH = CSM_UI_LINEBOX_MIDDLE_WIDTH,

 		CSM_UI_SUB_TITLE_WIDTH	= CSM_UI_LINEBOX_TOP_WIDTH  , 
 		CSM_UI_SUB_TITLE_POSX	= CSM_UI_LINEBOX_TOP_POSX ,
 		CSM_UI_SUB_TITLE_POSY	= CSM_UI_LINEBOX_TOP_POSY ,
 		CSM_UI_SUB_TITLE_HEIGHT	= 31,
  		
 		CSM_UI_LIST_BOX_POSX	= CSM_UI_SUB_TITLE_POSX,
 		CSM_UI_LIST_BOX_POSY	= CSM_UI_SUB_TITLE_POSY + CSM_UI_SUB_TITLE_HEIGHT + CSM_UI_GAP_POS_TOP,
		CSM_UI_LIST_BOX_WIDTH	= CSM_UI_SUB_TITLE_WIDTH,
 		CSM_UI_LIST_BOX_HEIGHT	= CSM_UI_LINEBOX_MIDDLE_HEIGHT - CSM_UI_SUB_TITLE_HEIGHT - CSM_UI_GAP_POS_TOP,
 		
		CSM_UI_POSX				= 100,
		CSM_UI_POSY				= 100,	
		CSM_UI_HEIGHT			= CSM_UI_TITLE_HEIGHT+CSM_UI_MIDDLE_BOX_HEIGHT+CSM_UI_BOTTOM_BOX_HEIGHT,
 
		CSM_UI_BUTTON_GAP_FROM_BOTTOM = 15,
		CSM_UI_BUTTON_GAP_FROM_WIDTH = 7,
		CSM_UI_BUTTON_OK_WIDTH = 53,
		CSM_UI_BUTTON_OK_HEIGHT = 21,
		CSM_UI_BUTTON_CANCEL_WIDTH = 53,
		CSM_UI_BUTTON_CANCEL_HEIGHT = 21,

		CSM_UI_BUTTON_OK_POSY	= CSM_UI_HEIGHT-CSM_UI_BUTTON_GAP_FROM_BOTTOM-CSM_UI_BUTTON_OK_HEIGHT,
		CSM_UI_BUTTON_CANCEL_POSY = CSM_UI_HEIGHT-CSM_UI_BUTTON_GAP_FROM_BOTTOM-CSM_UI_BUTTON_CANCEL_HEIGHT,
		
		CSM_UI_BUTTON_CANCEL_POSX = CSM_UI_WIDTH-CSM_UI_BUTTON_GAP_FROM_WIDTH-CSM_UI_BUTTON_CANCEL_WIDTH,
		CSM_UI_BUTTON_OK_POSX	= CSM_UI_BUTTON_CANCEL_POSX-5-CSM_UI_BUTTON_OK_WIDTH,
		
		CSM_UI_SCROLL_SIZEX		=	9,
		CSM_UI_SCROLL_GAPX		=	CSM_UI_SCROLL_SIZEX + CSM_UI_GAP_POS_RIGHT + CSM_UI_GAP_POS_LEFT,
	};

	enum __tag_CharServerMove_ErrorType
	{
		CHAR_SERVERMOVE_EXIST_GUILD = 0,
		CHAR_SERVERMOVE_EXIST_GUARDIAN,
		CHAR_SERVERMOVE_300AYS,
		CHAR_SERVERMOVE_BLOCK_SERVER,
		CHAR_SERVERMOVE_INVALID_SERVER,
		CHAR_SERVERMOVE_DB_INSERT,
		CHAR_SERVERMOVE_NOT_EXIST_ITEM,
		CHAR_SERVERMOVE_ROLLBACK_SUCCESS
	};
	// methods.
protected:
	void			_openUI(const bool bType);
	void			_closeUI();
	void			AddReadingString( CUIListBox& lbList, CTString &strContent, COLOR colContent, int iMaxChar = 30 );

public:
	// interface
	void			ToggleVisible(const bool bResultView = false);		// false : select server list view. // true : success or failed result view.

	void			AddServerListForCharMove(CTString strServerName, ULONG ulServerNumber); 
	void			AddToServerListForRender();
	void			ResetServerList();
		
	void			SetItemIndex(ULONG ulItemIndex)		{ m_ulItemIndex = ulItemIndex; } 
	int				GetServerNo(int nCurrSelList);	
	CTString		GetServerName(int nServerNo);
	void			GetErrorMessage(int nErrorCode, int nCount);	// Get Error string [7/25/2012 Ranma]

public:
	CUICharServerMove();
	~CUICharServerMove();

	void			Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void			Render();
	WMSG_RESULT		MouseMessage( MSG *pMsg );
	void			MsgBoxCommand(int nCommandCode, BOOL bOK, CTString &strInput );
	virtual BOOL	CloseWindowByEsc()			{ _closeUI();	return TRUE; }

	// server to client message procedure.
	void			RecvCharServerMoveResult(CNetworkMessage* istr);
private:
	// network message.
	void			SendCharServerMoveItem(); 

	// attributes.
private:
	CUIRectSurface			m_rsBackground; 
	
	UIRect					m_rtTitleBar;	
	UIRect					m_rtSubTitleBar;
	UIRectUV				m_rtSubTitleBarUV;
	
	CUIButton				m_btnClose;		
	CUIButton				m_btnOK;		

	CTString				m_strTitle;		
	CTString				m_strServerSelectTitle; 

	CUIListBox				m_SelectContentListBox;
	CUIListBox				m_OnceContentListBox;
	CUIListBox*				m_TargetListBox;

	ULONG					m_ulCurSelServerNo; // 유저가 선택한 서버군 번호
	ULONG					m_ulItemIndex;		// 아이템 인덱스

	// SubTitle 추가 할지 여부 [7/20/2012 Ranma]
	bool					m_bSubTitle;

	// 버튼 하나 그릴지 두개 그릴지 여부 [7/23/2012 Ranma]
	bool					m_bBtnTwin;

	// 타이틀 클릭 했는지 여부 [7/23/2012 Ranma]
	BOOL					m_bPickTitle;

	// 서버군의 정보와 이름을 저장할 구조체 [7/24/2012 Ranma]
	struct sServerInfo 
	{
		CTString	m_strServerName;
		ULONG		m_ServerNumber;
		
		sServerInfo()
		{
			m_strServerName = CTString("");
			m_ServerNumber = -1;
		};
		~sServerInfo()
		{
			
		};
	};
	std::vector<sServerInfo>	m_vectorServerInfo;

public : 

protected :
	void initialize();
	void OnUpdate( float fElapsedTime );
	void OnRender( CDrawPort* pDraw );
};


#endif	// CUICharServerMove

