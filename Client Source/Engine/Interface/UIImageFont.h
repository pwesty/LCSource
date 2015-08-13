
#ifndef		UI_IMAGE_FONT_H_
#define		UI_IMAGE_FONT_H_

class CUIBase;

class ENGINE_API CUIImageFont : public CUIBase
{
public:
	CUIImageFont();
	~CUIImageFont();

	CUIBase* Clone();

	//-------------------------------------------------------------------
	void	setString(const char* str)				{ m_strText = str; }

	void	setOrigString(const char* strOrig)		{ m_strSource = strOrig; }
	void	setFontSizeGap(int sx, int sy, int gap)	{ m_nFontSX = sx; m_nFontSY = sy; m_nFontGap = gap; }
	void	setSourceImage(UIRectUV uv);
	void	setOutputGap(int gap)					{ m_nOutGap = gap; }

	CUIRectSurface*	GetRectSurface()			{	return m_prsSource; }
	void	SetRectSurface(CUIRectSurface* pRS)	{ m_prsSource = pRS;	}

	
	void	OnRender(CDrawPort* pDraw);

#ifdef UI_TOOL
	void	getFontSizeGap(int& sx, int& sy, int& gap)	{ sx = m_nFontSX; sy = m_nFontSY; gap = m_nFontGap; }
	int		getOutputGap()								{ return m_nOutGap;				}
	const char* getOrigString()							{ return m_strSource.c_str();	}
	const char* getString()								{ return m_strText.c_str();		}
	UIRectUV	getSourceImageUV()						{ return m_uvSource;			}

#endif // UI_TOOL
private:
	void	calcSource();

	std::string		m_strSource;
	std::string		m_strText;
	UIRectUV		m_uvSource;	
	CUIRectSurface*	m_prsSource;
	int				m_nFontSX;
	int				m_nFontSY;
	int				m_nFontGap;
	int				m_nOutGap;
};

#endif		// UI_IMAGE_FONT_H_