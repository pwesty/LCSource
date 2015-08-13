// ----------------------------------------------------------------------------
//  File : UISecurity.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	UISECURITY_H_
#define	UISECURITY_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// #include <Engine/Interface/UIButton.h>
// #include <Engine/Interface/UIEditBox.h>
// #include <Engine/Interface/UIListBox.h>

// Define text position
#define	SECURITY_TITLE_TEXT_OFFSETX		25
#define	SECURITY_TITLE_TEXT_OFFSETY		5

// Define size of skill learn
#define	SECURITY_CHANGEPW_WIDTH			236
#define	SECURITY_CHANGEPW_HEIGHT		214


// ----------------------------------------------------------------------------
// Name : CUISecurity
// Desc :
// ----------------------------------------------------------------------------
class CUISecurity : public CUIWindow
{
public:
	enum eSelection
	{
		SET_PASSWORD,	// 암호설정
		UNSET_PASSWORD,	// 암호분실
	};

protected:

	// Controls
	CUIButton				m_btnClose;						// Close button
	CUIButton				m_btnOK;						// Learn button
	CUIButton				m_btnCancel;					// Cancel button
	
	CUIEditBox				m_ebPersonalNumber;				// 암호 확인
	CUIEditBox				m_ebConfirmNumber;				// 암호 확인			

	// Region of each part
	UIRect					m_rcTitle;						// Region of title bar

	CUIListBox				m_lbSecurityDesc;				// List box of guild description

	// UV of each part
	UIRectUV				m_rtBackTop;					// UV of top background
	UIRectUV				m_rtBackMiddle1;				// UV of middle background
	UIRectUV				m_rtBackMiddle2;				// UV of middle background
	UIRectUV				m_rtBackBottom;					// UV of bottom background
	UIRectUV				m_rtInputBoxL;					// UV of left region of input box
	UIRectUV				m_rtInputBoxM;					// UV of middle region of input box
	UIRectUV				m_rtInputBoxR;					// UV of right region of input box
	UIRectUV				m_rtConfirmBoxL;					// UV of left region of input box
	UIRectUV				m_rtConfirmBoxM;					// UV of middle region of input box
	UIRectUV				m_rtConfirmBoxR;					// UV of right region of input box
	
	CTString				m_strOldPassWord;
	CTString				m_strNewPassWord;
	CTString				m_strConfirmPassWord;
	CTString				m_strPersonalNumber;
	CTString				m_strConfirmPersonal;			// 051123 for taiwan2 

	BOOL					m_bHasPassWord;

protected:
	void	PressOKBtn();

	// Description
	void	GetSecurityDesc( BOOL bShow = TRUE );
	void	AddSecurityDescString( CTString &strDesc, COLOR colDesc = 0xF2F2F2FF );

	void	MsgBoxInputNewPW();

public:
	CUISecurity();
	~CUISecurity();

	void SetFocus( BOOL bVisible );
	
	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Open security
	void	OpenSecurity( BOOL bHasPassWord );				// 보안 접근
	void	OpenChangePassWord( BOOL bHasPassWord );		// 암호 변경
	void	ResetSecurity();

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	WMSG_RESULT	KeyMessage( MSG *pMsg );
	WMSG_RESULT	IMEMessage( MSG *pMsg );
	WMSG_RESULT	CharMessage( MSG *pMsg );

	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );

	void	MsgBoxLCommand( int nCommandCode, int nResult );

	BOOL	IsEditBoxFocused() { return m_ebPersonalNumber.IsFocused(); }
	void	KillFocusEditBox() { m_ebPersonalNumber.SetFocus( FALSE ); }

	void	ShowMsgBoxDelPW(UINT8 result);
};


#endif	// UISECURITY_H_

