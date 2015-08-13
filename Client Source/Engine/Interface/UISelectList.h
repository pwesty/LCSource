// ----------------------------------------------------------------------------
//  File : UISelectList.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	UISELECTLIST_H_
#define	UISELECTLIST_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


// #include <Engine/Interface/UIButton.h>
// #include <Engine/Interface/UICheckButton.h>
// #include <Engine/Interface/UIEditBox.h>

// SelectList Style
#define SLS_OK			0x0001
#define SLS_CANCEL		0x0002
#define SLS_PREV		0x0004 
#define SLS_NEXT		0x0008 
#define SLS_REGIST		0x0010
#define SLS_REWARD		0x0020

// Define size of skill learn
#define	SELECTLIST_WIDTH		240
#define	SELECTLIST_HEIGHT		290

struct SEL_LIST {
	INDEX		m_nIndex;
	CTString	m_sName;
	INDEX		m_nState;
};

enum LIST_STATE {
	LS_WAIT,
	LS_REGISTER,
	LS_REWARD,
	LS_REWARDX,
	LS_RECEIVED,
	LS_END,
};

typedef std::vector<SEL_LIST> vSEL_LIST;

// ----------------------------------------------------------------------------
// Name : CUISelectList
// Desc :
// ----------------------------------------------------------------------------
class CUISelectList : public CUIWindow
{

protected:
	
	CTString		m_strTitle;						// Title of message box
	CTString*		m_pstrState;						// state string
	
	// Buttons
	CUIButton		m_btnOk;						// Ok button 
	CUIButton		m_btnCancel;					// Cancel button
	CUIButton		m_btnPrev;						// Ok button 
	CUIButton		m_btnNext;						// Cancel button
	CUIButton		m_btnRegist;					// registration button
	CUIButton		m_btnReward;					// Reward button

	int				m_dwStyle;						// window style
	vSEL_LIST		m_vSelList;						// candidate list

	INDEX			m_nPageCur;						// current page
	INDEX			m_nPageAll;						// total page
	INDEX			m_nSelLine;						// Selected line number
	
	int				m_nTextRegionHeight;			// text height range 
	int				m_nMaxLine;						// max line size

	UIRect			m_rcTitle;						// title region
	UIRect			m_rcList;						// List region

	// UV of each part
	UIRectUV		m_rtBackTop;					// UV of top background
	UIRectUV		m_rtBackMiddle1;				// UV of middle background
	UIRectUV		m_rtBackMiddle2;				// UV of middle background
	UIRectUV		m_rtBackBottom;					// UV of bottom background

	// Select List Item Color
	COLOR			m_selColor;
	//TEST POS
	int m_x,m_y;
	
public:
	CUISelectList();
	~CUISelectList();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Open
	void	CreateSelectList(int nStyle,int nLine,CTString strTitle="");
	void	ResetSelectList();

	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	
	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );	

	// Other Functions
	void SetTitle(CTString strTitle) {m_strTitle = strTitle;}
	void SetPageCur(int nPage) { m_nPageCur = nPage; }
	void SetPageAll(int nPage) { m_nPageAll = nPage; }
	void InsideNumY(UIRect rc,int btnSize,int btnGap, int* n_x, int* n_y );
	vSEL_LIST* GetList() { return &m_vSelList;}
	
	
};


#endif	

