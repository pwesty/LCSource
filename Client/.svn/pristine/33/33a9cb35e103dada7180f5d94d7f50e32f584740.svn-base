#ifndef _UI_TEST_H_
#define _UI_TEST_H_
#pragma once

#define	MAX_MSGSTRING					20
#define	MSGBOX_MESSAGE_OFFSETX			15

#include "UIWindow.h"

//class CUIImage;
class ENGINE_API CUIMultiText : public CUIWindow
{
public:
	CUIMultiText(void);
	~CUIMultiText(void);

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

public:
	void				AddString( CTString &strMessage, COLOR colMessage = 0xF2F2F2FF, TEXT_ALIGN eAlign = TEXT_LEFT );
	void				SetRenderRect( UIRect &rc );
	void				SetRenderRect( );
	inline void			SetRenderPos( int x, int y )	{ m_nRenderPosX = x; m_nRenderPosY = y; }
	inline const int	GetRenderWidth()	const		{ return m_nRenderWidth;	}
	const int			GetRenderHeight()	const;//		{ return m_msgList[m_nCurStringCount-1].nStrPosY + _pUIFontTexMgr->GetLineHeight();	}
	inline const int	GetLastLineCnt()	const		{ return m_nCurStringCount; }
	inline const int	GetLastLinePosX()	const		{ return m_nRenderPosX + m_msgList[m_nCurStringCount-1].nStrPosX; }
	inline const int	GetLastLinePosY()	const		{ return m_nRenderPosY + m_msgList[m_nCurStringCount-1].nStrPosY + 5; }
	inline const COLOR	GetLastLineColor()	const		{ return m_msgList[m_nCurStringCount-1].colMessage;	}
	void				Clear();

protected:
	TEXT_OUT_INFO			m_msgList[MAX_MSGSTRING];
	int						m_nCurStringCount;				// Current count of message strings
	int						m_nMaxRow;						// Maximum row for calculating height of message box
	int						m_nRenderPosX;
	int						m_nRenderPosY;
	int						m_nRenderWidth;
	int						m_nMaxMsgStringChar;
};
#endif //_UI_TEST_H_
