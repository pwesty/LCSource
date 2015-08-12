// ----------------------------------------------------------------------------
//  File : UICREATECHAR.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	UICREATECHAR_H_
#define	UICREATECHAR_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// Define length of description
// 3월 버그 수정 [2/28/2013 Ranma]
#define	MAX_CHARINFO_CHAR_WIDTH			220	// Date : 2005-03-24,   By Lee Ki-hwan
#define	MAX_CHARINFO_LINE				13


// Define text position
#define	CREATECHAR_TITLE_OFFSETX		25
#define	CREATECHAR_TITLE_OFFSETY		5
#define	CREATECHAR_DESC_SX				19
#define	CREATECHAR_DESC_SY				35
#define	CREATECHAR_NAME_RX				72
#define	CREATECHAR_NAME_SY				229
#define	CREATECHAR_JOB_SY				264
#define	CREATECHAR_FACE_SY				283
#define	CREATECHAR_HAIR_SY				303
#define	CREATECHAR_HEAD_SY				323
#define	CREATECHAR_JOB_MAIN_CY			131


// Define size
#define	CREATECHAR_INFO_WIDTH			255
#define	CREATECHAR_INFO_HEIGHT			386
#define	CREATECHAR_CONTROL_WIDTH		153
#define	CREATECHAR_CONTROL_HEIGHT		83

class CWorld;


// ----------------------------------------------------------------------------
// Name : CUICreateChar
// Desc :
// -----------------------------------------------------------------------------
class CUICreateChar : public CUIWindow
{
public:
	CUICreateChar();
	~CUICreateChar();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Set Background World
	void	SetBackgroundWorld( CWorld *pWorld );

	// Render
	void	Render();	

	// Reset
	void	Reset();

	// Set focus
	void	SetFocus( BOOL bVisible )
	{
		CUIWindow::SetFocus( bVisible );
		if( !bVisible )
			m_ebCharName.SetFocus( FALSE );
	}

	// Edit box focus
	BOOL	IsEditBoxFocused() { return m_ebCharName.IsFocused(); }
	void	KillFocusEditBox() { m_ebCharName.SetFocus( FALSE ); }

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	WMSG_RESULT	KeyMessage( MSG *pMsg );
	WMSG_RESULT	IMEMessage( MSG *pMsg );
	WMSG_RESULT	CharMessage( MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );

	void	Lock(BOOL bLock);
	void	CharWearing(void);
	ENGINE_API void	SetCharaterAngle(FLOAT f_inL, FLOAT f_inR);
	
	BOOL	CheckCharacterHK(const char* chHK); // 홍콩 특수문자 체크

protected:
	void	PressOKBtn();
	void	ChangeSelJob();			// 다른 직업으로 바뀐경우.
	void	CreateCharacter();		// 캐릭터 생성

	// Information
	void	AddCharInfoString( CTString &strCharInfo, COLOR colCharInfo );
	void	GetCharInfo();

private:
	// texture container.
	CTextureData*		m_ptdMsgTexture;
	// Controls
	CUIEditBox			m_ebCharName;							// Input Box for Character Name
	CUIButton			m_btnZoomIn;							// Button for Zoom-In
	CUIButton			m_btnZoomOut;							// Button for Zoom-Out
	CUIButton			m_btnRotateToLeft;						// Button for Rotate To Left
	CUIButton			m_btnRotateToRight;						// Button for Rotate To Left
	CUIButton			m_btnOK;								// OK button(Create Character)
	CUIButton			m_btnCancel;							// Cancel button(Back)
	CUIButton			m_btnJobPrev;							// Prev Character (Job)
	CUIButton			m_btnJobNext;							// Next Character (Job)
	CUIButton			m_btnHeadPrev;							// Prev Character (Head)
	CUIButton			m_btnHeadNext;							// Next Character (Head)
	CUIButton			m_btnHairPrev;							// Prev Character (Hair)
	CUIButton			m_btnHairNext;							// Next Character (Hair)
	CUIButton			m_btnFacePrev;							// Prev Character (Face)
	CUIButton			m_btnFaceNext;							// Next Character (Face)
	CUIButton			m_btnRandom;							// Random Button(추후에 사용됨)

	// Description
	int					m_nCurInfoLines;						// Count of current Char information lines
	CTString			m_strCharInfo[MAX_CHARINFO_LINE];		// Char information string
	COLOR				m_colCharInfo[MAX_CHARINFO_LINE];		// Color of item information string

	CTString			m_astrDescription[18];	//2013/01/11 jeil EX메이지 추가 수정 원본값 16 -> 수정값 18

	// Region of each part
	UIRect				m_rcCharInfo;							// Region of character information
	UIRect				m_rcCharControl;						// Region of character control
	// night shadow cards. [11/6/2009 rumist]
	UIRect				m_rcMessageInfo;

	// UV of each part
	UIRectUV			m_rtCharInfoTop;						// UV of Char information region
	UIRectUV			m_rtCharInfoBottom;						// UV of Char information region
	// UV of message part. [11/6/2009 rumist]
	CUIDrawBox			m_bxNoticeMsg;
	BOOL				m_bIsShowMessageInfo;
	CUIMultiLineText	m_msgNCText;

	//
	CTString			m_strCharName;							// Character Name
	CWorld*				m_pWorld;								// Background World
	int					m_iSelectedJob;							// Selected Job
	SBYTE				m_sbSelectedFace;						// Selected Face
	SBYTE				m_sbSelectedHair;						// Selected Hair
	int					m_iSelectedColor;						// Selected Color
	BOOL				m_bAutoRotate;							// Auto Rotate
	FLOAT				m_fZDelta;								// 카메라 Z값의 델타값...
	FLOAT				m_fRotDelta;							// 캐릭터 회전 델타값...

	//CEntity*			m_pCurChar;				// 현재 선택된 캐릭터의 엔티티.
	//CPlacement3D		m_plChar;				// 캐릭터의 위치
};

#endif // UISELCHAR_H_
