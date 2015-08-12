// ----------------------------------------------------------------------------
//  File : UIEditBox.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIEDITBOX_H_
#define	UIEDITBOX_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIIME.h>
#include <Engine/Interface/UIWindow.h>

class CUIImage;
 
// ----------------------------------------------------------------------------
// Name : CUIEditBox
// Desc :
// ----------------------------------------------------------------------------
class ENGINE_API CUIEditBox : public CUIWindow
{
protected:
	BOOL		m_bIsPWEditBox;				// If this is password box or not
	char		*m_pInputBuffer;			// Input buffer
	char		*m_pTempBuffer;				// Temporary buffer
	char		*m_pPWBuffer;				// Buffer for password
	BOOL		m_b2ByteChar;				// If current input is 2 byte character or not
	BOOL		m_bOnComposition;			// If it is composing now or now
	int			m_nMaxCharCount;			// Maximum count of input characters
	int			m_nShowCharCount;			// Count of shown input characters
	int			m_nShowCharSize;			// for thai font wooss 051002
	int			m_nFirstShowChar;			// Index of first shown character
	int			m_nCurCharCount;			// Current count of input characters	
	int			m_nCursorIndex;				// Position of cursor
	int			m_nTextSY;					// Start position y of text
	BOOL		m_bShowCursor;				// If cursor is shown now or not
	DOUBLE		m_dElapsedTime;				// Elapsed time for cursor blinking
	DOUBLE		m_dOldPromptTime;			// Prompt time for cursor blinking
	BOOL		m_bMsgInput;				// 메세지 박스에서 입력 되는 암호

	BOOL		m_bOnlyInteger;				// 오로지 숫자만 입력 가능한 모드
	int			m_nTextColor;

	// 이기환 수정 시작 (11. 17) : IME 작업
	static		BOOL		s_bShowReadingWindow;
	static		char		s_szReadingString[34];
	static		int			s_nReadingStringLength;
	static		CCandList	s_CandList;
//	ENGINE_API static		int			s_nRefCount;

//	ENGINE_API	static CUIEditBox*	s_FocusingEditBox;
	static		int			s_nRefCount;
	static CUIEditBox*	s_FocusingEditBox;
	static CUIEditBox*	s_PrevFocus;			// 기존 시스템과 겹치기 때문에 새로운 UILib 에서 사용

	// 이기환 수정 끝 (11. 17)

	// Region of each part
	UIRect		m_rcReadWnd;				// Regin of reading window
	UIRect		m_rcCand;					// Region of candidate

	// UV of each part
	UIRectUV	m_rtReadWndL;				// UV of left reading window
	UIRectUV	m_rtReadWndM;				// UV of middle reading window
	UIRectUV	m_rtReadWndR;				// UV of right reading window
	UIRectUV	m_rtCandUL;					// UV of upper left candidate
	UIRectUV	m_rtCandUM;					// UV of upper middle background
	UIRectUV	m_rtCandUR;					// UV of upper right background
	UIRectUV	m_rtCandML;					// UV of middle left background
	UIRectUV	m_rtCandMM;					// UV of middle middle background
	UIRectUV	m_rtCandMR;					// UV of middle right background
	UIRectUV	m_rtCandLL;					// UV of lower left background
	UIRectUV	m_rtCandLM;					// UV of lower middle background
	UIRectUV	m_rtCandLR;					// UV of lower right background

	BOOL		m_InValidEditBox;		
	// [2011/11/02 : Sora] 커서 이동 불가플래그
	BOOL		m_bMoveCursor;

	CUIRectSurface* m_pBackground;
	Command*	m_pInputCallback;
	int			m_nCursorSX;
#ifdef UI_TOOL
	UIRectUV m_ReadingWindowUV;
	UIRectUV m_CandidateUV;
#endif // UI_TOOL
	UIRectUV m_uvBackGround;
	UIRect	 m_rcBackGround;

	CUIImage* m_pImgSelText;
	bool	m_bCopyAndPaste;
	int		m_nCopyStartPos;
protected:
	// Internal functions for edit box
	BOOL	Is2ByteChar( int nCharIndex, int nFirstCheck = 0 );
	void	MoveCursor( MoveDirection mdDirection );
	void	InsertIMEChar( char *pcInsert, BOOL bOnComposition = TRUE );
	inline void	IncRef()					{ s_nRefCount = 1; }
	inline void	DecRef()					{ s_nRefCount = 0; }
	inline void	AutoRef( BOOL Ref )			{ Ref ? IncRef() : DecRef();	}
public:
	CUIEditBox();
	~CUIEditBox();

	// Create & destroy
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight,
					int nMaxChar, BOOL bIsPWBox = FALSE , int color = 0xF2F2F2FF);
	void	Destroy();

	void	initialize();

	// Render
	void	Render();
	void	RenderReadingWindow();

	void	OnRender(CDrawPort* pDraw);

	// Set UV
	void	SetReadingWindowUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight );
	void	SetCandidateUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight );
#ifdef UI_TOOL
	UIRectUV getReadingWindowUV()	{ return m_ReadingWindowUV; }
	UIRectUV getCandidateUV()		{ return m_CandidateUV;		}
#endif // UI_TOOL
	void	 setBackGroundUV(UIRectUV uv) { m_uvBackGround = uv; }
	void	 setBackGroundRect(UIRect rc) { m_rcBackGround = rc; }
	UIRectUV getBackGroundUV()		{ return m_uvBackGround;	}
	UIRect	 getBackGroundRect()	{ return m_rcBackGround;	}
	
	void	 SetCursorSX(int sx)	{	m_nCursorSX = sx;	}
	int		 GetCursorSX()			{	return m_nCursorSX;	}
	// Set focus
	void	SetFocus( BOOL bVisible );

	// Show reading and candidate window
	BOOL	DoesShowReadingWindow() { return IsFocused() && ( s_bShowReadingWindow || s_CandList.bShowWindow ); }

	// Set count of max characters
	void	SetMaxChar( int nMaxChar );
	int		GetMaxChar() { return m_nMaxCharCount; }

	// String
	void	InsertChar( int nInsertPos, char cInsert );
	void	InsertChars( int nInsertPos, char *pcInsert );
	void	DeleteChar( int nDeletePos );
	void	DeleteChars( int nDeletePos, int nCharCount );
	void	ResetString();
	void	SetString( char *pcString );
	char	*GetString() const { return m_pInputBuffer; }
	void	ClearInputBuffer();

	void	SetPassWord(BOOL bPassWord, BOOL bMsgInput = FALSE )
	{
		m_bIsPWEditBox = bPassWord;
		m_bMsgInput = bMsgInput;

		if (m_pPWBuffer == NULL)
		{
			if (m_nShowCharCount > 0)
				m_pPWBuffer = new char[m_nShowCharCount + 1];
		}
	}
	void	GetPassWord( BOOL& bPassWord, BOOL& bMsgInput )
	{
		bPassWord = m_bIsPWEditBox;
		bMsgInput = m_bMsgInput;
	}

	// Messages
	WMSG_RESULT	KeyMessage( MSG *pMsg );
	WMSG_RESULT CharMessage( MSG *pMsg );
	WMSG_RESULT	IMEMessage( MSG *pMsg );
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// 이기환 수정 시작 (11. 15) : IME 작업 
	WMSG_RESULT	IMEMessageKOR( MSG *pMsg ); // 한국어
	WMSG_RESULT	IMEMessageCHT( MSG *pMsg ); // 대만어
	WMSG_RESULT	IMEMessageCHS( MSG *pMsg );	// 중국어(PRC)
	WMSG_RESULT	IMEMessageJPN( MSG *pMsg ); // 일본어
	WMSG_RESULT	IMEMessageTHAI( MSG *pMsg ); // 태국어
	WMSG_RESULT IMEMessageUSA( MSG *pMsg ); // 미국
	WMSG_RESULT IMEMessageBRZ( MSG *pMsg ); // 브라질

	// IME
	WMSG_RESULT GetCand();
	void StopComposition ();	// 이기환 추가 (11.18) : Password 와 입력 포커스가 없을 시 Composition 중지 
	
	// Cursor
	void SetCursorIndex ( int nIndex );
	int GetCursorIndex () const {	return m_nCursorIndex;	}

	// Etc 
	int Get2ByteCharCount ( char* szBuffer, int nPos ); // Date : 2005-03-09,   By Lee Ki-hwan
	
	void ResetShowCount();

	void SetInValidEditBox(BOOL bValid) { m_InValidEditBox = bValid; }
	BOOL GetInValidEditBox() { return m_InValidEditBox; }

	inline const BOOL IsFocusedInAll() { return s_nRefCount; }
	// [2011/11/02 : Sora] 커서 이동 불가플래그
	void SetCursorMove( BOOL bMove ) { m_bMoveCursor = bMove; }
	BOOL GetCursorMove() { return m_bMoveCursor; }


	void SetOnlyIntegerMode(BOOL mode);
	BOOL GetOnlyIntegerMode();

	void SetTextColor(int color);
	int  GetTextColor();

	static const BOOL IsFocusingEditBox(void) {
		if (s_FocusingEditBox && s_FocusingEditBox->IsFocused())
			return TRUE;
		
		return FALSE;
	}

	void setBackGround( UIRect rect, UIRectUV uv );

	void setInputCallback(Command* pCmd)	{ m_pInputCallback = pCmd; }

	void SetRectSurface(CUIRectSurface* pRS)	{ m_pBackground = pRS;	}
	CUIRectSurface* GetRectSurface()			{ return m_pBackground;	}

	CUIBase* Clone();
	WMSG_RESULT OnCharMessage(MSG* pMsg);
	WMSG_RESULT OnKeyMessage(MSG* pMsg);
	WMSG_RESULT OnIMEMessage(MSG* pMsg);

	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);

	WMSG_RESULT OnCloseProc();

	bool Paste(HWND hWnd);
	bool Copy(HWND hWnd);
	void SetCopyStartPos();
	void SetCopyPos();
	void DelChars();
	void ResetSelArea();
};

#endif	// UIEDITBOX_H_

