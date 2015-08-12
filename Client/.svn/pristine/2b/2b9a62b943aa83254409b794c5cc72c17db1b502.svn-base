// ----------------------------------------------------------------------------
//  File : UIMessageBox.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIMESSAGEBOX_H_
#define	UIMESSAGEBOX_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UIEditBox.h>
#include <Engine/Interface/UIComboBox.h>
#include <Engine/Interface/UIListBox.h>
#include <Engine/Interface/UIDrawFigure.h>
#include <Engine/Interface/UISpinButton.h>
#include <Engine/Interface/UIWindowDeclare.h>
#include <Engine/Interface/UIIcon.h>
#include <list>										// to add dynamic button list  


// Define message box style
#define	UMBS_OK							0x00000001
#define	UMBS_CANCEL						0x00000002
#define	UMBS_YES						0x00000004
#define	UMBS_NO							0x00000008
#define	UMBS_OKCANCEL					0x00000010
#define	UMBS_YESNO						0x00000020
#define	UMBS_INPUTBOX					0x00000040
#define UMBS_INPUTPASSWORD				0x00000080
#define UMBS_CONFIRMPASSWORD			0x00001000	// 패스워드 확인 
#define	UMBS_ONEBTN_MASK				0x0000010F
#define	UMBS_OKBTN_MASK					0x00000535
#define	UMBS_CANCELBTN_MASK				0x0000063A
#define UMBS_INPUT_MASK					0x000000C0

#define UMBS_USER_1						0x00000100			
#define UMBS_USER_2						0x00000200	
#define UMBS_USER_12					0x00000400								

#define	UMBS_ALIGN_CENTER				0x00000000	// 버튼 정렬 (기본)
#define	UMBS_ALIGN_LEFT					0x10000000
#define	UMBS_ALIGN_RIGHT				0x20000000
//wooss 050804								
#define UMBS_SELECTBOX					0x00000800
#define UMBS_COMBOBOX					0x00002000
//WSS_GUILD_MASTER 070411 ----------------------->>
#define UMBS_LISTBOX					0X00010000
// ----------------------------------------------<<

// Date : 2006-06-01(오후 4:22:25), By eons
#define UMBS_SECOND_INPUTBOX			0x00004000	// Second edit box
#define UMBS_BUTTONEX					0x00008000	// buttonex slot( 사용시 SetBtnType()함수를 꼭 사용 )

// [071122: Su-won]
#define UMBS_SPINBUTTON					0x00020000

// [sora] 시간 지정
#define UMBS_USE_TIMER					0x00040000	// 지정된 시간후 자동으로 지정된 명령 실행

// Define max line of strings
#define	MAX_MSGSTRING					20


// Define text position
#define	MSGBOX_TITLE_TEXT_OFFSETX		25
#define	MSGBOX_TITLE_TEXT_OFFSETY		5
#define	MSGBOX_MESSAGE_OFFSETX			15
#define	MSGBOX_MESSAGE_OFFSETY			33

// Input box
#define	MSGBOX_INPUT_DEFAULT_POSX		52
#define	MSGBOX_INPUT_WIDTH				111


// Define size of message box
#define MSGBOX_WIDTH					216
#define	MSGBOX_HEIGHT					107

//WSS_GUILD_MASTER 070411 ----------------------->>
typedef struct _LIST_BOX_INFO
{
	int pos_x;
	int pos_y;
	int width;
	int height;
	int columnCnt;
	int lineCab;
	
} LIST_BOX_INFO;
// ----------------------------------------------<<

typedef struct COLOR_BOX_INFO
{
	WRect		m_rcColorBox;
	int			m_nColorBoxType;
	double		m_dStartTime;
	double		m_dDurationTime;
} COLOR_BOX_INFO;

class CUIColorBox
{
private:
	CUIDrawBox		m_dbColorBox[9];
	
public:
	CUIColorBox();
	~CUIColorBox();

	COLOR_BOX_INFO*	m_ColorBoxInfo;
	int				m_nColorBoxCount;

	void Create(int iCount, CTextureData *ptdBaseTexture);
	void Start(int index, double dDurationTime);
	void Render(int PosX, int PosY, int nCommandCode);
	BOOL IsComplete(int index);
	void Reset();

	void MsgBoxCommand(int index, int m_nCommandCode);
};

// ----------------------------------------------------------------------------
// Name : CUIMsgBox_Info
// Desc :
// ----------------------------------------------------------------------------
class ENGINE_API CUIMsgBox_Info
{
public:
	DWORD			m_dwStyle;						// Message box style
	int				m_nWhichUI;						// Which UI does create this message box
	int				m_nCommandCode;					// Command code for returning to UI

	CTString		m_strTitle;						// Title of message box
	CTString		m_strMessage[MAX_MSGSTRING];	// Message strings of message box
	COLOR			m_colMessage[MAX_MSGSTRING];	// Colors of message string
	int				m_nPosX[MAX_MSGSTRING];			// Position x of string
	int				m_nPosY[MAX_MSGSTRING];			// Position y of string
	TEXT_ALIGN		m_taAlign[MAX_MSGSTRING];		// Aligns of message box
	int				m_nCurStringCount;				// Current count of message strings
	int				m_nMaxRow;						// Maximum row for calcualting heigth of message box

	int				m_nInputPosX;					// Position x of input box
	int				m_nInputPosY;					// Position y of input box
	int				m_nInputMaxChar;				// Maximum character of input box

	int				m_nSEInputPosX;					// Position x of second input box
	int				m_nSEInputPosY;					// Position y of second input box
	int				m_nSEInputMaxChar;				// maximum character of second input box

	CTString		m_strUserBtn1;
	CTString		m_strUserBtn2;
	
	int				m_nWidth;
	int				m_nHeight;
	int				m_nInputWidth;
	int				m_nSEInputWidth;

	int				m_nBtnType;
	int				m_nItemType;
	int				m_nItemSubType;
	int				m_nBtnCount;

	//WSS_GUILD_MASTER 070411 ----------------------->>
	LIST_BOX_INFO	m_listBoxPlacement;
	// ----------------------------------------------<<

	int				m_nColorBoxCount;
	WRect*			m_rcColorBox;
	int*			m_nColorBoxType;

	int				m_nSpinButtonPosX;
	int				m_nSpinButtonPosY;
	int				m_nSpinButtonWidth;
	CTString		m_strSpinButtonTitle;
	
	__int64			m_nTime;					// [sora] 대기 시간
	BOOL			m_bTimeOutBtnMessage;		// [sora] 실행 메시지
	
	FLOAT			m_fNPCPosX, m_fNPCPosZ;
public:
	CUIMsgBox_Info();
	~CUIMsgBox_Info();

	void	SetMsgBoxInfo( CTString &strTitle, DWORD dwStyle, int nWhichUI, int nCommandCode, int nWidth = -1 , FLOAT fNPCPosX = -1.0f, FLOAT fNPCPosZ = -1.0f);
	// AddString()과 AddStringEx()는 혼용해서 사용하지 말것!!
	void	AddString( CTString &strMessage, COLOR colMessage = 0xF2F2F2FF, TEXT_ALIGN eAlign = TEXT_LEFT );
	void	AddStringEx( CTString &strMessage, int nRow, int nCol,
							COLOR colMessage = 0xF2F2F2FF, TEXT_ALIGN eAlign = TEXT_LEFT );
	void	SetInputBox( int nRow, int nCol, int nMaxChar, int nInputWidth = -1 );
	void	SetSEInputBox( int nRow, int nCol, int nMaxChar, int nInputWidth = -1, int nGapX = -1 );
	void	Reset();
	void	SetUserBtnName( CTString strBtnName1, CTString strBtnName2 = "");
	void	SetMaxRow( int nRow ) { m_nMaxRow = nRow; }
	void	SetBtnType( int nBtnType, int nItemType, int nItemSubType, int nBtnCount=1 )	{
		m_nBtnType = nBtnType;
		m_nItemType = nItemType;
		m_nItemSubType = nItemSubType;
		m_nBtnCount =nBtnCount;
	}
	//WSS_GUILD_MASTER 070411 ----------------------->>
	inline void	SetListBoxPlacement(int t_x,int t_y ,int t_width, int t_height ,int t_lineCab, int t_columnCnt =1)
	{
		m_listBoxPlacement.pos_x = t_x;
		m_listBoxPlacement.pos_y = t_y;
		m_listBoxPlacement.width = t_width;
		m_listBoxPlacement.height = t_height;				
		m_listBoxPlacement.columnCnt = t_columnCnt;
		m_listBoxPlacement.lineCab = t_lineCab;
	}	
	// ----------------------------------------------<<

	void	SetSpinButton(int nRow, int nCol, int nWidth, CTString strTitle);

	void	SetMsgBoxTimer(__int64 nTime, BOOL bTimeOutBtnMessage)	// [sora] 동작 시간 설정
	{
		m_nTime = nTime;
		m_bTimeOutBtnMessage = bTimeOutBtnMessage;
	}
};

// 추가 구조체. [11/19/2009 rumist]
// 이것도 맘에 안듬. [11/19/2009 rumist]
typedef struct _TEXT_OUT_INFO
{
	CTString		strMessage;	// Message strings of message box
	COLOR			colMessage;	// Colors of message string
	int				nStrPosX;			// Position x of string
	int				nStrPosY;			// Position y of string
	TEXT_ALIGN		taAlign;		// Aligns of message box

	_TEXT_OUT_INFO() : nStrPosX(MSGBOX_MESSAGE_OFFSETX), nStrPosY(-1), taAlign( TEXT_LEFT ) { strMessage = CTString(""); }
	~_TEXT_OUT_INFO()	{}
} TEXT_OUT_INFO, *PTEXT_OUT_INFO;

class  ENGINE_API CUIMultiLineText
{
private:
	TEXT_OUT_INFO			m_msgList[MAX_MSGSTRING];
	int						m_nCurStringCount;				// Current count of message strings
	int						m_nMaxRow;						// Maximum row for calculating height of message box
	int						m_nRenderPosX;
	int						m_nRenderPosY;
	int						m_nRenderWidth;
	int						m_nMaxMsgStringChar;
public:
	CUIMultiLineText() : m_nCurStringCount(0), m_nMaxRow(0), m_nRenderPosX(0), m_nRenderPosY(0), m_nRenderWidth(0), m_nMaxMsgStringChar(0) {}
	~CUIMultiLineText()	{}
	void				AddString( CTString &strMessage, COLOR colMessage = 0xF2F2F2FF, TEXT_ALIGN eAlign = TEXT_LEFT );
	void				SetRenderRect( UIRect &rc );
	void				SetRenderRect( int x, int y, int width, int height );
	inline void			SetRenderPos( int x, int y )	{ m_nRenderPosX = x; m_nRenderPosY = y; }
	inline const int	GetRenderWidth()	const		{ return m_nRenderWidth;	}
		   const int	GetRenderHeight()	const;//		{ return m_msgList[m_nCurStringCount-1].nStrPosY + _pUIFontTexMgr->GetLineHeight();	}
	inline const int	GetLastLineCnt()	const		{ return m_nCurStringCount; }
	inline const int	GetLastLinePosX()	const		{ return m_nRenderPosX + m_msgList[m_nCurStringCount-1].nStrPosX; }
	inline const int	GetLastLinePosY()	const		{ return m_nRenderPosY + m_msgList[m_nCurStringCount-1].nStrPosY + 5; }
	inline const COLOR	GetLastLineColor()	const		{ return m_msgList[m_nCurStringCount-1].colMessage;	}
	void				Render();
	void				Clear();
};

// ----------------------------------------------------------------------------
// Name : CUIMessageBox
// Desc :
// ----------------------------------------------------------------------------
class CUIMessageBox : public CUIWindow
{
protected:
	typedef	std::vector< CUIIcon* >		vecIcon;

	int				m_nUIIndex;						// Index of message box
	DWORD			m_dwStyle;						// Message box style
	int				m_nWhichUI;						// Which UI does create this message box
	int				m_nCommandCode;					// Command code for returning to UI

	CUIButton		m_btnOk;						// Ok button ( ok or yes )
	CUIButton		m_btnCancel;					// Cancel button ( cancel or no )
	CUIEditBox		m_ebInput;						// Input box
	CUIEditBox		m_ebSEInput;					// SE Input box
	CUIEditBox		m_ebConfirm;					// confirm edit box 

	//CUIIcon*		m_pIconSlot;					// ButtonEx slot
	vecIcon			m_vecIcon;
	CUIComboBox		m_cbList;						// combo box list

	//WSS_GUILD_MASTER 070411 ----------------------->>
	CUIListBox		m_lbListBox;					// List Box
	CUIButton		m_btnClose;						// Add Close Button
	// ----------------------------------------------<<	
	
	CUIColorBox		m_cboxColorBox;

	CUISpinButton	m_sbtnSpin;


	std::list<CUIButton> m_pBtnlist;				//wooss 050803
	typedef std::list<CUIButton>::iterator iBTNLIST;//
	iBTNLIST		m_iBtnlist;						//button list iterator
	
	CTString		m_strTitle;						// Title of message box
	CTString		m_strMessage[MAX_MSGSTRING];	// Message strings of message box
	COLOR			m_colMessage[MAX_MSGSTRING];	// Colors of message string
	int				m_nStrPosX[MAX_MSGSTRING];		// Position x of string
	int				m_nStrPosY[MAX_MSGSTRING];		// Position y of string
	TEXT_ALIGN		m_taAlign[MAX_MSGSTRING];		// Aligns of message box
	int				m_nCurStringCount;				// Current count of message strings
	int				m_nTextRegionHeight;			// Height of text region
	
	BOOL			m_bTitleBarClick;				// If title bar is clicked or not

	// Region of each part
	UIRect			m_rcTitle;						// Region of title bar
	UIRect			m_rcInputBox;					// Region of input box
	UIRect			m_rcSEInputBox;					// Region of second input box
	UIRect			m_rcConfirmBox;					// Region of input box
	UIRect*			m_rcBtnSlot;					// Region of ButtonEx slot
	UIRect*			m_rcBtnInsertSlot;				// Region of ButtonEx Insert slot

	// UV of each part
	UIRectUV		m_rtBackTopL;					// UV of top background
	UIRectUV		m_rtBackTopM;					// UV of top background
	UIRectUV		m_rtBackTopR;					// UV of top background
	
	UIRectUV		m_rtBackMiddle1L;				// UV of middle background
	UIRectUV		m_rtBackMiddle1M;				// UV of middle background
	UIRectUV		m_rtBackMiddle1R;				// UV of middle background

	UIRectUV		m_rtBackMiddle2L;				// UV of middle background
	UIRectUV		m_rtBackMiddle2M;				// UV of middle background
	UIRectUV		m_rtBackMiddle2R;				// UV of middle background

	UIRectUV		m_rtBackBottomL;					// UV of bottom background
	UIRectUV		m_rtBackBottomM;					// UV of bottom background
	UIRectUV		m_rtBackBottomR;					// UV of bottom background

	UIRectUV		m_rtInputBoxL;					// UV of left region of input box
	UIRectUV		m_rtInputBoxM;					// UV of middle region of input box
	UIRectUV		m_rtInputBoxR;					// UV of right region of input box

	UIRectUV		m_rtSEInputBoxL;					// UV of left region of second input box
	UIRectUV		m_rtSEInputBoxM;					// UV of middle region of second input box
	UIRectUV		m_rtSEInputBoxR;					// UV of right region of second input box
	
	UIRectUV		m_rtBtnSlot;					// UV of ButtonEx slot

	int				m_nMaxChar;						// 한라인에 출력시킬수 있는 최대 문자수

	int				m_nBtnType;
	int				m_nItemType;
	int				m_nItemSubType;
	int				m_nBtnTab;
	int				m_nInvenIdx;
	int				m_nUniIndex;
	int				m_nBtnCount;

	FLOAT			m_fNPCPosX, m_fNPCPosZ;

	__int64			m_nStartTime;					// [sora] 시작 시간
	__int64			m_nTime;						// [sora] 대기 시간
	BOOL			m_bTimeOutBtnMessage;			// [sora] 실행 메시지

	CTextureData*	m_ptdNewCharacterInfoTex;

	// Internal functions
protected:
	void	SetBtnSlot( int nBtn );
public:
	CUIMessageBox();
	~CUIMessageBox();

	void	release();

	// Internal functions
	void	ReturnCommand( BOOL bOK = TRUE );
	void	ReturnCommonCommand(); // 커먼 메시지 박스의 커맨드 호출
	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	CreateMessageBox( CUIMsgBox_Info &rMsgBoxInfo, int nPosX, int nPosY );
	void	InitMessageBox();
	void	SetUIIndex( int nUIIndex ) { m_nUIIndex = nUIIndex; }

	// [2011/06/28 : Sora]
	const int GetUIIndex() { return m_nUIIndex; }

	// wooss 050803
	// new button insert
	void	addSelectContext(CTString );
	
	// Get data of message box
	int		GetCmdCode() const { return m_nCommandCode; }
	CUIEditBox	&GetInputBox() { return m_ebInput; }
	CUIEditBox	&GetSEInputBox() { return m_ebSEInput; }
	CUIEditBox	&GetConfirmBox() { return m_ebConfirm; }
	CUIComboBox	&GetComboBox() { return m_cbList; }
	CUIIcon*	GetBtnEx(int nBtn=0) {	return m_vecIcon[nBtn]; }
	UIRect		&GetBtnSlotRect(int nSlot=0) {	return m_rcBtnSlot[nSlot]; }
	UIRect		&GetBtnInsertSlot(int nSlot=0) { return m_rcBtnInsertSlot[nSlot]; } 

	CUIButton	&GetBtnOK() { return m_btnOk; }						
	CUIListBox	&GetListBox() { return m_lbListBox; }						

	CUIColorBox		&GetColorBox() { return m_cboxColorBox; }

	CUISpinButton	&GetSpinButton() { return m_sbtnSpin; }
	
	// ($E_WC2010) [100517: selo] 버튼 설정하기
	void SetBtnEx(int nBtn, CUIIcon* pIcon)
	{
		m_vecIcon[nBtn]->copyItem(pIcon);
	}

	// Set focus
	void	SetFocus( BOOL bVisible )
	{
		CUIWindow::SetFocus( bVisible );
		if( !bVisible )
			m_ebInput.SetFocus( FALSE );
	}

	// Edit box focus
	BOOL	IsEditBoxFocused()
	{ 
		if( m_ebInput.IsFocused() || m_ebSEInput.IsFocused() ) { return TRUE; }
		else { return FALSE; }
	}

	void	KillFocusEditBox()
	{ 
		m_ebInput.SetFocus( FALSE );
		m_ebSEInput.SetFocus( FALSE );
	}

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	KeyMessage( MSG *pMsg );
	WMSG_RESULT CharMessage( MSG *pMsg );
	WMSG_RESULT	IMEMessage( MSG *pMsg );
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	void	GetBtnUseItemInfo( int	&nTab, int &inven_idx, int &nUniIndex );
	void	SetInvenUseItemInfo( int nTab, int inven_idx );

	///////////////////////////////////////////////////////////////////////////////////
	// [070807: Su-won] EVENT_ADULT_OPEN
	void	SetString( int iIndex, CTString &strMessage, COLOR colMessage =0xF2F2F2FF)
	{
		m_strMessage[iIndex] =strMessage;
		m_colMessage[iIndex] =colMessage;
	}
	// [070807: Su-won] EVENT_ADULT_OPEN
	///////////////////////////////////////////////////////////////////////////////////	

	void SetColorBox(INDEX i, int nType, WRect rcDrawBox);
};


#endif	// UIMESSAGEBOX_H_

