#ifndef		UI_TEXT_H_
#define		UI_TEXT_H_

#include "UIBase.h"

class ENGINE_API CUIText : public CUIBase
{
public:
	CUIText();
	~CUIText();

	virtual CUIBase* Clone();
	void	OnRender( CDrawPort* pDraw );

	void	SetText( CTString& str, CTString strEllipsis = "");
	void	setFontColor( COLOR col )	{ m_color = col; }
	void	setFontShadow( COLOR colShadow, BOOL bOn ) {
		m_colShadow = colShadow;	m_bShadow = bOn; }
	BOOL	getEdge()					{ return m_bEdge;	}
	void	setEdge(BOOL edge)			{ m_bEdge = edge;	}
	void	setDepth(FLOAT fZ)			{ m_fZ = fZ; }

	void	setAlignTextH(eALIGN_H align)	{ m_eAlignText_h = align; }
	void	setBackGround( UIRect rect, UIRectUV uv );

	void	setBackGround(CUIRectSurface* pSurface)	{ m_pBackground = pSurface; }

	WMSG_RESULT OnLButtonDown( UINT16 x, UINT16 y );
	WMSG_RESULT OnLButtonUp( UINT16 x, UINT16 y );

	const char*	getText() { return m_strOrigin.str_String; }
	const char* getEllipsisText()	{ return m_strEllipsis.str_String;	}

	void	SetPointColor(char* str, COLOR col);
#ifdef UI_TOOL
	FLOAT		getDepth() { return m_fZ; }
	COLOR		getFontColor() { return m_color; }
	COLOR		getFontShadow() { return m_colShadow; }
	bool		getShadow()	{ return m_bShadow ? true : false; }

	void		setStringIdx(INDEX stringIdx) { m_stringIndex = stringIdx; }
	INDEX		getStringIdx()	{ return m_stringIndex; }
	eALIGN_H	getAlignTextH()		{ return m_eAlignText_h;	}
	void		getBackGround(UIRect& rc, UIRectUV& uv)	{ rc = m_rcBackGround; uv = m_uvBackGround; }

private:
	INDEX		m_stringIndex;
#endif // UI_TOOL
protected:
	CTString	m_strOrigin;
	CTString	m_strView;
	CTString	m_strEllipsis;
	COLOR		m_color;
	BOOL		m_bShadow;
	BOOL		m_bEdge;
	FLOAT		m_fZ;
	COLOR		m_colShadow;
	eALIGN_H	m_eAlignText_h;
	CUIRectSurface* m_pBackground;
	UIRectUV	m_uvBackGround;
	UIRect		m_rcBackGround;

	struct stCont
	{
		std::string str;
		COLOR		col;
	};

	typedef std::vector<stCont> vec_cont;
	vec_cont	m_vecData;
};



#endif		// UI_TEXT_H_