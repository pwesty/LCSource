#ifndef		__UI_TEXT_EX_H__
#define		__UI_TEXT_EX_H__

class CUIBase;

class ENGINE_API CUITextEx : public CUIBase
{
public:
	CUITextEx();
	~CUITextEx();

	CUIBase* Clone();
	void addUI(CUIBase* pBase);
	void AddText(const char* str, DWORD dwCol = DEF_UI_COLOR_WHITE, int pos_x = -1);
	void AddBtn(const char* str, Command* pCmd, 
		DWORD dwColIdle = DEF_UI_COLOR_WHITE, DWORD dwColOver = DEF_UI_COLOR_YELLOW,
		DWORD dwColClick = DEF_UI_COLOR_WHITE, DWORD dwColDisable = DEF_UI_COLOR_YELLOW);
	const char* GetOriginString()	{ return m_strOrigin.c_str();	}
	void ClearUI();

private:
	typedef std::vector<std::string>	vecValcont;
	void DeleteUICont();
private:
	typedef std::vector<CUIBase*>		vecUICont;
	typedef vecUICont::iterator			vec_UICont_iter;

	vecUICont	m_vecUICont;
	std::string m_strOrigin;
	COLOR		m_colBase;

	int			m_nNextPosX;
};

#endif // __UI_TEXT_EX_H__