// ----------------------------------------------------------------------------
//  File : UISelectWord.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	UISELECTWORD_H_
#define	UISELECTWORD_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


// #include <Engine/Interface/UIButton.h>
// #include <Engine/Interface/UICheckButton.h>
// #include <Engine/Interface/UIEditBox.h>

// Define text position
#define	SELECTREWORD_TITLE_TEXT_OFFSETX		25
#define	SELECTREWORD_TITLE_TEXT_OFFSETY		5

// Define size of skill learn
#define	SELECTREWORD_WIDTH		171
#define	SELECTREWORD_HEIGHT		214

typedef std::vector<CUIButton>	UIButton_vector;

// ----------------------------------------------------------------------------
// Name : CUISelectWord
// Desc :
// ----------------------------------------------------------------------------
class CUISelectWord : public CUIWindow
{
public:

protected:
	CUIButton				m_btnClose;						// Close button

	// Region of each part
	UIRect					m_rcTitle;						// Region of title bar

	// UV of each part
	UIRectUV				m_rtBackTop;					// UV of top background
	UIRectUV				m_rtBackMiddle1;				// UV of middle background
	UIRectUV				m_rtBackMiddle2;				// UV of middle background
	UIRectUV				m_rtBackBottom;					// UV of bottom background	

	UIButton_vector			m_vectorWordList;

	CTextureData			*m_ptdNakoTitle;			// Texture of window

public:
	CUISelectWord();
	~CUISelectWord();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Open
	void	OpenSelectWord();
	void	ResetSelectWord();

	void	ResultProcess( LONG lError );
	
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	
	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );	
	
};


#endif	// UISELECTRESOURCE_H_

