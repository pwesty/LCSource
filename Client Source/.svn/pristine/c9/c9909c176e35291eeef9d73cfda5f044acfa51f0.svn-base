// ----------------------------------------------------------------------------
//  File : UIWebBoard.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIWEBBOARD_H_
#define	UIWEBBOARD_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


// #include <Engine/Interface/UIListBox.h>
// #include <Engine/Interface/UIEditBox.h>
#include <Engine/Interface/UIComboBox.h>
// #include <Engine/Interface/UIMultiEditBox.h>
// //안태훈 수정 시작	//(5th Closed beta)(0.2)
// #include <string>
// #include <vector>
// //안태훈 수정 끝	//(5th Closed beta)(0.2)


// Section of web board
enum UIWebBoardSection
{
	UWS_HELP		= 0,
	UWS_NOTICE,
	UWS_FREEBOARD,
	UWS_GUILD,	
	UWS_TOTAL,
};

// Type of web board
enum UIWebBoardType
{
	UWT_LIST		= 0,
	UWT_READ,
	UWT_WRITE,
	UWT_REPLY,
	UWT_MODIFY,
};

//안태훈 수정 시작	//(5th Closed beta)(0.2)
// Type of modify mode
enum UIWebBoardModifyType
{
	UWM_WRITE = 0,
	UWM_REPLY,
	UWM_MODIFY,
	UWM_DELETE,
};
//안태훈 수정 끝	//(5th Closed beta)(0.2)

//안태훈 수정 시작	//(5th Closed beta)(0.2)
class CUIWebBoardDelayCommand
{
public:
	virtual BOOL	ClearOldCommand();
	virtual BOOL	ExecuteImmediate(class CUIWebBoard &rUIWebBoard) = 0;
	virtual BOOL	ExecuteDelay(class CUIWebBoard &rUIWebBoard, BOOL &bRender) = 0;
	void			ConvertStringToWebParameter(const char *szParam, std::string& strOutput);
};

class CCommandOpen : public CUIWebBoardDelayCommand
{
public:
	virtual BOOL ClearOldCommand();
	virtual BOOL ExecuteImmediate(class CUIWebBoard &rUIWebBoard);
	virtual BOOL ExecuteDelay(class CUIWebBoard &rUIWebBoard, BOOL &bRender);
};

class CCommandList : public CUIWebBoardDelayCommand
{
public:
	virtual BOOL ExecuteImmediate(class CUIWebBoard &rUIWebBoard);
	virtual BOOL ExecuteDelay(CUIWebBoard &rUIWebBoard, BOOL &bRender);
};

class CCommandView : public CUIWebBoardDelayCommand
{
public:
	virtual BOOL ExecuteImmediate(class CUIWebBoard &rUIWebBoard);
	virtual BOOL ExecuteDelay(CUIWebBoard &rUIWebBoard, BOOL &bRender);
};

class CCommandModify : public CUIWebBoardDelayCommand
{
public:
	virtual BOOL ExecuteImmediate(class CUIWebBoard &rUIWebBoard);
	virtual BOOL ExecuteDelay(CUIWebBoard &rUIWebBoard, BOOL &bRender);
};

//안태훈 수정 끝	//(5th Closed beta)(0.2)

#define	WBOARD_TAB_WIDTH					95

// Count
#define	WBOARD_LIST_SUBJECT_MAX_CHAR		40
#define	WBOARD_MAX_LIST						10
#define	WBOARD_READ_MAX_CHAR				86

// Define text position
#define	WBOARD_TITLE_OFFSETX				25
#define	WBOARD_TITLE_OFFSETY				5
#define	WBOARD_TAB_TEXT_OFFSETY				34
#define	WBOARD_LIST_TITLE_SY				57
#define	WBOARD_LIST_TITLE_NO_SX				46
#define	WBOARD_LIST_TITLE_SUBJECT_SX		187
#define	WBOARD_LIST_TITLE_NAME_SX			365
#define	WBOARD_LIST_TITLE_NOTICE_NAME_SX	416
#define	WBOARD_LIST_TITLE_DATE_SX			467
#define	WBOARD_LIST_TITLE_NOTICE_DATE_SX	512
#define	WBOARD_LIST_TITLE_HIT_SX			541
#define	WBOARD_LIST_MAIN_SY					83
#define	WBOARD_LIST_MAIN_OFFSETY			29
#define	WBOARD_LIST_MAIN_NO_CX				36
#define	WBOARD_LIST_MAIN_SUBJECT_SX			69
#define	WBOARD_LIST_MAIN_NAME_CX			368
#define	WBOARD_LIST_MAIN_NOTICE_NAME_CX		419
#define	WBOARD_LIST_MAIN_DATE_SX			442
#define	WBOARD_LIST_MAIN_NOTICE_DATE_SX		487
#define	WBOARD_LIST_MAIN_HIT_CX				537
#define	WBOARD_READ_SUBJECT_SY				67
#define	WBOARD_READ_NO_SX					35
#define	WBOARD_READ_SUBJECT_SX				96
#define	WBOARD_READ_NAME_RX					561
#define	WBOARD_READ_DATE_SY					112
#define	WBOARD_READ_DATE_SX					412
#define	WBOARD_READ_HIT_RX					564
#define	WBOARD_READ_CONTENT_SX				36
#define	WBOARD_READ_CONTENT_SY				141
#define	WBOARD_WRITE_TITLE_CX				49
#define	WBOARD_WRITE_TITLE_NAME_SY			73
#define	WBOARD_WRITE_TITLE_SUBJECT_SY		97
#define	WBOARD_WRITE_TITLE_CONTENT_SY		247

#define WBOARD_WRITE_NAME_SX				83 // 이기환 추가 (05.01.01) : 게시판 쓰기 
#define WBOARD_WRITE_SUBJECT_SX				83
#define WBOARD_WRITE_CONTENT_SX				80


// Splitter
#define	WBOARD_SPLIT_WIDTH					551
#define	WBOARD_LIST_SPLIT_SX				27
#define	WBOARD_LIST_SPLITS_SY				104 
#define	WBOARD_LIST_SPLITL_SY				366
#define	WBOARD_READ_SPLIT_SX				25
#define	WBOARD_READ_SPLITS_SY				93
#define	WBOARD_READ_SPLITL_SY				384
#define	WBOARD_WRITE_SPLIT_SX				24
#define	WBOARD_WRITE_SPLITS_SY				124
#define	WBOARD_WRITE_SPLITL_SY				384

// Buttons
#define	WBOARD_PAGE_BUTTON_SY				373

// Define size of web board
#define	WBOARD_WIDTH						600
#define	WBOARD_HEIGHT						450

// ----------------------------------------------------------------------------
// Name : CUIWebBoard
// Desc :
// ----------------------------------------------------------------------------
class CUIWebBoard : public CUIWindow
{
protected:
//안태훈 수정 시작	//(5th Closed beta)(0.2)
	class cWeb			&m_web;
	BOOL				m_bWaitResponseMode;
	BOOL				m_bVisible;
	std::vector<CUIWebBoardDelayCommand *>
						m_vectorDelayCommand;
	BOOL				m_bAuthorIsMe;						//현재 view에 보이는 글이 내글인가?
	int					m_nCurrentViewWriteIndex;			//현재 보이는 자유게시판 글의 index
	int					m_nWantPage;						//CCommandList에서 사용
	int					m_nWantWrite;						//CCommandView에서 사용
	int					m_nWantModifyMode;					//CCommandModify
//안태훈 수정 끝	//(5th Closed beta)(0.2)
	int					m_nCurSection;						// Section of web board
	int					m_nCurBoardType;					// Type of web board

	// Controls
	CUIButton			m_btnClose;							// Close button
	CUIButton			m_btnPrev;							// Prev button
	CUIButton			m_btnNext;							// Next button
	CUIButton			m_btnList;							// List button
	CUIButton			m_btnWrite;							// Write button
	CUIButton			m_btnReply;							// Reply button
	CUIButton			m_btnModify;						// Modifu button
	CUIButton			m_btnDelete;						// Delete button
	CUIButton			m_btnPage[10];						// Buttons of page
	CUIButton			m_btnSearch;						// Search button
	CUIEditBox			m_ebSearch;							// Search edit box
	CUIComboBox			m_cmbSearch;						// Combobox of search

	int					m_nValidPageBtnCount;				// Count of pages
	int					m_nCurrentPage;						// Index of current page
	int					m_nCurrentFirstPage;				// First index of current page
	int					m_nCurrentLastPage;					// Last index of current page
	int					m_nTotalPage;						// Count of total page

	// List type
	CUIListBox			m_lbListContent;					// Listbox of list content ( col : 0-No, 1-Subject, 2-Writer, 3-Date, 4-Hit )
	int					m_nListIndex[WBOARD_MAX_LIST];
	int					m_nListDepth[WBOARD_MAX_LIST];

	// Read type
	CUIListBox			m_lbReadContent;					// Listbox of reading content
	CTString			m_strReadNo;
	CTString			m_strReadSubject;
	CTString			m_strReadName;
	CTString			m_strReadDate;
	CTString			m_strReadHit;

	// Write type
	CUIEditBox			m_ebWriteSubject;					// Subject edit box in writing

	// Region of each part
	UIRect				m_rcTitle;							// Region of title
	UIRect				m_rcTab;							// Region of tab
	UIRect				m_rcSearchEditBox;					// Region of search edit box
	UIRect				m_rcListTitle;						// Region of list title
	UIRect				m_rcListContent;					// Region of list content
	UIRect				m_rcReadSubject;					// Region of read subject
	UIRect				m_rcReadContent;					// Region of read content
	UIRect				m_rcWriteWriter;					// Region of write writer
	UIRect				m_rcWriteSubject;					// Region of write subject
	UIRect				m_rcWriteContent;					// Region of write content
	UIRect				m_rcWriteWriterMain;				// Region of write writer
	UIRect				m_rcWriteSubjectMain;				// Region of write subject
	UIRect				m_rcWriteContentMain;				// Region of write content

	// UV of each part
	UIRectUV			m_rtBackUL;							// UV of upper left background
	UIRectUV			m_rtBackUM;							// UV of upper middle background
	UIRectUV			m_rtBackUR;							// UV of upper right background
	UIRectUV			m_rtBackML;							// UV of middle left background
	UIRectUV			m_rtBackMM;							// UV of middle middle background
	UIRectUV			m_rtBackMR;							// UV of middle right background
	UIRectUV			m_rtBackLL;							// UV of lower left background
	UIRectUV			m_rtBackLM;							// UV of lower middle background
	UIRectUV			m_rtBackLR;							// UV of lower right background
	UIRectUV			m_rtTab;							// UV of section tab
	UIRectUV			m_rtSplitterS;						// UV of small splitter
	UIRectUV			m_rtSplitterL;						// UV of large splitter
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
	
	// Write ...
	CUIMultiEditBox		m_mebContent;						// 쓰기 내용

protected:
	enum AuthorityLevel
	{
		AL_NONE = 0,
		AL_AUTHORISME = 1,
		AL_BBSMANAGER = 2,
	};

	// Internal functions
	void	RenderListCommon();
	void	RenderReadCommon();
	void	RenderWriteCommon();
	void	AdjustListColumnPos();

//안태훈 수정 시작	//(5th Closed beta)(0.2)
	void	DelayCommandPrepare(CUIWebBoardDelayCommand *pCommand);
	BOOL	DelayCommandExecute();
	void	SetListContent(std::string &szContent);
	void	SetViewContent(std::string &szContent);
//안태훈 수정 끝	//(5th Closed beta)(0.2)
	void	SetPageButtons( int nTotalWrite, int nCurPage );

	void	AddListSubjectString( CTString &strSubject );
	void	AddReadingString( CTString &strContent );

	void ChangeBlockWriteInfo(BOOL bBlock, AuthorityLevel authorityLevel);
public:
	CUIWebBoard();
	~CUIWebBoard();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Set focus
	void	SetFocus( BOOL bVisible )
	{
		CUIWindow::SetFocus( bVisible );
		if( !bVisible )
		{
			m_mebContent.SetFocus( FALSE );
			m_ebSearch.SetFocus( FALSE );
			m_ebWriteSubject.SetFocus( FALSE );
		}
	}

	// Edit box focus
	BOOL	IsEditBoxFocused() { return m_mebContent.IsFocused() || m_ebSearch.IsFocused() || m_ebWriteSubject.IsFocused(); }
	void	KillFocusEditBox()
	{
		m_mebContent.SetFocus( FALSE );
		m_ebSearch.SetFocus( FALSE );
		m_ebWriteSubject.SetFocus( FALSE );
	}

	// Toggle visible of web board
	void	ToggleVisible();

	// Open & close web board
	void	OpenWebBoard();
	void	CloseWebBoard();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	KeyMessage( MSG *pMsg );
	WMSG_RESULT CharMessage( MSG *pMsg );
	WMSG_RESULT	IMEMessage( MSG *pMsg );
	WMSG_RESULT	MouseMessage( MSG *pMsg );

//안태훈 수정 시작	//(5th Closed beta)(0.2)
	friend class CCommandOpen;
	friend class CCommandList;
	friend class CCommandView;
	friend class CCommandModify;
//안태훈 수정 끝	//(5th Closed beta)(0.2)
	// WSS_NEW_GUILD_SYSTEM 070719
	friend class CUIGuild;

	//내 길드가 삭제되거나, 길드를 탈퇴하거나, 길드에서 강퇴당하거나 호출됨
	void NotifyGuildChange();
	// esc closing support [5/30/2011 rumist]
	BOOL	CloseWindowByEsc()				{ ToggleVisible();	return TRUE;			}
};


#endif	// UIBUTTON_H_

