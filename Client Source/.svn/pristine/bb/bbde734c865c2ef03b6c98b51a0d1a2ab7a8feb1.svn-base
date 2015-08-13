// ----------------------------------------------------------------------------
//  File : UIHelp.h
//  Desc : Created by Dongmin
// ----------------------------------------------------------------------------

#ifndef	UIHELP_H_
#define	UIHELP_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// #include <Engine/Interface/UIListBox.h>
// #include <Engine/Interface/UIEditBox.h>
// #include <Engine/Interface/UIComboBox.h>
// #include <Engine/Interface/UIMultiEditBox.h>
// #include <string>
// #include <vector>

// Count
#define	HELP_LIST_SUBJECT_MAX_CHAR		25
#define	HELP_READ_MAX_CHAR				62
#define	HELP3_READ_MAX_CHAR				34

// Define text position
#define	HELP_TITLE_OFFSETX				25
#define	HELP_TITLE_OFFSETY				5

// Define size of web board
#define	HELP_WIDTH						600
#define	HELP_HEIGHT						450

// [KH_07043] 3차 도움말 관련 추가
#define	HELP3_WIDTH						250
#define	HELP3_HEIGHT					370

#define HELP_TUTORIAL_MSG_CNT			7

// ----------------------------------------------------------------------------
// Name : CUIHelp
// Desc : 
// ----------------------------------------------------------------------------
class CUIHelp : public CUIWindow
{
protected:	
	CTextureData		*m_ptdImageTexture;			// Texture of help default Image
	CTextureData		*m_ptdImageTemp;			// Texture of help other Image
	
	int					m_iImageIdx;						// view Image Index
	int					m_bVisible;							// Right View 
	int					m_bVisibleImg;						// Right View Image
	int					m_bSlideImgX;						// for sliding image

	// tutorial string array [8/3/2010 rumist]
	int					m_arrTutorialMessage[HELP_TUTORIAL_MSG_CNT];
	
	// Controls
	CUIButton			m_btnClose;							// Close button
	
	// List 
	CUIListBox			m_lbListContent;					// Listbox of list content 
	CUIListBox			m_lbReadContent;					// Listbox of reading content
	
	// Region of each part
	UIRect				m_rcTitle;							// Region of title	
	UIRect				m_rcRightView;						// Region of list title
	
	// UV of each part
	UIRectUV			m_rtTitleUL;						// UV of upper left background
	UIRectUV			m_rtTitleUM;						// UV of upper middle background
	UIRectUV			m_rtTitleUR;						// UV of upper right background

	UIRectUV			m_rtBackUL;							// UV of upper left background
	UIRectUV			m_rtBackUM;							// UV of upper middle background
	UIRectUV			m_rtBackUR;							// UV of upper right background
	UIRectUV			m_rtBackML;							// UV of middle left background
	UIRectUV			m_rtBackMM;							// UV of middle middle background
	UIRectUV			m_rtBackMR;							// UV of middle right background
	UIRectUV			m_rtBackLL;							// UV of lower left background
	UIRectUV			m_rtBackLM;							// UV of lower middle background
	UIRectUV			m_rtBackLR;							// UV of lower right background
	
	UIRectUV			m_rtBackSub;						// UV of sub background	

	UIRectUV			m_rtEditBoxL;						// UV of left region of edit box
	UIRectUV			m_rtEditBoxM;						// UV of middle region of edit box
	UIRectUV			m_rtEditBoxR;						// UV of right region of edit box
	UIRectUV			m_rtEditBoxUL;						// UV of upper left region of edit box
	UIRectUV			m_rtEditBoxUM;						// UV of upper middle region of edit box
	UIRectUV			m_rtEditBoxUR;						// UV of upper right region of edit box
	UIRectUV			m_rtEditBoxML;						// UV of middle left region of edit box
	UIRectUV			m_rtEditBoxMM;						// UV of middle middle region of edit box
	UIRectUV			m_rtEditBoxMR;						// UV of middle right region of edit box
	UIRectUV			m_rtEditBoxLL;						// UV of lower left region of edit box
	UIRectUV			m_rtEditBoxLM;						// UV of lower middle region of edit box
	UIRectUV			m_rtEditBoxLR;						// UV of lower right region of edit box

	BOOL				m_bBeginnerMode;					// Tutorial renewal.
protected:
	void	AddListSubjectString( CTString &strSubject );
	void	AddReadingString( CUIListBox& lbList, CTString &strContent, COLOR colContent, int iMaxChar = HELP_READ_MAX_CHAR );

	
public:
	CUIHelp();
	~CUIHelp();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();
	void	RenderList();
	void	RenderContent();
	void	RenderImage(int iListIndex);

	// Set content
	CStaticArray<int> m_ilistIndex;
	int		SetListArrange(int iStart);
	void	SetListContent( int iIndex );
	void	SetViewContent( int iIndex );

	// Toggle visible of web board
	void	ToggleVisible();

	// Open & close web board
	void	OpenHelp();
	void	CloseHelp();

	void	OpenTutorial();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	KeyMessage( MSG *pMsg );
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	
	// [090909 sora] 튜토리얼 메시지 출력
	void	CreateTutorialMessageBoxL(int nCommandCode);
	void	MsgBoxLCommand( int nCommandCode, int nResult);

	void	ReqBeginnerTitle();
	void	RepBeginnerTitle( CNetworkMessage* istr );
	void	ReqActiveTitle( INDEX index );

	const BOOL	IsBeginner()	const				{ return m_bBeginnerMode;								}
	BOOL		SetBeginner( BOOL	bBeginner )		{ m_bBeginnerMode = bBeginner; return m_bBeginnerMode;	}
	// esc closing support [5/30/2011 rumist]
	BOOL		CloseWindowByEsc()					{ ToggleVisible();	return TRUE;						}
};

class CUIHelp3 : public CUIWindow
{
	CTextureData		*m_ptdImageTexture;			// Texture of help default Image

	// Controls
	CUIButton			m_btnTClose;					// Top Close button
	CUIButton			m_btnBClose;					// Buttom Close button

	// List 
	CUIListBox			m_lbContent;					// Listbox of reading content

	// Region of each part
	UIRect				m_rcTitle;						// Region of title	

	// UV of each part
	UIRectUV			m_rtTitleUL;						// UV of upper left background
	UIRectUV			m_rtTitleUM;						// UV of upper middle background
	UIRectUV			m_rtTitleUR;						// UV of upper right background

	UIRectUV			m_rtBackUL;							// UV of upper left background
	UIRectUV			m_rtBackUM;							// UV of upper middle background
	UIRectUV			m_rtBackUR;							// UV of upper right background
	UIRectUV			m_rtBackML;							// UV of middle left background
	UIRectUV			m_rtBackMM;							// UV of middle middle background
	UIRectUV			m_rtBackMR;							// UV of middle right background
	UIRectUV			m_rtBackLL;							// UV of lower left background
	UIRectUV			m_rtBackLM;							// UV of lower middle background
	UIRectUV			m_rtBackLR;							// UV of lower right background
	
	UIRectUV			m_rtBackSub;						// UV of sub background	

public:
	CUIWindow			*m_pMammyWnd;						// [KH_070426] 엄마 윈도우 ㅡ,.ㅡㅋ

	CUIHelp3();
	~CUIHelp3();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	void	AddHelpString( CTString &strContent, COLOR colContent = 0xFFE7ADFF, int iMaxChar = HELP3_READ_MAX_CHAR );
	void	ClearHelpString();

	// Render
	void	Render();

	// Open & close web board
	void	OpenHelp(CUIWindow *pMammyWnd);
	void	CloseHelp();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	
	CUIListBox* GetHelpString() {return &m_lbContent;}

	// Messages
	WMSG_RESULT	KeyMessage( MSG *pMsg );
	WMSG_RESULT	MouseMessage( MSG *pMsg );
};


#endif	// UIBUTTON_H_

