#ifndef __UI_TEXTBOX_EX_H__
#define __UI_TEXTBOX_EX_H__

class CUIBase;
class CUIScrollBar;
class CUITextEx;

class ENGINE_API CUITextBoxEx : public CUIBase
{
public:
	CUITextBoxEx();
	~CUITextBoxEx();

	CUIBase* Clone();
	void AddText(std::string& str, DWORD dwCol = DEF_UI_COLOR_WHITE);
	void AddBtn(std::string& str, Command* pCmd, 
		DWORD dwColIdle = DEF_UI_COLOR_WHITE, DWORD dwColOver = DEF_UI_COLOR_BLUE,
		DWORD dwColClick = DEF_UI_COLOR_BLUE, DWORD dwColDisable = DEF_UI_COLOR_WHITE);
	void SetSplitMode(eSPLIT_TYPE eType = SPLIT_SPACE)	{ m_eSplitType = eType;	}
	eSPLIT_TYPE GetSplitMode()	{return m_eSplitType;	}
	
	void SetStartX(int x)	{ m_nStartX = x;	}
	int	 GetStartX()		{ return m_nStartX;	}

	void SetStartY(int y)	{ m_nStartY = y;	}
	int	 GetStartY()		{ return m_nStartY;	}

	void SetGapY(int gap)	{ m_nGapY = gap;	}			
	int	 GetGapY()			{ return m_nGapY;	}

	COLOR GetBaseColor()	{ return m_colBase;	}

	const char* GetText()	{ return m_strOrigin.c_str();	}

	void ClearUI();
	void Release();

	void AddUI(CUIBase* pBase = NULL);
	int GetTextExCount()	{ return m_vecUICont.size();	}
	CUIBase* GetLineText(int nIdx);

private:
	typedef std::vector<CUIBase*>		vecUICont;
	typedef std::vector<std::string>	vecValcont;
	typedef vecUICont::iterator			vec_UICont_iter;

	void split(std::string str);
	void splitNone(std::string& strAdd, vecValcont& val, std::string& strTagID, int nTextArea);
	void splitSpace(std::string& strAdd, vecValcont& val, std::string& strTagID, int nTextArea);
	void splitOneLine(std::string& strAdd, vecValcont& val, std::string& strTagID);

	void calcSplit(std::string& str, int& nCurCnt, int& nSplitCnt);
	bool checkWidth(std::string& strPrev, std::string& strAdd, int nTextArea, int& nSplitPos);
	bool checkSpace(std::string& strAdd, int nTextArea, int& nSplitCnt);
	std::string parseSyntax(std::string& src, std::string& tagID);
	void getTagID(std::string& strSyntax, std::string& strID, int &start, int &end);

	void createControl(std::string& strTagID, CUITextEx* pTextEx, vecValcont& val, std::string& str);
	bool getValue(std::string& strSyntax, std::string& strTagID, vecValcont& val, std::string& strText);
	
	void insertText(CUITextEx* pTextEx, vecValcont& val, std::string& str);	
	void insertBtn(CUITextEx* pTextEx, vecValcont& val, std::string& str);
	int				m_nStartX;

	std::string		m_strOrigin;
	vecUICont		m_vecUICont;
	COLOR			m_colBase;
	COLOR			m_colBtn[UBS_TOTAL];
	eSPLIT_TYPE		m_eSplitType;
	CUIScrollBar*	m_pScroll;

	Command*		m_pBtnCmd;
	int				m_nStartY;
	int				m_nGapY;
	
	enum eCONTROL_TYPE
	{
		eCT_TEXT = 0,
		eCT_BTN
	};

	eCONTROL_TYPE m_eControlType;
};
#endif // __UI_TEXTBOX_EX_H__