#ifndef		UI_TEXTBOX_H_
#define		UI_TEXTBOX_H_

#include "UIBase.h"
#include <Engine/Interface/UIScrollBar.h>

enum eSPLIT_TYPE
{
	SPLIT_NONE = 0,
	SPLIT_SPACE,
	SPLIT_HYPHEN,
	SPLIT_MIXED,
	SPLIT_CHAR_COUNT,
	SPLIT_ONELINE
};

class ENGINE_API CUITextBox : public CUIBase
{
public:
	CUITextBox();
	~CUITextBox();

	virtual CUIBase*	Clone();

	void	initialize();
	void	OnRender( CDrawPort* pDraw );

	void	OnUpdateScrollPos();
	void	OnUpdateBoxInfo(); // TextBox 업데이트
	
	void	SetScroll( CUIScrollBar* pScroll );
	void	SetText( CTString& str, COLOR color = 0xFFFFFFFF, BOOL bUpdateHeight = FALSE);
	void	SetSplitMode( eSPLIT_TYPE eType = SPLIT_NONE ) { m_eSplitType = eType; }
	void	SetStartXY( int nSX, int nSY ) 
	{ 
		m_nStartX = nSX;
		m_nStartY = nSY; 
	}
	void	SetMaxChar(int nCount)	{ m_nCharMaxCount = nCount;	}

	void	setBackGround( UIRect rect, UIRectUV uv );
	int		GetSplitMode()						{	return m_eSplitType;	}
	void	SetGapY( int nGapY )				{	m_nGapY = nGapY;		}
	int		GetGapY()							{	return m_nGapY;			}
	void	SetLineHeight( int nlineHeight )	{	m_nLineHeight = nlineHeight;		}
	void	SetAlignTextH(int align)			{	m_eAlignText_h = (eALIGN_H)align;	}
	void	SetAlignTextV(int align)			{	m_eAlignText_v = (eALIGN_V)align;	}
	void	SetRectSurface(CUIRectSurface* pRS)	{	m_pBackground = pRS;				}
	CUIRectSurface* GetRectSurface()			{	return m_pBackground;				}

	void	StringEllipsis(int nCnt);
	bool	IsEllipsis()	{ return m_bEllipsis; }

public:
	void		GetStartXY( int& SX, int& SY )				{ SX = m_nStartX; SY = m_nStartY;	}
	int			GetLineHeight( )							{ return m_nLineHeight;				}
	int			GetTextTotalHeight()						{ return m_nLineHeight * m_vecBoxString.size();	}
	eALIGN_H	GetAlignTextH()								{ return m_eAlignText_h;			}
	eALIGN_V	GetAlignTextV()								{ return m_eAlignText_v;			}
	const char*	GetString()									{ return m_Str.c_str();				}
	COLOR		GetTextColor()								{ return m_color;					}
	int			GetLineCount()								{ return m_vecBoxString.size();		}
	CTString	GetLineString(int nIdx)						
	{
		if (GetLineCount() > 0 && m_vecBoxString.size() > nIdx)
			return m_vecBoxString[nIdx];

		return CTString("");
	}

private:
	std::string				m_Str;
	std::vector<CTString>	m_vecBoxString;
	COLOR					m_color;
	eSPLIT_TYPE				m_eSplitType;

	int						m_nMaxLine;			// Maximum count of line
	int						m_nShowLine;
	int						m_nLineHeight;		// Height of one line
	int						m_nTextSX;			// Position x of text in drop list
	int						m_nTextSY;			// Position y of text in drop list
	int						m_nStartX; 
	int						m_nStartY;
	int						m_nGapY;
	int						m_nBoxWidth;
	eALIGN_H				m_eAlignText_h;
	eALIGN_V				m_eAlignText_v;

	CUIScrollBar*			m_pScroll;
	int						m_nScrollPos;
	BOOL					m_bUpdateHeight;
	CUIRectSurface*			m_pBackground;
	bool					m_bEllipsis;
	int						m_nCharMaxCount;

protected:
	void	_RenderString( CDrawPort* pDraw, int nX, int nY );
	void	_ResetString();
	void	_UpdateBoxSize();

	void	_StringSplit();

	void	CheckSplit(int& nCurCnt, int& nSplitCnt, int& nEnterCnt);
	void	SplitNone(int nTextCnt);
	void	SplitSpace(int nTextCnt);
	void	SplitHyphen(int nTextCnt);
	void	SplitMixed(int nTextCnt);
	void	SplitCharCount();

#	ifdef UI_TOOL
public:
	void	setStringIdx(INDEX stringIdx)			
	{ 
		m_stringIndex = stringIdx; 
	}
	
	INDEX	getStringIdx()
	{ 
		return m_stringIndex; 
	}
	void	getBackGround(UIRect& rc, UIRectUV& uv)
	{ 
		rc = m_rcBackGround; 
		uv = m_uvBackGround; 
	}
private:
	INDEX				m_stringIndex;
	UIRectUV			m_uvBackGround;
	UIRect				m_rcBackGround;
#	endif	//	UI_TOOL
};

#endif		// UI_TEXTBOX_H_